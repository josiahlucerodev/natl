
//natl
#include <natl/util/enum.h>
#include <natl/util/test.h>


constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlEnumTest";

enum struct TestSequentialEnum {
	value0 = 0, value1 = 1, value2 = 2, value3 = 3,
	value4 = 4, value5 = 5, value6 = 6, value7 = 7,
};

enum struct TestSequentialEnumBase1 {
	value0 = 1, value1 = 2, value2 = 3, value3 = 4,
	value4 = 5, value5 = 6, value6 = 7, value7 = 8,
};

enum struct TestBitFlagEnum {
	value0 = 0,      value1 = 1 << 0, value2 = 1 << 1, value3 = 1 << 2,
	value4 = 1 << 3, value5 = 1 << 4, value6 = 1 << 5, value7 = 1 << 6,
};

enum struct TestBitFlagEnumBase1 {
	value0 = 1 << 0, value1 = 1 << 1, value2 = 1 << 2, value3 = 1 << 3,
	value4 = 1 << 4, value5 = 1 << 5, value6 = 1 << 6, value7 = 1 << 7,
};


constexpr natl::Size c_GapValue = 5;

enum struct TestSequentialEnumGap {
	value0 = 0, value1 = 1, value2 = 2, value3 = 3,
	value4 = 4 + c_GapValue, value5 = 5 + c_GapValue,
	value6 = 6 + c_GapValue, value7 = 7 + c_GapValue,
};

template<> struct natl::EnumTraits<TestSequentialEnumGap> {
	using enum_type = TestSequentialEnumGap;
	using underlying_type = UnderlyingType<enum_type>;
	constexpr inline static underlying_type startValue = toUnderlying(TestSequentialEnumGap::value0);
	constexpr inline static EnumValueFlag enumValueFlag = EnumValueFlag::sequential;
	using unique_values = SequentialEnumValuesInRange<TestSequentialEnumGap,
		TestSequentialEnumGap::value4, TestSequentialEnumGap::value7>;
};

enum struct TestBitFlagEnumGap {
	value0 = 0, value1 = 1 << 0, value2 = 1 << 1, value3 = 1 << 2,
	value4 = 1 << (3 + c_GapValue), value5 = 1 << (4 + c_GapValue),
	value6 = 1 << (5 + c_GapValue), value7 = 1 << (6 + c_GapValue),
};

template<> struct natl::EnumTraits<TestBitFlagEnumGap> {
	using enum_type = TestBitFlagEnumGap;
	using underlying_type = UnderlyingType<enum_type>;
	constexpr inline static underlying_type startValue = toUnderlying(TestBitFlagEnumGap::value0);
	constexpr inline static EnumValueFlag enumValueFlag = EnumValueFlag::bitFlags;
	using unique_values = BitFlagEnumValuesInRange<TestBitFlagEnumGap,
		TestBitFlagEnumGap::value4, TestBitFlagEnumGap::value7>;
};
static_assert(natl::IsSameC<
	natl::BitFlagEnumValuesInRange<TestBitFlagEnumGap, TestBitFlagEnumGap::value4, TestBitFlagEnumGap::value7>,
	natl::BitFlagEnumValuesInIndexRange<TestBitFlagEnumGap, 0, 1 + 3 + c_GapValue, 1 + 6 + c_GapValue>
>);

enum struct TestAllValuesEnum {
	value0 = 0 + c_GapValue, value1 = 1 + c_GapValue,
	value2 = 2 + c_GapValue, value3 = 3 + c_GapValue,
	value4 = 1 << (3 + c_GapValue), value5 = 1 << (4 + c_GapValue),
	value6 = 1 << (5 + c_GapValue), value7 = 1 << (6 + c_GapValue),
};

template<> struct natl::EnumTraits<TestAllValuesEnum> {
	using enum_type = TestAllValuesEnum;
	using underlying_type = UnderlyingType<enum_type>;
	constexpr inline static EnumValueFlag enumValueFlag = EnumValueFlag::custom;
	using all_values = TypePackMerge<
		SequentialEnumValuesInRange<TestAllValuesEnum, TestAllValuesEnum::value0, TestAllValuesEnum::value3>,
		BitFlagEnumValuesInRange<TestAllValuesEnum, TestAllValuesEnum::value4, TestAllValuesEnum::value7>
	>;
};

