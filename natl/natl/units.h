#pragma once

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "typePack.h"
#include "compilerDependent.h"
#include "array.h"

//interface
namespace natl {
    namespace impl {
        template<typename DataType>
        NATL_FORCE_INLINE constexpr DataType unitAbsFn(DataType x) noexcept {
            return (x < DataType(0)) ? -x : x;
        }

        template<typename DataType>
        NATL_FORCE_INLINE constexpr DataType unitPowFn(DataType base, SSize exponent) noexcept {
            DataType result = DataType(1);
            for (SSize i = 0; SSize(i) < static_cast<SSize>(unitAbsFn<SSize>(exponent)); ++i) {
                result *= base;
            }
            return (static_cast<DataType>(exponent) < DataType(0)) ? DataType(1) / result : result;
        }
    }

    struct StrongUnitGroupTypeMagnitude {};
    struct StrongUnitGroupTypeTable {};
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
    concept IsTheSameUnitTagGroup = IsSame<typename LhsUnitTage::tag_group, typename RhsUnitTag::tag_group>;

    template<typename LhsUnitTage, typename RhsUnitTag>
    concept IsNotTheSameUnitTagGroup = !IsTheSameUnitTagGroup<LhsUnitTage, RhsUnitTag>;

    template <typename FromTag, typename ToTag, typename FromDataType, typename ToDataType>
    struct StrongUnitConversionFactor {
        static_assert(IsSame<FromTag, ToTag>, "Unsupported conversion");
        static_assert(IsNotUnitTag<FromTag>, "FromTag is not a UnitTag");
        static_assert(IsNotUnitTag<ToTag>, "ToTag is not a UnitTag");
        constexpr static FromDataType factor = FromDataType(0);
    };

    template<typename FromTag, typename ToTag>
    concept DoesNotUnitConvertDown = FromTag::magnitudeIndex > ToTag::magnitudeIndex;

    template<typename FromTag, typename ToTag, typename ToDataType>
    concept DoesNotUnitConvertRequireFloat = IsBuiltInFloatingPoint<ToDataType> || DoesNotUnitConvertDown<FromTag, ToTag>;

    //magnitude based conversion
    template <typename FromTag, typename ToTag, typename FromDataType, typename ToDataType>
        requires(IsUnitTag<FromTag> 
            && IsUnitTag<ToTag> 
            && std::is_same_v<typename FromTag::tag_group, typename ToTag::tag_group>
            && std::is_same_v<typename FromTag::tag_group::group_type, StrongUnitGroupTypeMagnitude>)
        struct StrongUnitConversionFactor<FromTag, ToTag, FromDataType, ToDataType> {

        static_assert(DoesNotUnitConvertRequireFloat<FromTag, ToTag, ToDataType>, "converting down requires a base float type");
        using tag_group = typename FromTag::tag_group;
        constexpr static ToDataType value = impl::unitPowFn<ToDataType>(tag_group::orderOfMagnitude, FromTag::magnitudeIndex - ToTag::magnitudeIndex);
    };

    //table based conversion 
    template <typename FromTag, typename ToTag, typename FromDataType, typename ToDataType>
        requires(
            IsUnitTag<FromTag>
            && IsUnitTag<ToTag> 
            && IsSame<typename FromTag::tag_group, typename ToTag::tag_group>
            && IsSame<typename FromTag::tag_group::group_type, StrongUnitGroupTypeTable>)
        struct StrongUnitConversionFactor<FromTag, ToTag, FromDataType, ToDataType> {

        static_assert(!(!IsBuiltInFloatingPoint<ToDataType> && (FromTag::tableIndex > ToTag::tableIndex)), "converting down requires a base float type");
        using tag_group = typename FromTag::tag_group;
        constexpr static ToDataType value =  
            static_cast<ToDataType>(tag_group::convertionTable[FromTag::tableIndex]) 
            / static_cast<ToDataType>(tag_group::convertionTable[ToTag::tableIndex]);
    };

