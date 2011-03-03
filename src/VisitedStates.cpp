#include "VisitedStates.h"

void VisitedStates::push(const VisitedStateInput &elem) {
	const Status &st = elem.first;
	visitedStates_.insert(std::make_pair(st.state(), VisitedStateInfo(st.currentPos(), elem.second)));
}

bool VisitedStates::hasElem(const VisitedStateInput &elem) const {
	const Status &status = elem.first;
	int heur = elem.second;
	std::pair<VisitedStateSet::const_iterator,
		VisitedStateSet::const_iterator> found =
			visitedStates_.equal_range(status.state());
	if (found.first != found.second)
	{
		for (VisitedStateSet::iterator it = found.first;
				it != found.second; ++it) {
			if (status.reachable(it->second.currentPos())) {
				if (heur < it->second.heur()) {
				} else {
					return true;
				}
			}
		}
	}
	return false;
}
