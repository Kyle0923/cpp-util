#!/usr/bin/env python3
import os
import sys
import re
import clang.cindex
import graphviz


def is_user_defined_type(node):
    # Ensure both paths are absolute
    if node.location.file:
        file_path = os.path.abspath(node.location.file.name)
        return file_path.startswith(project_dir)
    return False

def find_classes(node, classes):
    if node.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
        if node.is_definition() and is_user_defined_type(node):
            class_name = node.spelling
            base_classes = [re.sub("^(class|struct)\\s+", "", base.spelling) for base in node.get_children() if base.kind == clang.cindex.CursorKind.CXX_BASE_SPECIFIER]
            if base_classes:
                classes[class_name] = base_classes
    for child in node.get_children():
        find_classes(child, classes)

def parse_file(filename, index):
    try:
        additional_options = ['-x', 'c++-header'] # treat .h as c++ header
        translation_unit = index.parse(filename, additional_options)
    except Exception as e:
        print(f"Error parsing file {filename}: {e}")
        return {}

    classes = {}
    find_classes(translation_unit.cursor, classes)
    return classes

def find_descendants(classes:dict):
    child_dict = {}
    for cls, bases in classes.items():
        if cls not in child_dict:
            child_dict[cls] = []
        for base in bases:
            if base not in child_dict:
                child_dict[base] = []
            child_dict[base].append(cls)
    # print(child_dict)
    return child_dict

def print_descendants(child_dict):
    top_level_classes = [c for c in child_dict.keys() if all(c not in children for children in child_dict.values())]
    for top_class in top_level_classes:
        print_tree(child_dict, top_class)


def print_ancestors(classes, class_name, indent=""):
    print_tree(classes, class_name)

def print_tree(connection: dict, node: str, indent: str = ""):
    is_top = (indent == "")
    next_level_nodes = connection.get(node, [])
    if is_top:
        if not next_level_nodes:
            print(node, ": end of hierarchy")
            return
        else:
            print(node)

    for i, next_node in enumerate(next_level_nodes):
        is_last = (i == len(next_level_nodes) - 1)
        prefix = "└── " if is_last else "├── "
        print(indent + prefix + next_node)
        print_tree(connection, next_node, indent + ("    " if is_last else "│   ") )

# generate a graph view of the class hierarchy using Graphviz
def generate_graph(parent_dict: dict, nodes: str | list, dot: graphviz.Digraph = None, inserted: dict = None):
    top = False
    if not dot:
        top = True
        dot = graphviz.Digraph()
        dot.node_attr["shape"] = "box"
        dot.node_attr["style"] = "rounded"

    if not inserted:
        inserted = {}

    if isinstance(nodes, str):
        nodes = [nodes]


    for node in nodes:
        if top:
            dot.node(node, node, style="filled, rounded", fillcolor="turquoise")
        else:
            dot.node(node, node)

        for parent in parent_dict.get(node, []):
            dot.node(parent, parent)
            if not f"{parent}->{node}" in inserted:
                inserted[f"{parent}->{node}"] = True
                dot.edge(parent, node)

            generate_graph(parent_dict, parent, dot, inserted)

    if top:
        dot.render('class_graph', format='pdf')
        print("use https://dreampuf.github.io/GraphvizOnline/ to view graph")
        print("graph file is at './class_graph'")

def main(directory, query):
    global project_dir
    project_dir = os.path.abspath(directory)

    index = clang.cindex.Index.create()
    parent_dict = {} # key: class, value: base class
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.h') or file.endswith('.hpp'):
                filepath = os.path.join(root, file)
                classes = parse_file(filepath, index)
                parent_dict.update(classes)

    child_dict = find_descendants(parent_dict)
    if (not query):
        print_descendants(child_dict)
    else:
        generate_graph(parent_dict, query)

if __name__ == "__main__":
    query = None
    if len(sys.argv) > 1:
        query = sys.argv[1]

    directory = os.getcwd()  # Use the current directory if no argument is provided
    main(directory, query)
