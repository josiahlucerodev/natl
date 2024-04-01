
//natl
#include <natl/iteration.h>

constexpr natl::Size compileTimeTest() {
	natl::Size sum = 0;
	for ([[maybe_unused]] natl::Size index : natl::Repeat(10)) { sum += 1; }
	for (auto [index, index2] : natl::makeCombindedIteration<const natl::Size, const natl::Size>(natl::Repeat(10), natl::RepeatFromTo(10, 20))) {
		sum += (index + index2);
	}
	return sum;
}

int main() {
	[[maybe_unused]] constexpr natl::Size number = compileTimeTest();
	[[maybe_unused]] natl::Size number2 = compileTimeTest();
}