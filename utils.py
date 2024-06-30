import re
import clang.cindex
import graphviz
import fnmatch
import os
import json

def find_descendants(parent_dict: dict):
    child_dict = {}
    for derived, bases in parent_dict.items():
        if derived not in child_dict:
            child_dict[derived] = []
        for base in bases:
            if base not in child_dict:
                child_dict[base] = []
            child_dict[base].append(derived)
    return child_dict

##############################################################################################################
# Tree report
##############################################################################################################
def tree_report(parent_dict: dict, query: list, args):
    matched_nodes = search_query(parent_dict, {}, query)
    if query:
        verbal(args, f"query: {query}, found {len(matched_nodes)} nodes", matched_nodes)

    if query and not matched_nodes:
        print("no symbol found for ", query)
        print("consider using wildcard")
        return
    if args.base:
        print_ancestors(parent_dict, matched_nodes)
    if args.derived:
        child_dict = find_descendants(parent_dict)
        print_descendants(child_dict, matched_nodes)

def print_ancestors(parent_dict: dict, classes: list):
    if not classes:
        classes = [p for p in parent_dict.keys() if all(p not in parent for parent in parent_dict.values())]
    print("#######################################################")
    print("printing ancestors")
    print('============================')
    tree_print(parent_dict, classes, "", "ancestors")
    print("#######################################################")
    print()

def print_descendants(child_dict: dict, classes: list):
    if not classes:
        classes = [c for c in child_dict.keys() if all(c not in children for children in child_dict.values())]
    print("#######################################################")
    print("printing descendants")
    print('============================')
    tree_print(child_dict, classes, "", "descendants")
    print("#######################################################")
    print()

def tree_print(connection: dict, nodes: list, indent: str = "", msg: str = ""):
    if isinstance(nodes, str):
        nodes = [nodes]

    for node in nodes:
        is_top = (indent == "")
        next_level_nodes = connection.get(node, [])
        if is_top:
            if not next_level_nodes:
                print(node, f": no {msg} found")
                return
            else:
                print(node)

        for i, next_node in enumerate(next_level_nodes):
            is_last = (i == len(next_level_nodes) - 1)
            prefix = "└── " if is_last else "├── "
            print(indent + prefix + next_node)
            tree_print(connection, next_node, indent + ("    " if is_last else "│   ") )

        if is_top:
            print('----------------------------')


##############################################################################################################
# Graph report
##############################################################################################################

# secondary_edge is used to represent a secondary relation such as template link
def graph_report(parent_dict: dict, query: list, out_file: str, args, secondary_edge: dict):
    dot = graphviz.Digraph()
    dot.node_attr["shape"] = "box"
    dot.node_attr["style"] = "rounded"
    inserted = {}

    matched_nodes = search_query(parent_dict, secondary_edge, query)

    if query:
        verbal(args, f"query: {query}, found {len(matched_nodes)} nodes", matched_nodes)

    if query and not matched_nodes:
        print("no symbol found for ", query)
        print("consider using wildcard")
        return

    for node in matched_nodes:
        insert_node_to_dot(dot, node, inserted, style="filled, rounded", fillcolor="turquoise")

    if not matched_nodes:
        # print all nodes and edges
        generate_graph(parent_dict, None, parent_dict.keys(), dot, inserted, args, secondary_edge)
    else:
        child_dict = find_descendants(parent_dict)
        generate_graph(parent_dict, child_dict, matched_nodes, dot, inserted, args, secondary_edge)

    dot.render(out_file, format='pdf')
    print("use https://dreampuf.github.io/GraphvizOnline/ to view graph")
    print(f"graph file is at ./{out_file}")

