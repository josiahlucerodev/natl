# Test 

## Example

```cpp
//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>

struct ExampleTestSuite : public natl::TestSuite<ExampleTestSuite> {};

template<typename test_suite_type = ExampleTestSuite>
struct ExampleTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		...
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(ExampleTestSuite);

NATL_TESTS_MAIN();

```