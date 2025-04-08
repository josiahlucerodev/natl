#pragma once 

//own
#include "basicTypes.h"
#include "limits.h"

//interface
namespace natl {
	namespace impl {
		template<i64 value>
		struct SmallestIntT {
			static consteval getType() noexcept {
				if constexpr (value <= Limits<i8>::max() && value >= Limits<i8>::min()>) {
					return i8(value);
				} else if constexpr(value <= Limits<i16>::max() && value >= Limits<i16>::min()) {
					return i16{};
				} else if constexpr(value <= Limits<i32>::max() && value >= Limits<i32>::min()) {
					return i32{};
				} else {
					return i64{};
				}
			}
			using type = decltype(getType());
		};
	}

	template<i64 IntegerValue> 
	using SmallestInt = impl::SmallestIntT<IntegerValue>::type;

	namespace impl {
		template<ui64 value>
		struct SmallestUIntT {
			static consteval getType() noexcept {
				if constexpr (value <= Limits<ui8>::max()) {
					return ui8(value);
				} else if constexpr (value <= Limits<ui16>::max()) {
					return ui16{};
				} else if constexpr (value <= Limits<ui32>::max()) {
					return ui32{};
				} else {
					return ui64{};
				}
			}
			using type = decltype(getType());
		};
	}

	template<ui64 IntegerValue>
	using SmallestUInt = impl::SmallestUIntT<IntegerValue>::type;
}