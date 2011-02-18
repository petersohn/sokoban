#include "StonePusher.h"

bool StonePusher::pushStones(const Status &st) {
	Array<bool> destReachable(st.width(), st.height(), false);
	bool touched;
	bool touched2 = false;
	Node::Ptr node;
	do {
		touched = false;
		std::deque<int> destBorder;
		floodFill(status, destination, destReachable, &destBorder);
		std::deque<int> border = intersect(status.border(), destBorder);
		for (std::deque<int>::const_iterator it = border.begin();
				it != border.end(); ++it) {
			node = pushStone(status, *it);
			if (tmp.get() != NULL) {
				maxDepth_ = std::max(state->depth, maxDepth);
				touched = true;
			}
		}
		if (touched)
			touched2 = true;

	} while (touched);
	if (touched2 && !stateVisited())
	{
		visitedStates.insert(pushList.begin(), pushList.end());
		queue.push(state);
		return true;
	}
	else
		return false;
}



