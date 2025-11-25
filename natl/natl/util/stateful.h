#pragma once 

//own
#include "compilerDependent.h"
 
#if defined(NATL_COMPILER_GCC)
#define NATL_DEFINE_STATEFUL_IMPL_START \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic ignored \"-Wnon-template-friend\"")

#define NATL_DEFINE_STATEFUL_IMPL_END \
        _Pragma("GCC diagnostic pop")
#else
#define NATL_DEFINE_STATEFUL_IMPL_START
#define NATL_DEFINE_STATEFUL_IMPL_END
#endif

#define NATL_DEFINE_STATEFUL_COUNTER(CounterName) \
    NATL_DEFINE_STATEFUL_IMPL_START \
    namespace impl { \
        template<natl::Size Number> \
        struct CounterName##StatefulCounterReader { \
            friend auto countedFlag(CounterName##StatefulCounterReader<Number>); \
        }; \
        template<natl::Size Number> \
        struct CounterName##StatefulTypePackStatefulCounterSetter { \
            friend auto countedFlag(CounterName##StatefulCounterReader<Number>) {} \
            static constexpr natl::Size number = Number; \
        }; \
        template<auto Tag, natl::Size NextValue = 0> \
        [[nodiscard]] consteval auto CounterName##StatefulCounterImpl() { \
            constexpr bool countedPast = requires(CounterName##StatefulCounterReader<NextValue> reader) { \
                countedFlag(reader); \
            }; \
            if constexpr (countedPast) { \
                return CounterName##StatefulCounterImpl<Tag, NextValue + 1>(); \
            } else { \
                CounterName##StatefulTypePackStatefulCounterSetter<NextValue> setter; \
                return setter.number; \
            } \
        } \
    } \
    template<auto Tag = [] {}, auto Value = impl::CounterName##StatefulCounterImpl<Tag>()> \
    constexpr auto CounterName = Value; \
    NATL_DEFINE_STATEFUL_IMPL_END

#define NATL_DEFINE_STATEFUL_TYPEPACK(TypePackName) \
    NATL_DEFINE_STATEFUL_IMPL_START \
    namespace impl { \
        template<auto> struct TypePackName##StatefulTypePackNth { constexpr auto friend get(TypePackName##StatefulTypePackNth); constexpr auto friend get(TypePackName##StatefulTypePackNth); }; \
        template<auto Number, typename Type> struct TypePackName##StatefulTypePackSet { constexpr auto friend get(TypePackName##StatefulTypePackNth<Number>) { return Type{}; } }; \
        template<typename Type, template<typename...> typename TemplateList, typename... Types> \
        constexpr auto TypePackName##Append(TemplateList<Types...>) -> TemplateList<Types..., Type>; \
    } \
    template<typename Type, auto Number = 0, auto Unique = [] {} > \
    consteval auto TypePackName##Append() { \
        if constexpr (requires { get(impl::TypePackName##StatefulTypePackNth<Number>{}); }) { \
            TypePackName##Append<Type, Number + 1, Unique>(); \
        } else if constexpr (Number == 0) { \
            void(impl::TypePackName##StatefulTypePackSet<Number, natl::TypePack<Type>>{}); \
        } else { \
            void(impl::TypePackName##StatefulTypePackSet < Number, decltype(impl::TypePackName##Append<Type>(get(impl::TypePackName##StatefulTypePackNth<Number - 1>{}))) > {}); \
        } \
    } \
    template<typename Type, auto Unique = [] {} > \
    consteval natl::Size TypePackName##AppendInClass() { TypePackName##Append<Type>(); return 0; } \
    template<auto Unique = [] {}, auto Number = 0 > \
    consteval auto get##TypePackName() { \
        if constexpr (requires { get(impl::TypePackName##StatefulTypePackNth<Number>{}); }) { \
            return get##TypePackName<Unique, Number + 1>(); \
        } else if constexpr (Number == 0) { \
            return natl::TypePack{}; \
        } else { \
            return get(impl::TypePackName##StatefulTypePackNth<Number - 1>{}); \
        } \
    } \
    template<auto Unique = [] {}> \
    using TypePackName = decltype(get##TypePackName<Unique>()); \
    NATL_DEFINE_STATEFUL_IMPL_END

#define NATL_UNIQUE_FUNCTION template<auto Unique = []{}>
#define NATL_UNIQUE_FUNCTION_TYPE auto Unique = []{}


