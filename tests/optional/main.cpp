#ifdef NATL_MODULES
import natl.test;
import natl.optional;
#else 
//natl
#include <natl/test.h>
#include <natl/optional.h>
#endif //NATL_MODULES


bool basicTest(int) {
	return true;
}

int main() {
	static_assert(std::is_trivial_v<natl::Optional<int>>, "natl::Optional with Type thats trivial is not trivial");


	natl::testStart("Optional Test");
	natl::runTest(basicTest, 1, "basic test");
}