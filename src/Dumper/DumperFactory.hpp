#ifndef SRC_DUMPER_DUMPERFACTORY_HPP
#define SRC_DUMPER_DUMPERFACTORY_HPP

#include <functional>
#include <memory>

namespace sokoban {

class Dumper;

typedef std::function<std::unique_ptr<Dumper>()> DumperFactory;

} // namespace sokoban

#endif /* SRC_DUMPER_DUMPERFACTORY_HPP */
