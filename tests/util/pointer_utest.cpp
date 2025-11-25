//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/pointer.h>
#include <natl/system/printFormatted.h>
#include <natl/container/smallDynArray.h>

struct PointerTestSuite : public natl::TestSuite<PointerTestSuite> {};

template<typename test_suite_type = PointerTestSuite >
struct PointerTestSuiteTests {
	constexpr static void sharedPtrTest(test_suite_type& ts) noexcept {
		natl::WeakPtr<int> weakPtr{};
		{
			natl::SharedPtr<int> sharedPtr(natl::SharedPtrFusedConstruct{}, natl::DefaultAllocator{}, 1);
			weakPtr = sharedPtr;
			ts.assertEqual(1, *sharedPtr);
			ts.assertFalse(weakPtr.isEmpty());
			ts.assertEqual(1, *weakPtr.lock());
		}
		natl::SharedPtr<int> sharedPtr2 = weakPtr.lock();
		ts.assertTrue(sharedPtr2.isEmpty());
	}
	NATL_REGISTER_TEST(sharedPtrTest);

	constexpr static void largeSharedPtrTest(test_suite_type& ts) noexcept {
		using array_type = natl::Array<natl::ui64, 1024>;
		array_type& evalue = *(new array_type());
		for (natl::ui64 i : natl::Repeat(array_type::size())) {
			evalue[i] = i;
		}
		
		natl::SmallDynArray<natl::SharedPtr<array_type>, 128> ptrArray;
		for ([[maybe_unused]] natl::ui64 i : natl::Repeat(1024)) {
			ptrArray.pushBack(natl::makeShared<array_type>(evalue));
		}

		for (natl::WeakPtr<array_type> ptr : ptrArray) {
			ts.assertNotNull(ptr.lock().get());
			array_type& value = *ptr.lock().get();
			ts.assertTrue([&]() -> natl::Bool {
				for (natl::ui64 i : natl::Repeat(array_type::size())) {
					if (value[i] != evalue[i]) {
						return false;
					}
				}
				return true;
			}());
		}
		delete& evalue;
	}
	NATL_REGISTER_TEST(largeSharedPtrTest);

	enum struct TestEnum : natl::ui16 {
		red = 9,
		green = 1
	};

	constexpr static void packedPtrTest(test_suite_type& ts) noexcept {
		natl::PackedPtrAndSmallData<natl::Size, TestEnum> the(new natl::Size, TestEnum::red);
		*the.getPtr() = 3;
		ts.assertEqual(3, *the.getPtr());
		ts.assertEqual(TestEnum::red, the.getSmallData());

		*the.getPtr() = 4;
		the.setPtr(the.getPtr());
		the.setSmallData(TestEnum::green);

		ts.assertEqual(4, *the.getPtr());
		ts.assertEqual(TestEnum::green, the.getSmallData());

		delete the.getPtr();
	}
	NATL_REGISTER_TEST_CONSTEXPR(packedPtrTest);
};

NATL_REGISTER_STD_TESTS(PointerTestSuite);

NATL_TESTS_MAIN();