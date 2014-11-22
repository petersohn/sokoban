#include "CompareQueue.hpp"
#include "FakeNodeFactory.hpp"
#include <boost/test/unit_test.hpp>

#define CHECK_NODE_LESS(less, more) do {\
	BOOST_CHECK_EQUAL(compareQueueUnderTest((less), (more)), false);\
	BOOST_CHECK_EQUAL(compareQueueUnderTest((more), (less)), true);\
} while (false)

struct CompareQueueTestFixture {
	FakeNodeFactory nodeFactory;
	void zeroHeurAlwaysComesFirst(CompareQueue& compareQueueUnderTest)
	{
		auto zeroHeurRootNode = nodeFactory.createNode({}, 0);
		auto nonZeroHeurRootNode = nodeFactory.createNode({}, 5);
		auto zeroHeurNode = nodeFactory.createNode(nonZeroHeurRootNode, 0);
		auto nonZeroHeurNode = nodeFactory.createNode(nonZeroHeurRootNode, 3);

		CHECK_NODE_LESS(zeroHeurRootNode, nonZeroHeurRootNode);
		CHECK_NODE_LESS(zeroHeurRootNode, nonZeroHeurNode);
		CHECK_NODE_LESS(zeroHeurNode, nonZeroHeurRootNode);
		CHECK_NODE_LESS(zeroHeurNode, nonZeroHeurNode);
	}

	void lessHeurPlusCostNodesComeFirst(CompareQueue& compareQueueUnderTest)
	{
		auto rootNode1 = nodeFactory.createNode({}, 5); // 5
		auto rootNode2 = nodeFactory.createNode({}, 10); // 10
		auto node1 = nodeFactory.createNode(rootNode1, 3); // 4
		auto node2 = nodeFactory.createNode(node1, 7); // 9

		CHECK_NODE_LESS(node1, rootNode1);
		CHECK_NODE_LESS(node1, rootNode2);
		CHECK_NODE_LESS(node1, node2);
		CHECK_NODE_LESS(rootNode1, node2);
		CHECK_NODE_LESS(rootNode1, rootNode2);
		CHECK_NODE_LESS(node2, rootNode2);
	}

};

BOOST_AUTO_TEST_SUITE(CompareQueueTest)

struct EmptyQueueFixture: CompareQueueTestFixture {
	CompareQueue compareQueueUnderTest;
};

BOOST_FIXTURE_TEST_SUITE(EmptyQueue, EmptyQueueFixture)

BOOST_AUTO_TEST_CASE(zero_heur_always_comes_first)
{
	zeroHeurAlwaysComesFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(less_heur_plus_cost_nodes_come_first)
{
	lessHeurPlusCostNodesComeFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_SUITE_END() // EmptyQueue

struct ForwardTimeFixture: CompareQueueTestFixture {
	CompareQueue compareQueueUnderTest{{{CompareMethod::time, false}}};
};

BOOST_FIXTURE_TEST_SUITE(ForwardTime, ForwardTimeFixture)

BOOST_AUTO_TEST_CASE(zero_heur_always_comes_first)
{
	zeroHeurAlwaysComesFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(less_heur_plus_cost_nodes_come_first)
{
	lessHeurPlusCostNodesComeFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(earlier_nodes_come_first_when_costFgv_is_equal)
{
		auto rootNode1 = nodeFactory.createNode({}, 3);
		auto node1 = nodeFactory.createNode(rootNode1, 2);
		auto rootNode2 = nodeFactory.createNode({}, 3);
		auto node2 = nodeFactory.createNode(node1, 1);

		CHECK_NODE_LESS(rootNode1, node1);
		CHECK_NODE_LESS(rootNode1, rootNode2);
		CHECK_NODE_LESS(rootNode1, node2);
		CHECK_NODE_LESS(node1, rootNode2);
		CHECK_NODE_LESS(node1, node2);
		CHECK_NODE_LESS(rootNode2, node2);
}

BOOST_AUTO_TEST_CASE(costFgv_has_priority_over_time)
{
		auto rootNode1 = nodeFactory.createNode({}, 3);
		auto node1 = nodeFactory.createNode(rootNode1, 2);
		auto rootNode2 = nodeFactory.createNode({}, 1);

		CHECK_NODE_LESS(rootNode2, rootNode1);
		CHECK_NODE_LESS(rootNode2, node1);
		CHECK_NODE_LESS(rootNode1, node1);
}

BOOST_AUTO_TEST_SUITE_END() // ForwardTime

struct BackwardTimeFixture: CompareQueueTestFixture {
	CompareQueue compareQueueUnderTest{{{CompareMethod::time, true}}};
};

BOOST_FIXTURE_TEST_SUITE(BackwardTime, BackwardTimeFixture)

BOOST_AUTO_TEST_CASE(zero_heur_always_comes_first)
{
	zeroHeurAlwaysComesFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(less_heur_plus_cost_nodes_come_first)
{
	lessHeurPlusCostNodesComeFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(later_nodes_come_first_when_costFgv_is_equal)
{
		auto rootNode1 = nodeFactory.createNode({}, 3);
		auto node1 = nodeFactory.createNode(rootNode1, 2);
		auto rootNode2 = nodeFactory.createNode({}, 3);
		auto node2 = nodeFactory.createNode(node1, 1);

		CHECK_NODE_LESS(node2, rootNode1);
		CHECK_NODE_LESS(node2, node1);
		CHECK_NODE_LESS(node2, rootNode2);
		CHECK_NODE_LESS(rootNode2, rootNode1);
		CHECK_NODE_LESS(rootNode2, node1);
		CHECK_NODE_LESS(node1, rootNode1);
}

BOOST_AUTO_TEST_CASE(costFgv_has_priority_over_time)
{
		auto rootNode1 = nodeFactory.createNode({}, 3);
		auto node1 = nodeFactory.createNode(rootNode1, 2);
		auto rootNode2 = nodeFactory.createNode({}, 1);

		CHECK_NODE_LESS(rootNode2, rootNode1);
		CHECK_NODE_LESS(rootNode2, node1);
		CHECK_NODE_LESS(node1, rootNode1);
}

BOOST_AUTO_TEST_SUITE_END() // BackwardTime

BOOST_AUTO_TEST_SUITE_END() // CompareQueueTest


