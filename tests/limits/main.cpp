
//natl
#include <natl/util/test.h>
#include <natl/container/any.h>
#include <natl/container/string.h>

constexpr natl::Bool constructionTest() noexcept {
	[[maybe_unused]] natl::Limits<natl::i8> limitsI8{};
	[[maybe_unused]] natl::Limits<natl::i16> limitsI16{};
	[[maybe_unused]] natl::Limits<natl::i32> limitsI32{};
	[[maybe_unused]] natl::Limits<natl::i64> limitsI64{};
	[[maybe_unused]] natl::Limits<natl::ui8> limitsUI8{};
	[[maybe_unused]] natl::Limits<natl::ui16> limitsUI16{};
	[[maybe_unused]] natl::Limits<natl::ui32> limitsUI32{};
	[[maybe_unused]] natl::Limits<natl::ui64> limitsUI64{};
	[[maybe_unused]] natl::Limits<natl::f32> limitsF32{};
	[[maybe_unused]] natl::Limits<natl::f64> limitsF64{};
	return true;
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	constructionTest();
	//NATL_TEST_ASSERT("NatlLimitsTest", constructionTest(), "construction test failed")
	return true;
}

int main() noexcept {
	tests();
}