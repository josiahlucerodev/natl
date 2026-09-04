//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/container/variant.h>
#include <natl/util/test.h>

struct VariantTestSuite : public natl::TestSuite<VariantTestSuite> {};

template<typename test_suite_type = VariantTestSuite>
struct VariantTestSuiteTests {
	using TestType = natl::Variant<
		natl::i8,
		natl::i64,
		natl::ui8,
		natl::ui64,
		natl::f32,
		natl::f64
	>;

	template<natl::Size vIndex, typename ValueType>
	constexpr static void assignAndCheck(
		test_suite_type& ts,
		TestType& value,
		const ValueType& first,
		const ValueType& second) noexcept {
		value.template assign<vIndex>(first);
		ts.assertEqual(first, value.template get<vIndex>());
		ts.assertEqual(natl::Size(vIndex), value.getIndex());
		ts.assertEqual(true, value.template isIndex<vIndex>());

		value.template assign<vIndex>(second);
		ts.assertEqual(second, value.template get<vIndex>());
	}

	constexpr static void defaultState(test_suite_type& ts) noexcept {
		TestType value;
		ts.assertEqual(true, value.doesNotHaveValue());
		ts.assertEqual(false, value.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(defaultState);

	constexpr static void assignAndAccess(test_suite_type& ts) noexcept {
		TestType value;

		assignAndCheck<0>(ts, value, natl::Limits<natl::i8>::min(), natl::Limits<natl::i8>::max());
		assignAndCheck<1>(ts, value, natl::Limits<natl::i64>::min(), natl::Limits<natl::i64>::max());
		assignAndCheck<2>(ts, value, natl::Limits<natl::ui8>::min(), natl::Limits<natl::ui8>::max());
		assignAndCheck<3>(ts, value, natl::Limits<natl::ui64>::min(), natl::Limits<natl::ui64>::max());
		assignAndCheck<4>(ts, value, natl::Limits<natl::f32>::min(), natl::Limits<natl::f32>::max());
		assignAndCheck<5>(ts, value, natl::Limits<natl::f64>::min(), natl::Limits<natl::f64>::max());
	}
	NATL_REGISTER_TEST_CONSTEXPR(assignAndAccess);

	constexpr static void typeAndIndexState(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Size(0), TestType::getIndexOf<natl::i8>());
		ts.assertEqual(natl::Size(1), TestType::getIndexOf<natl::i64>());
		ts.assertEqual(natl::Size(2), TestType::getIndexOf<natl::ui8>());
		ts.assertEqual(natl::Size(3), TestType::getIndexOf<natl::ui64>());
		ts.assertEqual(natl::Size(4), TestType::getIndexOf<natl::f32>());
		ts.assertEqual(natl::Size(5), TestType::getIndexOf<natl::f64>());

		TestType value;
		value.assign(natl::i64(42));
		ts.assertEqual(true, value.hasValue());
		ts.assertEqual(false, value.doesNotHaveValue());
		ts.assertEqual(true, value.isValue<natl::i64>());
		ts.assertEqual(false, value.isValue<natl::f64>());
		ts.assertEqual(true, value.isIndex<1>());
		ts.assertEqual(natl::Size(1), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(typeAndIndexState);

	constexpr static void valueConstructionAndAssignment(test_suite_type& ts) noexcept {
		TestType value(natl::i64(10));
		ts.assertEqual(true, value.isValue<natl::i64>());
		ts.assertEqual(natl::i64(10), value.get<1>());

		value = natl::f64(20.5);
		ts.assertEqual(true, value.isValue<natl::f64>());
		ts.assertEqual(natl::f64(20.5), value.get<5>());

		const natl::i8 source = natl::i8(-7);
		value = source;
		ts.assertEqual(true, value.isValue<natl::i8>());
		ts.assertEqual(source, value.get<0>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(valueConstructionAndAssignment);

	constexpr static void copyAndMoveConstruction(test_suite_type& ts) noexcept {
		TestType empty;
		TestType emptyCopy(empty);
		ts.assertEqual(false, emptyCopy.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), emptyCopy.getIndex());

		TestType source(natl::i64(123456789));
		TestType copy(source);
		ts.assertEqual(natl::i64(123456789), copy.get<1>());
		source.assign(natl::i64(1));
		ts.assertEqual(natl::i64(123456789), copy.get<1>());

		TestType moveSource(natl::ui64(987654321));
		TestType moved(natl::move(moveSource));
		ts.assertEqual(true, moved.isValue<natl::ui64>());
		ts.assertEqual(natl::ui64(987654321), moved.get<3>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(copyAndMoveConstruction);

	constexpr static void copyAndMoveAssignment(test_suite_type& ts) noexcept {
		TestType lhs(natl::i64(100));
		TestType sameType(natl::i64(200));
		lhs = sameType;
		ts.assertEqual(natl::i64(200), lhs.get<1>());

		TestType differentType(natl::f64(123.5));
		lhs = differentType;
		ts.assertEqual(true, lhs.isValue<natl::f64>());
		ts.assertEqual(natl::f64(123.5), lhs.get<5>());

		TestType moveSource(natl::ui64(999));
		lhs = natl::move(moveSource);
		ts.assertEqual(true, lhs.isValue<natl::ui64>());
		ts.assertEqual(natl::ui64(999), lhs.get<3>());

		TestType empty;
		lhs = empty;
		ts.assertEqual(false, lhs.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), lhs.getIndex());

		lhs.assign(natl::i8(1));
		lhs = natl::move(empty);
		ts.assertEqual(false, lhs.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), lhs.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(copyAndMoveAssignment);

	constexpr static void constGet(test_suite_type& ts) noexcept {
		TestType value(natl::i64(123));
		const TestType& constValue = value;
		ts.assertEqual(natl::i64(123), constValue.get<1>());
		ts.assertEqual(natl::Size(1), constValue.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(constGet);
};

struct NamedVariantTestSuite : public natl::TestSuite<NamedVariantTestSuite> {};

template<typename test_suite_type = NamedVariantTestSuite>
struct NamedVariantTestSuiteTests {
	using TestType = natl::NamedVariant<
		natl::NamedElement<"i8", natl::i8>,
		natl::NamedElement<"i64", natl::i64>,
		natl::NamedElement<"ui8", natl::ui8>,
		natl::NamedElement<"ui64", natl::ui64>,
		natl::NamedElement<"f32", natl::f32>,
		natl::NamedElement<"f64", natl::f64>
	>;

	template<natl::TemplateStringLiteral vName, natl::Size vIndex, typename ValueType>
	constexpr static void assignByNameAndCheck(
		test_suite_type& ts,
		TestType& value,
		const ValueType& first,
		const ValueType& second
	) noexcept {
		value.template assign<vName>(first);
		ts.assertEqual(first, value.template get<vName>());
		ts.assertEqual(true, value.template isValue<vName>());
		ts.assertEqual(true, value.template isIndex<vIndex>());
		ts.assertEqual(natl::Size(vIndex), value.getIndex());

		value.template assign<vName>(second);
		ts.assertEqual(second, value.template get<vName>());
	}

	constexpr static void defaultState(test_suite_type& ts) noexcept {
		TestType value;
		ts.assertEqual(true, value.doesNotHaveValue());
		ts.assertEqual(false, value.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(defaultState);

	constexpr static void assignByName(test_suite_type& ts) noexcept {
		TestType value;

		assignByNameAndCheck<"i8", 0>(ts, value, natl::Limits<natl::i8>::min(), natl::Limits<natl::i8>::max());
		assignByNameAndCheck<"i64", 1>(ts, value, natl::Limits<natl::i64>::min(), natl::Limits<natl::i64>::max());
		assignByNameAndCheck<"ui8", 2>(ts, value, natl::Limits<natl::ui8>::min(), natl::Limits<natl::ui8>::max());
		assignByNameAndCheck<"ui64", 3>(ts, value, natl::Limits<natl::ui64>::min(), natl::Limits<natl::ui64>::max());
		assignByNameAndCheck<"f32", 4>(ts, value, natl::Limits<natl::f32>::min(), natl::Limits<natl::f32>::max());
		assignByNameAndCheck<"f64", 5>(ts, value, natl::Limits<natl::f64>::min(), natl::Limits<natl::f64>::max());
	}
	NATL_REGISTER_TEST_CONSTEXPR(assignByName);

	constexpr static void assignByIndex(test_suite_type& ts) noexcept {
		TestType value;

		value.assign<0>(natl::i8(12));
		ts.assertEqual(natl::i8(12), value.get<0>());
		value.assign<1>(natl::i64(123456));
		ts.assertEqual(natl::i64(123456), value.get<1>());
		value.assign<2>(natl::ui8(200));
		ts.assertEqual(natl::ui8(200), value.get<2>());
		value.assign<3>(natl::ui64(123456789));
		ts.assertEqual(natl::ui64(123456789), value.get<3>());
		value.assign<4>(natl::f32(12.5f));
		ts.assertEqual(natl::f32(12.5f), value.get<4>());
		value.assign<5>(natl::f64(123.25));
		ts.assertEqual(natl::f64(123.25), value.get<5>());
		ts.assertEqual(natl::Size(5), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(assignByIndex);

	constexpr static void nameAndIndexState(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Size(0), TestType::getIndexOf<"i8">());
		ts.assertEqual(natl::Size(1), TestType::getIndexOf<"i64">());
		ts.assertEqual(natl::Size(2), TestType::getIndexOf<"ui8">());
		ts.assertEqual(natl::Size(3), TestType::getIndexOf<"ui64">());
		ts.assertEqual(natl::Size(4), TestType::getIndexOf<"f32">());
		ts.assertEqual(natl::Size(5), TestType::getIndexOf<"f64">());

		TestType value;
		value.assign<"i8">(natl::i8(1));
		ts.assertEqual(true, value.hasValue());
		ts.assertEqual(true, value.isValue<"i8">());
		ts.assertEqual(false, value.isValue<"i64">());
		ts.assertEqual(true, value.isIndex<0>());
		ts.assertEqual(natl::Size(0), value.getIndex());

		value.assign<"f64">(natl::f64(2.0));
		ts.assertEqual(true, value.isValue<"f64">());
		ts.assertEqual(false, value.isValue<"i8">());
		ts.assertEqual(true, value.isIndex<5>());
		ts.assertEqual(natl::Size(5), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(nameAndIndexState);

	constexpr static void copyAndMoveConstruction(test_suite_type& ts) noexcept {
		TestType empty;
		TestType emptyCopy(empty);
		ts.assertEqual(false, emptyCopy.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), emptyCopy.getIndex());

		TestType source;
		source.assign<"i64">(natl::i64(123456789));
		TestType copy(source);
		ts.assertEqual(natl::i64(123456789), copy.get<"i64">());
		source.assign<"i64">(natl::i64(1));
		ts.assertEqual(natl::i64(123456789), copy.get<"i64">());

		TestType moveSource;
		moveSource.assign<"ui64">(natl::ui64(987654321));
		TestType moved(natl::move(moveSource));
		ts.assertEqual(true, moved.isValue<"ui64">());
		ts.assertEqual(natl::ui64(987654321), moved.get<"ui64">());
	}
	NATL_REGISTER_TEST_CONSTEXPR(copyAndMoveConstruction);

	constexpr static void copyAndMoveAssignment(test_suite_type& ts) noexcept {
		TestType lhs;
		TestType rhs;

		lhs.assign<"i64">(natl::i64(100));
		rhs.assign<"i64">(natl::i64(200));
		lhs = rhs;
		ts.assertEqual(natl::i64(200), lhs.get<"i64">());

		rhs.assign<"f64">(natl::f64(100.5));
		lhs = rhs;
		ts.assertEqual(true, lhs.isValue<"f64">());
		ts.assertEqual(natl::f64(100.5), lhs.get<"f64">());

		TestType moveSource;
		moveSource.assign<"ui64">(natl::ui64(999));
		lhs = natl::move(moveSource);
		ts.assertEqual(true, lhs.isValue<"ui64">());
		ts.assertEqual(natl::ui64(999), lhs.get<"ui64">());

		TestType empty;
		lhs = empty;
		ts.assertEqual(false, lhs.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), lhs.getIndex());

		lhs.assign<"i8">(natl::i8(1));
		lhs = natl::move(empty);
		ts.assertEqual(false, lhs.hasValue());
		ts.assertEqual(TestType::getIndexOfEmpty(), lhs.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(copyAndMoveAssignment);

	constexpr static void constGet(test_suite_type& ts) noexcept {
		TestType value;
		value.assign<"i64">(natl::i64(123));
		const TestType& constValue = value;
		ts.assertEqual(natl::i64(123), constValue.get<"i64">());
		ts.assertEqual(natl::i64(123), constValue.get<1>());
		ts.assertEqual(natl::Size(1), constValue.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(constGet);
};

NATL_REGISTER_STD_TESTS(VariantTestSuite);
NATL_REGISTER_STD_TESTS(NamedVariantTestSuite);

NATL_TESTS_MAIN();
