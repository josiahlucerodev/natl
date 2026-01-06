//@file_not_module

//@begin_non_modules
#include <natl/util/compilerDependent.h>
#include <natl/util/testMacros.h>
#include <natl/util/executionSessionMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/allocParts.h>


using region_manager = natl::InlineAllocRegionManager<natl::pageSize64k, true, true, 128, natl::BaseAllocRoot>;
using os_alloc = natl::OsAlloctor<>;

struct OsAllocTestSuite : public natl::TestSuite<OsAllocTestSuite> {};

template<typename test_suite_type = OsAllocTestSuite>
struct OsAllocTestSuiteTests {
	using alloc_type = os_alloc;
	
	static void zeroedTest(test_suite_type& ts) noexcept {
		natl::ByteAllocResult allocResult = alloc_type::allocate(natl::pageSize, natl::AllocateFlags::zeroed);
		ts.assertNotNull(allocResult.mPtr);
		ts.assertEqual(allocResult.mSize, natl::pageSize);
		for (natl::Size i : natl::Repeat(allocResult.mSize)) {
			ts.assertEq(natl::Byte(0), allocResult.mPtr[i]);
		}
		ts.assertTrue(alloc_type::deallocate(allocResult.mPtr));
	}
	NATL_REGISTER_TEST(zeroedTest);
};

template<natl::Size vMinAlloc, natl::Size vMaxAlloc, typename Alloc>
struct FreeListAllocTestSuite : public natl::TestSuite<FreeListAllocTestSuite<vMinAlloc, vMaxAlloc, Alloc>> {
	using alloc_type = Alloc;
	constexpr static natl::Size minAlloc = vMinAlloc;
	constexpr static natl::Size maxAlloc = vMaxAlloc;
};

template<typename test_suite_type>
struct FreeListAllocTestSuiteTests {
	using alloc_type = test_suite_type::alloc_type;
	constexpr static natl::Size minAlloc = test_suite_type::minAlloc;
	constexpr static natl::Size maxAlloc = test_suite_type::maxAlloc;

	static void allocsTest(test_suite_type& ts) noexcept {
		alloc_type alloc;
		alloc.init();

		natl::ByteAllocResult allocResult = alloc.allocate(8, minAlloc, natl::AllocateFlags::zeroed);
		ts.assertNotNull(allocResult.mPtr);
		ts.assertGe(minAlloc, allocResult.mSize);
		ts.assertFalse(alloc.getAlloc().isEmpty());
		for (natl::Size i : natl::Repeat(allocResult.mSize)) {
			ts.assertEqual(natl::Byte(0), allocResult.mPtr[i]);
		}
		ts.assertTrue(alloc.deallocate(allocResult.mPtr));
		ts.assertTrue(alloc.getAlloc().isEmpty());
		alloc.deinit();
	}
	NATL_REGISTER_TEST(allocsTest);

	static void fullTest(test_suite_type& ts) noexcept {
		alloc_type alloc;
		alloc.init();

		for (natl::Size r : natl::Repeat(3)) {
			natl::Size count = 0;
			natl::DynArray<natl::ByteAllocResult> allocResults;
			while (true) {
				natl::ByteAllocResult allocResult = alloc.allocate(8, minAlloc, natl::AllocateFlags::none);
				if (allocResult.mPtr == nullptr) {
					break;
				}
				ts.assertGe(minAlloc, allocResult.mSize);
				allocResults.pushBack(allocResult);
				natl::Size* ptr = reinterpret_cast<natl::Size*>(allocResult.mPtr);
				for (natl::Size i : natl::Repeat(allocResult.mSize / sizeof(natl::Size))) {
					ptr[i] = count;
					count++;
				}
			}

			count = 0;
			for (natl::ByteAllocResult allocResult : allocResults) {
				natl::Size* ptr = reinterpret_cast<natl::Size*>(allocResult.mPtr);
				for (natl::Size i : natl::Repeat(allocResult.mSize / sizeof(natl::Size))) {
					ts.assertEq(count, ptr[i]);
					count++;
				}
				natl::zeroMemory(allocResult.mPtr, allocResult.mSize);
				ts.assertTrue(alloc.deallocate(allocResult.mPtr));
			}
			ts.assertTrue(alloc.getAlloc().isEmpty());
		}

		alloc.deinit();
	}
	NATL_REGISTER_TEST(fullTest);

	static void overheadTest(test_suite_type& ts) noexcept {
		alloc_type alloc;
		alloc.init();

		natl::Size allocatedByteCount = 0;
		while (true) {
			natl::ByteAllocResult allocResult = alloc.allocate(8, minAlloc, natl::AllocateFlags::none);
			if (allocResult.mPtr == nullptr) {
				break;
			}
			ts.assertGe(minAlloc, allocResult.mSize);
			allocatedByteCount += allocResult.mSize;
		}

		ts.assertGe(alloc_type::region_manager::regionSize / 2, allocatedByteCount);
		alloc.deinit();
	}
	NATL_REGISTER_TEST(overheadTest);

