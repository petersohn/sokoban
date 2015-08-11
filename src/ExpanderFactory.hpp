#ifndef SRC_EXPANDERFACTORY_HPP
#define SRC_EXPANDERFACTORY_HPP

#include <memory>

class Expander;
class Status;

typedef std::function<std::unique_ptr<Expander>(const Status&)> ExpanderFactory;

#endif /* SRC_EXPANDERFACTORY_HPP */
