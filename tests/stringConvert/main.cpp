
//natl
#include <natl/stringConvert.h>

int main() {
	[[maybe_unused]] constexpr natl::i64 uint32DecimalTest1Constexpr = natl::stringDecimalToInt(natl::intToStringDecimal(32));
	[[maybe_unused]] constexpr natl::ui64 uint32DecimalTest4Constexpr = natl::stringHexadecimalToInt(natl::intToStringHexadecimal(32));
	[[maybe_unused]] constexpr natl::ui64 uint32DecimalTest2Constexpr = natl::stringBinaryToInt(natl::intToStringBinary(32));
	[[maybe_unused]] constexpr natl::i64 uint32DecimalTest3Constexpr = natl::stringDecimalToInt(natl::intToStringDecimal(32));
	[[maybe_unused]] constexpr natl::i64 uint8DecimalTest1Constexpr = natl::stringDecimalToInt("32");
	[[maybe_unused]] constexpr float floatDecimalTest1Constexpr = natl::stringToFloat("32.2");

	std::variant<int> the;
}