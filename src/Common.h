#ifndef COMMON_H_
#define COMMON_H_

#include <exception>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <deque>
#include <queue>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

typedef boost::mutex MutexType;
typedef boost::shared_mutex SharedMutexType;

template<class T> class Array;
class Point;
class Status;
class Node;
class Table;

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

template<class T>
inline std::string toStr(const T &t) {
	return (boost::format("%1%") % t).str();
}

std::string direction(const Point &d);
std::string pointStr(const Point &p);


#endif /* COMMON_H_ */
