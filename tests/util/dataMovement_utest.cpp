//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/dataMovement.h>
#include <natl/container/smallDynArray.h>
#include <natl/container/dynArray.h>
#include <natl/container/string.h>
#include <natl/container/heapArray.h>

struct DataMovementTestSuite : public natl::TestSuite<DataMovementTestSuite> {};

template<typename test_suite_type = DataMovementTestSuite>
struct DataMovementTestSuiteTests {
	constexpr static void moveAdapterTest(test_suite_type& ts) noexcept {
		natl::DynArray<char> charArray(100, 't');
		natl::SmallDynArray<char, 10> smallCharArray;
		natl::String charString = charArray.getAlloctionMoveAdapater();
		natl::HeapArray<char> heapArray;
		charArray = charString.getAlloctionMoveAdapater();
		charString = charArray.getAlloctionMoveAdapater();
		smallCharArray = charString.getAlloctionMoveAdapater();
		charArray = smallCharArray.getAlloctionMoveAdapater();
		smallCharArray = charArray.getAlloctionMoveAdapater();
		charString = smallCharArray.getAlloctionMoveAdapater();
		heapArray = charString.getAlloctionMoveAdapater();
		charString = heapArray.getAlloctionMoveAdapater();

		ts.assertEqual('t', charString.at(5));
	}
	NATL_REGISTER_TEST_CONSTEXPR(moveAdapterTest);
};

NATL_REGISTER_STD_TESTS(DataMovementTestSuite);

NATL_TESTS_MAIN();
