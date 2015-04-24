#include "VisitedStates.hpp"
#include <boost/foreach.hpp>

bool VisitedStates::checkAndPush(const VisitedStateInput& elem)
{
    const Status& status = elem.first;
    int heur = elem.second;
    VisitedStateInfo info(status);
    VisitedStateSet::iterator it = visitedStates_.find(info);
    if (it == visitedStates_.end()) {
        visitedStates_.insert(std::make_pair(info, heur));
        return true;
    }
    if (it->second > heur) {
        it->second = heur;
        return true;
    }
    return false;
}

