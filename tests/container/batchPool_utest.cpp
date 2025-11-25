//@file_not_module

//@begin_non_modules
//natl
#define NATL_DISABLE_CONSTEXPR_TESTS
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/batchPool.h>
#include <natl/container/string.h>
#include <natl/container/dynArray.h>

template<typename BatchPoolType, natl::Bool Dealloc>
struct BaseBatchPoolTestSuite : public natl::TestSuite<BaseBatchPoolTestSuite<BatchPoolType, Dealloc>> {
	using batch_pool_type = BatchPoolType;
	constexpr static inline natl::Size dealloc = Dealloc;
};

template<typename test_suite_type = BaseBatchPoolTestSuite<natl::MonotonicBatchPool<natl::String, 10>, false>>
struct BaseBatchPoolTestSuiteTests {
	using batch_pool_type = test_suite_type::batch_pool_type;

	constexpr static void tests(test_suite_type& ts) noexcept {
		const natl::Size count = ts.compileOrRun(10, 1000);

		batch_pool_type batchPool;
		batchPool.reserve(100);

		constexpr natl::ConstAsciiStringView testString = "TestString";
		natl::String* value1 = batchPool.allocateConstructed();
		*value1 = testString;

		ts.assertEqual(testString, *value1);
		natl::DynArray<natl::String*> allocatedStrings;
		for ([[maybe_unused]] natl::Size index : natl::Repeat(count)) {
			natl::String* generatedValue = batchPool.allocateConstructed();
			allocatedStrings.pushBack(generatedValue);
			*generatedValue = testString;
			ts.assertEqual(testString, *generatedValue);
		}

		if constexpr (test_suite_type::dealloc) {
			for (natl::String* generatedValue : allocatedStrings) {
				batchPool.deallocate(generatedValue);
			}
			ts.assertEqual(testString, *value1);
			batchPool.deallocate(value1);
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

struct MonotonicBatchPoolTestSuite : public BaseBatchPoolTestSuite<natl::MonotonicBatchPool<natl::String, 10>, false> {};
NATL_REGISTER_TESTS(MonotonicBatchPoolTestSuite, BaseBatchPoolTestSuiteTests);

struct RecycleMonotonicBatchPoolTestSuite : public BaseBatchPoolTestSuite<natl::RecycleMonotonicBatchPool<natl::String, 10>, true> {};
NATL_REGISTER_TESTS(RecycleMonotonicBatchPoolTestSuite, BaseBatchPoolTestSuiteTests);

struct BatchPoolTestSuite : public BaseBatchPoolTestSuite<natl::BatchPool<natl::String, 10>, true> {};
NATL_REGISTER_TESTS(BatchPoolTestSuite, BaseBatchPoolTestSuiteTests);

NATL_TESTS_MAIN();