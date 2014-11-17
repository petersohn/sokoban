#include "FieldType.hpp"

std::ostream& operator<<(std::ostream& os, FieldType fieldType)
{
	switch (fieldType) {
	case FieldType::wall: return os << "wall";
	case FieldType::floor: return os << "floor";
	case FieldType::stone: return os << "stone";
	default: return os;
	}
}

