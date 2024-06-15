
//natl
#include <natl/simd.h>
#include <natl/math.h>
#include <natl/util/test.h>

using simd_arch = nasimd::StandardArch<256>;

constexpr natl::Bool constructionTest() noexcept {
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::i8, simd_arch> simd_i8;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::i16, simd_arch> simd_i16;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::i32, simd_arch> simd_i32;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::i64, simd_arch> simd_i64;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::ui8, simd_arch> simd_ui8;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::ui16, simd_arch> simd_ui16;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::ui32, simd_arch> simd_ui32;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::ui64, simd_arch> simd_ui64;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::f32, simd_arch> simd_f32;
	[[maybe_unused]] nasimd::SimdRegisterClass<natl::f64, simd_arch> simd_f64;
	return true;
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	constructionTest();
	//NATL_TEST_ASSERT("NatlSimdTest", constructionTest(), "construction test failed")
	return true;
}

int main() noexcept {
	{
		nasimd::SimdRegisterClass<natl::i8, simd_arch> simd_i8(1);
		++simd_i8;

		[[maybe_unused]] nasimd::SimdRegisterClass<natl::i32, simd_arch> type{};
		nasimd::SimdRegisterI32<simd_arch> r1_i8{};
		nasimd::SimdRegisterI32<simd_arch> r2_i8{};
		r1_i8 = nasimd::set_i32<simd_arch>(4);
		r2_i8 = nasimd::set_i32<simd_arch>(4);
		r1_i8 = nasimd::add_i32<simd_arch>(r1_i8, r2_i8);
	}
	//tests();
}