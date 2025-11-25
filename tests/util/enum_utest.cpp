//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/enumUtils.h>

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

struct EnumTestSuite : public natl::TestSuite<EnumTestSuite> {};

template<typename test_suite_type = EnumTestSuite>
struct EnumTestSuiteTests {

	template<typename EnumType>
	constexpr static void testEnumValues(test_suite_type& ts) noexcept {
		using enum_values = natl::EnumValues<EnumType>;

		ts.assertEqual(EnumType::value0, enum_values::template at<0>::value);
		ts.assertEqual("value0", enum_values::template at<0>::name.toStringView());
		ts.assertEqual(EnumType::value1, enum_values::template at<1>::value);
		ts.assertEqual("value1", enum_values::template at<1>::name.toStringView());
		ts.assertEqual(EnumType::value2, enum_values::template at<2>::value);
		ts.assertEqual("value2", enum_values::template at<2>::name.toStringView());
		ts.assertEqual(EnumType::value3, enum_values::template at<3>::value);
		ts.assertEqual("value3", enum_values::template at<3>::name.toStringView());
		ts.assertEqual(EnumType::value4, enum_values::template at<4>::value);
		ts.assertEqual("value4", enum_values::template at<4>::name.toStringView());
		ts.assertEqual(EnumType::value5, enum_values::template at<5>::value);
		ts.assertEqual("value5", enum_values::template at<5>::name.toStringView());
		ts.assertEqual(EnumType::value6, enum_values::template at<6>::value);
		ts.assertEqual("value6", enum_values::template at<6>::name.toStringView());
		ts.assertEqual(EnumType::value7, enum_values::template at<7>::value);
		ts.assertEqual("value7", enum_values::template at<7>::name.toStringView());
	}


