#include "CompareQueue.hpp"
#include "FakeNodeFactory.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/rest_n.hpp>


#define CHECK_NODE_LESS(less, more) do {\
	BOOST_CHECK(!compareQueueUnderTest((less), (more))); \
	BOOST_CHECK(compareQueueUnderTest((more), (less))); \
} while (false)

#define CHECK_NODE_ORDER_ELEMENT(r, elem1, elem2) CHECK_NODE_LESS(elem1, elem2);

#define CHECK_NODE_ORDER_INNER_LOOP(r, list, i, elem) \
	BOOST_PP_SEQ_FOR_EACH(CHECK_NODE_ORDER_ELEMENT, elem, \
			BOOST_PP_SEQ_REST_N(i, list))

#define CHECK_NODE_ORDER(nodes) \
	BOOST_PP_SEQ_FOR_EACH_I(CHECK_NODE_ORDER_INNER_LOOP, \
			BOOST_PP_SEQ_TAIL(nodes), nodes)

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
		auto rootNode1 = nodeFactory.createNode({}, 5); // 6
		auto rootNode2 = nodeFactory.createNode({}, 10); // 11
		auto node1 = nodeFactory.createNode(rootNode1, 3); // 5
		auto node2 = nodeFactory.createNode(node1, 7); // 10

		CHECK_NODE_ORDER((node1)(rootNode1)(node2)(rootNode2));
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

		CHECK_NODE_ORDER((rootNode1)(node1)(rootNode2)(node2));
}

BOOST_AUTO_TEST_CASE(costFgv_has_priority_over_time)
{
		auto rootNode1 = nodeFactory.createNode({}, 3);
		auto node1 = nodeFactory.createNode(rootNode1, 2);
		auto rootNode2 = nodeFactory.createNode({}, 1);

		CHECK_NODE_ORDER((rootNode2)(rootNode1)(node1));
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

		CHECK_NODE_ORDER((node2)(rootNode2)(node1)(rootNode1));
}

BOOST_AUTO_TEST_CASE(costFgv_has_priority_over_time)
{
		auto rootNode1 = nodeFactory.createNode({}, 3);
		auto node1 = nodeFactory.createNode(rootNode1, 2);
		auto rootNode2 = nodeFactory.createNode({}, 1);

		CHECK_NODE_ORDER((rootNode2)(node1)(rootNode1));
}

BOOST_AUTO_TEST_SUITE_END() // BackwardTime

struct ForwardHeurFixture: CompareQueueTestFixture {
	CompareQueue compareQueueUnderTest{{{CompareMethod::heur, false}}};
};

BOOST_FIXTURE_TEST_SUITE(ForwardHeur, ForwardHeurFixture)

BOOST_AUTO_TEST_CASE(zero_heur_always_comes_first)
{
	zeroHeurAlwaysComesFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(less_heur_plus_cost_nodes_come_first)
{
	lessHeurPlusCostNodesComeFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(less_heur_nodes_come_first_when_costFgv_is_equal)
{
		auto rootNode1 = nodeFactory.createNode({}, 3); // 4
		auto node1 = nodeFactory.createNode(rootNode1, 2); // 4
		auto rootNode2 = nodeFactory.createNode({}, 4); // 5
		auto node2 = nodeFactory.createNode(node1, 1); // 4
		auto node3 = nodeFactory.createNode(node2, 1); // 5

		CHECK_NODE_ORDER((node2)(node1)(rootNode1)(node3)(rootNode2));
}

BOOST_AUTO_TEST_SUITE_END() // ForwardHeur

struct BackwardHeurFixture: CompareQueueTestFixture {
	CompareQueue compareQueueUnderTest{{{CompareMethod::heur, true}}};
};

BOOST_FIXTURE_TEST_SUITE(BackwardHeur, BackwardHeurFixture)

BOOST_AUTO_TEST_CASE(zero_heur_always_comes_first)
{
	zeroHeurAlwaysComesFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(less_heur_plus_cost_nodes_come_first)
{
	lessHeurPlusCostNodesComeFirst(compareQueueUnderTest);
}

BOOST_AUTO_TEST_CASE(more_heur_nodes_come_first_when_costFgv_is_equal)
{
		auto rootNode1 = nodeFactory.createNode({}, 3); // 4
		auto node1 = nodeFactory.createNode(rootNode1, 2); // 4
		auto node2 = nodeFactory.createNode(node1, 1); // 4
		auto node3 = nodeFactory.createNode(node2, 1); // 5
		auto rootNode2 = nodeFactory.createNode({}, 4); // 5

		CHECK_NODE_ORDER((rootNode1)(node1)(node2)(rootNode2)(node3));
}

BOOST_AUTO_TEST_SUITE_END() // BackwardHeur

BOOST_AUTO_TEST_SUITE_END() // CompareQueueTest


