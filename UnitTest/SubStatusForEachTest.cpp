#include "Dumper/DumperFunctions.hpp"

#include "Status/StatusCreator.hpp"

#include "Array.hpp"
#include "createBoolArray.hpp"
#include "CreateTestStatus.hpp"
#include "MockChecker.hpp"
#include "MockHeurCalculator.hpp"
#include "SubStatusForEach.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/test/unit_test.hpp>

#include <vector>

using namespace sokoban;

struct SubStatusForEachFixture {
    std::pair<std::unique_ptr<Table>, Status> data = createStatus(3, 2, {
            "y.o",
            "x..",
        });
    Table* table = data.first.get();
    std::shared_ptr<MockHeurCalculator> heurCalculator =
        std::make_shared<MockHeurCalculator>();
    std::shared_ptr<MockChecker> checker =
        std::make_shared<MockChecker>();
    boost::asio::io_service ioService;
    MOCK_FUNCTOR(action, void(const Status&, std::size_t));
};

BOOST_FIXTURE_TEST_SUITE(SubStatusForEachTest, SubStatusForEachFixture)

BOOST_AUTO_TEST_CASE(iterate_through_points)
{
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oy.",
                    "..."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "yo.",
                    "..."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "y.o",
                    "..."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "y..",
                    ".o."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "y..",
                    "..o"
                }), mock::any);

    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{0},
            SubStatusForEach::ChokePointDistantNum{0}, {},
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(1, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(iterate_with_correct_indexes)
{
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(mock::any, 0);
    MOCK_EXPECT(action).once().with(mock::any, 1);
    MOCK_EXPECT(action).once().with(mock::any, 2);
    MOCK_EXPECT(action).once().with(mock::any, 3);
    MOCK_EXPECT(action).once().with(mock::any, 4);

    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{0},
            SubStatusForEach::ChokePointDistantNum{0}, {},
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(1, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(ignore_walls)
{
    data = createStatus(3, 2, {
            "yo*",
            "x.*",
        });
    table = data.first.get();
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oy*",
                    "..*"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "yo*",
                    "..*"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "y.*",
                    ".o*"
                }), mock::any);

    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{0},
            SubStatusForEach::ChokePointDistantNum{0}, {},
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(1, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_partitions)
{
    auto data = createStatus(3, 2, {
            "yo.",
            "x*.",
        });
    table = data.first.get();
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oy.",
                    ".*."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "y*."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "yo.",
                    ".*."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oy",
                    ".*."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".yo",
                    ".*."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    ".*y"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "y..",
                    ".*o"
                }), mock::any);

    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{0},
            SubStatusForEach::ChokePointDistantNum{0}, {},
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(1, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_partitions)
{
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oo.",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o.o",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oy.",
                    ".o."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oo",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oy",
                    "..o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "...",
                    "yoo"
                }), mock::any);


    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{0},
            SubStatusForEach::ChokePointDistantNum{0}, {},
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(2, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_distance_limit)
{
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oo.",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oy.",
                    ".o."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oo",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oy",
                    "..o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "...",
                    "yoo"
                }), mock::any);


    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{1},
            SubStatusForEach::ChokePointDistantNum{0}, {},
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(2, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_distance_limit_and_choke_point1)
{
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oo.",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oy.",
                    ".o."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oo",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oy",
                    "..o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "...",
                    "yoo"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o.o",
                    "y.."
                }), mock::any);


    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{1},
            SubStatusForEach::ChokePointDistantNum{1},
            createBoolArray(*table, {Point{2, 0}}),
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(2, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_distance_limit_and_choke_point2)
{
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oo.",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "oy.",
                    ".o."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oo",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".o.",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    ".oy",
                    "..o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "yo."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    ".oy"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "..o",
                    "y.o"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "...",
                    "yoo"
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o.o",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "y.o"
                }), mock::any);


    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{1},
            SubStatusForEach::ChokePointDistantNum{1},
            createBoolArray(*table, {Point{0, 0}}),
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(2, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_minimum_distance)
{
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
    MOCK_EXPECT(checker->check).returns(true);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o.o",
                    "y.."
                }), mock::any);
    MOCK_EXPECT(action).once().with(createTestStatus(*table, {
                    "o..",
                    "y.o"
                }), mock::any);

    SubStatusForEach subStatusForEachUnderTest{*table, action,
            SubStatusForEach::MinDistance{2},
            SubStatusForEach::MaxDistance{0},
            SubStatusForEach::ChokePointDistantNum{0}, {},
            SubStatusForEach::WorkQueueLength{1},
            SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
    subStatusForEachUnderTest.start(2, heurCalculator, ComplexChecker{checker});
    ioService.run();
    subStatusForEachUnderTest.wait(false);
}

BOOST_AUTO_TEST_SUITE_END()

