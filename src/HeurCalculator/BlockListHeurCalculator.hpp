#ifndef HEURCALCULATOR_BLOCKLISTHEURCALCULATOR_HPP
#define HEURCALCULATOR_BLOCKLISTHEURCALCULATOR_HPP

#include "HeurCalculator/BlockListHeurCalculatorFwd.hpp"
#include "HeurCalculator/HeurInfo.hpp"
#include "HeurCalculator/SubStatusHeurCalculator.hpp"

#include <boost/serialization/utility.hpp>
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

namespace sokoban {

class BlockListHeurListFactory {
    HeurList heurList_;
public:
    friend class Next;
    class Next {
        const BlockListHeurListFactory* owner_;
        HeurList::const_iterator iterator_;
    public:
        explicit Next(const BlockListHeurListFactory* owner):
            owner_(owner),
            iterator_(owner->heurList_.begin())
        {}

        const HeurInfo* operator()(const PseudoStatus&)
        {
            return iterator_ == owner_->heurList_.end() ?
                    nullptr : &*(iterator_++);
        }
    };

    BlockListHeurListFactory() = default;

    template <typename HeurListType>
    explicit BlockListHeurListFactory(const HeurListType& heurList):
        heurList_(std::begin(heurList), std::end(heurList)) {}

    Next operator()() const
    {
        return Next{this};
    }

    template <typename Archive>
    void save(Archive& ar, const unsigned int /*version*/) const {
        std::vector<std::pair<std::shared_ptr<Status>, float>> v;
        v.reserve(heurList_.size());

        for (const auto& element: heurList_) {
            v.emplace_back(std::make_shared<Status>(element.first), element.second);
        }

        ar << v;
    }

    template <typename Archive>
    void load(Archive& ar, const unsigned int /*version*/) {
        std::vector<std::pair<std::shared_ptr<Status>, float>> v;
        ar >> v;

        heurList_.reserve(v.size());
        for (const auto& element: v) {
            heurList_.emplace_back(*element.first, element.second);
        }
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

} // namespace sokoban

#endif // HEURCALCULATOR_BLOCKLISTHEURCALCULATOR_HPP
