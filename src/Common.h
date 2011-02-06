#ifndef COMMON_H_
#define COMMON_H_

#include <exception>
#include <boost/shared_ptr.hpp>

template<class T> class Array;
class Point;

class SokobanException:public std::exception
{
	virtual const char* what() const throw()
	{
		return "Sokoban exception";
	}
};

struct MinMax {
	int minX, maxX, minY, maxY;
};

template<class T>
class FixedObject {
	T value_;
public:
	typedef boost::shared_ptr<const FixedObject<T> > Ptr;
	typedef T value_type;

	FixedObject(const T &val): value_(val) {}
	const T &get() const { return value_; }
};


enum FieldType { ftFloor, ftWall, ftStone };

void floodFill(const Array<FieldType> &table, const Point &p0, Array<bool> &result,
			std::deque<int> *border = NULL, MinMax *minmax = NULL);

#endif /* COMMON_H_ */
