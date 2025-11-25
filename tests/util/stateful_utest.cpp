//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
#include <natl/util/stateful.h>
//@end_non_modules

//natl
#include <natl/util/test.h>

struct StatefulTestSuite : public natl::TestSuite<StatefulTestSuite> {};

NATL_DEFINE_STATEFUL_COUNTER(TestCounter);
NATL_DEFINE_STATEFUL_TYPEPACK(TestTypePack);

constexpr void statefulTypePackTestImpl(StatefulTestSuite& ts) noexcept {
	ts.assertEqual(natl::Size(0), TestTypePack<>::size);
	TestTypePackAppend<natl::i8>();
	ts.assertEqual(natl::Size(1), TestTypePack<>::size);
	ts.assertEqual(natl::getNameOfType<natl::TypePack<natl::i8>>(), natl::getNameOfType<TestTypePack<>>());
	TestTypePackAppend<natl::i16>();
	ts.assertEqual(natl::Size(2), TestTypePack<>::size);
	ts.assertEqual(natl::getNameOfType<natl::TypePack<natl::i8, natl::i16>>(), natl::getNameOfType<TestTypePack<>>());
	TestTypePackAppend<natl::i32>();
	ts.assertEqual(natl::Size(3), TestTypePack<>::size);
	ts.assertEqual(natl::getNameOfType<natl::TypePack<natl::i8, natl::i16, natl::i32>>(), natl::getNameOfType<TestTypePack<>>());
	TestTypePackAppend<natl::i64>();
	ts.assertEqual(natl::Size(4), TestTypePack<>::size);
	ts.assertEqual(natl::getNameOfType<natl::TypePack<natl::i8, natl::i16, natl::i32, natl::i64>>(), natl::getNameOfType<TestTypePack<>>());
}

template<typename test_suite_type = StatefulTestSuite>
struct StatefulTestSuiteTests {
	constexpr static void statefulCounterTest(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Size(0), TestCounter<>);
		ts.assertEqual(natl::Size(1), TestCounter<>);
		ts.assertEqual(natl::Size(2), TestCounter<>);
		ts.assertEqual(natl::Size(3), TestCounter<>);
		ts.assertEqual(natl::Size(4), TestCounter<>);
	}
	NATL_REGISTER_TEST_CONSTEXPR(statefulCounterTest);

	
	constexpr static void statefulTypePackTest(test_suite_type& ts) noexcept {
		statefulTypePackTestImpl(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(statefulTypePackTest);
};

NATL_REGISTER_STD_TESTS(StatefulTestSuite);

NATL_TESTS_MAIN();