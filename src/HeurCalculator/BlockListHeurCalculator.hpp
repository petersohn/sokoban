#ifndef BLOCKLISTHEURCALCULATOR_H_
#define BLOCKLISTHEURCALCULATOR_H_

#include "HeurCalculator/HeurInfo.hpp"
#include "HeurCalculator/SubStatusHeurCalculator.hpp"


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

    template <typename HeurListType>
    explicit BlockListHeurListFactory(const HeurListType& heurList):
        heurList_(std::begin(heurList), std::end(heurList)) {}

    Next operator()() const
    {
        return Next{this};
    }

};

using BlocklistHeurCalculator = SubStatusHeurCalculator<BlockListHeurListFactory>;

#endif /* BLOCKLISTHEURCALCULATOR_H_ */
