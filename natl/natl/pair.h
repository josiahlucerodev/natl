#pragma once 

//own
#include "typeTraits.h"
#include "dataMovement.h"

//interface
namespace natl {
	template<class DataTypeFirst, class DataTypeSecond>
	struct Pair {
	public:
		//movement info 
		constexpr static bool triviallyRelocatable = IsTriviallyRelocatable<DataTypeFirst> && IsTriviallyRelocatable<DataTypeSecond>;
		constexpr static bool triviallyDefaultConstructible = IsTriviallyDefaultConstructible<DataTypeFirst> && IsTriviallyDefaultConstructible<DataTypeSecond>;
		constexpr static bool triviallyCompareable = IsTriviallyCompareable<DataTypeFirst> && IsTriviallyCompareable<DataTypeSecond>;
		constexpr static bool triviallyDestructible = IsTriviallyDestructible<DataTypeFirst> && IsTriviallyDestructible<DataTypeSecond>;
		constexpr static bool triviallyConstRefConstructedable = IsTriviallyConstRefConstructible<DataTypeFirst> && IsTriviallyConstRefConstructible<DataTypeSecond>;
		constexpr static bool triviallyMoveConstructedable = IsTriviallyMoveConstructible<DataTypeFirst> && IsTriviallyMoveConstructible<DataTypeSecond>;
	public:
		DataTypeFirst first;
		DataTypeFirst second;
	public:
		//constructor
		constexpr Pair() noexcept = default;
		constexpr Pair(const Pair& other) noexcept = default;
		constexpr Pair(Pair&& other) noexcept = default;
		constexpr Pair(const DataTypeFirst& first, const DataTypeSecond& second) noexcept : first(first), second(second) {}
		constexpr Pair(DataTypeFirst&& first, DataTypeSecond& second) noexcept : first(forward<DataTypeFirst>(first)), second(forward<DataTypeSecond>(second)) {}

		//destructor 
		constexpr ~Pair() noexcept = default;

		//util
		constexpr Pair& self() noexcept { return *this; }
		constexpr const Pair& self() const noexcept { return *this; }

		//assignment
		constexpr Pair& operator=(const Pair& other) noexcept {
			first = other.first;
			second = other.second;
			return self();
		}
		constexpr Pair& operator=(Pair&& other) noexcept {
			first = forward<DataTypeFirst>(other.first);
			second = forward<DataTypeSecond>(other.second);
			return self();
		}
		constexpr void swap(Pair& other) noexcept {
			natl::swap<DataTypeFirst>(first, other.first);
			natl::swap<DataTypeFirst>(second, other.second);
		}
	};

	//compare 
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator==(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return (lhs.first == rhs.first) && (lhs.second == rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator!=(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return (lhs.first != rhs.first) && (lhs.second != rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator<(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator<=(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator>(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return rhs < lhs;
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator>=(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return !(lhs < rhs);
	}
	template< class T1, class T2, class U1, class U2 >
	constexpr std::common_comparison_category_t<SynthesizeThreeWayResult<T1, U1>, SynthesizeThreeWayResult<T2, U2>> 
		operator<=>(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		if (auto result = SynthesizeThreeWay{}(lhs.first, rhs.first); result != 0) {
			return result;
		} else {
			return SynthesizeThreeWay{}(lhs.second, rhs.second);
		}
	}

	template<class DataTypeFirst, class DataTypeSecond>
	constexpr Pair<typename std::decay<DataTypeFirst>::type, typename std::decay<DataTypeSecond>::type> makePair(DataTypeFirst&& first, DataTypeSecond&& second) {
		return Pair<typename std::decay<DataTypeFirst>::type, typename std::decay<DataTypeSecond>::type>(first, second);
	}

	template<class DataTypeFirst, class DataTypeSecond>
	struct CompressedPair {
	public:
		//movement info 
		constexpr static bool triviallyRelocatable = IsTriviallyRelocatable<DataTypeFirst> && IsTriviallyRelocatable<DataTypeSecond>;
		constexpr static bool triviallyDefaultConstructible = IsTriviallyDefaultConstructible<DataTypeFirst> && IsTriviallyDefaultConstructible<DataTypeSecond>;
		constexpr static bool triviallyCompareable = IsTriviallyCompareable<DataTypeFirst> && IsTriviallyCompareable<DataTypeSecond>;
		constexpr static bool triviallyDestructible = IsTriviallyDestructible<DataTypeFirst> && IsTriviallyDestructible<DataTypeSecond>;
		constexpr static bool triviallyConstRefConstructedable = IsTriviallyConstRefConstructible<DataTypeFirst> && IsTriviallyConstRefConstructible<DataTypeSecond>;
		constexpr static bool triviallyMoveConstructedable = IsTriviallyMoveConstructible<DataTypeFirst> && IsTriviallyMoveConstructible<DataTypeSecond>;
	public:
		[[no_unique_address]] DataTypeFirst first;
		[[no_unique_address]] DataTypeFirst second;
	public:
		//constructor
		constexpr CompressedPair() noexcept = default;
		constexpr CompressedPair(const CompressedPair& other) noexcept = default;
		constexpr CompressedPair(CompressedPair&& other) noexcept = default;
		constexpr CompressedPair(const DataTypeFirst& first, const DataTypeSecond& second) noexcept : first(first), second(second) {}
		constexpr CompressedPair(DataTypeFirst&& first, DataTypeSecond& second) noexcept : first(forward<DataTypeFirst>(first)), second(forward<DataTypeSecond>(second)) {}

		//destructor 
		constexpr ~CompressedPair() noexcept = default;

		//util
		constexpr CompressedPair& self() noexcept { return *this; }
		constexpr const CompressedPair& self() const noexcept { return *this; }

		//assignment
		constexpr CompressedPair& operator=(const CompressedPair& other) noexcept {
			first = other.first;
			second = other.second;
			return self();
		}
		constexpr CompressedPair& operator=(CompressedPair&& other) noexcept {
			first = forward<DataTypeFirst>(other.first);
			second = forward<DataTypeSecond>(other.second);
			return self();
		}
		constexpr void swap(CompressedPair& other) noexcept {
			natl::swap<DataTypeFirst>(first, other.first);
			natl::swap<DataTypeFirst>(second, other.second);
		}
	};

	//compare 
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator==(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return (lhs.first == rhs.first) && (lhs.second == rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator!=(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return (lhs.first != rhs.first) && (lhs.second != rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator<(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator<=(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator>(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return rhs < lhs;
	}
	template<class T1, class T2, class U1, class U2>
	constexpr bool operator>=(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return !(lhs < rhs);
	}
	template< class T1, class T2, class U1, class U2 >
	constexpr std::common_comparison_category_t<SynthesizeThreeWayResult<T1, U1>, SynthesizeThreeWayResult<T2, U2>>
		operator<=>(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		if (auto result = SynthesizeThreeWay{}(lhs.first, rhs.first); result != 0) {
			return result;
		}
		else {
			return SynthesizeThreeWay{}(lhs.second, rhs.second);
		}
	}

	template<class DataTypeFirst, class DataTypeSecond>
	constexpr CompressedPair<typename std::decay<DataTypeFirst>::type, typename std::decay<DataTypeSecond>::type> makePair(DataTypeFirst&& first, DataTypeSecond&& second) {
		return CompressedPair<typename std::decay<DataTypeFirst>::type, typename std::decay<DataTypeSecond>::type>(first, second);
	}

}