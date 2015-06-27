#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

#include "Options.hpp"

#include "util/TimeMeter.hpp"

#include <memory>

class Status;
class Table;
class Loader;

class Sokoban {
public:
    Sokoban(int argc, const char* argv[]);
    ~Sokoban();

    int run();

private:
    Options options;

    std::unique_ptr<const Table> table;
    std::unique_ptr<Status> status;
    std::unique_ptr<Loader> loader;

    util::TimeMeter timeMeter;
    std::size_t expandedNodes = 0;
    util::TimerData chokePointFinderTime;
    util::TimerData preprocessingIterationTime;
};

#endif // SOKOBAN_HPP