    template<typename FromTagGroup, typename ToTagGroup, typename FromTag, typename ToTag, typename FloatDataType>
    struct UnitCategoryConversionFactor;

    namespace impl {
        template<typename FloatDataType, FloatDataType defaultUnitsConvertFactor,
            typename FormDefaultUnitTag, typename ToDefaultUnitTag,
            typename FromTag, typename ToTag>
        struct UnitCategoryConversionFactorImpl {
            constexpr static FloatDataType srcTodefaultUnitConvertFactor = StrongUnitConversionFactor<FromTag, FormDefaultUnitTag, FloatDataType, FloatDataType>::value;
            constexpr static FloatDataType defaultUnitToDstConvertFactor = StrongUnitConversionFactor<ToDefaultUnitTag, ToTag, FloatDataType, FloatDataType>::value;
            constexpr static FloatDataType value = srcTodefaultUnitConvertFactor * defaultUnitsConvertFactor * defaultUnitToDstConvertFactor;
        };
    }

    //unit category change conversion 
    template <typename FromTag, typename ToTag, typename FromDataType, typename ToDataType>
        requires(
            IsUnitTag<FromTag>
            && IsUnitTag<ToTag> 
            && IsNotSame<typename FromTag::tag_group, typename ToTag::tag_group>
            && IsSame<typename FromTag::tag_group::unit_category, typename ToTag::tag_group::unit_category>)
        struct StrongUnitConversionFactor<FromTag, ToTag, FromDataType, ToDataType> {

        static_assert(IsBuiltInFloatingPoint<ToDataType>, "converting to different unit group requires a base float type");
        using from_tag_group = typename FromTag::tag_group;
        using to_tag_group = typename ToTag::tag_group;
        constexpr static ToDataType value = UnitCategoryConversionFactor<from_tag_group, to_tag_group, FromTag, ToTag, ToDataType>::value;
    };

    template<class UnitTag, SSize Magnitude>
    struct Unit {
        using unit_tag = UnitTag;
        constexpr static SSize magnitude = Magnitude;
    };

    template<typename DataType, typename... Units>
    struct UnitValue {
        using value_type = DataType;
        using units_type = TypePack<Units...>;

    public:
        DataType internalData;
    public:
        //constructor
        NATL_FORCE_INLINE constexpr UnitValue() noexcept = default;
        NATL_FORCE_INLINE constexpr explicit UnitValue(const DataType dataIn) noexcept : internalData(dataIn) {}

        //destructor
        NATL_FORCE_INLINE constexpr ~UnitValue() noexcept = default;

        //util
        NATL_FORCE_INLINE constexpr UnitValue& self() noexcept { return *this; }
        NATL_FORCE_INLINE constexpr const UnitValue& self() const noexcept { return *this; }

        //assignment 
        NATL_FORCE_INLINE constexpr UnitValue& operator=(const UnitValue&) noexcept = default;

        //observers 
        NATL_FORCE_INLINE constexpr DataType& value() { return internalData; }
        NATL_FORCE_INLINE constexpr const DataType& value() const { return internalData; }

        template<typename Interger>
            requires(IsBuiltInInterger<Interger>)
        NATL_FORCE_INLINE constexpr Interger valueAsInt() const noexcept requires(IsBuiltInFloatingPoint<DataType>) {
            return static_cast<Interger>(internalData);
        }
        template<typename Float>
            requires(IsBuiltInFloatingPoint<Float>)
        NATL_FORCE_INLINE constexpr Float valueAsFloat() const noexcept requires(IsBuiltInInterger<DataType>) {
            return static_cast<Float>(internalData);
        }

