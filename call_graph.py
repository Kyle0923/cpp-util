#!/usr/bin/env python3
import os
import json
import argparse
import clang.cindex
import graphviz
import utils

FUNCTION_GRAPH_DB_JSON = "function_graph_db.json"

# location = "file:line" of the declaration, cannot use definition because the def can be in other translation unit
symbol_dict = {} # "unique_id": {"name": symbol_name, "has_template_callee": bool}
call_dict = {} # "unique_id": ["callee_1_id", "callee_2_id", ...]
unique_id_dict = {} # function_name: ["unique_id1", "unique_id2"]

MAX_SYMBOL_LENGTH = 100

def register_func(node: clang.cindex.Cursor, has_template_callee: bool = False):
    global symbol_dict, unique_id_dict
    if node.kind not in [clang.cindex.CursorKind.CXX_METHOD, clang.cindex.CursorKind.FUNCTION_DECL, \
                         clang.cindex.CursorKind.FUNCTION_TEMPLATE, clang.cindex.CursorKind.CONSTRUCTOR, \
                         clang.cindex.CursorKind.CONVERSION_FUNCTION, clang.cindex.CursorKind.VAR_DECL, \
                         clang.cindex.CursorKind.TEMPLATE_NON_TYPE_PARAMETER]:
        raise ValueError(f"wrong node: {node.spelling}, {node.kind}")
    unique_id = node.get_usr()
    if unique_id in symbol_dict:
        # has_template_callee == True means further process is needed when this function is called
        symbol_dict[unique_id]["has_template_callee"] |= has_template_callee
        return unique_id

    sym_name = node.spelling
    func_template = ""
    if node.kind == clang.cindex.CursorKind.FUNCTION_TEMPLATE: # TODO: this case is obsolete
        func_template = get_template_list_from_declaration(node)
    elif node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        # TODO: what if func_ptr points to a template function
        # TODO: provide template info for other function kinds
        func_template = get_template_list_from_instantiation(node)

    parent = node.semantic_parent
    parent_name = ""
    if parent.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
        parent_name = parent.type.spelling + "::"
    elif parent.kind == clang.cindex.CursorKind.CLASS_TEMPLATE: # TODO: this case is obsolete
        parent_template = get_template_list_from_declaration(parent)
        parent_name = parent.spelling + parent_template + "::"

    return_type = ""
    if node.kind != clang.cindex.CursorKind.CONSTRUCTOR:
        if node.kind == clang.cindex.CursorKind.VAR_DECL:
            # function pointer
            return_type = replace_lambda_name(node.type.get_pointee().get_result().spelling) + ' '
            sym_name = f"(*{sym_name})"
        else:
            return_type = replace_lambda_name(node.type.get_result().spelling) + ' '

    param_str = get_param_list(node)

    full_sym_name = f"{return_type}{parent_name}{sym_name}{func_template}({param_str})"

    if len(full_sym_name) > MAX_SYMBOL_LENGTH:
        # if the name is too long, trim the name
        return_type = utils.trim_namespace(return_type, False)
        parent_name = utils.trim_namespace(parent_name, False)
        func_template = utils.trim_namespace(func_template, False)
        param_str = utils.trim_namespace(param_str, False)

        full_sym_name = f"{return_type}{parent_name}{sym_name}{func_template}({param_str})"
    if len(full_sym_name) > MAX_SYMBOL_LENGTH:
        # still too long
        if "<" in parent_name:
            # use .spelling instead of .type.spelling, .spelling doesn't contain the template part
            parent_name = utils.trim_namespace(parent.spelling) + "<>::"
        if func_template:
            func_template = "<>"
        param_str = utils.trim_namespace(get_param_list(node, type_only=True), False)
        full_sym_name = f"{return_type}{parent_name}{sym_name}{func_template}({param_str})"

    loc = utils.get_symbol_decl_loc_from_def(node)

    symbol_dict[unique_id] = {"name": full_sym_name, "has_template_callee": has_template_callee, "loc": loc}

    if full_sym_name not in unique_id_dict:
        unique_id_dict[full_sym_name] = []
    unique_id_dict[full_sym_name].append(unique_id)

    if node.kind != clang.cindex.CursorKind.FUNCTION_TEMPLATE:
        process_ast(node)

    return unique_id

