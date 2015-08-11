#ifndef SRC_PRIOQUEUE_HPP
#define SRC_PRIOQUEUE_HPP

#include <queue>

template <typename T, typename Greater>
class PrioQueue {
    std::priority_queue<T, std::vector<T>, Greater> queue_;
public:
    explicit PrioQueue(Greater greater = Greater{}):
        queue_(std::move(greater))
    {}

    PrioQueue(const PrioQueue&) = default;
    PrioQueue(PrioQueue&&) = default;
    PrioQueue& operator=(const PrioQueue&) = default;
    PrioQueue& operator=(PrioQueue&&) = default;

    void push(const T& t) { queue_.push(t); }

    T pop()
    {
        if (queue_.empty())
            return T{};
        auto result = queue_.top();
        queue_.pop();
        return result;
    }

    T peek() const
    {
        if (queue_.empty())
            return T{};
        auto result = queue_.top();
        return result;
    }

    std::size_t size() const { return queue_.size(); }
};



#endif /* SRC_PRIOQUEUE_HPP */
