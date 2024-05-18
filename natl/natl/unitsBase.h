#pragma once

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "typePack.h"
#include "compilerDependent.h"
#include "array.h"
#include "stringLiteral.h"
#include "format.h"

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
    struct StrongUnitGroupTypeUnitWeights {};
    struct StrongUnitGroupTypeNone {};

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
    concept DoesNotUnitConvertDown = FromTag::sagnitudeIndex > ToTag::magnitudeIndex;

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
            && IsSame<typename FromTag::tag_group::group_type, StrongUnitGroupTypeUnitWeights>)
        struct StrongUnitConversionFactor<FromTag, ToTag, FromDataType, ToDataType> {

        static_assert(!(!IsBuiltInFloatingPoint<ToDataType> && (FromTag::unitWeight < ToTag::unitWeight)), "converting down requires a base float type");
        using tag_group = typename FromTag::tag_group;
        constexpr static ToDataType value = static_cast<ToDataType>(FromTag::unitWeight) / static_cast<ToDataType>(ToTag::unitWeight);
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

        using squared = Unit<UnitTag, Magnitude + Magnitude>;
        using cubed = Unit<UnitTag, Magnitude + Magnitude + Magnitude>;
        using quaded = Unit<UnitTag, Magnitude + Magnitude + Magnitude + Magnitude>;

        using negSquared = Unit<UnitTag, -(Magnitude + Magnitude)>;
        using negCubed = Unit<UnitTag, -(Magnitude + Magnitude + Magnitude)>;
        using negQuaded = Unit<UnitTag, -(Magnitude + Magnitude + Magnitude + Magnitude)>;
    };

    namespace impl {
        template<typename UnitType>
        struct IsUnitV : FalseType {};

        template<typename UnitTag, SSize Magnitude>
        struct IsUnitV<Unit<UnitTag, Magnitude>> : TrueType {};
    }

    template<typename Type>
    constexpr inline Bool IsUnit = impl::IsUnitV<Type>::value;
    template<typename Type>
    concept IsUnitC = IsUnit<Type>;

    template<typename... Units>
    struct BaseUnitTypePack {
        using type_pack = TypePack<Units...>;
    };

    template<typename DataType, typename... Units>
    struct BaseUnitValue;

    namespace impl {
        template<typename UnitValueType>
        struct IsUnitValueV : FalseType {};

        template<typename DataType, typename... UnitTypes>
        struct IsUnitValueV<BaseUnitValue<DataType, UnitTypes...>> : TrueType {};
    }

    template<typename Type>
    constexpr inline Bool IsUnitValue = impl::IsUnitValueV<Type>::value;

    namespace impl {
        template<typename UnitOrUnitValueType>
        struct IsUnitOrUnitValueV : FalseType {};

        template<typename UnitTag, SSize Magnitude>
        struct IsUnitOrUnitValueV<Unit<UnitTag, Magnitude>> : TrueType {};

        template<typename DataType, typename... UnitTypes>
        struct IsUnitOrUnitValueV<BaseUnitValue<DataType, UnitTypes...>> : TrueType {};
    }

    template<typename UnitOrUnitValueType>
    constexpr inline Bool IsUnitOrUnitValue = impl::IsUnitOrUnitValueV<UnitOrUnitValueType>::value;

    namespace impl {
        template<typename Unit>
        struct ReduceToUnitTypePackT {};

        template<typename UnitTag, SSize Magnitude>
        struct ReduceToUnitTypePackT<Unit<UnitTag, Magnitude>> {
            using type = TypePack<Unit<UnitTag, Magnitude>>;
        };
        template<typename ReduceDataType, typename... ReduceUnits>
        struct ReduceToUnitTypePackT<BaseUnitValue<ReduceDataType, ReduceUnits...>> {
            using type = TypePack<ReduceUnits...>;
        };

        template<typename Type>
        using ReduceToUnitTypePack = ReduceToUnitTypePackT<Type>::type;
    }

    template<typename... UnitAndUnitValueBlendTypes>
    using ReduceToUnits = TypePackMerge<impl::ReduceToUnitTypePack<UnitAndUnitValueBlendTypes>...>;
    
    template<typename DataType, typename... Units>
    struct BaseUnitValue {
        using value_type = DataType;
        using units_type = TypePack<Units...>;
        using unit_specific_type_pack = BaseUnitTypePack<Units...>;

        using squared = BaseUnitValue<DataType, Unit<typename Units::unit_tag, Units::magnitude + Units::magnitude>...>;
        using cubed = BaseUnitValue<DataType, Unit<typename Units::unit_tag, Units::magnitude + Units::magnitude + Units::magnitude>...>;
        using quaded = BaseUnitValue<DataType, Unit<typename Units::unit_tag, Units::magnitude + Units::magnitude + Units::magnitude + Units::magnitude>...>;

        using negSquared = BaseUnitValue<DataType, Unit<typename Units::unit_tag, -(Units::magnitude + Units::magnitude)>...>;
        using negCubed = BaseUnitValue<DataType, Unit<typename Units::unit_tag, -(Units::magnitude + Units::magnitude + Units::magnitude)>...>;
        using negQuaded = BaseUnitValue<DataType, Unit<typename Units::unit_tag, -(Units::magnitude + Units::magnitude + Units::magnitude + Units::magnitude)>...>;
    public:
        DataType internalData;
    public:
        //constructor
        NATL_FORCE_INLINE constexpr BaseUnitValue() noexcept = default;
        NATL_FORCE_INLINE constexpr explicit BaseUnitValue(const DataType dataIn) noexcept : internalData(dataIn) {}

        //destructor
        NATL_FORCE_INLINE constexpr ~BaseUnitValue() noexcept = default;

        //util
        NATL_FORCE_INLINE constexpr BaseUnitValue& self() noexcept { return *this; }
        NATL_FORCE_INLINE constexpr const BaseUnitValue& self() const noexcept { return *this; }

        //assignment 
        NATL_FORCE_INLINE constexpr BaseUnitValue& operator=(const BaseUnitValue&) noexcept = default;

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
        NATL_FORCE_INLINE constexpr BaseUnitValue<Interger, Units...> asInt() const noexcept requires(IsBuiltInInterger<DataType>) {
            return BaseUnitValue<Interger, Units...>(static_cast<Interger>(internalData));
        }
        template<typename Float>
            requires(IsBuiltInFloatingPoint<Float>)
        NATL_FORCE_INLINE constexpr BaseUnitValue<Float, Units...> asFloat() const noexcept requires(IsBuiltInInterger<DataType>) {
            return BaseUnitValue<Float, Units...>(static_cast<Float>(internalData));
        }



        //operator 
        NATL_FORCE_INLINE constexpr BaseUnitValue operator+(const BaseUnitValue& other) const noexcept { return BaseUnitValue(internalData + other.internalData); }
        NATL_FORCE_INLINE constexpr BaseUnitValue operator-(const BaseUnitValue& other) const noexcept { return BaseUnitValue(internalData - other.internalData); }

        //operators 
        NATL_FORCE_INLINE constexpr BaseUnitValue& operator+=(const BaseUnitValue& other) noexcept {
            internalData += other.internalData;
            return *this;
        }
        NATL_FORCE_INLINE constexpr BaseUnitValue& operator-=(const BaseUnitValue& other) noexcept {
            internalData -= other.internalData;
            return *this;
        }

        //compare 
        NATL_FORCE_INLINE constexpr Bool operator==(const BaseUnitValue& other) const noexcept { return internalData == other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator!=(const BaseUnitValue& other) const noexcept { return internalData != other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator<(const BaseUnitValue& other) const noexcept { return internalData < other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator>(const BaseUnitValue& other) const noexcept { return internalData > other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator<=(const BaseUnitValue& other) const noexcept { return internalData <= other.internalData; }
        NATL_FORCE_INLINE constexpr Bool operator>=(const BaseUnitValue& other) const noexcept { return internalData >= other.internalData; }
        NATL_FORCE_INLINE constexpr auto operator<=>(const BaseUnitValue& other) const noexcept { return internalData <=> other.internalData; }

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

        template<typename LhsUnit, typename RhsUnit>
        struct UnitGroupUnitCategoryCompare {
            constexpr static Bool value =  
                IsSame<typename LhsUnit::unit_tag::tag_group::unit_category, typename RhsUnit::unit_tag::tag_group::unit_category>;
        };

        template<typename TransfromUnit, typename UnitTypePack>
        struct UnitConvertTransformUnitCategoryTransform {
            constexpr static Size index = TypePackFindIndexOfTypeCompareValue<UnitGroupUnitCategoryCompare, TransfromUnit, UnitTypePack>;
            static_assert(!(index == IndexNotFound::value), "natl: Unit convert - unsupport conversion - could not find matching tag_group or unit_category");
            using NewUnit = TypePackNthElement<index, UnitTypePack>;
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
                    using covert_type = typename UnitConvertTransformUnitCategoryTransform<TransfromUnit, UnitTypePack>::type;
                    return covert_type();
                }
            }
            using new_unit = decltype(chooseConvertTypeFunc());
            static_assert(new_unit::magnitude == TransfromUnit::magnitude, "natl: Unit convert - unsupport conversion - unit magnitudes are different");
            using type = new_unit;
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
            using type = BaseUnitValue<DataType, NewUnits...>;
        };

        //unit operation 
        consteval unit_specific_type_pack units() const noexcept { return unit_specific_type_pack{}; }

        template<typename... OtherUnits>
        NATL_FORCE_INLINE constexpr auto operator*(const BaseUnitValue<DataType, OtherUnits...>& other) const noexcept {
            using transformed_units_type = TypePackTransformWithTypePackArgs<AddTransform, TypePack<OtherUnits...>, TypePack<Units...>>;
            using new_unit_type_pack = TypePackAddUnique<UnitCompare, transformed_units_type, OtherUnits...>;
            using unit_value_arg_type_pack = typename new_unit_type_pack::template add_new_elements_front<DataType>;
            using new_created_unit_value_type = InstantiateWithTypePack<BaseUnitValue, unit_value_arg_type_pack>;
            using new_unit_value_type = new_created_unit_value_type;
            return new_unit_value_type(internalData * other.internalData);
        }

        template<typename... OtherUnits>
        NATL_FORCE_INLINE constexpr auto operator/(const BaseUnitValue<DataType, OtherUnits...>& other) const noexcept {
            using transformed_units_type = TypePackTransformWithTypePackArgs<SubTransform, TypePack<OtherUnits...>, TypePack<Units...>>;
            using new_unit_type_pack = TypePackRemoveElementsIf<UnitRemoveIfZero, transformed_units_type>;
            using unit_value_arg_type_pack = typename new_unit_type_pack::template add_new_elements_front<DataType>;
            using new_created_unit_value_type = InstantiateWithTypePack<BaseUnitValue, unit_value_arg_type_pack>;
            using new_unit_value_type = new_created_unit_value_type;
            return new_unit_value_type(internalData / other.internalData);
        }

        template<typename... OtherUnitsOrUnitValues>
        NATL_FORCE_INLINE constexpr auto convertTo() const noexcept {
            using units_type_pack = TypePack<Units...>;
            using reduced_other_units_type_pack = ReduceToUnits<Decay<OtherUnitsOrUnitValues>...>;
            using transformed_units_type = TypePackTransformWithTypePackArgs<UnitConvertTransform, reduced_other_units_type_pack, units_type_pack>;
            constexpr DataType convertValue = TypePackOpFoldWithIndexAndArgValue<DataType, transformed_units_type,
                UnitConvertFactorValuePredicate, UnitConvertFactorMultiplyOp, units_type_pack>;
            using new_unit_value_type = InstantiateWithTypePack<ConvertNewUnitType, transformed_units_type>::type;
            const DataType newValue = convertValue * internalData;
            return new_unit_value_type(newValue);
        }
    };

    namespace impl {
        template<typename LhsUnit, typename RhsUnit>
        struct CompareUnitCategoryLessThan {
            constexpr static Bool value = 
                LhsUnit::unit_tag::tag_group::unit_category::value
                > RhsUnit::unit_tag::tag_group::unit_category::value;
        };

        template<typename TypePackArg>
        using SortUnits = TypePackSortCompare<CompareUnitCategoryLessThan, TypePackArg>;
    }

    template<typename DataType, typename... UnitsOrUnitValues>
    using UnitValue = InstantiateWithTypePack<BaseUnitValue, 
        TypePackMergeBlend<
            DataType, 
            impl::SortUnits<ReduceToUnits<UnitsOrUnitValues...>>
        >
    >;
    
    struct GenericUnitTagGroup {};

    namespace impl {

        template<typename TestUnitType>
        concept HasSystemGenericCatagory = requires {
            typename TestUnitType::unit_tag::system_generic_category;
        };


        template<typename TestUnitType, typename UnitType>
        struct TestUnitCompatibleV {

            consteval static Bool testUnitCompatibility() noexcept {
                if constexpr (IsSame<typename TestUnitType::unit_tag::tag_group, GenericUnitTagGroup>) {
                    //TestUnitType is generic unit 
                    return IsSame<
                        typename TestUnitType::unit_tag::generic_category,
                        typename UnitType::unit_tag::tag_group::unit_category>;
                } else if constexpr (HasSystemGenericCatagory<TestUnitType>) {
                    //TestUnitType is a generic unit of unit system 
                    return IsSame<
                        typename TestUnitType::unit_tag::system_generic_category,
                        typename UnitType::unit_tag::tag_group>;
                } else {
                    //TestUnitType is a specific unit
                    return IsSame<
                        typename TestUnitType::unit_tag,
                        typename UnitType::unit_tag>;
                }
            }

            consteval static Bool testMagnitudeCompatibility() noexcept {
                return UnitType::magnitude == TestUnitType::magnitude;
            }

            constexpr static Bool value = testMagnitudeCompatibility() && testUnitCompatibility();
        };

        template<typename TestUnitType, typename UnitType>
        constexpr inline Bool TestUnitCompatible = TestUnitCompatibleV<TestUnitType, UnitType>::value;

        template<typename...>
        struct IsQuantityOfV;

        template<typename... TypePackUnits, typename... TestUnits>
            requires(sizeof...(TypePackUnits) == sizeof...(TestUnits))
        struct IsQuantityOfV<TypePack<TypePackUnits...>, TypePack<TestUnits...>> {
            constexpr static Bool value = (TestUnitCompatible<TestUnits, TypePackUnits> && ...);
        };

        template<typename UnitTag, SSize Magnitude, typename TestUnit>
        struct IsQuantityOfV<Unit<UnitTag, Magnitude>, TypePack<TestUnit>> {
            constexpr static Bool value = TestUnitCompatible<TestUnit, Unit<UnitTag, Magnitude>>;
        };

        template<typename DataType, typename... UnitTypes, typename... TestUnits>
            requires(sizeof...(UnitTypes) == sizeof...(TestUnits))
        struct IsQuantityOfV<BaseUnitValue<DataType, UnitTypes...>, TypePack<TestUnits...>> {
            constexpr static Bool value = (TestUnitCompatible<TestUnits, UnitTypes> && ...);
        };
    }

    template<typename QuantityType, typename... UnitsOrUnitValues>
    concept IsQuantityOf = (IsUnitOrUnitValue<UnitsOrUnitValues> && ... ) && 
        impl::IsQuantityOfV<QuantityType, impl::SortUnits<ReduceToUnits<UnitsOrUnitValues...>>>::value;


    namespace impl {
        template<typename UnitsOrUnitValue>
        struct SquaredImplT;

        template<typename UnitTag, SSize Magnitude>
        struct SquaredImplT<Unit<UnitTag, Magnitude>> {
            using type = Unit<UnitTag, Magnitude + Magnitude>;
        };
        template<typename DataType, typename... Units>
        struct SquaredImplT<BaseUnitValue<DataType, Units...>> {
            using type = BaseUnitValue<DataType, typename SquaredImplT<Units>::type...>;
        };
    }

    template<typename UnitsOrUnitValue>
    using Squared = impl::SquaredImplT<UnitsOrUnitValue>::type;

    namespace impl {
        template<typename UnitsOrUnitValue>
        struct CubedImplT;

        template<typename UnitTag, SSize Magnitude>
        struct CubedImplT<Unit<UnitTag, Magnitude>> {
            using type = Unit<UnitTag, Magnitude + Magnitude + Magnitude>;
        };
        template<typename DataType, typename... Units>
        struct CubedImplT<BaseUnitValue<DataType, Units...>> {
            using type = BaseUnitValue<DataType, typename CubedImplT<Units>::type...>;
        };
    }

    template<typename UnitsOrUnitValue>
    using Cubed = impl::CubedImplT<UnitsOrUnitValue>::type;

    namespace impl {
        template<typename UnitsOrUnitValue>
        struct QuadedImplT;

        template<typename UnitTag, SSize Magnitude>
        struct QuadedImplT<Unit<UnitTag, Magnitude>> {
            using type = Unit<UnitTag, Magnitude + Magnitude + Magnitude + Magnitude>;
        };
        template<typename DataType, typename... Units>
        struct QuadedImplT<BaseUnitValue<DataType, Units...>> {
            using type = BaseUnitValue<DataType, typename QuadedImplT<Units>::type...>;
        };
    }

    template<typename UnitsOrUnitValue>
    using Quaded = impl::QuadedImplT<UnitsOrUnitValue>::type;

    //unit dimensions 
    struct LengthUnitCategory {
        constexpr static Size value = 0;
        using name = StringLiteral<"length">;
    };
    struct MassUnitCategory {
        constexpr static Size value = 1;
        using name = StringLiteral<"mass">;
    };
    struct TimeUnitCategory {
        constexpr static Size value = 2;
        using name = StringLiteral<"time">;
    };
    struct TemperatureUnitCategory {
        constexpr static Size value = 3;
        using name = StringLiteral<"temperature">;
    };
    struct DataUnitCategory {
        constexpr static Size value = 4;
        using name = StringLiteral<"data">;
    };


    namespace impl {
        struct LengthTag {
            using tag_group = GenericUnitTagGroup;
            using generic_category = LengthUnitCategory;
            using name = generic_category::name;
            using abbreviation_name = name;
        };
        struct MassTag {
            using tag_group = GenericUnitTagGroup;
            using generic_category = MassUnitCategory;
            using name = generic_category::name;
            using abbreviation_name = name;
        };
        struct TimeTag {
            using tag_group = GenericUnitTagGroup;
            using generic_category = TimeUnitCategory;
            using name = generic_category::name;
            using abbreviation_name = name;
        };
    }

    template<SSize Magnitude = 1>
    using LengthUnit = Unit<impl::LengthTag, Magnitude>;
    template<SSize Magnitude = 1>
    using MassUnit = Unit<impl::MassTag, Magnitude>;
    template<SSize Magnitude = 1>
    using TimeUnit = Unit<impl::TimeTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1>
    using Length = BaseUnitValue<DataType, LengthUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Mass = BaseUnitValue<DataType, MassUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Time = BaseUnitValue<DataType, TimeUnit<Magnitude>>;

    namespace value {
        constexpr inline Length<Size> length = Length<Size>(1);
        constexpr inline Mass<Size> mass = Mass<Size>(1);
        constexpr inline Time<Size> time = Time<Size>(1);
    }

    namespace abbr {
        constexpr inline Length<Size> length = Length<Size>(1);
        constexpr inline Mass<Size> mass = Mass<Size>(1);
        constexpr inline Time<Size> time = Time<Size>(1);
    }

    namespace abbrt {
        using length = LengthUnit<1>;
        using mass = MassUnit<1>;
        using time = TimeUnit<1>;
    }

    enum class FormatUnit {
        abbreviated,
        fullname, 
        dimension,
        system,
        standard = abbreviated,
    };

    class FormatUnitAbbreviated {};
    class FormatUnitFullname {};
    class FormatUnitDimension {};
    class FormatUnitSystem {};
    class FormatUnitStandard {};

    using FormatUnitAbbr = FormatUnitAbbreviated;

    namespace impl {
        template<typename TemplateFlag>
        constexpr void unitHandelTemplateFlag(FormatUnit& formatUnit) noexcept {
            if constexpr (IsStringLiteralV<TemplateFlag>) {
                constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
                if constexpr (tflagName == "fullname") {
                    formatUnit = FormatUnit::fullname;
                } else if constexpr (tflagName == "abbreviated" || tflagName == "abbr") {
                    formatUnit = FormatUnit::abbreviated;
                } else if constexpr (tflagName == "dimension" || tflagName == "d") {
                    formatUnit = FormatUnit::dimension;
                } else if constexpr (tflagName == "system" || tflagName == "s") {
                    formatUnit = FormatUnit::system;
                }
            } else {
                if constexpr (IsSame<TemplateFlag, FormatUnitFullname>) {
                    formatUnit = FormatUnit::fullname;
                } else if constexpr (IsSame<TemplateFlag, FormatUnitAbbreviated>) {
                    formatUnit = FormatUnit::abbreviated;
                } else if constexpr (IsSame<TemplateFlag, FormatUnitDimension>) {
                    formatUnit = FormatUnit::dimension;
                } else if constexpr (IsSame<TemplateFlag, FormatUnitSystem>) {
                    formatUnit = FormatUnit::system;
                } else if constexpr (IsSame<TemplateFlag, FormatUnitStandard>) {
                    formatUnit = FormatUnit::standard;
                }
            }
        }

        template<typename TemplateFlag>
        struct IsUnitTemplateFlagV {
        public:
            consteval static Bool testFlag() noexcept {
                if constexpr (IsStringLiteralV<TemplateFlag>) {
                    constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
                    if constexpr (tflagName == "fullname") {
                        return true;
                    } else if constexpr (tflagName == "abbreviated" || tflagName == "abbr") {
                        return true;
                    } else if constexpr (tflagName == "dimension" || tflagName == "d") {
                        return true;
                    } else if constexpr (tflagName == "system" || tflagName == "s") {
                        return true;
                    }
                } else {
                    if constexpr (IsSame<TemplateFlag, FormatUnitFullname>) {
                        return true;
                    } else if constexpr (IsSame<TemplateFlag, FormatUnitAbbreviated>) {
                        return true;
                    } else if constexpr (IsSame<TemplateFlag, FormatUnitDimension>) {
                        return true;
                    } else if constexpr (IsSame<TemplateFlag, FormatUnitSystem>) {
                        return true;
                    } else if constexpr (IsSame<TemplateFlag, FormatUnitStandard>) {
                        return true;
                    }
                }
                return false;
            }

            constexpr static Bool value = testFlag();
        };
    }

    template<typename UnitTag, SSize Magnitude, typename CharType>
    struct Formatter<Unit<UnitTag, Magnitude>, CharType> {
    public:
        using value_type = Unit<UnitTag, Magnitude>;

        template<typename OutputIter>
        constexpr static void formatMagnitude(OutputIter& outputIter) noexcept {
            outputIter = '^';
            Formatter<SSize, Ascii>::format<OutputIter>(outputIter, Magnitude);
        }

        template<typename OutputIter>
        constexpr static void formatAbbreviated(OutputIter& outputIter) noexcept {
            Formatter<ConstAsciiStringView, Ascii>::format<OutputIter>(outputIter, UnitTag::abbreviation_name::toStringView());
            formatMagnitude<OutputIter>(outputIter);
        }
        
        template<typename OutputIter>
        constexpr static void formatFullname(OutputIter& outputIter) noexcept {
            Formatter<ConstAsciiStringView, Ascii>::format<OutputIter>(outputIter, UnitTag::name::toStringView());
            formatMagnitude<OutputIter>(outputIter);
        }

        template<typename OutputIter>
        constexpr static void formatDimension(OutputIter& outputIter) noexcept {
            if constexpr (IsSame<typename UnitTag::tag_group, GenericUnitTagGroup>) {
                Formatter<ConstAsciiStringView, Ascii>::format<OutputIter>(outputIter, UnitTag::name::toStringView());
            } else {
                Formatter<ConstAsciiStringView, Ascii>::format<OutputIter>(outputIter, UnitTag::tag_group::unit_category::name::toStringView());
            }
            formatMagnitude<OutputIter>(outputIter);
        }

        template<typename OutputIter>
        constexpr static void formatSystem(OutputIter& outputIter) noexcept {
            if constexpr (IsSame<typename UnitTag::tag_group, GenericUnitTagGroup>) {
                Formatter<ConstAsciiStringView, Ascii>::format<OutputIter>(outputIter, UnitTag::name::toStringView());
            } else {
                Formatter<ConstAsciiStringView, Ascii>::format<OutputIter>(outputIter, UnitTag::tag_group::name::toStringView());
            }
            formatMagnitude<OutputIter>(outputIter);
        }

        template<typename OutputIter>
        constexpr static void universalFormat(OutputIter& outputIter, const FormatUnit formatUnit) noexcept {
            switch (formatUnit) {
            case FormatUnit::abbreviated:
                formatAbbreviated<OutputIter>(outputIter);
                break;
            case FormatUnit::fullname:
                formatFullname<OutputIter>(outputIter);
                break;
            case FormatUnit::dimension:
                formatDimension<OutputIter>(outputIter);
                break;
            case FormatUnit::system:
                formatSystem<OutputIter>(outputIter);
                break;
            default:
                unreachable();
            }
        }

        template<typename... TemplateFlags>
        class WithTemplateFlags {
        public:

            template<typename OutputIter>
            constexpr static OutputIter format(OutputIter outputIter, value_type, const FormatUnit formatUnit = FormatUnit::standard) noexcept {
                (impl::unitHandelTemplateFlag<TemplateFlags>(formatUnit), ...);
                universalFormat<OutputIter>(outputIter, formatUnit);
                return outputIter;
            }
        };

        template<typename OutputIter>
        constexpr static OutputIter format(OutputIter outputIter, value_type, const FormatUnit formatUnit = FormatUnit::standard) noexcept {
            universalFormat<OutputIter>(outputIter, formatUnit);
            return outputIter;
        }
    };

    template<typename CharType, typename... UnitTypes> 
    struct Formatter<BaseUnitTypePack<UnitTypes...>, CharType> {
        using value_type = BaseUnitTypePack<UnitTypes...>;

        template<typename OutputIter, typename UnitType, Size Index>
        constexpr static void baseFormatUnit(OutputIter& outputIter, const FormatUnit formatUnit) noexcept {
            if constexpr (Index != 0) {
                outputIter = ' ';
            }
            outputIter = Formatter<UnitType, Ascii>::template format<OutputIter>(outputIter, UnitType{}, formatUnit);
        }

        template<typename OutputIter, Size... Indices>
        constexpr static void formatUnits(OutputIter& outputIter, const FormatUnit formatUnit, IndexSequence<Indices...>) noexcept {
            (baseFormatUnit<OutputIter, UnitTypes, Indices>(outputIter, formatUnit), ...);
        }

        template<typename OutputIter>
        constexpr static void universalFormat(OutputIter& outputIter, const FormatUnit formatUnit) noexcept {
            formatUnits<OutputIter>(outputIter, formatUnit, MakeIndexSequence<sizeof...(UnitTypes)>{});
        }

        template<typename... TemplateFlags>
        class WithTemplateFlags {
        public:
            template<typename OutputIter>
            constexpr static OutputIter format(OutputIter outputIter, value_type, FormatUnit formatUnit = FormatUnit::standard) noexcept {
                (impl::unitHandelTemplateFlag<TemplateFlags>(formatUnit), ...);
                universalFormat<OutputIter>(outputIter, formatUnit);
                return outputIter;
            }
        };

        template<typename OutputIter>
        constexpr static OutputIter format(OutputIter outputIter, value_type, FormatUnit formatUnit = FormatUnit::standard) noexcept {
            universalFormat<OutputIter>(outputIter, formatUnit);
            return outputIter;
        }
    };

    
    template<typename CharType, typename DataType, typename... UnitTypes>
        requires(Formattable<DataType, CharType>)
    struct Formatter<BaseUnitValue<DataType, UnitTypes...>, CharType> {
        using value_type = BaseUnitValue<DataType, UnitTypes...>;

        template<typename OutputIter, typename UnitType>
        constexpr static void baseFormatUnit(OutputIter& outputIter, const FormatUnit formatUnit) noexcept {
            outputIter = ' ';
            outputIter = Formatter<UnitType, Ascii>::format(outputIter, UnitType{}, formatUnit);
        }

        template<typename OutputIter>
        constexpr static void formatUnits(OutputIter& outputIter, const FormatUnit formatUnit) noexcept {
            (baseFormatUnit<OutputIter, UnitTypes>(outputIter, formatUnit), ...);
        }

        template<typename OutputIter, typename... FormatArgTypes>
        constexpr static void universalFormat(
            OutputIter& outputIter, 
            const value_type unitValue,
            const FormatUnit formatUnit,
            FormatArgTypes&&... formatArgs) noexcept {
            outputIter = Formatter<DataType>::format(outputIter, unitValue.value(), natl::forward<FormatArgTypes>(formatArgs)...);
            formatUnits<OutputIter>(outputIter, formatUnit);
        }

        template<typename OutputIter, typename... FormatArgTypes>
        constexpr static void universalFormat(
            OutputIter& outputIter,
            const value_type unitValue,
            FormatArgTypes&&... formatArgs) noexcept {
            outputIter = Formatter<DataType>::format(outputIter, unitValue.value(), natl::forward<FormatArgTypes>(formatArgs)...);
            formatUnits<OutputIter>(outputIter, FormatUnit::standard);
        }

        template<typename... TemplateFlags>
        class WithTemplateFlags {
        public:

            using FilteredTemplateFlags = TypePackRemoveElementsIf<impl::IsUnitTemplateFlagV, TypePack<TemplateFlags...>>;

            template<typename OutputIter, typename... FormatArgTypes>
            constexpr static void universalFormat(
                OutputIter& outputIter,
                const value_type unitValue,
                FormatUnit, //templateFormatUnit
                const FormatUnit formatUnitArg,
                FormatArgTypes&&... formatArgs) noexcept {
                if (FilteredTemplateFlags::size > 0) {
                    using date_type_formatter = InstantiateWithTypePack<Formatter<DataType>::template WithTemplateFlags, FilteredTemplateFlags>;
                    outputIter = date_type_formatter::format(outputIter, unitValue.value(), natl::forward<FormatArgTypes>(formatArgs)...);
                } else {
                    outputIter = Formatter<DataType>::format(outputIter, unitValue.value(), natl::forward<FormatArgTypes>(formatArgs)...);
                }
                formatUnits<OutputIter>(outputIter, formatUnitArg);
            }

            template<typename OutputIter, typename... FormatArgTypes>
            constexpr static void universalFormat(
                OutputIter& outputIter,
                const value_type unitValue,
                const FormatUnit templateFormatUnit,
                FormatArgTypes&&... formatArgs) noexcept {
                if constexpr (FilteredTemplateFlags::size > 0) {
                    using date_type_formatter = InstantiateWithTypePack<Formatter<DataType>::template WithTemplateFlags, FilteredTemplateFlags>;
                    outputIter = date_type_formatter::format(outputIter, unitValue.value(), natl::forward<FormatArgTypes>(formatArgs)...);
                } else {
                    outputIter = Formatter<DataType>::format(outputIter, unitValue.value(), natl::forward<FormatArgTypes>(formatArgs)...);
                }
                formatUnits<OutputIter>(outputIter, templateFormatUnit);
            }

        public:
            template<typename OutputIter, typename... FormatArgTypes>
            constexpr static OutputIter format(
                OutputIter outputIter,
                const value_type unitValue,
                FormatArgTypes&&... formatArgs) noexcept {
                FormatUnit formatUnit = FormatUnit::standard;
                (impl::unitHandelTemplateFlag<TemplateFlags>(formatUnit), ...);
                universalFormat(outputIter, unitValue, formatUnit, natl::forward<FormatArgTypes>(formatArgs)...);
                return outputIter;
            }
        };

        template<typename OutputIter, typename... FormatArgTypes>
        constexpr static OutputIter format(
            OutputIter outputIter, 
            const value_type unitValue,
            FormatArgTypes&&... formatArgs) noexcept {
            universalFormat(outputIter,  unitValue, natl::forward<FormatArgTypes>(formatArgs)...);
            return outputIter;
        }
    };

}

