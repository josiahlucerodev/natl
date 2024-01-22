#pragma once

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "peramaterPackOperations.h"
#include "compilerDependent.h"

//interface
namespace natl {
    namespace impl {
        template<typename DataType>
        NATL_FORCE_INLINE constexpr DataType unitAbsFn(DataType x) noexcept {
            return (x < DataType(0)) ? -x : x;
        }

        template<typename DataType>
        NATL_FORCE_INLINE constexpr DataType unitPowFn(DataType base, i64 exponent) noexcept {
            DataType result = DataType(1);
            for (i64 i = 0; i64(i) < static_cast<i64>(unitAbsFn<i64>(exponent)); ++i) {
                result *= base;
            }
            return (exponent < DataType(0)) ? DataType(1) / result : result;
        }
    }

    struct StrongUnitGroupTypeMagnitude {};
    struct StrongUnitGroupTypeNone {};

    struct LengthUnitCategory {};
    struct MassUnitCategory {};

    template<typename UnitTagGroup>
    concept IsUnitTagGroup = requires() {
        typename UnitTagGroup::group_type;
    };
    template<typename UnitTag>
    concept IsUnitTag = requires() {
        typename UnitTag::tag_group;
    };
    template<typename UnitTag>
    concept IsNotUnitTag = !IsUnitTag<UnitTag>;

    template<typename LhsUnitTage, typename RhsUnitTag>
    concept IsTheSameUnitTagGroup = IsTheSame<typename LhsUnitTage::tag_group, typename RhsUnitTag::tag_group>;

    template<typename LhsUnitTage, typename RhsUnitTag>
    concept IsNotTheSameUnitTagGroup = !IsTheSameUnitTagGroup<LhsUnitTage, RhsUnitTag>;

    template <typename FromTag, typename ToTag, typename FromDataType, typename ToDataType>
    struct StrongUnitConversionFactor {
        static_assert(IsTheSame<FromTag, ToTag>, "Unsupported conversion");
        static_assert(IsNotUnitTag<FromTag>, "FromTag is not a UnitTag");
        static_assert(IsNotUnitTag<ToTag>, "ToTag is not a UnitTag");
        constexpr static FromDataType factor = FromDataType(0);
    };

    template<typename FromTag, typename ToTag>
    concept DoesNotUnitConvertDown = FromTag::magnitudeIndex > ToTag::magnitudeIndex;

    template<typename FromTag, typename ToTag, typename ToDataType>
    concept DoesNotUnitConvertRequireFloat = IsFloatingPointType<ToDataType> || DoesNotUnitConvertDown<FromTag, ToTag>;

    template <typename FromTag, typename ToTag, typename FromDataType, typename ToDataType>
        requires(
            IsUnitTag<FromTag>&&
            IsUnitTag<ToTag> //&& 
        //std::is_same_v<typename FromTag::tag_group, typename ToTag::tag_group> &&
        //IsUnitTagGroup<typename FromTag::tag_group> && 
        //std::is_same_v<typename FromTag::tag_group, StrongUnitGroupTypeMagnitude>
        )
        struct StrongUnitConversionFactor<FromTag, ToTag, FromDataType, ToDataType> {
        static_assert(IsNotTheSame<FromTag, ToTag>, "Unsupported conversion");
        static_assert(DoesNotUnitConvertRequireFloat<FromTag, ToTag, ToDataType>, "converting down requires a base float type");
        using tag_group = typename FromTag::tag_group;
        constexpr static ToDataType value = impl::unitPowFn<ToDataType>(tag_group::orderOfMagnitude, FromTag::magnitudeIndex - ToTag::magnitudeIndex);
    };

    template<class UnitTag, i64 Magnitude>
    struct Unit {
        using unit_tag = UnitTag;
        constexpr static i64 magnitude = Magnitude;
    };

    struct MeterTag;

    template<typename DataType, typename... Units>
    struct UnitValue {
        using value_type = DataType;
        using units_type = TypePack<Units...>;

    public:
        DataType value;
    public:
        //constructor
        NATL_FORCE_INLINE constexpr UnitValue() noexcept = default;
        NATL_FORCE_INLINE constexpr explicit UnitValue(const DataType value) noexcept : value(value) {}

        //destructor
        NATL_FORCE_INLINE constexpr ~UnitValue() noexcept = default;

        //util
        NATL_FORCE_INLINE constexpr UnitValue& self() noexcept { return *this; }
        NATL_FORCE_INLINE constexpr const UnitValue& self() const noexcept { return *this; }

        //assignment 
        NATL_FORCE_INLINE constexpr UnitValue& operator=(const UnitValue&) noexcept = default;

