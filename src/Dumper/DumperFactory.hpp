#ifndef SRC_DUMPER_DUMPERFACTORY_HPP
#define SRC_DUMPER_DUMPERFACTORY_HPP

#include <functional>
#include <memory>

class Dumper;

typedef std::function<std::unique_ptr<Dumper>()> DumperFactory;


#endif /* SRC_DUMPER_DUMPERFACTORY_HPP */
