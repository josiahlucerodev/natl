#pragma once

//std
#include <initializer_list>

//own
#include "../util/typeTraits.h"
#include "../util/basicTypes.h"
#include "../util/iterators.h"
#include "../util/limits.h"
#include "../util/typePack.h"
#include "../util/dataMovement.h"
#include "../util/lexicographicalCompare.h"
#include "../util/compare.h"
#include "../fundamental/option.h"
#include "container.h"

//interface
namespace natl {
	template<typename DataType>
	struct ArrayView {
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

		using iterator = RandomAccessIterator<DataType>;
		using const_iterator = RandomAccessIterator<const DataType>;
		using reverse_iterator = ReverseRandomAccessIterator<DataType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const DataType>;

		constexpr static Size npos = Limits<Size>::max();
	private:
		mutable pointer dataPtr;
		Size arraySize;
	public:
		//constructor 
		constexpr ArrayView() : dataPtr(nullptr), arraySize(0) {}
		constexpr ArrayView(pointer ptr, const size_type size) : dataPtr(ptr), arraySize(size) {}
		constexpr ArrayView(ArrayView& other) noexcept : dataPtr(other.data()), arraySize(other.size()) {}
		constexpr ArrayView(const ArrayView& other) noexcept : dataPtr(other.data()), arraySize(other.size()) {}
		constexpr ArrayView(std::initializer_list<DataType> initList) noexcept : dataPtr(initList.begin()), arraySize(initList.size()) {}
		template<typename ArrayViewLike>
			requires(!IsSameC<ArrayViewLike, std::initializer_list<DataType>>&& IsArrayViewLike<ArrayViewLike, DataType>)
		constexpr ArrayView(const ArrayViewLike& arrayViewLike) noexcept : dataPtr(arrayViewLike.data()), arraySize(arrayViewLike.size()) {}

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
		constexpr void operator=(std::initializer_list<DataType> initList) noexcept requires(IsConst<DataType>) {
			dataPtr = initList.begin();
			arraySize = initList.size();
			return self();
		}

		//conversion 
		constexpr operator ArrayView<const DataType>() requires(IsNotConst<DataType>) {
			return ArrayView<const DataType>(data(), size());
		}

		//iterators 
		constexpr pointer beginPtr() noexcept requires(IsNotConst<DataType>) { return dataPtr; }
		constexpr const_pointer beginPtr() const noexcept { return dataPtr; }
		constexpr pointer endPtr() noexcept requires(IsNotConst<DataType>) { return dataPtr + arraySize; }
		constexpr const_pointer endPtr() const noexcept { return dataPtr + arraySize; }

