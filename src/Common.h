#ifndef COMMON_H_
#define COMMON_H_

#include <exception>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>

template<class T> class Array;
class Point;
class Status;
class Node;

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

void floodFill(const Status &table, const Point &p0, Array<bool> &result,
			std::deque<int> *border = NULL, MinMax *minmax = NULL);

void dumpStatus(std::ostream &file, const Status &status,
		const std::string &title = "", Array<bool> *highlight = NULL);

void dumpNode(std::ostream &file, const Table &table,
		const std::string &title = "", Array<bool> *highlight = NULL);

#endif /* COMMON_H_ */
