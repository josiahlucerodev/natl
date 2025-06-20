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

    template<typename... DataTypes>
    struct CommonComparisonCategoryType : impl::CommonCompareCatBaseType<
        (0u | ... |
            (   
                IsSame<DataTypes, StrongOrdering> ? 0u :
                IsSame<DataTypes, WeakOrdering> ? 4u :
                IsSame<DataTypes, PartialOrdering> ? 2u : 1u
            )
        )
    > {};

    template<typename... DataTypes>
    using CommonComparisonCategory = CommonComparisonCategoryType<DataTypes...>::type;

    template <typename Type>
    concept ThreeWayComparable = requires(const RemoveReference<Type>& lhs, const RemoveReference<Type>& rhs) {
            { lhs <=> rhs };
    };

    template <typename LhsType, typename RhsType>
    concept ThreeWayComparableWith = ThreeWayComparable<LhsType> && ThreeWayComparable<RhsType> && 
        requires(const RemoveReference<LhsType>& lhs, const RemoveReference<RhsType>& rhs) {
            { lhs <=> rhs };
    };

    struct CompareThreeWay {
        template <typename LhsDataType, typename RhsDataType>
        constexpr auto operator()(const LhsDataType& lhs, const RhsDataType& rhs) const
            requires(requires {
                { lhs < rhs } -> ConvertibleTo<Bool>;
                { rhs < lhs } -> ConvertibleTo<Bool>; }) {
            if constexpr (ThreeWayComparableWith<LhsDataType, RhsDataType>) {
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

    template<typename LhsDataType, typename RhsDataType = LhsDataType>
    using CompareThreeWayResult = decltype(declval<const RemoveReference<LhsDataType>&>() <=> declval<const RemoveReference<RhsDataType>&>());

    template<typename LhsDataType, typename RhsDataType = LhsDataType>
    struct CompareThreeWayResultType {};
    template<typename LhsDataType, typename RhsDataType>
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
    };

    template<typename DataType>
    struct CompareEqual {
    public:
        constexpr static Bool compare(const DataType& lhs, const DataType& rhs) noexcept {
            return lhs == rhs;
        }
        constexpr Bool operator()(const DataType& lhs, const DataType& rhs) const noexcept {
            return lhs == rhs;
        }
    };
    template<typename Type>
    struct CompareNotEqual {
    public:
        constexpr static Bool compare(const Type& lhs, const Type& rhs) noexcept {
            return lhs != rhs;
        }
        constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept {
            return lhs != rhs;
        }
    };

    template<typename Type>
    struct CompareLess {
    public:
        constexpr static Bool compare(const Type& lhs, const Type& rhs) noexcept {
            return lhs < rhs;
        }
        constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { 
            return lhs < rhs; 
        }
    };
    template<typename Type>
    struct CompareGreater {
    public:
        constexpr static Bool compare(const Type& lhs, const Type& rhs) noexcept {
            return lhs > rhs;
        }
        constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { 
            return lhs > rhs; 
        }
    };
    template<typename Type>
    struct CompareLessEqual {
    public:
        constexpr static Bool compare(const Type& lhs, const Type& rhs) noexcept {
            return lhs <= rhs;
        }
        constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { 
            return lhs <= rhs; 
        }
    };
    template<typename Type>
    struct CompareGreaterEqual {
    public:
        constexpr static Bool compare(const Type& lhs, const Type& rhs) noexcept {
            return lhs >= rhs;
        }
        constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { 
            return lhs >= rhs; 
        }
    };
    template<typename Type>
    struct CompareEqualTo {
    public:
        constexpr static Bool compare(const Type& lhs, const Type& rhs) noexcept {
            return lhs == rhs;
        }
        constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept {
            return lhs == rhs;
        }
    };
    template<typename Type>
    struct CompareNotEqualTo {
    public:
        constexpr static Bool compare(const Type& lhs, const Type& rhs) noexcept {
            return lhs != rhs;
        }
        constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept {
            return lhs != rhs;
        }
    };
}