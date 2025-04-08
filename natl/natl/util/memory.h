#pragma once 

//own
#include "../fundamental/expect.h"
#include "basicTypes.h"
#include "numerics.h"
#include "error.h"
#include "bits.h"

//interface
namespace natl {
	template<typename DataType>
	void* castToVoidPtr(DataType* ptr) noexcept {
		return reinterpret_cast<void*>(ptr);
	}
	template<typename DataType>
	const void* castToConstVoidPtr(const DataType* ptr) noexcept {
		return reinterpret_cast<const void*>(ptr);
	}

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

	enum struct AlignPtrError {
		invalidAlignment,  
		outOfSpace,  
	};

	struct AlignPtrInfo {
		void* alignedPtr;
		void* nextPtr;
		natl::Size remainingSpace;
	};

	constexpr AlignPtrInfo createAlignPtrInfo(void* ptr, const natl::Size space) noexcept {
		AlignPtrInfo alignPtrInfo;
		alignPtrInfo.alignedPtr = nullptr;
		alignPtrInfo.nextPtr = ptr;
		alignPtrInfo.remainingSpace = space;
		return alignPtrInfo;
	}
	using AlignPtrExpect = natl::Expect<AlignPtrInfo, AlignPtrError>;

	constexpr AlignPtrExpect alignPtr(
		const natl::Size alignment, const natl::Size size, 
		void* ptr, const natl::Size space) noexcept {
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

		alignPtrInfo.alignedPtr = reinterpret_cast<natl::Byte*>(ptr) + adjustment;
		alignPtrInfo.nextPtr = reinterpret_cast<natl::Byte*>(alignPtrInfo.alignedPtr) + size;
		alignPtrInfo.remainingSpace = space - (adjustment + size);

		return alignPtrInfo;
	}

	template<typename DataType>
	constexpr AlignPtrExpect alignPtrWithArray(const natl::Size number, void* ptr, const natl::Size space) noexcept {
		return alignPtr(alignof(DataType), sizeof(DataType) * number, ptr, space);
	}

	template<typename DataType>
	constexpr AlignPtrExpect alignPtrWithType(void* ptr, const natl::Size space) noexcept {
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
}