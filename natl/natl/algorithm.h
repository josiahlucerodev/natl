#pragma once

//interface
namespace natl {
	template<class DataType> const DataType& min(const DataType& a, const DataType& b) {
		return (b < a) ? b : a;
	}
	template<class DataType> const DataType& max(const DataType& a, const DataType& b) {
		return (b > a) ? b : a;
	}
}