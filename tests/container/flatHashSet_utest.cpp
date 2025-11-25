//@file_not_module

//@begin_non_modules
//natl
#define NATL_DISABLE_CONSTEXPR_TESTS
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/persistentHashSet.h>

//own
#include "hashSetTests.h"

struct FlatHashSetTestSuite : public impl::BaseHashSetTestSuite<natl::FlatHashSet<natl::Size>, natl::FlatHashSet<natl::String>> {};
NATL_REGISTER_TESTS(FlatHashSetTestSuite, impl::BaseHashSetTestSuiteTests);

struct SmallFlatHashSetTestSuite : public impl::BaseHashSetTestSuite<natl::SmallFlatHashSet<natl::Size, 10>, natl::SmallFlatHashSet<natl::String, 10>> {};
NATL_REGISTER_TESTS(SmallFlatHashSetTestSuite, impl::BaseHashSetTestSuiteTests);

NATL_TESTS_MAIN();