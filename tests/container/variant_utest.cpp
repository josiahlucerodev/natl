//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/container/variant.h>
#include <natl/container/dynArray.h>
#include <natl/util/test.h>

struct VariantTestSuite : public natl::TestSuite<VariantTestSuite> {};

template<typename test_suite_type = VariantTestSuite>
struct VariantTestSuiteTests {
	using NamedVariantTestType = natl::NamedVariant<
		natl::NamedElement<"i8", natl::i8>,
		natl::NamedElement<"i64", natl::i64>,
		natl::NamedElement<"ui8", natl::ui8>,
		natl::NamedElement<"ui64", natl::ui64>,
		natl::NamedElement<"f32", natl::f32>,
		natl::NamedElement<"f64", natl::f64>
	>;

	using VariantTestType = natl::Variant<
		natl::i8,
		natl::i64,
		natl::ui8,
		natl::ui64,
		natl::f32,
		natl::f64
	>;

	constexpr static void namedVariantDefault(test_suite_type& ts) noexcept {
		NamedVariantTestType value;

		ts.assertEqual(true, value.doesNotHaveValue());
		ts.assertEqual(false, value.hasValue());
		ts.assertEqual(NamedVariantTestType::getIndexOfEmpty(), value.getIndex());
		ts.assertEqual(natl::Size(0), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantDefault);

	constexpr static void namedVariantAssignByName(test_suite_type& ts) noexcept {
		NamedVariantTestType value;

		value.assign<"i8">(natl::Limits<natl::i8>::min());
		ts.assertEqual(natl::Limits<natl::i8>::min(), value.get<"i8">());
		value.assign<"i8">(natl::Limits<natl::i8>::max());
		ts.assertEqual(natl::Limits<natl::i8>::max(), value.get<"i8">());

		value.assign<"i64">(natl::Limits<natl::i64>::min());
		ts.assertEqual(natl::Limits<natl::i64>::min(), value.get<"i64">());
		value.assign<"i64">(natl::Limits<natl::i64>::max());
		ts.assertEqual(natl::Limits<natl::i64>::max(), value.get<"i64">());

		value.assign<"ui8">(natl::Limits<natl::ui8>::min());
		ts.assertEqual(natl::Limits<natl::ui8>::min(), value.get<"ui8">());
		value.assign<"ui8">(natl::Limits<natl::ui8>::max());
		ts.assertEqual(natl::Limits<natl::ui8>::max(), value.get<"ui8">());

		value.assign<"ui64">(natl::Limits<natl::ui64>::min());
		ts.assertEqual(natl::Limits<natl::ui64>::min(), value.get<"ui64">());
		value.assign<"ui64">(natl::Limits<natl::ui64>::max());
		ts.assertEqual(natl::Limits<natl::ui64>::max(), value.get<"ui64">());

		value.assign<"f32">(natl::Limits<natl::f32>::min());
		ts.assertEqual(natl::Limits<natl::f32>::min(), value.get<"f32">());
		value.assign<"f32">(natl::Limits<natl::f32>::max());
		ts.assertEqual(natl::Limits<natl::f32>::max(), value.get<"f32">());

		value.assign<"f64">(natl::Limits<natl::f64>::min());
		ts.assertEqual(natl::Limits<natl::f64>::min(), value.get<"f64">());
		value.assign<"f64">(natl::Limits<natl::f64>::max());
		ts.assertEqual(natl::Limits<natl::f64>::max(), value.get<"f64">());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantAssignByName);

	constexpr static void namedVariantState(test_suite_type& ts) noexcept {
		NamedVariantTestType value;

		value.assign<"i8">(natl::i8(1));
		ts.assertEqual(true, value.hasValue());
		ts.assertEqual(false, value.doesNotHaveValue());
		ts.assertEqual(true, value.isValue<"i8">());
		ts.assertEqual(false, value.isValue<"i64">());
		ts.assertEqual(NamedVariantTestType::getIndexOf<"i8">(), value.getIndex());

		value.assign<"f64">(natl::f64(2.0));
		ts.assertEqual(true, value.isValue<"f64">());
		ts.assertEqual(false, value.isValue<"i8">());
		ts.assertEqual(NamedVariantTestType::getIndexOf<"f64">(), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantState);

	constexpr static void namedVariantIndexMapping(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Size(1), NamedVariantTestType::getIndexOf<"i8">());
		ts.assertEqual(natl::Size(2), NamedVariantTestType::getIndexOf<"i64">());
		ts.assertEqual(natl::Size(3), NamedVariantTestType::getIndexOf<"ui8">());
		ts.assertEqual(natl::Size(4), NamedVariantTestType::getIndexOf<"ui64">());
		ts.assertEqual(natl::Size(5), NamedVariantTestType::getIndexOf<"f32">());
		ts.assertEqual(natl::Size(6), NamedVariantTestType::getIndexOf<"f64">());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantIndexMapping);

	constexpr static void namedVariantAssignByIndex(test_suite_type& ts) noexcept {
		NamedVariantTestType value;

		value.assign<0>(natl::i8(12));
		ts.assertEqual(natl::i8(12), value.get<0>());
		ts.assertEqual(natl::Size(1), value.getIndex());

		value.assign<1>(natl::i64(123456));
		ts.assertEqual(natl::i64(123456), value.get<1>());
		ts.assertEqual(natl::Size(2), value.getIndex());

		value.assign<2>(natl::ui8(200));
		ts.assertEqual(natl::ui8(200), value.get<2>());
		ts.assertEqual(natl::Size(3), value.getIndex());

		value.assign<3>(natl::ui64(123456789));
		ts.assertEqual(natl::ui64(123456789), value.get<3>());
		ts.assertEqual(natl::Size(4), value.getIndex());

		value.assign<4>(natl::f32(12.5f));
		ts.assertEqual(natl::f32(12.5f), value.get<4>());
		ts.assertEqual(natl::Size(5), value.getIndex());

		value.assign<5>(natl::f64(123.25));
		ts.assertEqual(natl::f64(123.25), value.get<5>());
		ts.assertEqual(natl::Size(6), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantAssignByIndex);

	constexpr static void namedVariantSameTypeReassign(test_suite_type& ts) noexcept {
		NamedVariantTestType value;

		value.assign<"i64">(natl::i64(10));
		ts.assertEqual(natl::i64(10), value.get<"i64">());

		value.assign<"i64">(natl::i64(20));
		ts.assertEqual(natl::i64(20), value.get<"i64">());

		value.assign<"i64">(natl::i64(-30));
		ts.assertEqual(natl::i64(-30), value.get<"i64">());
		ts.assertEqual(NamedVariantTestType::getIndexOf<"i64">(), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantSameTypeReassign);

	constexpr static void namedVariantCopyConstruct(test_suite_type& ts) noexcept {
		NamedVariantTestType value;
		value.assign<"i64">(natl::i64(123456789));

		NamedVariantTestType copy(value);

		ts.assertEqual(true, copy.hasValue());
		ts.assertEqual(value.getIndex(), copy.getIndex());
		ts.assertEqual(natl::i64(123456789), copy.get<"i64">());

		value.assign<"i64">(natl::i64(1));

		ts.assertEqual(natl::i64(1), value.get<"i64">());
		ts.assertEqual(natl::i64(123456789), copy.get<"i64">());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantCopyConstruct);

	constexpr static void namedVariantCopyConstructEmpty(test_suite_type& ts) noexcept {
		NamedVariantTestType value;
		NamedVariantTestType copy(value);

		ts.assertEqual(false, copy.hasValue());
		ts.assertEqual(true, copy.doesNotHaveValue());
		ts.assertEqual(natl::Size(0), copy.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantCopyConstructEmpty);

	constexpr static void namedVariantCopyAssignSameType(test_suite_type& ts) noexcept {
		NamedVariantTestType lhs;
		NamedVariantTestType rhs;

		lhs.assign<"i64">(natl::i64(100));
		rhs.assign<"i64">(natl::i64(200));

		lhs = rhs;

		ts.assertEqual(natl::i64(200), lhs.get<"i64">());
		ts.assertEqual(rhs.getIndex(), lhs.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantCopyAssignSameType);

	constexpr static void namedVariantCopyAssignDifferentType(test_suite_type& ts) noexcept {
		NamedVariantTestType lhs;
		NamedVariantTestType rhs;

		lhs.assign<"i8">(natl::i8(10));
		rhs.assign<"f64">(natl::f64(100.5));

		lhs = rhs;

		ts.assertEqual(true, lhs.isValue<"f64">());
		ts.assertEqual(natl::f64(100.5), lhs.get<"f64">());
		ts.assertEqual(rhs.getIndex(), lhs.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantCopyAssignDifferentType);

	constexpr static void namedVariantAssignEmpty(test_suite_type& ts) noexcept {
		NamedVariantTestType lhs;
		NamedVariantTestType rhs;

		lhs.assign<"i64">(natl::i64(100));
		lhs = rhs;

		ts.assertEqual(false, lhs.hasValue());
		ts.assertEqual(true, lhs.doesNotHaveValue());
		ts.assertEqual(natl::Size(0), lhs.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantAssignEmpty);

	constexpr static void namedVariantConstGet(test_suite_type& ts) noexcept {
		NamedVariantTestType value;
		value.assign<"i64">(natl::i64(123));

		const NamedVariantTestType& constValue = value;

		ts.assertEqual(natl::i64(123), constValue.get<"i64">());
		ts.assertEqual(NamedVariantTestType::getIndexOf<"i64">(), constValue.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(namedVariantConstGet);

	constexpr static void variantDefault(test_suite_type& ts) noexcept {
		VariantTestType value;

		ts.assertEqual(true, value.doesNotHaveValue());
		ts.assertEqual(false, value.hasValue());
		ts.assertEqual(VariantTestType::getIndexOfEmpty(), value.getIndex());
		ts.assertEqual(natl::Size(0), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantDefault);

	constexpr static void variantAssign(test_suite_type& ts) noexcept {
		VariantTestType value;

		value.assign<0>(natl::Limits<natl::i8>::min());
		ts.assertEqual(natl::Limits<natl::i8>::min(), value.get<0>());
		value.assign<0>(natl::Limits<natl::i8>::max());
		ts.assertEqual(natl::Limits<natl::i8>::max(), value.get<0>());

		value.assign<1>(natl::Limits<natl::i64>::min());
		ts.assertEqual(natl::Limits<natl::i64>::min(), value.get<1>());
		value.assign<1>(natl::Limits<natl::i64>::max());
		ts.assertEqual(natl::Limits<natl::i64>::max(), value.get<1>());

		value.assign<2>(natl::Limits<natl::ui8>::min());
		ts.assertEqual(natl::Limits<natl::ui8>::min(), value.get<2>());
		value.assign<2>(natl::Limits<natl::ui8>::max());
		ts.assertEqual(natl::Limits<natl::ui8>::max(), value.get<2>());

		value.assign<3>(natl::Limits<natl::ui64>::min());
		ts.assertEqual(natl::Limits<natl::ui64>::min(), value.get<3>());
		value.assign<3>(natl::Limits<natl::ui64>::max());
		ts.assertEqual(natl::Limits<natl::ui64>::max(), value.get<3>());

		value.assign<4>(natl::Limits<natl::f32>::min());
		ts.assertEqual(natl::Limits<natl::f32>::min(), value.get<4>());
		value.assign<4>(natl::Limits<natl::f32>::max());
		ts.assertEqual(natl::Limits<natl::f32>::max(), value.get<4>());

		value.assign<5>(natl::Limits<natl::f64>::min());
		ts.assertEqual(natl::Limits<natl::f64>::min(), value.get<5>());
		value.assign<5>(natl::Limits<natl::f64>::max());
		ts.assertEqual(natl::Limits<natl::f64>::max(), value.get<5>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantAssign);

	constexpr static void variantIndex(test_suite_type& ts) noexcept {
		VariantTestType value;

		value.assign<0>(natl::i8(1));
		ts.assertEqual(natl::Size(1), value.getIndex());

		value.assign<1>(natl::i64(2));
		ts.assertEqual(natl::Size(2), value.getIndex());

		value.assign<2>(natl::ui8(3));
		ts.assertEqual(natl::Size(3), value.getIndex());

		value.assign<3>(natl::ui64(4));
		ts.assertEqual(natl::Size(4), value.getIndex());

		value.assign<4>(natl::f32(5));
		ts.assertEqual(natl::Size(5), value.getIndex());

		value.assign<5>(natl::f64(6));
		ts.assertEqual(natl::Size(6), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantIndex);

	constexpr static void variantSameTypeReassign(test_suite_type& ts) noexcept {
		VariantTestType value;

		value.assign<1>(natl::i64(10));
		ts.assertEqual(natl::i64(10), value.get<1>());

		value.assign<1>(natl::i64(20));
		ts.assertEqual(natl::i64(20), value.get<1>());

		value.assign<1>(natl::i64(-30));
		ts.assertEqual(natl::i64(-30), value.get<1>());
		ts.assertEqual(natl::Size(2), value.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantSameTypeReassign);

	constexpr static void variantSwitchType(test_suite_type& ts) noexcept {
		VariantTestType value;

		value.assign<0>(natl::i8(10));
		ts.assertEqual(natl::Size(1), value.getIndex());
		ts.assertEqual(natl::i8(10), value.get<0>());

		value.assign<5>(natl::f64(64.5));
		ts.assertEqual(natl::Size(6), value.getIndex());
		ts.assertEqual(natl::f64(64.5), value.get<5>());

		value.assign<2>(natl::ui8(42));
		ts.assertEqual(natl::Size(3), value.getIndex());
		ts.assertEqual(natl::ui8(42), value.get<2>());

		value.assign<3>(natl::ui64(999999));
		ts.assertEqual(natl::Size(4), value.getIndex());
		ts.assertEqual(natl::ui64(999999), value.get<3>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantSwitchType);

	constexpr static void variantCopyConstruct(test_suite_type& ts) noexcept {
		VariantTestType value;
		value.assign<1>(natl::i64(123456789));

		VariantTestType copy(value);

		ts.assertEqual(true, copy.hasValue());
		ts.assertEqual(value.getIndex(), copy.getIndex());
		ts.assertEqual(natl::i64(123456789), copy.get<1>());

		value.assign<1>(natl::i64(1));

		ts.assertEqual(natl::i64(1), value.get<1>());
		ts.assertEqual(natl::i64(123456789), copy.get<1>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantCopyConstruct);

	constexpr static void variantCopyConstructEmpty(test_suite_type& ts) noexcept {
		VariantTestType value;
		VariantTestType copy(value);

		ts.assertEqual(false, copy.hasValue());
		ts.assertEqual(true, copy.doesNotHaveValue());
		ts.assertEqual(natl::Size(0), copy.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantCopyConstructEmpty);

	constexpr static void variantMoveConstruct(test_suite_type& ts) noexcept {
		VariantTestType value;
		value.assign<3>(natl::ui64(987654321));

		VariantTestType moved(natl::move(value));

		ts.assertEqual(true, moved.hasValue());
		ts.assertEqual(natl::Size(4), moved.getIndex());
		ts.assertEqual(natl::ui64(987654321), moved.get<3>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantMoveConstruct);

	constexpr static void variantCopyAssignSameType(test_suite_type& ts) noexcept {
		VariantTestType lhs;
		VariantTestType rhs;

		lhs.assign<1>(natl::i64(100));
		rhs.assign<1>(natl::i64(200));

		lhs = rhs;

		ts.assertEqual(natl::i64(200), lhs.get<1>());
		ts.assertEqual(rhs.getIndex(), lhs.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantCopyAssignSameType);

	constexpr static void variantCopyAssignDifferentType(test_suite_type& ts) noexcept {
		VariantTestType lhs;
		VariantTestType rhs;

		lhs.assign<0>(natl::i8(100));
		rhs.assign<5>(natl::f64(123.5));

		lhs = rhs;

		ts.assertEqual(natl::Size(6), lhs.getIndex());
		ts.assertEqual(natl::f64(123.5), lhs.get<5>());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantCopyAssignDifferentType);

	constexpr static void variantAssignEmpty(test_suite_type& ts) noexcept {
		VariantTestType lhs;
		VariantTestType rhs;

		lhs.assign<1>(natl::i64(100));
		lhs = rhs;

		ts.assertEqual(false, lhs.hasValue());
		ts.assertEqual(true, lhs.doesNotHaveValue());
		ts.assertEqual(natl::Size(0), lhs.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantAssignEmpty);

	constexpr static void variantConstGet(test_suite_type& ts) noexcept {
		VariantTestType value;
		value.assign<1>(natl::i64(123));

		const VariantTestType& constValue = value;

		ts.assertEqual(natl::i64(123), constValue.get<1>());
		ts.assertEqual(natl::Size(2), constValue.getIndex());
	}
	NATL_REGISTER_TEST_CONSTEXPR(variantConstGet);
};

NATL_REGISTER_STD_TESTS(VariantTestSuite);

NATL_TESTS_MAIN();
