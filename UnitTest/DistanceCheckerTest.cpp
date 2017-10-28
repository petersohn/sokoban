#include "Checker/DistanceChecker.hpp"

#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include "CreateTestStatus.hpp"
#include "MockHeurCalculator.hpp"
#include "Node.hpp"

#include <boost/test/unit_test.hpp>

using namespace sokoban;

class DistanceCheckerFixture {
public:
    std::shared_ptr<MockHeurCalculator> heurCalculator =
            std::make_shared<MockHeurCalculator>();
    DistanceChecker distanceCheckerUnderTest{heurCalculator};

    DistanceCheckerFixture() {
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{0, 0}).returns(4);
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{1, 0}).returns(3);
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{2, 0}).returns(2);
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{3, 0}).returns(1);
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{0, 1}).returns(3);
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{1, 1}).returns(2);
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{2, 1}).returns(1);
        MOCK_EXPECT(heurCalculator->calculateStone).
                with(mock::any, Point{3, 1}).returns(0);
    }
};

BOOST_FIXTURE_TEST_SUITE(DistanceCheckerTest, DistanceCheckerFixture)

BOOST_AUTO_TEST_CASE(allow_single_input)
{
    auto data = createStatus(4, 2, {
            "y.o.",
            "...x"
        });
    Status& status = data.second;
    Node node{status.state(), {Point{1, 0}, Point{2, 0}}, {}, 1, 2, 0, 0};

    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status, node), true);
}

BOOST_AUTO_TEST_CASE(allow_single_input_with_multiple_stones)
{
    auto data = createStatus(4, 2, {
            "y.o.",
            "o..x"
        });
    Status& status = data.second;
    Node node{status.state(), {Point{1, 0}, Point{2, 0}}, {}, 1, 2, 0, 0};

    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status, node), true);
}

BOOST_AUTO_TEST_CASE(allow_input_if_stone_is_moved_farther)
{
    auto data = createStatus(4, 2, {
            "y.o.",
            "...x"
        });
    auto table = *data.first;
    Status& status1 = data.second;
    auto node1 = std::make_shared<Node>(status1.state(),
            MoveDescriptor{Point{1, 0}, Point{2, 0}},
            std::shared_ptr<Node>{}, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status1, *node1), true);

    auto status2 = createTestStatus(table, {
            "yo..",
            "...x"
            });
    auto node2 = std::make_shared<Node>(status2.state(),
            MoveDescriptor{Point{2, 0}, Point{1, 0}}, node1, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status2, *node2), true);
}

BOOST_AUTO_TEST_CASE(do_not_allow_input_if_stone_is_moved_closer)
{
    auto data = createStatus(4, 2, {
            "y.o.",
            "...x"
        });
    auto table = *data.first;
    Status& status1 = data.second;
    auto node1 = std::make_shared<Node>(status1.state(),
            MoveDescriptor{Point{1, 0}, Point{2, 0}},
            std::shared_ptr<Node>{}, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status1, *node1), true);

    auto status2 = createTestStatus(table, {
            "y..o",
            "...x"
            });
    auto node2 = std::make_shared<Node>(status2.state(),
            MoveDescriptor{Point{2, 0}, Point{3, 0}}, node1, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status2, *node2), false);
}

BOOST_AUTO_TEST_CASE(allow_input_if_nearer_stone_is_moved_farther)
{
    auto data = createStatus(4, 2, {
            "y.o.",
            "o..x"
        });
    auto table = *data.first;
    Status& status1 = data.second;
    auto node1 = std::make_shared<Node>(status1.state(),
            MoveDescriptor{Point{1, 0}, Point{2, 0}},
            std::shared_ptr<Node>{}, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status1, *node1), true);

    auto status2 = createTestStatus(table, {
            "yo..",
            "o..x"
            });
    auto node2 = std::make_shared<Node>(status2.state(),
            MoveDescriptor{Point{2, 0}, Point{1, 0}}, node1, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status2, *node2), true);
}

BOOST_AUTO_TEST_CASE(do_not_allow_input_if_nearer_stone_is_moved_closer)
{
    auto data = createStatus(4, 2, {
            "y.o.",
            "o..x"
        });
    auto table = *data.first;
    Status& status1 = data.second;
    auto node1 = std::make_shared<Node>(status1.state(),
            MoveDescriptor{Point{1, 0}, Point{2, 0}},
            std::shared_ptr<Node>{}, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status1, *node1), true);

    auto status2 = createTestStatus(table, {
            "y..o",
            "o..x"
            });
    auto node2 = std::make_shared<Node>(status2.state(),
            MoveDescriptor{Point{2, 0}, Point{3, 0}}, node1, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status2, *node2), false);
}

BOOST_AUTO_TEST_CASE(allow_input_if_farther_stone_is_moved_farther)
{
    auto data = createStatus(4, 2, {
            "..oy",
            ".o.x"
        });
    auto table = *data.first;
    Status& status1 = data.second;
    auto node1 = std::make_shared<Node>(status1.state(),
            MoveDescriptor{Point{1, 0}, Point{2, 0}},
            std::shared_ptr<Node>{}, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status1, *node1), true);

    auto status2 = createTestStatus(table, {
            "..o.",
            "oy.x"
            });
    auto node2 = std::make_shared<Node>(status2.state(),
            MoveDescriptor{Point{1, 1}, Point{0, 1}}, node1, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status2, *node2), true);
}

BOOST_AUTO_TEST_CASE(allow_input_if_farther_stone_is_moved_closer)
{
    auto data = createStatus(4, 2, {
            "y..o",
            ".o.x"
        });
    auto table = *data.first;
    Status& status1 = data.second;
    auto node1 = std::make_shared<Node>(status1.state(),
            MoveDescriptor{Point{1, 0}, Point{2, 0}},
            std::shared_ptr<Node>{}, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status1, *node1), true);

    auto status2 = createTestStatus(table, {
            "...o",
            "y.ox"
            });
    auto node2 = std::make_shared<Node>(status2.state(),
            MoveDescriptor{Point{1, 1}, Point{3, 1}}, node1, 1, 2, 0, 0);
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status2, *node2), true);
}

BOOST_AUTO_TEST_CASE(allow_empty_input)
{
    auto data = createStatus(4, 2, {
            "y...",
            "..ox"
        });
    Status& status = data.second;
    status.removeStone(Point{2, 1});
    Node node{status.state(), {Point{2, 1}, Point{3, 1}}, {}, 1, 0, 0, 0};

    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status, node), true);
}

BOOST_AUTO_TEST_CASE(allow_empty_input_after_non_empty_input)
{
    auto data = createStatus(4, 2, {
            "y...",
            "..ox"
        });
    Status& status = data.second;
    Node node1{status.state(), {Point{2, 1}, Point{3, 1}}, {}, 1, 1, 0, 0};
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status, node1), true);

    status.removeStone(Point{2, 1});
    Node node2{status.state(), {Point{2, 1}, Point{3, 1}}, {}, 1, 0, 0, 0};
    BOOST_CHECK_EQUAL(distanceCheckerUnderTest.check(status, node2), true);
}

BOOST_AUTO_TEST_SUITE_END()

