#ifndef INDEXEDSTATUSLIST_HPP
#define INDEXEDSTATUSLIST_HPP

#include "Status/Status.hpp"

#include <memory>
#include <vector>

namespace sokoban {

class IndexedStatusList {
private:
    Matrix<std::vector<Status>> index_;
    size_t size_;

public:
    IndexedStatusList():size_(0) {}
    IndexedStatusList(const IndexedStatusList&) = default;
    IndexedStatusList& operator=(const IndexedStatusList&) = default;
    IndexedStatusList(IndexedStatusList&&) = default;
    IndexedStatusList& operator=(IndexedStatusList&&) = default;

    void add(const Status& status);

    void clear() {
        index_.reset(0, 0);
        size_ = 0;
    }

    bool hasSubStatus(const Status& status, Point p) const;

    size_t size() const { return size_; }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & index_;
        ar & size_;
    }
};

} // namespace sokoban

#endif // INDEXEDSTATUSLIST_HPP
