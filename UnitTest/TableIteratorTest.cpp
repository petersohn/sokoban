#include "TableIterator.hpp"
#include "Status/StatusCreator.hpp"
#include "MockHeurCalculator.hpp"
#include "MockChecker.hpp"
#include "Dumper/DumperFunctions.hpp"
#include <boost/test/unit_test.hpp>
#include <vector>

struct TableIteratorFixture {
	std::shared_ptr<MockHeurCalculator> heurCalculator =
		std::make_shared<MockHeurCalculator>();
	std::shared_ptr<MockChecker> checker =
		std::make_shared<MockChecker>();
	boost::asio::io_service ioService;
	MOCK_FUNCTOR(action, void(const Status&));

	Status statusWithStones(const Table& table, Point currentPos,
			const std::vector<Point>& stones)
	{
		Status status{table};
		for (const auto& stone: stones) {
			status.addStone(stone);
		}
		status.currentPos(currentPos);
		return status;
	}
};

BOOST_FIXTURE_TEST_SUITE(TableIteratorTest, TableIteratorFixture)

BOOST_AUTO_TEST_CASE(iterate_through_points)
{
	auto data = createStatus(3, 2, {
			"y.o",
			"x..",
		});
	const auto& table = *data.first;
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
	MOCK_EXPECT(checker->check).returns(true);
	MOCK_EXPECT(action).once().with(statusWithStones(table, Point{1, 0},
				{Point{0, 0}}));
	MOCK_EXPECT(action).once().with(statusWithStones(table, Point{0, 0},
				{Point{1, 0}}));
	MOCK_EXPECT(action).once().with(statusWithStones(table, Point{0, 0},
				{Point{2, 0}}));
	MOCK_EXPECT(action).once().with(statusWithStones(table, Point{0, 0},
				{Point{1, 1}}));
	MOCK_EXPECT(action).once().with(statusWithStones(table, Point{0, 0},
				{Point{2, 1}}));

	TableIterator tableIteratorUnderTest{table, action, 1, 1, 0, ioService};
	tableIteratorUnderTest.start(1, heurCalculator, checker);
	ioService.run();
	tableIteratorUnderTest.wait(false);
}

BOOST_AUTO_TEST_SUITE_END()

