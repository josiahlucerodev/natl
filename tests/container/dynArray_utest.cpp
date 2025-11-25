//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/dynArray.h>
#include <natl/container/smallDynArray.h>
#include <natl/container/batchPool.h>

struct DynArrayTestSuite : public natl::TestSuite<DynArrayTestSuite> {};

template<typename test_suite_type = DynArrayTestSuite>
struct DynArrayTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		const natl::Size count = ts.compileOrRun(10, 1000);

		natl::SmallDynArray<natl::Size, 5> smallDynArray;
		for (natl::Size i : natl::Repeat(count)) {
			smallDynArray.pushBack(i);
		}
		natl::DynArray<natl::Size> dynArray = smallDynArray.getAlloctionMoveAdapater();
		
		for (natl::Size i : natl::Repeat(count)) {
			dynArray.pushBack(i);
		}

		for (natl::Size i : natl::Repeat(count)) {
			dynArray.pushBack(i);
			ts.assertEqual(i, dynArray.at(i));
		}
		for (natl::Size i : natl::Repeat(count)) {
			dynArray.pushBack(i);
			ts.assertEqual(i, dynArray.at(count + i));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(DynArrayTestSuite);

NATL_TESTS_MAIN();