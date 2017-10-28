#include "Dumper/DumperFunctions.hpp"

#include "Status/Table.hpp"
#include "Status/Status.hpp"

namespace sokoban {

std::ostream& operator<<(std::ostream& os, const Table& table)
{
    return os << Status{table};
}

} // namespace sokoban
