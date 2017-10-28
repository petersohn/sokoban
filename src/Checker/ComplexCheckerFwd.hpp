#ifndef SRC_COMPLEXCHECKER_HPP
#define SRC_COMPLEXCHECKER_HPP

namespace sokoban {

class Checker;
class NodeChecker;
template <typename Checker> class ComplexCheckerBase;

using ComplexChecker = ComplexCheckerBase<const Checker>;
using ComplexNodeChecker = ComplexCheckerBase<NodeChecker>;

} // namespace sokoban

#endif /* SRC_COMPLEXCHECKER_HPP */
