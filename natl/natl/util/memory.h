#pragma once 

//@begin_non_modules
//std
#include <memory>
//@end_non_modules

//own
#include "expect.h"
#include "basicTypes.h"
#include "error.h"
#include "bits.h"
#include "numerics.h"

//@export
namespace natl {
	//Constants
	constexpr inline Size cacheLine = 128;
	constexpr inline Size pageSize = 4096;
	constexpr inline Size pageSize64k = 65536;
	constexpr inline Size pagePrefixSize = 12;

	//Ptr
	template<typename DataType>
	void* castToVoidPtr(DataType* ptr) noexcept {
		return reinterpret_cast<void*>(ptr);
	}
	template<typename DataType>
	const void* castToConstVoidPtr(const DataType* ptr) noexcept {
		return reinterpret_cast<const void*>(ptr);
	}

	constexpr Size ptrDistance(const Byte* first, const Byte* last) noexcept {
		return bitCast<Size>(last - first);
	}

	consteval Size virtualAddressBitsCount() noexcept {
		return 64 - pagePrefixSize;
	}
	consteval Size virtualAddressMask() noexcept {
		return (1 << virtualAddressBitsCount()) - 1;
	}

	//Lifetime
	template<typename DataType>
	DataType* startLifetimeAs(void* ptr) noexcept {
		return std::launder(static_cast<DataType*>(std::memmove(ptr, ptr, sizeof(DataType))));
	}
	template<typename DataType>
	const DataType* startLifetimeAs(const void* ptr) noexcept {
		return std::launder(static_cast<const DataType*>(std::memmove(const_cast<DataType*>(ptr), ptr, sizeof(DataType))));
	}
	template<typename DataType>
	DataType* startLifetimeAsArray(void* ptr, const natl::Size number) noexcept {
		return std::launder(static_cast<DataType*>(std::memmove(ptr, ptr, sizeof(DataType) * number)));
	}
	template<typename DataType>
	const DataType* startLifetimeAsArray(const void* ptr, const natl::Size number) noexcept {
		return std::launder(static_cast<const DataType*>(std::memmove(const_cast<DataType*>(ptr), ptr, sizeof(DataType) * number)));
	}

	//Buffers

	inline Bool buffersOverlap(const Byte* a, Size alen, const Byte* b, Size blen) noexcept {
		const Size aStart = reinterpret_cast<Size>(a);
		const Size aEnd = aStart + alen;
		const Size bStart = reinterpret_cast<Size>(b);
		const Size bEnd = bStart + blen;
		return (aStart < bEnd) && (bStart < aEnd);
	}

	inline Bool buffersOverlapRange(const Byte* aStart, const Byte* aEnd, const Byte* bStart, const Byte* bEnd) noexcept {
		return (aStart < bEnd) && (bStart < aEnd);
	}

	inline Bool buffersOverlapIndex(const Size a, Size alen, const Size b, Size blen) noexcept {
		const Size aStart = a;
		const Size aEnd = aStart + alen;
		const Size bStart = b;
		const Size bEnd = bStart + blen;
		return (aStart < bEnd) && (bStart < aEnd);
	}

	inline Bool buffersOverlapIndexRange(const Size aStart, const Size aEnd, const Size bStart, const Size bEnd) noexcept {
		return (aStart < bEnd) && (bStart < aEnd);
	}

	//Alignment 

	inline Byte* alignToPage(Byte* ptr, Size pageSize) noexcept {
		Size p = bitCast<Size>(ptr);
		Size aligned = (p + pageSize - 1) & ~(pageSize - 1);
		return bitCast<Byte*>(aligned);
	}

	constexpr Byte* alignDown(Byte* ptr, Size alignment) noexcept {
		Size p = bitCast<Size>(ptr);
		p &= ~(alignment - 1);
		return bitCast<Byte*>(p);
	}

	constexpr Size maxAlignment(Size blockSize) noexcept {
		return blockSize & (~blockSize + 1);
	}

	enum struct AlignPtrError {
		invalidAlignment,  
		outOfSpace,  
	};

	struct AlignPtrInfo {
		Byte* alignedPtr;
		Byte* nextPtr;
		natl::Size remainingSpace;
	};

	constexpr AlignPtrInfo createAlignPtrInfo(Byte* ptr, const natl::Size space) noexcept {
		AlignPtrInfo alignPtrInfo;
		alignPtrInfo.alignedPtr = nullptr;
		alignPtrInfo.nextPtr = ptr;
		alignPtrInfo.remainingSpace = space;
		return alignPtrInfo;
	}
	using AlignPtrExpect = natl::Expect<AlignPtrInfo, AlignPtrError>;