enum struct TestNothingEnum {
	value0 = 69,
	value1 = 420,
	value2 = 1337,
	value3 = 666,
	value4 = 42069,
	value5 = 7,
	value6 = 13,
	value7 = 80085,
};

template<typename EnumType>
constexpr natl::Bool testEnumValues(const natl::ConstAsciiStringView& name) noexcept {
	using enum_values = natl::EnumValues<EnumType>;
	natl::String256 nameOfEnum = natl::getNameOfEnum<EnumType>();
	natl::Test test(natlTestFrom, name, natl::TestType::leaf);
	natl::testAssertEquals(test, EnumType::value0, enum_values::template at<0>::value, nameOfEnum + "::value0 value");
	natl::testAssertEquals(test, "value0", enum_values::template at<0>::name.toStringView(), nameOfEnum + "::value0 name");
	natl::testAssertEquals(test, EnumType::value1, enum_values::template at<1>::value, nameOfEnum + "::value1 value");
	natl::testAssertEquals(test, "value1", enum_values::template at<1>::name.toStringView(), nameOfEnum + "::value1 name");
	natl::testAssertEquals(test, EnumType::value2, enum_values::template at<2>::value, nameOfEnum + "::value2 value");
	natl::testAssertEquals(test, "value2", enum_values::template at<2>::name.toStringView(), nameOfEnum + "::value2 name");
	natl::testAssertEquals(test, EnumType::value3, enum_values::template at<3>::value, nameOfEnum + "::value3 value");
	natl::testAssertEquals(test, "value3", enum_values::template at<3>::name.toStringView(), nameOfEnum + "::value3 name");
	natl::testAssertEquals(test, EnumType::value4, enum_values::template at<4>::value, nameOfEnum + "::value4 value");
	natl::testAssertEquals(test, "value4", enum_values::template at<4>::name.toStringView(), nameOfEnum + "::value4 name");
	natl::testAssertEquals(test, EnumType::value5, enum_values::template at<5>::value, nameOfEnum + "::value5 value");
	natl::testAssertEquals(test, "value5", enum_values::template at<5>::name.toStringView(), nameOfEnum + "::value5 name");
	natl::testAssertEquals(test, EnumType::value6, enum_values::template at<6>::value, nameOfEnum + "::value6 value");
	natl::testAssertEquals(test, "value6", enum_values::template at<6>::name.toStringView(), nameOfEnum + "::value6 name");
	natl::testAssertEquals(test, EnumType::value7, enum_values::template at<7>::value, nameOfEnum + "::value7 value");
	natl::testAssertEquals(test, "value7", enum_values::template at<7>::name.toStringView(), nameOfEnum + "::value7 name");
	return test;
}


template<typename EnumType>
constexpr natl::Bool testEnumToString(const natl::ConstAsciiStringView& name) noexcept {
	using enum_values = natl::EnumValues<EnumType>;
	using string_type = natl::String256;
	natl::String256 nameOfEnum = natl::getNameOfEnum<EnumType>();

	natl::Test test(natlTestFrom, name, natl::TestType::leaf);
	natl::testAssertEquals(test, enum_values::template at<0>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value0), nameOfEnum + "::value0 to string");
	natl::testAssertEquals(test, enum_values::template at<1>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value1), nameOfEnum + "::value1 to string");
	natl::testAssertEquals(test, enum_values::template at<2>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value2), nameOfEnum + "::value2 to string");
	natl::testAssertEquals(test, enum_values::template at<3>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value3), nameOfEnum + "::value3 to string");
	natl::testAssertEquals(test, enum_values::template at<4>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value4), nameOfEnum + "::value4 to string");
	natl::testAssertEquals(test, enum_values::template at<5>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value5), nameOfEnum + "::value5 to string");
	natl::testAssertEquals(test, enum_values::template at<6>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value6), nameOfEnum + "::value6 to string");
	natl::testAssertEquals(test, enum_values::template at<7>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value7), nameOfEnum + "::value7 to string");
	return test;
}

