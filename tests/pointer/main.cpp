
//natl
#include <natl/pointer.h>

constexpr natl::Size constexprTest() {
	natl::WeakPtr<int> weakPtr{};
	{
		natl::SharedPtr<int> sharedPtr(natl::SharedPtrFusedConstruct{}, natl::DefaultAllocatorByte{}, 1);
		//weakPtr = sharedPtr;
	}
	natl::SharedPtr<int> sharedPtr2 = weakPtr.lock();
	return 3;
}

int main() {
	[[maybe_unused]] constexpr natl::Size value = constexprTest();
	[[maybe_unused]] natl::Size value2 = constexprTest();
	std::cout << value2;
}