	template<typename EnumType>
	constexpr static void testEnumToString(test_suite_type& ts) noexcept {
		using enum_values = natl::EnumValues<EnumType>;
		using string_type = natl::String256;

		ts.assertEqual(enum_values::template at<0>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value0));
		ts.assertEqual(enum_values::template at<1>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value1));
		ts.assertEqual(enum_values::template at<2>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value2));
		ts.assertEqual(enum_values::template at<3>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value3));
		ts.assertEqual(enum_values::template at<4>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value4));
		ts.assertEqual(enum_values::template at<5>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value5));
		ts.assertEqual(enum_values::template at<6>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value6));
		ts.assertEqual(enum_values::template at<7>::name.toStringView(), natl::enumToString<EnumType, string_type>(EnumType::value7));
	}

	template<typename EnumType>
	constexpr static void testEnumFromString(test_suite_type& ts) noexcept {
		using enum_values = natl::EnumValues<EnumType>;
		using string_type = natl::String256;

		ts.assertEqual(enum_values::template at<0>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value0)).value());
		ts.assertEqual(enum_values::template at<1>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value1)).value());
		ts.assertEqual(enum_values::template at<2>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value2)).value());
		ts.assertEqual(enum_values::template at<3>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value3)).value());
		ts.assertEqual(enum_values::template at<4>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value4)).value());
		ts.assertEqual(enum_values::template at<5>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value5)).value());
		ts.assertEqual(enum_values::template at<6>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value6)).value());
		ts.assertEqual(enum_values::template at<7>::value,
			natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(EnumType::value7)).value());
	}

	template<typename EnumType>
	struct GeneratedBitFlagEnumInfo {
		EnumType value;
		natl::String256 name;
	};

	template<typename EnumType, typename EnumValueTypes>
	constexpr static GeneratedBitFlagEnumInfo<EnumType> generateTestBitFlagEnumValue(const natl::ui64 seed) noexcept {
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
	constexpr static  void testBitFlagEnumToString(test_suite_type& ts) noexcept {
		using string_type = natl::String256;
		using enum_values = natl::EnumValues<EnumType>;

		natl::String256 nameOfEnum = natl::getNameOfEnum<EnumType>();
		natl::Size maxFlagIndex = static_cast<natl::Size>(natl::bitscanForward(natl::toUnderlying(enum_values::template at<7>::value)));
		natl::Size numIterations = natl::min<natl::Size>(natl::isConstantEvaluated() ? 10 : 0xffffff, (natl::Size(1) << maxFlagIndex));
		for (natl::ui64 i = natl::EnumTraits<EnumType>::startValue; i <= numIterations; i++) {
			GeneratedBitFlagEnumInfo<EnumType> info = generateTestBitFlagEnumValue<EnumType, enum_values>(i);
			ts.assertEqual(info.name, natl::enumToString<EnumType, string_type>(info.value));
		}
	};

	template<typename EnumType>
	constexpr static void testBitFlagEnumFromString(test_suite_type& ts) noexcept {
		using string_type = natl::String256;
		using enum_values = natl::EnumValues<EnumType>;

		natl::String256 nameOfEnum = natl::getNameOfEnum<EnumType>();
		natl::Size numIterations = natl::min<natl::Size>(natl::isConstantEvaluated() ? 10 : 0xffffff, natl::Limits<natl::ui8>::max());

		for (natl::ui64 i = natl::EnumTraits<EnumType>::startValue; i <= numIterations; i++) {
			GeneratedBitFlagEnumInfo<EnumType> info = generateTestBitFlagEnumValue<EnumType, enum_values>(i);
			ts.assertEqual(info.value, natl::enumFromString<EnumType>(natl::enumToString<EnumType, string_type>(info.value)).value());
		}
	};

	constexpr static void utilTest(test_suite_type& ts) noexcept {
		ts.assertEqual("TestSequentialEnum", natl::getNameOfEnum<TestSequentialEnum>());
		ts.assertEqual("value0", natl::getNameOfEnumValue<TestSequentialEnum::value0>());
		ts.assertEqual(true, natl::testIfValidEnumValue<TestSequentialEnum, natl::toUnderlying(TestSequentialEnum::value0)>());
		ts.assertEqual(false, natl::testIfValidEnumValue<TestSequentialEnum, 100>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(utilTest);

	constexpr static void defaultEnumValueTest(test_suite_type& ts) noexcept {
		testEnumValues<TestSequentialEnum>(ts);
		testEnumValues<TestSequentialEnumBase1>(ts);
		testEnumValues<TestBitFlagEnum>(ts);
		testEnumValues<TestBitFlagEnumBase1>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(defaultEnumValueTest);

	constexpr static void customEnumValueTest(test_suite_type& ts) noexcept {
		testEnumValues<TestSequentialEnumGap>(ts);
		testEnumValues<TestBitFlagEnumGap>(ts);
		testEnumValues<TestAllValuesEnum>(ts);
		ts.assertFalse(natl::HasEnumValues<TestNothingEnum>);
	}
	NATL_REGISTER_TEST_CONSTEXPR(customEnumValueTest);

	constexpr static void defaultToStringTest(test_suite_type& ts) noexcept {
		testEnumToString<TestSequentialEnum>(ts);
		testEnumToString<TestSequentialEnumBase1>(ts);
		testEnumToString<TestBitFlagEnum>(ts);
		testEnumToString<TestBitFlagEnumBase1>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(defaultToStringTest);

	constexpr static void customToStringTest(test_suite_type& ts) noexcept {
		testEnumToString<TestSequentialEnumGap>(ts);
		testEnumToString<TestBitFlagEnumGap>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(customToStringTest);

	constexpr static void defaultFromStringTest(test_suite_type& ts) noexcept {
		testEnumFromString<TestSequentialEnum>(ts);
		testEnumFromString<TestSequentialEnumBase1>(ts);
		testEnumFromString<TestBitFlagEnum>(ts);
		testEnumFromString<TestBitFlagEnumBase1>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(defaultFromStringTest);


	constexpr static void customFromStringTest(test_suite_type& ts) noexcept {
		testEnumFromString<TestSequentialEnumGap>(ts);
		testEnumFromString<TestBitFlagEnumGap>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(customFromStringTest);

	constexpr static void defaultToStringBitFlagAllFlagsTest(test_suite_type& ts) noexcept {
		testBitFlagEnumToString<TestBitFlagEnum>(ts);
		testBitFlagEnumToString<TestBitFlagEnumBase1>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(defaultToStringBitFlagAllFlagsTest);

	constexpr static void defaultFromStringBitFlagAllFlagsTest(test_suite_type& ts) noexcept {
		testBitFlagEnumFromString<TestBitFlagEnum>(ts);
		testBitFlagEnumFromString<TestBitFlagEnumBase1>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(defaultFromStringBitFlagAllFlagsTest);
};

NATL_REGISTER_STD_TESTS(EnumTestSuite);

NATL_TESTS_MAIN();