        template<typename Interger>
            requires(IsBuiltInFloatingPoint<Interger>)
        NATL_FORCE_INLINE constexpr UnitValue<Interger, Units...> asInt() const noexcept requires(IsBuiltInInterger<DataType>) {
            return UnitValue<Interger, Units...>(static_cast<Interger>(internalData));
        }
        template<typename Float>
            requires(IsBuiltInFloatingPoint<Float>)
        NATL_FORCE_INLINE constexpr UnitValue<Float, Units...> asFloat() const noexcept requires(IsBuiltInInterger<DataType>) {
            return UnitValue<Float, Units...>(static_cast<Float>(internalData));
        }



        //operator 
        NATL_FORCE_INLINE constexpr UnitValue operator+(const UnitValue& other) const noexcept { return UnitValue(internalData + other.internalData); }
        NATL_FORCE_INLINE constexpr UnitValue operator-(const UnitValue& other) const noexcept { return UnitValue(internalData - other.internalData); }
        NATL_FORCE_INLINE constexpr UnitValue operator*(const UnitValue& other) const noexcept { return UnitValue(internalData * other.internalData); }
        NATL_FORCE_INLINE constexpr UnitValue operator/(const UnitValue& other) const noexcept { return UnitValue(internalData / other.internalData); }

        //operators 
        NATL_FORCE_INLINE constexpr UnitValue& operator+=(const UnitValue& other) noexcept {
            internalData += other.internalData;
            return *this;
        }
        NATL_FORCE_INLINE constexpr UnitValue& operator-=(const UnitValue& other) noexcept {
            internalData -= other.internalData;
            return *this;
        }
        NATL_FORCE_INLINE constexpr UnitValue& operator*=(const UnitValue& other) noexcept {
            internalData *= other.internalData;
            return *this;
        }
        NATL_FORCE_INLINE constexpr UnitValue& operator/=(const UnitValue& other) noexcept {
            internalData /= other.internalData;
            return *this;
        }

