#pragma once 

//own
#include "peramaterPackOperations.h"
#include "container.h"
#include "typeTraits.h"
#include "functional.h"
#include "compare.h"
#include "hash.h"

//interface 
namespace natl {
	template<typename... DataType> class Tuple;
	
	template<>
	class Tuple<> {
	public:
		//special 
		constexpr Size hash() const noexcept { return 0; }

		//compare
		constexpr bool operator==(const Tuple<>&) const noexcept { return true; }
		constexpr bool operator!=(const Tuple<>&) const noexcept { return true; }
		constexpr bool operator<(const Tuple<>&) const noexcept { return true; }
		constexpr bool operator<=(const Tuple<>&) const noexcept { return true; }
		constexpr bool operator>(const Tuple<>&) const noexcept { return true; }
		constexpr bool operator>=(const Tuple<>&) const noexcept { return true; }
		constexpr StrongOrdering operator<=>(const Tuple<>&) const noexcept { return StrongOrdering::equal; }
	};

	template<typename FirstDataType, typename... RestDataTypes>
	class Tuple<FirstDataType, RestDataTypes...> {
	public:
		using first_value_type = FirstDataType;
		using rest_value_types = TypePack<RestDataTypes...>;
		using value_types = TypePack<FirstDataType, RestDataTypes...>;
		using rest_tuple_type = Tuple<RestDataTypes...>;

		constexpr static Size tupleSize = value_types::size;
	public:
		FirstDataType first;
		rest_tuple_type rest;
	public:
		//constructor 
		constexpr Tuple() noexcept = default;

		template<typename FirstU, typename... RestU>
			requires(std::is_constructible_v<FirstDataType, const FirstU&>&& std::is_constructible_v<rest_tuple_type, const RestU&...>)
		constexpr Tuple(const FirstU& first, const RestU&... rest) noexcept : first(first), rest(rest...) {}
		template<typename FirstU, typename... RestU>
			requires(std::is_constructible_v<FirstDataType, FirstU&&>&& std::is_constructible_v<rest_tuple_type, RestU&&...>)
		constexpr Tuple(FirstU&& first, RestU&&... rest) noexcept : first(natl::forward<FirstU>(first)), rest(natl::forward<RestU>(rest)...) {}

		template<typename FirstU, typename... RestU>
			requires(std::is_constructible_v<FirstDataType, const FirstU&>&& std::is_constructible_v<rest_tuple_type, const RestU&...>)
		constexpr Tuple(const Tuple<FirstU, RestU...>& other) noexcept : first(other.first), rest(other.rest) {}
		template<typename FirstU, typename... RestU>
			requires(std::is_constructible_v<FirstDataType, FirstU&&>&& std::is_constructible_v<rest_tuple_type, RestU&&...>)
		constexpr Tuple(Tuple<FirstU, RestU...>&& other) noexcept : first(natl::move(other.first)), rest(natl::move(other.rest)) {}

		//destructor 
		constexpr ~Tuple() noexcept = default;

		//util 
		constexpr Tuple& self() noexcept { return *this; }
		constexpr const Tuple& self() const noexcept { return *this; }

		//assignment 
		template<typename FirstU, typename... RestU>
			requires(std::is_assignable_v<FirstDataType, const FirstU&>&& std::is_assignable_v<rest_tuple_type, const RestU&...>)
		constexpr Tuple& operator=(const Tuple<FirstU, RestU...>& other) noexcept {
			first = other.first;
			rest = other.rest;
			return self();
		}
		template<typename FirstU, typename... RestU>
			requires(std::is_assignable_v<FirstDataType, FirstU&&>&& std::is_assignable_v<rest_tuple_type, RestU&&...>)
		constexpr Tuple& operator=(Tuple<FirstU, RestU...>&& other) noexcept {
			first = natl::move(other.first);
			rest = natl::move(other.rest);
			return self();
		}

