#ifndef SRC_COMPAREQUEUE_HPP
#define SRC_COMPAREQUEUE_HPP

#include "Compare.hpp"
#include "Node.hpp"

#include <boost/serialization/vector.hpp>

#include <utility>
#include <vector>

namespace sokoban {

class CompareQueue {
private:
    std::vector<Compare> compares;
public:
    CompareQueue() = default;

    CompareQueue(std::vector<Compare> compares):
        compares(std::move(compares))
    {
    }

    CompareQueue(const CompareQueue&) = default;
    CompareQueue& operator=(const CompareQueue&) = default;
    CompareQueue(CompareQueue&&) = default;
    CompareQueue& operator=(CompareQueue&&) = default;

    bool operator()(const std::shared_ptr<Node>& first,
            const std::shared_ptr<Node>& second) {

        if (first->heur() == 0 && second->heur() != 0) {
            return false;
        }

        if (second->heur() == 0 && first->heur() != 0) {
            return true;
        }

        int value = first->costFgv() - second->costFgv();
        for (const auto& compare: compares) {
            if (value != 0) {
                break;
            }

            Node* lhs;
            Node* rhs;

            if (compare.reverse) {
                lhs = second.get();
                rhs = first.get();
            } else {
                lhs = first.get();
                rhs = second.get();
            }

            switch (compare.type) {
            case CompareMethod::heur:
                value = lhs->heur() - rhs->heur();
                break;
            case CompareMethod::depth:
                value = lhs->depth() - rhs->depth();
                break;
            case CompareMethod::time:
                value = lhs->time() - rhs->time();
                break;
            }
        }

        return value > 0;
    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & compares;
    }
};

} // namespace sokoban

#endif /* SRC_COMPAREQUEUE_HPP */
