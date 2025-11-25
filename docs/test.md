# Test 

## Example

```cpp

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