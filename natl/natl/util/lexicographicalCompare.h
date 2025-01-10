#pragma once 

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "iterators.h"
#include "algorithm.h"
#include "compare.h"

//interface 
namespace natl {
	template<typename LhsIter, typename RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& IsConvertibleC<typename IteratorTraits<LhsIter>::value_type, typename IteratorTraits<RhsIter>::value_type>)
	constexpr Bool lexicographicalCompareEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		if (lhsSize != rhsSize) { return false; }
		for (Size i = 0; i < lhsSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter != static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return true;
	}

	template<typename LhsIter, typename RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& IsConvertibleC<typename IteratorTraits<LhsIter>::value_type, typename IteratorTraits<RhsIter>::value_type>)
	constexpr Bool lexicographicalCompareNotEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		return !lexicographicalCompareEqual<LhsIter, RhsIter>(lhsIter, lhsSize, rhsIter, rhsSize);
	}

	template<typename LhsIter, typename RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& IsConvertibleC<typename IteratorTraits<LhsIter>::value_type, typename IteratorTraits<RhsIter>::value_type>)
	constexpr Bool lexicographicalCompareLessThan(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter < static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter > static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize < rhsSize;
	}

	template<typename LhsIter, typename RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& IsConvertibleC<typename IteratorTraits<LhsIter>::value_type, typename IteratorTraits<RhsIter>::value_type>)
	constexpr Bool lexicographicalCompareLessThanEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter < static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter > static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize <= rhsSize;
	}

	template<typename LhsIter, typename RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& IsConvertibleC<typename IteratorTraits<LhsIter>::value_type, typename IteratorTraits<RhsIter>::value_type>)
	constexpr Bool lexicographicalCompareGreaterThan(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter > static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter < static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize > rhsSize;
	}

	template<typename LhsIter, typename RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& IsConvertibleC<typename IteratorTraits<LhsIter>::value_type, typename IteratorTraits<RhsIter>::value_type>)
	constexpr Bool lexicographicalCompareGreaterThanEqual(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			if (*lhsIter > static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return true; }
			if (*lhsIter < static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter)) { return false; }
		}
		return lhsSize >= rhsSize;
	}

	template<typename LhsIter, typename RhsIter>
		requires(IsIterPtr<LhsIter>&& IsIterPtr<RhsIter>&& IsConvertibleC<typename IteratorTraits<LhsIter>::value_type, typename IteratorTraits<RhsIter>::value_type>)
	constexpr StrongOrdering lexicographicalCompareSpaceship(LhsIter lhsIter, const Size lhsSize, RhsIter rhsIter, const Size rhsSize) noexcept {
		const Size commonSize = min<Size>(lhsSize, rhsSize);
		for (Size i = 0; i < commonSize; ++lhsIter, ++rhsIter) {
			StrongOrdering cmpResult = (*lhsIter <=> static_cast<typename IteratorTraits<LhsIter>::value_type>(*rhsIter));
			if (cmpResult != 0) { return cmpResult; }
		}
		return lhsSize <=> rhsSize;
	}
}