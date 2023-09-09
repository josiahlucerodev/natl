#pragma once

//std
#include <initializer_list>

//own
#include "typeTraits.h"
#include "iterators.h"
#include "container.h"
#include "option.h"

//interface
namespace natl {
	template <class Type>
	class Partition {
	public:
		using value_type = Type;
		using reference = Type&;
		using const_reference = const Type&;
		using pointer = Type*;
		using const_pointer = const Type*;
		using optional_pointer = Option<Type*>;
		using optional_const_pointer = Option<const Type*>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;

		using iterator = RandomAccessIterator<Type>;
		using const_iterator = RandomAccessIterator<const Type>;
		using reverse_iterator = ReverseRandomAccessIterator<Type>; 
		using const_reverse_iterator = ReverseRandomAccessIterator<const Type>;
	private:
		pointer dataPtr;
		size_type sizePartition;
	public:
		constexpr Partition() = default;
		constexpr ~Partition() = default;
		constexpr Partition(pointer dataPtr, const size_type size) : dataPtr(dataPtr), sizePartition(size) {}
		constexpr Partition(std::initializer_list<Type> initList) 
			noexcept requires(isConst<Type>) : 
			dataPtr(initList.begin()), 
			sizePartition(initList.size()) {}

		constexpr size_type size() const noexcept { return sizePartition; }
		constexpr pointer data() noexcept requires(isNotConst<Type>) { return dataPtr; }
		constexpr const_pointer data() const noexcept { return dataPtr; };

		constexpr pointer beginPtr() noexcept requires(isNotConst<Type>) { return dataPtr; }
		constexpr const_pointer beginPtr() const noexcept { return dataPtr; }
		constexpr pointer endPtr() noexcept requires(isNotConst<Type>)  { return dataPtr + sizePartition; }
		constexpr const_pointer endPtr() const noexcept { return dataPtr + sizePartition; }

		constexpr iterator begin() noexcept requires(isNotConst<Type>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(isNotConst<Type>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(isNotConst<Type>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(isNotConst<Type>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		constexpr bool isEmpty() const noexcept { return !bool(sizePartition); }
		constexpr bool isNotEmpty() const noexcept { return bool(sizePartition); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr reference at(const size_type index) noexcept requires(isNotConst<Type>) { return dataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return dataPtr[index]; };

		constexpr size_type clamp(const size_type value, const size_type min, const size_type max) const noexcept {
			const size_type t = value < min ? min : value;
			return t > max ? max : t;
		}

		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp(index, 0, sizePartition - 1); }

		constexpr reference atClamped(const size_type index) noexcept requires(isNotConst<Type>) { return dataPtr[clampIndex(index)]; }
		constexpr const_pointer atClamped(const size_type index) const noexcept { return dataPtr[clampIndex(index)]; }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return sizePartition ? sizePartition - 1 : 0; }

		constexpr reference front() noexcept requires(isNotConst<Type>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr bool has(const size_type index) const noexcept { return index < sizePartition; }
		constexpr bool notHave(const size_type index) const noexcept { return index >= sizePartition; }

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

		constexpr void assign(std::initializer_list<Type> initList) noexcept requires(isConst<Type>) {
			dataPtr = initList.begin();
			sizePartition = initList.size();
		}

		constexpr void copy(std::initializer_list<Type> initList) noexcept requires(isNotConst<Type>) {
			const size_type count = std::max<size_type>(initList.size(), size());
			for (size_type i = 0; i < count; i++) {
				at(i) = initList[i];
			}
		}
		constexpr bool sizeMatchCopy(std::initializer_list<Type> initList) noexcept requires(isNotConst<Type>) {
			if (initList.size() != size()) { 
				return false; 
			} else {
				copy(initList);
				return true;
			}
		}

		constexpr void operator=(std::initializer_list<Type> initList) noexcept requires(isConst<Type>) {  
			dataPtr = initList.begin();
			sizePartition = initList.size();
		}
	};

	template<class Container>
	concept ContainerPartitionConstructable =
		HasIteratorType<Container> &&
		HasBeginIteratorToPtr<Container> &&
		HasSizeMethod<Container> &&
		std::random_access_iterator<typename Container::iterator>;

	template<class Container>
		requires(ContainerPartitionConstructable<Container>)
	constexpr Partition<typename ContainerIteratorTraits<Container>::value_type> newPartitionFromContainer(Container& container) noexcept {
		return Partition<typename ContainerIteratorTraits<Container>::value_type>(&*container.begin(), container.size());
	}

	template<class Container>
		requires(ContainerPartitionConstructable<Container>)
	constexpr Partition<typename ContainerIteratorTraits<Container>::value_type> makePartition(Container& container) noexcept {
		return Partition<typename ContainerIteratorTraits<Container>::value_type>(&*container.begin(), container.size());
	}
}