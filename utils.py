import clang.cindex
import graphviz
import fnmatch

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
def tree_report(parent_dict, query, args):
    if args.base:
        print_ancestors(parent_dict, query)
    if args.derived:
        child_dict = find_descendants(parent_dict)
        print_descendants(child_dict, query)

def print_ancestors(parent_dict: dict, classes: list):
    if not classes:
        classes = [p for p in parent_dict.keys() if all(p not in parent for parent in parent_dict.values())]
    print("#######################################################")
    print("printing ancestors")
    print('============================')
    print_tree(parent_dict, classes, "", "ancestors")
    print("#######################################################")
    print()

def print_descendants(child_dict: dict, classes: list):
    if not classes:
        classes = [c for c in child_dict.keys() if all(c not in children for children in child_dict.values())]
    print("#######################################################")
    print("printing descendants")
    print('============================')
    print_tree(child_dict, classes, "", "descendants")
    print("#######################################################")
    print()

def print_tree(connection: dict, nodes: list, indent: str = "", msg: str = ""):
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
            print_tree(connection, next_node, indent + ("    " if is_last else "│   ") )

        if is_top:
            print('----------------------------')


##############################################################################################################
# Graph report
##############################################################################################################

# secondary_edge is used to represent a secondary relation such as template link
def graph_report(parent_dict: dict, query, out_file: str, args, secondary_edge: dict):
    dot = graphviz.Digraph()
    dot.node_attr["shape"] = "box"
    dot.node_attr["style"] = "rounded"
    inserted = {}

    query_nodes = []
    for query_name in query:
        joined_list = []
        for value in parent_dict.values():
            joined_list.extend(value)
        for node in [*parent_dict.keys(), *joined_list]:
            if query_name == node or f"::{query_name}" in node:
                if node not in query_nodes:
                    query_nodes.append(node)

    if (query):
        verbal(args, f"query: {query}, found {len(query_nodes)} nodes", query_nodes)

    for node in query_nodes:
        insert_node_to_dot(dot, node, inserted, style="filled, rounded", fillcolor="turquoise")

    if not query_nodes:
        # print all nodes and edges
        generate_graph(parent_dict, None, parent_dict.keys(), dot, inserted, args, secondary_edge)
    else:
        child_dict = find_descendants(parent_dict)
        generate_graph(parent_dict, child_dict, query_nodes, dot, inserted, args, secondary_edge)

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
            for other_node in secondary_edge.get(curr_node, []):
                edge_key = f"{other_node}->{curr_node}"
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
            dot.node(node_name, trim_namesapce(node), tooltip=node, **attrs)
    inserted[node] = True
    return node_name

##############################################################################################################
# clang symbol handling
##############################################################################################################

# return the fully quilified name with the complete namespace
def get_full_type_name(node: clang.cindex.CursorKind):
    type_name = node.type.get_declaration().type.get_canonical().spelling
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
    return f'S: {node.spelling}, K: {node.kind}, PS: {node.semantic_parent.spelling if node.semantic_parent else ""},' + \
            f'L: {node.lexical_parent.spelling if node.lexical_parent else ""}, T: {node.type.get_declaration().type.spelling}, #T: {node.type.get_num_template_arguments()}'

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
def trim_namesapce(name: str):
    index = find_last_double_colon_before_template(name)

    # preserve std:: namespace
    if index != -1 and not name.startswith("std::") :
        name = name[index+2:]

    # handle template args
    if '<' in name and '>' in name:
        template_arg = name[name.find('<')+1 : name.rfind('>')]
        trimmed_arg = trim_namesapce(template_arg)
        name = name.replace(template_arg, trimmed_arg)

    return name

def verbal(opton, *args):
    if (opton.verbal):
        print(*args)