        //observers 
        NATL_FORCE_INLINE constexpr DataType& getValue() { return value; }
        NATL_FORCE_INLINE constexpr const DataType& getValue() const { return value; }

        template<typename Interger>
            requires(IsItergerType<Interger>)
        NATL_FORCE_INLINE constexpr Interger asInt() const noexcept requires(IsFloatingPointType<DataType>) {
            return static_cast<Interger>(value);
        }
        template<typename Float>
            requires(IsFloatingPointType<Float>)
        NATL_FORCE_INLINE constexpr Float asFloat() const noexcept requires(IsItergerType<DataType>) {
            return static_cast<Float>(value);
        }

        //operator 
        NATL_FORCE_INLINE constexpr UnitValue operator+(const UnitValue& other) const noexcept { return UnitValue(value + other.value); }
        NATL_FORCE_INLINE constexpr UnitValue operator-(const UnitValue& other) const noexcept { return UnitValue(value - other.value); }
        NATL_FORCE_INLINE constexpr UnitValue operator*(const UnitValue& other) const noexcept { return UnitValue(value * other.value); }
        NATL_FORCE_INLINE constexpr UnitValue operator/(const UnitValue& other) const noexcept { return UnitValue(value / other.value); }

        //operators 
        NATL_FORCE_INLINE constexpr UnitValue& operator+=(const UnitValue& other) noexcept {
            value += other.value;
            return *this;
        }
        NATL_FORCE_INLINE constexpr UnitValue& operator-=(const UnitValue& other) noexcept {
            value -= other.value;
            return *this;
        }
        NATL_FORCE_INLINE constexpr UnitValue& operator*=(const UnitValue& other) noexcept {
            value *= other.value;
            return *this;
        }
        NATL_FORCE_INLINE constexpr UnitValue& operator/=(const UnitValue& other) noexcept {
            value /= other.value;
            return *this;
        }

        //compare 
        NATL_FORCE_INLINE constexpr bool operator==(const UnitValue& other) const noexcept { return value == other.value; }
        NATL_FORCE_INLINE constexpr bool operator!=(const UnitValue& other) const noexcept { return value != other.value; }
        NATL_FORCE_INLINE constexpr bool operator<(const UnitValue& other) const noexcept { return value < other.value; }
        NATL_FORCE_INLINE constexpr bool operator>(const UnitValue& other) const noexcept { return value > other.value; }
        NATL_FORCE_INLINE constexpr bool operator<=(const UnitValue& other) const noexcept { return value <= other.value; }
        NATL_FORCE_INLINE constexpr bool operator>=(const UnitValue& other) const noexcept { return value >= other.value; }
        NATL_FORCE_INLINE constexpr auto operator<=>(const UnitValue& other) const noexcept { return value <=> other.value; }

    private:
        template <typename... UnitTypes>
        struct CalculateMagnitudeTypePack {};
        template <typename TargetUnit, typename... UnitTypes>
        struct CalculateMagnitudeTypePack<TargetUnit, TypePack<UnitTypes...>> {
            constexpr static i64 magnitude = TargetUnit::magnitude + ((static_cast<i64>(std::is_same_v<typename TargetUnit::unit_tag, typename UnitTypes::unit_tag>) * UnitTypes::magnitude) + ...);
        };

        template<typename LhsUnit, typename RhsUnit>
        struct StructSameUnitGroupError {
            constexpr static bool value = !(!std::is_same_v<typename LhsUnit::unit_tag, typename RhsUnit::unit_tag> &&
                std::is_same_v<typename LhsUnit::unit_tag::tag_group, typename RhsUnit::unit_tag::tag_group>);
        };


        template <typename... UnitTypes>
        struct CalculateMagnitudeSubTypePack {};
        template <typename TargetUnit, typename... UnitTypes>
        struct CalculateMagnitudeSubTypePack<TargetUnit, TypePack<UnitTypes...>> {
            static_assert((StructSameUnitGroupError<TargetUnit, UnitTypes>::value || ...),
                "natl: Unit div - doing an operation with a unit form the same group but of differnt type. Requires conversion");
            constexpr static i64 magnitude = TargetUnit::magnitude - ((static_cast<i64>(std::is_same_v<typename TargetUnit::unit_tag, typename UnitTypes::unit_tag>) * UnitTypes::magnitude) + ...);
        };

