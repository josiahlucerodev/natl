
//natl
#include <natl/option.h>
#include <natl/string.h>
#include <natl/test.h>


constexpr natl::Bool constructionTest() noexcept {
	using namespace natl::literals;
	natl::Option<natl::Size> number1 = 1_natl_size;

	natl::Option<natl::Size> number2;
	number2 = 2_natl_size;

	natl::Option<natl::Size> number3 = 5_natl_size;
	number3 = natl::OptionEmpty();
	number3 = 3_natl_size;

	natl::Option<natl::Size> number4 = 4_natl_size;
	number4 = natl::OptionEmpty();

	natl::Option<natl::String> number5 = "5";

	natl::Size number6 = 6_natl_size;
	natl::OptionPtr<natl::Size> number6Ptr = &number6;

	natl::OptionPtr<natl::Size> number7Ptr = nullptr;


	return number1.value() == 1_natl_size 
		&& number2.value() == 2_natl_size 
		&& number3.value() == 3_natl_size 
		&& number4.doesNotHaveValue() 
		&& number5.value() == "5" 
		&& number6Ptr.value() == 6_natl_size 
		&& number7Ptr.doesNotHaveValue();
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	NATL_TEST_ASSERT("NatlOptionTest", constructionTest(), "construction test failed")
	return true;
}

int main() {
	tests();
}