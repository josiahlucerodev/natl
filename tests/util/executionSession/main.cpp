
//natl
#include <natl/util/executionSession.h>
#include <natl/system/nullptr.h>
#include <natl/system/print.h>
#include <natl/units/standard.h>

int main() {
	constexpr natl::ConstAsciiStringView sessionName = "ExecutionSessionTest";
	natl::ObserverPtr<natl::ExecutionSession> session = natl::pushExecutionSession(sessionName);
	natl::enableExecutionSessionNullptrHandling();
	natl::enableAllocationTracking();
	natl::enableStadardAllocatorTracking();

	if (NATL_EXECUTION_SESSION_ERROR(session)) {
		auto errorInfoExpect = natl::getExecutionSessionError();
		if (errorInfoExpect.hasValue()) {
			natl::println(errorInfoExpect.value()->errorMessage.c_str());
		}
		
		natl::popExecutionSession(sessionName);
		natl::executionSessionErrorHandled();
		return 0;
	}


	NATL_DISABLE_WARNINGS
	{
		for (natl::Size i : natl::Repeat(100)) {
			constexpr natl::Size size = natl::abbr::MB.convertTo<natl::abbrt::B>().value();
			auto ptr = natl::DefaultAllocator::allocate(size);
			if (i % 2) {
				natl::DefaultAllocator::deallocate(ptr, size);
			}
		}

		//trigger nullptr
		volatile natl::Size* ptr = nullptr;
		*ptr = 0;
	}
	NATL_ENABLE_WARNINGS
}