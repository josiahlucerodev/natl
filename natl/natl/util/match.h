#pragma once

//own
#include "typeTraits.h"
#include "typePack.h"

//@export
namespace natl {
	namespace impl {
		template<typename TypeKind, typename Predicate, typename... Types>
		constexpr Bool matchType(TypeKind kind, Predicate&& predicate, TypePack<Types...>) noexcept {
			Bool matched = false;
			(
				(!matched && kind == Types::value
						? (predicate.template operator() < typename Types::value_type > (), matched = true) : false),
				...
			);
			return matched;
		}
	}

	template<typename TypeKind, typename... Types, typename Predicate>
	constexpr Bool matchType(TypeKind kind, Predicate&& predicate) noexcept {
		return impl::matchType(kind, forward<Predicate>(predicate), TypePack<Types...>{});
	}
}