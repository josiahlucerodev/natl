#pragma once

//std
#include <functional>
#include <bit>

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "compilerDependent.h"

//interface 
namespace natl {
	template<class DataType>
	concept HasStaticHashFunction = requires(const DataType& value) {
		{ DataType::staticHash(value) } -> std::same_as<Size>;
	};
	template<class DataType>
	concept HasHashFunction = requires(const DataType& value) {
		{ value.hash() } -> std::same_as<Size>;
	};
	template <typename DataType>
	concept StdHashable = requires(const DataType& a) {
		{ std::hash<DataType>{}(a) } -> std::convertible_to<std::size_t>;
	};


	template<typename DataType>
	concept Hashable = HasStaticHashFunction<DataType> || HasHashFunction<DataType> || std::is_pointer_v<DataType> || StdHashable<DataType>;

	template<class DataType>
	struct Hash {
	public:
		constexpr static Size hash(const DataType& value) requires(Hashable<DataType>) {
			if constexpr (HasStaticHashFunction<DataType>) {
				return DataType::staticHash(value);
			} else if constexpr (HasHashFunction<DataType>) {
				return value.hash();
			} else if constexpr (std::is_pointer_v<DataType>) {
				return static_cast<Size>(std::bit_cast<UIntPtrSized, DataType>(value));
			} else if constexpr (StdHashable<DataType>) {
				return static_cast<Size>(std::hash<DataType>{}(value));
			} else {
				unreachable();
			}
		}
	};


	template<> struct Hash<i8> {
		constexpr static Size hash(const i8 value) { return static_cast<Size>(std::bit_cast<ui8, i8>(value)); }
	};
	template<> struct Hash<i16> {
		constexpr static Size hash(const i16 value) { return static_cast<Size>(std::bit_cast<ui16, i16>(value)); }
	};
	template<> struct Hash<i32> {
		constexpr static Size hash(const i32 value) { return static_cast<Size>(std::bit_cast<ui32, i32>(value)); }
	};
	template<> struct Hash<i64> {
		constexpr static Size hash(const i64 value) { return static_cast<Size>(std::bit_cast<ui64, i64>(value)); }
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


//used for constexpr hash of size, only for EMSCRIPTEN
//the reason why is idk, im either stupid or things just don't work out
#ifdef __EMSCRIPTEN__
	template<> struct Hash<Size> {
		constexpr static Size hash(const Size value) { return value; }
	};
#endif // __EMSCRIPTEN__



	template<> struct Hash<float> {
		constexpr static Size hash(const float value) { return static_cast<Size>(std::bit_cast<ui32, float>(value)); }
	};
	template<> struct Hash<double> {
		constexpr static Size hash(const double value) { return static_cast<Size>(std::bit_cast<ui64, double>(value)); }
	};

	template<typename DataType>
		requires(Hashable<DataType>)
	constexpr Size hashValue(const DataType& value) noexcept {
		return Hash<DecayT<DataType>>().hash(value);
	}
 }