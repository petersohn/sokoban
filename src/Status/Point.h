#ifndef POINT_H_
#define POINT_H_

#include <stddef.h>
#include <boost/format.hpp>
#include "Hash.h"

class Point
{
public:
	int x, y;
	Point():x(0), y(0) { }
	explicit Point(int x, int y):x(x),y(y) {}
	Point(const Point &other):
		x(other.x),
		y(other.y)
	{ }
	Point& operator=(const Point &other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	Point &operator+=(const Point &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Point &operator-=(const Point &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Point &operator*=(int n)
	{
		x *= n;
		y *= n;
		return *this;
	}

	static const Point p10, pm10, p01, p0m1;

};


inline bool operator==(const Point &p1, const Point &p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=(const Point &p1, const Point &p2) {
	return !(p1 == p2);
}

inline Point operator+(const Point &p1, const Point &p2) {
	return Point(p1.x+p2.x, p1.y+p2.y);
}

inline Point operator-(const Point &p1, const Point &p2) {
	return Point(p1.x-p2.x, p1.y-p2.y);
}

inline Point operator*(const Point &p, int n) {
	return Point(p.x*n, p.y*n);
}

inline Point operator-(const Point &p) {
	return p * -1;
}

inline bool operator<(const Point &p1, const Point &p2) {
	return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
}

inline std::string direction(const Point &d)
 {
	return
			d.x > 0 ? "right" :
			d.x < 0 ? "left" :
			d.y > 0 ? "down" :
			d.y < 0 ? "up" : "???";
}

inline std::string pointStr(const Point &p)
{
	return (boost::format("(%d, %d)") % p.x % p.y).str();
}

inline std::ostream& operator<<(std::ostream& os, const Point& p)
{
	os << pointStr(p);
	return os;
}



namespace std {

template<>
struct hash<Point> {
	size_t operator()(const Point &p) const
	{
		size_t seed = 0;
		hash_combine(seed, p.x);
		hash_combine(seed, p.y);
		return seed;
	}
};


}


#endif /*POINT_H_*/
