#pragma once

//interface 
namespace natl {

	template<class DataType>
	concept HasStaticHashFunction = requires(const DataType& value) {
		{ DataType::staticHash(value) } -> std::same_as<std::size_t>;
	};

	template<class DataType>
	struct Hash {
	public:
		constexpr static std::size_t hash(const DataType& dataType) 
			requires(HasStaticHashFunction<DataType>)
		{ return DataType::staticHash(dataType); }
		constexpr static std::size_t hash(const DataType& dataType)
			requires(std::is_pointer_v<DataType>) {
			return std::bit_cast<std::size_t, DataType>(dataType);
		}
	};

	template<typename T>
	concept Hashable = requires(T a) {
		{ Hash<T>{}(a) } -> std::convertible_to<std::size_t>;
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