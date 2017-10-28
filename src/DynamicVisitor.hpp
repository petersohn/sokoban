#ifndef DYNAMICVISITOR_HPP
#define DYNAMICVISITOR_HPP

#include <boost/mpl/for_each.hpp>

namespace sokoban {

namespace detail {

template <typename T>
struct Wrapper {
    using type = T;
};

struct Wrap {
    template <typename T>
    struct apply {
        using type = Wrapper<T>;
    };
};

}

template <typename Types, typename Visitor, typename Base>
void applyDynamicVisitor(const Visitor& visitor, Base& object) {
    boost::mpl::for_each<Types, detail::Wrap>(
            [&](auto wrapper) {
                using Type = typename decltype(wrapper)::type;
                auto ptr = dynamic_cast<Type*>(&object);
                if (ptr) {
                    visitor(*ptr);
                }
            });
}

} // namespace sokoban

#endif // DYNAMICVISITOR_HPP
