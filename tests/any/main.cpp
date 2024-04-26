
//natl
#include <natl/any.h>
#include <natl/array.h>

constexpr natl::Size compileTimeTest() noexcept {
	natl::Any any = natl::makeAny<natl::Byte>();
	return 3;
}

using LargeType = natl::Array<natl::Byte, 40>;

int main() {
	static_assert(compileTimeTest() == 3);
	natl::AnyByteSize<64> any = natl::makeAnyByteSize<64, LargeType>();
}