#ifndef UNITTEST_BORDERTYPEIO_HPP
#define UNITTEST_BORDERTYPEIO_HPP

#include "Status/Status.hpp"

#include <algorithm>
#include <ostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace sokoban {

inline bool operator==(Status::BorderType lhs, Status::BorderType rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }

    std::sort(lhs.begin(), lhs.end());
    std::sort(rhs.begin(), rhs.end());

    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

inline std::ostream& operator<<(std::ostream& os, Status::BorderType border)
{
    std::sort(border.begin(), border.end());
    return os << '{' << boost::algorithm::join(
            border | boost::adaptors::transformed(boost::lexical_cast<std::string, Point>),
            ", ") << '}';
}

} // namespace sokoban

#endif /* UNITTEST_BORDERTYPEIO_HPP */
