#pragma once

//std
#include <functional>
#include <bit>

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "compilerDependent.h"
#include "bits.h"

//interface 
namespace natl {
	template<typename DataType>
	concept HasStaticHashFunction = requires(const DataType& value) {
		{ DataType::staticHash(value) } -> SameAs<Size>;
	};
	template<typename DataType>
	concept HasHashFunction = requires(const DataType& value) {
		{ value.hash() } -> SameAs<Size>;
	};
	template <typename DataType>
	concept StdHashable = requires(const DataType& a) {
		{ std::hash<DataType>{}(a) } -> ConvertibleTo<Size>;
	};


	template<typename DataType>
	concept Hashable = HasStaticHashFunction<DataType> || HasHashFunction<DataType> || IsPointerC<DataType> || StdHashable<DataType>;

	template<typename DataType>
	struct Hash {
	public:
		constexpr static Size hash(const DataType& value) requires(Hashable<DataType>) {
			if constexpr (HasStaticHashFunction<DataType>) {
				return DataType::staticHash(value);
			} else if constexpr (HasHashFunction<DataType>) {
				return value.hash();
			} else if constexpr (IsPointerC<DataType>) {
				return static_cast<Size>(natl::bitCast<UIntPtrSized, DataType>(value));
			} else if constexpr (StdHashable<DataType>) {
				return static_cast<Size>(std::hash<DataType>{}(value));
			} else {
				unreachable();
			}
		}
	};


	template<> struct Hash<i8> {
		constexpr static Size hash(const i8 value) { return static_cast<Size>(natl::bitCast<ui8, i8>(value)); }
	};
	template<> struct Hash<i16> {
		constexpr static Size hash(const i16 value) { return static_cast<Size>(natl::bitCast<ui16, i16>(value)); }
	};
	template<> struct Hash<i32> {
		constexpr static Size hash(const i32 value) { return static_cast<Size>(natl::bitCast<ui32, i32>(value)); }
	};
	template<> struct Hash<i64> {
		constexpr static Size hash(const i64 value) { return static_cast<Size>(natl::bitCast<ui64, i64>(value)); }
	};

	template<> struct Hash<ui8> {
		constexpr static Size hash(const ui8 value) { return static_cast<Size>(value); }
	};
	template<> struct Hash<ui16> {
		constexpr static Size hash(const ui16 value) { return static_cast<Size>(value); }
	};
	template<> struct Hash<ui32> {
		constexpr static Size hash(const ui32 value) { return static_cast<Size>(value); }
	};
	template<> struct Hash<ui64> {
		constexpr static Size hash(const ui64 value) { return static_cast<Size>(value); }
	};

	template<> struct Hash<float> {
		constexpr static Size hash(const float value) { return static_cast<Size>(natl::bitCast<ui32, float>(value)); }
	};
	template<> struct Hash<double> {
		constexpr static Size hash(const double value) { return static_cast<Size>(natl::bitCast<ui64, double>(value)); }
	};

	template<typename DataType>
		requires(Hashable<DataType>)
	constexpr Size hashValue(const DataType& value) noexcept {
		return Hash<Decay<DataType>>().hash(value);
	}
 }