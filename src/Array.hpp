#ifndef ARRAY_H_
#define ARRAY_H_

#include "Status/Point.hpp"
#include "Status/PointRange.hpp"

#include "Hash.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/serialization/vector.hpp>

#include <assert.h>
#include <type_traits>
#include <vector>

template <typename Array>
inline bool isInsideArray(const Array& array, Point p)
{
    return p.x >= 0 && p.y >= 0 && p.x < static_cast<int>(array.width()) &&
            p.y < static_cast<int>(array.height());
}

template<typename T>
class Array {
    typedef std::vector<T> Data;
    std::size_t width_, height_;
    Data data_;
public:
    typedef T valueType;
    typedef typename Data::reference reference;
    typedef typename Data::const_reference const_reference;
    typedef typename Data::iterator iterator;
    typedef typename Data::const_iterator const_iterator;

    Array(): width_(0), height_(0) {}
    Array(std::size_t width, std::size_t height, const T& defValue = T()):
        width_(width), height_(height), data_(width * height, defValue)
    {}
    Array(const Array& ) = default;
    Array(Array&& ) = default;
    Array& operator=(const Array& ) = default;
    Array& operator=(Array&& ) = default;

    reference operator[](std::size_t pos) {
        return data_[pos];
    }
    const_reference operator[](std::size_t pos) const {
        assert(pos >= 0 && pos < size());
        return data_[pos];
    }
    reference operator[](Point p) {
        assert(isInsideArray(*this, p));
        return data_[p.y*width_ + p.x];
    }
    const_reference operator[](Point p) const {
        assert(isInsideArray(*this, p));
        return data_[p.y*width_ + p.x];
    }
    std::size_t size() const { return data_.size(); }
    std::size_t width() const { return width_; }
    std::size_t height() const { return height_; }
    void reset(std::size_t newWidth, std::size_t newHeight, const T& defValue = T())
    {
        width_ = newWidth;
        height_ = newHeight;
        data_.resize(width_ * height_);
        fill(defValue);
    }
    void fill(const T& value)
    {
        boost::fill(data_, value);
    }
    void clear()
    {
        data_.clear();
        width_ = 0;
        height_ = 0;
    }

    bool operator==(const Array<T>& other) const
    {
        return data_ == other.data_;
    }

    iterator begin() { return data_.begin(); }
    iterator end() { return data_.end(); }
    const_iterator begin() const { return data_.begin(); }
    const_iterator end() const { return data_.end(); }
    const_iterator cbegin() const { return data_.cbegin(); }
    const_iterator cend() const { return data_.cend(); }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & width_;
        ar & height_;
        ar & data_;
    }
};

template<typename T>
inline const typename Array<T>::const_reference arrayAt(const Array<T> &arr, Point p, const T& def)
{
    return isInsideArray(arr, p) ? arr[p] : def;
}

template <typename Array>
inline PointRange arrayRange(const Array& array)
{
    return PointRange(Point(0,0), Point(array.width(), array.height()));
}

template<typename T>
inline bool operator!=(const Array<T>& lhs, const Array<T>& rhs)
{
    return !(lhs == rhs);
}

namespace std {
template<typename T>
struct hash<Array<T>> {
    size_t operator()(const Array<T>& arr) const
    {
        size_t seed = 0;
        for (size_t i = 0; i < arr.size(); ++i) {
            hash_combine(seed, arr[i]);
        }
        return seed;
    }
};

}

#endif /* ARRAY_H_ */
