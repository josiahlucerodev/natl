
//natl
#include <natl/stringConvert.h>

int main() {
	constexpr std::uint32_t uint32DecimalTest1Constexpr = natl::stringDecimalToInt(natl::intToStringDecimal(32).toStringView());
	constexpr std::uint32_t uint32DecimalTest4Constexpr = natl::stringHexadecimalToInt(natl::intToStringHexadecimal(32).toStringView());
	constexpr std::uint32_t uint32DecimalTest2Constexpr = natl::stringBinaryToInt(natl::intToStringBinary(32).toStringView());
	constexpr std::uint32_t uint32DecimalTest3Constexpr = natl::stringDecimalToInt(natl::intToStringDecimal(32).toStringView());
	constexpr std::uint8_t uint8DecimalTest1Constexpr = natl::stringDecimalToInt("32");
	constexpr float floatDecimalTest1Constexpr = natl::stringToFloat("32.2");
}