#pragma once

//@begin_non_modules
//std
#include <tuple>
//@end_non_modules

//own
#include "typeTraits.h"
#include "typePack.h"
#include "../util/tuple.h"

//@export
namespace natl {
	template<typename... ArgTypes>
	struct FuncArgs {
	public:
		using arg_types = TypePack<ArgTypes...>;
		using storage_type = Tuple<ArgTypes...>;

		//public
		mutable storage_type storage;

		//constructor
		constexpr FuncArgs() noexcept = default;
		constexpr FuncArgs(ArgTypes&&... args) noexcept 
			: storage(natl::forward<ArgTypes>(args)...) {};

		//destructor
		constexpr ~FuncArgs() noexcept = default;

		//element access 
		template<Size Index>
			requires(Index < arg_types::size)
		constexpr auto get() const noexcept {
			return storage.template get<Index>();
		}
		constexpr Size size() const noexcept { 
			return arg_types::size;
		}
	};

	template<>
	struct FuncArgs<> {
		using arg_types = TypePack<>;
		using storage_type = Tuple<>;
		constexpr Size size() const noexcept { return 0; }
		template<Size Index>
		constexpr void get() const noexcept {}
	};

	template<typename... ArgTypes>
	constexpr FuncArgs<ArgTypes...> passFuncArgs(ArgTypes&&... args) noexcept {
		return FuncArgs<ArgTypes...>(natl::forward<ArgTypes>(args)...);
	}
}

namespace std {
	template<typename... ArgTypes>
	struct tuple_size<natl::FuncArgs<ArgTypes...>> : natl::IntegralConstant<natl::Size, sizeof...(ArgTypes)> {};
	template<> struct tuple_size<natl::FuncArgs<>> : natl::IntegralConstant<natl::Size, 0> {};
	template<natl::StdSize Index, typename... ArgTypes>
		requires(Index < sizeof...(ArgTypes))
	struct tuple_element<Index, natl::FuncArgs<ArgTypes...>> {
		using type = natl::FuncArgs<ArgTypes...>::arg_types::template at<Index>;
	};
	template<natl::StdSize Index>
	struct tuple_element<Index, natl::FuncArgs<>> { using type = void; };

	template <std::size_t Index, typename... ArgTypes>
	constexpr decltype(auto) get(const natl::FuncArgs<ArgTypes...>& obj) noexcept {
		static_assert(Index < sizeof...(ArgTypes), "natl: std::get(natl::FuncArgs<...>) index out of bounds");
		return obj.template get<Index>();
	}
	template <std::size_t Index, typename... ArgTypes>
	constexpr decltype(auto) get(natl::FuncArgs<ArgTypes...>& obj) noexcept {
		static_assert(Index < sizeof...(ArgTypes), "natl: std::get(natl::FuncArgs<...>) index out of bounds");
		return obj.template get<Index>();
	}
}