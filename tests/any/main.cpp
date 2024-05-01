
//natl
#include <natl/any.h>
#include <natl/test.h>
#include <natl/string.h>

constexpr natl::Bool constructionTest() noexcept {
	using namespace natl::literals;
	natl::Any number1 = natl::makeAny<natl::Size>(1_natl_size);
	natl::Any number2 = natl::makeAny<natl::Size>(2_natl_size);


	return number1.getAs<natl::Size>().value() == 1_natl_size 
		&& number2.getAs<natl::String>().doesNotHaveValue();
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	NATL_TEST_ASSERT("NatlAnyTest", constructionTest(), "construction test failed")
	return true;
}

int main() noexcept {
	tests();
}