        template <typename... UnitTypes>
        struct CalculateConversionFactor {};
        template <typename TargetUnit, typename... UnitTypes>
        struct CalculateConversionFactor<TargetUnit, TypePack<UnitTypes...>> {
            static_assert((StructSameUnitGroupError<TargetUnit, UnitTypes>::value || ...),
                "natl: Unit div - doing an operation with a unit form the same group but of differnt type. Requires conversion");
            constexpr static i64 magnitude = TargetUnit::magnitude - ((static_cast<i64>(std::is_same_v<typename TargetUnit::unit_tag, typename UnitTypes::unit_tag>) * UnitTypes::magnitude) + ...);
        };
        
        template<typename TransfromUnit, typename UnitTypePack>
        struct AddTransform {
            using type = Unit<typename TransfromUnit::unit_tag, CalculateMagnitudeTypePack<TransfromUnit, UnitTypePack>::magnitude>;
        };
        template<typename TransfromUnit, typename UnitTypePack>
        struct SubTransform {
            using type = Unit<typename TransfromUnit::unit_tag, CalculateMagnitudeSubTypePack<TransfromUnit, UnitTypePack>::magnitude>;
            //using type = TransfromUnit;
        };

        template<typename LhsUnit, typename RhsUnit>
        struct UnitCompare {
            static_assert(StructSameUnitGroupError<LhsUnit, RhsUnit>::value,
                "natl: Unit mul - doing an operation with a unit form the same group but of differnt type. Requires conversion");
            constexpr static bool value = std::is_same_v<typename LhsUnit::unit_tag, typename RhsUnit::unit_tag>;
        };

        template<typename TestUnit>
        struct UnitRemoveIfZero {
            constexpr static bool value = TestUnit::magnitude == 0;
        };



        template<typename TransfromUnit, typename NewUnit> 
        struct UnitConvertUnitTest {
            static_assert(TransfromUnit::magnitude == NewUnit::magnitude, "natl: Unit convert - unsupport conversion - unit magnitudes are different");
            using type = NewUnit;
        };

        template<typename LhsUnit, typename RhsUnit>
        struct UnitGroupUnitCategoryCompare {
            constexpr static bool value =  std::is_same_v<typename LhsUnit::unit_tag::tag_group::unit_category, typename RhsUnit::unit_tag::tag_group::unit_category>;
        };

        template<typename TransfromUnit, typename UnitTypePack>
        struct UnitConvertTransformUnitCategoryTransform {
            constexpr static Size index = TypePackFindIndexOfTypeCompareValue<UnitGroupUnitCategoryCompare, TransfromUnit, UnitTypePack>;
            static_assert(index == TypePackIndexNotFound::value, "natl: Unit convert - unsupport conversion - could not find matching tag_group or unit_category");
            using NewUnit = typename UnitConvertUnitTest<TransfromUnit, TypePackNthElement<index, UnitTypePack>>::type;
            using type = NewUnit;
        };

        template<Size index, typename UnitTypePack>
        struct BaseUnitConvertType {
            using NewUnit = TypePackNthElement<index, UnitTypePack>;
        };

        public:
        template<typename LhsUnit, typename RhsUnit>
        struct UnitValueGroupCompare {
            using LhsUnitTag = LhsUnit::unit_tag;
            using RhsUnitTag = RhsUnit::unit_tag;
            constexpr static bool value = std::is_same_v<typename LhsUnitTag::tag_group, typename RhsUnitTag::tag_group>;
        };
    public:
        template<typename TransfromUnit, typename UnitTypePack>
        struct UnitConvertTransform {
            constexpr static Size index = TypePackFindIndexOfTypeCompareValue<UnitValueGroupCompare, TransfromUnit, UnitTypePack>;
            constexpr static auto chooseConvertTypeFunc() noexcept {
                if constexpr (index != TypePackIndexNotFound::value) {
                    return TypePackNthElement<index, UnitTypePack>();
                } else {
                    using CovertType = typename UnitConvertUnitTest<
                        TransfromUnit,
                        typename UnitConvertTransformUnitCategoryTransform<TransfromUnit, UnitTypePack>::type
                    >::type;
                    return CovertType();
                }
            }
            using NewUnit = decltype(chooseConvertTypeFunc());
            using type = NewUnit;
        };

        template<typename UnitType, Size Index, typename TransformedUnitTypePack>
        struct UnitConvertFactorValuePredicate {
            using TransformToType = TypePackNthElement<Index, TransformedUnitTypePack>;

            constexpr static DataType getConvertFactorFunc() noexcept {
                if constexpr (std::is_same_v<UnitType, TransformToType>) {
                    return TypeValue<DataType(1)>;
                } else {
                    return StrongUnitConversionFactor<typename UnitType::unit_tag, typename TransformToType::unit_tag, DataType, DataType>::value;
                }
            }