# generate a graph view of the class hierarchy using Graphviz
def generate_graph(parent_dict: dict, child_dict: dict, nodes: list, dot: graphviz.Digraph, inserted: dict, args, secondary_edge: dict = {}):
    if isinstance(nodes, str):
        nodes = [nodes]

    for curr_node in nodes:
        # travese towards base
        if parent_dict and args.base:
            for other_node in parent_dict.get(curr_node, []):
                edge_key = f"{other_node}->{curr_node}"
                if edge_key in inserted:
                    continue

                insert_to_dot(dot, other_node, curr_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, args, secondary_edge)
                else:
                    # don't need the child of the parent
                    generate_graph(parent_dict, None, other_node, dot, inserted, args, secondary_edge)

        # travese towards derived
        if child_dict and args.derived:
            for other_node in child_dict.get(curr_node, []):
                edge_key = f"{curr_node}->{other_node}"
                if edge_key in inserted:
                    continue

                insert_to_dot(dot, curr_node, other_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, args, secondary_edge)
                else:
                    # don't need the parent of the child
                    generate_graph(None, child_dict, other_node, dot, inserted, args, secondary_edge)

        # handle secondary_edge
        if curr_node in secondary_edge:
            for other_node in secondary_edge[curr_node]:
                edge_key = f"{other_node['name']}->{curr_node}"
                if edge_key in inserted:
                    continue

                edge_label = other_node["label"] if "label" in other_node else ""
                insert_to_dot(dot, other_node["name"], curr_node, edge_key, inserted, minlen="2", \
                              color="navy", arrowhead="odot", style="dashed", splines="false", \
                              headlabel=edge_label)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node["name"], dot, inserted, args, secondary_edge)
                else:
                    # don't need the child of the parent
                    generate_graph(parent_dict, None, other_node["name"], dot, inserted, args, secondary_edge)


def insert_to_dot(dot: graphviz.Digraph, src: str, dest: str, edge_key: str, inserted: dict, **attrs):
    src_name = insert_node_to_dot(dot, src, inserted)
    dest_name = insert_node_to_dot(dot, dest, inserted)
    inserted[edge_key] = True
    dot.edge(src_name, dest_name, **attrs)

# insert to dot if not exist
# and return the escaped name of the node
def insert_node_to_dot(dot: graphviz.Digraph, node: str, inserted: dict, **attrs) -> str:
    node_name = node
    if "::" in node_name:
        node_name = node_name.replace("::", "__")
        if node not in inserted:
            dot.node(node_name, trim_namespace(node), tooltip=node, **attrs)
    inserted[node] = True
    return node_name

##############################################################################################################
# clang symbol handling
##############################################################################################################

# return the fully quilified name with the complete namespace
def get_full_type_name(node: clang.cindex.CursorKind):
    type_name = node.type.get_declaration().type.get_canonical().spelling or node.spelling
    return replace_std_string(node, type_name)

# std::string usually expands to hard-to-read underlying type
# replace_std_string() will convert it back to std::string
std_string_canonical = ""
def replace_std_string(node: clang.cindex.CursorKind, type_name: str):
    global std_string_canonical
    STD_STRING = "std::string"
    if std_string_canonical:
        return type_name.replace(std_string_canonical, STD_STRING)

    if node.kind.is_declaration() or node.kind == clang.cindex.CursorKind.CXX_BASE_SPECIFIER:
        for child in node.get_children():
            if child.kind == clang.cindex.CursorKind.TYPE_REF and STD_STRING in child.spelling:
                std_string_canonical = child.type.get_canonical().spelling
                return type_name.replace(std_string_canonical, STD_STRING)

    return type_name

# print AST for debug
def print_ast(node, level=0):
    if (level == 1):
        print("==========================")

    if (level > 0):
        print('    ' * (level - 1) + to_string(node))
        # template_args = ""
        # if (node.type.get_num_template_arguments() > 0):
        #     template_args = node.type.get_template_argument_type(0).spelling
        # print('    ' * (level - 1) + f'{node.spelling}, K:{node.kind}, T:{node.type.spelling}, #A: {node.type.get_num_template_arguments()}, A:{template_args}')

    for child in node.get_children():
        print_ast(child, level + 1)