	constexpr AlignPtrExpect alignPtr(
		const natl::Size alignment, const natl::Size size, 
		Byte* ptr, const natl::Size space) noexcept {
		AlignPtrInfo alignPtrInfo{};

		if (!isPowerOfTwo<natl::Size>(alignment)) {
			return natl::unexpected(AlignPtrError::invalidAlignment); 
		}

		natl::Size misalignment = static_cast<natl::Size>(natl::bitCast<UIntPtrSized>(ptr)) % alignment;
		natl::Size adjustment = (misalignment == natl::Size(0)) ? natl::Size(0) : (alignment - misalignment);

		if (space < size + adjustment) {
			alignPtrInfo.alignedPtr = nullptr;
			return natl::unexpected(AlignPtrError::outOfSpace);
		}

		alignPtrInfo.alignedPtr = ptr + adjustment;
		alignPtrInfo.nextPtr = alignPtrInfo.alignedPtr + size;
		alignPtrInfo.remainingSpace = space - (adjustment + size);

		return alignPtrInfo;
	}

	template<typename DataType>
	constexpr AlignPtrExpect alignPtrWithArray(const natl::Size number, Byte* ptr, const natl::Size space) noexcept {
		return alignPtr(alignof(DataType), sizeof(DataType) * number, ptr, space);
	}

	template<typename DataType>
	constexpr AlignPtrExpect alignPtrWithType(Byte* ptr, const natl::Size space) noexcept {
		return alignPtr(alignof(DataType), sizeof(DataType), ptr, space);
	}

	constexpr AlignPtrExpect alignPtr(
		const natl::Size alignment, const natl::Size size, 
		const AlignPtrInfo& alignPtrInfo) noexcept {
		return alignPtr(alignment, size, alignPtrInfo.nextPtr, alignPtrInfo.remainingSpace);
	}

	template<typename DataType>
	constexpr AlignPtrExpect alignPtrWithType(const AlignPtrInfo& alignPtrInfo) noexcept {
		return alignPtr(alignof(DataType), sizeof(DataType), alignPtrInfo);
	}

	template<typename DataType>
	constexpr AlignPtrExpect alignPtrWithArray(const natl::Size number, const AlignPtrInfo& alignPtrInfo) noexcept {
		return alignPtr(alignof(DataType), sizeof(DataType) * number, alignPtrInfo);
	}

	constexpr void handelStandardAlignPtrError(const AlignPtrExpect& alignPtrExpect) noexcept {
		if (alignPtrExpect.doesNotHaveValue()) {
			switch (alignPtrExpect.error()) {
			case AlignPtrError::invalidAlignment:
				fatalError("natl: alignPtr error - invalid alignment");
				break;
			case AlignPtrError::outOfSpace:
				fatalError("natl: alignPtr error - out of space");
				break;
			default:
				unreachable();
			}
		}
	}

	//Offset
	constexpr Size memberOffset(const Size sizeA, const Size alignmentB) noexcept {
		return (sizeA + (alignmentB - 1)) & ~(alignmentB - 1);
	}
	template<typename LhsMemberType, typename RhsMemberType> 
	consteval Size memberOffset() noexcept {
		return memberOffset(sizeof(LhsMemberType), alignof(RhsMemberType));
	}

	constexpr Size memberToMemberOffset(const Size sizeA, const Size alignmentB) noexcept {
		return memberOffset(sizeA, alignmentB) - sizeA ;
	}
	template<typename LhsMemberType, typename RhsMemberType>
	consteval Size memberToMemberOffset() noexcept {
		return memberToMemberOffset(sizeof(LhsMemberType), alignof(RhsMemberType));
	}

	//Memory
	template<typename Type>
	constexpr Type* constexprAllocate(const Size count) noexcept {
		return std::allocator<Type>().allocate(static_cast<StdSize>(count));
	}
	template<typename Type>
	constexpr void constexprDeallocate(Type* ptr, Size size) noexcept {
		std::allocator<Type>().deallocate(ptr, size);
	}

	inline Size roundSizeToPage(Size size, Size pageSize) noexcept {
		return ((size + pageSize - 1) / pageSize) * pageSize;
	}

	inline Byte* memcpy(Byte* dest, const Byte* src, Size count) noexcept {
		Byte* d = dest; 
		const Byte* s = src;
		for (Size i = 0; i < count; i++, d++, s++) {
			*d = *s;
		}
		return dest;
	}

	constexpr void zeroMemory(Byte* ptr, Size size) noexcept {
		for (Size i = 0; i < size; i++) {
			ptr[i] = Byte(0);
		}
	}
	constexpr void zeroMemory(ByteAllocResult allocResult) noexcept {
		zeroMemory(allocResult.mPtr, allocResult.mSize);
	}

	//os
	Bool osIsValidPageSize(Size pageSize) noexcept;
	ByteAllocResult osMemoryAllocate(Size alignment, Size size, AllocateFlags flags, Size pageSize) noexcept;
	Bool osMemoryDeallocate(Byte* ptr) noexcept;

	//makes memory read only 
	Bool osMemoryRemapProtect(Byte* donorPtr, Size size) noexcept;

	//makes memory read and write 
	Bool osMemoryRemapUnprotect(Byte* donorPtr, Size size) noexcept;

	//Decommits donorPtr physical memory and remaps its virtual address space to the physcial memory of targetPtr
	Bool osMemoryRemap(Byte* targetPtr, Byte* donorPtr, Size size) noexcept;


}