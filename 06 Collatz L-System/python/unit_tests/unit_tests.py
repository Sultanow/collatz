import unittest
from graph import *


class TestGenerateTree(unittest.TestCase):

    def test_list(self):
        tree = generateAberkaneTree(14, 1)
        self.check_parent(tree, 31, 47)
        self.check_parent(tree, 127, 191)
        self.check_parent(tree, 511, 767)
        self.check_parent(tree, 17, 13)
        self.check_parent(tree, 41, 31)
        self.check_parent(tree, 65, 49)
        self.check_parent(tree, 89, 67)
        self.check_parent(tree, 113, 85)

    def test_height(self):
        height = 8
        tree = generateAberkaneTree(height, 1)
        self.assertEqual(tree.num_rows, height)

    def test_mark(self):
        tree = generateAberkaneTree(10, 1, 0)
        tree.mark(5, 1)
        self.assertEqual(tree.getNode(5).state, NodeState.ORANGE)
        self.assertEqual(tree.getNode(5).parent.number, 1)

    def test_node_count(self):
        tree = generateAberkaneTree(14, 1)
        self.assertEqual(tree.getNodeCount(), 5948)

        tree = generateAberkaneTree(14, 65)
        self.assertEqual(tree.getNodeCount(), 981)

    def test_get_node(self):
        tree = generateAberkaneTree(14, 1)
        self.assertEqual(tree.getNode(161).number, 161)
        self.assertEqual(tree.getNode(1).number, 1)
        self.assertEqual(tree.getNode(5).number, 5)
        self.assertEqual(tree.getNode(7).number, 7)

    def test_count(self):
        tree = generateAberkaneTree(14, 1, count=0)
        self.assertEqual(tree.getNodeCount(), 1)

        tree = generateAberkaneTree(14, 1, count=1)
        self.assertEqual(tree.getNodeCount(), 3)

        tree = generateAberkaneTree(14, 1, count=2)
        self.assertEqual(tree.getNodeCount(), 4)

        tree = generateAberkaneTree(14, 1, count=10)
        self.assertEqual(tree.getNodeCount(), 18)

    def test_row_length(self):
        tree = generateAberkaneTree(13, 1, count=0)
        self.assertEqual(len(tree.rows[-1]), 2048)

        tree = generateAberkaneTree(14, 1, count=0)
        self.assertEqual(len(tree.rows[-1]), 4096)

        tree = generateAberkaneTree(15, 1, count=0)
        self.assertEqual(len(tree.rows[-1]), 8192)

    def test_marked(self):
        tree = generateAberkaneTree(10, 161, count=10)
        self.assertEqual(tree.getNode(31).state, 2)

    def check_parent(self, tree, number, parent):
        node = tree.getNode(number)
        self.assertTrue(node is not None)
        self.assertEqual(node.parent.number, parent)

class TestRules(unittest.TestCase):

    def test_types(self):
        self.assertTrue(isTypeA(2))
        self.assertTrue(isTypeB(3))
        self.assertTrue(isTypeC(1))

    def test_rule_two(self):
        self.assertTrue(ruleTwo(11))
        self.assertFalse(ruleTwo(13))
        self.assertTrue(ruleTwo(161))
        self.assertTrue(ruleTwo(12345))

    def test_actions(self):
        self.assertEqual(G(5), 9)
        self.assertEqual(S(17), 35)
        self.assertEqual(V(15), 61)

    def test_rules(self):
        self.assertEqual(Ra(23), 15)
        self.assertEqual(Rb(11), 59)
        self.assertEqual(Rc(25), 33)

unittest.main()
