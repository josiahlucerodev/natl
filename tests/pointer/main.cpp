
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

enum class TestEnum : natl::ui16 {
	red = 9,
	green = 1
};

constexpr natl::Size constexprTest2() {
	natl::PackedPtrAndSmallData<natl::Size, TestEnum> the(new natl::Size, TestEnum::red);
	*the.getPtr() = 3;
	the.setPtr(the.getPtr());
	the.setSmallData(TestEnum::red);
	the.getSmallData();
	natl::Size data = *the.getPtr();
	delete the.getPtr();
	return data;
}

int main() {
	[[maybe_unused]] constexpr natl::Size value = constexprTest();
	[[maybe_unused]] natl::Size value2 = constexprTest();
	[[maybe_unused]] constexpr natl::Size value3 = constexprTest2();
	[[maybe_unused]] natl::Size value4 = constexprTest2();
	std::cout << value << value3;
	std::cout << value2 << value2;
}