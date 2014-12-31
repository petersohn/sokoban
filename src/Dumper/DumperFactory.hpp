#ifndef SRC_DUMPER_DUMPERFACTORY_HPP
#define SRC_DUMPER_DUMPERFACTORY_HPP

#include <memory>

class Dumper;

typedef std::function<std::shared_ptr<Dumper>()> DumperFactory;


#endif /* SRC_DUMPER_DUMPERFACTORY_HPP */