	private:
		template<Size Index, class ReturnType>
		constexpr ReturnType& internalGet() noexcept {
			if constexpr (Index == 0) {
				return first;
			} else {
				if constexpr (sizeof...(RestDataTypes) > 0) {
					return rest.template internalGet<Index - 1, ReturnType>();
				}
			}
		}
		template<Size Index, class ReturnType>
		constexpr const ReturnType& internalGet() const noexcept {
			if constexpr (Index == 0) {
				return first;
			} else {
				if constexpr (sizeof...(RestDataTypes) > 0) {
					return rest.template internalGet<Index - 1, ReturnType>();
				}
			}
		}
	public:

		//element access 
		template<Size Index>
			requires(Index < value_types::size)
		constexpr TypePackNthElement<Index, value_types>& get() noexcept {
			return internalGet<Index, TypePackNthElement<Index, value_types>>();
		}

		template<Size Index>
			requires(Index < value_types::size)
		constexpr const TypePackNthElement<Index, value_types>& get() const noexcept {
			return internalGet<Index, TypePackNthElement<Index, value_types>>();
		}

		constexpr Size size() const noexcept { return tupleSize; }

		//modifier 
		constexpr void swap(const Tuple& other) noexcept {
			natl::swap(other, self());
		}

		//special 
	private:
		template<typename Tuple, Size... Indices>
		constexpr Size internalHash([[maybe_unused]] const Tuple& tuple, IndexSequence<Indices...>) const noexcept 
			requires(Hashable<FirstDataType> && (Hashable<RestDataTypes> && ...)) {
			Size result = 0;
			const Size hashValues[] = { hashValue(tuple.template get<Indices>())... };
			for (Size index = 0; index < size(); ++index) {
				result++;
				result ^= hashValues[index] + Size(0x9e3779b9) + (result << Size(6)) + (result >> Size(2));
			}
			return result;
		};
	public:

		constexpr Size hash() const noexcept 
			requires(Hashable<FirstDataType> && (Hashable<RestDataTypes> && ...)) {
			return internalHash(self(), MakeIndexSequence<tupleSize>());
		}

		template<typename... OtherDataTypes>
		friend class Tuple;

		//compare 
		template<typename... RhsDataTypes>
			requires(tupleSize == sizeof...(RhsDataTypes) && IsOneWayEqualTestable<FirstDataType, typename Tuple<RhsDataTypes...>::first_value_type>)
		constexpr bool operator==(const Tuple<RhsDataTypes...>& rhs) const noexcept {
			const Tuple& lhs = self();
			return lhs.first == rhs.first && lhs.rest == rhs.rest;
		}
		template<typename... RhsDataTypes>
			requires(tupleSize == sizeof...(RhsDataTypes) && IsOneWayNotEqualTestable<FirstDataType, typename Tuple<RhsDataTypes...>::first_value_type>)
		constexpr bool operator!=(const Tuple<RhsDataTypes...>& rhs) const noexcept {
			const Tuple& lhs = self();
			return lhs.first != rhs.first && lhs.rest != rhs.rest;
		}
		template<typename... RhsDataTypes>
			requires(tupleSize == sizeof...(RhsDataTypes) && IsOneWayLessThanTestable<FirstDataType, typename Tuple<RhsDataTypes...>::first_value_type>)
		constexpr bool operator<(const Tuple<RhsDataTypes...>&rhs) const noexcept {
			const Tuple& lhs = self();
			return lhs.first < rhs.first && lhs.rest < rhs.rest;
		}
		template<typename... RhsDataTypes>
			requires(tupleSize == sizeof...(RhsDataTypes) && IsOneWayLessThanEqualTestable<FirstDataType, typename Tuple<RhsDataTypes...>::first_value_type>)
		constexpr bool operator<=(const Tuple<RhsDataTypes...>& rhs) const noexcept {
			const Tuple& lhs = self();
			return lhs.first <= rhs.first && lhs.rest <= rhs.rest;
		}
		template<typename... RhsDataTypes>
			requires(tupleSize == sizeof...(RhsDataTypes) && IsOneWayGreaterThanTestable<FirstDataType, typename Tuple<RhsDataTypes...>::first_value_type>)
		constexpr bool operator>(const Tuple<RhsDataTypes...>& rhs) const noexcept {
			const Tuple& lhs = self();
			return lhs.first > rhs.first && lhs.rest > rhs.rest;
		}
		template<typename... RhsDataTypes>
			requires(tupleSize == sizeof...(RhsDataTypes) && IsOneWayGreaterThanEqualTestable<FirstDataType, typename Tuple<RhsDataTypes...>::first_value_type>)
		constexpr bool operator>=(const Tuple<RhsDataTypes...>& rhs) const noexcept {
			const Tuple& lhs = self();
			return lhs.first >= rhs.first && lhs.rest >= rhs.rest;
		}
	};

