#pragma once 

//own
#include "unitsBase.h"

//interface
namespace natl {
    enum class MetricLengthType {
        millimeter,
        centimeter,
        decimeter,
        meter,
        decameter,
        hectometer,
        kilometer,
    };

    namespace impl {
        struct MetricLengthTagGroup {
            using unit_category = LengthUnitCategory;
            using group_type = StrongUnitGroupTypeMagnitude;
            constexpr static SSize orderOfMagnitude = 10;
        };

        struct MetricLengthTag {
            using system_generic_category = impl::MetricLengthTagGroup;
            using tag_group = impl::MetricLengthTagGroup;
        };

        struct MillimeterTag {
            using tag_group = MetricLengthTagGroup;
            constexpr static SSize magnitudeIndex = -3;
            constexpr static MetricLengthType unitType = MetricLengthType::millimeter;
            using name = StringLiteral<"millimeter">;
            using abbreviation_name = StringLiteral<"mm">;
        };
        struct CentimeterTag {
            using tag_group = MetricLengthTagGroup;
            constexpr static SSize magnitudeIndex = -2;
            constexpr static MetricLengthType unitType = MetricLengthType::centimeter;
            using name = StringLiteral<"centimeter">;
            using abbreviation_name = StringLiteral<"cm">;
        };
        struct DecimeterTag {
            using tag_group = MetricLengthTagGroup;
            constexpr static SSize magnitudeIndex = -1;
            constexpr static MetricLengthType unitType = MetricLengthType::decimeter;
            using name = StringLiteral<"decimeter">;
            using abbreviation_name = StringLiteral<"dm">;
        };
        struct MeterTag {
            using tag_group = MetricLengthTagGroup;
            constexpr static SSize magnitudeIndex = 0;
            constexpr static MetricLengthType unitType = MetricLengthType::meter;
            using name = StringLiteral<"meter">;
            using abbreviation_name = StringLiteral<"m">;
        };
        struct DecameterTag {
            using tag_group = MetricLengthTagGroup;
            constexpr static SSize magnitudeIndex = 1;
            constexpr static MetricLengthType unitType = MetricLengthType::decameter;
            using name = StringLiteral<"decameter">;
            using abbreviation_name = StringLiteral<"dam">;
        };
        struct HectometerTag {
            using tag_group = MetricLengthTagGroup;
            constexpr static SSize magnitudeIndex = 2;
            constexpr static MetricLengthType unitType = MetricLengthType::hectometer;
            using name = StringLiteral<"hectometer">;
            using abbreviation_name = StringLiteral<"hm">;
        };
        struct KilometerTag {
            using tag_group = MetricLengthTagGroup;
            constexpr static SSize magnitudeIndex = 3;
            constexpr static MetricLengthType unitType = MetricLengthType::kilometer;
            using name = StringLiteral<"kilometer">;
            using abbreviation_name = StringLiteral<"km">;
        };
    }

    template<SSize Magnitude = 1>
    using MetricLengthUnit = Unit<impl::MetricLengthTag, Magnitude>;
    template<SSize Magnitude = 1>
    using MillimeterUnit = Unit<impl::MillimeterTag, Magnitude>;
    template<SSize Magnitude = 1>
    using CentimeterUnit = Unit<impl::CentimeterTag, Magnitude>;
    template<SSize Magnitude = 1>
    using DecimeterUnit = Unit<impl::DecimeterTag, Magnitude>;
    template<SSize Magnitude = 1>
    using MeterUnit = Unit<impl::MeterTag, Magnitude>;
    template<SSize Magnitude = 1>
    using DecameterUnit = Unit<impl::DecameterTag, Magnitude>;
    template<SSize Magnitude = 1>
    using HectometerUnit = Unit<impl::HectometerTag, Magnitude>;
    template<SSize Magnitude = 1>
    using KilometerUnit = Unit<impl::KilometerTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1>
    using MetricLength = BaseUnitValue<DataType, MetricLengthUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Millimeter = BaseUnitValue<DataType, MillimeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Centimeter = BaseUnitValue<DataType, CentimeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Decimeter = BaseUnitValue<DataType, DecimeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Meter = BaseUnitValue<DataType, MeterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Decameter = BaseUnitValue<DataType, DecameterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Hectometer = BaseUnitValue<DataType, HectometerUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Kilometer = BaseUnitValue<DataType, KilometerUnit<Magnitude>>;

