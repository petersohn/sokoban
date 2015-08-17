#ifndef SRC_PRIOQUEUE_HPP
#define SRC_PRIOQUEUE_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/priority_queue.hpp>

#include <functional>
#include <memory>
#include <queue>

template <typename T, typename Greater>
class PrioQueue {
    Greater greater;
    std::priority_queue<T, std::vector<T>,
            std::reference_wrapper<Greater>> queue;
public:
    explicit PrioQueue(Greater greater = Greater{}):
        greater(std::move(greater)),
        queue(std::ref(this->greater))
    {}

    PrioQueue(const PrioQueue&) = default;
    PrioQueue(PrioQueue&&) = default;
    PrioQueue& operator=(const PrioQueue&) = default;
    PrioQueue& operator=(PrioQueue&&) = default;

    void push(const T& t) { queue.push(t); }

    T pop()
    {
        if (queue.empty())
            return T{};
        auto result = queue.top();
        queue.pop();
        return result;
    }

    T peek() const
    {
        if (queue.empty())
            return T{};
        auto result = queue.top();
        return result;
    }

    const Greater& getGreater() const { return greater; }

    std::size_t size() const { return queue.size(); }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & queue;
    }
};

template <typename Archive, typename T, typename Greater>
void save_construct_data(Archive& ar, const PrioQueue<T, Greater>* queue,
        const unsigned int /*version*/)
{
    auto greater = std::make_unique<Greater>(queue->getGreater());
    ar << greater;
}

template <typename Archive, typename T, typename Greater>
void load_construct_data(Archive& ar, PrioQueue<T, Greater>* queue,
        const unsigned int /*version*/)
{
    std::unique_ptr<Greater> greater;
    ar >> greater;
    ::new(queue)PrioQueue<T, Greater>{std::move(*greater)};
}

#endif /* SRC_PRIOQUEUE_HPP */
