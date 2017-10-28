#ifndef SRC_EXCEPTION_HPP
#define SRC_EXCEPTION_HPP

#include <stdexcept>

namespace sokoban {

struct SokobanException: public std::logic_error {
    using std::logic_error::logic_error;
};

} // namespace sokoban

#endif /* SRC_EXCEPTION_HPP */
