
//natl
#include <natl/util/stringConvert.h>
#include <natl/math/basicFloatOperations.h>

int main() {
	static_assert(natl::stringDecimalToInt<natl::i64>(natl::intToStringDecimal(32)) == 32);
	static_assert(natl::stringBinaryToInt<natl::ui64>(natl::intToStringBinary(32)) == 32);
	static_assert(natl::stringDecimalToInt<natl::i64>(natl::intToStringDecimal(32)) == 32);
	static_assert(natl::stringDecimalToInt<natl::i64>("32") == 32);
	static_assert(natl::math::isRoughtlyEqual<natl::f32>(natl::stringDecimalToFloat<natl::f32>("32.2"), 32.2f, 0.001f));
}