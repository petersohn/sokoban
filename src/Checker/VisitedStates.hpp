#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "Status/State.hpp"
#include "Checker/VisitedStateInfo.hpp"
#include "Mutexes.hpp"

#include <boost/serialization/unordered_map.hpp>

#include <unordered_map>

class Status;

typedef std::pair<const Status&, int> VisitedStateInput;

class VisitedStates {
    typedef std::unordered_map<
            VisitedStateInfo, int, std::hash<VisitedStateInfo>> VisitedStateSet;
    VisitedStateSet visitedStates_;
public:
    VisitedStates() = default;

    bool checkAndPush(const VisitedStateInput& elem);
    size_t size() const { return visitedStates_.size(); }
    bool empty() const { return visitedStates_.empty(); }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & visitedStates_;
    }
};

#endif /* VISITEDSTATES_H_ */