    namespace value {
        constexpr inline MetricLength<Size> metricLength = MetricLength<Size>(1);
        constexpr inline Millimeter<Size> millimeter = Millimeter<Size>(1); 
        constexpr inline Centimeter<Size> centimeter = Centimeter<Size>(1); 
        constexpr inline Decimeter<Size> decimeter = Decimeter<Size>(1); 
        constexpr inline Meter<Size> meter = Meter<Size>(1); 
        constexpr inline Decameter<Size> decameter = Decameter<Size>(1); 
        constexpr inline Hectometer<Size> hectometer = Hectometer<Size>(1); 
        constexpr inline Kilometer<Size> kilometer = Kilometer<Size>(1);
    }

    namespace abbr {
        constexpr inline MetricLength<Size> metricLength = MetricLength<Size>(1);
        constexpr inline Millimeter<Size> mm = Millimeter<Size>(1);
        constexpr inline Centimeter<Size> cm = Centimeter<Size>(1);
        constexpr inline Decimeter<Size> dm = Decimeter<Size>(1);
        constexpr inline Meter<Size> m = Meter<Size>(1);
        constexpr inline Decameter<Size> dam = Decameter<Size>(1);
        constexpr inline Hectometer<Size> hm = Hectometer<Size>(1);
        constexpr inline Kilometer<Size> km = Kilometer<Size>(1);
    }

    namespace abbrt {
        using metricLength = MetricLengthUnit<1>;
        using mm = MillimeterUnit<1>;
        using cm = CentimeterUnit<1>;
        using dm = DecimeterUnit<1>;
        using m = MeterUnit<1>;
        using dam = DecameterUnit<1>;
        using hm = HectometerUnit<1>;
        using km = KilometerUnit<1>;
    }

    //mass
    enum class MetricMassType {
        milligram,
        centigram,
        decigram,
        gram,
        decagram,
        hectogram,
        kilogram,
        metricTon,
    };

    namespace impl {
        struct MetricMassTagGroup {
            using unit_category = MassUnitCategory;
            using group_type = StrongUnitGroupTypeMagnitude;
            constexpr static SSize orderOfMagnitude = 10;
        };

        struct MetricMassTag {
            using system_generic_category = impl::MetricMassTagGroup;
            using tag_group = impl::MetricMassTagGroup;
        };

