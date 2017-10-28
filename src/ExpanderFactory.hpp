#ifndef SRC_EXPANDERFACTORY_HPP
#define SRC_EXPANDERFACTORY_HPP

#include <functional>
#include <memory>

namespace sokoban {

class Expander;
class Status;

typedef std::function<std::unique_ptr<Expander>(const Status&)> ExpanderFactory;

} // namespace sokoban

#endif /* SRC_EXPANDERFACTORY_HPP */
