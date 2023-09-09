
//natl
#include <natl/expect.h>
#include <natl/StringConvert.h>

enum class TestError {
	none,
	invalid,
	overflow
};

constexpr natl::Expect<bool, TestError> testFunc() {
	return natl::unexpected<TestError>(TestError::overflow);
}

int main() {
	constexpr natl::Expect<bool, TestError> number = testFunc();
	constexpr natl::Expect<float, natl::StringNumericConvertError> number2 = natl::stringToFloatExpect<float>("3.3");

	if (number.hasValue()) {
		std::cout << "number: " << number.value() << "\n";
	} else {
		std::cout << "error\n";
	}

}