	static void manyAllocsTest(test_suite_type& ts) noexcept {
		constexpr natl::Size allocCount = 10;
		constexpr natl::Size actionCount = allocCount * 256;
		
		natl::Array<alloc_type, allocCount> allocs;
		natl::Array<natl::DynArray<natl::ByteAllocResult>, allocCount> allocResults;

		for (alloc_type& alloc : allocs) {
			alloc.init();
		}

		for (auto NATL_UNUSED_PLACE_HOLDER_NAME : natl::Repeat(actionCount)) {
			natl::Size allocIndex = ts.getRNG().randomUI64(0, allocs.size() - 1);
			alloc_type& alloc = allocs[allocIndex];
			auto& results = allocResults[allocIndex];

			natl::Size allocSize = ts.getRNG().randomUI64(minAlloc, maxAlloc);
			natl::ByteAllocResult result = alloc.allocate(8, allocSize, natl::AllocateFlags::none);

			if (result.mPtr == nullptr) {

				natl::shuffle(results.begin(), results.end(), ts.getRNG());
				for (natl::ByteAllocResult allocResult : results) {
					natl::Size count = 0;
					natl::Size* ptr = reinterpret_cast<natl::Size*>(allocResult.mPtr);
					for (natl::Size i : natl::Repeat(allocResult.mSize / sizeof(natl::Size))) {
						ts.assertEqual(count, ptr[i]);
						count++;
					}
					ts.assertTrue(alloc.deallocate(allocResult.mPtr));
				}
				results.clear();

			} else {
				ts.assertGe(allocSize, result.mSize);
				natl::Size count = 0;
				natl::Size* ptr = reinterpret_cast<natl::Size*>(result.mPtr);
				for (natl::Size i : natl::Repeat(result.mSize / sizeof(natl::Size))) {
					ptr[i] = count;
					count++;
				}
				results.pushBack(result);
			}
		}

		for (alloc_type& alloc : allocs) {
			alloc.deinit();
		}
	}
	NATL_REGISTER_TEST(manyAllocsTest);
};

template<natl::Size vBlockSize, typename SlabAlloc>
struct SlabTestSuite : public natl::TestSuite<SlabTestSuite<vBlockSize, SlabAlloc>> {
	using alloc_type = SlabAlloc;
	constexpr static natl::Size blockSize = vBlockSize;
};

template<typename test_suite_type>
struct SlabTestSuiteTests {
	using alloc_type = test_suite_type::alloc_type;
	constexpr static natl::Size blockSize = test_suite_type::blockSize;

	static void allocsTest(test_suite_type& ts) noexcept {
		alloc_type alloc;
		alloc.init();

		natl::ByteAllocResult allocResult = alloc.allocate(8, blockSize, natl::AllocateFlags::zeroed);
		ts.assertNotNull(allocResult.mPtr);
		ts.assertEqual(blockSize, allocResult.mSize);
		ts.assertFalse(alloc.getAlloc().isEmpty());
		for (natl::Size i : natl::Repeat(blockSize)) {
			ts.assertEqual(natl::Byte(0), allocResult.mPtr[i]);
		}
		ts.assertTrue(alloc.deallocate(allocResult.mPtr));
		ts.assertTrue(alloc.getAlloc().isEmpty());
		alloc.deinit();
	}
	NATL_REGISTER_TEST(allocsTest);

	static void fullTest(test_suite_type& ts) noexcept {
		alloc_type alloc;
		alloc.init();

		for (natl::Size r : natl::Repeat(3)) {
			natl::Size count = 0;
			natl::DynArray<natl::ByteAllocResult> allocResults;
			while (true) {
				natl::ByteAllocResult allocResult = alloc.allocate(8, blockSize, natl::AllocateFlags::none);
				if (allocResult.mPtr == nullptr) {
					break;
				}
				ts.assertEqual(blockSize, allocResult.mSize);
				allocResults.pushBack(allocResult);
				natl::Size* ptr = reinterpret_cast<natl::Size*>(allocResult.mPtr);
				for (natl::Size i : natl::Repeat(blockSize / sizeof(natl::Size))) {
					ptr[i] = count;
					count++;
				}
			}
			ts.assertTrue(alloc.getAlloc().isFull());

			count = 0;
			for (natl::ByteAllocResult allocResult : allocResults) {
				natl::Size* ptr = reinterpret_cast<natl::Size*>(allocResult.mPtr);
				for (natl::Size i : natl::Repeat(blockSize / sizeof(natl::Size))) {
					ts.assertEq(count, ptr[i]);
					count++;
				}
				natl::zeroMemory(allocResult.mPtr, blockSize);
				ts.assertTrue(alloc.deallocate(allocResult.mPtr));
			}
			ts.assertTrue(alloc.getAlloc().isEmpty());
		}

		alloc.deinit();
	}
	NATL_REGISTER_TEST(fullTest);

