
//natl
#include <natl/container/functionCache.h>


constexpr natl::Size compileTimeTest() noexcept {
	natl::FunctionCache<natl::Size(natl::Size)> functionCache([](natl::Size a) -> natl::Size { return 4 + a; });
	functionCache.bindArguments(3);
	natl::FunctionCache<natl::Size(natl::Size)> functionCache2 = natl::move(functionCache);
	return functionCache2.invoke();
}

constexpr natl::Size recursiveFibonacci(const natl::Size number) noexcept {
	if (number == 0 || number == 1) {
		return number;
	} else {
		return recursiveFibonacci(number - 1) + recursiveFibonacci(number - 2);
	}
}
constexpr natl::Size recursiveFibonacciCached(const natl::Size number) noexcept {
	if (number == 0 || number == 1) {
		return number;
	} else {
		return natl::callFunctionCached(recursiveFibonacciCached, number - 1)  
			+  natl::callFunctionCached(recursiveFibonacciCached, number - 2);
	}
}

int main() {
	[[maybe_unused]] constexpr natl::Size testNumber1 = compileTimeTest();
	[[maybe_unused]] natl::Size testNumber2 = compileTimeTest();
	[[maybe_unused]] constexpr natl::Size testNumber3 = recursiveFibonacci(20);
	[[maybe_unused]] natl::Size testNumber4 = recursiveFibonacci(40);
	[[maybe_unused]] natl::Size testNumber5 = recursiveFibonacciCached(50);
	[[maybe_unused]] constexpr natl::Size testNumber6 = recursiveFibonacciCached(10);
	std::cout << testNumber6;
}