# this is used to parse the declaration AST to gather the original template arguments
# only use for non-member functions
# e.g., template <typename T, int N> void func(int a) => returns <T, int>
def get_template_list_from_declaration(node: clang.cindex.Cursor):
    if node.kind not in [clang.cindex.CursorKind.CLASS_TEMPLATE, clang.cindex.CursorKind.FUNCTION_TEMPLATE]:
        raise ValueError(f"wrong node: {node.spelling}")
    template_types = []
    for child in node.get_children():
        if child.kind == clang.cindex.CursorKind.TEMPLATE_TYPE_PARAMETER:
            template_types.append(child.spelling)
        elif child.kind == clang.cindex.CursorKind.TEMPLATE_NON_TYPE_PARAMETER:
            template_types.append(child.type.spelling)

    return "<" + (", ".join(template_types)) + ">"

# this is used to gather the actual template arguments that are used for instantiation
# e.g., func<int, 1>(); => returns <int, 1>
def get_template_list_from_instantiation(node):
    if node.type.kind != clang.cindex.TypeKind.FUNCTIONPROTO:
        raise ValueError(f"wrong node: {node.spelling}")
    template_args = []

    for idx in range(0, node.get_num_template_arguments()):
        node_kind = None
        try:
            # clang will throw on variadic templates
            node_kind = node.get_template_argument_kind(idx)
        except:
            continue

        if node_kind == clang.cindex.TemplateArgumentKind.TYPE:
            type_name = node.get_template_argument_type(idx).spelling
            if is_lambda(type_name):
                template_args.append("(lambda)")
            else:
                template_args.append(node.get_template_argument_type(idx).spelling)
        elif node_kind == clang.cindex.TemplateArgumentKind.INTEGRAL:
            template_args.append( str(node.get_template_argument_value(idx)) )
        elif node_kind in [clang.cindex.TemplateArgumentKind.NULL, clang.cindex.TemplateArgumentKind.NULLPTR]:
            continue
        else:
            # ref: https://github.com/llvm-mirror/clang/blob/aa231e4be75ac4759c236b755c57876f76e3cf05/bindings/python/clang/cindex.py#L1383
            # the last kind is DECLARATION meaning the template arg is an pointer or reference to some object
            continue

    if template_args:
        return "<" + (", ".join(template_args)) + ">"
    elif node.get_num_template_arguments() > 0:
        # template args contains only NULL, NULLPTR, or DECLARATION
        return "<>"
    else:
        return ""

def get_param_list(node: clang.cindex.Cursor, type_only=False):
    params = []
    if node.kind == clang.cindex.CursorKind.FUNCTION_TEMPLATE:
        for child in node.get_children():
            if child.kind != clang.cindex.CursorKind.PARM_DECL:
                continue
            param_name = child.spelling
            param_type = ""
            for grandchild in child.get_children():
                if grandchild.kind != clang.cindex.CursorKind.TYPE_REF:
                    continue
                param_type = replace_lambda_name(grandchild.spelling)
                break
            if type_only:
                params.append(param_type.strip())
            else:
                params.append(f"{param_type} {param_name}".strip())
    elif node.kind == clang.cindex.CursorKind.VAR_DECL:
        # function pointer
        for param in node.type.get_canonical().get_pointee().argument_types():
            param_type = replace_lambda_name(param.spelling)
            params.append( param_type.strip() )
    else:
        for param in node.get_arguments():
            param_type = replace_lambda_name(param.type.spelling)
            if type_only:
                params.append(param_type.strip())
            else:
                params.append( f"{param_type} {param.spelling}".strip() )

    param_str = ", ".join(params)
    return param_str

