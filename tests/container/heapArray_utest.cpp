//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/heapArray.h>

struct HeapArrayTestSuite : public natl::TestSuite<HeapArrayTestSuite> {};

template<typename test_suite_type = HeapArrayTestSuite>
struct HeapArrayTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::HeapArray<natl::i64> intArray(100, 5);
		ts.assertEqual(100, intArray.size());
		for (natl::Size i : natl::Repeat(intArray.size())) {
			ts.assertEqual(5, intArray.at(i));
		}

		natl::HeapArray<natl::i64> intArray2 = natl::move(intArray);
		ts.assertEqual(100, intArray2.size());
		for (natl::Size i : natl::Repeat(intArray2.size())) {
			ts.assertEqual(5, intArray2.at(i));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(HeapArrayTestSuite);

NATL_TESTS_MAIN();