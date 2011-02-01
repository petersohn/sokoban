#ifndef ARRAY_H_
#define ARRAY_H_

template<class T>
class Array {
	T *data;
	size_t width, height, size;
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
		return data[pos];
	}
	T& operator[](const Point &p) {
		return data[p.y*width + p.x];
	}
	const T& operator[](const Point &p) const {
		return data[p.y*width + p.x];
	}
	size_t getSize() const { return size; }
	size_t getWidth() const { return width; }
	size_t getHeight() const { return height; }
	void reset(size_t newWidth, size_t newHeight);
	void reset(size_t newWidth, size_t newHeight, const T& defValue) {
		reset(newWidth, newHeight);
		fill(defValue);
	}
	void fill(const T &value) {
		for (int i = 0; i < size; i++)
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
	width(0), height(0), size(0), data(NULL)
{

}

template<class T>
Array<T>::Array(size_t width, size_t height):
	width(width), height(height), size(width*height)
{
	data = new T[size];
}

template<class T>
Array<T>::Array(size_t width, size_t height, const T& defValue):
	width(width), height(height), size(width*height)
{
	data = new T[size];
	fill(defValue);
}

template<class T>
Array<T>::Array(const Array &other):
	width(other.width), height(other.height)
{
	data = new T[size];
	for (int i = 0; i < size; i++)
		data[i] = other[i];
}

template<class T>
Array<T>& Array<T>::operator=(const Array &other) {
	delete[] data;
	width = other.width;
	height = other.height;
	size = width * height;
	data = new T[size];
	for (int i = 0; i < size; i++)
		data[i] = other[i];
	return *this;
}

template<class T>
void Array<T>::reset(size_t newWidth, size_t newHeight) {
	width = newWidth;
	height = newHeight;
	size = newWidth * newHeight;
	if (data != NULL)
		delete[] data;
	data = new T[size];
}

#endif /* ARRAY_H_ */
