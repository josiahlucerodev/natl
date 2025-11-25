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

struct PersistentHashSetTestSuite : public impl::BaseHashSetTestSuite<natl::PersistentHashSet<natl::Size>, natl::PersistentHashSet<natl::String>> {};
NATL_REGISTER_TESTS(PersistentHashSetTestSuite, impl::BaseHashSetTestSuiteTests);

struct SmallPersistentHashSetTestSuite : public impl::BaseHashSetTestSuite<natl::SmallPersistentHashSet<natl::Size, 10>, natl::SmallPersistentHashSet<natl::String, 10>> {};
NATL_REGISTER_TESTS(SmallPersistentHashSetTestSuite, impl::BaseHashSetTestSuiteTests);

NATL_TESTS_MAIN();