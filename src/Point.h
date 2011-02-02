#ifndef POINT_H_
#define POINT_H_

class Point
{
public:
	int x, y;
	Point() { x = 0; y = 0; }
	explicit Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Point(const Point &other)
	{
		x = other.x;
		y = other.y;
	}
	Point& operator=(const Point &other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
	bool operator==(const Point &other) const { return x == other.x && y == other.y; }
	bool operator!=(const Point &other) const { return !(*this == other); }
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
};

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

#endif /*POINT_H_*/
