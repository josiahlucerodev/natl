#pragma once

//interface
namespace natl {
	template<class DataType> 
	constexpr const DataType& min(const DataType& a, const DataType& b) {
		return (b < a) ? b : a;
	}
	template<class DataType> 
	constexpr const DataType& max(const DataType& a, const DataType& b) {
		return (b > a) ? b : a;
	}
	template<class DataType>
	constexpr static DataType clamp(const DataType value, const DataType min, const DataType max) noexcept {
		return (value < min) ? min : ((value > max) ? max : value);
	}
}