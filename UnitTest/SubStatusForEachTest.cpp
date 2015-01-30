#include "SubStatusForEach.hpp"
#include "Status/StatusCreator.hpp"
#include "MockHeurCalculator.hpp"
#include "MockChecker.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "CreateTestStatus.hpp"
#include "createBoolArray.hpp"
#include "Array.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/asio/io_service.hpp>
#include <vector>

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
	MOCK_FUNCTOR(action, void(const Status&));
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
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"yo.",
					"..."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"y.o",
					"..."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"y..",
					".o."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"y..",
					"..o"
				}));

	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{0},
			SubStatusForEach::MaxDistance{0},
			SubStatusForEach::ChokePointDistantNum{0}, {},
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(1, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
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
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"yo*",
					"..*"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"y.*",
					".o*"
				}));

	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{0},
			SubStatusForEach::MaxDistance{0},
			SubStatusForEach::ChokePointDistantNum{0}, {},
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(1, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
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
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"y*."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"yo.",
					".*."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oy",
					".*."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".yo",
					".*."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					".*y"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"y..",
					".*o"
				}));

	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{0},
			SubStatusForEach::MaxDistance{0},
			SubStatusForEach::ChokePointDistantNum{0}, {},
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(1, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_partitions)
{
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
	MOCK_EXPECT(checker->check).returns(true);
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oo.",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o.o",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oy.",
					".o."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oo",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oy",
					"..o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"...",
					"yoo"
				}));


	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{0},
			SubStatusForEach::MaxDistance{0},
			SubStatusForEach::ChokePointDistantNum{0}, {},
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(2, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_distance_limit)
{
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
	MOCK_EXPECT(checker->check).returns(true);
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oo.",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oy.",
					".o."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oo",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oy",
					"..o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"...",
					"yoo"
				}));


	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{0},
			SubStatusForEach::MaxDistance{1},
			SubStatusForEach::ChokePointDistantNum{0}, {},
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(2, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_distance_limit_and_choke_point1)
{
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
	MOCK_EXPECT(checker->check).returns(true);
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oo.",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oy.",
					".o."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oo",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oy",
					"..o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"...",
					"yoo"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o.o",
					"y.."
				}));


	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{0},
			SubStatusForEach::MaxDistance{1},
			SubStatusForEach::ChokePointDistantNum{1},
			createBoolArray(*table, {Point{2, 0}}),
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(2, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_distance_limit_and_choke_point2)
{
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
	MOCK_EXPECT(checker->check).returns(true);
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oo.",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"oy.",
					".o."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oo",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".o.",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					".oy",
					"..o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"yo."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					".oy"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"..o",
					"y.o"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"...",
					"yoo"
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o.o",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"y.o"
				}));


	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{0},
			SubStatusForEach::MaxDistance{1},
			SubStatusForEach::ChokePointDistantNum{1},
			createBoolArray(*table, {Point{0, 0}}),
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(2, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
}

BOOST_AUTO_TEST_CASE(multiple_stones_with_minimum_distance)
{
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
	MOCK_EXPECT(checker->check).returns(true);
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o.o",
					"y.."
				}));
	MOCK_EXPECT(action).once().with(createTestStatus(*table, {
					"o..",
					"y.o"
				}));
	MOCK_EXPECT(action).calls([](const Status& status) {
			dumpStatus(std::cerr, status, "this is called");
		});


	SubStatusForEach tableIteratorUnderTest{*table, action,
			SubStatusForEach::MinDistance{2},
			SubStatusForEach::MaxDistance{0},
			SubStatusForEach::ChokePointDistantNum{0}, {},
			SubStatusForEach::WorkQueueLength{1},
			SubStatusForEach::ReverseSearchMaxDepth{0}, ioService};
	tableIteratorUnderTest.start(2, heurCalculator, ComplexChecker{checker});
	ioService.run();
	tableIteratorUnderTest.wait(false);
}

BOOST_AUTO_TEST_SUITE_END()

