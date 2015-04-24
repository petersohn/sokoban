#ifndef SRC_INTERSECT_HPP
#define SRC_INTERSECT_HPP

#include <vector>
#include <algorithm>
#include <boost/optional.hpp>

template<class T>
std::vector<T> intersect(std::vector<T> l1, std::vector<T> l2)
{
    std::sort(l1.begin(), l1.end());
    std::sort(l2.begin(), l2.end());
    std::vector<T> result;
    std::set_intersection(l1.begin(), l1.end(), l2.begin(), l2.end(),
            std::back_inserter(result));
    return result;
}



#endif /* SRC_INTERSECT_HPP */