template<typename EnumType>
constexpr natl::Bool testEnumFromString(const natl::ConstAsciiStringView& name) noexcept {
	using enum_values = natl::EnumValues<EnumType>;
	using string_type = natl::String256;
	natl::String256 nameOfEnum = natl::getNameOfEnum<EnumType>();

	natl::Test test(natlTestFrom, name, natl::TestType::leaf);
	natl::testOptionAssertEquals(test,
		enum_values::template at<0>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value0)),
		nameOfEnum + "::value0 from string");
	natl::testOptionAssertEquals(test,
		enum_values::template at<1>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value1)),
		nameOfEnum + "::value1 from string");
	natl::testOptionAssertEquals(test,
		enum_values::template at<2>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value2)),
		nameOfEnum + "::value2 from string");
	natl::testOptionAssertEquals(test,
		enum_values::template at<3>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value3)),
		nameOfEnum + "::value3 from string");
	natl::testOptionAssertEquals(test,
		enum_values::template at<4>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value4)),
		nameOfEnum + "::value4 from string");
	natl::testOptionAssertEquals(test,
		enum_values::template at<5>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value5)),
		nameOfEnum + "::value5 from string");
	natl::testOptionAssertEquals(test,
		enum_values::template at<6>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value6)),
		nameOfEnum + "::value6 from string");
	natl::testOptionAssertEquals(test,
		enum_values::template at<7>::value,
		natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value7)),
		nameOfEnum + "::value7 from string");
	return test;
}

template<typename EnumType> 
struct GeneratedBitFlagEnumInfo {
	EnumType value;
	natl::String256 name;
};

template<typename EnumType, typename EnumValueTypes>
constexpr GeneratedBitFlagEnumInfo<EnumType> generateTestBitFlagEnumValue(const natl::ui64 seed) noexcept {
	constexpr natl::Size offset = natl::EnumTraits<EnumType>::startValue == 0 ? 1 : 0;
	natl::Bool addSeperator = false;
	natl::UnderlyingType<EnumType> value = 0;
	natl::String256 name = "";
	natl::String256 seperator = " | ";

	if (seed == 0) {
		name = EnumValueTypes::template at<0>::name.toStringView();
	}

	if (seed & (1 << 0)) {
		value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<0 + offset>::value);
		name += EnumValueTypes::template at<0 + offset>::name.toStringView();
		addSeperator = true;
	}
	if (seed & (1 << 1)) {
		if (addSeperator) {
			name += seperator;
		}
		value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<1 + offset>::value);
		name += EnumValueTypes::template at<1 + offset>::name.toStringView();
		addSeperator = true;
	}
	if (seed & (1 << 2)) {
		if (addSeperator) {
			name += seperator;
		}
		value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<2 + offset>::value);
		name += EnumValueTypes::template at<2 + offset>::name.toStringView();
		addSeperator = true;
	}
	if (seed & (1 << 3)) {
		if (addSeperator) {
			name += seperator;
		}
		value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<3 + offset>::value);
		name += EnumValueTypes::template at<3 + offset>::name.toStringView();
		addSeperator = true;
	}
	if (seed & (1 << 4)) {
		if (addSeperator) {
			name += seperator;
		}
		value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<4 + offset>::value);
		name += EnumValueTypes::template at<4 + offset>::name.toStringView();
		addSeperator = true;
	}
	if (seed & (1 << 5)) {
		if (addSeperator) {
			name += seperator;
		}
		value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<5 + offset>::value);
		name += EnumValueTypes::template at<5 + offset>::name.toStringView();
		addSeperator = true;
	}
	if (seed & (1 << 6)) {
		if (addSeperator) {
			name += seperator;
		}
		value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<6 + offset>::value);
		name += EnumValueTypes::template at<6 + offset>::name.toStringView();
		addSeperator = true;
	}
	if constexpr (offset == 0) {
		if (seed & (1 << 7)) {
			if (addSeperator) {
				name += seperator;
			}
			value |= natl::toUnderlying<EnumType>(EnumValueTypes::template at<7 + offset>::value);
			name += EnumValueTypes::template at<7 + offset>::name.toStringView();
			addSeperator = true;
		}
	}
	return GeneratedBitFlagEnumInfo<EnumType>{natl::fromUnderlying<EnumType>(value), name};
}

