#ifndef SRC_FIELDTYPE_HPP
#define SRC_FIELDTYPE_HPP

#include <ostream>

namespace sokoban {

enum class FieldType { floor, wall, stone };

std::ostream& operator<<(std::ostream& os, FieldType fieldType);

} // namespace sokoban

#endif /* SRC_FIELDTYPE_HPP */
