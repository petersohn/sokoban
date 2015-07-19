#ifndef PREPROCESSEDRESULT_HPP
#define PREPROCESSEDRESULT_HPP

#include <memory>

class Checker;
class HeurCalculator;

struct PreprocessedResult {
    std::shared_ptr<Checker> checker;
    std::shared_ptr<HeurCalculator> heurCalculator;
};

template <typename Archive>
void serialize(Archive& ar, PreprocessedResult& preprocessedResult,
        const unsigned int /*version*/)
{
    ar & preprocessedResult.checker;
    ar & preprocessedResult.heurCalculator;
}


#endif // PREPROCESSEDRESULT_HPP