def replace_lambda_name(typename: str):
    if is_lambda(typename):
        return trim_lambda_name(typename)
    return typename

def is_lambda(typename: str):
    return typename.startswith("(lambda at")

def trim_lambda_name(typename: str):
    # e.g., "(lambda at /file/foo.cpp:10:2) &&"
    suffix = typename.split(')')[-1]
    location = typename.split('at', 1)[1].strip()
    file, line, _ = location.split(':')
    file = os.path.basename(file)
    param_type = f"(lambda@{file}:{line}){suffix}"
    return param_type

def process_ast(cursor: clang.cindex.Cursor):
    global symbol_dict, call_dict, project_dir
    # TODO: replace walk_preorder with a self implement preorder traversal to avoid repetition
    # use a stack, if is_not_func, push children, if is_func, register(func) and do not push children
    for node in cursor.walk_preorder():
        if node.kind not in [clang.cindex.CursorKind.CXX_METHOD, clang.cindex.CursorKind.FUNCTION_DECL, clang.cindex.CursorKind.CONSTRUCTOR]:
            continue
        if not utils.is_project_defined_symbol(node, project_dir):
            continue
        caller_id = register_func(node)
        if caller_id in call_dict and symbol_dict[caller_id]["has_template_callee"] == False:
            continue
        callee = [] # using array can preserve order
        def_node = node.get_definition()

        if not def_node:
            # function's definition is in other translation unit
            continue

        for child in def_node.walk_preorder():
            if not child.kind == clang.cindex.CursorKind.CALL_EXPR:
                continue

            if child.type.kind == clang.cindex.TypeKind.DEPENDENT:
                # template function
                register_func(node, True) # set flag `has_template_callee` to True
            else:
                if not child.get_definition() and not child.referenced:
                    # functions defined in other translation unit has no .get_definition() but has .referenced
                    # some other CALL_EXPR nodes has neither, these are usually compiler generated intermediate CALL_EXPR
                    continue
                # normal function
                if child.get_definition() and child.get_definition().is_default_method():
                    # compiler-provided methods such as default ctor, copy/move ctor, ...
                    continue
                if child.referenced.kind == clang.cindex.CursorKind.CONVERSION_FUNCTION:
                    # conversion (casting) functions such as `operator int()` usually don't provide much useful information
                    # about the call flow. Also for lambdas and function pointers, the function signatures are very hard to read
                    # e.g., `int (*ptr)(void) = []() {return 0;}` will invoke a conversion function `int (*)() ::operator int (*)()()`
                    # so we ignore them
                    continue
                callee_id = register_func(child.referenced)
                if callee_id not in callee:
                    callee.append(callee_id)

        if caller_id not in call_dict:
            call_dict[caller_id] = callee
        else:
            call_dict[caller_id] += [func for func in callee if func not in call_dict[caller_id]]

    return

def parse_file(filename, index):
    if (utils.path_name_match(filename, args.excl)):
        utils.verbal(args, "skipping ", filename)
        return

    utils.verbal(args, "parsing ", filename)

    try:
        additional_options = ['-x', 'c++-header'] # treat .h as c++ header
        abs_path = os.path.abspath(filename)
        if abs_path in utils.compile_db:
            additional_options += utils.compile_db[abs_path]
        else:
            additional_options += utils.compile_default_options.keys()

        translation_unit = index.parse(filename, additional_options)
    except Exception as e:
        print(f"Error parsing file {filename}: {e}")
        return

    for diag in translation_unit.diagnostics:
        if 'file not found' in diag.spelling:
            # possible incomplete parsing
            global parse_error
            parse_error = True
            print(f"Missing include: {diag.spelling} - {diag.location.file}:{diag.location.line}")

    # utils.print_ast(translation_unit.cursor)
    # exit(0)
    process_ast(translation_unit.cursor)
    return


