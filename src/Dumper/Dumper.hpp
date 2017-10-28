#ifndef DUMPER_H_
#define DUMPER_H_

#include <memory>

namespace sokoban {

class Status;
class Node;

class Dumper {
public:
    virtual void initialStatus(const Status& status) = 0;
    virtual void addNode(const std::shared_ptr<Node>& node) = 0;
    virtual void addToSolution(const std::shared_ptr<Node>& node) = 0;
    virtual void expand(const std::shared_ptr<Node>& node) = 0;
    virtual void startPushing(const std::shared_ptr<Node>& node) = 0;
    virtual void push(const std::shared_ptr<Node>& node) = 0;
    virtual void reject(const std::shared_ptr<Node>& node, const char *text) = 0;
    virtual void save() = 0;
    virtual ~Dumper() {}
};

} // namespace sokoban

#endif /* DUMPER_H_ */
