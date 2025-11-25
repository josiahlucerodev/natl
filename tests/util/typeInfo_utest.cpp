//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/typeInfo.h>

struct TypeInfoTestSuite : public natl::TestSuite<TypeInfoTestSuite> {};

template<typename test_suite_type = TypeInfoTestSuite>
struct TypeInfoTestSuiteTests {
	constexpr static void builtInTypesNameTest(test_suite_type& ts) noexcept {
#ifdef NATL_COMPILER_GCC
		ts.assertEqual("bool", natl::getNameOfType<natl::Bool>());
		ts.assertEqual("signed char", natl::getNameOfType<natl::i8>());
		ts.assertEqual("short int", natl::getNameOfType<natl::i16>());
		ts.assertEqual("int", natl::getNameOfType<natl::i32>());
		ts.assertEqual("long int", natl::getNameOfType<natl::i64>());
		ts.assertEqual("unsigned char", natl::getNameOfType<natl::ui8>());
		ts.assertEqual("short unsigned int", natl::getNameOfType<natl::ui16>());
		ts.assertEqual("unsigned int", natl::getNameOfType<natl::ui32>());
		ts.assertEqual("long unsigned int", natl::getNameOfType<natl::ui64>());
		ts.assertEqual("float", natl::getNameOfType<natl::f32>());
		ts.assertEqual("double", natl::getNameOfType<natl::f64>());
#endif // NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
		ts.assertEqual("bool", natl::getNameOfType<natl::Bool>());
		ts.assertEqual("signed char", natl::getNameOfType<natl::i8>());
		ts.assertEqual("short", natl::getNameOfType<natl::i16>());
		ts.assertEqual("int", natl::getNameOfType<natl::i32>());
		ts.assertEqual("__int64", natl::getNameOfType<natl::i64>());
		ts.assertEqual("unsigned char", natl::getNameOfType<natl::ui8>());
		ts.assertEqual("unsigned short", natl::getNameOfType<natl::ui16>());
		ts.assertEqual("unsigned int", natl::getNameOfType<natl::ui32>());
		ts.assertEqual("unsigned __int64", natl::getNameOfType<natl::ui64>());
		ts.assertEqual("float", natl::getNameOfType<natl::f32>());
		ts.assertEqual("double", natl::getNameOfType<natl::f64>());
#endif // NATL_COMPILER_MSVC

#ifdef NATL_COMPILER_EMSCRIPTEN
		ts.assertEqual("bool", natl::getNameOfType<natl::Bool>());
		ts.assertEqual("signed char", natl::getNameOfType<natl::i8>());
		ts.assertEqual("short", natl::getNameOfType<natl::i16>());
		ts.assertEqual("int", natl::getNameOfType<natl::i32>());
		ts.assertEqual("long long", natl::getNameOfType<natl::i64>());
		ts.assertEqual("unsigned char", natl::getNameOfType<natl::ui8>());
		ts.assertEqual("unsigned short", natl::getNameOfType<natl::ui16>());
		ts.assertEqual("unsigned int", natl::getNameOfType<natl::ui32>());
		ts.assertEqual("unsigned long long", natl::getNameOfType<natl::ui64>());
		ts.assertEqual("float", natl::getNameOfType<natl::f32>());
		ts.assertEqual("double", natl::getNameOfType<natl::f64>());
#endif // NATL_COMPILER_EMSCRIPTEN
	}
	NATL_REGISTER_TEST_CONSTEXPR(builtInTypesNameTest);
};

NATL_REGISTER_STD_TESTS(TypeInfoTestSuite);

NATL_TESTS_MAIN();
