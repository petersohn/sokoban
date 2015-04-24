#include "SolutionChecker.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"
#include "CreateTestStatus.hpp"
#include "Node.hpp"
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
                ".....",
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

BOOST_AUTO_TEST_SUITE(good_solutions)

BOOST_AUTO_TEST_CASE(one_stone)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
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

BOOST_AUTO_TEST_CASE(one_stone_more_complicated_moves)
{
    auto initialStatus = createTestStatus(*table, {
                "x.y..",
                ".....",
                "**o**",
                ".....",
                ".....",
                "..*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{2, 3}},
            MoveDescriptor{Point{2, 2}, Point{0, 1}},
        },
        NodeData{
            State{Point{2, 4}},
            MoveDescriptor{Point{2, 3}, Point{0, 1}},
        },
        NodeData{
            State{Point{3, 4}},
            MoveDescriptor{Point{2, 4}, Point{1, 0}},
        },
        NodeData{
            State{Point{3, 3}},
            MoveDescriptor{Point{3, 4}, Point{0, -1}},
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

BOOST_AUTO_TEST_CASE(two_stones)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
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

BOOST_AUTO_TEST_SUITE_END() // good_solutions

BOOST_AUTO_TEST_SUITE(invalid_moves)

BOOST_AUTO_TEST_CASE(stone_is_moved_too_much)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
                "**o**",
                ".....",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{2, 0}},
            MoveDescriptor{Point{2, 2}, Point{0, -2}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_diagonally)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
                "**.**",
                ".....",
                "...o.",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{2, 3}},
            MoveDescriptor{Point{3, 4}, Point{-1, -1}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_not_moved)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
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
            State{Point{2, 1}},
            MoveDescriptor{Point{2, 1}, Point{0, 0}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_into_another)
{
    auto initialStatus = createTestStatus(*table, {
                "xoo..",
                ".....",
                "**.**",
                ".....",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{1, 0}},
            MoveDescriptor{Point{2, 0}, Point{-1, 0}},
        },
        NodeData{
            State{},
            MoveDescriptor{Point{1, 0}, Point{-1, 0}},
        }
        })};
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(the_finishing_state_is_not_empty)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
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
        }
        })};
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_in_invalid_direction)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".o...",
                "**.**",
                ".....",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{1, 0}},
            MoveDescriptor{Point{1, 1}, Point{0, -1}},
        },
        NodeData{
            State{},
            MoveDescriptor{Point{1, 0}, Point{-1, 0}},
        }
        })};
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_through_wall1)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
                "**.**",
                "...o.",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{3, 2}},
            MoveDescriptor{Point{3, 3}, Point{0, -1}},
        },
        NodeData{
            State{Point{3, 1}},
            MoveDescriptor{Point{3, 2}, Point{0, -1}},
        },
        NodeData{
            State{Point{3, 0}},
            MoveDescriptor{Point{3, 1}, Point{0, -1}},
        },
        NodeData{
            State{Point{2, 0}},
            MoveDescriptor{Point{3, 0}, Point{-1, 0}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_through_wall2)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
                "**.**",
                "...o.",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{3, 2}},
            MoveDescriptor{Point{3, 3}, Point{0, -1}},
        },
        NodeData{
            State{Point{3, 1}},
            MoveDescriptor{Point{3, 2}, Point{0, -1}},
        },
        NodeData{
            State{Point{2, 1}},
            MoveDescriptor{Point{3, 1}, Point{-1, 0}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_moved_from_unreachable_point)
{
    auto initialStatus = createTestStatus(*table, {
                "x.y..",
                ".....",
                "**o**",
                ".....",
                ".....",
                "..*.."});

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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_out_of_the_table_up)
{
    auto initialStatus = createTestStatus(*table, {
                "x.o..",
                "y....",
                "**.**",
                ".....",
                ".....",
                "..*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{2, -1}},
            MoveDescriptor{Point{2, 0}, Point{0, -1}},
        },
        NodeData{
            State{Point{1, -1}},
            MoveDescriptor{Point{2, -1}, Point{-1, 0}},
        },
        NodeData{
            State{Point{0, -1}},
            MoveDescriptor{Point{1, -1}, Point{-1, 0}},
        },
        NodeData{
            State{},
            MoveDescriptor{Point{0, -1}, Point{0, 1}},
        }
        })};
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_out_of_the_table_left)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                "o...y",
                "**.**",
                ".....",
                ".....",
                "..*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{-1, 1}},
            MoveDescriptor{Point{0, 1}, Point{-1, 0}},
        },
        NodeData{
            State{Point{-1, 0}},
            MoveDescriptor{Point{-1, 1}, Point{0, -1}},
        },
        NodeData{
            State{},
            MoveDescriptor{Point{-1, 0}, Point{1, 0}},
        }
        })};
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_out_of_the_table_right)
{
    auto initialStatus = createTestStatus(*table, {
                "x...o",
                "y....",
                "**.**",
                ".....",
                ".....",
                "..*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{5, 0}},
            MoveDescriptor{Point{4, 0}, Point{1, 0}},
        },
        NodeData{
            State{Point{4, 0}},
            MoveDescriptor{Point{5, 0}, Point{-1, 0}},
        },
        NodeData{
            State{Point{3, 0}},
            MoveDescriptor{Point{4, 0}, Point{-1, 0}},
        },
        NodeData{
            State{Point{2, 0}},
            MoveDescriptor{Point{3, 0}, Point{-1, 0}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(stone_is_moved_out_of_the_table_down)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                "y....",
                "**.**",
                ".....",
                ".....",
                ".o*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{1, 6}},
            MoveDescriptor{Point{1, 5}, Point{0, 1}},
        },
        NodeData{
            State{Point{1, 5}},
            MoveDescriptor{Point{1, 6}, Point{0, -1}},
        },
        NodeData{
            State{Point{1, 4}},
            MoveDescriptor{Point{1, 5}, Point{0, -1}},
        },
        NodeData{
            State{Point{1, 3}},
            MoveDescriptor{Point{1, 4}, Point{0, -1}},
        },
        NodeData{
            State{Point{2, 3}},
            MoveDescriptor{Point{1, 3}, Point{1, 0}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_SUITE_END() // invalid_moves

BOOST_AUTO_TEST_SUITE(inconsistent_nodes)

BOOST_AUTO_TEST_CASE(wrong_move_descriptor_source_point)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
                "**o**",
                ".....",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{2, 1}},
            MoveDescriptor{Point{2, 3}, Point{0, -1}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(wrong_move_descriptor_destination_point)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
                "**o**",
                ".....",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{2, 1}},
            MoveDescriptor{Point{2, 2}, Point{0, 1}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(wrong_state)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
                "**o**",
                ".....",
                ".....",
                "y.*.."});

    std::deque<std::shared_ptr<Node>> nodes{createNodes({
        NodeData{
            State{Point{2, 0}},
            MoveDescriptor{Point{2, 2}, Point{0, -1}},
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
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(step_is_missing_from_the_end)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
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
            State{},
            MoveDescriptor{Point{1, 0}, Point{-1, 0}},
        }
        })};
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_CASE(step_is_missing_from_the_middle)
{
    auto initialStatus = createTestStatus(*table, {
                "x....",
                ".....",
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
            State{Point{1, 0}},
            MoveDescriptor{Point{2, 0}, Point{-1, 0}},
        },
        NodeData{
            State{},
            MoveDescriptor{Point{1, 0}, Point{-1, 0}},
        }
        })};
    BOOST_CHECK(!solutionCheckerUnderTest.checkResult(initialStatus, nodes));
}

BOOST_AUTO_TEST_SUITE_END() // inconsistent_nodes

BOOST_AUTO_TEST_SUITE_END() // SolutionCheckerTest


