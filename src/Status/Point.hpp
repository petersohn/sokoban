#ifndef POINT_H_
#define POINT_H_

#include "Hash.hpp"

#include <ostream>
#include <stddef.h>

namespace sokoban {

class Point {
public:
    int x, y;
    Point():x(0), y(0) { }
    explicit constexpr Point(int x, int y):x(x), y(y) {}

    constexpr Point(const Point&) = default;
    Point& operator=(const Point&) = default;

    Point& operator+=(Point other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point& operator-=(Point other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Point& operator*=(int n)
    {
        x *= n;
        y *= n;
        return *this;
    }
};

template<typename Archive>
void serialize(Archive& ar, Point& p, const unsigned int /*version*/) {
    ar & p.x;
    ar & p.y;
}


constexpr Point p10{1, 0}, p01{0, 1}, p11{1, 1};

inline constexpr bool operator==(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

inline constexpr bool operator!=(Point p1, Point p2) {
    return !(p1 == p2);
}

inline constexpr Point operator+(Point p1, Point p2) {
    return Point(p1.x+p2.x, p1.y+p2.y);
}

inline constexpr Point operator-(Point p1, Point p2) {
    return Point(p1.x-p2.x, p1.y-p2.y);
}

inline constexpr Point operator*(Point p, int n) {
    return Point(p.x*n, p.y*n);
}

inline constexpr Point operator-(Point p) {
    return p * -1;
}

inline constexpr bool operator<(Point p1, Point p2) {
    return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
}

std::string direction(Point d);
std::ostream& operator<<(std::ostream& os, Point p);

} // namespace sokoban

namespace std {

template<>
struct hash<sokoban::Point> {
    size_t operator()(sokoban::Point p) const
    {
        size_t seed = 0;
        sokoban::hash_combine(seed, p.x);
        sokoban::hash_combine(seed, p.y);
        return seed;
    }
};

} // namespace std

#endif /*POINT_H_*/