            constexpr static DataType value = getConvertFactorFunc();
        };

        template<DataType LhsValue, DataType RhsValue>
        struct UnitConvertFactorMultiplyOp {
            constexpr static DataType value = LhsValue * RhsValue;
        };

        template<typename... NewUnits>
        struct ConvertNewUnitType {
            using type = UnitValue<DataType, NewUnits...>;
        };

        //unit operation 
        template<typename... OtherUnits>
        NATL_FORCE_INLINE constexpr auto operator*(const UnitValue<DataType, OtherUnits...>& other) const noexcept {
            using TransformedUnitsType = TypePackTransformWithTypePackArgs<AddTransform, TypePack<OtherUnits...>, TypePack<Units...>>;
            using NewUnitTypePack = TypePackAddUnique<UnitCompare, TransformedUnitsType, OtherUnits...>;
            using UnitValueArgTypePack = typename NewUnitTypePack::template add_new_elements_front<DataType>;
            using NewCreatedUnitValueType = CreateTypeWithTypePack<UnitValue, UnitValueArgTypePack>;
            using NewUnitValueType = NewCreatedUnitValueType;
            return NewUnitValueType(value * other.value);
        }
        

        template<typename... OtherUnits>
        NATL_FORCE_INLINE constexpr auto operator/(const UnitValue<DataType, OtherUnits...>& other) const noexcept {
            using TransformedUnitsType = TypePackTransformWithTypePackArgs<SubTransform, TypePack<OtherUnits...>, TypePack<Units...>>;
            using NewUnitTypePack = TypePackRemoveElementsIf<UnitRemoveIfZero, TransformedUnitsType>;
            using UnitValueArgTypePack = typename NewUnitTypePack::template add_new_elements_front<DataType>;
            using NewCreatedUnitValueType = CreateTypeWithTypePack<UnitValue, UnitValueArgTypePack>;
            using NewUnitValueType = NewCreatedUnitValueType;
            return NewUnitValueType(value / other.value);
        }

        template<typename... OtherUnits>
        NATL_FORCE_INLINE constexpr auto convertTo() const noexcept {
            using UnitsTypePack = TypePack<Units...>;
            using TransformedUnitsType = TypePackTransformWithTypePackArgs<UnitConvertTransform, TypePack<OtherUnits...>, UnitsTypePack>;
            constexpr DataType convertValue = TypePackOpFoldWithIndexAndArgValue<DataType, TransformedUnitsType,
                UnitConvertFactorValuePredicate, UnitConvertFactorMultiplyOp, UnitsTypePack>;
            using NewUnitValueType = CreateTypeWithTypePack<ConvertNewUnitType, TransformedUnitsType>::type;
            const DataType newValue = convertValue * value;
            return NewUnitValueType(newValue);
        }

    };

    template<typename InputUnitTag>
        requires(IsUnitTag<InputUnitTag>)
    using UnitSquared = Unit<InputUnitTag, 2>;

    template<typename InputUnitTag>
        requires(IsUnitTag<InputUnitTag>)
    using UnitCubed = Unit<InputUnitTag, 3>;

    //metric

    //metric length
    struct MetricLengthGroupTag {
        using unit_category = LengthUnitCategory;
        using group_type = StrongUnitGroupTypeMagnitude;
        constexpr static i64 orderOfMagnitude = 10;
    };

    struct MillimeterTag {
        using tag_group = MetricLengthGroupTag;
        constexpr static i64 magnitudeIndex = -3;
    };
    template<i64 Magnitude>
    using MillimeterUnit = Unit<MillimeterTag, Magnitude>;
    template<class DataType> using MillimeterValue = UnitValue<DataType, MillimeterUnit<1>>;
    using Millimeter = MillimeterValue<i64>;

    struct CentimeterTag {
        using tag_group = MetricLengthGroupTag;
        constexpr static i64 magnitudeIndex = -2;
    };
    template<i64 Magnitude>
    using CentimeterUnit = Unit<CentimeterTag, Magnitude>;
    template<class DataType> using CentimeterValue = UnitValue<DataType, CentimeterUnit<1>>;
    using Centimeter = CentimeterValue<i64>;

    struct DecimeterTag {
        using tag_group = MetricLengthGroupTag;
        constexpr static i64 magnitudeIndex = -1;
    };
    template<i64 Magnitude>
    using DecimeterUnit = Unit<DecimeterTag, Magnitude>;
    template<class DataType> using DecimeterValue = UnitValue<DataType, DecimeterUnit<1>>;
    using Decimeter = DecimeterValue<i64>;

