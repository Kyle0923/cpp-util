#!/usr/bin/env python3
import os
import json
import argparse
import clang.cindex
import utils

FUNCTION_GRAPH_DB_JSON = "function_graph_db.json"

# location = "file:line" of the declaration, cannot use definition because the def can be in other translation unit
symbol_dict = {} # "location": {"name": symbol_name, "has_template_callee": bool}
call_dict = {} # "caller_location": ["callee_1_location", "callee_2_location", ...]

def register_func(node: clang.cindex.Cursor, has_template_callee: bool = False):
    global symbol_dict
    # node = node.get_definition()
    if not node or node.kind not in [clang.cindex.CursorKind.CXX_METHOD, clang.cindex.CursorKind.FUNCTION_DECL, clang.cindex.CursorKind.FUNCTION_TEMPLATE]:
        # print(node.spelling)
        # raise ("wrong kind")
        return
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

    return_type = node.type.get_result().spelling

    sym_name = f"{return_type} {sym_name}({get_param_list(node)})"

    # symbol_dict[loc] = {"name": sym_name, "type": node.type.spelling}
    symbol_dict[loc] = {"name": sym_name, "has_template_callee": has_template_callee}

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
            for grandchild in child.get_children():
                if grandchild.kind != clang.cindex.CursorKind.TYPE_REF:
                    continue
                param_type = grandchild.spelling
                break
            params.append(f"{param_type} {param_name}")
    else:
        for param in node.get_arguments():
            params.append(param.type.spelling + " " + param.spelling)

    param_str = ", ".join(params)
    return param_str

def process_ast(cursor: clang.cindex.Cursor):
    global symbol_dict, call_dict, project_dir
    for node in cursor.walk_preorder():
        if node.kind not in [clang.cindex.CursorKind.CXX_METHOD, clang.cindex.CursorKind.FUNCTION_DECL, clang.cindex.CursorKind.FUNCTION_TEMPLATE]:
            continue
        caller_loc = register_func(node)
        if not utils.is_project_defined_symbol(node, project_dir):
            continue
        if caller_loc in call_dict and symbol_dict[caller_loc]["has_template_callee"] == False:
            continue
        callee = {} # use dict to avoid duplicates
        def_node = node.get_definition()

        if not def_node:
            # function's definition is in other translation unit
            continue

        for child in def_node.walk_preorder():
            if not child.kind == clang.cindex.CursorKind.CALL_EXPR:
                continue

            if child.type.kind != clang.cindex.TypeKind.DEPENDENT:
                # normal function
                if (child.get_definition() and child.get_definition().is_default_method()):
                    # compiler-provided methods such as default ctor, copy/move ctor, ...
                    continue
                callee_loc = utils.get_symbol_decl_loc_from_call(child)
                callee[callee_loc] = True
                if symbol_dict[callee_loc]["has_template_callee"] == True and child.get_definition():
                    # child.get_definition() points to the clang-instantiated version of the template function
                    # the loc of the callees in the instantiated template will point to the correct tempaltes
                    # including (partical) specialization
                    process_ast(child.get_definition())
            else:
                # template function
                register_func(node, True) # set flag `has_template_callee` to True

        callee = list(callee.keys())
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
    global symbol_dict, call_dict

    full_json_db_path = os.path.join(dir, FUNCTION_GRAPH_DB_JSON)

    if args.rebuild and os.path.isfile(full_json_db_path):
        os.remove(full_json_db_path)

    if os.path.isfile(full_json_db_path):
        with open(full_json_db_path) as fd:
            print(f"[[ {full_json_db_path} ]] exists, skip parsing source code, use --rebuild to force parsing source\n")
            db = json.load(fd)
            symbol_dict = db["symbol_dict"]
            call_dict = db["call_dict"]
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
            json.dump({"symbol_dict": symbol_dict, "call_dict": call_dict}, fd)

    return

def main(dir: str):
    global symbol_dict, call_dict, project_dir

    project_dir = os.path.abspath(dir)

    utils.verbal(args, "workspace path:", project_dir)
    generate_function_dict(dir)
    query = args.functions
    # if args.tree:
    #     utils.tree_report(call_dict, query, args)
    # else:
    #     utils.graph_report(call_dict, query, "class_graph", args, {})

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='generate the inheritance hierarchy')
    parser.add_argument('--rebuild', action='store_true', help="regenerate the json database, use when source code is modify")
    parser.add_argument('--compile_db', metavar="compile_commands.json", help="JSON Compilation Database in Clang Format, will attempt to use ./compile_commands.json when not provided")
    parser.add_argument('--path', help="path to workspace root, defult is current directory")
    parser.add_argument('--tree', action='store_true', help="output in tree view instead of graph view")
    parser.add_argument('--excl', action='append', help="file or directory names to be excluded, support glob, support multiple --excl")
    parser.add_argument('-v', '--verbal', action='store_true', help="turn on verbel printouts")
    # parser.add_argument('-b', '--base', action='store_true', help="only print the ancestor classes")
    # parser.add_argument('-d', '--derived', action='store_true', help="only print the descendant classes")
    # parser.add_argument('-r', '--related', action='store_true', help="print both the ancestor and descendant classes, this is the default")
    # parser.add_argument('-c', '--connected', '--all', action='store_true', help="print all classes that are connected to any of the ancestor and descendant classes, only available in graph report")

    parser.add_argument('functions', nargs='*', help="name(s) of the querying functions")


    global args
    args = parser.parse_args()

    # FIXME: remove test code
    # the compile_commands.json currently only use c1.cpp
    print("WARNING: using hard-coded path for compile_commands.json")
    args.path = "test/call_graph"
    # args.compile_db = "compile_commands.json"


    # process --derived vs --base vs --related, use --related as default
    # if args.derived == False and args.base == False:
    #     args.related = True
    # if args.related == True or args.connected == True:
    #     args.base = True
    #     args.derived = True

    if not args.path:
        args.path = os.getcwd() # Use the current directory if no argument is provided

    main(args.path)
