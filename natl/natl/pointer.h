#pragma once

//interface
namespace natl {
	template<class DataType>
	class PtrWrapper {
	public:
		DataType* data;

		constexpr PtrWrapper() : data(nullptr) {}
		constexpr PtrWrapper(DataType& data) : data(&data) {};
		constexpr PtrWrapper(DataType* const data) : data(data) {};

		constexpr DataType& unwrap() { return *data; };
		constexpr const DataType& unwrap() const { return *data; };
		constexpr DataType& operator*() { return *data; };
		constexpr const DataType& operator*() const { return *data; };
		constexpr DataType& operator->() { return *data; };
		constexpr const DataType& operator->() const { return *data; };
	};
}