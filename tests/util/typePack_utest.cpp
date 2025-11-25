//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/typePack.h>
#include <natl/util/typeInfo.h>

template<typename Type>
struct IsLessThanInt32V {
	constexpr static natl::Bool value = natl::IsSame<Type, natl::i8> || natl::IsSame<Type, natl::i16>;
};

struct TypePackTestSuite : public natl::TestSuite<TypePackTestSuite> {};

template<typename test_suite_type = TypePackTestSuite>
struct TypePackTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		using TestTypePack = natl::TypePack<natl::i8, natl::i16, natl::i32, natl::i64>;

		ts.assertTrue(natl::IsSame<natl::TypePackTake<2, TestTypePack>, natl::TypePack<natl::i8, natl::i16>>);

		using TypePackFilteredTestType = natl::TypePackFilter<IsLessThanInt32V, TestTypePack>;
		ts.assertTrue(natl::IsSame<TypePackFilteredTestType, natl::TypePack<natl::i8, natl::i16>>);

		using TypePackUniformTestType = natl::TypePackUniform<3, TestTypePack>;
		ts.assertTrue(natl::IsSame<TypePackUniformTestType, natl::TypePack<TestTypePack, TestTypePack, TestTypePack>>);

		ts.assertTrue(natl::packAt<1>(2, 3, 4) == 3);
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(TypePackTestSuite);

NATL_TESTS_MAIN();