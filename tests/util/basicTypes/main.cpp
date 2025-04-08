
//natl
#include <natl/util/basicTypes.h>
#include <natl/util/test.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlBasicTypeTest";

constexpr natl::Bool byteSizeTests() noexcept {
	natl::Test test(natlTestFrom, "built in types byte sizes", natl::TestType::leaf);
	natl::testAssertEquals(test, static_cast<natl::Size>(1), natl::TypeByteSize<natl::Bool>, "natl::Bool bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(1), natl::TypeByteSize<natl::i8>, "natl::i8 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(2), natl::TypeByteSize<natl::i16>, "natl::i16 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(4), natl::TypeByteSize<natl::i32>, "natl::i32 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(8), natl::TypeByteSize<natl::i64>, "natl::i64 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(1), natl::TypeByteSize<natl::ui8>, "natl::ui8 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(2), natl::TypeByteSize<natl::ui16>, "natl::ui16 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(4), natl::TypeByteSize<natl::ui32>, "natl::ui32 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(8), natl::TypeByteSize<natl::ui64>, "natl::ui64 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(4), natl::TypeByteSize<natl::f32>, "natl::f32 bytes size");
	natl::testAssertEquals(test, static_cast<natl::Size>(8), natl::TypeByteSize<natl::f64>, "natl::f64 bytes size");
	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, byteSizeTests());
	return test;
}

int main() noexcept {
	tests();
}