    struct MeterTag {
        using tag_group = MetricLengthGroupTag;
        constexpr static i64 magnitudeIndex = 0;
    };
    template<i64 Magnitude>
    using MeterUnit = Unit<MeterTag, Magnitude>;
    template<class DataType> using MeterValue = UnitValue<DataType, MeterUnit<1>>;
    using Meter = MeterValue<i64>;

    struct DecameterTag {
        using tag_group = MetricLengthGroupTag;
        constexpr static i64 magnitudeIndex = 1;
    };
    template<i64 Magnitude>
    using DecameterUnit = Unit<DecameterTag, Magnitude>;
    template<class DataType> using DecameterValue = UnitValue<DataType, DecameterUnit<1>>;
    using Decameter = DecameterValue<i64>;

    struct HectometerTag {
        using tag_group = MetricLengthGroupTag;
        constexpr static i64 magnitudeIndex = 2;
    };
    template<i64 Magnitude>
    using HectometerUnit = Unit<HectometerTag, Magnitude>;
    template<class DataType> using HectometerValue = UnitValue<DataType, HectometerUnit<1>>;
    using Hectometer = HectometerValue<i64>;

    struct KilometerTag {
        using tag_group = MetricLengthGroupTag;
        constexpr static i64 magnitudeIndex = 3;
    };
    template<i64 Magnitude>
    using KilometerUnit = Unit<KilometerTag, Magnitude>;
    template<class DataType> using KilometerValue = UnitValue<DataType, KilometerUnit<1>>;
    using Kilometer = KilometerValue<i64>;

    //metric length
    struct MetricMassGroupTag {
        using unit_category = MassUnitCategory;
        using group_type = StrongUnitGroupTypeMagnitude;
        constexpr static i64 orderOfMagnitude = 10;
    };

    struct MilligramTag {
        using tag_group = MetricMassGroupTag;
        constexpr static i64 magnitudeIndex = -3;
    };
    template<i64 Magnitude>
    using MilligramUnit = Unit<MilligramTag, Magnitude>;
    template<class DataType> using MilligramValue = UnitValue<DataType, MilligramUnit<1>>;
    using Milligram = MilligramValue<i64>;

    struct CentigramTag {
        using tag_group = MetricMassGroupTag;
        constexpr static i64 magnitudeIndex = -2;
    };
    template<i64 Magnitude>
    using CentigramUnit = Unit<CentigramTag, Magnitude>;
    template<class DataType> using CentigramValue = UnitValue<DataType, CentigramUnit<1>>;
    using Centigram = CentigramValue<i64>;

    struct DecigramTag {
        using tag_group = MetricMassGroupTag;
        constexpr static i64 magnitudeIndex = -1;
    };
    template<i64 Magnitude>
    using DecigramUnit = Unit<DecigramTag, Magnitude>;
    template<class DataType> using DecigramValue = UnitValue<DataType, DecigramUnit<1>>;
    using Decigram = DecigramValue<i64>;

    struct GramTag {
        using tag_group = MetricMassGroupTag;
        constexpr static i64 magnitudeIndex = 0;
    };
    template<i64 Magnitude>
    using GramUnit = Unit<GramTag, Magnitude>;
    template<class DataType> using GramValue = UnitValue<DataType, GramUnit<1>>;
    using Gram = GramValue<i64>;

    struct DecagramTag {
        using tag_group = MetricMassGroupTag;
        constexpr static i64 magnitudeIndex = 1;
    };
    template<i64 Magnitude>
    using DecagramUnit = Unit<DecagramTag, Magnitude>;
    template<class DataType> using DecagramValue = UnitValue<DataType, DecagramUnit<1>>;
    using Decagram = DecagramValue<i64>;

    struct HectogramTag {
        using tag_group = MetricMassGroupTag;
        constexpr static i64 magnitudeIndex = 2;
    };
    template<i64 Magnitude>
    using HectogramUnit = Unit<HectogramTag, Magnitude>;
    template<class DataType> using HectogramValue = UnitValue<DataType, HectogramUnit<1>>;
    using Hectogram = HectogramValue<i64>;

    struct KilogramTag {
        using tag_group = MetricMassGroupTag;
        constexpr static i64 magnitudeIndex = 3;
    };
    template<i64 Magnitude>
    using KilogramUnit = Unit<KilogramTag, Magnitude>;
    template<class DataType> using KilogramValue = UnitValue<DataType, KilogramUnit<1>>;
    using Kilogram = KilogramValue<i64>;


}

#undef NATL_FORCE_INLINE