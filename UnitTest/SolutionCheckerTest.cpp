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
		auto data = createStatus(5, 6, {
				"xo...",
				"....*",
				"**.**",
				".....",
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

	struct NodeData {
		State state;
		MoveDescriptor moveDescriptor;
	};

	std::deque<std::shared_ptr<Node>> createNodes(
			const std::vector<NodeData>& nodeDatas)
	{
		std::deque<std::shared_ptr<Node>> result;
		std::shared_ptr<Node> previousNode;
		for (const auto& nodeData: nodeDatas) {
			result.push_back(createNode(
					nodeData.state, nodeData.moveDescriptor, previousNode));
			previousNode = result.back();
		}

		return result;
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
				".....",
				"y.*.."});

	std::deque<std::shared_ptr<Node>> nodes{createNodes({
		NodeData{
			State{Point{2, 1}},
			MoveDescriptor{Point{2, 2}, Point{0, -1}},
		},
		NodeData{
			State{Point{2, 0}},
			MoveDescriptor{Point{2, 1}, Point{0, -1}},
		},
		NodeData{
			State{Point{1, 0}},
			MoveDescriptor{Point{2, 0}, Point{-1, 0}},
		},
		NodeData{
			State{},
			MoveDescriptor{Point{1, 0}, Point{-1, 0}},
		}
		})};
	BOOST_CHECK(solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(good_solution_with_multiple_stones)
{
	auto initialStatus = createTestStatus(*table, {
				"x....",
				"....*",
				"**o**",
				"..o..",
				".....",
				"y.*.."});

	std::deque<std::shared_ptr<Node>> nodes{createNodes({
		NodeData{
			State{Point{2, 2}, Point{3, 3}},
			MoveDescriptor{Point{2, 3}, Point{1, 0}},
		},
		NodeData{
			State{Point{2, 1}, Point{3, 3}},
			MoveDescriptor{Point{2, 2}, Point{0, -1}},
		},
		NodeData{
			State{Point{2, 0}, Point{3, 3}},
			MoveDescriptor{Point{2, 1}, Point{0, -1}},
		},
		NodeData{
			State{Point{1, 0}, Point{3, 3}},
			MoveDescriptor{Point{2, 0}, Point{-1, 0}},
		},
		NodeData{
			State{Point{3, 3}},
			MoveDescriptor{Point{1, 0}, Point{-1, 0}},
		},
		NodeData{
			State{Point{2, 3}},
			MoveDescriptor{Point{3, 3}, Point{-1, 0}},
		},
		NodeData{
			State{Point{2, 2}},
			MoveDescriptor{Point{2, 3}, Point{0, -1}},
		},
		NodeData{
			State{Point{2, 1}},
			MoveDescriptor{Point{2, 2}, Point{0, -1}},
		},
		NodeData{
			State{Point{2, 0}},
			MoveDescriptor{Point{2, 1}, Point{0, -1}},
		},
		NodeData{
			State{Point{1, 0}},
			MoveDescriptor{Point{2, 0}, Point{-1, 0}},
		},
		NodeData{
			State{},
			MoveDescriptor{Point{1, 0}, Point{-1, 0}},
		}
		})};
	BOOST_CHECK(solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_SUITE_END()


