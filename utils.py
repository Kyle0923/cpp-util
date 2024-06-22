import clang.cindex
import graphviz

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

def print_ancestors(parent_dict: dict, classes: list|str):
    if not classes:
        classes = [p for p in parent_dict.keys() if all(p not in parent for parent in parent_dict.values())]
    print("#######################################################")
    print("printing ancestors")
    print('============================')
    print_tree(parent_dict, classes, "", "ancestors")
    print("#######################################################")
    print()

def print_descendants(child_dict: dict, classes: list|str):
    if not classes:
        classes = [c for c in child_dict.keys() if all(c not in children for children in child_dict.values())]
    print("#######################################################")
    print("printing descendants")
    print('============================')
    print_tree(child_dict, classes, "", "descendants")
    print("#######################################################")
    print()

def print_tree(connection: dict, nodes: list|str, indent: str = "", msg: str = ""):
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
def graph_report(parent_dict: dict, query, out_file: str, args):
    dot = graphviz.Digraph()
    dot.node_attr["shape"] = "box"
    dot.node_attr["style"] = "rounded"
    inserted = {}

    for node in query:
        dot.node(node, style="filled, rounded", fillcolor="turquoise")

    if not query:
        # print all nodes and edges
        generate_graph(parent_dict, None, parent_dict.keys(), dot, inserted, args)
    else:
        child_dict = find_descendants(parent_dict)
        generate_graph(parent_dict, child_dict, query, dot, inserted, args)

    dot.render(out_file, format='pdf')
    print("use https://dreampuf.github.io/GraphvizOnline/ to view graph")
    print(f"graph file is at ./{out_file}")

# generate a graph view of the class hierarchy using Graphviz
def generate_graph(parent_dict: dict, child_dict: dict, nodes: str | list, dot: graphviz.Digraph, inserted: dict, args):

    if isinstance(nodes, str):
        nodes = [nodes]

    for curr_node in nodes:
        # travese towards base
        if parent_dict and args.base:
            for other_node in parent_dict.get(curr_node, []):
                edge_key = f"{other_node}->{curr_node}"
                if edge_key in inserted:
                    # TODO: verify if we could miss some case with `continue` here
                    continue

                insert_to_dot(dot, other_node, curr_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, args)
                else:
                    # don't need the child of the parent
                    generate_graph(parent_dict, None, other_node, dot, inserted, args)

        # travese towards derived
        if child_dict and args.derived:
            for other_node in child_dict.get(curr_node, []):
                inserted[other_node] = True
                edge_key = f"{curr_node}->{other_node}"
                if edge_key in inserted:
                    # TODO: verify if we could miss some case with `continue` here
                    continue

                insert_to_dot(dot, curr_node, other_node, edge_key, inserted)
                if args.connected:
                    generate_graph(parent_dict, child_dict, other_node, dot, inserted, args)
                else:
                    # don't need the parent of the child
                    generate_graph(None, child_dict, other_node, dot, inserted, args)

def insert_to_dot(dot: graphviz.Digraph, src: str, dest: str, edge_key: str, inserted: dict):
    src_name = insert_node_to_dot(dot, src, inserted)
    dest_name = insert_node_to_dot(dot, dest, inserted)
    inserted[edge_key] = True
    dot.edge(src_name, dest_name)

# insert to dot if not exist
# and return the escaped name of the node
def insert_node_to_dot(dot: graphviz.Digraph, node: str, inserted: dict) -> str:
    node_name = node
    if "::" in node_name:
        node_name = node_name.replace("::", "__")
        if node not in inserted:
            dot.node(node_name, node)
    inserted[node] = True
    return node_name

##############################################################################################################
# clang symbol handling
##############################################################################################################

# return the fully quilified name with the complete namespace
def get_full_type_name(node: clang.cindex.CursorKind):
    return node.type.get_declaration().type.spelling

# print AST for debug
def print_ast(node, level=0):
    if (level == 1):
        print("==========================")

    if (level > 0):
        print('    ' * (level - 1) + f'K: {node.kind}, S: {node.spelling}, D: {node.displayname}, S: {node.semantic_parent.spelling if node.semantic_parent else ""},' +
                f'L: {node.lexical_parent.spelling if node.lexical_parent else ""}, T:{node.type.get_declaration().type.spelling}')

    for child in node.get_children():
        print_ast(child, level + 1)
