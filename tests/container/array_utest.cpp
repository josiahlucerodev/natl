//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/array.h>

struct ArrayTestSuite : public natl::TestSuite<ArrayTestSuite> {};

template<typename test_suite_type = ArrayTestSuite>
struct ArrayTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::Array<natl::Size, 5> array = { 0, 1, 2, 3, 4 };
		ts.assertEqual(0, array.at(0));
		ts.assertEqual(1, array.at(1));
		ts.assertEqual(2, array.at(2));
		ts.assertEqual(3, array.at(3));
		ts.assertEqual(4, array.at(4));

		array.fill(3);
		ts.assertEqual(3, array.at(0));
		ts.assertEqual(3, array.at(1));
		ts.assertEqual(3, array.at(2));
		ts.assertEqual(3, array.at(3));
		ts.assertEqual(3, array.at(4));

		natl::MDArray<natl::Size, 4, 4> mdArray;
		mdArray.at(0, 0) = 1;
		mdArray.at(0, 1) = 2;
		mdArray.at(0, 2) = 3;
		mdArray.at(0, 3) = 4;

		mdArray.at(1, 0) = 5;
		mdArray.at(1, 1) = 6;
		mdArray.at(1, 2) = 7;
		mdArray.at(1, 3) = 8;
		
		mdArray.at(2, 0) = 9;
		mdArray.at(2, 1) = 10;
		mdArray.at(2, 2) = 11;
		mdArray.at(2, 3) = 12;

		mdArray.at(3, 0) = 13;
		mdArray.at(3, 1) = 14;
		mdArray.at(3, 2) = 15;
		mdArray.at(3, 3) = 16;

		ts.assertEqual(1, mdArray.at(0, 0));
		ts.assertEqual(2, mdArray.at(0, 1));
		ts.assertEqual(3, mdArray.at(0, 2));
		ts.assertEqual(4, mdArray.at(0, 3));

		ts.assertEqual(5, mdArray.at(1, 0));
		ts.assertEqual(6, mdArray.at(1, 1));
		ts.assertEqual(7, mdArray.at(1, 2));
		ts.assertEqual(8, mdArray.at(1, 3));

		ts.assertEqual(9, mdArray.at(2, 0));
		ts.assertEqual(10, mdArray.at(2, 1));
		ts.assertEqual(11, mdArray.at(2, 2));
		ts.assertEqual(12, mdArray.at(2, 3));

		ts.assertEqual(13, mdArray.at(3, 0));
		ts.assertEqual(14, mdArray.at(3, 1));
		ts.assertEqual(15, mdArray.at(3, 2));
		ts.assertEqual(16, mdArray.at(3, 3));
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(ArrayTestSuite);

NATL_TESTS_MAIN();