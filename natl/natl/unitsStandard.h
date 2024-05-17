#pragma once 

//own
#include "unitsBase.h"

//interface
namespace natl {
    //time
    enum class StandardTimeType {
        unknown,
        nanoseconds,
        microseconds,
        milliseconds,
        centiseconds,
        deciseconds,
        seconds,
        decaseconds,
        minutes,
        hectoseconds,
        kiloseconds,
        hours,
        days,
        weeks,
        megaseconds,
        fortnights,
        years,
        decades,
        gigaseconds,
        centurys,
    };

    namespace impl {
        struct StandardTimeTagGroup {
            using unit_category = TimeUnitCategory;
            using group_type = StrongUnitGroupTypeUnitWeights;
            using name = StringLiteral<"time">;
        };

        struct StandardTimeTag {
            using system_generic_category = impl::StandardTimeTagGroup;
            using tag_group = impl::StandardTimeTagGroup;
        };

        enum class StandardTimeUnitWeight : i64 {
            nanoseconds = 1,
            microseconds = nanoseconds * 1000,
            milliseconds = microseconds * 1000,
            centiseconds = milliseconds * 10,
            deciseconds = centiseconds * 10,
            seconds = deciseconds * 10,
            decaseconds = seconds * 10,
            minutes = seconds * 60,
            hectoseconds = seconds * 100,
            kiloseconds = seconds * 1000,
            hours = minutes * 60,
            days = hours * 24,
            megaseconds = kiloseconds * 1000,
            weeks = days * 7,
            fortnights = days * 14,
            years = days * 365,
            decades = years * 10,
            gigaseconds = megaseconds * 1000,
            centurys = years * 100,
        };

