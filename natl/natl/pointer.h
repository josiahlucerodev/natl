#pragma once

//interface
namespace natl {
	template<class DataType>
	class PtrWrapper {
	public:
		constexpr PtrWrapper() = default;
		constexpr PtrWrapper(DataType& data) : data(&data) {};
		constexpr PtrWrapper(DataType* const data) : data(data) {};

		DataType* data;
		constexpr DataType& unwrap() { return *data; };
		constexpr const DataType& unwrap() const { return *data; };
		constexpr DataType& operator*() { return *data; };
		constexpr const DataType& operator*() const { return *data; };
		constexpr DataType& operator->() { return *data; };
		constexpr const DataType& operator->() const { return *data; };
	};
}