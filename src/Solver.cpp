#include "Solver.hpp"

#include "Dumper/Dumper.hpp"
#include "Dumper/DumperFunctions.hpp"

#include "Status/Status.hpp"

#include "Expander.hpp"
#include "Node.hpp"
#include "PrioNodeQueue.hpp"

#include <util/ThreadPool.hpp>

#include <memory>

class InternalSolver {
    PrioNodeQueue queue_;
    std::shared_ptr<Expander> expander_;
    std::shared_ptr<Dumper> dumper_;
    int costFgv_;
    std::shared_ptr<Node> currentNode_;
public:
    InternalSolver(PrioNodeQueue queue,
            std::shared_ptr<Expander> expander,
            std::shared_ptr<Dumper> dumper):
        queue_(std::move(queue)),
        expander_(std::move(expander)),
        dumper_(dumper),
        costFgv_(-1)
    {
        assert(expander_.get() != nullptr);
    }

    bool expandSerial(Status& status)
    {
        expander_->expand(status, currentNode_, queue_, dumper_);
        currentNode_ = queue_.pop();
        if (!currentNode_) {
            return false;
        }
        status.set(*currentNode_);
        return true;
    }

    std::deque<std::shared_ptr<Node>> solve(Status& status) {
        costFgv_ = -1;
        currentNode_.reset();

        if (dumper_) {
            dumper_->initialStatus(status);
        }

        do {
            if (!expandSerial(status)) {
                break;
            }
        } while (currentNode_->heur() > 0);

        std::deque<std::shared_ptr<Node>> result;
        if (currentNode_ && currentNode_->heur() == 0) {
            result = pathToRoot(currentNode_);
        }

        if (dumper_) {
            for (const auto& node: result) {
                dumper_->addToSolution(node);
            }
            dumper_->save();
        }
        return result;
    }

};

std::deque<std::shared_ptr<Node>> Solver::solve(Status status) const
{
    InternalSolver solver(
            queueFactory_(),
            expanderFactory_(status),
            dumperFactory_());
    return solver.solve(status);

}

