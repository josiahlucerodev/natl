//@file_not_module

//@begin_non_modules
//natl
#define NATL_DISABLE_CONSTEXPR_TESTS
#include <natl/util/testMacros.h>
//@end_non_modules


//natl
#include <natl/util/test.h>
#include <natl/container/flatHashMap.h>

//own
#include "hashMapTests.h"

struct FlatHashMapTestSuite : public impl::BaseHashMapTestSuite<natl::FlatHashMap<natl::Size, natl::Size>, natl::FlatHashMap<natl::String, natl::String>> {};
NATL_REGISTER_TESTS(FlatHashMapTestSuite, impl::BaseHashMapTestSuiteTests);

struct SmallFlatHashMapTestSuite : public impl::BaseHashMapTestSuite<natl::SmallFlatHashMap<natl::Size, natl::Size, 10>, natl::SmallFlatHashMap<natl::String, natl::String, 10>> {};
NATL_REGISTER_TESTS(SmallFlatHashMapTestSuite, impl::BaseHashMapTestSuiteTests);

NATL_TESTS_MAIN();