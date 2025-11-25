//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
#include <natl/util/castMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/cast.h>

class TestStruct {
	int member;
};
NATL_PUBLIC_CAST_HELPER(impl, TestStruct, obj, member)

struct CastTestSuite : public natl::TestSuite<CastTestSuite> {};

template<typename test_suite_type = CastTestSuite>
struct CastTestSuiteTests {
	constexpr static void privateCastTest(test_suite_type& ts) noexcept {
		TestStruct obj;
		auto& member = NATL_PUBLIC_CAST(impl, obj, member);
		member = 10;
		ts.assertEqual(10, member);
	}
	NATL_REGISTER_TEST(privateCastTest);
};

NATL_REGISTER_STD_TESTS(CastTestSuite);

NATL_TESTS_MAIN();