
//natl
#include <natl/functional.h>
#include <natl/string.h>
#include <natl/pair.h>
#include <natl/pointer.h>

constexpr natl::Size compileTimeTest() {
	natl::Function<natl::Size()> function = []() -> natl::Size { return 4; };
	natl::Function<natl::Size()> function2 = natl::move(function);
	natl::Function<natl::Size()> function3 = function2;
	return function3.invoke();
}

int main() {
	[[maybe_unused]] constexpr natl::Size number = compileTimeTest();
	[[maybe_unused]] natl::Size number2 = compileTimeTest();
}