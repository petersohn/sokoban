#ifndef EXPANDER_H_
#define EXPANDER_H_

#include <memory>
#include "PrioNodeQueueFwd.hpp"

namespace sokoban {

class Status;
class Node;
class Dumper;

class Expander {
public:
    // returns true if no more expansions should be made
    virtual void expand(const Status&, std::shared_ptr<Node>, PrioNodeQueue&,
            Dumper*) = 0;
    virtual ~Expander() {}

    template <typename Archive>
    void serialize(Archive& /*ar*/, const unsigned int /*version*/)
    {}
};

} // namespace sokoban

#endif /* EXPANDER_H_ */
