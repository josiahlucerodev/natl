
//natl
#include <natl/util/stringConvert.h>

int main() {
	[[maybe_unused]] constexpr natl::i64 uint32DecimalTest1Constexpr = natl::stringDecimalToInt<natl::i64>(natl::intToStringDecimal(32));
	[[maybe_unused]] constexpr natl::ui64 uint32DecimalTest4Constexpr = natl::stringHexadecimalToInt<natl::ui64>(natl::intToStringHexadecimal(32));
	[[maybe_unused]] constexpr natl::ui64 uint32DecimalTest2Constexpr = natl::stringBinaryToInt<natl::ui64>(natl::intToStringBinary(32));
	[[maybe_unused]] constexpr natl::i64 uint32DecimalTest3Constexpr = natl::stringDecimalToInt<natl::i64>(natl::intToStringDecimal(32));
	[[maybe_unused]] constexpr natl::i64 uint8DecimalTest1Constexpr = natl::stringDecimalToInt<natl::i64>("32");
	[[maybe_unused]] constexpr natl::f32 floatDecimalTest1Constexpr = natl::stringToFloatDecimal<natl::f32>("32.2");
}