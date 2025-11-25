//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/tuple.h>
#include <natl/processing/format.h>

struct TupleTestSuite : public natl::TestSuite<TupleTestSuite> {};

template<typename test_suite_type = TupleTestSuite>
struct TupleTestSuiteTests {

	constexpr static natl::i64 add(natl::i64 a, natl::i64 b) noexcept {
		return a + b;
	}

	constexpr static void tests(test_suite_type& ts) noexcept {
		ts.assertEqual("{FALSE, t}", 
			natl::sformat(natl::formatArgText<"0: FULL", "1: s">(natl::Tuple<natl::Bool, natl::Bool>(false, true))));
		
		natl::Tuple<natl::i64, natl::i64> tuple1(1, 2);
		ts.assertEqual(1, tuple1.get<0>());
		ts.assertEqual(2, tuple1.get<1>());

		auto tuple2 = natl::makeTuple(1, 2);
		ts.assertEqual(1, tuple2.get<0>());
		ts.assertEqual(2, tuple2.get<1>());

		ts.assertTrue(tuple1 == tuple2);
		ts.assertFalse(tuple1 != tuple2);

		ts.assertEqual(tuple1.hash(), tuple2.hash());
		ts.assertEqual(2, natl::TupleSize<decltype(tuple1)>);
		ts.assertEqual(2, natl::TupleSize<decltype(tuple2)>);
		
		ts.assertEqual(3, natl::callFunctionWithTuple(add, tuple2));
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(TupleTestSuite);

NATL_TESTS_MAIN();