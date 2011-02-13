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
		std::string title = "", Array<bool> *highlight = NULL);

void dumpNode(std::ostream &file, const Table &table, const Node &node,
		std::string title = "", Array<bool> *highlight = NULL);

template<class T>
std::deque<T> intersect(std::deque<T> l1, std::deque<T> l2)
{
	std::priority_queue<T> q1(l1.begin(), l1.end());
	std::priority_queue<T> q2(l2.begin(), l2.end());
	std::deque<T> result;
	T last;
	bool start = true;
	while (q1.size() > 0 && q2.size() > 0)
	{
		if (q1.top() == q2.top())
		{
			if (start || last != q1.top())
			{
				result.push_front(q1.top());
				last = q1.top();
				start = false;
			}
			q1.pop();
			q2.pop();
		} else if (q1.top() < q2.top())
			q2.pop();
		else
			q1.pop();
	}
	return result;
}


#endif /* COMMON_H_ */
