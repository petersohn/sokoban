#ifndef SRC_COMPLEXCHECKERBASE_HPP
#define SRC_COMPLEXCHECKERBASE_HPP

#include <assert.h>
#include <memory>
#include <type_traits>
#include <vector>

template <typename Checker>
class ComplexCheckerBase {
    typedef std::vector<std::shared_ptr<Checker>> ContainerType;
    ContainerType funcs_;
#ifndef NO_UNSAFE_DIAGNOSTICS
    mutable const char *lastError_;
#endif

public:
    ComplexCheckerBase() = default;

    template<class Range>
    ComplexCheckerBase(const Range& range):
        funcs_(range.begin(), range.end())
    {}
    ComplexCheckerBase(const std::initializer_list<std::shared_ptr<Checker>>& range):
        funcs_(range.begin(), range.end())
    {}

    ComplexCheckerBase(const ComplexCheckerBase&) = default;
    ComplexCheckerBase& operator=(const ComplexCheckerBase&) = default;
    ComplexCheckerBase(ComplexCheckerBase&&) = default;
    ComplexCheckerBase& operator=(ComplexCheckerBase&&) = default;

    void append(std::shared_ptr<Checker> checker)
    {
        funcs_.push_back(std::move(checker));
    }

    template <typename... Args, bool enabled = std::is_const<Checker>::value>
    typename std::enable_if<enabled, bool>::type
    check(const Args&... args) const {

#ifndef NO_UNSAFE_DIAGNOSTICS
        lastError_ = "";
#endif
        for (const auto& func: funcs_) {
            assert(func.get() != NULL);
            if (!func->check(args...)) {
#ifndef NO_UNSAFE_DIAGNOSTICS
                lastError_ = func->errorMessage();
#endif
                return false;
            }
        }
        return true;
    }

    template <typename... Args, bool enabled = !std::is_const<Checker>::value>
    typename std::enable_if<enabled, bool>::type
    check(const Args&... args) {

#ifndef NO_UNSAFE_DIAGNOSTICS
        lastError_ = "";
#endif
        for (const auto& func: funcs_) {
            assert(func.get() != NULL);
            if (!func->check(args...)) {
#ifndef NO_UNSAFE_DIAGNOSTICS
                lastError_ = func->errorMessage();
#endif
                return false;
            }
        }
        return true;
    }

    const char* errorMessage() const
    {
#ifndef NO_UNSAFE_DIAGNOSTICS
        return lastError_;
#else
        return "check failed";
#endif
    }
};

#endif /* SRC_COMPLEXCHECKERBASE_HPP */