		constexpr iterator begin() noexcept requires(IsNotConst<DataType>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(IsNotConst<DataType>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(IsNotConst<DataType>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(IsNotConst<DataType>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		//element access
		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return arraySize ? arraySize - 1 : 0; }

		constexpr reference front() noexcept requires(IsNotConst<DataType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept requires(IsNotConst<DataType>) { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr reference at(const size_type index) noexcept requires(IsNotConst<DataType>) { return dataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return dataPtr[index]; };

		constexpr reference operator[](const size_type index) noexcept requires(IsNotConst<DataType>) { return dataPtr[index]; };
		constexpr const_reference operator[](const size_type index) const noexcept { return dataPtr[index]; };

		constexpr pointer data() noexcept { return dataPtr; };
		constexpr pointer data() const noexcept { return dataPtr; };

		//observers
		constexpr size_type size() const noexcept { return arraySize; }
		constexpr size_type size_bytes() const noexcept { return arraySize * sizeof(DataType); }

		constexpr Bool empty() const noexcept { return arraySize == 0 || dataPtr == nullptr; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return arraySize != 0 && dataPtr != nullptr; }
		constexpr operator Bool() const noexcept { return isNotEmpty(); }

		//subviews
		constexpr ArrayView first(const size_type count) noexcept requires(IsNotConst<DataType>) {
			return ArrayView(data(), min<Size>(count, size()));
		}
		constexpr ArrayView first(const size_type count) const noexcept {
			return ArrayView(data(), min<Size>(count, size()));
		}

		constexpr ArrayView last(const size_type count) noexcept requires(IsNotConst<DataType>) {
			const Size newCount = min<Size>(count, size());
			return ArrayView(data() + newCount, size() - newCount);
		}
		constexpr ArrayView last(const size_type count) const noexcept {
			const Size newCount = min<Size>(count, size());
			return ArrayView(data() + newCount, size() - newCount);
		}

		constexpr ArrayView subview(size_type offset, size_type count = npos) noexcept requires(IsNotConst<DataType>) {
			offset = min<Size>(offset, size());
			count = min<Size>(count, size() - offset);
			return ArrayView(data() + offset, count);
		}

		constexpr ArrayView<const DataType> subview(size_type offset, size_type count = npos) const noexcept {
			offset = min<Size>(offset, size());
			count = min<Size>(count, size() - offset);
			return ArrayView<const DataType>(data() + offset, count);
		}

		//as bytes
		ArrayView<const Byte> as_bytes() const requires(IsNotConst<DataType>) {
			return ArrayView<const Byte>(reinterpret_cast<const Byte*>(data()), size_bytes());
		}
		ArrayView<Byte> as_writable_bytes() const noexcept {
			return ArrayView<Byte>(reinterpret_cast<Byte*>(data()), size_bytes());
		}


		//special element access
		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp<Size>(index, frontIndex(), backIndex()); }

		constexpr reference atClamped(const size_type index) noexcept requires(IsNotConst<DataType>) { return dataPtr[clampIndex(index)]; }
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

		constexpr Bool has(const size_type index) const noexcept { return index < arraySize; }
		constexpr Bool notHave(const size_type index) const noexcept { return index >= arraySize; }

		//operations
		constexpr void fill(const DataType& value) noexcept requires(IsNotConst<DataType>) {
			fillCount<pointer, DataType>(data(), value, size());
		}
		constexpr void swap(ArrayView& other) noexcept {
			natl::swap<ArrayView>(self(), other);
		}

		//compare operators 
		friend constexpr Bool operator==(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator==(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(IsNotConst<DataType>) {
			return lexicographicalCompareEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator==(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs == ArrayView<const DataType>(&rhs, 1);
		}
		template<typename ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator==(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs == ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<typename ArrayViewLike>
			requires(IsConvertible<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator==(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs == static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr Bool operator==(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs == ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator!=(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareNotEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator!=(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(IsNotConst<DataType>) {
			return lexicographicalCompareNotEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator!=(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs != ArrayView<const DataType>(&rhs, 1);
		}
		template<typename ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator!=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs != ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<typename ArrayViewLike>
			requires(IsConvertibleC<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator!=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs != static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr Bool operator!=(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs != ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator<(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareLessThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator<(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(IsNotConst<DataType>) {
			return lexicographicalCompareLessThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator<(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs < ArrayView<const DataType>(&rhs, 1);
		}
		template<typename ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator<(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs < ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<typename ArrayViewLike>
			requires(IsConvertibleC<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator<(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs < static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr Bool operator<(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs < ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator<=(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareLessThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator<=(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(IsNotConst<DataType>) {
			return lexicographicalCompareLessThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator<=(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs <= ArrayView<const DataType>(&rhs, 1);
		}
		template<typename ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator<=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <= ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<typename ArrayViewLike>
			requires(IsConvertibleC<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator<=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <= static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr Bool operator<=(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs <= ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator>(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareGreaterThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator>(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(IsNotConst<DataType>) {
			return lexicographicalCompareGreaterThan<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator>(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs > ArrayView<const DataType>(&rhs, 1);
		}
		template<typename ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs > ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<typename ArrayViewLike>
			requires(IsConvertibleC<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs > static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr Bool operator>(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs > ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr Bool operator>=(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareGreaterThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator>=(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(IsNotConst<DataType>) {
			return lexicographicalCompareGreaterThanEqual<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr Bool operator>=(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs >= ArrayView<const DataType>(&rhs, 1);
		}
		template<typename ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator>=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs >= ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<typename ArrayViewLike>
			requires(IsConvertibleC<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr Bool operator>=(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs >= static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr Bool operator>=(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs >= ArrayView<const DataType>(rhs.begin(), rhs.size());
		}

		friend constexpr StrongOrdering operator<=>(const ArrayView& lhs, const ArrayView& rhs) noexcept {
			return lexicographicalCompareSpaceship<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr StrongOrdering operator<=>(const ArrayView& lhs, const ArrayView<const DataType>& rhs) noexcept requires(IsNotConst<DataType>) {
			return lexicographicalCompareSpaceship<const DataType*, const DataType*>(lhs.data(), lhs.size(), rhs.data(), rhs.size());
		}
		friend constexpr StrongOrdering operator<=>(const ArrayView& lhs, const DataType rhs) noexcept {
			return lhs <=> ArrayView<const DataType>(&rhs, 1);
		}
		template<typename ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr StrongOrdering operator<=>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <=> ArrayView<const DataType>(rhs.data(), rhs.size());
		}
		template<typename ArrayViewLike>
			requires(IsConvertibleC<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr StrongOrdering operator<=>(const ArrayView& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs <=> static_cast<const ArrayView<DataType>>(rhs);
		}
		friend constexpr Bool operator<=>(const ArrayView& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs <=> ArrayView<const DataType>(rhs.begin(), rhs.size());
		}
	};

	template<typename DataType>
	using ConstArrayView = ArrayView<const DataType>;

	template<typename DataType>
	struct IsCopyableStorageDstT<ArrayView<DataType>> : TrueType {};

	template<typename Container>
	concept IsContainerArrayViewConstructable =
		HasIteratorType<Container> &&
		HasBeginIteratorToPtr<Container> &&
		HasSizeMethod<Container> &&
		IsRandomAccessIteratorC<typename Container::iterator>;

	template<typename Container>
		requires(IsArrayViewLike<Container, typename Container::value_type>)
	constexpr ArrayView<typename Container::value_type> makeArrayView(Container& container) noexcept {
		return ArrayView<typename Container::value_type>(container.data(), container.size());
	}

	template<typename Container>
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
		SpatialRegion(const Size offsetIn, const Size extentIn, const Size sizeIn) : offset(offsetIn), extent(extentIn), size(sizeIn) {}
	};

	namespace impl {
		struct MDArrayViewDim {};

		template<typename DataType, MDArrayViewDim... Dimensions>
		struct OffsetMDArrayViewDimensions {
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
				noexcept requires(IsConst<DataType>) : dataPtr(dataSrc) {
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
					index += (natl::min<Size>(indexes[i], spatialRegions[i].extent) + spatialRegions[i].offset) * mul;
				}
				return index;
			}

			//element access
			constexpr reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(IsNotConst<DataType>) { return data()[calculateLinearIndex(indices...)]; }
			constexpr const_reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndex(indices...)]; }
			
			constexpr reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(IsNotConst<DataType>) { return data()[calculateLinearIndexBounded(indices...)]; }
			constexpr const_reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndexBounded(indices...)]; }

			constexpr pointer data() noexcept requires(IsNotConst<DataType>) { return dataPtr; };
			constexpr const_pointer data() const noexcept { return dataPtr; };

			//capacity 
			constexpr Bool isEmpty() const noexcept { return !dataPtr; }
			constexpr Bool isNotEmpty() const noexcept { return !isEmpty(); }
			constexpr operator Bool() const noexcept { return isNotEmpty(); }

			consteval static Size numberOfDimension() noexcept {
				return sizeof...(Dimensions);
			}
			template<Size dimensionIndex>
			constexpr Size dimensionSize() const noexcept {
				static_assert(dimensionIndex < numberOfDimension(), "natl: OffsetMDArrayViewDimensions dimensionSize() call | error: dimensionIndex >= numberOfDimension");
				return spatialRegions[dimensionIndex].extent;
			}
			constexpr Size dimensionSize(const Size index) const noexcept {
				return spatialRegions[natl::min<Size>(index, numberOfDimension())].extent;
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
			constexpr void fill(const DataType& value) noexcept requires(IsNotConst<DataType>) {
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
				natl::swap<OffsetMDArrayViewDimensions>(self(), other);
			}
		};

		template <typename DataType, typename Seq> struct OffsetsMDArrayViewImpl;
		template <typename DataType, Size... Is>
		struct OffsetsMDArrayViewImpl<DataType, IndexSequence<Is...>> {
			using OffsetsMDArrayViewType = 
				OffsetMDArrayViewDimensions<
				DataType, 
				AlwaysType<Is, MDArrayViewDim>{} ...
				> ;
		};

		template<typename DataType, MDArrayViewDim... Dimensions>
		struct MDArrayViewDimensions {
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
				noexcept requires(IsConst<DataType>) : dataPtr(dataSrc) {
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
			constexpr reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(IsNotConst<DataType>) { return data()[calculateLinearIndex(indices...)]; }
			constexpr const_reference at(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndex(indices...)]; }

			constexpr reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) noexcept requires(IsNotConst<DataType>) { return data()[calculateLinearIndexBounded(indices...)]; }
			constexpr const_reference atBounded(CustomAlwaysType<MDArrayViewDim, Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndexBounded(indices...)]; }

			constexpr pointer data() noexcept requires(IsNotConst<DataType>) { return dataPtr; };
			constexpr const_pointer data() const noexcept { return dataPtr; };

			//capacity 
			constexpr Bool isEmpty() const noexcept { return !dataPtr; }
			constexpr Bool isNotEmpty() const noexcept { return !isEmpty(); }
			constexpr operator Bool() const noexcept { return isNotEmpty(); }

			consteval static Size numberOfDimension() noexcept {
				return sizeof...(Dimensions);
			}
			template<Size dimensionIndex>
			constexpr Size dimensionSize() const noexcept {
				static_assert(dimensionIndex < numberOfDimension(), "natl: MDArrayViewDimensions dimensionSize() call | error: dimensionIndex >= numberOfDimension");
				return dimensionsSizes[dimensionIndex];
			}
			constexpr Size dimensionSize(const Size index) const noexcept {
				return dimensionsSizes[natl::min<Size>(index, numberOfDimension())];
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
			constexpr void fill(const DataType& value) noexcept requires(IsNotConst<DataType>) {
				fillCount<DataType*, DataType>(data(), value, size());
			}
			constexpr void swap(MDArrayViewDimensions& other) noexcept {
				natl::swap<MDArrayViewDimensions>(self(), other);
			}
		};


		template <typename DataType, typename Seq> struct MDArrayViewImpl;
		template <typename DataType, Size... Is>
		struct MDArrayViewImpl<DataType, IndexSequence<Is...>> {
			using OffsetsMDArrayViewType =
				MDArrayViewDimensions<
				DataType,
				AlwaysType<Is, MDArrayViewDim>{} ...
				> ;
		};
	}

	template<typename DataType>
	struct IsTriviallyCompareableV<ArrayView<DataType>>
		: FalseType {};

	template<typename DataType>
	struct IsTriviallyRelocatableV<ArrayView<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyConstructibleV<ArrayView<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyDestructibleV<ArrayView<DataType>>
		: TrueType {};

	template<typename DataType>
	struct IsTriviallyConstRefConstructibleV<ArrayView<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyMoveConstructibleV<ArrayView<DataType>>
		: TrueType {};

	template<typename DataType>
	struct IsTriviallyConstRefAssignableV<ArrayView<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyMoveAssignableV<ArrayView<DataType>>
		: TrueType {};

	template <typename DataType, Size DimensionNumber>
	struct OffsetsMDArrayView : public impl::OffsetsMDArrayViewImpl<DataType, MakeIndexSequence<DimensionNumber>>::OffsetsMDArrayViewType {
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

		using OffsetsMDArrayViewImplType = impl::OffsetsMDArrayViewImpl<DataType, MakeIndexSequence<DimensionNumber>>::OffsetsMDArrayViewType;
		using BaseArrayView = OffsetsMDArrayViewImplType::BaseArrayView;
	public:
		//constructor
		constexpr OffsetsMDArrayView() noexcept = default;
		constexpr OffsetsMDArrayView(const OffsetsMDArrayView& other) noexcept = default;
		constexpr OffsetsMDArrayView(OffsetsMDArrayView&& other) noexcept = default;
		constexpr OffsetsMDArrayView(std::initializer_list<DataType> dataSrc) noexcept requires(IsConst<DataType>) : OffsetsMDArrayViewImplType(dataSrc) {}

		//util 
		constexpr OffsetsMDArrayView& self() noexcept { return *this; }
		constexpr const OffsetsMDArrayView& self() const noexcept { return *this; }

		constexpr OffsetsMDArrayViewImplType& selfAsOffsetsMDArrayViewImpl() noexcept { return static_cast<const OffsetsMDArrayViewImplType&>(*this); }
		constexpr const OffsetsMDArrayViewImplType& selfAsOffsetsMDArrayViewImpl() const noexcept { return static_cast<const OffsetsMDArrayViewImplType&>(*this); }

		constexpr OffsetsMDArrayView& operator=(const OffsetsMDArrayView& other) noexcept = default;
		constexpr OffsetsMDArrayView& operator=(OffsetsMDArrayView&& other) noexcept = default;
	};

	template <typename DataType, Size DimensionNumber>
	struct MDArrayView : public impl::MDArrayViewImpl<DataType, MakeIndexSequence<DimensionNumber>>::MDArrayViewType {
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

		using MDArrayViewImplType = impl::MDArrayViewImpl<DataType, MakeIndexSequence<DimensionNumber>>::MDArrayViewType;
		using BaseArrayView = MDArrayViewImplType::BaseArrayView;
	public:
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

	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyCompareableV<MDArrayView<DataType, DimensionNumber>>
		: FalseType {};

	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyRelocatableV<MDArrayView<DataType, DimensionNumber>>
		: TrueType {};
	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyConstructibleV<MDArrayView<DataType, DimensionNumber>>
		: TrueType {};
	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyDestructibleV<MDArrayView<DataType, DimensionNumber>>
		: TrueType {};

	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyConstRefConstructibleV<MDArrayView<DataType, DimensionNumber>>
		: TrueType {};
	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyMoveConstructibleV<MDArrayView<DataType, DimensionNumber>>
		: TrueType {};

	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyConstRefAssignableV<MDArrayView<DataType, DimensionNumber>>
		: TrueType {};
	template<typename DataType, Size DimensionNumber>
	struct IsTriviallyMoveAssignableV<MDArrayView<DataType, DimensionNumber>>
		: TrueType {};

	template<typename ArrayViewLike>
		requires(IsBroadlyArrayViewLike<ArrayViewLike>)
	constexpr Bool isInRange(ArrayViewLike arrayViewLike, const Size pos) noexcept {
		return pos < arrayViewLike.size();
	}
}