        //compare 
        NATL_FORCE_INLINE constexpr Bool operator==(const UnitValue& other) const noexcept { return internalData == other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator!=(const UnitValue& other) const noexcept { return internalData != other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator<(const UnitValue& other) const noexcept { return internalData < other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator>(const UnitValue& other) const noexcept { return internalData > other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator<=(const UnitValue& other) const noexcept { return internalData <= other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator>=(const UnitValue& other) const noexcept { return internalData >= other.internalData; }
        NATL_FORCE_INLINE constexpr auto operator<=>(const UnitValue& other) const noexcept { return internalData <=> other.internalData; }

    private:
        template <typename... UnitTypes>
        struct CalculateMagnitudeTypePack {};
        template <typename TargetUnit, typename... UnitTypes>
        struct CalculateMagnitudeTypePack<TargetUnit, TypePack<UnitTypes...>> {
            constexpr static SSize magnitude = TargetUnit::magnitude + ((static_cast<SSize>(std::is_same_v<typename TargetUnit::unit_tag, typename UnitTypes::unit_tag>) * UnitTypes::magnitude) + ...);
        };

        template<typename LhsUnit, typename RhsUnit>
        struct StructSameUnitGroupError {
            constexpr static Bool value = !(!std::is_same_v<typename LhsUnit::unit_tag, typename RhsUnit::unit_tag> &&
                std::is_same_v<typename LhsUnit::unit_tag::tag_group, typename RhsUnit::unit_tag::tag_group>);
        };


        template <typename... UnitTypes>
        struct CalculateMagnitudeSubTypePack {};
        template <typename TargetUnit, typename... UnitTypes>
        struct CalculateMagnitudeSubTypePack<TargetUnit, TypePack<UnitTypes...>> {
            static_assert((StructSameUnitGroupError<TargetUnit, UnitTypes>::value || ...),
                "natl: Unit div - doing an operation with a unit form the same group but of differnt type. Requires conversion");
            constexpr static SSize magnitude = TargetUnit::magnitude - ((static_cast<SSize>(std::is_same_v<typename TargetUnit::unit_tag, typename UnitTypes::unit_tag>) * UnitTypes::magnitude) + ...);
        };

        template <typename... UnitTypes>
        struct CalculateConversionFactor {};
        template <typename TargetUnit, typename... UnitTypes>
        struct CalculateConversionFactor<TargetUnit, TypePack<UnitTypes...>> {
            static_assert((StructSameUnitGroupError<TargetUnit, UnitTypes>::value || ...),
                "natl: Unit div - doing an operation with a unit form the same group but of differnt type. Requires conversion");
            constexpr static SSize magnitude = TargetUnit::magnitude - ((static_cast<SSize>(std::is_same_v<typename TargetUnit::unit_tag, typename UnitTypes::unit_tag>) * UnitTypes::magnitude) + ...);
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
            constexpr static Bool value = std::is_same_v<typename LhsUnit::unit_tag, typename RhsUnit::unit_tag>;
        };

        template<typename TestUnit>
        struct UnitRemoveIfZero {
            constexpr static Bool value = TestUnit::magnitude == 0;
        };

        template<typename TransfromUnit, typename NewUnit> 
        struct UnitConvertUnitTest {
            static_assert(TransfromUnit::magnitude == NewUnit::magnitude, "natl: Unit convert - unsupport conversion - unit magnitudes are different");
            using type = NewUnit;
        };

        template<typename LhsUnit, typename RhsUnit>
        struct UnitGroupUnitCategoryCompare {
            constexpr static Bool value =  
                IsSame<typename LhsUnit::unit_tag::tag_group::unit_category, typename RhsUnit::unit_tag::tag_group::unit_category>;
        };

        template<typename TransfromUnit, typename UnitTypePack>
        struct UnitConvertTransformUnitCategoryTransform {
            constexpr static Size index = TypePackFindIndexOfTypeCompareValue<UnitGroupUnitCategoryCompare, TransfromUnit, UnitTypePack>;
            static_assert(!(index == IndexNotFound::value), "natl: Unit convert - unsupport conversion - could not find matching tag_group or unit_category");
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
            constexpr static Bool value = std::is_same_v<typename LhsUnitTag::tag_group, typename RhsUnitTag::tag_group>;
        };
    public:
        template<typename TransfromUnit, typename UnitTypePack>
        struct UnitConvertTransform {
            constexpr static Size index = TypePackFindIndexOfTypeCompareValue<UnitValueGroupCompare, TransfromUnit, UnitTypePack>;
            constexpr static auto chooseConvertTypeFunc() noexcept {
                if constexpr (index != IndexNotFound::value) {
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
            constexpr static DataType baseConvertFactor = StrongUnitConversionFactor<typename UnitType::unit_tag, typename TransformToType::unit_tag, DataType, DataType>::value;
            constexpr static DataType value = impl::unitPowFn<DataType>(baseConvertFactor, UnitType::magnitude);
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
            return NewUnitValueType(internalData * other.internalData);
        }
        

        template<typename... OtherUnits>
        NATL_FORCE_INLINE constexpr auto operator/(const UnitValue<DataType, OtherUnits...>& other) const noexcept {
            using TransformedUnitsType = TypePackTransformWithTypePackArgs<SubTransform, TypePack<OtherUnits...>, TypePack<Units...>>;
            using NewUnitTypePack = TypePackRemoveElementsIf<UnitRemoveIfZero, TransformedUnitsType>;
            using UnitValueArgTypePack = typename NewUnitTypePack::template add_new_elements_front<DataType>;
            using NewCreatedUnitValueType = CreateTypeWithTypePack<UnitValue, UnitValueArgTypePack>;
            using NewUnitValueType = NewCreatedUnitValueType;
            return NewUnitValueType(internalData / other.internalData);
        }

        template<typename... OtherUnits>
        NATL_FORCE_INLINE constexpr auto convertTo() const noexcept {
            using UnitsTypePack = TypePack<Units...>;
            using TransformedUnitsType = TypePackTransformWithTypePackArgs<UnitConvertTransform, TypePack<OtherUnits...>, UnitsTypePack>;
            constexpr DataType convertValue = TypePackOpFoldWithIndexAndArgValue<DataType, TransformedUnitsType,
                UnitConvertFactorValuePredicate, UnitConvertFactorMultiplyOp, UnitsTypePack>;
            using NewUnitValueType = CreateTypeWithTypePack<ConvertNewUnitType, TransformedUnitsType>::type;
            const DataType newValue = convertValue * internalData;
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
    namespace impl {
        struct MetricLengthGroupTag {
            using unit_category = LengthUnitCategory;
            using group_type = StrongUnitGroupTypeMagnitude;
            constexpr static SSize orderOfMagnitude = 10;
        };
        struct MillimeterTag {
            using tag_group = MetricLengthGroupTag;
            constexpr static SSize magnitudeIndex = -3;
        };
        struct CentimeterTag {
            using tag_group = MetricLengthGroupTag;
            constexpr static SSize magnitudeIndex = -2;
        };
        struct DecimeterTag {
            using tag_group = MetricLengthGroupTag;
            constexpr static SSize magnitudeIndex = -1;
        };
        struct MeterTag {
            using tag_group = MetricLengthGroupTag;
            constexpr static SSize magnitudeIndex = 0;
        };
        struct DecameterTag {
            using tag_group = MetricLengthGroupTag;
            constexpr static SSize magnitudeIndex = 1;
        };
        struct HectometerTag {
            using tag_group = MetricLengthGroupTag;
            constexpr static SSize magnitudeIndex = 2;
        };
        struct KilometerTag {
            using tag_group = MetricLengthGroupTag;
            constexpr static SSize magnitudeIndex = 3;
        };
    }

    template<SSize Magnitude>
    using MillimeterUnit = Unit<impl::MillimeterTag, Magnitude>;
    template<SSize Magnitude>
    using CentimeterUnit = Unit<impl::CentimeterTag, Magnitude>;
    template<SSize Magnitude>
    using DecimeterUnit = Unit<impl::DecimeterTag, Magnitude>;
    template<SSize Magnitude>
    using MeterUnit = Unit<impl::MeterTag, Magnitude>;
    template<SSize Magnitude>
    using DecameterUnit = Unit<impl::DecameterTag, Magnitude>;
    template<SSize Magnitude>
    using HectometerUnit = Unit<impl::HectometerTag, Magnitude>;
    template<SSize Magnitude>
    using KilometerUnit = Unit<impl::KilometerTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1> 
    using Millimeter = UnitValue<DataType, MillimeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1> 
    using Centimeter = UnitValue<DataType, CentimeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1> 
    using Decimeter = UnitValue<DataType, DecimeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1> 
    using Meter = UnitValue<DataType, MeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1> 
    using Decameter = UnitValue<DataType, DecameterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1> 
    using Hectometer = UnitValue<DataType, HectometerUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1> 
    using Kilometer = UnitValue<DataType, KilometerUnit<Magnitude>>;

   //metric length
    namespace impl {
        struct MetricMassGroupTag {
            using unit_category = MassUnitCategory;
            using group_type = StrongUnitGroupTypeMagnitude;
            constexpr static SSize orderOfMagnitude = 10;
        };
        struct MilligramTag {
            using tag_group = MetricMassGroupTag;
            constexpr static SSize magnitudeIndex = -3;
        };
        struct CentigramTag {
            using tag_group = MetricMassGroupTag;
            constexpr static SSize magnitudeIndex = -2;
        };
        struct DecigramTag {
            using tag_group = MetricMassGroupTag;
            constexpr static SSize magnitudeIndex = -1;
        };
        struct GramTag {
            using tag_group = MetricMassGroupTag;
            constexpr static SSize magnitudeIndex = 0;
        };
        struct DecagramTag {
            using tag_group = MetricMassGroupTag;
            constexpr static SSize magnitudeIndex = 1;
        };
        struct HectogramTag {
            using tag_group = MetricMassGroupTag;
            constexpr static SSize magnitudeIndex = 2;
        };
        struct KilogramTag {
            using tag_group = MetricMassGroupTag;
            constexpr static SSize magnitudeIndex = 3;
        };
    }

    template<SSize Magnitude>
    using MilligramUnit = Unit<impl::MilligramTag, Magnitude>;
    template<SSize Magnitude>
    using CentigramUnit = Unit<impl::CentigramTag, Magnitude>;
    template<SSize Magnitude>
    using DecigramUnit = Unit<impl::DecigramTag, Magnitude>;
    template<SSize Magnitude>
    using GramUnit = Unit<impl::GramTag, Magnitude>;
    template<SSize Magnitude>
    using DecagramUnit = Unit<impl::DecagramTag, Magnitude>;
    template<SSize Magnitude>
    using HectogramUnit = Unit<impl::HectogramTag, Magnitude>;
    template<SSize Magnitude>
    using KilogramUnit = Unit<impl::KilogramTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 0> 
    using Milligram = UnitValue<DataType, MilligramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 0> 
    using Centigram = UnitValue<DataType, CentigramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 0> 
    using Decigram = UnitValue<DataType, DecigramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 0> 
    using Gram = UnitValue<DataType, GramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 0> 
    using Decagram = UnitValue<DataType, DecagramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 0> 
    using Hectogram = UnitValue<DataType, HectogramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 0> 
    using Kilogram = UnitValue<DataType, KilogramUnit<Magnitude>>;


    //Imperial

    //Imperial length
    namespace impl {
        struct ImperialLengthGroupTag {
            using unit_category = LengthUnitCategory;
            using group_type = StrongUnitGroupTypeTable;
            constexpr static Array<SSize, 4> convertionTable = { 1, 12, 12 * 3, 12 * 3 * 1760 };
        };
        struct InchesTag {
            using tag_group = ImperialLengthGroupTag;
            constexpr static Size tableIndex = 0;
        };
        struct FeetTag {
            using tag_group = ImperialLengthGroupTag;
            constexpr static Size tableIndex = 1;
        };
        struct YardsTag {
            using tag_group = ImperialLengthGroupTag;
            constexpr static Size tableIndex = 2;
        };
        struct MilesTag {
            using tag_group = ImperialLengthGroupTag;
            constexpr static Size tableIndex = 3;
        };
    }

    template<SSize Magnitude>
    using InchesUnit = Unit<impl::InchesTag, Magnitude>;
    template<SSize Magnitude>
    using FeetUnit = Unit<impl::FeetTag, Magnitude>;
    template<SSize Magnitude>
    using YardsUnit = Unit<impl::YardsTag, Magnitude>;
    template<SSize Magnitude>
    using MilesUnit = Unit<impl::MilesTag, Magnitude>;


    template<typename DataType, SSize Magnitude = 1>
    using Inches = UnitValue<DataType, InchesUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Feet = UnitValue<DataType, FeetUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Yards = UnitValue<DataType, YardsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Miles = UnitValue<DataType, MilesUnit<Magnitude>>;

    template<typename FloatDataType, typename FromTag, typename ToTag>
    struct UnitCategoryConversionFactor<
        impl::ImperialLengthGroupTag, impl::MetricLengthGroupTag, FromTag, ToTag, FloatDataType> 
        : public impl::UnitCategoryConversionFactorImpl<FloatDataType, 0.0254, impl::InchesTag, impl::MeterTag, FromTag, ToTag> {
    };
    template<typename FloatDataType, typename FromTag, typename ToTag>
    struct UnitCategoryConversionFactor<
        impl::MetricLengthGroupTag, impl::ImperialLengthGroupTag, FromTag, ToTag, FloatDataType>
        : public impl::UnitCategoryConversionFactorImpl<FloatDataType, 39.3701, impl::MeterTag, impl::InchesTag, FromTag, ToTag> {
    };
}

