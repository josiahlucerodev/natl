#pragma once 

//own
#include "compilerDependent.h"
#include "typeTraits.h"
#include "bits.h"
#include "stateful.h"

//interface 
namespace natl {
	template<typename Type>
	[[nodiscard]] NATL_FORCE_INLINE constexpr Type implicitCast(TypeIdentity<Type> value) noexcept {
		return value;
	}
	template<typename To, typename From>
		requires(IsSameByteSizeC<To, From>)
	[[nodiscard]] NATL_FORCE_INLINE constexpr To punCast(const From& from) noexcept {
		return bitCast<To, From>(from);
	}
	
	namespace impl {
	}

NATL_DEFINE_STATEFUL_TYPEPACK(PublicCastTypePack)
	template<typename M, typename Secret>
	struct PublicCast {
		static inline M value{};
	};
	template<typename Secret, auto M>
	struct PrivateAccess {
		static const inline auto value
			= PublicCast<decltype(M), Secret>::value = M;
		constexpr static auto t = PublicCastTypePackAppendInClass<decltype(M)>();
	};

}

#define NATL_PUBLIC_CAST_HELPER(NamespaceName, ObjectType, Object, Member) \
	namespace NamespaceName { \
		NATL_DEFINE_STATEFUL_TYPEPACK(Object##Member##PublicCastTypePack) \
		template<typename M, typename Secret> struct Object##Member##PublicCast { static inline M value{}; }; \
		template<typename Secret, auto M> struct Object##Member##PrivateAccess { \
			static const inline auto value = Object##Member##PublicCast<decltype(M), Secret>::value = M; \
			constexpr static auto t = Object##Member##PublicCastTypePackAppendInClass<decltype(M)>(); \
		}; \
		template struct Object##Member##PrivateAccess<class Object##Member##PrivateSecret, &ObjectType::Member>; \
	}

#define NATL_PUBLIC_CAST(NamespaceName, Object, Member) \
    Object.*NamespaceName::Object##Member##PublicCast<natl::TypePackBack<NamespaceName::Object##Member##PublicCastTypePack<>>, NamespaceName::Object##Member##PrivateSecret>::value


#define NATL_TURN_OFF_PRIVATE #define private public 
#define NATL_TURN_ON_PRIVATE #undef private 