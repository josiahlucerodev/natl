#pragma once

//std
#include <initializer_list>

//own
#include "typeTraits.h"
#include "basicTypes.h"
#include "iterators.h"
#include "container.h"
#include "option.h"
#include "Limits.h"
#include "peramaterPackOperations.h"
#include "dataMovement.h"
#include "lexicographicalCompare.h"

//interface
namespace natl {

	template <typename ArrayViewLike, class DataType>
	concept IsArrayViewLike = requires(ArrayViewLike arrayViewLike) {
		{ arrayViewLike.data() } -> std::convertible_to<const DataType*>;
		{ arrayViewLike.size() } -> std::convertible_to<Size>;
		{ arrayViewLike[std::declval<Size>()] } -> std::convertible_to<DataType>;
	};

	template <class DataType>
	class ArrayView {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = std::ptrdiff_t;
		using size_type = Size;

		using iterator = RandomAccessIterator<DataType>;
		using const_iterator = RandomAccessIterator<const DataType>;
		using reverse_iterator = ReverseRandomAccessIterator<DataType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const DataType>;

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = true;
	private:
		pointer dataPtr;
		Size arraySize;
	public:
		//constructor 
		constexpr ArrayView() : dataPtr(nullptr), arraySize(0) {}
		constexpr ArrayView(pointer dataPtr, const size_type size) : dataPtr(dataPtr), arraySize(size) {}
		constexpr ArrayView(ArrayView& other) noexcept : dataPtr(other.data()), arraySize(other.size()) {}
		constexpr ArrayView(const ArrayView& other) noexcept requires(isConst<DataType>) : dataPtr(other.data()), arraySize(other.size()) {}
		constexpr ArrayView(std::initializer_list<DataType> initList) noexcept requires(isConst<DataType>) : dataPtr(initList.begin()), arraySize(initList.size()) {}
		template<class ArrayViewLike>
			requires(!std::is_same_v<ArrayViewLike, std::initializer_list<DataType>>&& IsArrayViewLike<ArrayViewLike, DataType>)
		constexpr ArrayView(const ArrayViewLike& arrayViewLike) noexcept requires(isConst<DataType>) : dataPtr(arrayViewLike.data()), arraySize(arrayViewLike.size()) {}

		//destructor
		constexpr ~ArrayView() = default;

		//util
		constexpr ArrayView& self() noexcept { return *this; }
		constexpr const ArrayView& self() const noexcept { return *this; }

		//assignment 
		constexpr ArrayView& operator=(const ArrayView& other) noexcept {
			dataPtr = other.dataPtr;
			arraySize = other.arraySize;
			return self();
		};
		constexpr void operator=(std::initializer_list<DataType> initList) noexcept requires(isConst<DataType>) {
			dataPtr = initList.begin();
			arraySize = initList.size();
			return self();
		}

		//conversion 
		constexpr operator ArrayView<const DataType>() requires(isNotConst<DataType>) {
			return ArrayView<const DataType>(data(), size());
		}

		//iterators 
		constexpr pointer beginPtr() noexcept requires(isNotConst<DataType>) { return dataPtr; }
		constexpr const_pointer beginPtr() const noexcept { return dataPtr; }
		constexpr pointer endPtr() noexcept requires(isNotConst<DataType>) { return dataPtr + arraySize; }
		constexpr const_pointer endPtr() const noexcept { return dataPtr + arraySize; }