        struct MilligramTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = -3;
            constexpr static MetricMassType unitType = MetricMassType::milligram;
            using name = StringLiteral<"milligram">;
            using abbreviation_name = StringLiteral<"mg">;
        };
        struct CentigramTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = -2;
            constexpr static MetricMassType unitType = MetricMassType::centigram;
            using name = StringLiteral<"centigram">;
            using abbreviation_name = StringLiteral<"cg">;
        };
        struct DecigramTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = -1;
            constexpr static MetricMassType unitType = MetricMassType::decigram;
            using name = StringLiteral<"decigram">;
            using abbreviation_name = StringLiteral<"dg">;
        };
        struct GramTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = 0;
            constexpr static MetricMassType unitType = MetricMassType::gram;
            using name = StringLiteral<"gram">;
            using abbreviation_name = StringLiteral<"g">;
        };
        struct DecagramTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = 1;
            constexpr static MetricMassType unitType = MetricMassType::decagram;
            using name = StringLiteral<"decagram">;
            using abbreviation_name = StringLiteral<"dag">;
        };
        struct HectogramTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = 2;
            constexpr static MetricMassType unitType = MetricMassType::hectogram;
            using name = StringLiteral<"hectogram">;
            using abbreviation_name = StringLiteral<"hg">;
        };
        struct KilogramTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = 3;
            constexpr static MetricMassType unitType = MetricMassType::kilogram;
            using name = StringLiteral<"kilogram">;
            using abbreviation_name = StringLiteral<"kg">;
        };
        struct MetricTonTag {
            using tag_group = MetricMassTagGroup;
            constexpr static SSize magnitudeIndex = 6;
            constexpr static MetricMassType unitType = MetricMassType::metricTon;
            using name = StringLiteral<"metric_ton">;
            using abbreviation_name = StringLiteral<"mt">;
        };
    }

    template<SSize Magnitude = 1>
    using MetricMassUnit = Unit<impl::MetricMassTag, Magnitude>;
    template<SSize Magnitude = 1>
    using MilligramUnit = Unit<impl::MilligramTag, Magnitude>;
    template<SSize Magnitude = 1>
    using CentigramUnit = Unit<impl::CentigramTag, Magnitude>;
    template<SSize Magnitude = 1>
    using DecigramUnit = Unit<impl::DecigramTag, Magnitude>;
    template<SSize Magnitude = 1>
    using GramUnit = Unit<impl::GramTag, Magnitude>;
    template<SSize Magnitude = 1>
    using DecagramUnit = Unit<impl::DecagramTag, Magnitude>;
    template<SSize Magnitude = 1>
    using HectogramUnit = Unit<impl::HectogramTag, Magnitude>;
    template<SSize Magnitude = 1>
    using KilogramUnit = Unit<impl::KilogramTag, Magnitude>;
    template<SSize Magnitude = 1>
    using MetricTonUnit = Unit<impl::MetricTonTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1>
    using MetricMass = BaseUnitValue<DataType, MetricMassUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Milligram = BaseUnitValue<DataType, MilligramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Centigram = BaseUnitValue<DataType, CentigramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Decigram = BaseUnitValue<DataType, DecigramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Gram = BaseUnitValue<DataType, GramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Decagram = BaseUnitValue<DataType, DecagramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Hectogram = BaseUnitValue<DataType, HectogramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Kilogram = BaseUnitValue<DataType, KilogramUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using MetricTon = BaseUnitValue<DataType, MetricTonUnit<Magnitude>>;

    namespace value {
        constexpr inline MetricMass<Size> metricMass = MetricMass<Size>(1);
        constexpr inline Milligram<Size> milligram = Milligram<Size>(1);
        constexpr inline Centigram<Size> centigram = Centigram<Size>(1);
        constexpr inline Decigram<Size> decigram = Decigram<Size>(1);
        constexpr inline Gram<Size> gram = Gram<Size>(1);
        constexpr inline Decagram<Size> decagram = Decagram<Size>(1);
        constexpr inline Hectogram<Size> hectogram = Hectogram<Size>(1);
        constexpr inline Kilogram<Size> kilogram = Kilogram<Size>(1);
        constexpr inline MetricTon<Size> metricTon = MetricTon<Size>(1);
    }

    namespace abbr {
        constexpr inline MetricMass<Size> metricMass = MetricMass<Size>(1);
        constexpr inline Milligram<Size> mg = Milligram<Size>(1);
        constexpr inline Centigram<Size> cg = Centigram<Size>(1);
        constexpr inline Decigram<Size> dg = Decigram<Size>(1);
        constexpr inline Gram<Size> g = Gram<Size>(1);
        constexpr inline Decagram<Size> dag = Decagram<Size>(1);
        constexpr inline Hectogram<Size> hg = Hectogram<Size>(1);
        constexpr inline Kilogram<Size> kg = Kilogram<Size>(1);
        constexpr inline MetricTon<Size> mt = MetricTon<Size>(1);
    }

    namespace abbrt {
        using metricMass = MetricMassUnit<1>;
        using mg = MilligramUnit<1>;
        using cg = CentigramUnit<1>;
        using dg = DecigramUnit<1>;
        using g = GramUnit<1>;
        using dag = DecagramUnit<1>;
        using hg = HectogramUnit<1>;
        using kg = KilogramUnit<1>;
        using mt = MetricTonUnit<1>;
    }


}