#pragma once 

//own
#include "../util/typeTraits.h"
#include "../util/dataMovement.h"
#include "../util/compare.h"

//interface
namespace natl {
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct Pair {
	public:
		using first_type = DataTypeFirst;
		using second_type = DataTypeSecond;
	public:
		DataTypeFirst first;
		DataTypeFirst second;
	public:
		//constructor
		constexpr Pair() noexcept = default;
		constexpr Pair(const Pair& other) noexcept = default;
		constexpr Pair(Pair&& other) noexcept = default;
		constexpr Pair(const DataTypeFirst& firstIn, const DataTypeSecond& secondIn) noexcept : first(firstIn), second(secondIn) {}
		constexpr Pair(DataTypeFirst&& firstIn, DataTypeSecond& secondIn) noexcept : first(natl::forward<DataTypeFirst>(firstIn)), second(natl::forward<DataTypeSecond>(secondIn)) {}

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
	constexpr Bool operator==(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return (lhs.first == rhs.first) && (lhs.second == rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator!=(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return (lhs.first != rhs.first) && (lhs.second != rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator<(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator<=(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator>(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return rhs < lhs;
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator>=(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		return !(lhs < rhs);
	}
	template< class T1, class T2, class U1, class U2 >
	constexpr CommonComparisonCategory<CompareThreeWayResultType<T1, U1>, CompareThreeWayResultType<T2, U2>>
		operator<=>(const Pair<T1, T2>& lhs, const Pair<U1, U2>& rhs) noexcept {
		if (auto result = CompareThreeWay{}(lhs.first, rhs.first); result != 0) {
			return result;
		} else {
			return CompareThreeWay{}(lhs.second, rhs.second);
		}
	}

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyCompareableV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyCompareable<DataTypeFirst>&& IsTriviallyCompareable<DataTypeSecond>> {};

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyRelocatableV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyRelocatable<DataTypeFirst>&& IsTriviallyRelocatable<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyConstructibleV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyConstructible<DataTypeFirst>&& IsTriviallyConstructible<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyDestructibleV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyDestructible<DataTypeFirst>&& IsTriviallyDestructible<DataTypeSecond>> {};

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyConstRefConstructibleV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyConstRefConstructible<DataTypeFirst>&& IsTriviallyConstRefConstructible<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyMoveConstructibleV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyMoveConstructible<DataTypeFirst>&& IsTriviallyMoveConstructible<DataTypeSecond>> {};

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyConstRefAssignableV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyConstRefAssignable<DataTypeFirst>&& IsTriviallyConstRefAssignable<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyMoveAssignableV<Pair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyMoveAssignable<DataTypeFirst>&& IsTriviallyMoveAssignable<DataTypeSecond>> {};

	template<class DataTypeFirst, class DataTypeSecond>
	constexpr Pair<typename Decay<DataTypeFirst>::type, typename Decay<DataTypeSecond>::type> makePair(DataTypeFirst&& first, DataTypeSecond&& second) {
		return Pair<typename Decay<DataTypeFirst>::type, typename Decay<DataTypeSecond>::type>(first, second);
	}

	template<class DataTypeFirst, class DataTypeSecond>
	struct CompressedPair {
	public:
		using first_type = DataTypeFirst;
		using second_type = DataTypeSecond;
	public:
		[[no_unique_address]] DataTypeFirst first;
		[[no_unique_address]] DataTypeFirst second;
	public:
		//constructor
		constexpr CompressedPair() noexcept = default;
		constexpr CompressedPair(const CompressedPair& other) noexcept = default;
		constexpr CompressedPair(CompressedPair&& other) noexcept = default;
		constexpr CompressedPair(const DataTypeFirst& firstIn, const DataTypeSecond& secondIn) noexcept : first(firstIn), second(secondIn) {}
		constexpr CompressedPair(DataTypeFirst&& firstIn, DataTypeSecond&& secondIn) noexcept : first(natl::forward<DataTypeFirst>(firstIn)), second(natl::forward<DataTypeSecond>(secondIn)) {}

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
	constexpr Bool operator==(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return (lhs.first == rhs.first) && (lhs.second == rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator!=(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return (lhs.first != rhs.first) && (lhs.second != rhs.second);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator<(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator<=(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return !(rhs < lhs);
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator>(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return rhs < lhs;
	}
	template<class T1, class T2, class U1, class U2>
	constexpr Bool operator>=(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		return !(lhs < rhs);
	}
	template< class T1, class T2, class U1, class U2 >
	constexpr CommonComparisonCategory<CompareThreeWayResult<T1, U1>, CompareThreeWayResult<T2, U2>>
		operator<=>(const CompressedPair<T1, T2>& lhs, const CompressedPair<U1, U2>& rhs) noexcept {
		if (auto result = CompareThreeWay{}(lhs.first, rhs.first); result != 0) {
			return result;
		} else {
			return CompareThreeWay{}(lhs.second, rhs.second);
		}
	}

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyCompareableV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyCompareable<DataTypeFirst>&& IsTriviallyCompareable<DataTypeSecond>> {};

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyRelocatableV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyRelocatable<DataTypeFirst>&& IsTriviallyRelocatable<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyConstructibleV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyConstructible<DataTypeFirst>&& IsTriviallyConstructible<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyDestructibleV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyDestructible<DataTypeFirst>&& IsTriviallyDestructible<DataTypeSecond>> {};

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyConstRefConstructibleV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyConstRefConstructible<DataTypeFirst>&& IsTriviallyConstRefConstructible<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyMoveConstructibleV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyMoveConstructible<DataTypeFirst>&& IsTriviallyMoveConstructible<DataTypeSecond>> {};

	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyConstRefAssignableV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyConstRefAssignable<DataTypeFirst>&& IsTriviallyConstRefAssignable<DataTypeSecond>> {};
	template<typename DataTypeFirst, typename DataTypeSecond>
	struct IsTriviallyMoveAssignableV<CompressedPair<DataTypeFirst, DataTypeSecond>>
		: BoolConstant<IsTriviallyMoveAssignable<DataTypeFirst>&& IsTriviallyMoveAssignable<DataTypeSecond>> {};

	template<class DataTypeFirst, class DataTypeSecond>
	constexpr CompressedPair<typename Decay<DataTypeFirst>::type, typename Decay<DataTypeSecond>::type> makePair(DataTypeFirst&& first, DataTypeSecond&& second) {
		return CompressedPair<typename Decay<DataTypeFirst>::type, typename Decay<DataTypeSecond>::type>(first, second);
	}

}