template<typename EnumType>
constexpr natl::Bool testBitFlagEnumToString(const natl::ConstAsciiStringView& name) noexcept {
	using string_type = natl::String256;
	using enum_values = natl::EnumValues<EnumType>;
	natl::String256 nameOfEnum = natl::getNameOfEnum<EnumType>();
	natl::Size maxFlagIndex = static_cast<natl::Size>(natl::bitscanForward(natl::toUnderlying(enum_values::template at<7>::value)));
	natl::Size numIterations = natl::min<natl::Size>(natl::isConstantEvaluated() ? 10 : 0xffffff, (natl::Size(1) << maxFlagIndex));
	natl::Test test(natlTestFrom, name, natl::TestType::leaf);
	for (natl::ui64 i = natl::EnumTraits<EnumType>::startValue; i <= numIterations; i++) {
		GeneratedBitFlagEnumInfo<EnumType> info = generateTestBitFlagEnumValue<EnumType, enum_values>(i);
		natl::testAssertEquals(test, 
			info.name,
			natl::enumToString<EnumType, string_type>(info.value),
			nameOfEnum + " to string value (" + info.name, ")");
	}
	return test;
};

template<typename EnumType>
constexpr natl::Bool testBitFlagEnumFromString(const natl::ConstAsciiStringView& name) noexcept {
	using string_type = natl::String256;
	using enum_values = natl::EnumValues<EnumType>;
	natl::String256 nameOfEnum = natl::getNameOfEnum<EnumType>();
	natl::Size numIterations = natl::min<natl::Size>(natl::isConstantEvaluated() ? 10 : 0xffffff, natl::Limits<natl::ui8>::max());

	natl::Test test(natlTestFrom, name, natl::TestType::leaf);
	for (natl::ui64 i = natl::EnumTraits<EnumType>::startValue; i <= numIterations; i++) {
		GeneratedBitFlagEnumInfo<EnumType> info = generateTestBitFlagEnumValue<EnumType, enum_values>(i);
		natl::testOptionAssertEquals(test,
			info.value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(info.value)),
			nameOfEnum + " from string value (" + info.name, ")");
	}
	return test;
};


natl::Bool utilTests() noexcept {
	natl::Test test(natlTestFrom, "util tests", natl::TestType::leaf);
	natl::testAssertEquals(test, "TestSequentialEnum", natl::getNameOfEnum<TestSequentialEnum>(), "getNameOfEnum from string");
	natl::testAssertEquals(test, "value0", natl::getNameOfEnumValue<TestSequentialEnum::value0>(), "getNameOfEnumValue from string");
	natl::testAssertEquals(test, true, natl::testIfValidEnumValue<TestSequentialEnum, natl::toUnderlying(TestSequentialEnum::value0)>(), "test if value0 is valid");
	natl::testAssertEquals(test, false, natl::testIfValidEnumValue<TestSequentialEnum, 100>(), "test if 100 is valid enum");
	return test;
}


constexpr natl::Bool defaultSequentialEnumTest() noexcept {
	return testEnumValues<TestSequentialEnum>("default sequential enum value tests");
}
static_assert(defaultSequentialEnumTest());
constexpr natl::Bool defaultSequentialEnumBase1Test() noexcept {
	return testEnumValues<TestSequentialEnumBase1>("default sequential enum base 1 value tests");
}
static_assert(defaultSequentialEnumBase1Test());
constexpr natl::Bool defaultBitFlagEnumTest() noexcept {
	return testEnumValues<TestBitFlagEnum>("default bit flag enum value tests");
}
static_assert(defaultBitFlagEnumTest());
constexpr natl::Bool defaultBitFlagEnumBase1Test() noexcept {
	return testEnumValues<TestBitFlagEnumBase1>("default bit flag enum base 1 value tests");
}
static_assert(defaultBitFlagEnumBase1Test());

natl::Bool defaultEnumValueTests() noexcept {
	natl::Test test(natlTestFrom, "default enum value tests", natl::TestType::node);
	natl::subTestAssert(test, defaultSequentialEnumTest());
	natl::subTestAssert(test, defaultSequentialEnumBase1Test());
	natl::subTestAssert(test, defaultBitFlagEnumTest());
	natl::subTestAssert(test, defaultBitFlagEnumBase1Test());
	return test;
}

