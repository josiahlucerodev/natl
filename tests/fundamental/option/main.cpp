
//natl
#include <natl/fundamental/option.h>
#include <natl/container/string.h>
#include <natl/util/test.h>


constexpr natl::Bool constructionTest() noexcept {
	using namespace natl::literals;
	natl::Option<natl::Size> number1 = 1_size;

	natl::Option<natl::Size> number2;
	number2 = 2_size;

	natl::Option<natl::Size> number3 = 5_size;
	number3 = natl::OptionEmpty();
	number3 = 3_size;

	natl::Option<natl::Size> number4 = 4_size;
	number4 = natl::OptionEmpty();

	natl::Option<natl::String> number5 = "5";

	natl::Size number6 = 6_size;
	natl::OptionPtr<natl::Size> number6Ptr = &number6;

	natl::OptionPtr<natl::Size> number7Ptr = nullptr;


	return number1.value() == 1_size 
		&& number2.value() == 2_size 
		&& number3.value() == 3_size 
		&& number4.doesNotHaveValue() 
		&& number5.value() == "5" 
		&& number6Ptr.value() == 6_size 
		&& number7Ptr.doesNotHaveValue();
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	constructionTest();
	//NATL_TEST_ASSERT("NatlOptionTest", constructionTest(), "construction test failed")
	return true;
}

int main() {
	tests();
}