#ifndef CHECKER_BLOCKLISTCHECKER_HPP
#define CHECKER_BLOCKLISTCHECKER_HPP

#include "Checker/Checker.hpp"
#include "IndexedStatusList.hpp"

#include <boost/serialization/base_object.hpp>

class BlockListChecker: public Checker {
private:
    IndexedStatusList blockList_;
public:
    BlockListChecker() = default;

    BlockListChecker(IndexedStatusList blockList):
            blockList_(std::move(blockList))
    {
    }

    bool check(const Status& status, Point  p) const override;
    const char* errorMessage() const override;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & boost::serialization::base_object<Checker>(*this);
        ar & blockList_;
    }
};


#endif // CHECKER_BLOCKLISTCHECKER_HPP
