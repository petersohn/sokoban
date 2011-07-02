#ifndef ARRAY_H_
#define ARRAY_H_

#include "Point.h"
#include <iostream>

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
	T *data;
	size_t width_, height_, size_;
public:
	typedef T valueType;

	Array();
	Array(size_t width, size_t height);
	Array(size_t width, size_t height, const T& defValue);
	Array(const Array &other);
	~Array() {
		if (data != NULL)
			delete[] data;
	}
	Array& operator=(const Array &other);

	T& operator[](size_t pos) {
		return data[pos];
	}
	const T& operator[](size_t pos) const {
		assert(pos >= 0 && pos < size_);
		return data[pos];
	}
	T& operator[](const Point &p) {
		assert(p.x >= 0 && p.y >= 0 && p.x < width_ && p.y < height_);
		return data[p.y*width_ + p.x];
	}
	const T& operator[](const Point &p) const {
		assert(p.x >= 0 && p.y >= 0 && p.x < width_ && p.y < height_);
		return data[p.y*width_ + p.x];
	}
	size_t size() const { return size_; }
	size_t width() const { return width_; }
	size_t height() const { return height_; }
	void reset(size_t newWidth, size_t newHeight);
	void reset(size_t newWidth, size_t newHeight, const T& defValue) {
		reset(newWidth, newHeight);
		fill(defValue);
	}
	void fill(const T &value) {
		for (int i = 0; i < size_; i++)
			data[i] = value;
	}
	void zero() {
		if (data != NULL) {
			delete[] data;
			data = NULL;
		}
	}
};

template<class T>
Array<T>::Array():
	width_(0), height_(0), size_(0), data(NULL)
{

}

template<class T>
Array<T>::Array(size_t width, size_t height):
	width_(width), height_(height), size_(width*height)
{
	data = new T[size_];
}

template<class T>
Array<T>::Array(size_t width, size_t height, const T& defValue):
	width_(width), height_(height), size_(width*height)
{
	data = new T[size_];
	fill(defValue);
}

template<class T>
Array<T>::Array(const Array<T> &other):
	width_(other.width_), height_(other.height_), size_(other.size_)
{
	data = new T[size_];
	for (int i = 0; i < size_; i++)
		data[i] = other[i];
}

template<class T>
Array<T>& Array<T>::operator=(const Array &other) {
	delete[] data;
	width_ = other.width_;
	height_ = other.height_;
	size_ = width_ * height_;
	data = new T[size_];
	for (int i = 0; i < size_; i++)
		data[i] = other[i];
	return *this;
}

template<class T>
void Array<T>::reset(size_t newWidth, size_t newHeight) {
	width_ = newWidth;
	height_ = newHeight;
	size_ = newWidth * newHeight;
	if (data != NULL)
		delete[] data;
	data = new T[size_];
}

template<class T>
inline const T& arrayAt(const Array<T> &arr, const Point &p, const T& def) {
	if (p.x >= 0 && p.y >= 0 && p.x < arr.width() && p.y < arr.height())
			return arr[p];
		return def;
}

#endif /* ARRAY_H_ */