def generate_function_dict(dir: str):
    global symbol_dict, call_dict, unique_id_dict

    full_json_db_path = os.path.join(dir, FUNCTION_GRAPH_DB_JSON)

    if args.rebuild and os.path.isfile(full_json_db_path):
        os.remove(full_json_db_path)

    if os.path.isfile(full_json_db_path):
        with open(full_json_db_path) as fd:
            print(f"[[ {full_json_db_path} ]] exists, skip parsing source code, use --rebuild to force parsing source\n")
            db = json.load(fd)
            symbol_dict = db["symbol_dict"]
            call_dict = db["call_dict"]
            unique_id_dict = db["unique_id_dict"]
            return

    src_files = utils.parse_compile_options(dir, args)

    index = clang.cindex.Index.create()

    global parse_error
    parse_error = False

    print("starting parsing source files, this can take a while")
    if src_files:
        for src in src_files:
            parse_file(src, index)

    else:
        for root, _, files in os.walk(dir):
            for file in files:
                if not (file.endswith('.cpp') or file.endswith('.h') or file.endswith('.hpp')):
                    continue
                # file is .cpp or .h or .hpp
                filepath = os.path.join(root, file)
                parse_file(filepath, index)

    if not parse_error:
        utils.verbal(args, "saving parse output to", full_json_db_path)
        with open(full_json_db_path, 'w') as fd:
            json.dump({"symbol_dict": symbol_dict, "call_dict": call_dict, "unique_id_dict": unique_id_dict}, fd)

    return

def main(dir: str):
    global symbol_dict, call_dict, unique_id_dict, project_dir, args

    project_dir = os.path.abspath(dir)

    utils.verbal(args, "workspace path:", project_dir)
    generate_function_dict(dir)
    query = args.functions
    query_symbol = utils.search_query(list(unique_id_dict.keys()), query)
    # FIXME: doesn't match `Bar<T>::func1`
    if args.list:
        if query:
            print("query:", query, "matched symbols:", query_symbol)
        else:
            print("symbols:")
            print("==========================")
            for func in sorted(unique_id_dict.keys()):
                print(func)
            print("==========================")
        exit(0)

    utils.verbal(args, "matched symbols:", query_symbol)
    query_id = []
    for q in query_symbol:
        query_id.extend(unique_id_dict[q])

    # TODO: implement tree_report

    # if args.tree:
    #     utils.tree_report(call_dict, query, args)
    # else:
    #     utils.graph_report(call_dict, query, "class_graph", args, {})
    graph_report(call_dict, query_id, "call_graph")


######################################################################################################################################
# FIXME: refactor
# temporarily provide an implementation of graph_report, will refactor and move to utils.py later
######################################################################################################################################

def graph_report(call_dict: dict, query: list, out_file: str):
    global args, symbol_dict
    dot = graphviz.Digraph()
    dot.attr(rankdir="LR", splines="polyline")
    dot.node_attr["shape"] = "box"
    dot.node_attr["style"] = "rounded"
    inserted = {}

    if args.functions and not query:
        print("no symbol found for ", args.functions)
        print("consider using wildcard")
        return

    reserve_dict = utils.find_descendants(call_dict)

    for node in query:
        if (node in call_dict and call_dict[node]) or (node in reserve_dict and reserve_dict[node]):
            # avoid standalone node
            insert_node_to_dot(dot, node, inserted, style="filled, rounded", fillcolor="turquoise")

    if not query:
        # print all nodes and edges
        generate_graph(None, call_dict, call_dict.keys(), dot, inserted, 0, args)
    else:
        generate_graph(reserve_dict, call_dict, query, dot, inserted, 0, args)

    dot.render(out_file, format='pdf')
    print("use https://www.devtoolsdaily.com/graphviz to view graph")
    print(f"graph file is at ./{out_file}")