        struct NanosecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::nanoseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::nanoseconds;
            using name = StringLiteral<"nanoseconds">;
            using abbreviation_name = StringLiteral<"ns">;
        };
        struct MicrosecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::microseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::microseconds;
            using name = StringLiteral<"microseconds">;
            using abbreviation_name = StringLiteral<"mus">;
        };
        struct MillisecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::milliseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::milliseconds;
            using name = StringLiteral<"milliseconds">;
            using abbreviation_name = StringLiteral<"ms">;
        };
        struct CentisecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::centiseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::centiseconds;
            using name = StringLiteral<"centiseconds">;
            using abbreviation_name = StringLiteral<"cs">;
        };
        struct DecisecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::deciseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::deciseconds;
            using name = StringLiteral<"deciseconds">;
            using abbreviation_name = StringLiteral<"ds">;
        };
        struct SecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::seconds;
            constexpr static StandardTimeType unitType = StandardTimeType::seconds;
            using name = StringLiteral<"seconds">;
            using abbreviation_name = StringLiteral<"s">;
        };
        struct DecasecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::decaseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::decaseconds;
            using name = StringLiteral<"decaseconds">;
            using abbreviation_name = StringLiteral<"das">;
        };
        struct MinutesTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::minutes;
            constexpr static StandardTimeType unitType = StandardTimeType::minutes;
            using name = StringLiteral<"minutes">;
            using abbreviation_name = StringLiteral<"min">;
        };
        struct HectosecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::hectoseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::hectoseconds;
            using name = StringLiteral<"hectoseconds">;
            using abbreviation_name = StringLiteral<"hs">;
        };
        struct KilosecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::kiloseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::kiloseconds;
            using name = StringLiteral<"kiloseconds">;
            using abbreviation_name = StringLiteral<"ks">;
        };
        struct HoursTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::hours;
            constexpr static StandardTimeType unitType = StandardTimeType::hours;
            using name = StringLiteral<"hours">;
            using abbreviation_name = StringLiteral<"hrs">;
        };
        struct DaysTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::days;
            constexpr static StandardTimeType unitType = StandardTimeType::days;
            using name = StringLiteral<"days">;
            using abbreviation_name = StringLiteral<"dys">;
        };
        struct WeeksTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::weeks;
            constexpr static StandardTimeType unitType = StandardTimeType::weeks;
            using name = StringLiteral<"weeks">;
            using abbreviation_name = StringLiteral<"wks">;
        };
        struct MegasecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::megaseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::megaseconds;
            using name = StringLiteral<"megaseconds">;
            using abbreviation_name = StringLiteral<"Ms">;
        };
        struct FortnightsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::fortnights;
            constexpr static StandardTimeType unitType = StandardTimeType::fortnights;
            using name = StringLiteral<"fortnights">;
            using abbreviation_name = StringLiteral<"ftns">;
        };
        struct YearsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::years;
            constexpr static StandardTimeType unitType = StandardTimeType::years;
            using name = StringLiteral<"years">;
            using abbreviation_name = StringLiteral<"yrs">;
        };
        struct DecadesTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::decades;
            constexpr static StandardTimeType unitType = StandardTimeType::decades;
            using name = StringLiteral<"decades">;
            using abbreviation_name = StringLiteral<"dcads">;
        };
        struct GigasecondsTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::gigaseconds;
            constexpr static StandardTimeType unitType = StandardTimeType::gigaseconds;
            using name = StringLiteral<"gigaseconds">;
            using abbreviation_name = StringLiteral<"Gs">;
        };
        struct CenturysTag {
            using tag_group = StandardTimeTagGroup;
            constexpr static i64 unitWeight = (i64)StandardTimeUnitWeight::centurys;
            constexpr static StandardTimeType unitType = StandardTimeType::centurys;
            using name = StringLiteral<"centurys">;
            using abbreviation_name = StringLiteral<"ctrys">;
        };
    }

    template<SSize Magnitude = 1> 
    using NanosecondsUnit = Unit<impl::NanosecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using MicrosecondsUnit = Unit<impl::MicrosecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using MillisecondsUnit = Unit<impl::MillisecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using CentisecondsUnit = Unit<impl::CentisecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using DecisecondsUnit = Unit<impl::DecisecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using SecondsUnit = Unit<impl::SecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using DecasecondsUnit = Unit<impl::DecasecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using MinutesUnit = Unit<impl::MinutesTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using HectosecondsUnit = Unit<impl::HectosecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using KilosecondsUnit = Unit<impl::KilosecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using HoursUnit = Unit<impl::HoursTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using DaysUnit = Unit<impl::DaysTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using WeeksUnit = Unit<impl::WeeksTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using MegasecondsUnit = Unit<impl::MegasecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using FortnightsUnit = Unit<impl::FortnightsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using YearsUnit = Unit<impl::YearsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using DecadesUnit = Unit<impl::DecadesTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using GigasecondsUnit = Unit<impl::GigasecondsTag, Magnitude>;
    template<SSize Magnitude = 1> 
    using CenturysUnit = Unit<impl::CenturysTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1>
    using Nanoseconds = BaseUnitValue<DataType, NanosecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Microseconds = BaseUnitValue<DataType, MicrosecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Milliseconds = BaseUnitValue<DataType, MillisecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Centiseconds = BaseUnitValue<DataType, CentisecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Deciseconds = BaseUnitValue<DataType, DecisecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Seconds = BaseUnitValue<DataType, SecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Decaseconds = BaseUnitValue<DataType, DecasecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Minutes = BaseUnitValue<DataType, MinutesUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Hectoseconds = BaseUnitValue<DataType, HectosecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Kiloseconds = BaseUnitValue<DataType, KilosecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Hours = BaseUnitValue<DataType, HoursUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Days = BaseUnitValue<DataType, DaysUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Weeks = BaseUnitValue<DataType, WeeksUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Megaseconds = BaseUnitValue<DataType, MegasecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Fortnights = BaseUnitValue<DataType, FortnightsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Years = BaseUnitValue<DataType, YearsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Decades = BaseUnitValue<DataType, DecadesUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Gigaseconds = BaseUnitValue<DataType, GigasecondsUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Centurys = BaseUnitValue<DataType, CenturysUnit<Magnitude>>;

    namespace value {
        constexpr inline Nanoseconds<Size> nanoseconds = Nanoseconds<Size>(1);
        constexpr inline Microseconds<Size> microseconds = Microseconds<Size>(1);
        constexpr inline Milliseconds<Size> milliseconds = Milliseconds<Size>(1);
        constexpr inline Centiseconds<Size> centiseconds = Centiseconds<Size>(1);
        constexpr inline Deciseconds<Size> deciseconds = Deciseconds<Size>(1);
        constexpr inline Seconds<Size> seconds = Seconds<Size>(1);
        constexpr inline Decaseconds<Size> decaseconds = Decaseconds<Size>(1);
        constexpr inline Minutes<Size> minutes = Minutes<Size>(1);
        constexpr inline Hectoseconds<Size> hectoseconds = Hectoseconds<Size>(1);
        constexpr inline Kiloseconds<Size> kiloseconds = Kiloseconds<Size>(1);
        constexpr inline Hours<Size> hours = Hours<Size>(1);
        constexpr inline Days<Size> days = Days<Size>(1);
        constexpr inline Weeks<Size> weeks = Weeks<Size>(1);
        constexpr inline Megaseconds<Size> megaseconds = Megaseconds<Size>(1);
        constexpr inline Fortnights<Size> fortnights = Fortnights<Size>(1);
        constexpr inline Years<Size> years = Years<Size>(1);
        constexpr inline Decades<Size> decades = Decades<Size>(1);
        constexpr inline Gigaseconds<Size> gigaseconds = Gigaseconds<Size>(1);
        constexpr inline Centurys<Size> centurys = Centurys<Size>(1);
    }

    namespace abbr {
        constexpr inline Nanoseconds<Size> ns = Nanoseconds<Size>(1);
        constexpr inline Microseconds<Size> mus = Microseconds<Size>(1);
        constexpr inline Milliseconds<Size> ms = Milliseconds<Size>(1);
        constexpr inline Centiseconds<Size> cs = Centiseconds<Size>(1);
        constexpr inline Deciseconds<Size> ds = Deciseconds<Size>(1);
        constexpr inline Seconds<Size> s = Seconds<Size>(1);
        constexpr inline Decaseconds<Size> das = Decaseconds<Size>(1);
        constexpr inline Minutes<Size> min = Minutes<Size>(1);
        constexpr inline Hectoseconds<Size> hs = Hectoseconds<Size>(1);
        constexpr inline Kiloseconds<Size> ks = Kiloseconds<Size>(1);
        constexpr inline Hours<Size> hrs = Hours<Size>(1);
        constexpr inline Days<Size> dys = Days<Size>(1);
        constexpr inline Weeks<Size> wks = Weeks<Size>(1);
        constexpr inline Megaseconds<Size> Ms = Megaseconds<Size>(1);
        constexpr inline Fortnights<Size> ftns = Fortnights<Size>(1);
        constexpr inline Years<Size> yrs = Years<Size>(1);
        constexpr inline Decades<Size> dcads = Decades<Size>(1);
        constexpr inline Gigaseconds<Size> Gs = Gigaseconds<Size>(1);
        constexpr inline Centurys<Size> ctrys = Centurys<Size>(1);
    }

    namespace abbrt {
        using ns = NanosecondsUnit<1>;
        using mus = MicrosecondsUnit<1>;
        using ms = MillisecondsUnit<1>;
        using cs = CentisecondsUnit<1>;
        using ds = DecisecondsUnit<1>;
        using s = SecondsUnit<1>;
        using das = DecasecondsUnit<1>;
        using min = MinutesUnit<1>;
        using hs = HectosecondsUnit<1>;
        using ks = KilosecondsUnit<1>;
        using hrs = HoursUnit<1>;
        using dys = DaysUnit<1>;
        using wks = WeeksUnit<1>;
        using Ms = MegasecondsUnit<1>;
        using ftns = FortnightsUnit<1>;
        using yrs = YearsUnit<1>;
        using dcads = DecadesUnit<1>;
        using Gs = GigasecondsUnit<1>;
        using ctrys = CenturysUnit<1>;
    }

    //data
    enum class StandardDataType {
       bit,
       byte,
       kilobyte,
       megabyte,
       gigabyte,
       terabyte,
       petabyte
    };

    namespace impl {
        struct StandardDataTagGroup {
            using unit_category = DataUnitCategory;
            using group_type = StrongUnitGroupTypeUnitWeights;
            using name = StringLiteral<"data">;
        };

        struct StandardDataTag {
            using system_generic_category = impl::StandardDataTagGroup;
            using tag_group = impl::StandardDataTagGroup;
        };

        enum class StandardDataUnitWeight : i64 {
            bit = 1,
            byte = bit * 8,
            kilobyte = byte * 1024,
            megabyte = kilobyte * 1024,
            gigabyte = megabyte * 1024,
            terabyte = gigabyte * 1024,
            petabyte = terabyte * 1024,
        };

        struct BitTag {
            using tag_group = StandardDataTagGroup;
            constexpr static i64 unitWeight = (i64)StandardDataUnitWeight::bit;
            constexpr static StandardDataType unitType = StandardDataType::bit;
            using name = StringLiteral<"bit">;
            using abbreviation_name = StringLiteral<"b">;
        };
        struct ByteTag {
            using tag_group = StandardDataTagGroup;
            constexpr static i64 unitWeight = (i64)StandardDataUnitWeight::byte;
            constexpr static StandardDataType unitType = StandardDataType::byte;
            using name = StringLiteral<"byte">;
            using abbreviation_name = StringLiteral<"B">;
        };
        struct KilobyteTag {
            using tag_group = StandardDataTagGroup;
            constexpr static i64 unitWeight = (i64)StandardDataUnitWeight::kilobyte;
            constexpr static StandardDataType unitType = StandardDataType::kilobyte;
            using name = StringLiteral<"kilobyte">;
            using abbreviation_name = StringLiteral<"KB">;
        };
        struct MegabyteTag {
            using tag_group = StandardDataTagGroup;
            constexpr static i64 unitWeight = (i64)StandardDataUnitWeight::megabyte;
            constexpr static StandardDataType unitType = StandardDataType::megabyte;
            using name = StringLiteral<"megabyte">;
            using abbreviation_name = StringLiteral<"MB">;
        };
        struct GigabyteTag {
            using tag_group = StandardDataTagGroup;
            constexpr static i64 unitWeight = (i64)StandardDataUnitWeight::gigabyte;
            constexpr static StandardDataType unitType = StandardDataType::gigabyte;
            using name = StringLiteral<"gigabyte">;
            using abbreviation_name = StringLiteral<"GB">;
        };
        struct TerabyteTag {
            using tag_group = StandardDataTagGroup;
            constexpr static i64 unitWeight = (i64)StandardDataUnitWeight::terabyte;
            constexpr static StandardDataType unitType = StandardDataType::terabyte;
            using name = StringLiteral<"terabyte">;
            using abbreviation_name = StringLiteral<"TB">;
        };
        struct PetabyteTag {
            using tag_group = StandardDataTagGroup;
            constexpr static i64 unitWeight = (i64)StandardDataUnitWeight::petabyte;
            constexpr static StandardDataType unitType = StandardDataType::petabyte;
            using name = StringLiteral<"petabyte">;
            using abbreviation_name = StringLiteral<"PB">;
        };
    }

    template<SSize Magnitude = 1>
    using StandardDataUnit = Unit<impl::StandardDataTag, Magnitude>;
    template<SSize Magnitude = 1>
    using BitUnit = Unit<impl::BitTag, Magnitude>;
    template<SSize Magnitude = 1>
    using ByteUnit = Unit<impl::ByteTag, Magnitude>;
    template<SSize Magnitude = 1>
    using KilobyteUnit = Unit<impl::KilobyteTag, Magnitude>;
    template<SSize Magnitude = 1>
    using MegabyteUnit = Unit<impl::MegabyteTag, Magnitude>;
    template<SSize Magnitude = 1>
    using GigabyteUnit = Unit<impl::GigabyteTag, Magnitude>;
    template<SSize Magnitude = 1>
    using TerabyteUnit = Unit<impl::TerabyteTag, Magnitude>;
    template<SSize Magnitude = 1>
    using PetabyteUnit = Unit<impl::PetabyteTag, Magnitude>;

    template<typename DataType, SSize Magnitude = 1>
    using StandardData = BaseUnitValue<DataType, StandardDataUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Bit = BaseUnitValue<DataType, BitUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using ByteV = BaseUnitValue<DataType, ByteUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Kilobyte = BaseUnitValue<DataType, KilobyteUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Megabyte = BaseUnitValue<DataType, MegabyteUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Gigabyte = BaseUnitValue<DataType, GigabyteUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Terabyte = BaseUnitValue<DataType, TerabyteUnit<Magnitude>>;
    template<typename DataType, SSize Magnitude = 1>
    using Petabyte = BaseUnitValue<DataType, PetabyteUnit<Magnitude>>;

    namespace value {
        constexpr inline StandardData<Size> standardData = StandardData<Size>(1);
        constexpr inline Bit<Size> bit = Bit<Size>(1);
        constexpr inline ByteV<Size> byte = ByteV<Size>(1);
        constexpr inline Kilobyte<Size> kilobyte = Kilobyte<Size>(1);
        constexpr inline Megabyte<Size> megabyte = Megabyte<Size>(1);
        constexpr inline Gigabyte<Size> gigabyte = Gigabyte<Size>(1);
        constexpr inline Terabyte<Size> terabyte = Terabyte<Size>(1);
        constexpr inline Petabyte<Size> petabyte = Petabyte<Size>(1);
    }

    namespace abbr {
        constexpr inline StandardData<Size> standardData = StandardData<Size>(1);
        constexpr inline Bit<Size> b = Bit<Size>(1);
        constexpr inline ByteV<Size> B = ByteV<Size>(1);
        constexpr inline Kilobyte<Size> KB = Kilobyte<Size>(1);
        constexpr inline Megabyte<Size> MB = Megabyte<Size>(1);
        constexpr inline Gigabyte<Size> GB = Gigabyte<Size>(1);
        constexpr inline Terabyte<Size> TB = Terabyte<Size>(1);
        constexpr inline Petabyte<Size> PB = Petabyte<Size>(1);
    }

    namespace abbrt {
        using standardData = StandardDataUnit<1>;
        using b = BitUnit<1>;
        using B = ByteUnit<1>;
        using KB = KilobyteUnit<1>;
        using MB = MegabyteUnit<1>;
        using GB = GigabyteUnit<1>;
        using TB = TerabyteUnit<1>;
        using PB = PetabyteUnit<1>;
    }
}