
//natl
#include <natl/util/typeInfo.h>
#include <natl/util/test.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlTypeInfoTest";

constexpr natl::Bool builtInTypesNameTest() noexcept {
	natl::Test test(natlTestFrom, "built in types names", natl::TestType::leaf);

#ifdef NATL_COMPILER_GCC
	natl::testAssertEquals(test, "bool", natl::getNameOfType<natl::Bool>(), "natl::Bool name");
	natl::testAssertEquals(test, "signed char", natl::getNameOfType<natl::i8>(), "natl::i8 name");
	natl::testAssertEquals(test, "short int", natl::getNameOfType<natl::i16>(), "natl::i16 name");
	natl::testAssertEquals(test, "int", natl::getNameOfType<natl::i32>(), "natl::i32 name");
	natl::testAssertEquals(test, "long int", natl::getNameOfType<natl::i64>(), "natl::i64 name");
	natl::testAssertEquals(test, "unsigned char", natl::getNameOfType<natl::ui8>(), "natl::ui8 name");
	natl::testAssertEquals(test, "short unsigned int", natl::getNameOfType<natl::ui16>(), "natl::ui16 name");
	natl::testAssertEquals(test, "unsigned int", natl::getNameOfType<natl::ui32>(), "natl::ui32 name");
	natl::testAssertEquals(test, "long unsigned int", natl::getNameOfType<natl::ui64>(), "natl::ui64 name");
	natl::testAssertEquals(test, "float", natl::getNameOfType<natl::f32>(), "natl::f32 name");
	natl::testAssertEquals(test, "double", natl::getNameOfType<natl::f64>(), "natl::f64 name");
#endif // NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
	natl::testAssertEquals(test, "bool", natl::getNameOfType<natl::Bool>(), "natl::Bool name");
	natl::testAssertEquals(test, "signed char", natl::getNameOfType<natl::i8>(), "natl::i8 name");
	natl::testAssertEquals(test, "short", natl::getNameOfType<natl::i16>(), "natl::i16 name");
	natl::testAssertEquals(test, "int", natl::getNameOfType<natl::i32>(), "natl::i32 name");
	natl::testAssertEquals(test, "__int64", natl::getNameOfType<natl::i64>(), "natl::i64 name");
	natl::testAssertEquals(test, "unsigned char", natl::getNameOfType<natl::ui8>(), "natl::ui8 name");
	natl::testAssertEquals(test, "unsigned short", natl::getNameOfType<natl::ui16>(), "natl::ui16 name");
	natl::testAssertEquals(test, "unsigned int", natl::getNameOfType<natl::ui32>(), "natl::ui32 name");
	natl::testAssertEquals(test, "unsigned __int64", natl::getNameOfType<natl::ui64>(), "natl::ui64 name");
	natl::testAssertEquals(test, "float", natl::getNameOfType<natl::f32>(), "natl::f32 name");
	natl::testAssertEquals(test, "double", natl::getNameOfType<natl::f64>(), "natl::f64 name");
#endif // NATL_COMPILER_MSVC

#ifdef NATL_COMPILER_EMSCRIPTEN
	natl::testAssertEquals(test, "bool", natl::getNameOfType<natl::Bool>(), "natl::Bool name");
	natl::testAssertEquals(test, "signed char", natl::getNameOfType<natl::i8>(), "natl::i8 name");
	natl::testAssertEquals(test, "short", natl::getNameOfType<natl::i16>(), "natl::i16 name");
	natl::testAssertEquals(test, "int", natl::getNameOfType<natl::i32>(), "natl::i32 name");
	natl::testAssertEquals(test, "long long", natl::getNameOfType<natl::i64>(), "natl::i64 name");
	natl::testAssertEquals(test, "unsigned char", natl::getNameOfType<natl::ui8>(), "natl::ui8 name");
	natl::testAssertEquals(test, "unsigned short", natl::getNameOfType<natl::ui16>(), "natl::ui16 name");
	natl::testAssertEquals(test, "unsigned int", natl::getNameOfType<natl::ui32>(), "natl::ui32 name");
	natl::testAssertEquals(test, "unsigned long long", natl::getNameOfType<natl::ui64>(), "natl::ui64 name");
	natl::testAssertEquals(test, "float", natl::getNameOfType<natl::f32>(), "natl::f32 name");
	natl::testAssertEquals(test, "double", natl::getNameOfType<natl::f64>(), "natl::f64 name");
#endif // NATL_COMPILER_EMSCRIPTEN

	return test;
}


natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, builtInTypesNameTest());
	return test;
}

int main() {
	tests();
}