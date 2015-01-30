#ifndef SRC_COMPLEXCHECKER_HPP
#define SRC_COMPLEXCHECKER_HPP

class Checker;
template <typename Checker> class ComplexCheckerBase;

using ComplexChecker = ComplexCheckerBase<const Checker>;


#endif /* SRC_COMPLEXCHECKER_HPP */
