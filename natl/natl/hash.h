#pragma once

//std
#include <functional>
#include <bit>

//own
#include "basicTypes.h"

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
			} 
			if constexpr (HasHashFunction<DataType>) {
				return value.hash();
			}
			if constexpr (std::is_pointer_v<DataType>) {
				return std::bit_cast<Size, DataType>(value);
			}
			if constexpr (StdHashable<DataType>) {
				return static_cast<Size>(std::hash<DataType>{}(value));
			}
		}
	};


	template<> struct Hash<i8> {
		constexpr static Size hash(const i8& value) { return static_cast<Size>(std::bit_cast<Size, i64>(static_cast<i64>(value))); }
	};
	template<> struct Hash<i16> {
		constexpr static Size hash(const i16& value) { return static_cast<Size>(std::bit_cast<Size, i64>(static_cast<i64>(value))); }
	};
	template<> struct Hash<i32> {
		constexpr static Size hash(const i32& value) { return static_cast<Size>(std::bit_cast<Size, i64>(static_cast<i64>(value))); }
	};
	template<> struct Hash<i64> {
		constexpr static Size hash(const i64& value) { return static_cast<Size>(std::bit_cast<Size, i64>(value)); }
	};

	template<> struct Hash<ui8> {
		constexpr static Size hash(const ui8& value) { return static_cast<Size>(std::bit_cast<Size, ui64>(static_cast<ui64>(value))); }
	};
	template<> struct Hash<ui16> {
		constexpr static Size hash(const ui16& value) { return static_cast<Size>(std::bit_cast<Size, ui64>(static_cast<ui64>(value))); }
	};
	template<> struct Hash<ui32> {
		constexpr static Size hash(const ui32& value) { return static_cast<Size>(std::bit_cast<Size, ui64>(static_cast<ui64>(value))); }
	};
	template<> struct Hash<ui64> {
		constexpr static Size hash(const ui64& value) { return static_cast<Size>(std::bit_cast<Size, ui64>(value)); }
	};

	template<> struct Hash<float> {
		constexpr static Size hash(const float& value) { return static_cast<Size>(std::bit_cast<ui32, float>(value)); }
	};
	template<> struct Hash<double> {
		constexpr static Size hash(const double& value) { return static_cast<Size>(std::bit_cast<ui64, double>(value)); }
	};
 }