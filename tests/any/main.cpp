
//natl
#include <natl/util/test.h>
#include <natl/container/any.h>
#include <natl/container/string.h>

constexpr natl::Bool constructionTest() noexcept {
	using namespace natl::literals;
	natl::Any number1 = natl::makeAny<natl::Size>(1_size);
	natl::Any number2 = natl::makeAny<natl::Size>(2_size);
	return number1.getAs<natl::Size>().value() == 1_size 
		&& number2.getAs<natl::String>().doesNotHaveValue();
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	constructionTest();
	//NATL_TEST_ASSERT("NatlAnyTest", constructionTest(), "construction test failed")
	return true;
}

int main() noexcept {
	tests();
}