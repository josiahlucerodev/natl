#ifdef NATL_MODULES
//std
import std;
//own
import natl.timer;
import natl.test;
import natl.uninitialized;
#else 
//own
#include <natl/pch.h>
#include <natl/test.h>
#include <natl/uninitialized.h>
#endif //NATL_MODULES

consteval void constexprTest() {
	natl::UninitilizedArray<natl::TestType, 2> array = { natl::TestType(), natl::TestType() };
}

int main() {
	constexprTest();

	static_assert(std::is_trivial_v<natl::UninitilizedArray<std::uint32_t, 2>>, "std::is_trivial_v<natl::UninitilizedArray<std::uint32_t, 2>> not trivial");
	natl::UninitilizedArray<std::uint32_t, 2> arrayInt = { 3, 3};
	natl::UninitilizedArray<std::uint32_t, 2> arrayInt2;
	natl::UninitilizedArray<natl::TestTypeStdOut, 2> array = { natl::TestTypeStdOut(), natl::TestTypeStdOut()};
	//arrayInt2.(1);
}