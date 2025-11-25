//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
#include <natl/util/executionSessionMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/executionSession.h>
#include <natl/system/nullptrUtils.h>
#include <natl/system/print.h>
#include <natl/units/standard.h>

struct ExecutionSessionTestSuite : public natl::TestSuite<ExecutionSessionTestSuite> {};

template<typename test_suite_type = ExecutionSessionTestSuite>
struct ExecutionSessionTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		constexpr natl::ConstAsciiStringView sessionName = "ExecutionSessionTest";
		natl::ObserverPtr<natl::ExecutionSession> session = natl::pushExecutionSession(sessionName);
		natl::enableExecutionSessionNullptrHandling();
		natl::enableAllocationTracking();
		natl::enableStadardAllocatorTracking();

		if (NATL_EXECUTION_SESSION_ERROR(session)) {
			auto errorInfoExpect = natl::getExecutionSessionError();
			ts.assertTrue(errorInfoExpect.hasValue());
			ts.assertEqual(natl::ConstStringView("natl: null pointer dereference"), errorInfoExpect.value()->errorMessage);
			
			natl::popExecutionSession(sessionName);
			natl::executionSessionErrorHandled();
			return;
		}


		NATL_DISABLE_WARNINGS;
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
		NATL_ENABLE_WARNINGS;
		ts.fail();
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(ExecutionSessionTestSuite);

NATL_TESTS_MAIN();
