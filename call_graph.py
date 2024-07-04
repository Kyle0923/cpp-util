#!/usr/bin/env python3
import os
import json
import argparse
import clang.cindex
import graphviz
import utils

FUNCTION_GRAPH_DB_JSON = "function_graph_db.json"

# location = "file:line" of the declaration, cannot use definition because the def can be in other translation unit
symbol_dict = {} # "location": {"name": symbol_name, "has_template_callee": bool}
call_dict = {} # "caller_location": ["callee_1_location", "callee_2_location", ...]
location_dic = {} # function_name: ["location1", "location1"]

def register_func(node: clang.cindex.Cursor, has_template_callee: bool = False):
    global symbol_dict, location_dic
    if node.kind not in [clang.cindex.CursorKind.CXX_METHOD, clang.cindex.CursorKind.FUNCTION_DECL, \
                                     clang.cindex.CursorKind.FUNCTION_TEMPLATE, clang.cindex.CursorKind.CONSTRUCTOR, \
                                     clang.cindex.CursorKind.CONVERSION_FUNCTION, clang.cindex.CursorKind.VAR_DECL]:
        print(node.spelling, node.kind)
        raise ("wrong kind")
    loc = utils.get_symbol_decl_loc_from_def(node)
    if loc in symbol_dict:
        # has_template_callee == True means further process is needed when this function is called
        symbol_dict[loc]["has_template_callee"] |= has_template_callee
        return loc

    sym_name = node.spelling
    if node.kind == clang.cindex.CursorKind.FUNCTION_TEMPLATE:
        template = get_template_type_list(node)
        sym_name = f"{sym_name}<{template}>"

    parent = node.semantic_parent
    if parent.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
        sym_name = f"{parent.spelling}::{sym_name}"
    elif parent.kind == clang.cindex.CursorKind.CLASS_TEMPLATE:
        template = get_template_type_list(parent)
        sym_name = f"{parent.spelling}<{template}>::{sym_name}"

    if node.kind == clang.cindex.CursorKind.VAR_DECL:
        # function pointer
        return_type = node.type.get_pointee().get_result().spelling
        sym_name = f"(*{sym_name})"
    else:
        return_type = node.type.get_result().spelling

    sym_name = f"{return_type} {sym_name}({get_param_list(node)})"

    symbol_dict[loc] = {"name": sym_name, "has_template_callee": has_template_callee}

    if sym_name not in location_dic:
        location_dic[sym_name] = []
    location_dic[sym_name].append(loc)

    return loc

def get_template_type_list(node: clang.cindex.Cursor):
    if node.kind not in [clang.cindex.CursorKind.CLASS_TEMPLATE, clang.cindex.CursorKind.FUNCTION_TEMPLATE]:
        print("wrong node: ", node.spelling)
        raise
    template_types = []
    for child in node.get_children():
        if child.kind == clang.cindex.CursorKind.TEMPLATE_TYPE_PARAMETER:
            template_types.append(child.spelling)
        elif child.kind == clang.cindex.CursorKind.TEMPLATE_NON_TYPE_PARAMETER:
            template_types.append(child.type.spelling)

    return ", ".join(template_types)

def get_param_list(node: clang.cindex.Cursor):
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
                param_type = replace_lambda_type(grandchild)
                break
            params.append(f"{param_type} {param_name}".strip())
    elif node.kind == clang.cindex.CursorKind.VAR_DECL:
        # function pointer
        for param in node.type.get_pointee().argument_types():
            param_type = replace_lambda_type(param)
            params.append( param_type.strip() )
    else:
        for param in node.get_arguments():
            param_type = replace_lambda_type(param.type)
            params.append( (param_type + " " + param.spelling).strip() )

    param_str = ", ".join(params)
    return param_str

def replace_lambda_type(node: clang.cindex.Cursor):
    if is_lambda(node):
        return get_lambda_name(node)
    return node.spelling

def is_lambda(node: clang.cindex.Cursor):
    typename = node.spelling
    return typename.startswith("(lambda at")

def get_lambda_name(node: clang.cindex.Cursor):
    typename = node.spelling
    suffix = typename.split(')')[-1].strip()
    if suffix:
        suffix = f" {suffix}"
    location = typename.split('at')[1].strip()
    file, line, _ = location.split(':')
    file = os.path.basename(file)
    param_type = f"(lambda@{file}:{line}){suffix}"
    return param_type