	static void randomAllocsTest(test_suite_type& ts) noexcept {
		alloc_type alloc;
		alloc.init();
		constexpr natl::Size maxLiveAllocs = natl::pageSize / blockSize;
		constexpr natl::Size maxActions = maxLiveAllocs * 256;
		natl::Array<natl::ByteAllocResult, maxLiveAllocs> allocResults{};
		for (natl::Size NATL_UNUSED_PLACE_HOLDER_NAME : natl::Repeat(maxActions)) {
			natl::ByteAllocResult& allocResult = allocResults[ts.getRNG().randomUI64(0, maxLiveAllocs - 1)];
			if (allocResult.mPtr == nullptr) {
				allocResult = alloc.allocate(8, blockSize, natl::AllocateFlags::zeroed);
				if (allocResult.mPtr == nullptr) {
					continue;
				}

				ts.assertEqual(blockSize, allocResult.mSize);
				natl::Size* ptr = reinterpret_cast<natl::Size*>(allocResult.mPtr);
				for (natl::Size i : natl::Repeat(blockSize / sizeof(natl::Size))) {
					ptr[i] = i;
				}
			} else {
				natl::Size* ptr = reinterpret_cast<natl::Size*>(allocResult.mPtr);
				for (natl::Size i : natl::Repeat(blockSize / sizeof(natl::Size))) {
					ts.assertEq(i, ptr[i]);
				}
				natl::zeroMemory(allocResult.mPtr, blockSize);
				ts.assertTrue(alloc.deallocate(allocResult.mPtr));
				allocResult = natl::ByteAllocResult{};
			}
		}

		alloc.deinit();
	}
	NATL_REGISTER_TEST(randomAllocsTest);

	static void meshTest(test_suite_type& ts) noexcept {
		struct AllocInfo {
			alloc_type alloc;
			natl::DynArray<natl::ByteAllocResult> allocResults{};
		};

		natl::Byte value = natl::Byte(42);
		AllocInfo fullAlloc;
		fullAlloc.alloc.init();
		while (true) {
			natl::ByteAllocResult allocResult = fullAlloc.alloc.allocate(8, blockSize, natl::AllocateFlags::none);
			if (allocResult.mPtr != nullptr) {
				fullAlloc.allocResults.pushBack(allocResult);
				natl::fill(allocResult.mPtr, allocResult.mPtr + allocResult.mSize, value);
			} else {
				break;
			}
		}

		using base_alloc_type = natl::Decay<decltype(fullAlloc.alloc.getAlloc())>;
		natl::Array<AllocInfo, base_alloc_type::region_manager::metaDataSize / 32> allocs;
		for (AllocInfo& allocInfo : allocs) {
			allocInfo.alloc.init();
			while (true) {
				natl::ByteAllocResult allocResult = allocInfo.alloc.allocate(8, blockSize, natl::AllocateFlags::none);
				if (allocResult.mPtr != nullptr) {
					allocInfo.allocResults.pushBack(allocResult);
					natl::fill(allocResult.mPtr, allocResult.mPtr + allocResult.mSize, value);
				} else {
					break;
				}
			}
		}

		for (natl::Size allocationIndex : natl::Repeat(allocs.front().allocResults.size())) {
			natl::Size liveIndex = ts.getRNG().randomUI64(0, allocs.backIndex());

			for (natl::Size allocIndex : natl::Repeat(allocs.size())) {
				if (allocIndex == liveIndex) {
					continue;
				}

				AllocInfo& allocInfo = allocs[allocIndex];
				natl::ByteAllocResult& allocResult = allocInfo.allocResults[allocationIndex];
				allocInfo.alloc.deallocate(allocResult.mPtr);
				allocResult = natl::ByteAllocResult{};
			}
		}

		AllocInfo& targetAlloc = allocs.front();
		for (natl::Size allocIndex : natl::RepeatFrom(1, allocs.size())) {
			AllocInfo& donorAlloc = allocs[allocIndex];
			using base_alloc_type = natl::Decay<decltype(targetAlloc.alloc.getAlloc())>;
			ts.assertFalse(base_alloc_type::canMesh(100, &fullAlloc.alloc.getAlloc(), &donorAlloc.alloc.getAlloc()).mCanMesh);
			auto result = base_alloc_type::canMesh(100, &targetAlloc.alloc.getAlloc(), &donorAlloc.alloc.getAlloc());
			ts.assertTrue(result.mCanMesh);
			ts.assertEqual(1, result.mMeshMoveCount);
			ts.assertEqual(&targetAlloc.alloc.getAlloc(), result.mTargetAlloc);
			ts.assertEqual(&donorAlloc.alloc.getAlloc(), result.mDonorAlloc);

			base_alloc_type::mesh(result.mTargetAlloc, result.mDonorAlloc);
			ts.assertTrue(result.mTargetAlloc->isRootAlloc());
			ts.assertFalse(result.mDonorAlloc->isRootAlloc());
		}

		auto emptyAlloc = [&](AllocInfo& allocInfo) noexcept {
			for (natl::ByteAllocResult allocResult : allocInfo.allocResults) {
				if (allocResult.mPtr != nullptr) {
					for (natl::Size i : natl::Repeat(allocResult.mSize)) {
						ts.assertEqual(value, allocResult.mPtr[i]);
					}

					ts.assertTrue(allocInfo.alloc.deallocate(allocResult.mPtr));
				}
			}
		};
		emptyAlloc(targetAlloc);
		emptyAlloc(fullAlloc);
		ts.assertFalse(targetAlloc.alloc.getAlloc().isEmpty());

		for (natl::Size allocIndex : natl::RepeatFrom(1, allocs.size() - 1)) {
			AllocInfo& donorAlloc = allocs[allocIndex];
			emptyAlloc(donorAlloc);
			ts.assertFalse(targetAlloc.alloc.getAlloc().isEmpty());
		}
		emptyAlloc(allocs.back());
		ts.assertTrue(targetAlloc.alloc.getAlloc().isEmpty());
	}
	NATL_REGISTER_TEST(meshTest);
};

