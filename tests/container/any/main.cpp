
//natl
#include <natl/util/test.h>
#include <natl/container/any.h>
#include <natl/container/string.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlAnyTest";

constexpr natl::Bool constructionTest() noexcept {
	natl::Test test(natlTestFrom, "construction test", natl::TestType::leaf);
	using namespace natl::literals;
	natl::testAssertEquals(test, 1, natl::makeAny<natl::Size>(1_size).getAs<natl::Size>().value());
	natl::testAssertEquals(test, 2, natl::makeAny<natl::Size>(2_size).getAs<natl::Size>().value());
	natl::testAssertEquals(test, "Test String", natl::makeAny<natl::String256>("Test String").getAs<natl::String256>().value());
	return test;
}

static_assert(constructionTest());

constexpr natl::Bool moveTest() noexcept {
	natl::Test test(natlTestFrom, "move test", natl::TestType::leaf);

	natl::Any anyValue = natl::makeAny<natl::String256>("Test String");
	natl::Any anyMoveDst = move(anyValue);	
	natl::testAssertEquals(test, "Test String", anyMoveDst.getAs<natl::String256>().value());
	natl::testAssertEquals(test, false, anyValue.hasValue());
	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::testAssert(test, constructionTest(), "construction test");
	natl::testAssert(test, moveTest(), "move test");
	return test;
}

int main() noexcept {
	tests();
}