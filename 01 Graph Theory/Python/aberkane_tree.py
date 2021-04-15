from __future__ import annotations
import math
import networkx as nx
import matplotlib.pyplot as plt
import pydot
from networkx.drawing.nx_pydot import graphviz_layout


def V(num: int) -> int:
    return 4 * num + 1


def S(num: int) -> int:
    return 2 * num + 1


def G(num: int) -> int:
    return 2 * num - 1


def IV(num: int) -> int:
    if (num - 1) % 4 == 0 and ((num - 1) // 4) % 2 == 1:
        return (num - 1) // 4
    else:
        return None


def IS(num: int) -> int:
    if (num - 1) % 2 == 0 and ((num - 1) // 2) % 2 == 1:
        return (num - 1) // 2
    else:
        return None


def IG(num: int) -> int:
    if (num + 1) % 2 == 0 and ((num + 1) // 2) % 2 == 1:
        return (num + 1) // 2
    else:
        return None


def Ra(num: int) -> int:
    return (2 * num - 1) // 3


def Rb(num: int) -> int:
    return 16 * num // 3 + 1


def Rc(num: int) -> int:
    return (4 * num - 1) // 3


def isTypeA(num: int) -> bool:
    return num % 3 == 2


def isTypeB(num: int) -> bool:
    return num % 3 == 0


def isTypeC(num: int) -> bool:
    return num % 3 == 1


def ruleTwo(num: int) -> bool:
    result = (num - 1) / 2
    k = 0
    while result % 2 != 0:
        result = (result - 1) / 2
        k += 1
    r = result / 2
    return ((k) % 2 == 0 and r % 2 == 0) or ((k) % 2 != 0 and r % 2 != 0)


def getStartNumbers(start: int) -> List[int]:
    start_numbers = [start]

    if start == 1:
        return start_numbers

    number = start
    while number % 24 != 17:

        number = int((3 * number + 1) / 2)

        if number % 2 == 0:
            while number % 2 == 0:
                number = int(number / 2)

        if number not in start_numbers:
            start_numbers.append(number)

    return start_numbers


class Tree:
    def __init__(self, num_rows: int):
        self.num_rows = num_rows
        self.rows = []
        self.generate()

    def generate(self) -> None:
        for i in range(self.num_rows):
            row = []
            for j in range(2**i, 2**(i + 1)):
                if j % 2 == 1:
                    row.append(Node(j))
            self.rows.append(row)

    def getNode(self, number: int) -> Node:
        row = Node.getRow(number)
        if row < self.num_rows:
            return self.rows[row][Node.getCol(row, number)]
        return None

    def mark(self, number: int) -> None:
        node = self.getNode(number)
        if node is not None and node.state != NodeState.BLUE:
            node.state = NodeState.ORANGE
            parent = self.getNode(Node.getParent(number))
            if parent is not None and parent.state != NodeState.BLACK:
                node.connect(parent)

    def printTree(self) -> None:
        for row in reversed(self.rows):
            for node in row:
                print("{0}-{1}".format(node.number, node.state), end=" ")
            print('')

    def getWidth(self, row: int = None) -> int:
        if row is None:
            return self.getWidth(self.num_rows - 1)
        return (2**row + 1) // 2

    def getNodeCount(self) -> int:
        node_count = 0
        for row in self.rows:
            for node in row:
                if node.state != NodeState.BLACK:
                    node_count += 1
        return node_count


class NodeState:
    BLACK = 1
    ORANGE = 2
    BLUE = 3


class Node:
    def __init__(self, number: int, parent: Node = None):
        self.row = Node.getRow(number)
        self.state = NodeState.BLACK
        self.number = number
        self.parent = parent

    def connect(self, parent: Node = None) -> None:
        self.parent = parent

    @staticmethod
    def getRow(number: int) -> int:
        return int(math.log(number, 2))

    @staticmethod
    def getCol(row: int, number: int) -> int:
        return (number - (2**row + 1)) // 2

    @staticmethod
    def getParent(num: int) -> int:
        if num == 17:
            return 1
        result = IS(num)
        if result is not None:
            return result
        result = IV(num)
        if result is not None:
            return result
        result = IG(num)
        if result is not None:
            return result
        return None


def generateTree(height: int, count: int, start: int = 1) -> None:
    tree = Tree(height)

    for start_num in getStartNumbers(start):
        node = tree.getNode(start_num)
        if node is not None:
            node.state = NodeState.ORANGE
        else:
            print("Warning: Could not add number {0} to start number".format(
                start_num))

    for r in range(count):
        for number in range(1, 2**height, 2):
            node = tree.getNode(number)
            if node is None:
                break

            if node.state == NodeState.ORANGE:

                tree.mark(V(number))

                if ruleTwo(number):
                    tree.mark(S(number))

                if isTypeA(number):
                    tree.mark(Ra(number))

                if isTypeC(number):
                    tree.mark(Rc(number))

                if node.state == NodeState.ORANGE:
                    node.state = NodeState.BLUE

                break

    return tree


def getPos(tree: Tree, number: int) -> tuple[float, float]:
    size = 1
    row = Node.getRow(number)
    tree_width = tree.getWidth()
    width = tree.getWidth(row)
    node_pos = Node.getCol(row, number)

    space = float(tree_width) / float(width) * size
    pos_x = (float(Node.getCol(row, number)) / float(width)) * float(
        tree_width * size) + space / 2.0

    return (pos_x, float(row))


def getColor(node: Node) -> String:
    if node.state == NodeState.ORANGE:
        return '#ffa500'
    elif node.state == NodeState.BLUE:
        return '#ffa500'
        # return '#0000ff'
    else:
        return '#000000'


def generatePlot(tree: Tree) -> None:
    graph = nx.Graph()

    pos = {}
    node_colors = []
    edge_colors = []

    for row in tree.rows:
        for node in row:
            if node.number == 1:
                pos[node.number] = (getPos(tree, 3)[0], 0)  # TODO
            else:
                pos[node.number] = getPos(tree, node.number)
            graph.add_node(node.number)
            if node.number != 1:
                graph.add_edge(Node.getParent(node.number), node.number)

    for number in graph:
        node_colors.append(getColor(tree.getNode(number)))

    for parent, node in graph.edges():
        n = tree.getNode(node)
        if n.parent is not None:
            edge_colors.append('#000000')
        else:
            edge_colors.append('#FFFFFF')

    w = 2 + float(tree.getWidth()) * 0.5
    h = 2 + tree.num_rows * 0.5

    fig = plt.figure(figsize=(w, h))

    nx.draw_networkx_labels(graph, pos=pos, font_color='w', font_size=4)

    nx.draw(graph,
            pos=pos,
            node_color=node_colors,
            node_shape="s",
            node_size=250,
            with_labels=False,
            linewidths=1,
            font_color='g',
            edge_color=edge_colors)

height = 8  # The height of the tree
start_number = 31  # The number to start with

tree = generateTree(height, 2**height, start_number)

generatePlot(tree)

print(tree.getNodeCount())

plt.savefig('tree.svg')
