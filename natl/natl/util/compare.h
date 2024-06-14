#pragma once 

//std
#include <compare>

//own
#include "typeTraits.h"

//interface 
namespace natl {
	using PartialOrdering = std::partial_ordering;
	using WeakOrdering = std::weak_ordering;
	using StrongOrdering = std::strong_ordering;

    namespace impl {
        template<unsigned long long>
        struct CommonCompareCatBaseType { using type = void; };
        template<> struct CommonCompareCatBaseType<0> { using type = StrongOrdering; };
        template<> struct CommonCompareCatBaseType<2> { using type = PartialOrdering; };
        template<> struct CommonCompareCatBaseType<4> { using type = WeakOrdering; };
        template<> struct CommonCompareCatBaseType<6> { using type = PartialOrdering; };
    } 

    template<class... DataTypes>
    struct CommonComparisonCategoryType : impl::CommonCompareCatBaseType<
        (0u | ... |
            (   
                IsSame<DataTypes, StrongOrdering> ? 0u :
                IsSame<DataTypes, WeakOrdering> ? 4u :
                IsSame<DataTypes, PartialOrdering> ? 2u : 1u
            )
        )
    > {};

    template<class... DataTypes>
    using CommonComparisonCategory = CommonComparisonCategoryType<DataTypes...>::type;

    struct CompareThreeWay {
        template <typename LhsDataType, typename RhsDataType>
        constexpr auto operator()(const LhsDataType& lhs, const RhsDataType& rhs) const
            requires(requires {
                { lhs < rhs } -> std::convertible_to<Bool>;
                { rhs < lhs } -> std::convertible_to<Bool>; }) {
            if constexpr (std::three_way_comparable_with<LhsDataType, RhsDataType>) {
                return lhs <=> rhs;
            } else {
                if (lhs < rhs) {
                    return WeakOrdering::less;
                } else if (rhs < lhs) {
                    return WeakOrdering::greater;
                } else {
                    return WeakOrdering::equivalent;
                }
            }
        }
    };

    template<class LhsDataType, class RhsDataType = LhsDataType>
    using CompareThreeWayResult = decltype(declval<const RemoveReference<LhsDataType>&>() <=> declval<const RemoveReference<RhsDataType>&>());

    template<class LhsDataType, class RhsDataType = LhsDataType>
    struct CompareThreeWayResultType {};
    template<class LhsDataType, class RhsDataType> 
        requires(requires { typename CompareThreeWayResult<LhsDataType, RhsDataType>; })
    struct CompareThreeWayResultType<LhsDataType, RhsDataType> {
        using type = CompareThreeWayResult<LhsDataType, RhsDataType>;
    };

    template<typename LhsDataType, typename RhsDataType = LhsDataType>
    concept IsOneWayThreeWayTestable = requires(const LhsDataType& lhs, const RhsDataType& rhs) {
        { lhs <=> rhs };
    };
    template<typename LhsDataType, typename RhsDataType = LhsDataType>
    concept IsThreeWayTestable = requires(const LhsDataType& lhs, const RhsDataType& rhs) {
        { lhs <=> rhs };
        { rhs <=> lhs };
    };
}