def generate_graph(parent_dict: dict, child_dict: dict, nodes: list, dot: graphviz.Digraph, inserted: dict, level: int, args):
    if isinstance(nodes, str):
        nodes = [nodes]

    level += 1

    for curr_node in nodes:

        # travese towards up
        if parent_dict and args.up and (args.up_level < 0 or level <= args.up_level):
            for other_node in parent_dict.get(curr_node, []):
                edge_key = f"{other_node}->{curr_node}"
                if edge_key in inserted:
                    continue

                insert_to_dot(dot, other_node, curr_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, level, args)
                else:
                    # don't need the child of the parent
                    generate_graph(parent_dict, None, other_node, dot, inserted, level, args)

        # travese towards down
        if child_dict and args.down and (args.down_level < 0 or level <= args.down_level):
            for other_node in child_dict.get(curr_node, []):
                edge_key = f"{curr_node}->{other_node}"
                if edge_key in inserted:
                    continue

                insert_to_dot(dot, curr_node, other_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, level, args)
                else:
                    # don't need the parent of the child
                    generate_graph(None, child_dict, other_node, dot, inserted, level, args)

def insert_to_dot(dot: graphviz.Digraph, src: str, dest: str, edge_key: str, inserted: dict, **attrs):
    src_name = insert_node_to_dot(dot, src, inserted)
    dest_name = insert_node_to_dot(dot, dest, inserted)
    inserted[edge_key] = True
    dot.edge(src_name, dest_name, headport='w', tailport='e', **attrs)

def insert_node_to_dot(dot: graphviz.Digraph, node: str, inserted: dict, **attrs) -> str:
    global symbol_dict
    node_name = node.replace(":", "~")
    label = symbol_dict[node]["name"]
    if node not in inserted:
        dot.node(node_name, label, tooltip=symbol_dict[node]["loc"], **attrs)
    inserted[node] = True
    return node_name


######################################################################################################################################


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='generate the inheritance hierarchy')
    parser.add_argument('--rebuild', action='store_true', help="regenerate the json database, use when source code is modify")
    parser.add_argument('--compile_db', metavar="compile_commands.json", help="JSON Compilation Database in Clang Format, will attempt to use ./compile_commands.json when not provided")
    parser.add_argument('--path', help="path to workspace root, defult is current directory")
    # parser.add_argument('--tree', action='store_true', help="output in tree view instead of graph view")
    parser.add_argument('--excl', action='append', help="file or directory names to be excluded, support glob, support multiple --excl")
    parser.add_argument('-v', '--verbal', action='store_true', help="turn on verbel printouts")

    parser.add_argument('-l', '--list', '--ls', action='store_true', help="list symbols matching the input")

    parser.add_argument('-L', '--level', action='store', default=-1, type=int, help="level of traversal in both direction")
    parser.add_argument('--up-level', '--up-lv', action='store', default=-1, type=int, metavar='LEVEL' , help="level of traversal towards the caller direction")
    parser.add_argument('--down-level', '--down-lv', action='store', default=-1, type=int, metavar='LEVEL' , help="level of traversal towards the callee direction")

    parser.add_argument('-u', '--up', action='store_true', help="only find the callers (and their callers)")
    parser.add_argument('-d', '--down', action='store_true', help="only find the callees (and their callees)")
    parser.add_argument('-r', '--related', action='store_true', help="find both the callers and the callees, this is the default")
    parser.add_argument('-c', '--connected', '--all', action='store_true', help="find all connected nodes in the call graph, includes for example, other callees of the caller (sibling nodes), only available in graph report")

    parser.add_argument('functions', nargs='*', help="name(s) of the querying functions")


    global args
    args = parser.parse_args()

    # FIXME: remove test code
    # print("WARNING: using hard-coded path for compile_commands.json")
    # args.path = "test/call_graph_template" # TODO: void callFunction<>() doesn't have the callee (*Func)()
    # args.rebuild = True

    if args.down == False and args.up == False:
        args.related = True
    if args.related == True or args.connected == True:
        args.up = True
        args.down = True

    if args.level != -1:
        if args.up_level == -1:
            args.up_level = args.level
        if args.down_level == -1:
            args.down_level = args.level

    if not args.path:
        args.path = os.getcwd() # Use the current directory if no argument is provided

    main(args.path)
