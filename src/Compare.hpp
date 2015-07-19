#ifndef SRC_COMPARE_HPP
#define SRC_COMPARE_HPP

enum class CompareMethod {time = 1, heur, depth};

struct Compare {
    CompareMethod type;
    bool reverse;
    Compare(): type(CompareMethod::time), reverse(false) {}
    Compare(CompareMethod type, bool reverse):
        type(type), reverse(reverse)
    {}
};

template <typename Ar>
void serialize(Ar& ar, Compare& compare, const unsigned int /*version*/)
{
    ar & compare.type;
    ar & compare.reverse;
}

#endif /* SRC_COMPARE_HPP */
