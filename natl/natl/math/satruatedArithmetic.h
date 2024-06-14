#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/typeTraits.h"
#include "../util/limits.h"

//intrinsics
#ifdef NATL_COMPILER_MSVC
#include <immintrin.h>
#include <intrin.h>
#endif //NATL_COMPILER_MSVC

//interface 
namespace natl::math {
	//add sat
	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr Bool isAddOverflowMax(const IntegerType lhs, const IntegerType rhs) noexcept {
		return rhs > IntegerType(0) && lhs > Limits<IntegerType>::max() - rhs;
	}
	template<typename SignedIntegerType>
		requires(IsBuiltInSignedIntegerC<SignedIntegerType>)
	NATL_FORCE_INLINE constexpr Bool isAddOverflowMin(const SignedIntegerType lhs, const SignedIntegerType rhs) noexcept {
		return rhs < 0 && lhs < Limits<SignedIntegerType>::min() - rhs;
	}
	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr Bool isAddOverflow(const IntegerType lhs, const IntegerType rhs) noexcept {
		if constexpr (IsBuiltInSignedIntegerC<IntegerType>) {
			return isAddOverflowMax<IntegerType>() && isAddOverflowMin<IntegerType>();
		} else {
			return isAddOverflowMax<IntegerType>();
		}
	}

