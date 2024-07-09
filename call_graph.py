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

    full_sym_name, display_name = get_function_name(node)
    loc = utils.get_symbol_decl_loc_from_def(node)

    symbol_dict[unique_id] = {"name": full_sym_name, "display_name": display_name, "has_template_callee": has_template_callee, "loc": loc}

    if full_sym_name not in unique_id_dict:
        unique_id_dict[full_sym_name] = []
    unique_id_dict[full_sym_name].append(unique_id)

    if node.kind != clang.cindex.CursorKind.FUNCTION_TEMPLATE:
        process_ast(node)

    return unique_id

# recompose the function/method full name
# in the form of [return_type] [class]<template>::[function_name]<template>(param_type param_name, ...)
def get_function_name(node: clang.cindex.Cursor):
    sym_name = node.spelling
    func_template = ""
    if node.kind == clang.cindex.CursorKind.FUNCTION_TEMPLATE: # TODO: this case is obsolete
        func_template = utils.get_template_list_from_declaration(node)
    elif node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        # TODO: what if func_ptr points to a template function
        # TODO: provide template info for other function kinds
        func_template = utils.get_template_list_from_instantiation(node)

    parent = node.semantic_parent
    parent_name = ""
    parent_seperator = ""
    if parent.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
        parent_name = parent.type.spelling
        parent_seperator = "::"
    elif parent.kind == clang.cindex.CursorKind.CLASS_TEMPLATE: # TODO: this case is obsolete
        parent_template = utils.get_template_list_from_declaration(parent)
        parent_name = parent.spelling + parent_template
        parent_seperator = "::"

    return_type = ""
    if node.kind != clang.cindex.CursorKind.CONSTRUCTOR:
        if node.kind == clang.cindex.CursorKind.VAR_DECL:
            # function pointer
            return_type = utils.replace_lambda_name(node.type.get_pointee().get_result().spelling) + ' '
            sym_name = f"(*{sym_name})"
        else:
            return_type = utils.replace_lambda_name(node.type.get_result().spelling) + ' '

    param_str = utils.get_param_list(node)

    full_sym_name = f"{return_type}{parent_name}{parent_seperator}{sym_name}{func_template}({param_str})"
    display_name = full_sym_name

    if len(display_name) > MAX_SYMBOL_LENGTH:
        # if the name is too long, trim the name
        return_type = utils.trim_namespace(return_type, False)
        parent_name = utils.trim_namespace(parent_name, False)
        func_template = utils.trim_namespace(func_template, False)
        param_str = utils.trim_namespace(param_str, False)

        display_name = f"{return_type}{parent_name}{parent_seperator}{sym_name}{func_template}({param_str})"
    if len(display_name) > MAX_SYMBOL_LENGTH:
        # still too long
        if "<" in parent_name:
            # use .spelling instead of .type.spelling, .spelling doesn't contain the template part
            parent_name = utils.trim_namespace(parent.spelling) + "<>"
        if func_template:
            func_template = "<>"
        param_str = utils.trim_namespace(utils.get_param_list(node, type_only=True), False)
        display_name = f"{return_type}{parent_name}{parent_seperator}{sym_name}{func_template}({param_str})"

    return full_sym_name, display_name

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
            process_call_expr(child, callee)

        if caller_id not in call_dict:
            call_dict[caller_id] = callee
        else:
            call_dict[caller_id] += [func for func in callee if func not in call_dict[caller_id]]

    return

def process_call_expr(node: clang.cindex.Cursor, callee: list):
    if node.kind != clang.cindex.CursorKind.CALL_EXPR:
        raise ValueError(f"wrong node: {node.spelling}, {node.kind}")

    if node.type.kind == clang.cindex.TypeKind.DEPENDENT:
        # template function
        # register_func(node, True) # set flag `has_template_callee` to True
        pass
    else:
        if not node.get_definition() and not node.referenced:
            # functions defined in other translation unit has no .get_definition() but has .referenced
            # some other CALL_EXPR nodes has neither, these are usually compiler generated intermediate CALL_EXPR
            return
        # normal function
        if node.get_definition() and node.get_definition().is_default_method():
            # compiler-provided methods such as default ctor, copy/move ctor, ...
            return
        if node.referenced.kind == clang.cindex.CursorKind.CONVERSION_FUNCTION:
            # conversion (casting) functions such as `operator int()` usually don't provide much useful information
            # about the call flow. Also for lambdas and function pointers, the function signatures are very hard to read
            # e.g., `int (*ptr)(void) = []() {return 0;}` will invoke a conversion function `int (*)() ::operator int (*)()()`
            # so we ignore them
            return
        callee_id = register_func(node.referenced)
        if callee_id not in callee:
            callee.append(callee_id)


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
    label = symbol_dict[node]["display_name"]
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
