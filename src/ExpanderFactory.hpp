#ifndef SRC_EXPANDERFACTORY_HPP
#define SRC_EXPANDERFACTORY_HPP

#include <memory>

class Expander;

typedef std::function<std::shared_ptr<Expander>()> ExpanderFactory;

#endif /* SRC_EXPANDERFACTORY_HPP */
