#pragma once

//interface
namespace natl {
	template<class DataType> 
	constexpr DataType min(const DataType& a, const DataType& b) noexcept {
		return (b < a) ? b : a;
	}
	template<class DataType> 
	constexpr DataType max(const DataType& a, const DataType& b) noexcept {
		return (b > a) ? b : a;
	}
	template<class DataType>
	constexpr static DataType clamp(const DataType value, const DataType min, const DataType max) noexcept {
		return (value < min) ? min : ((value > max) ? max : value);
	}

	template <typename SizeType, typename ArrayViewLike>
		requires(IsArrayViewLike<ArrayViewLike, SizeType>)
	constexpr SizeType findLowerBoundIndex(const SizeType& value, ArrayViewLike& arrayView) noexcept {
		SizeType left = 0;
		SizeType right = arrayView.size();
		while (left < right) {
			SizeType mid = left + (right - left) / 2;
			if (arrayView.data()[mid] < value) {
				left = mid + 1;
			} else {
				right = mid;
			}
		}
		return left;
	}
}