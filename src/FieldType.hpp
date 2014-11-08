#ifndef SRC_FIELDTYPE_HPP
#define SRC_FIELDTYPE_HPP

#include <ostream>

enum class FieldType { floor, wall, stone };

std::ostream& operator<<(std::ostream& os, FieldType fieldType);

#endif /* SRC_FIELDTYPE_HPP */
