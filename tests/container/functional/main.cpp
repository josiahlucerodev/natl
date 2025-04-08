
//natl
#include <natl/container/functional.h>
#include <natl/util/test.h>

constexpr natl::Bool work() noexcept {
	return true;
}

constexpr natl::Bool constructionTest() {
	auto test = []() noexcept -> natl::Bool { return true; };
	natl::impl::testHasFunctionSignature<decltype(test)&&, natl::Bool>();

	natl::Function<natl::Bool()> function = []() noexcept -> natl::Bool { return true; };
	natl::Function<natl::Bool()> function2 = natl::move(function);
	natl::Function<natl::Bool()> function3 = function2;
	natl::Function<natl::Bool()> function4(work);
	return function3.invoke();
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	constructionTest();
	return true;
}

int main() {
	tests();
}