def to_string(node):
    loc_str = f"{node.location.file.name}:{node.location.line}" if node.location.file else ""
    return f'S: {node.spelling}, K: {node.kind}, PS: {node.semantic_parent.spelling if node.semantic_parent else ""},' + \
            f'L: {loc_str}, T: {node.type.kind.spelling}, #T: {node.type.get_num_template_arguments()}'

##############################################################################################################
# clang interaction
##############################################################################################################
# return a list of the source files if compile_commands.json exists
def get_compile_options(dir: str, args) -> list:

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

def is_project_defined_symbol(node, project_dir: str, ws_root: str = ""):
    # Ensure both paths are absolute
    if node.location.file:
        file_path = os.path.abspath(node.location.file.name)
        return file_path.startswith(project_dir) or (ws_root and file_path.startswith(ws_root))
    return False

##############################################################################################################
# miscellaneous
##############################################################################################################
def path_name_match(path: str, patterns: list) -> bool:
    if not patterns:
        return False

    for pattern in patterns:
        if is_glob(pattern):
            if fnmatch.fnmatch(path, pattern):
                return True
        else:
            if pattern in path:
                return True

    return False

def is_glob(pattern: str) -> bool:
    glob_chars = ['*', '?', '[', ']', '{', '}', '!']
    return any(char in pattern for char in glob_chars)

def find_last_double_colon_before_template(s):
    # Find the index of the first occurrence of '<'
    template_index = s.find('<')
    if template_index == -1:
        substring = s
    else:
        # Slice the string up to the first occurrence of '<'
        substring = s[:template_index]

    # Find the last occurrence of '::' in the sliced portion
    return substring.rfind('::')

def break_long_name(name):
    if len(name) > 50:
        index = find_last_double_colon_before_template(name)
        if index != -1:
            index += 2
            return name[:index] + "\\n" + name[index:]
    return name

# remove the namespaces
def trim_namespace(type_name):
    result = ''
    temp_part = ''

    for char in type_name:
        if char == ' ':
            continue
        temp_part += char
        if char == ':':
            if temp_part == 'std:' or temp_part == 'std::':
                continue
            temp_part = ''
        elif char == '<':
            result += temp_part
            temp_part = ''
        elif char == '>':
            result += temp_part
            temp_part = ''
        elif char == ',':
            result += temp_part + ' '
            temp_part = ''
        else:
            # do nothing
            pass
    result += temp_part

    return result

def verbal(opton, *args):
    if (opton.verbal):
        print(*args)

def search_query(parent_dict: dict, secondary_edge: dict, query: list):
    query_nodes = []
    node_list = []
    for value in parent_dict.values():
        node_list.extend(value)
    node_list.extend(parent_dict.keys())

    # searching in secondary_edge can be noisy
    # e.g., printing std::vector<BaseClass> doesn't provide much insight
    # at the time being, we exclude the types starts with std:: to avoid the ones from stl
    if secondary_edge:
        for t_list in secondary_edge.values():
            node_list.extend([t_arg["name"] for t_arg in t_list if not t_arg["name"].startswith("std::")])
        node_list.extend([key for key in secondary_edge.keys() if not key.startswith("std::")])

    for query_name in query:
        for node in node_list:
            if match_query(node, query_name):
                if node not in query_nodes:
                    query_nodes.append(node)

    return query_nodes

def match_query(declaration: str, query: str) -> bool:

    # break down declaration like a::b<c::d, e>::f to [a::b, c::d, e, ::f]
    pattern = r'[<>,]'
    names = [n.strip() for n in re.split(pattern, declaration)]

    if '*' not in query:
        return exact_match_query(names, query)
    else:
        return fuzzy_match_query(names, query)

def exact_match_query(names: list, query_name: str) -> bool:
    for name in names:
        if name == query_name or name.endswith(f"::{query_name}"):
            return True
    return False

def fuzzy_match_query(names: list, query: str) -> bool:
    query = "^(.*::)?" + query.replace("*", ".*") + "$"
    return any([re.match(query, name) for name in names])
