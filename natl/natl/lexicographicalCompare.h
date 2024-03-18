#pragma once 

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "iterators.h"
#include "algorithm.h"
#include "compare.h"

//interface 
namespace natl {
	template<class LhsIter, class RhsIter>
		requires(IsIterPtr<LhsIter> && IsIterPtr<RhsIter> && std::is_convertible_v<typename IterPtrTraits<LhsIter>::value_type, typename IterPtrTraits<RhsIter>::value_type>)
	constexpr bool lexicographicalCompareEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		if (lhsSize != rhsSize) { return false; }
		for (Size i = 0; i < lhsSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter != static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return true;
	}

	template<class LhsIter, class RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& std::is_convertible_v<typename IterPtrTraits<LhsIter>::value_type, typename IterPtrTraits<RhsIter>::value_type>)
	constexpr bool lexicographicalCompareNotEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		return !lexicographicalCompareEqual<LhsIter, RhsIter>(lhsIter, lhsSize, rhsIter, rhsSize);
	}

	template<class LhsIter, class RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& std::is_convertible_v<typename IterPtrTraits<LhsIter>::value_type, typename IterPtrTraits<RhsIter>::value_type>)
	constexpr bool lexicographicalCompareLessThan(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter < static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter > static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize < rhsSize;
	}

	template<class LhsIter, class RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& std::is_convertible_v<typename IterPtrTraits<LhsIter>::value_type, typename IterPtrTraits<RhsIter>::value_type>)
	constexpr bool lexicographicalCompareLessThanEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter < static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter > static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize <= rhsSize;
	}

	template<class LhsIter, class RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& std::is_convertible_v<typename IterPtrTraits<LhsIter>::value_type, typename IterPtrTraits<RhsIter>::value_type>)
	constexpr bool lexicographicalCompareGreaterThan(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter > static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter < static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize > rhsSize;
	}

	template<class LhsIter, class RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& std::is_convertible_v<typename IterPtrTraits<LhsIter>::value_type, typename IterPtrTraits<RhsIter>::value_type>)
	constexpr bool lexicographicalCompareGreaterThanEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter > static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter < static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize >= rhsSize;
	}

	template<class LhsIter, class RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& std::is_convertible_v<typename IterPtrTraits<LhsIter>::value_type, typename IterPtrTraits<RhsIter>::value_type>)
	constexpr StrongOrdering lexicographicalCompareSpaceship(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			StrongOrdering cmpResult = (*lhsIter <=> static_cast<typename IterPtrTraits<LhsIter>::value_type>(*rhsIter));
			if (cmpResult != 0) { return cmpResult; }
		}
		return lhsSize <=> rhsSize;
	}
}