constexpr natl::Bool customSequentialEnumGapTest() noexcept {
	return testEnumValues<TestSequentialEnumGap>("custom sequential enum with gap tests");
}
static_assert(customSequentialEnumGapTest());
constexpr natl::Bool customBitFlagEnumGapTest() noexcept {
	return testEnumValues<TestBitFlagEnumGap>("custom bit flag enum with gap tests");
}
static_assert(customBitFlagEnumGapTest());
constexpr natl::Bool customAllValuesEnumTest() noexcept {
	return testEnumValues<TestAllValuesEnum>("custom all values enum tests");
}
static_assert(customAllValuesEnumTest());
constexpr natl::Bool customNothingEnumTest() noexcept {
	natl::Test test(natlTestFrom, "nothing enum tests", natl::TestType::leaf);
	natl::testAssert(test, !natl::HasEnumValues<TestNothingEnum>, "has no enum values");
	return test;
}
static_assert(customNothingEnumTest());

natl::Bool customEnumValueTests() noexcept {
	natl::Test test(natlTestFrom, "custom enum value tests", natl::TestType::node);
	natl::subTestAssert(test, customSequentialEnumGapTest());
	natl::subTestAssert(test, customBitFlagEnumGapTest());
	natl::subTestAssert(test, customAllValuesEnumTest());
	natl::subTestAssert(test, customNothingEnumTest());
	return test;
}

constexpr natl::Bool defaultSequentialEnumToStringTest() noexcept {
	return testEnumToString<TestSequentialEnum>("default sequential enum to string tests");
}
static_assert(defaultSequentialEnumToStringTest());
constexpr natl::Bool defaultSequentialEnumBase1ToStringTest() noexcept {
	return testEnumToString<TestSequentialEnumBase1>("default sequential enum base 1 to string tests");
}
static_assert(defaultSequentialEnumBase1ToStringTest());
constexpr natl::Bool defaultBitFlagEnumToStringTest() noexcept {
	return testEnumToString<TestBitFlagEnum>("default bit flag enum to string tests");
}
static_assert(defaultBitFlagEnumToStringTest());
constexpr natl::Bool defaultBitFlagEnumBase1ToStringTest() noexcept {
	return testEnumToString<TestBitFlagEnumBase1>("default bit flag enum base 1 to string tests");
}
static_assert(defaultBitFlagEnumBase1ToStringTest());

natl::Bool defaultToStringTests() noexcept {
	natl::Test test(natlTestFrom, "default to string tests", natl::TestType::node);
	natl::subTestAssert(test, defaultSequentialEnumToStringTest());
	natl::subTestAssert(test, defaultSequentialEnumBase1ToStringTest());
	natl::subTestAssert(test, defaultBitFlagEnumToStringTest());
	natl::subTestAssert(test, defaultBitFlagEnumBase1ToStringTest());
	return test;
}

constexpr natl::Bool customSequentialEnumGapToStringTest() noexcept {
	return testEnumToString<TestSequentialEnumGap>("custom sequential enum with gap to string tests");
}
static_assert(customSequentialEnumGapToStringTest());
constexpr natl::Bool customBitFlagEnumGapToStringTest() noexcept {
	return testEnumToString<TestBitFlagEnumGap>("custom bit flag enum with gap to string tests");
}
static_assert(customBitFlagEnumGapToStringTest());

natl::Bool customToStringTests() noexcept {
	natl::Test test(natlTestFrom, "custom to string tests", natl::TestType::node);
	natl::subTestAssert(test, customSequentialEnumGapToStringTest());
	natl::subTestAssert(test, customBitFlagEnumGapToStringTest());
	return test;
}

constexpr natl::Bool defaultSequentialEnumFromStringTest() noexcept {
	return testEnumFromString<TestSequentialEnum>("default sequential enum from string tests");
}
static_assert(defaultSequentialEnumFromStringTest());
constexpr natl::Bool defaultSequentialEnumBase1FromStringTest() noexcept {
	return testEnumFromString<TestSequentialEnumBase1>("default sequential enum base 1 from string tests");
}
static_assert(defaultSequentialEnumBase1FromStringTest());
constexpr natl::Bool defaultBitFlagEnumFromStringTest() noexcept {
	return testEnumFromString<TestBitFlagEnum>("default bit flag enum from string tests");
}
static_assert(defaultBitFlagEnumFromStringTest());
constexpr natl::Bool defaultBitFlagEnumBase1FromStringTest() noexcept {
	return testEnumFromString<TestBitFlagEnumBase1>("default bit flag enum base 1 from string tests");
}
static_assert(defaultBitFlagEnumBase1FromStringTest());

