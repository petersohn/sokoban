#include "Status/IsStatusPossible.hpp"

#include "Status/Status.hpp"

#include <vector>

namespace sokoban {

class ReverseExpander {

    std::vector<Status> current_;
    std::vector<Status> next_;

    void expandNode(Status status, Point p, Point d)
    {
        Point pd = p+d, pdd = pd+d;
        if (status.reachable(pdd) && pd != status.table().destination())
        {
            if (status.moveStone(p, pd)) {
                status.currentPos(pdd);
                next_.push_back(std::move(status));
            }
        }
    }

    void expandStatus(const Status& status)
    {
        for (const auto& stone: status.state())
        {
            expandNode(status, stone, p10);
            expandNode(status, stone, -p10);
            expandNode(status, stone, p01);
            expandNode(status, stone, -p01);
        }
    }

    bool expandLevel(std::size_t depthRemaining)
    {
        if (depthRemaining == 0) {
            return true;
        }

        for (const auto& status: current_) {
            if (status.reachable(status.table().startingPoint())) {
                return true;
            }

            expandStatus(status);
        }

        if (next_.empty()) {
            return false;
        }

        current_ = std::move(next_);
        next_.clear();

        return expandLevel(depthRemaining - 1);
    }

public:

    bool expand(Status status, std::size_t maxDepth)
    {
        current_ = {std::move(status)};

        return expandLevel(maxDepth);
    }

};


bool isStatusPossible(const Status& status, std::size_t maxDepth)
{
    if (maxDepth == 0) {
        return true;
    }

    return ReverseExpander{}.expand(status, maxDepth);
}

} // namespace sokoban
