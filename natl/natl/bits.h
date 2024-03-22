#pragma once 

//interface 
namespace natl {
	template<typename DataType>
	constexpr static DataType setNthBitToZero(const DataType num, const DataType n) noexcept {
		const DataType mask = ~(DataType(1) << n);
		const DataType result = num & mask;
		return result;
	}
	template<typename DataType>
	constexpr static DataType setNthBitToOne(const DataType num, const DataType n) noexcept {
		const DataType mask = DataType(1) << n;
		const DataType result = num | mask;
		return result;
	}
}