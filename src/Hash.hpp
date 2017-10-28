#ifndef HASH_HPP_
#define HASH_HPP_

#include <functional>

namespace sokoban {

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

} // namespace sokoban

#endif /* HASH_HPP_ */
