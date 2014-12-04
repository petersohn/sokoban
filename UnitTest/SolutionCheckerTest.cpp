#include "SolutionChecker.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"
#include "CreateTestStatus.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>

struct SolutionCheckerTestFixture {
	std::ostringstream stream;
	SolutionChecker solutionCheckerUnderTest{stream, stream};
	std::unique_ptr<Table> table;
	int time = 0;

	SolutionCheckerTestFixture()
	{
		auto data = createStatus(5, 5, {
				"xo...",
				"....*",
				"**.**",
				".....",
				"y.*.."});
		table = std::move(data.first);
	}

	~SolutionCheckerTestFixture()
	{
		BOOST_MESSAGE(stream.str());
	}

	std::shared_ptr<Node> createNode(State state, MoveDescriptor moveDescriptor,
			std::shared_ptr<Node> ancestor)
	{
		return std::make_shared<Node>(state, moveDescriptor, ancestor,
				1, 0, 0, 1);
	}
};

BOOST_FIXTURE_TEST_SUITE(SolutionCheckerTest, SolutionCheckerTestFixture)

BOOST_AUTO_TEST_CASE(good_solution)
{
	auto initialStatus = createTestStatus(*table, {
				"x....",
				"....*",
				"**o**",
				".....",
				"y.*.."});

	auto node1 = createNode(
			State{Point{2, 1}},
			MoveDescriptor{Point{2, 2}, Point{0, -1}},
			std::shared_ptr<Node>{}
		);
	auto node2 = createNode(
			State{Point{2, 0}},
			MoveDescriptor{Point{2, 1}, Point{0, -1}},
			node1
		);
	auto node3 = createNode(
			State{Point{1, 0}},
			MoveDescriptor{Point{2, 0}, Point{-1, 0}},
			node2
		);
	auto node4 = createNode(
			State{},
			MoveDescriptor{Point{1, 0}, Point{-1, 0}},
			node3
		);
	std::deque<std::shared_ptr<Node>> nodes{node1, node2, node3, node4};
	BOOST_CHECK(solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_SUITE_END()


