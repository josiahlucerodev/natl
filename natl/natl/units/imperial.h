#pragma once 

//own
#include "base.h"

//interface
namespace natl {
    //length
    enum struct ImperialLengthType {
        inch,
        hand,
        foot,
        yard,
        chain,
        furlong,
        mile,
        league,
    };

    namespace impl {
        struct ImperialLengthTagGroup {
            using unit_category = LengthUnitCategory;
            using group_type = StrongUnitGroupTypeUnitWeights;
        };

        struct ImperialLengthTag {
            using system_generic_category = impl::ImperialLengthTagGroup;
            using tag_group = impl::ImperialLengthTagGroup;
        };

        enum struct ImperialLengthUnitWeights : i64 {
            inch = 1,
            hand = inch * 4,
            foot = inch * 12,
            yard = inch * 3,
            chain = yard * 22,
            furlong = chain * 10,
            mile = yard * 1760,
            league = mile * 3,
        };

        struct InchTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::inch);
            constexpr static ImperialLengthType unitType = ImperialLengthType::inch;
            using name = StringLiteral<"inch">;
            using abbreviation_name = StringLiteral<"in">;
        };
        struct HandTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::hand);
            constexpr static ImperialLengthType unitType = ImperialLengthType::hand;
            using name = StringLiteral<"hand">;
            using abbreviation_name = StringLiteral<"hh">;
        };
        struct FootTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::foot);
            constexpr static ImperialLengthType unitType = ImperialLengthType::foot;
            using name = StringLiteral<"foot">;
            using abbreviation_name = StringLiteral<"ft">;
        };
        struct YardTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::yard);
            constexpr static ImperialLengthType unitType = ImperialLengthType::yard;
            using name = StringLiteral<"yard">;
            using abbreviation_name = StringLiteral<"yd">;
        };
        struct ChainTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::chain);
            constexpr static ImperialLengthType unitType = ImperialLengthType::chain;
            using name = StringLiteral<"chain">;
            using abbreviation_name = StringLiteral<"ch">;
        };
        struct FurlongTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::furlong);
            constexpr static ImperialLengthType unitType = ImperialLengthType::furlong;
            using name = StringLiteral<"furlong">;
            using abbreviation_name = StringLiteral<"fur">;
        };
        struct MileTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::mile);
            constexpr static ImperialLengthType unitType = ImperialLengthType::mile;
            using name = StringLiteral<"mile">;
            using abbreviation_name = StringLiteral<"mi">;
        };
        struct LeagueTag {
            using tag_group = ImperialLengthTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialLengthUnitWeights::league);
            constexpr static ImperialLengthType unitType = ImperialLengthType::league;
            using name = StringLiteral<"league">;
            using abbreviation_name = StringLiteral<"lea">;
        };
    }

    template<SSize Magnitude = 1>
    using ImperialLengthUnit = Unit<impl::ImperialLengthTag, Magnitude>;
    template<SSize Magnitude = 1>
    using InchUnit = Unit<impl::InchTag, Magnitude>;
    template<SSize Magnitude = 1>
    using HandUnit = Unit<impl::HandTag, Magnitude>;
    template<SSize Magnitude = 1>
    using FootUnit = Unit<impl::FootTag, Magnitude>;
    template<SSize Magnitude = 1>
    using YardUnit = Unit<impl::YardTag, Magnitude>;
    template<SSize Magnitude = 1>
    using ChainUnit = Unit<impl::ChainTag, Magnitude>;
    template<SSize Magnitude = 1>
    using FurlongUnit = Unit<impl::FurlongTag, Magnitude>;
    template<SSize Magnitude = 1>
    using MileUnit = Unit<impl::MileTag, Magnitude>;
    template<SSize Magnitude = 1>
    using LeagueUnit = Unit<impl::LeagueTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1>
    using ImperialLength = BaseUnitValue<DataType, ImperialLengthUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Inch = BaseUnitValue<DataType, InchUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Hand = BaseUnitValue<DataType, HandUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Foot = BaseUnitValue<DataType, FootUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Yard = BaseUnitValue<DataType, YardUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Chain = BaseUnitValue<DataType, ChainUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Furlong = BaseUnitValue<DataType, FurlongUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Mile = BaseUnitValue<DataType, MileUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using League = BaseUnitValue<DataType, LeagueUnit<Magnitude>>;

    namespace value {
        constexpr inline ImperialLength<Size> imperialLength = ImperialLength<Size>(1);
        constexpr inline Inch<Size> inch = Inch<Size>(1); 
        constexpr inline Hand<Size> hand = Hand<Size>(1);
        constexpr inline Foot<Size> foot = Foot<Size>(1); 
        constexpr inline Yard<Size> yard = Yard<Size>(1); 
        constexpr inline Chain<Size> chain = Chain<Size>(1);
        constexpr inline Furlong<Size> furlong = Furlong<Size>(1);
        constexpr inline Mile<Size> mile = Mile<Size>(1); 
        constexpr inline League<Size> league = League<Size>(1);
    }

    namespace abbr {
        constexpr inline ImperialLength<Size> imperialLength = ImperialLength<Size>(1);
        constexpr inline Inch<Size> in = Inch<Size>(1);
        constexpr inline Hand<Size> hh = Hand<Size>(1);
        constexpr inline Foot<Size> ft = Foot<Size>(1);
        constexpr inline Yard<Size> yd = Yard<Size>(1);
        constexpr inline Chain<Size> ch = Chain<Size>(1);
        constexpr inline Furlong<Size> fur = Furlong<Size>(1);
        constexpr inline Mile<Size> mi = Mile<Size>(1);
        constexpr inline League<Size> lea = League<Size>(1);
    }

    namespace abbrt {
        using imperialLength = ImperialLengthUnit<1>;
        using in = InchUnit<1>;
        using hh = HandUnit<1>;
        using ft = FootUnit<1>;
        using yd = YardUnit<1>;
        using ch = ChainUnit<1>;
        using fur = FurlongUnit<1>;
        using mi = MileUnit<1>;
        using lea = LeagueUnit<1>;
    }

    //mass
    enum struct ImperialMassType {
        drachm,
        ounce,
        pound,
        stone,
        quarter,
        hundredWeight,
        ton,
    };

    namespace impl {
        struct ImperialMassTagGroup {
            using unit_category = MassUnitCategory;
            using group_type = StrongUnitGroupTypeUnitWeights;
        };

        struct ImperialMassTag {
            using system_generic_category = impl::ImperialMassTagGroup;
            using tag_group = impl::ImperialMassTagGroup;
        };

        enum struct ImperialMassUnitWeights : i64 {
            drachm = 1,
            ounce = drachm * 16,
            pound = ounce * 16,
            stone = pound * 14,
            quarter = pound * 28,
            hundredWeight = pound * 112,
            ton = 2240,
        };

        struct DrachmTag {
            using tag_group = ImperialMassTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialMassUnitWeights::drachm);
            constexpr static ImperialMassType unitType = ImperialMassType::drachm;
            using name = StringLiteral<"drachm">;
            using abbreviation_name = StringLiteral<"dr">;
        };
        struct OunceTag {
            using tag_group = ImperialMassTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialMassUnitWeights::ounce);
            constexpr static ImperialMassType unitType = ImperialMassType::ounce;
            using name = StringLiteral<"ounce">;
            using abbreviation_name = StringLiteral<"oz">;
        };
        struct PoundTag {
            using tag_group = ImperialMassTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialMassUnitWeights::pound);
            constexpr static ImperialMassType unitType = ImperialMassType::pound;
            using name = StringLiteral<"pound">;
            using abbreviation_name = StringLiteral<"lb">;
        };
        struct StoneTag {
            using tag_group = ImperialMassTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialMassUnitWeights::stone);
            constexpr static ImperialMassType unitType = ImperialMassType::stone;
            using name = StringLiteral<"stone">;
            using abbreviation_name = StringLiteral<"st">;
        };
        struct QuarterTag {
            using tag_group = ImperialMassTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialMassUnitWeights::quarter);
            constexpr static ImperialMassType unitType = ImperialMassType::quarter;
            using name = StringLiteral<"quarter">;
            using abbreviation_name = StringLiteral<"qtr">;
        };
        struct HundredWeightTag {
            using tag_group = ImperialMassTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialMassUnitWeights::hundredWeight);
            constexpr static ImperialMassType unitType = ImperialMassType::hundredWeight;
            using name = StringLiteral<"hundred_weight">;
            using abbreviation_name = StringLiteral<"cwt">;
        };
        struct TonTag {
            using tag_group = ImperialMassTagGroup;
            constexpr static i64 unitWeight = static_cast<i64>(ImperialMassUnitWeights::ton);
            constexpr static ImperialMassType unitType = ImperialMassType::ton;
            using name = StringLiteral<"ton">;
            using abbreviation_name = StringLiteral<"ton">;
        };
    }

    template<SSize Magnitude = 1>
    using ImperialMassUnit = Unit<impl::ImperialMassTag, Magnitude>;
    template<SSize Magnitude = 1>
    using DrachmUnit = Unit<impl::DrachmTag, Magnitude>;
    template<SSize Magnitude = 1>
    using OunceUnit = Unit<impl::OunceTag, Magnitude>;
    template<SSize Magnitude = 1>
    using PoundUnit = Unit<impl::PoundTag, Magnitude>;
    template<SSize Magnitude = 1>
    using StoneUnit = Unit<impl::StoneTag, Magnitude>;
    template<SSize Magnitude = 1>
    using QuarterUnit = Unit<impl::QuarterTag, Magnitude>;
    template<SSize Magnitude = 1>
    using HundredWeightUnit = Unit<impl::HundredWeightTag, Magnitude>;
    template<SSize Magnitude = 1>
    using TonUnit = Unit<impl::TonTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1>
    using ImperialMass = BaseUnitValue<DataType, ImperialMassUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Drachm = BaseUnitValue<DataType, DrachmUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Ounce = BaseUnitValue<DataType, OunceUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Pound = BaseUnitValue<DataType, PoundUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Stone = BaseUnitValue<DataType, StoneUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Quarter = BaseUnitValue<DataType, QuarterUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using HundredWeight = BaseUnitValue<DataType, HundredWeightUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Ton = BaseUnitValue<DataType, TonUnit<Magnitude>>;

    namespace value {
        constexpr inline ImperialMass<Size> imperialMass = ImperialMass<Size>(1);
        constexpr inline Drachm<Size> drachm = Drachm<Size>(1);
        constexpr inline Ounce<Size> ounce = Ounce<Size>(1);
        constexpr inline Pound<Size> pound = Pound<Size>(1);
        constexpr inline Stone<Size> stone = Stone<Size>(1);
        constexpr inline Quarter<Size> quarter = Quarter<Size>(1);
        constexpr inline HundredWeight<Size> hundredWeight = HundredWeight<Size>(1);
        constexpr inline Ton<Size> ton = Ton<Size>(1);
    }

    namespace abbr {
        constexpr inline ImperialMass<Size> imperialMass = ImperialMass<Size>(1);
        constexpr inline Drachm<Size> dr = Drachm<Size>(1);
        constexpr inline Ounce<Size> oz = Ounce<Size>(1);
        constexpr inline Pound<Size> lb = Pound<Size>(1);
        constexpr inline Stone<Size> st = Stone<Size>(1);
        constexpr inline Quarter<Size> qtr = Quarter<Size>(1);
        constexpr inline HundredWeight<Size> cwt = HundredWeight<Size>(1);
        constexpr inline Ton<Size> ton = Ton<Size>(1);
    }

    namespace abbrt {
        using imperialMass = ImperialMassUnit<1>;
        using dr = DrachmUnit<1>;
        using oz = OunceUnit<1>;
        using lb = PoundUnit<1>;
        using st = StoneUnit<1>;
        using qtr = QuarterUnit<1>;
        using cwt = HundredWeightUnit<1>;
        using ton = TonUnit<1>;
    }
}