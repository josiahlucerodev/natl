//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/mixin.h>

struct AlgorithmTestSuite : public natl::TestSuite<AlgorithmTestSuite> {};

struct StructMembers1 : 
	natl::AggregateConstructorMixin<StructMembers1>,
	natl::ComparableMixin<StructMembers1>, natl::HashMixin<StructMembers1> {
	natl::String m1;
};

static_assert(natl::IsMixinC<natl::AggregateConstructorMixin<StructMembers1>>);
static_assert(natl::IsMixinC<natl::EqualityMixin<StructMembers1>>);
static_assert(natl::IsMixinC<natl::HashMixin<StructMembers1>>);

struct StructMembers2 : 
	natl::AggregateConstructorMixin<StructMembers2>,
	natl::ComparableMixin<StructMembers2>, natl::HashMixin<StructMembers2> {
	natl::String m1; natl::i8 m2;
};
struct StructMembers3 : 
	natl::AggregateConstructorMixin<StructMembers3>,
	natl::ComparableMixin<StructMembers3>, natl::HashMixin<StructMembers3> {
	natl::String m1; natl::i8 m2; natl::i16 m3;
};
struct StructMembers4 : 
	natl::AggregateConstructorMixin<StructMembers4>,
	natl::ComparableMixin<StructMembers4>, natl::HashMixin<StructMembers4> {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4;
};


struct MixinTestSuite : public natl::TestSuite<MixinTestSuite> {};

using namespace natl::literals;

template<typename test_suite_type = AlgorithmTestSuite>
struct MixinTestSuiteTests {

	constexpr static void equalityMixinTests(test_suite_type& ts) noexcept {
		{
			StructMembers1 sm1_1 = StructMembers1::construct("1");
			StructMembers1 sm1_2 = StructMembers1::construct("1");
			StructMembers1 sm1_3 = StructMembers1::construct("2");
			ts.tassert(sm1_1 == sm1_2);
			ts.tassert(sm1_1 != sm1_3);
		}

		{
			StructMembers2 sm2_1 = StructMembers2::construct("1", 2_i8);
			StructMembers2 sm2_2 = StructMembers2::create("1", 2_i8);
			StructMembers2 sm2_3 = StructMembers2::create("1", 3_i8);
			ts.tassert(sm2_1 == sm2_2);
			ts.tassert(sm2_1 != sm2_3);
		}

		{
			StructMembers3 sm3_1 = StructMembers3::construct("1", 2_i8, 3_i16);
			StructMembers3 sm3_2 = StructMembers3::construct("1", 2_i8, 3_i16);
			StructMembers3 sm3_3 = StructMembers3::construct("1", 2_i8, 4_i16);
			ts.tassert(sm3_1 == sm3_2);
			ts.tassert(sm3_1 != sm3_3);
		}

		{
			StructMembers4 sm4_1 = StructMembers4::construct("1", 2_i8, 3_i16, 4_i32);
			StructMembers4 sm4_2 = StructMembers4::construct("1", 2_i8, 3_i16, 4_i32);
			StructMembers4 sm4_3 = StructMembers4::construct("1", 2_i8, 3_i16, 5_i32);
			ts.tassert(sm4_1 == sm4_2);
			ts.tassert(sm4_1 != sm4_3);
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(equalityMixinTests);

	constexpr static void hashMixinTests(test_suite_type& ts) noexcept {
		StructMembers1 sm1 = StructMembers1::construct("1");
		StructMembers2 sm2 = StructMembers2::construct("1", 2_i8);
		StructMembers3 sm3 = StructMembers3::construct("1", 2_i8, 3_i16);
		StructMembers4 sm4 = StructMembers4::construct("1", 2_i8, 3_i16, 4_i32);

		ts.assertNotEqual(0, sm1.hash());
		ts.assertNotEqual(0, sm2.hash());
		ts.assertNotEqual(0, sm3.hash());
		ts.assertNotEqual(0, sm4.hash());

		ts.assertNotEqual(0, natl::hashValue(sm1));
		ts.assertNotEqual(0, natl::hashValue(sm2));
		ts.assertNotEqual(0, natl::hashValue(sm3));
		ts.assertNotEqual(0, natl::hashValue(sm4));

		ts.assertEqual(sm1.hash(), natl::hashValue(sm1));
		ts.assertEqual(sm2.hash(), natl::hashValue(sm2));
		ts.assertEqual(sm3.hash(), natl::hashValue(sm3));
		ts.assertEqual(sm4.hash(), natl::hashValue(sm4));
	}
	NATL_REGISTER_TEST_CONSTEXPR(hashMixinTests);

	constexpr static void orderingMixinTests(test_suite_type& ts) noexcept {
		auto preformTests = [&] <typename Type>(const Type & sm1, const Type & sm2) {
			const Type sm3 = sm1;
			ts.tassert(sm1 == sm3);
			ts.tassert(not (sm1 == sm2));
			ts.tassert(sm1 != sm2);
			ts.tassert(not (sm1 != sm3));

			ts.tassert(sm1 < sm2);
			ts.tassert(not (sm2 < sm1));
			ts.tassert(sm1 <= sm1);
			ts.tassert(sm1 <= sm3);
			ts.tassert(not (sm2 <= sm1));

			ts.tassert(sm2 > sm1);
			ts.tassert(not (sm1 > sm2));
			ts.tassert(sm2 >= sm2);
			ts.tassert(sm2 >= sm3);
			ts.tassert(not (sm1 >= sm2));
		};

		preformTests(StructMembers1::construct("1"), StructMembers1::construct("2"));
		preformTests(StructMembers2::construct("1", 2_i8), StructMembers2::construct("1", 3_i8));
		preformTests(StructMembers3::construct("1", 2_i8, 3_i16), StructMembers3::construct("1", 2_i8, 4_i16));
		preformTests(StructMembers4::construct("1", 2_i8, 3_i16, 4_i32), StructMembers4::construct("1", 2_i8, 3_i16, 5_i32));
	}
	NATL_REGISTER_TEST_CONSTEXPR(orderingMixinTests);
};

NATL_REGISTER_STD_TESTS(MixinTestSuite);

NATL_TESTS_MAIN();
