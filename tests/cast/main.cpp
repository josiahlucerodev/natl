
//natl
#include <natl/util/test.h>
#include <natl/util/cast.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlCastTest";

class TestStruct{
	int member;
};
NATL_PUBLIC_CAST_HELPER(impl, TestStruct, obj, member)

constexpr natl::Bool privateCastTest() noexcept {
	natl::Test test(natlTestFrom, "private cast", natl::TestType::leaf);

	TestStruct obj;
	auto& member = NATL_PUBLIC_CAST(impl, obj, member);
	member = 10;
	natl::testAssertEquals(test, 10, member, "set private member to 10");
	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, privateCastTest());
	return test;
}



int main() {
	tests();
}