//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
#define NATL_DISABLE_CONSTEXPR_TESTS
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/iteration.h>
#include <natl/container/string.h>

struct IterationTestSuite : public natl::TestSuite<IterationTestSuite> {};

template<typename test_suite_type = IterationTestSuite>
struct IterationTestSuiteTests {
	constexpr static void backInsertIteratorTest(test_suite_type& ts) noexcept {
		natl::String output;

		natl::BackInsertIterator<natl::String> outputIter(output);
		ts.assertEqual(output, "");
		outputIter = 'h';
		ts.assertEqual(output, "h");
		outputIter = 'e';
		ts.assertEqual(output, "he");
		outputIter = 'l';
		ts.assertEqual(output, "hel");
		outputIter = 'l';
		ts.assertEqual(output, "hell");
		outputIter = 'o';
		ts.assertEqual(output, "hello");
		outputIter = ' ';
		ts.assertEqual(output, "hello ");

		natl::TypeErasedBackInsertIterator<natl::Ascii> typeErasedOutputIter(output);
		typeErasedOutputIter = 'w';
		ts.assertEqual(output, "hello w");
		typeErasedOutputIter = 'o';
		ts.assertEqual(output, "hello wo");
		typeErasedOutputIter = 'r';
		ts.assertEqual(output, "hello wor");
		typeErasedOutputIter = 'l';
		ts.assertEqual(output, "hello worl");
		typeErasedOutputIter = 'd';
		ts.assertEqual(output, "hello world");
		typeErasedOutputIter = '!';
		ts.assertEqual(output, "hello world!");
	}
	NATL_REGISTER_TEST_CONSTEXPR(backInsertIteratorTest);

	constexpr static void repeatTest(test_suite_type& ts) noexcept {
		natl::Size eIndex = 0;
		natl::Size lIndex;
		for (natl::Size index : natl::Repeat(10)) { 
			ts.assertEqual(eIndex, index);
			eIndex++;
			lIndex = index;
		}
		ts.assertEqual(9, lIndex);

		eIndex = 0;
		for (natl::Size index : natl::RepeatTo(10)) {
			ts.assertEqual(eIndex, index);
			eIndex++;
			lIndex = index;
		}
		ts.assertEqual(10, lIndex);

		eIndex = 10;
		for (natl::Size index : natl::RepeatFrom(10, 20)) {
			ts.assertEqual(eIndex, index);
			eIndex++;
			lIndex = index;
		}
		ts.assertEqual(19, lIndex);

		eIndex = 10;
		for (natl::Size index : natl::RepeatFromTo(10, 20)) {
			ts.assertEqual(eIndex, index);
			eIndex++;
			lIndex = index;
		}
		ts.assertEqual(20, lIndex);
	}
	NATL_REGISTER_TEST_CONSTEXPR(repeatTest);
};

NATL_REGISTER_STD_TESTS(IterationTestSuite);

NATL_TESTS_MAIN();