		constexpr iterator begin() noexcept requires(isNotConst<DataType>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(isNotConst<DataType>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(isNotConst<DataType>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(isNotConst<DataType>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		//element access
		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return arraySize ? arraySize - 1 : 0; }

		constexpr reference front() noexcept requires(isNotConst<DataType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept requires(isNotConst<DataType>) { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr reference at(const size_type index) noexcept requires(isNotConst<DataType>) { return dataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return dataPtr[index]; };

		constexpr reference operator[](const size_type index) noexcept requires(isNotConst<DataType>) { return dataPtr[index]; };
		constexpr const_reference operator[](const size_type index) const noexcept { return dataPtr[index]; };

		constexpr pointer data() noexcept requires(isNotConst<DataType>) { return dataPtr; };
		constexpr const_pointer data() const noexcept { return dataPtr; };

		//observers
		constexpr size_type size() const noexcept { return arraySize; }
		constexpr size_type size_bytes() const noexcept { return arraySize * sizeof(DataType); }

		constexpr bool empty() const noexcept { return arraySize == 0 || dataPtr == nullptr; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return arraySize != 0 && dataPtr != nullptr; }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		//subviews
		constexpr ArrayView first(const size_type count) noexcept requires(isNotConst<DataType>) {
			return ArrayView(data(), min<Size>(count, size()));
		}
		constexpr ArrayView first(const size_type count) const noexcept {
			return ArrayView(data(), min<Size>(count, size()));
		}

		constexpr ArrayView last(const size_type count) noexcept requires(isNotConst<DataType>) {
			const Size newCount = min<Size>(count, size());
			return ArrayView(data() + newCount, size() - newCount);
		}
		constexpr ArrayView last(const size_type count) const noexcept {
			const Size newCount = min<Size>(count, size());
			return ArrayView(data() + newCount, size() - newCount);
		}

		constexpr ArrayView subview(size_type offset, size_type count = Limits<Size>::max()) noexcept requires(isNotConst<DataType>) {
			offset = min<Size>(offset, size());
			count = min<Size>(count, size() - offset);
			return ArrayView(data() + offset, count);
		}

		constexpr ArrayView<const DataType> subview(size_type offset, size_type count = Limits<Size>::max()) const noexcept {
			offset = min<Size>(offset, size());
			count = min<Size>(count, size() - offset);
			return ArrayView<const DataType>(data() + offset, count);
		}

		//as bytes
		ArrayView<const Byte> as_bytes() const requires(isNotConst<DataType>) {
			return ArrayView<const Byte>(reinterpret_cast<const Byte*>(data()), size_bytes());
		}
		ArrayView<Byte> as_writable_bytes() const noexcept {
			return ArrayView<Byte>(reinterpret_cast<Byte*>(data()), size_bytes());
		}


		//special element access
		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp<Size>(index, frontIndex(), backIndex()); }

		constexpr reference atClamped(const size_type index) noexcept requires(isNotConst<DataType>) { return dataPtr[clampIndex(index)]; }
		constexpr const_pointer atClamped(const size_type index) const noexcept { return dataPtr[clampIndex(index)]; }

		optional_pointer optionalAt(const size_type index) {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(at(index));
		};
		optional_const_pointer optionalAt(const size_type index) const {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(at(index));
		};

		optional_pointer optionalFront() { return optionalAt(frontIndex()); }
		optional_const_pointer optionalFront() const { return optionalAt(frontIndex()); }
		optional_pointer optionalBack() { return optionalAt(backIndex()); }
		optional_const_pointer optionalBack() const { return optionalAt(backIndex()); }

		constexpr bool has(const size_type index) const noexcept { return index < arraySize; }
		constexpr bool notHave(const size_type index) const noexcept { return index >= arraySize; }

		//operations
		constexpr void fill(const DataType& value) noexcept requires(isNotConst<DataType>) {
			fillCount<pointer, DataType>(data(), value, size());
		}
		constexpr void swap(ArrayView& other) noexcept {
			swap<ArrayView>(self(), other);
		}

		//compare operators 
		friend constexpr bool operator==(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator==(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(isNotConst<DataType>) {
			return lexicographicalCompareEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator==(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs == ArrayView<const DataType>(&rhs, 1);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator==(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs == ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator==(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs == static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr bool operator==(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs == ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr bool operator!=(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareNotEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator!=(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(isNotConst<DataType>) {
			return lexicographicalCompareNotEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator!=(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs != ArrayView<const DataType>(&rhs, 1);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator!=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs != ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator!=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs != static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr bool operator!=(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs != ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr bool operator<(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareLessThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator<(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(isNotConst<DataType>) {
			return lexicographicalCompareLessThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator<(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs < ArrayView<const DataType>(&rhs, 1);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs < ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs < static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr bool operator<(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs < ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr bool operator<=(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareLessThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator<=(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(isNotConst<DataType>) {
			return lexicographicalCompareLessThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator<=(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs <= ArrayView<const DataType>(&rhs, 1);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <= ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <= static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr bool operator<=(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs <= ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr bool operator>(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareGreaterThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator>(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(isNotConst<DataType>) {
			return lexicographicalCompareGreaterThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator>(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs > ArrayView<const DataType>(&rhs, 1);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs > ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs > static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr bool operator>(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs > ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr bool operator>=(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareGreaterThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator>=(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(isNotConst<DataType>) {
			return lexicographicalCompareGreaterThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr bool operator>=(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs >= ArrayView<const DataType>(&rhs, 1);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs >= ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs >= static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr bool operator>=(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs >= ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr std::strong_ordering operator<=>(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareSpaceship<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr std::strong_ordering operator<=>(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(isNotConst<DataType>) {
			return lexicographicalCompareSpaceship<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr std::strong_ordering operator<=>(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs <=> ArrayView<const DataType>(&rhs, 1);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr std::strong_ordering operator<=>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <=> ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr std::strong_ordering operator<=>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <=> static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr bool operator<=>(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs <=> ArrayView<const DataType>(rhs.begin(), rhs.size());
		}
	};

	template<class Container>
	concept IsContainerArrayViewConstructable =
		HasIteratorType<Container> &&
		HasBeginIteratorToPtr<Container> &&
		HasSizeMethod<Container> &&
		std::random_access_iterator<typename Container::iterator>;

	template<class Container>
		requires(IsArrayViewLike<Container, typename Container::value_type>)
	constexpr ArrayView<typename Container::value_type> makeArrayView(Container& container) noexcept {
		return ArrayView<typename Container::value_type>(container.data(), container.size());
	}

	template<class Container>
		requires(IsContainerArrayViewConstructable<Container> && !IsArrayViewLike<Container, typename ContainerIteratorTraits<Container>::value_type>)
	constexpr ArrayView<typename ContainerIteratorTraits<Container>::value_type> makeArrayView(Container& container) noexcept {
		return ArrayView<typename ContainerIteratorTraits<Container>::value_type>(&*container.begin(), container.size());
	}

	struct SpatialRegion {
	public:
		Size offset;
		Size extent;
		Size size;
	public:
		//constructor
		SpatialRegion() = default;
		SpatialRegion(const Size offset, const Size extent, const Size size) : offset(offset), extent(extent), size(size) {}
	};

	namespace impl {
		struct MDArrayViewDim {};

		template<class DataType, MDArrayViewDim... Dimensions>
		class OffsetMDArrayViewDimensions {
		public:
			using value_type = DataType;
			using reference = DataType&;
			using const_reference = const DataType&;
			using pointer = DataType*;
			using const_pointer = const DataType*;
			using optional_pointer = Option<DataType*>;
			using optional_const_pointer = Option<const DataType*>;
			using difference_type = PtrDiff;
			using size_type = Size;

			//movement info  
			constexpr static bool triviallyRelocatable = IsTriviallyRelocatable<DataType>;
			constexpr static bool triviallyDefaultConstructible = IsTriviallyDefaultConstructible<DataType>;
			constexpr static bool triviallyCompareable = IsTriviallyCompareable<DataType>;
			constexpr static bool triviallyDestructible = IsTriviallyDestructible<DataType>;

			using BaseArrayView = ArrayView<DataType>;
		private:
			DataType* dataPtr;
			SpatialRegion spatialRegions[sizeof...(Dimensions)];

			constexpr void setSpatialRegions(const CustomAlwaysType<MDArrayViewDim, Dimensions, Size>&... regions) {
				const SpatialRegion regionsArray[] = { regions... };
				uninitializedCopyCountNoOverlap<const SpatialRegion*, SpatialRegion*>(regionsArray, spatialRegions, numberOfDimension());
			}

		public:
			//constructor
			constexpr OffsetMDArrayViewDimensions() : dataPtr(), spatialRegions() {}
			constexpr OffsetMDArrayViewDimensions(const OffsetMDArrayViewDimensions& other) noexcept = default;
			constexpr OffsetMDArrayViewDimensions(OffsetMDArrayViewDimensions&& other) noexcept = default;
			constexpr OffsetMDArrayViewDimensions(std::initializer_list<DataType> dataSrc, const CustomAlwaysType<MDArrayViewDim, Dimensions, SpatialRegion>&... regions) 
				noexcept requires(isConst<DataType>) : dataPtr(dataSrc) {
				setSpatialRegions(regions...);
			}
			constexpr OffsetMDArrayViewDimensions(DataType* dataSrc, const CustomAlwaysType<MDArrayViewDim, Dimensions, SpatialRegion>&... regions) noexcept : dataPtr(dataSrc) {
				setSpatialRegions(regions...);
			}
		
			//destructor 
			constexpr ~OffsetMDArrayViewDimensions() noexcept = default;

			//util
			constexpr OffsetMDArrayViewDimensions& self() noexcept { return *this; }
			constexpr const OffsetMDArrayViewDimensions& self() const noexcept { return *this; }

			//index calculation 
			constexpr Size calculateLinearIndex(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indexesArgs) const noexcept {
				const Size indexes[] = { indexesArgs... };
				Size index = 0;
				for (Size i = 0; i < numberOfDimension(); ++i) {
					Size mul = 1;
					for (Size j = 0; j < i; ++j) {
						mul *= spatialRegions[j].size;
					}
					index += (indexes[i] + spatialRegions[i].offset) * mul;
				}
				return index;
			}
			constexpr Size calculateLinearIndexBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indexesArgs) const noexcept {
				const Size indexes[] = { indexesArgs... };
				Size index = 0;
				for (Size i = 0; i < numberOfDimension(); ++i) {
					Size mul = 1;
					for (Size j = 0; j < i; ++j) {
						mul *= spatialRegions[j].size;
					}
					index += (std::min<Size>(indexes[i], spatialRegions[i].extent) + spatialRegions[i].offset) * mul;
				}
				return index;
			}

			//element access
			constexpr reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(isNotConst<DataType>) { return data()[calculateLinearIndex(indices...)]; }
			constexpr const_reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndex(indices...)]; }
			
			constexpr reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(isNotConst<DataType>) { return data()[calculateLinearIndexBounded(indices...)]; }
			constexpr const_reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndexBounded(indices...)]; }

			constexpr pointer data() noexcept requires(isNotConst<DataType>) { return dataPtr; };
			constexpr const_pointer data() const noexcept { return dataPtr; };

			//capacity 
			constexpr bool isEmpty() const noexcept { return !dataPtr; }
			constexpr bool isNotEmpty() const noexcept { return !isEmpty(); }
			constexpr operator bool() const noexcept { return isNotEmpty(); }

			consteval static Size numberOfDimension() noexcept {
				return sizeof...(Dimensions);
			}
			template<Size dimensionIndex>
			constexpr Size dimensionSize() const noexcept {
				static_assert(dimensionIndex < numberOfDimension(), "natl: OffsetMDArrayViewDimensions dimensionSize() call | error: dimensionIndex >= numberOfDimension");
				return spatialRegions[dimensionIndex].extent;
			}
			constexpr Size dimensionSize(const Size index) const noexcept {
				return spatialRegions[std::min<Size>(index, numberOfDimension())].extent;
			}
			constexpr Size size() noexcept {
				Size calculatedSize = 1;
				for (Size i = 0; i < numberOfDimension(); i++) {
					calculatedSize *= spatialRegions[i].extent;
				}
				return calculatedSize;
			}


			//assignment 
			constexpr OffsetMDArrayViewDimensions& operator=(const OffsetMDArrayViewDimensions& other) noexcept = default;
			constexpr OffsetMDArrayViewDimensions& operator=(OffsetMDArrayViewDimensions&& other) noexcept = default;

			//operations
			constexpr void fill(const DataType& value) noexcept requires(isNotConst<DataType>) {
				const Size count = size();
				for (Size fillIndex = 0; fillIndex < count; fillIndex++) {
					Size linearIndex = fillIndex;
					Size coordinates[numberOfDimension()];
					for (int i = numberOfDimension(); i > 0; --i) {
						coordinates[i - 1] = linearIndex % spatialRegions[i - 1].extent;
						linearIndex /= spatialRegions[i - 1].extent;
					}

					Size index = 0;
					for (Size i = 0; i < numberOfDimension(); ++i) {
						Size mul = 1;
						for (Size j = 0; j < i; ++j) {
							mul *= spatialRegions[j].size;
						}
						index += (coordinates[i] + spatialRegions[i].offset) * mul;
					}

					dataPtr[index] = value;
				}
			}
			constexpr void swap(OffsetMDArrayViewDimensions& other) noexcept {
				swap<OffsetMDArrayViewDimensions>(self(), other);
			}
		};


		template <typename DataType, typename Seq> struct OffsetsMDArrayViewImpl;
		template <typename DataType, Size... Is>
		struct OffsetsMDArrayViewImpl<DataType, std::index_sequence<Is...>> {
			using OffsetsMDArrayViewType = 
				OffsetMDArrayViewDimensions<
				DataType, 
				AlwaysType<Is, MDArrayViewDim>{} ...
				> ;
		};

		template<class DataType, MDArrayViewDim... Dimensions>
		class MDArrayViewDimensions {
		public:
			using value_type = DataType;
			using reference = DataType&;
			using const_reference = const DataType&;
			using pointer = DataType*;
			using const_pointer = const DataType*;
			using optional_pointer = Option<DataType*>;
			using optional_const_pointer = Option<const DataType*>;
			using difference_type = PtrDiff;
			using size_type = Size;

			//movement info  
			constexpr static bool triviallyRelocatable = true;
			constexpr static bool triviallyDefaultConstructible = true;
			constexpr static bool triviallyCompareable = false;
			constexpr static bool triviallyDestructible = true;

			using BaseArrayView = ArrayView<DataType>;
		private:
			DataType* dataPtr;
			Size dimensionsSizes[sizeof...(Dimensions)];
		
			constexpr void setDimensionSizes(const CustomAlwaysType<MDArrayViewDim, Dimensions, Size>&... dimensionsSizesInput) {
				const Size dimensionsSizeArray[] = { dimensionsSizesInput... };
				uninitializedCopyCountNoOverlap<const SpatialRegion*, SpatialRegion*>(dimensionsSizeArray, dimensionsSizes, numberOfDimension());
			}

		public:
			//constructor
			constexpr MDArrayViewDimensions() : dataPtr(), dimensionsSizes() {}
			constexpr MDArrayViewDimensions(const MDArrayViewDimensions& other) noexcept = default;
			constexpr MDArrayViewDimensions(MDArrayViewDimensions&& other) noexcept = default;
			constexpr MDArrayViewDimensions(std::initializer_list<DataType> dataSrc, const CustomAlwaysType<MDArrayViewDim, Dimensions, Size>&... dimensionsSizesInput) 
				noexcept requires(isConst<DataType>) : dataPtr(dataSrc) {
				setDimensionSizes(dimensionsSizesInput...);
			}
			constexpr MDArrayViewDimensions(DataType* dataSrc, const CustomAlwaysType<MDArrayViewDim, Dimensions, Size>&... dimensionsSizesInput) noexcept : dataPtr(dataSrc) {
				setDimensionSizes(dimensionsSizesInput...);
			}

			//destructor 
			constexpr ~MDArrayViewDimensions() noexcept = default;

			//util
			constexpr MDArrayViewDimensions& self() noexcept { return *this; }
			constexpr const MDArrayViewDimensions& self() const noexcept { return *this; }

			//index calculation 
			constexpr Size calculateLinearIndex(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indexesArgs) const noexcept {
				const Size indexes[] = { indexesArgs... };
				Size index = 0;
				for (Size i = 0; i < numberOfDimension(); ++i) {
					Size mul = 1;
					for (Size j = 0; j < i; ++j) {
						mul *= dimensionsSizes[j];
					}
					index += indexes[i] * mul;
				}
				return index;
			}
			constexpr Size calculateLinearIndexBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indexesArgs) const noexcept {
				const Size indexes[] = { indexesArgs... };
				Size index = 0;
				for (Size i = 0; i < numberOfDimension(); ++i) {
					Size mul = 1;
					for (Size j = 0; j < i; ++j) {
						mul *= dimensionsSizes[j];
					}
					index += min<Size>(indexes[i], dimensionsSizes[i]) + mul;
				}
				return index;
			}

			//element access
			constexpr reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(isNotConst<DataType>) { return data()[calculateLinearIndex(indices...)]; }
			constexpr const_reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndex(indices...)]; }

			constexpr reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(isNotConst<DataType>) { return data()[calculateLinearIndexBounded(indices...)]; }
			constexpr const_reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndexBounded(indices...)]; }

			constexpr pointer data() noexcept requires(isNotConst<DataType>) { return dataPtr; };
			constexpr const_pointer data() const noexcept { return dataPtr; };

			//capacity 
			constexpr bool isEmpty() const noexcept { return !dataPtr; }
			constexpr bool isNotEmpty() const noexcept { return !isEmpty(); }
			constexpr operator bool() const noexcept { return isNotEmpty(); }

			consteval static Size numberOfDimension() noexcept {
				return sizeof...(Dimensions);
			}
			template<Size dimensionIndex>
			constexpr Size dimensionSize() const noexcept {
				static_assert(dimensionIndex < numberOfDimension(), "natl: MDArrayViewDimensions dimensionSize() call | error: dimensionIndex >= numberOfDimension");
				return dimensionsSizes[dimensionIndex];
			}
			constexpr Size dimensionSize(const Size index) const noexcept {
				return dimensionsSizes[std::min<Size>(index, numberOfDimension())];
			}
			constexpr Size size() noexcept {
				Size calculatedSize = 1;
				for (Size i = 0; i < numberOfDimension(); i++) {
					calculatedSize *= dimensionsSizes[i];
				}
				return calculatedSize;
			}

			//assignment 
			constexpr MDArrayViewDimensions& operator=(const MDArrayViewDimensions& other) noexcept = default;
			constexpr MDArrayViewDimensions& operator=(MDArrayViewDimensions&& other) noexcept = default;

			//operations
			constexpr void fill(const DataType& value) noexcept requires(isNotConst<DataType>) {
				fillCount<DataType*, DataType>(data(), value, size());
			}
			constexpr void swap(MDArrayViewDimensions& other) noexcept {
				swap<MDArrayViewDimensions>(self(), other);
			}
		};


		template <typename DataType, typename Seq> struct MDArrayViewImpl;
		template <typename DataType, Size... Is>
		struct MDArrayViewImpl<DataType, std::index_sequence<Is...>> {
			using OffsetsMDArrayViewType =
				MDArrayViewDimensions<
				DataType,
				AlwaysType<Is, MDArrayViewDim>{} ...
				> ;
		};
	}

	template <typename DataType, Size DimensionNumber>
	class OffsetsMDArrayView : public impl::OffsetsMDArrayViewImpl<DataType, std::make_index_sequence<DimensionNumber>>::OffsetsMDArrayViewType {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = PtrDiff;
		using size_type = Size;

		using OffsetsMDArrayViewImplType = impl::OffsetsMDArrayViewImpl<DataType, std::make_index_sequence<DimensionNumber>>::OffsetsMDArrayViewType;
		using BaseArrayView = OffsetsMDArrayViewImplType::BaseArrayView;
	public:
		//movement info  
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = true;

		//constructor
		constexpr OffsetsMDArrayView() noexcept = default;
		constexpr OffsetsMDArrayView(const OffsetsMDArrayView& other) noexcept = default;
		constexpr OffsetsMDArrayView(OffsetsMDArrayView&& other) noexcept = default;
		constexpr OffsetsMDArrayView(std::initializer_list<DataType> dataSrc) noexcept requires(isConst<DataType>) : OffsetsMDArrayViewImplType(dataSrc) {}

		//util 
		constexpr OffsetsMDArrayView& self() noexcept { return *this; }
		constexpr const OffsetsMDArrayView& self() const noexcept { return *this; }

		constexpr OffsetsMDArrayViewImplType& selfAsOffsetsMDArrayViewImpl() noexcept { return static_cast<const OffsetsMDArrayViewImplType&>(*this); }
		constexpr const OffsetsMDArrayViewImplType& selfAsOffsetsMDArrayViewImpl() const noexcept { return static_cast<const OffsetsMDArrayViewImplType&>(*this); }

		constexpr OffsetsMDArrayView& operator=(const OffsetsMDArrayView& other) noexcept = default;
		constexpr OffsetsMDArrayView& operator=(OffsetsMDArrayView&& other) noexcept = default;
	};

	template <typename DataType, Size DimensionNumber>
	class MDArrayView : public impl::MDArrayViewImpl<DataType, std::make_index_sequence<DimensionNumber>>::MDArrayViewType {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = PtrDiff;
		using size_type = Size;

		using MDArrayViewImplType = impl::MDArrayViewImpl<DataType, std::make_index_sequence<DimensionNumber>>::MDArrayViewType;
		using BaseArrayView = MDArrayViewImplType::BaseArrayView;
	public:
		//movement info  
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = true;

		//constructor
		constexpr MDArrayView() noexcept = default;
		constexpr MDArrayView(const MDArrayView& other) noexcept = default;
		constexpr MDArrayView(MDArrayView&& other) noexcept = default;
		using MDArrayViewImplType::MDArrayViewImplType;

		//util 
		constexpr MDArrayView& self() noexcept { return *this; }
		constexpr const MDArrayView& self() const noexcept { return *this; }

		constexpr MDArrayViewImplType& selfAsMDArrayViewImpl() noexcept { return static_cast<const MDArrayViewImplType&>(*this); }
		constexpr const MDArrayViewImplType& selfAsMDArrayViewImpl() const noexcept { return static_cast<const MDArrayViewImplType&>(*this); }

		constexpr MDArrayView& operator=(const MDArrayView& other) noexcept = default;
		constexpr MDArrayView& operator=(MDArrayView&& other) noexcept = default;
	};
}