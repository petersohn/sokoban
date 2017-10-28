#include "Status/Point.hpp"

namespace sokoban {

std::string direction(Point d)
{
    return
            d.x > 0 ? "right" :
            d.x < 0 ? "left" :
            d.y > 0 ? "down" :
            d.y < 0 ? "up" : "???";
}

} // namespace sokoban
