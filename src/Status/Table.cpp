#include "Dumper/DumperFunctions.hpp"

#include "Status/Table.hpp"
#include "Status/Status.hpp"

std::ostream& operator<<(std::ostream& os, const Table& table)
{
    return os << Status{table};
}

