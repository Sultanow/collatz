from __future__ import annotations
import math


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


class AberkaneTree:
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

    def mark(self, number: int, parent: int) -> Tuple[int, int]:
        node = self.getNode(number)
        if node is not None and node.state != NodeState.BLUE:
            node.state = NodeState.ORANGE
            parent_node = self.getNode(parent)
            if parent_node is not None and parent_node.state != NodeState.BLACK:
                node.connect(parent_node)
                return (number, parent)
            return (number, None)
        return (None, None)

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


def generateAberkaneTree(height: int, start: int = 1, count: int = -1, progress_callback: Callable = None) -> None:
    if count < 0:
        count = 2**height

    tree = AberkaneTree(height)

    start_node = tree.getNode(start)
    if start_node is not None:
        start_node.state = NodeState.ORANGE
    else:
        print(
            "Warning: Could not add number {0} to start number".format(start_num))

    marked_nodes = [start_node]

    while len(marked_nodes) > 0:
        next_nodes = []
        for node in marked_nodes:
            if count <= 0:
                break

            if node.state != NodeState.ORANGE:
                continue

            results = calculateNode(tree, node)
            count -= 1

            filtered_results = list(
                filter(lambda e: e[0] is not None and e[0] != node.number, results))
            next_nodes.extend(
                list(map(lambda e: tree.getNode(e[0]), filtered_results)))
            if progress_callback is not None:
                progress_callback(filtered_results)
        marked_nodes.clear()
        marked_nodes.extend(next_nodes)
        marked_nodes.sort(key=lambda e: e.number)

        if count <= 0:
            break

    return tree


def calculateNextNode(tree: AberkaneTree) -> list[tuple[int, int]]:
    for number in range(1, 2**tree.num_rows, 2):
        node = tree.getNode(number)
        if node is None:
            break

        if node.state == NodeState.ORANGE:
            return calculateNode(tree, node)
    return None


def calculateNode(tree: AberkaneTree, node: Node) -> list[tuple[int, int]]:
    results = []
    if node.state == NodeState.ORANGE:
        connection = tree.mark(V(node.number), node.number)
        results.append(connection)

        if ruleTwo(node.number):
            connection = tree.mark(S(node.number), node.number)
            results.append(connection)

        if isTypeA(node.number):
            connection = tree.mark(Ra(node.number), node.number)
            results.append(connection)

        if isTypeC(node.number):
            connection = tree.mark(Rc(node.number), node.number)
            results.append(connection)

        if node.state == NodeState.ORANGE:
            node.state = NodeState.BLUE

    return results
