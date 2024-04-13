
//natl
#include <natl/expect.h>
#include <natl/stringConvert.h>

enum class TestError {
	none,
	invalid,
	overflow
};

constexpr natl::Expect<natl::Bool, TestError> testFunc() {
	return natl::unexpected<TestError>(TestError::overflow);
}

int main() {
	[[maybe_unused]] constexpr natl::Expect<natl::Bool, TestError> number = testFunc();
	[[maybe_unused]] constexpr natl::Expect<float, natl::StringNumericConvertError> number2 = natl::stringToFloatExpect<float>("3.3");

	if (number.hasValue()) {
		std::cout << "number: " << number.value() << "\n";
	} else {
		std::cout << "error\n";
	}

}