	namespace impl {
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const i8 lhs, const i8 rhs, i8& out) noexcept {
			if (isAddOverflowMax<i8>(lhs, rhs) || isAddOverflowMin<i8>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const i16 lhs, const i16 rhs, i16& out) noexcept {
			if (isAddOverflowMax<i16>(lhs, rhs) || isAddOverflowMin<i16>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const i32 lhs, const i32 rhs, i32& out) noexcept {
			if (isAddOverflowMax<i32>(lhs, rhs) || isAddOverflowMin<i32>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const i64 lhs, const i64 rhs, i64& out) noexcept {
			if (isAddOverflowMax<i64>(lhs, rhs) || isAddOverflowMin<i64>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
			if (isAddOverflowMax<ui8>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
			if (isAddOverflowMax<ui16>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
			if (isAddOverflowMax<ui32>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool addOverflowImplS(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
			if (isAddOverflowMax<ui64>(lhs, rhs)) {
				return true;
			} else {
				out = lhs + rhs;
				return false;
			}
		}

#ifdef NATL_COMPILER_MSVC
		NATL_FORCE_INLINE Bool addOverflowImplD(const i8 lhs, const i8 rhs, i8& out) noexcept {
			return _add_overflow_i8(0, lhs, rhs, &out);
		}
		NATL_FORCE_INLINE Bool addOverflowImplD(const i16 lhs, const i16 rhs, i16& out) noexcept {
			return _add_overflow_i16(0, lhs, rhs, &out);
		}
		NATL_FORCE_INLINE Bool addOverflowImplD(const i32 lhs, const i32 rhs, i32& out) noexcept {
			return _add_overflow_i32(0, lhs, rhs, &out);
		}
		NATL_FORCE_INLINE Bool addOverflowImplD(const i64 lhs, const i64 rhs, i64& out) noexcept {
			return _add_overflow_i64(0, lhs, rhs, &out);
		}
		NATL_FORCE_INLINE Bool addOverflowImplD(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
			return addOverflowImplS(lhs, rhs, out);
		}
		NATL_FORCE_INLINE Bool addOverflowImplD(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
			return addOverflowImplS(lhs, rhs, out);
		}
		NATL_FORCE_INLINE Bool addOverflowImplD(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
			return addOverflowImplS(lhs, rhs, out);
		}
		NATL_FORCE_INLINE Bool addOverflowImplD(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
			return addOverflowImplS(lhs, rhs, out);
		}
#endif // NATL_COMPILER_MSVC
	}

	NATL_FORCE_INLINE constexpr Bool addOverflow(const i8 lhs, const i8 rhs, i8& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool addOverflow(const i16 lhs, const i16 rhs, i16& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool addOverflow(const i32 lhs, const i32 rhs, i32& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool addOverflow(const i64 lhs, const i64 rhs, i64& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool addOverflow(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool addOverflow(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool addOverflow(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool addOverflow(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::addOverflowImplS(lhs, rhs, out);
		} else {
			return impl::addOverflowImplD(lhs, rhs, out);
		}
	}


	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr IntegerType addsat(const IntegerType lhs, const IntegerType rhs) noexcept {
		IntegerType addResult;
		if (!addOverflow(lhs, rhs, addResult)) {
			return addResult;
		}
		if constexpr (IsBuiltInUnsignedIntegerC<IntegerType>) {
			return Limits<IntegerType>::max();
		} else if (lhs < 0) {
			return Limits<IntegerType>::min();
		} else {
			return Limits<IntegerType>::max();
		}
	}

	//sub sat
	template<typename SignedIntegerType>
		requires(IsBuiltInSignedIntegerC<SignedIntegerType>)
	NATL_FORCE_INLINE constexpr Bool isSubOverflowMax(const SignedIntegerType lhs, const SignedIntegerType rhs) noexcept {
		return rhs < 0 && lhs > Limits<SignedIntegerType>::max() + rhs;
	}
	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr Bool isSubOverflowMin(const IntegerType lhs, const IntegerType rhs) noexcept {
		return rhs > 0 && lhs < Limits<IntegerType>::min() + rhs;
	}
	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr Bool isSubOverflow(const IntegerType lhs, const IntegerType rhs) noexcept {
		if constexpr (IsBuiltInSignedIntegerC<IntegerType>) {
			return isSubOverflowMax<IntegerType>() || isSubOverflowMin<IntegerType>();
		} else {
			return isSubOverflowMin<IntegerType>();
		}
	}

	namespace impl {
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const i8 lhs, const i8 rhs, i8& out) noexcept {
			if (isSubOverflowMax<i8>(lhs, rhs) || isSubOverflowMin<i8>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const i16 lhs, const i16 rhs, i16& out) noexcept {
			if (isSubOverflowMax<i16>(lhs, rhs) || isSubOverflowMin<i16>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const i32 lhs, const i32 rhs, i32& out) noexcept {
			if (isSubOverflowMax<i32>(lhs, rhs) || isSubOverflowMin<i32>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const i64 lhs, const i64 rhs, i64& out) noexcept {
			if (isSubOverflowMax<i64>(lhs, rhs) || isSubOverflowMin<i64>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
			if (isSubOverflowMin<ui8>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
			if (isSubOverflowMin<ui16>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
			if (isSubOverflowMin<ui32>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool subOverflowImplS(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
			if (isSubOverflowMin<ui64>(lhs, rhs)) {
				return true;
			} else {
				out = lhs - rhs;
				return false;
			}
		}

#ifdef NATL_COMPILER_MSVC
		NATL_FORCE_INLINE Bool subOverflowImplD(const i8 lhs, const i8 rhs, i8& out) noexcept {
			return _sub_overflow_i8(0, lhs, rhs, &out);
		}
		NATL_FORCE_INLINE Bool subOverflowImplD(const i16 lhs, const i16 rhs, i16& out) noexcept {
			return _sub_overflow_i16(0, lhs, rhs, &out);
		}
		NATL_FORCE_INLINE Bool subOverflowImplD(const i32 lhs, const i32 rhs, i32& out) noexcept {
			return _sub_overflow_i32(0, lhs, rhs, &out);
		}
		NATL_FORCE_INLINE Bool subOverflowImplD(const i64 lhs, const i64 rhs, i64& out) noexcept {
			return _sub_overflow_i64(0, lhs, rhs, &out);
		}

		NATL_FORCE_INLINE Bool subOverflowImplD(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
			return subOverflowImplS(lhs, rhs, out);
		}
		NATL_FORCE_INLINE Bool subOverflowImplD(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
			return subOverflowImplS(lhs, rhs, out);
		}
		NATL_FORCE_INLINE Bool subOverflowImplD(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
			return subOverflowImplS(lhs, rhs, out);
		}
		NATL_FORCE_INLINE Bool subOverflowImplD(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
			return subOverflowImplS(lhs, rhs, out);
		}
#endif // NATL_COMPILER_MSVC
	}

	NATL_FORCE_INLINE constexpr Bool subOverflow(const i8 lhs, const i8 rhs, i8& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool subOverflow(const i16 lhs, const i16 rhs, i16& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool subOverflow(const i32 lhs, const i32 rhs, i32& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool subOverflow(const i64 lhs, const i64 rhs, i64& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool subOverflow(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool subOverflow(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool subOverflow(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool subOverflow(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::subOverflowImplS(lhs, rhs, out);
		} else {
			return impl::subOverflowImplD(lhs, rhs, out);
		}
	}

	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr IntegerType subsat(const IntegerType lhs, const IntegerType rhs) noexcept {
		IntegerType subResult;
		if (!subOverflow(lhs, rhs, subResult)) {
			return subResult;
		}
		if constexpr (IsBuiltInUnsignedIntegerC<IntegerType>) {
			return Limits<IntegerType>::min();
		} else if (lhs < 0) {
			return Limits<IntegerType>::min();
		} else {
			return Limits<IntegerType>::max();
		}
	}

	//mul sat
	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr Bool isMulOverflowMax(const IntegerType lhs, const IntegerType rhs) noexcept {
		return lhs > 0 && rhs > Limits<IntegerType>::max() / lhs;
	}
	template<typename SignedIntegerType>
		requires(IsBuiltInSignedIntegerC<SignedIntegerType>)
	NATL_FORCE_INLINE constexpr Bool isMulOverflowMin(const SignedIntegerType lhs, const SignedIntegerType rhs) noexcept {
		return lhs < 0 && rhs > Limits<SignedIntegerType>::min() / lhs;
	}
	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr Bool isMulOverflow(const IntegerType lhs, const IntegerType rhs) noexcept {
		if constexpr (IsBuiltInSignedIntegerC<IntegerType>) {
			return isMulOverflowMax<IntegerType>() && isMulOverflowMin<IntegerType>();
		} else {
			return isMulOverflowMax<IntegerType>();
		}
	}

	namespace impl {
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const i8 lhs, const i8 rhs, i8& out) noexcept {
			if (isMulOverflowMax<i8>(lhs, rhs) || isMulOverflowMin<i8>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const i16 lhs, const i16 rhs, i16& out) noexcept {
			if (isMulOverflowMax<i16>(lhs, rhs) || isMulOverflowMin<i16>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const i32 lhs, const i32 rhs, i32& out) noexcept {
			if (isMulOverflowMax<i32>(lhs, rhs) || isMulOverflowMin<i32>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const i64 lhs, const i64 rhs, i64& out) noexcept {
			if (isMulOverflowMax<i64>(lhs, rhs) || isMulOverflowMin<i64>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
			if (isMulOverflowMax<ui8>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
			if (isMulOverflowMax<ui16>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
			if (isMulOverflowMax<ui32>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}
		NATL_FORCE_INLINE constexpr Bool mulOverflowImplS(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
			if (isMulOverflowMax<ui64>(lhs, rhs)) {
				return true;
			} else {
				out = lhs * rhs;
				return false;
			}
		}

#ifdef NATL_COMPILER_MSVC
		NATL_FORCE_INLINE Bool mulOverflowImplD(const i8 lhs, const i8 rhs, i8& out) noexcept {
			signed short result;
			const Bool overflowResult = _mul_full_overflow_i8(lhs, rhs, &result);
			out = static_cast<i8>(result);
			return overflowResult;
		}
		NATL_FORCE_INLINE Bool mulOverflowImplD(const i16 lhs, const i16 rhs, i16& out) noexcept {
			i16 unused;
			return _mul_full_overflow_i16(lhs, rhs, &out, &unused);
		}
		NATL_FORCE_INLINE Bool mulOverflowImplD(const i32 lhs, const i32 rhs, i32& out) noexcept {
			i32 unused;
			return _mul_full_overflow_i32(lhs, rhs, &out, &unused);
		}
		NATL_FORCE_INLINE Bool mulOverflowImplD(const i64 lhs, const i64 rhs, i64& out) noexcept {
			i64 unused;
			return _mul_full_overflow_i64(lhs, rhs, &out, &unused);
		}
		NATL_FORCE_INLINE Bool mulOverflowImplD(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
			unsigned short result;
			const Bool overflowResult = _mul_full_overflow_u8(lhs, rhs, &result);
			out = static_cast<ui8>(result);
			return overflowResult;
		}
		NATL_FORCE_INLINE Bool mulOverflowImplD(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
			ui16 unused;
			return _mul_full_overflow_u16(lhs, rhs, &out, &unused);
		}
		NATL_FORCE_INLINE Bool mulOverflowImplD(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
			ui32 unused;
			return _mul_full_overflow_u32(lhs, rhs, &out, &unused);
		}
		NATL_FORCE_INLINE Bool mulOverflowImplD(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
			ui64 unused;
			return _mul_full_overflow_u64(lhs, rhs, &out, &unused);
		}
#endif // NATL_COMPILER_MSVC
	}

	NATL_FORCE_INLINE constexpr Bool mulOverflow(const i8 lhs, const i8 rhs, i8& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool mulOverflow(const i16 lhs, const i16 rhs, i16& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool mulOverflow(const i32 lhs, const i32 rhs, i32& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool mulOverflow(const i64 lhs, const i64 rhs, i64& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool mulOverflow(const ui8 lhs, const ui8 rhs, ui8& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool mulOverflow(const ui16 lhs, const ui16 rhs, ui16& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool mulOverflow(const ui32 lhs, const ui32 rhs, ui32& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}
	NATL_FORCE_INLINE constexpr Bool mulOverflow(const ui64 lhs, const ui64 rhs, ui64& out) noexcept {
		if (isConstantEvaluated()) {
			return impl::mulOverflowImplS(lhs, rhs, out);
		} else {
			return impl::mulOverflowImplD(lhs, rhs, out);
		}
	}

	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr IntegerType mulsat(const IntegerType lhs, const IntegerType rhs) noexcept {
		IntegerType mulResult;
		if (!mulOverflow(lhs, rhs, mulResult)) {
			return mulResult;
		}
		if constexpr (IsBuiltInUnsignedIntegerC<IntegerType>) {
			return Limits<IntegerType>::max();
		} else if (lhs < 0 != rhs < 0) {
			return Limits<IntegerType>::min();
		} else {
			return Limits<IntegerType>::max();
		}
	}

	//div sat 
	template<typename IntegerType>
		requires(IsBuiltInIntegerC<IntegerType>)
	NATL_FORCE_INLINE constexpr IntegerType divsat(const IntegerType lhs, const IntegerType rhs) noexcept {
		if constexpr (IsBuiltInSignedIntegerC<IntegerType>) {
			if (lhs == Limits<IntegerType>::min() && rhs == IntegerType(-1)) {
				return Limits<IntegerType>::max();
			}
		}
		return lhs / rhs;
	}
}