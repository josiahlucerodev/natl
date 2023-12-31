
//natl
#include <natl/functional.h>
#include <natl/string.h>
#include <natl/pair.h>
#include <natl/pointer.h>

constexpr int compileTimeTest() {
	int value = 3;
	natl::String32 string;
	auto func = [value](int inValue) { return value + inValue; };
	natl::Function<int(int)> function = [value](int inValue) { return value + inValue; };
	return function.invoke(4);
}

int main() {
	[[maybe_unused]] constexpr int number = compileTimeTest();
	int value = 0;
	natl::String32 string = "hello world!";
	natl::FunctionByteSize<void(int), 64> function = [&value](int inValue) { value = inValue; };
	natl::FunctionByteSize<void(int), 64> function2 = [&value, string](int inValue) { value = inValue * string.size(); };
	function = natl::move(function2);
	function(5);
}