#ifndef SRC_COMPLEXCHECKER_HPP
#define SRC_COMPLEXCHECKER_HPP

class Checker;
class NodeChecker;
template <typename Checker> class ComplexCheckerBase;

using ComplexChecker = ComplexCheckerBase<const Checker>;
using ComplexNodeChecker = ComplexCheckerBase<const NodeChecker>;


#endif /* SRC_COMPLEXCHECKER_HPP */
