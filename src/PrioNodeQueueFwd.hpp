#ifndef SRC_PRIONODEQUEUEFWD_HPP
#define SRC_PRIONODEQUEUEFWD_HPP

#include <memory>
#include <functional>

class Node;
class CompareQueue;
template <typename T, typename Greater> class PrioQueue;

using PrioNodeQueue = PrioQueue<std::shared_ptr<Node>, CompareQueue>;
using QueueFactory = std::function<std::shared_ptr<PrioNodeQueue>()>;


#endif /* SRC_PRIONODEQUEUEFWD_HPP */