	template<typename... LhsDataTypes, typename... RhsDataTypes>
		requires(sizeof...(LhsDataTypes) == sizeof...(RhsDataTypes) && (IsOneWayThreeWayTestable<LhsDataTypes, RhsDataTypes> && ...))
	constexpr CommonComparisonCategory<CompareThreeWayResult<LhsDataTypes, RhsDataTypes>...>
		operator<=>(const Tuple<LhsDataTypes...>& lhs, const Tuple<RhsDataTypes...>& rhs) noexcept {
		if (auto result = CompareThreeWay{}(lhs.first, rhs.first); result != 0) {
			return result;
		} else {
			return lhs.rest <=> rhs.rest;
		}
	}

	template <typename... DataTypes> constexpr Tuple<Decay<DataTypes>...> makeTuple(DataTypes&&... args) {
		return Tuple<Decay<DataTypes>...>(natl::forward<DataTypes>(args)...);
	}

	template <typename... Args>
	constexpr Tuple<Args&...> tie(Args&... args) noexcept {
		return { args... };
	}

	template<typename... Types>
	constexpr Tuple<Types&&...> forwardAsTuple(Types&&... args) noexcept {
		return std::tuple<Types&&...>(natl::forward<Types>(args)...);
	}

	namespace impl {
		template<class TupleType> struct TupleSizeTypeImpl : IntegralConstant<Size, 0> {};
		template<class... Types> struct TupleSizeTypeImpl<Tuple<Types...>> : IntegralConstant<Size, sizeof...(Types)> {};

		template<Size Index, class TupleType> struct TupleElementTypeImpl;

		template<Size Index, class... DataTypes>
		struct TupleElementTypeImpl<Index, Tuple<DataTypes...>> {
			using type = TypePackNthElement<Index, typename Tuple<DataTypes...>::value_types>;
		};

	}

	template<class TupleType> using TupleSizeTypeImpl = impl::TupleSizeTypeImpl<Decay<TupleType>>;
	template<class TupleType> constexpr inline Size TupleSize = impl::TupleSizeTypeImpl<Decay<TupleType>>::value;
	template<Size Index, class TupleType> using TupleElementType = impl::TupleElementTypeImpl<Index, Decay<TupleType>>;
	template<Size Index, class TupleType> using TupleElement = impl::TupleElementTypeImpl<Index, Decay<TupleType>>::value;

	namespace impl {
		template<typename Functor, typename Tuple, Size... Indices>
		constexpr decltype(auto) callFunctionWithTupleImpl(Functor&& functor, Tuple&& tuple, IndexSequence<Indices...>) noexcept {
			return invokeFunction(natl::forward<Functor>(functor), tuple.template get<Indices>()...);
		}
	}

	template<typename Functor, typename Tuple> 
	constexpr decltype(auto) callFunctionWithTuple(Functor&& functor, Tuple&& tuple) noexcept {
		return impl::callFunctionWithTupleImpl(
			natl::forward<Functor>(functor), 
			natl::forward<Tuple>(tuple), 
			MakeIndexSequence<TupleSize<Tuple>>{}
		);
	}
}