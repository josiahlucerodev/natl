//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/memory.h>

struct MemoryTestSuite : public natl::TestSuite<MemoryTestSuite> {};

template<typename test_suite_type = MemoryTestSuite>
struct MemoryTestSuiteTests {
	static void allocateTest(test_suite_type& ts) noexcept {
		natl::ByteAllocResult result = natl::osMemoryAllocate(1, 1, natl::AllocateFlags::zeroed, natl::pageSize);
		ts.assertNotNull(result.mPtr);
		ts.assertEqual(result.mSize, natl::pageSize);
		ts.assertTrue(natl::bitCast<natl::Size>(result.mPtr) % natl::pageSize == 0);
		for (natl::Size i : natl::Repeat(result.mSize)) {
			ts.assertEqual(natl::Byte(0), result.mPtr[i]);
		}
		ts.assertTrue(natl::osMemoryDeallocate(result.mPtr));
	}
	NATL_REGISTER_TEST(allocateTest);

	static void remapTest(test_suite_type& ts) noexcept {
		natl::ByteAllocResult targetResult = natl::osMemoryAllocate(1, 1, natl::AllocateFlags::remappable, natl::pageSize);
		ts.assertNotNull(targetResult.mPtr);
		ts.assertEqual(targetResult.mSize, natl::pageSize);
		natl::ByteAllocResult donorResult = natl::osMemoryAllocate(1, 1, natl::AllocateFlags::remappable, natl::pageSize);
		ts.assertNotNull(donorResult.mPtr);
		ts.assertEqual(donorResult.mSize, natl::pageSize);

		constexpr natl::Byte targetValue = natl::Byte(42);
		constexpr natl::Byte donorValue = natl::Byte(69);

		for (natl::Size i : natl::Repeat(targetResult.mSize)) {
			targetResult.mPtr[i] = targetValue;
		}
		for (natl::Size i : natl::Repeat(donorResult.mSize)) {
			donorResult.mPtr[i] = donorValue;
		}

		ts.assertTrue(natl::osMemoryRemap(targetResult.mPtr, donorResult.mPtr, targetResult.mSize));
		
		for (natl::Size i : natl::Repeat(donorResult.mSize)) {
			ts.assertEqual(targetValue, donorResult.mPtr[i]);
		}
		for (natl::Size i : natl::Repeat(donorResult.mSize)) {
			ts.assertEqual(targetValue, donorResult.mPtr[i]);
		}

		ts.assertTrue(natl::osMemoryDeallocate(targetResult.mPtr));

		for (natl::Size i : natl::Repeat(donorResult.mSize)) {
			ts.assertEqual(targetValue, donorResult.mPtr[i]);
		}
		ts.assertTrue(natl::osMemoryDeallocate(donorResult.mPtr));
	}
	NATL_REGISTER_TEST(remapTest);
};

NATL_REGISTER_STD_TESTS(MemoryTestSuite);

NATL_TESTS_MAIN();