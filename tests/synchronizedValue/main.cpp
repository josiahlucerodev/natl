
//natl
#include <natl/synchronizedValue.h>
#include <natl/test.h>

constexpr natl::Bool constructionTest() noexcept {
	natl::SynchronizedValue<natl::Bool> syncValue(false);
	{
		natl::UpdateGuard<natl::Bool> guard = syncValue;
		guard.value() = true;
	}
	{
		natl::UpdateGuard<const natl::Bool> guard = syncValue;
		guard.value();
	}

	natl::synchronizedApply([](natl::Bool& value) -> void { value = true; }, syncValue);

	return syncValue.blockingGetCopy();
}

static_assert(constructionTest());

natl::Bool tests() noexcept {
	NATL_TEST_ASSERT("NatlSynchronizedValueTest", constructionTest(), "construction test failed")
	return true;
}

int main() noexcept {
	tests();
}