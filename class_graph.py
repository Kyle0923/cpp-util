#!/usr/bin/env python3
import os
import json
import re
import argparse
import clang.cindex
import utils

# workspace root
# any types defined under this directory will be considered project definition instead of system definition
WS_ROOT = ""

# when finished parsing the directory, this script will dump the result to this json file
# the second run of this script will use this json file instead, this will save time for parsing
# use --rebuild to regenerate this json file when source code is updated
CLASS_GRAPH_DB_JSON = "class_graph_db.json"

def is_project_defined_symbol(node):
    # Ensure both paths are absolute
    if node.location.file:
        file_path = os.path.abspath(node.location.file.name)
        return file_path.startswith(project_dir) or (WS_ROOT and file_path.startswith(WS_ROOT))
    return False

def find_classes(node, classes, templates):
    node_name = utils.get_full_type_name(node)
    if node.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
        if node.is_definition() and is_project_defined_symbol(node):
            base_classes = [utils.get_full_type_name(base) for base in node.get_children() if base.kind == clang.cindex.CursorKind.CXX_BASE_SPECIFIER]
            if base_classes:
                classes[node_name] = base_classes
    for child in node.get_children():
        find_classes(child, classes, templates)

    # handle template args
    for templ_idx in range(0, node.type.get_num_template_arguments()):
        if node_name not in templates:
            templates[node_name] = []
        template_arg = node.type.get_template_argument_type(templ_idx).get_declaration()
        template_arg_name = utils.get_full_type_name(template_arg)
        if template_arg_name:
            templates[node_name].append({"name": template_arg_name, "label": f"template#{templ_idx+1}"})
            find_classes(template_arg, classes, templates)

def parse_file(filename, index):
    if (utils.path_name_match(filename, args.excl)):
        utils.verbal(args, "skipping ", filename)
        return {}

    utils.verbal(args, "parsing ", filename)

    try:
        additional_options = ['-x', 'c++-header'] # treat .h as c++ header
        abs_path = os.path.abspath(filename)
        if abs_path in compile_db:
            additional_options += compile_db[abs_path]
        else:
            additional_options += compile_default_options.keys()

        translation_unit = index.parse(filename, additional_options)
    except Exception as e:
        print(f"Error parsing file {filename}: {e}")
        return {}

    for diag in translation_unit.diagnostics:
        if 'file not found' in diag.spelling:
            # possible incomplete parsing
            global parse_error
            parse_error = True
            print(f"Missing include: {diag.spelling} - {diag.location.file}:{diag.location.line}")

    # utils.print_ast(translation_unit.cursor)
    # exit(0)
    # translation_unit.save(filename.replace('/', '__'))

    classes = {}
    templates = {}
    find_classes(translation_unit.cursor, classes, templates)
    return classes, templates

# return a list of the source files if compile_commands.json exists
def get_compile_options(dir: str) -> list:

    # when a file doesn't have entry in compile_commands.json or the json doesn't exist
    # will use compile_default_options
    global compile_default_options
    compile_default_options = {}

    global compile_db
    compile_db = {}

    if not args.compile_db:
        args.compile_db = os.path.join(dir, "compile_commands.json")
    if os.path.isfile(args.compile_db):
        parse_compile_commands_json(args.compile_db)
        return compile_db.keys() # return a list of the source files

    guess_incl_path(dir)
    return []

def parse_compile_commands_json(file: str):
    with open(file) as fd:
        json_db = json.load(fd)

    for obj in json_db:
        compile_options = trim_compile_options(obj["arguments"])
        abs_path = os.path.join(obj["directory"], obj["file"])
        compile_db[abs_path] = compile_options

# only keep -I, -D, and -std options
def trim_compile_options(options: list) -> list:
    trimmed = []
    for opt in options:
        if opt.startswith("-I") or opt.startswith("-D") or opt.startswith("-std"):
            trimmed.append(opt)
            compile_default_options[opt] = True
    return trimmed

def guess_incl_path(dir: str):
    for dirpath, _, files in os.walk(dir):
        for file in files:
            if file.endswith('.h') or file.endswith('.hpp'):
                compile_default_options[f"-I{dirpath}"] = True
                break

def generate_parent_dict(dir: str):
    full_json_db_path = os.path.join(dir, CLASS_GRAPH_DB_JSON)

    if args.rebuild and os.path.isfile(full_json_db_path):
        os.remove(full_json_db_path)

    if os.path.isfile(full_json_db_path):
        with open(full_json_db_path) as fd:
            print(f"[[ {full_json_db_path} ]] exists, skip parsing source code, use --rebuild to force parsing source\n")
            db = json.load(fd)
            parent_dict = db["parent_dict"]
            template_dict = db["template_dict"]
            return parent_dict, template_dict

    global project_dir
    project_dir = os.path.abspath(dir)

    src_files = get_compile_options(dir)

    index = clang.cindex.Index.create()
    parent_dict = {} # key: class, value: base class
    template_dict = {} # key: class, value: template class

    global parse_error
    parse_error = False

    print("starting parsing source files, this can take a while")
    if src_files:
        for src in src_files:
            classes, template = parse_file(src, index)
            parent_dict.update(classes)
            template_dict.update(template)

    else:
        for root, _, files in os.walk(dir):
            for file in files:
                if not (file.endswith('.cpp') or file.endswith('.h') or file.endswith('.hpp')):
                    continue
                # file is .cpp or .h or .hpp
                filepath = os.path.join(root, file)
                classes, template = parse_file(filepath, index)
                parent_dict.update(classes)
                template_dict.update(template)

    if not parse_error:
        utils.verbal(args, "saving parse output to", full_json_db_path)
        with open(full_json_db_path, 'w') as fd:
            json.dump({"parent_dict": parent_dict, "template_dict": template_dict}, fd)

    return parent_dict, template_dict

def main(dir):
    utils.verbal(args, "workspace path:", dir)
    parent_dict, template_dict = generate_parent_dict(dir)
    query = args.classes
    if args.tree:
        utils.tree_report(parent_dict, query, args)
    else:
        utils.graph_report(parent_dict, query, "class_graph", args, template_dict)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='generate the inheritance hierarchy')
    parser.add_argument('--rebuild', action='store_true', help="regenerate the json database, use when source code is modify")
    parser.add_argument('--compile_db', metavar="compile_commands.json", help="JSON Compilation Database in Clang Format, will attempt to use ./compile_commands.json when not provided")
    parser.add_argument('--path', help="path to workspace root, defult is current directory")
    parser.add_argument('--tree', action='store_true', help="output in tree view instead of graph view")
    parser.add_argument('--excl', action='append', help="file or directory names to be excluded, support glob, support multiple --excl")
    parser.add_argument('-v', '--verbal', action='store_true', help="turn on verbel printouts")
    parser.add_argument('-b', '--base', action='store_true', help="only print the ancestor classes")
    parser.add_argument('-d', '--derived', action='store_true', help="only print the descendant classes")
    parser.add_argument('-r', '--related', action='store_true', help="print both the ancestor and descendant classes, this is the default")
    parser.add_argument('-c', '--connected', '--all', action='store_true', help="print all classes that are connected to any of the ancestor and descendant classes, only available in graph report")

    parser.add_argument('classes', nargs='*', help="name(s) of the querying class")


    global args
    args = parser.parse_args()

    # process --derived vs --base vs --related, use --related as default
    if args.derived == False and args.base == False:
        args.related = True
    if args.related == True or args.connected == True:
        args.base = True
        args.derived = True

    if not args.path:
        args.path = os.getcwd() # Use the current directory if no argument is provided

    main(args.path)
