#ifndef SRC_PRIONODEQUEUEFWD_HPP
#define SRC_PRIONODEQUEUEFWD_HPP

#include <memory>
#include <functional>

namespace sokoban {

class Node;
class CompareQueue;
template <typename T, typename Greater> class PrioQueue;

using PrioNodeQueue = PrioQueue<std::shared_ptr<Node>, CompareQueue>;
using QueueFactory = std::function<PrioNodeQueue()>;

} // namespace sokoban

#endif /* SRC_PRIONODEQUEUEFWD_HPP */
