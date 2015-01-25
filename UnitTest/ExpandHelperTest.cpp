#include "ExpandHelper.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"
#include "MockHeurCalculator.hpp"
#include "MockChecker.hpp"
#include "NodeFactory.hpp"
#include "CreateTestStatus.hpp"
#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

BOOST_AUTO_TEST_SUITE(ExpandHelperTest)

struct ExpandStatusFixture {
	~ExpandStatusFixture()
	{
		mock::verify();
		mock::reset();
	}
};

BOOST_FIXTURE_TEST_SUITE(ExpandStatus, ExpandStatusFixture)

MOCK_FUNCTION(mockExpandFunction, 2, void(Point, Point));

BOOST_AUTO_TEST_CASE(expandStatus_expand_one_stone)
{
	auto data = createStatus(4, 3, {
			"x...",
			".o..",
			"y..."});

	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(-1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(0, 1));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(0, -1));

	expandStatus(data.second, mockExpandFunction);
}

BOOST_AUTO_TEST_CASE(expandStatus_expand_more_stones)
{
	auto data = createStatus(4, 3, {
			"xo..",
			"...o",
			"y..."});

	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(-1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(0, 1)); MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(0, -1)); MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(1, 0)); MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(-1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(0, 1));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(0, -1));

	expandStatus(data.second, mockExpandFunction);
}

BOOST_AUTO_TEST_SUITE_END() // ExpandStatus

struct CreateNodeFixture {
	std::shared_ptr<MockHeurCalculator> heurCalculator =
			std::make_shared<MockHeurCalculator>();
	MockChecker checker;
	NodeFactory nodeFactory{heurCalculator};
	std::pair<std::unique_ptr<Table>, Status> data = createStatus(4, 3, {
			"xo..",
			"..o.",
			".oy."});

	const std::shared_ptr<Node> emptyNode;
};

BOOST_FIXTURE_TEST_SUITE(CreateNode, CreateNodeFixture)

BOOST_AUTO_TEST_CASE(create_one_node)
{
	auto expectedStatus = createTestStatus(*data.first, {
			"xo..",
			".oy.",
			".o.."});
	int heur = 43;
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).
			returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).with(expectedStatus, mock::any,
			emptyNode).returns(heur);
	MOCK_EXPECT(checker.check).with(expectedStatus, Point{1, 1}).returns(true);

	auto node = createNode(data.second, Point{2, 1}, Point{-1, 0}, emptyNode,
			nodeFactory, *heurCalculator, checker, nullptr);
	BOOST_REQUIRE(node);
	BOOST_CHECK_EQUAL(node->heur(), heur);
	BOOST_CHECK_EQUAL(node->ancestor(), emptyNode);
	BOOST_CHECK_EQUAL(node->moveDescriptor().from_, (Point{2, 1}));
	BOOST_CHECK_EQUAL(node->moveDescriptor().d_, (Point{-1, 0}));
	BOOST_CHECK_EQUAL((Status{*data.first, *node}), expectedStatus);
}

BOOST_AUTO_TEST_CASE(create_more_nodes)
{
	auto intermediateStatus = createTestStatus(*data.first, {
			"xo..",
			".oy.",
			".o.."});
	auto expectedStatus = createTestStatus(*data.first, {
			"xo..",
			"oy..",
			".o.."});
	int heur = 10;
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).
			returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).with(intermediateStatus, mock::any,
			emptyNode).returns(20);
	MOCK_EXPECT(checker.check).with(intermediateStatus, Point{1, 1}).returns(true);

	auto node1 = createNode(data.second, Point{2, 1}, Point{-1, 0}, emptyNode,
			nodeFactory, *heurCalculator, checker, nullptr);

	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{0, 1}).
			returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).with(expectedStatus, mock::any,
			node1).returns(heur);
	MOCK_EXPECT(checker.check).with(expectedStatus, Point{0, 1}).returns(true);

	auto node2 = createNode(intermediateStatus, Point{1, 1}, Point{-1, 0}, node1,
			nodeFactory, *heurCalculator, checker, nullptr);

	BOOST_REQUIRE(node1);
	BOOST_REQUIRE(node2);
	BOOST_CHECK_EQUAL(node2->heur(), heur);
	BOOST_CHECK_EQUAL(node2->ancestor(), node1);
	BOOST_CHECK_EQUAL(node2->moveDescriptor().from_, (Point{1, 1}));
	BOOST_CHECK_EQUAL(node2->moveDescriptor().d_, (Point{-1, 0}));
	BOOST_CHECK_EQUAL((Status{*data.first, *node2}), expectedStatus);
}

BOOST_AUTO_TEST_CASE(no_node_if_heur_calculator_returns_negative_for_stone)
{
	auto expectedStatus = createTestStatus(*data.first, {
			"xo..",
			".oy.",
			".o.."});
	int heur = 43;
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).
			returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStatus).with(expectedStatus, mock::any,
			emptyNode).returns(heur);
	MOCK_EXPECT(checker.check).with(expectedStatus, Point{1, 1}).returns(true);

	auto node = createNode(data.second, Point{2, 1}, Point{-1, 0}, emptyNode,
			nodeFactory, *heurCalculator, checker, nullptr);
	BOOST_CHECK(!node);
}

BOOST_AUTO_TEST_CASE(no_node_if_checker_returns_false)
{
	auto expectedStatus = createTestStatus(*data.first, {
			"xo..",
			".oy.",
			".o.."});
	int heur = 43;
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).
			returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).with(expectedStatus, mock::any,
			emptyNode).returns(heur);
	MOCK_EXPECT(checker.check).with(expectedStatus, Point{1, 1}).returns(false);

	auto node = createNode(data.second, Point{2, 1}, Point{-1, 0}, emptyNode,
			nodeFactory, *heurCalculator, checker, nullptr);
	BOOST_CHECK(!node);
}

BOOST_AUTO_TEST_SUITE_END() // CreateNode

BOOST_AUTO_TEST_SUITE_END() //ExpandHelperTest