NATL_REGISTER_STD_TESTS(OsAllocTestSuite);

struct SlabBitmap16AllocTestSuite : SlabTestSuite<16, natl::BindAllocComposer<natl::SlabBitmapAllocRoot<16, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabBitmap16AllocTestSuite, SlabTestSuiteTests);

struct SlabBitmap32AllocTestSuite : SlabTestSuite<32, natl::BindAllocComposer<natl::SlabBitmapAllocRoot<32, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabBitmap32AllocTestSuite, SlabTestSuiteTests);

struct SlabBitmap64AllocTestSuite : SlabTestSuite<64, natl::BindAllocComposer<natl::SlabBitmapAllocRoot<64, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabBitmap64AllocTestSuite, SlabTestSuiteTests);

struct SlabBitmap128AllocTestSuite : SlabTestSuite<128, natl::BindAllocComposer<natl::SlabBitmapAllocRoot<128, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabBitmap128AllocTestSuite, SlabTestSuiteTests);

struct SlabBitmap256AllocTestSuite : SlabTestSuite<256, natl::BindAllocComposer<natl::SlabBitmapAllocRoot<256, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabBitmap256AllocTestSuite, SlabTestSuiteTests);

struct SlabBitmap512AllocTestSuite : SlabTestSuite<512, natl::BindAllocComposer<natl::SlabBitmapAllocRoot<512, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabBitmap512AllocTestSuite, SlabTestSuiteTests);

struct SlabList16AllocTestSuite : SlabTestSuite<16, natl::BindAllocComposer<natl::SlabListAllocRoot<16, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabList16AllocTestSuite, SlabTestSuiteTests);

struct SlabList32AllocTestSuite : SlabTestSuite<32, natl::BindAllocComposer<natl::SlabListAllocRoot<32, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabList32AllocTestSuite, SlabTestSuiteTests);

struct SlabList64AllocTestSuite : SlabTestSuite<64, natl::BindAllocComposer<natl::SlabListAllocRoot<64, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabList64AllocTestSuite, SlabTestSuiteTests);

struct SlabList128AllocTestSuite : SlabTestSuite<128, natl::BindAllocComposer<natl::SlabListAllocRoot<128, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabList128AllocTestSuite, SlabTestSuiteTests);

struct SlabList256AllocTestSuite : SlabTestSuite<256, natl::BindAllocComposer<natl::SlabListAllocRoot<256, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabList256AllocTestSuite, SlabTestSuiteTests);

struct SlabList512AllocTestSuite : SlabTestSuite<512, natl::BindAllocComposer<natl::SlabListAllocRoot<512, os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(SlabList512AllocTestSuite, SlabTestSuiteTests);

/*
struct FreeList1024AllocTestSuite : FreeListAllocTestSuite<16, 1024, natl::BindAllocComposer<natl::FreeListAllocRoot<os_alloc>::alloc, region_manager>> {};
NATL_REGISTER_TESTS(FreeList1024AllocTestSuite, FreeListAllocTestSuiteTests);
*/

NATL_TESTS_MAIN();