natl::Bool defaultFromStringTests() noexcept {
	natl::Test test(natlTestFrom, "default from string tests", natl::TestType::node);
	natl::subTestAssert(test, defaultSequentialEnumFromStringTest());
	natl::subTestAssert(test, defaultSequentialEnumBase1FromStringTest());
	natl::subTestAssert(test, defaultBitFlagEnumFromStringTest());
	natl::subTestAssert(test, defaultBitFlagEnumBase1FromStringTest());
	return test;
}

constexpr natl::Bool customSequentialEnumGapFromStringTest() noexcept {
	return testEnumFromString<TestSequentialEnumGap>("custom sequential enum with gap from string tests");
}
static_assert(customSequentialEnumGapFromStringTest());
constexpr natl::Bool customBitFlagEnumGapFromStringTest() noexcept {
	return testEnumFromString<TestBitFlagEnumGap>("custom bit flag enum with gap from string tests");
}
static_assert(customBitFlagEnumGapFromStringTest());

natl::Bool customFromStringTests() noexcept {
	natl::Test test(natlTestFrom, "custom from string tests", natl::TestType::node);
	natl::subTestAssert(test, customSequentialEnumGapFromStringTest());
	natl::subTestAssert(test, customBitFlagEnumGapFromStringTest());
	return test;
}

constexpr natl::Bool defaultBitFlagEnumToStringAllFlagsTest() noexcept {
	return testBitFlagEnumToString<TestBitFlagEnum>("default bit flag enum to string all flags tests");
}
static_assert(defaultBitFlagEnumToStringAllFlagsTest());
constexpr natl::Bool defaultBitFlagEnumBase1ToStringAllFlagsTest() noexcept {
	return testBitFlagEnumToString<TestBitFlagEnumBase1>("default bit flag enum base 1 to string all flags tests");
}
static_assert(defaultBitFlagEnumBase1ToStringAllFlagsTest());

natl::Bool defaultToStringBitFlagAllFlagsTests() noexcept {
	natl::Test test(natlTestFrom, "default to string bit flag all flags tests", natl::TestType::node);
	natl::subTestAssert(test, defaultBitFlagEnumToStringAllFlagsTest());
	natl::subTestAssert(test, defaultBitFlagEnumBase1ToStringAllFlagsTest());
	return test;
}

constexpr natl::Bool defaultBitFlagEnumFromStringAllFlagsTest() noexcept {
	return testBitFlagEnumFromString<TestBitFlagEnum>("default bit flag enum from string all flags tests");
}
static_assert(defaultBitFlagEnumFromStringAllFlagsTest());
constexpr natl::Bool defaultBitFlagEnumBase1FromStringAllFlagsTest() noexcept {
	return testBitFlagEnumFromString<TestBitFlagEnumBase1>("default bit flag enum base 1 from string all flags tests");
}
static_assert(defaultBitFlagEnumBase1FromStringAllFlagsTest());

natl::Bool defaultFromStringBitFlagAllFlagsTests() noexcept {
	natl::Test test(natlTestFrom, "default from string bit flag all flags tests", natl::TestType::node);
	natl::subTestAssert(test, defaultBitFlagEnumFromStringAllFlagsTest());
	natl::subTestAssert(test, defaultBitFlagEnumBase1FromStringAllFlagsTest());
	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, utilTests());
	natl::subTestAssert(test, defaultEnumValueTests());
	natl::subTestAssert(test, customEnumValueTests());
	natl::subTestAssert(test, defaultToStringTests());
	natl::subTestAssert(test, customToStringTests());
	natl::subTestAssert(test, defaultFromStringTests());
	natl::subTestAssert(test, customFromStringTests());
	natl::subTestAssert(test, defaultToStringBitFlagAllFlagsTests());
	natl::subTestAssert(test, defaultFromStringBitFlagAllFlagsTests());
	return test;
}

int main() {
	tests();
}