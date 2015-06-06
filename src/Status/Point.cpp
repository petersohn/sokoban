#include "Status/Point.hpp"

std::string direction(Point d)
{
    return
            d.x > 0 ? "right" :
            d.x < 0 ? "left" :
            d.y > 0 ? "down" :
            d.y < 0 ? "up" : "???";
}

std::ostream& operator<<(std::ostream& os, Point p)
{
    os << '(' << p.x << ", " << p.y << ')';
    return os;
}

