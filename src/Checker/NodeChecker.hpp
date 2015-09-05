#ifndef NODECHECKER_HPP
#define NODECHECKER_HPP

#include <string>

class Status;
class Node;

class NodeChecker {
public:
    virtual bool check(const Status& status, Node& node) = 0;
    virtual ~NodeChecker() {}
    // for efficiency reasons, it is assumed that it will
    // return a *real* string constant, or at least
    // remain valid until the next call
    virtual const char* errorMessage() const = 0;

    template <typename Archive>
    void serialize(Archive& /*ar*/, const unsigned int /*version*/) {}
};

#endif // NODECHECKER_HPP

