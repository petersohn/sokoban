#ifndef ARRAY_H_
#define ARRAY_H_

#include "Status/Point.h"
#include "Status/PointRange.h"
#include "Hash.h"
#include <assert.h>
#include <vector>
#include <boost/range/algorithm.hpp>
#include <type_traits>

//#define NDEBUG
//
//#define assert assert_
//
//inline void assert_(bool expr) {
//	if (!expr) {
//		std::cerr << "bing" << std::endl;
//		exit(1);
//	}
//}

template<class T>
class Array {
	std::size_t width_, height_;
	std::vector<T> data_;
public:
	typedef T valueType;
	typedef typename std::vector<T>::reference reference;
	typedef typename std::vector<T>::const_reference const_reference;

	Array(): width_(0), height_(0) {}
	Array(std::size_t width, std::size_t height, const T& defValue = T()):
		width_(width), height_(height), data_(width * height, defValue)
	{}
	Array(const Array& other):
		width_(other.width_), height_(other.height_), data_(other.data_)
	{}
	Array(Array&& other):
		width_(other.width_), height_(other.height_), data_(std::move(other.data_))
	{}
	Array& operator=(const Array& other)
	{
		width_ = other.width_;
		height_ = other.height_;
		data_ = other.data_;
		return *this;
	}
	Array& operator=(Array&& other)
	 {
		width_ = other.width_;
		height_ = other.height_;
		data_ = std::move(other.data_);
		other.width_ = 0;
		other.height_ = 0;
		return *this;
	}

	reference operator[](std::size_t pos) {
		return data_[pos];
	}
	const_reference operator[](std::size_t pos) const {
		assert(pos >= 0 && pos < size());
		return data_[pos];
	}
	reference operator[](const Point &p) {
		assert(p.x >= 0 && p.y >= 0 && p.x < width_ && p.y < height_);
		return data_[p.y*width_ + p.x];
	}
	const_reference operator[](const Point &p) const {
		assert(p.x >= 0 && p.y >= 0 && p.x < width_ && p.y < height_);
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
	void fill(const T &value)
	{
		boost::fill(data_, value);
	}
	void clear()
	{
		data_.clear();
		width_ = 0;
		height_ = 0;
	}
};

template<class T>
inline const typename Array<T>::const_reference arrayAt(const Array<T> &arr, const Point &p, const T& def) {
	if (p.x >= 0 && p.y >= 0 && p.x < arr.width() && p.y < arr.height())
			return arr[p];
		return def;
}

template <class Array>
inline PointRange arrayRange(const Array& array)
{
	return PointRange(Point(0,0), Point(array.width(), array.height()));
}

namespace std {
template<class T>
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
