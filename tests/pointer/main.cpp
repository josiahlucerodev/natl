
//natl
#include <natl/pointer.h>

int main() {
	natl::SharedPtr<int> sharedPtr(natl::SharedPtrFusedConstruct{}, natl::DefaultAllocatorByte{}, 1);
}