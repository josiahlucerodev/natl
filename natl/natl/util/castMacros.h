#pragma once

#include "stateful.h"

#define NATL_PUBLIC_CAST_HELPER(NamespaceName, ObjectType, Object, Member) \
	namespace NamespaceName { \
		NATL_DEFINE_STATEFUL_TYPEPACK(Object##Member##PublicCastTypePack) \
		template<typename Type, typename Secret> struct Object##Member##PublicCast { static inline Type value{}; }; \
		template<typename Secret, auto Type> struct Object##Member##PrivateAccess { \
			static inline auto value = Object##Member##PublicCast<decltype(Type), Secret>::value = Type; \
			constexpr static auto u1 = Object##Member##PublicCastTypePackAppendInClass<decltype(Type)>(); \
			constexpr static auto u2 = natl::PublicCastTypePackAppendInClass<decltype(Type)>(); \
		}; \
		template struct Object##Member##PrivateAccess<class Object##Member##PrivateSecret, &ObjectType::Member>; \
	}

#define NATL_PUBLIC_CAST(NamespaceName, Object, Member) \
    Object.*NamespaceName::Object##Member##PublicCast<natl::TypePackBack<NamespaceName::Object##Member##PublicCastTypePack<>>, NamespaceName::Object##Member##PrivateSecret>::value
