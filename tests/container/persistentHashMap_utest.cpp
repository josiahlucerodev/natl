//@file_not_module

//@begin_non_modules
//natl
#define NATL_DISABLE_CONSTEXPR_TESTS
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/persistentHashMap.h>

//own
#include "hashMapTests.h"

struct PersistentHashMapTestSuite : public impl::BaseHashMapTestSuite<natl::PersistentHashMap<natl::Size, natl::Size>, natl::PersistentHashMap<natl::String, natl::String>> {};
NATL_REGISTER_TESTS(PersistentHashMapTestSuite, impl::BaseHashMapTestSuiteTests);

struct SmallPersistentHashMapTestSuite : public impl::BaseHashMapTestSuite<natl::SmallPersistentHashMap<natl::Size, natl::Size, 10>, natl::SmallPersistentHashMap<natl::String, natl::String, 10>> {};
NATL_REGISTER_TESTS(SmallPersistentHashMapTestSuite, impl::BaseHashMapTestSuiteTests);

NATL_TESTS_MAIN();