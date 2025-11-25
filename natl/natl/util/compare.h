#pragma once 

//@begin_non_modules
//natl
#include "../util/compilerDependent.h"
//@end_non_modules

//own
#include "typeTraits.h"

//@export
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

    template<typename LhsType, typename RhsType = LhsType>
    struct CompareEqual {
    public:
        constexpr static Bool compare(const LhsType& lhs, const RhsType& rhs) noexcept {
            return lhs == rhs;
        }
        constexpr Bool operator()(const LhsType& lhs, const RhsType& rhs) const noexcept {
            return lhs == rhs;
        }
    };

    template<typename LhsType, typename RhsType = LhsType> 
    concept HasCompareEqualC = requires(const LhsType& lhs, const RhsType& rhs) {
        { CompareEqual<LhsType, RhsType>::compare(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
        { CompareEqual<LhsType, RhsType>{}(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
    };
    template<typename LhsType, typename RhsType = LhsType> struct HasCompareEqualT : BoolConstant<HasCompareEqualC<LhsType, RhsType>> {};
    template<typename LhsType, typename RhsType = LhsType> constexpr inline Bool HasCompareEqual = HasCompareEqualC<LhsType, RhsType>;


    template<typename LhsType, typename RhsType = LhsType>
    struct CompareNotEqual {
    public:
        constexpr static Bool compare(const LhsType& lhs, const RhsType& rhs) noexcept {
            return lhs != rhs;
        }
        constexpr Bool operator()(const LhsType& lhs, const RhsType& rhs) const noexcept {
            return lhs != rhs;
        }
    };

    template<typename LhsType, typename RhsType = LhsType>
    concept HasCompareNotEqualC = requires(const LhsType & lhs, const RhsType & rhs) {
        { CompareNotEqual<LhsType, RhsType>::compare(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
        { CompareNotEqual<LhsType, RhsType>{}(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
    };
    template<typename LhsType, typename RhsType = LhsType> struct HasCompareNotEqualT : BoolConstant<HasCompareNotEqualC<LhsType, RhsType>> {};
    template<typename LhsType, typename RhsType = LhsType> constexpr inline Bool HasCompareNotEqual = HasCompareNotEqualC<LhsType, RhsType>;

    template<typename LhsType, typename RhsType = LhsType>
    struct CompareLess {
    public:
        constexpr static Bool compare(const LhsType& lhs, const RhsType& rhs) noexcept {
            return lhs < rhs;
        }
        constexpr Bool operator()(const LhsType& lhs, const LhsType& rhs) const noexcept { 
            return lhs < rhs; 
        }
    };

    template<typename LhsType, typename RhsType = LhsType>
    concept HasCompareLessC = requires(const LhsType & lhs, const RhsType & rhs) {
        { CompareLess<LhsType, RhsType>::compare(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
        { CompareLess<LhsType, RhsType>{}(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
    };
    template<typename LhsType, typename RhsType = LhsType> struct HasCompareLessT : BoolConstant<HasCompareLessC<LhsType, RhsType>> {};
    template<typename LhsType, typename RhsType = LhsType> constexpr inline Bool HasCompareLess = HasCompareLessC<LhsType, RhsType>;

    template<typename LhsType, typename RhsType = LhsType>
    struct CompareGreater {
    public:
        constexpr static Bool compare(const LhsType& lhs, const RhsType& rhs) noexcept {
            return lhs > rhs;
        }
        constexpr Bool operator()(const LhsType& lhs, const RhsType& rhs) const noexcept { 
            return lhs > rhs; 
        }
    };

    template<typename LhsType, typename RhsType = LhsType>
    concept HasCompareGreaterC = requires(const LhsType & lhs, const RhsType & rhs) {
        { CompareGreater<LhsType, RhsType>::compare(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
        { CompareGreater<LhsType, RhsType>{}(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
    };
    template<typename LhsType, typename RhsType = LhsType> struct HasCompareGreaterT : BoolConstant<HasCompareGreaterC<LhsType, RhsType>> {};
    template<typename LhsType, typename RhsType = LhsType> constexpr inline Bool HasCompareGreater = HasCompareGreaterC<LhsType, RhsType>;

    template<typename LhsType, typename RhsType = LhsType>
    struct CompareLessEqual {
    public:
        constexpr static Bool compare(const LhsType& lhs, const RhsType& rhs) noexcept {
            return lhs <= rhs;
        }
        constexpr Bool operator()(const LhsType& lhs, const RhsType& rhs) const noexcept { 
            return lhs <= rhs; 
        }
    };

    template<typename LhsType, typename RhsType = LhsType>
    concept HasCompareLessEqualC = requires(const LhsType & lhs, const RhsType & rhs) {
        { CompareLessEqual<LhsType, RhsType>::compare(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
        { CompareLessEqual<LhsType, RhsType>{}(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
    };
    template<typename LhsType, typename RhsType = LhsType> struct HasCompareLessEqualT : BoolConstant<HasCompareLessEqualC<LhsType, RhsType>> {};
    template<typename LhsType, typename RhsType = LhsType> constexpr inline Bool HasCompareLessEqual = HasCompareLessEqualC<LhsType, RhsType>;

    template<typename LhsType, typename RhsType = LhsType>
    struct CompareGreaterEqual {
    public:
        constexpr static Bool compare(const LhsType& lhs, const RhsType& rhs) noexcept {
            return lhs >= rhs;
        }
        constexpr Bool operator()(const LhsType& lhs, const RhsType& rhs) const noexcept {
            return lhs >= rhs; 
        }
    };

    template<typename LhsType, typename RhsType = LhsType>
    concept HasCompareGreaterEqualC = requires(const LhsType & lhs, const RhsType & rhs) {
        { CompareGreaterEqual<LhsType, RhsType>::compare(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
        { CompareGreaterEqual<LhsType, RhsType>{}(lhs, rhs) } -> natl::IsSameC<natl::Bool>;
    };
    template<typename LhsType, typename RhsType = LhsType> struct HasCompareGreaterEqualT : BoolConstant<HasCompareGreaterEqualC<LhsType, RhsType>> {};
    template<typename LhsType, typename RhsType = LhsType> constexpr inline Bool HasCompareGreaterEqual = HasCompareGreaterEqualC<LhsType, RhsType>;
}