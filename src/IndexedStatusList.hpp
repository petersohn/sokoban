#ifndef SRC_INDEXEDSTATUSLIST_HPP
#define SRC_INDEXEDSTATUSLIST_HPP

#include <memory>
#include <vector>
#include <boost/thread/locks.hpp>
#include "Status/Status.hpp"

class IndexedStatusList {
private:
    Array<std::vector<Status>> index_;
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
};

#endif /* SRC_INDEXEDSTATUSLIST_HPP */
