#pragma once

//std
#include <cstdint>
#include <bit>

//interface 
namespace natl {
	template<typename T>
	concept Hashable = requires(T a)
	{
		{ std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
	};

	template<class DataType>
	concept HasStaticHashFunction = requires(DataType value) {
		{ DataType::hash(value) } -> std::same_as<std::size_t>;
	};

	template<class DataType>
	struct Hash {
		constexpr static std::size_t hash(const DataType& dataType) 
			requires(HasStaticHashFunction<DataType>) 
		{ return DataType::hash(dataType); }
	};

	template<> struct Hash<std::int8_t> {
		constexpr static std::size_t hash(const std::int8_t& value) { return value; }
	};
	template<> struct Hash<std::int16_t> {
		constexpr static std::size_t hash(const std::int16_t& value) { return value; }
	};
	template<> struct Hash<std::int32_t> {
		constexpr static std::size_t hash(const std::int32_t& value) { return value; }
	};
	template<> struct Hash<std::int64_t> {
		constexpr static std::size_t hash(const std::int64_t& value) { return value; }
	};

	template<> struct Hash<std::uint8_t> {
		constexpr static std::size_t hash(const std::uint8_t& value) { return value; }
	};
	template<> struct Hash<std::uint16_t> {
		constexpr static std::size_t hash(const std::uint16_t& value) { return value; }
	};
	template<> struct Hash<std::uint32_t> {
		constexpr static std::size_t hash(const std::uint32_t& value) { return value; }
	};
	template<> struct Hash<std::uint64_t> {
		constexpr static std::size_t hash(const std::uint64_t& value) { return value; }
	};

	template<> struct Hash<float> {
		constexpr static std::size_t hash(const float& value) { return std::bit_cast<std::uint32_t, float>(value); }
	};
	template<> struct Hash<double> {
		constexpr static std::size_t hash(const double& value) { return std::bit_cast<std::uint64_t, double>(value); }
	};
 }