def process_ast(cursor: clang.cindex.Cursor):
    global symbol_dict, call_dict, project_dir
    for node in cursor.walk_preorder():
        if node.kind not in [clang.cindex.CursorKind.CXX_METHOD, clang.cindex.CursorKind.FUNCTION_DECL, clang.cindex.CursorKind.FUNCTION_TEMPLATE]:
            continue
        if not utils.is_project_defined_symbol(node, project_dir):
            continue
        caller_loc = register_func(node)
        if caller_loc in call_dict and symbol_dict[caller_loc]["has_template_callee"] == False:
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
                callee_loc = register_func(child.referenced)
                if callee_loc not in callee:
                    callee.append(callee_loc)
                if symbol_dict[callee_loc]["has_template_callee"] == True and child.get_definition():
                    # child.get_definition() points to the clang-instantiated version of the template function
                    # the loc of the callees in the instantiated template will point to the correct tempaltes
                    # including (partical) specialization
                    process_ast(child.get_definition())

        if caller_loc not in call_dict:
            call_dict[caller_loc] = callee
        else:
            call_dict[caller_loc] += [func for func in callee if func not in call_dict[caller_loc]]

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
    global symbol_dict, call_dict, location_dic

    full_json_db_path = os.path.join(dir, FUNCTION_GRAPH_DB_JSON)

    if args.rebuild and os.path.isfile(full_json_db_path):
        os.remove(full_json_db_path)

    if os.path.isfile(full_json_db_path):
        with open(full_json_db_path) as fd:
            print(f"[[ {full_json_db_path} ]] exists, skip parsing source code, use --rebuild to force parsing source\n")
            db = json.load(fd)
            symbol_dict = db["symbol_dict"]
            call_dict = db["call_dict"]
            location_dic = db["location_dic"]
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
            json.dump({"symbol_dict": symbol_dict, "call_dict": call_dict, "location_dic": location_dic}, fd)

    return

def main(dir: str):
    global symbol_dict, call_dict, location_dic, project_dir

    project_dir = os.path.abspath(dir)

    utils.verbal(args, "workspace path:", project_dir)
    generate_function_dict(dir)
    query = args.functions
    query_symbol = utils.search_query(list(location_dic.keys()), query)
    utils.verbal(args, "matched symbols:", query_symbol)
    query_loc = []
    for q in query_symbol:
        query_loc.extend(location_dic[q])

    # TODO: implement tree_report

    # if args.tree:
    #     utils.tree_report(call_dict, query, args)
    # else:
    #     utils.graph_report(call_dict, query, "class_graph", args, {})
    graph_report(call_dict, query_loc, "call_graph")


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
        generate_graph(None, call_dict, call_dict.keys(), dot, inserted, args)
    else:
        generate_graph(reserve_dict, call_dict, query, dot, inserted, args)

    dot.render(out_file, format='pdf')
    print("use https://www.devtoolsdaily.com/graphviz to view graph")
    print(f"graph file is at ./{out_file}")

def generate_graph(parent_dict: dict, child_dict: dict, nodes: list, dot: graphviz.Digraph, inserted: dict, args):
    if isinstance(nodes, str):
        nodes = [nodes]

    for curr_node in nodes:
        # travese towards up
        if parent_dict and args.up:
            for other_node in parent_dict.get(curr_node, []):
                edge_key = f"{other_node}->{curr_node}"
                if edge_key in inserted:
                    continue

                insert_to_dot(dot, other_node, curr_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, args)
                else:
                    # don't need the child of the parent
                    generate_graph(parent_dict, None, other_node, dot, inserted, args)

        # travese towards down
        if child_dict and args.down:
            for other_node in child_dict.get(curr_node, []):
                edge_key = f"{curr_node}->{other_node}"
                if edge_key in inserted:
                    continue

                insert_to_dot(dot, curr_node, other_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, args)
                else:
                    # don't need the parent of the child
                    generate_graph(None, child_dict, other_node, dot, inserted, args)

def insert_to_dot(dot: graphviz.Digraph, src: str, dest: str, edge_key: str, inserted: dict, **attrs):
    src_name = insert_node_to_dot(dot, src, inserted)
    dest_name = insert_node_to_dot(dot, dest, inserted)
    inserted[edge_key] = True
    dot.edge(src_name, dest_name, headport='w', tailport='e', **attrs)

def insert_node_to_dot(dot: graphviz.Digraph, node: str, inserted: dict, **attrs) -> str:
    global symbol_dict
    node_name = node.replace(":", "#")
    label = symbol_dict[node]["name"]
    if node not in inserted:
        dot.node(node_name, label, tooltip=node, **attrs)
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

    parser.add_argument('-u', '--up', action='store_true', help="only find the callers (and their callers)")
    parser.add_argument('-d', '--down', action='store_true', help="only find the callees (and their callees)")
    parser.add_argument('-r', '--related', action='store_true', help="find both the callers and the callees, this is the default")
    parser.add_argument('-c', '--connected', '--all', action='store_true', help="find all connected nodes in the call graph, includes for example, other callees of the caller (sibling nodes), only available in graph report")

    parser.add_argument('functions', nargs='*', help="name(s) of the querying functions")


    global args
    args = parser.parse_args()

    # FIXME: remove test code
    # print("WARNING: using hard-coded path for compile_commands.json")
    # args.path = "test/call_graph_lambda"
    # args.rebuild = True

    if args.down == False and args.up == False:
        args.related = True
    if args.related == True or args.connected == True:
        args.up = True
        args.down = True

    if not args.path:
        args.path = os.getcwd() # Use the current directory if no argument is provided

    main(args.path)
