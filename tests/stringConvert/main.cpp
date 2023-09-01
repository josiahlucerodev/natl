
//natl
#include <natl/stringConvert.h>

int main() {
	constexpr std::uint32_t uint32DecimalTest1Constexpr = natl::stringDecimalToInt<std::uint32_t>("32");
	constexpr std::uint8_t uint8DecimalTest1Constexpr = natl::stringDecimalToInt<std::uint8_t>("32");
	constexpr float floatDecimalTest1Constexpr = natl::stringDecimalToFloat<float>("32.2");
}