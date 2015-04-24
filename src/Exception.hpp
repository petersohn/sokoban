#ifndef SRC_EXCEPTION_HPP
#define SRC_EXCEPTION_HPP

#include <stdexcept>

struct SokobanException: public std::logic_error {
    using std::logic_error::logic_error;
};


#endif /* SRC_EXCEPTION_HPP */
