//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/pagedSparseArray.h>

template<typename PagedSparseArrayType>
struct PagedSparseArrayTestSuite : public natl::TestSuite<PagedSparseArrayTestSuite<PagedSparseArrayType>> {
	using paged_sparse_array_type = PagedSparseArrayType;
};

template<typename test_suite_type = PagedSparseArrayTestSuite<natl::PagedSparseArray<natl::Size>>>
struct PagedSparseArrayTestSuiteTests {
	using paged_sparse_array_type = test_suite_type::paged_sparse_array_type;
	using value_type = paged_sparse_array_type::value_type;

	constexpr static void constructionTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		ts.assertEqual(0, array.size());
		ts.assertTrue(array.empty());
		ts.assertTrue(array.isEmpty());
		ts.assertFalse(array.isNotEmpty());
		ts.assertEqual(array.begin(), array.end());
	}
	NATL_REGISTER_TEST_CONSTEXPR(constructionTest);

	constexpr static void insertionTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		auto iter0 = array.insert(0, value_type(10));
		auto iter5 = array.insert(5, value_type(20));
		auto iter10 = array.insert(10, value_type(30));

		ts.assertEqual(3, array.size());
		ts.assertFalse(array.empty());
		ts.assertEqual(value_type(10), *iter0);
		ts.assertEqual(value_type(20), *iter5);
		ts.assertEqual(value_type(30), *iter10);

		auto value0 = array.get(0);
		auto value5 = array.get(5);
		auto value10 = array.get(10);

		ts.assertTrue(value0.hasValue());
		ts.assertTrue(value5.hasValue());
		ts.assertTrue(value10.hasValue());

		ts.assertEqual(value_type(10), *value0);
		ts.assertEqual(value_type(20), *value5);
		ts.assertEqual(value_type(30), *value10);
	}
	NATL_REGISTER_TEST_CONSTEXPR(insertionTest);

	constexpr static void sparseInsertionTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(100, value_type(20));
		array.insert(1000, value_type(30));
		array.insert(10000, value_type(40));

		ts.assertEqual(4, array.size());

		ts.assertEqual(value_type(10), *array.get(1));
		ts.assertEqual(value_type(20), *array.get(100));
		ts.assertEqual(value_type(30), *array.get(1000));
		ts.assertEqual(value_type(40), *array.get(10000));

		ts.assertFalse(array.get(0).hasValue());
		ts.assertFalse(array.get(2).hasValue());
		ts.assertFalse(array.get(99).hasValue());
		ts.assertFalse(array.get(101).hasValue());
		ts.assertFalse(array.get(9999).hasValue());
	}
	NATL_REGISTER_TEST_CONSTEXPR(sparseInsertionTest);

	constexpr static void pageBoundaryTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;
		constexpr natl::Size pageSize = paged_sparse_array_type::pageSize;

		array.insert(pageSize - 1, value_type(10));
		array.insert(pageSize, value_type(20));
		array.insert(pageSize + 1, value_type(30));
		array.insert((pageSize * 2) - 1, value_type(40));
		array.insert(pageSize * 2, value_type(50));

		ts.assertEqual(5, array.size());

		ts.assertEqual(value_type(10), *array.get(pageSize - 1));
		ts.assertEqual(value_type(20), *array.get(pageSize));
		ts.assertEqual(value_type(30), *array.get(pageSize + 1));
		ts.assertEqual(value_type(40), *array.get((pageSize * 2) - 1));
		ts.assertEqual(value_type(50), *array.get(pageSize * 2));
	}
	NATL_REGISTER_TEST_CONSTEXPR(pageBoundaryTest);

	constexpr static void getTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(4, value_type(40));

		auto existingValue = array.get(4);
		auto missingValue = array.get(5);
		auto outOfRangeValue = array.get(10000);

		ts.assertTrue(existingValue.hasValue());
		ts.assertFalse(missingValue.hasValue());
		ts.assertFalse(outOfRangeValue.hasValue());

		ts.assertEqual(value_type(40), *existingValue);

		*existingValue = value_type(80);

		ts.assertEqual(value_type(80), *array.get(4));
	}
	NATL_REGISTER_TEST_CONSTEXPR(getTest);

	constexpr static void constGetTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;
		array.insert(7, value_type(70));

		const paged_sparse_array_type& constArray = array;

		auto existingValue = constArray.get(7);
		auto missingValue = constArray.get(8);

		ts.assertTrue(existingValue.hasValue());
		ts.assertFalse(missingValue.hasValue());
		ts.assertEqual(value_type(70), *existingValue);
	}
	NATL_REGISTER_TEST_CONSTEXPR(constGetTest);

	constexpr static void getIterTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(3, value_type(30));
		array.insert(20, value_type(200));

		auto iter3 = array.getIter(3);
		auto iter20 = array.getIter(20);
		auto missingIter = array.getIter(10);

		ts.assertNotEqual(iter3, array.end());
		ts.assertNotEqual(iter20, array.end());
		ts.assertEqual(missingIter, array.end());

		ts.assertEqual(value_type(30), *iter3);
		ts.assertEqual(value_type(200), *iter20);
	}
	NATL_REGISTER_TEST_CONSTEXPR(getIterTest);

	constexpr static void overwriteTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(5, value_type(10));
		ts.assertEqual(value_type(10), *array.get(5));

		array.insert(5, value_type(20));
		ts.assertEqual(value_type(20), *array.get(5));
	}
	NATL_REGISTER_TEST_CONSTEXPR(overwriteTest);

	constexpr static void eraseTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(2, value_type(20));
		array.insert(4, value_type(40));
		array.insert(6, value_type(60));

		ts.assertEqual(3, array.size());

		ts.assertTrue(array.erase(4));
		ts.assertEqual(2, array.size());
		ts.assertFalse(array.get(4).hasValue());

		ts.assertFalse(array.erase(4));
		ts.assertEqual(2, array.size());

		ts.assertFalse(array.erase(10000));
		ts.assertEqual(2, array.size());

		ts.assertEqual(value_type(20), *array.get(2));
		ts.assertEqual(value_type(60), *array.get(6));
	}
	NATL_REGISTER_TEST_CONSTEXPR(eraseTest);

	constexpr static void clearTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(100, value_type(20));
		array.insert(1000, value_type(30));

		ts.assertEqual(3, array.size());

		array.clear();

		ts.assertEqual(0, array.size());
		ts.assertTrue(array.empty());
		ts.assertTrue(array.isEmpty());
		ts.assertFalse(array.isNotEmpty());
	}
	NATL_REGISTER_TEST_CONSTEXPR(clearTest);

	constexpr static void iteratorTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(5, value_type(20));
		array.insert(20, value_type(30));
		array.insert(100, value_type(40));

		natl::Size count = 0;
		value_type sum = 0;

		for (auto iter = array.begin(); iter != array.end(); ++iter) {
			sum += *iter;
			count++;
		}

		ts.assertEqual(4, count);
		ts.assertEqual(value_type(100), sum);
	}
	NATL_REGISTER_TEST_CONSTEXPR(iteratorTest);

	constexpr static void iteratorOrderTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(50, value_type(50));
		array.insert(2, value_type(2));
		array.insert(30, value_type(30));
		array.insert(10, value_type(10));

		auto iter = array.begin();

		ts.assertEqual(value_type(2), *iter);
		++iter;
		ts.assertEqual(value_type(10), *iter);
		++iter;
		ts.assertEqual(value_type(30), *iter);
		++iter;
		ts.assertEqual(value_type(50), *iter);
		++iter;
		ts.assertEqual(iter, array.end());
	}
	NATL_REGISTER_TEST_CONSTEXPR(iteratorOrderTest);

	constexpr static void iteratorAcrossPagesTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;
		constexpr natl::Size pageSize = paged_sparse_array_type::pageSize;

		array.insert(0, value_type(10));
		array.insert(pageSize - 1, value_type(20));
		array.insert(pageSize, value_type(30));
		array.insert(pageSize * 2, value_type(40));

		auto iter = array.begin();

		ts.assertEqual(value_type(10), *iter);
		++iter;
		ts.assertEqual(value_type(20), *iter);
		++iter;
		ts.assertEqual(value_type(30), *iter);
		++iter;
		ts.assertEqual(value_type(40), *iter);
		++iter;
		ts.assertEqual(iter, array.end());
	}
	NATL_REGISTER_TEST_CONSTEXPR(iteratorAcrossPagesTest);

	constexpr static void iteratorModificationTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(5, value_type(20));
		array.insert(10, value_type(30));

		for (auto iter = array.begin(); iter != array.end(); ++iter) {
			*iter *= value_type(2);
		}

		ts.assertEqual(value_type(20), *array.get(1));
		ts.assertEqual(value_type(40), *array.get(5));
		ts.assertEqual(value_type(60), *array.get(10));
	}
	NATL_REGISTER_TEST_CONSTEXPR(iteratorModificationTest);

	constexpr static void constIteratorTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(5, value_type(20));
		array.insert(10, value_type(30));

		const paged_sparse_array_type& constArray = array;

		value_type sum = 0;
		natl::Size count = 0;

		for (auto iter = constArray.begin(); iter != constArray.end(); ++iter) {
			sum += *iter;
			count++;
		}

		ts.assertEqual(3, count);
		ts.assertEqual(value_type(60), sum);
	}
	NATL_REGISTER_TEST_CONSTEXPR(constIteratorTest);

	constexpr static void eraseDuringSparseStateTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(5, value_type(20));
		array.insert(10, value_type(30));
		array.insert(50, value_type(40));

		ts.assertTrue(array.erase(1));
		ts.assertTrue(array.erase(10));

		auto iter = array.begin();

		ts.assertEqual(value_type(20), *iter);
		++iter;
		ts.assertEqual(value_type(40), *iter);
		++iter;
		ts.assertEqual(iter, array.end());
	}
	NATL_REGISTER_TEST_CONSTEXPR(eraseDuringSparseStateTest);

	constexpr static void copyConstructionTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(20, value_type(20));

		paged_sparse_array_type copiedArray(array);

		ts.assertEqual(array.size(), copiedArray.size());
		ts.assertEqual(value_type(10), *copiedArray.get(1));
		ts.assertEqual(value_type(20), *copiedArray.get(20));
	}
	NATL_REGISTER_TEST_CONSTEXPR(copyConstructionTest);

	constexpr static void moveConstructionTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		array.insert(1, value_type(10));
		array.insert(20, value_type(20));

		paged_sparse_array_type movedArray(natl::move(array));

		ts.assertEqual(value_type(10), *movedArray.get(1));
		ts.assertEqual(value_type(20), *movedArray.get(20));
	}
	NATL_REGISTER_TEST_CONSTEXPR(moveConstructionTest);

	constexpr static void largeScaleStressTest(test_suite_type& ts) noexcept {
		paged_sparse_array_type array;

		constexpr natl::Size operationCount = 100000;
		constexpr natl::Size pageSize = paged_sparse_array_type::pageSize;

		for (natl::Size i = 0; i < operationCount; i++) {
			array.insert(i * 3, value_type(i));
		}

		ts.assertEqual(operationCount, array.size());

		for (natl::Size i = 0; i < operationCount; i++) {
			auto value = array.get(i * 3);
			ts.assertTrue(value.hasValue());
			ts.assertEqual(value_type(i), *value);

			ts.assertFalse(array.get((i * 3) + 1).hasValue());
			ts.assertFalse(array.get((i * 3) + 2).hasValue());
		}

		for (natl::Size i = 0; i < operationCount; i += 2) {
			ts.assertTrue(array.erase(i * 3));
		}

		ts.assertEqual(operationCount / 2, array.size());

		for (natl::Size i = 0; i < operationCount; i++) {
			if ((i % 2) == 0) {
				ts.assertFalse(array.get(i * 3).hasValue());
			} else {
				auto value = array.get(i * 3);
				ts.assertTrue(value.hasValue());
				ts.assertEqual(value_type(i), *value);
			}
		}

		for (natl::Size i = 0; i < operationCount; i += 2) {
			array.insert(i * 3, value_type(i * 10));
		}

		ts.assertEqual(operationCount, array.size());

		for (natl::Size i = 0; i < operationCount; i++) {
			auto value = array.get(i * 3);
			ts.assertTrue(value.hasValue());

			if ((i % 2) == 0) {
				ts.assertEqual(value_type(i * 10), *value);
			} else {
				ts.assertEqual(value_type(i), *value);
			}
		}

		natl::Size iterCount = 0;

		for (auto iter = array.begin(); iter != array.end(); ++iter) {
			iterCount++;
		}

		ts.assertEqual(operationCount, iterCount);

		for (natl::Size i = 0; i < operationCount; i++) {
			auto iter = array.getIter(i * 3);
			ts.assertNotEqual(iter, array.end());
		}

		for (natl::Size page = 0; page < 100; page++) {
			natl::Size baseIndex = page * pageSize;

			array.insert(baseIndex, value_type(baseIndex));
			array.insert(baseIndex + pageSize - 1, value_type(baseIndex + pageSize - 1));

			ts.assertEqual(value_type(baseIndex), *array.get(baseIndex));
			ts.assertEqual(
				value_type(baseIndex + pageSize - 1),
				*array.get(baseIndex + pageSize - 1)
			);
		}

		for (natl::Size i = 0; i < operationCount; i += 3) {
			array.erase(i * 3);
		}

		for (natl::Size i = 0; i < operationCount; i += 3) {
			array.insert(i * 3, value_type(i + 12345));
		}

		for (natl::Size i = 0; i < operationCount; i++) {
			auto value = array.get(i * 3);
			ts.assertTrue(value.hasValue());
		}

		array.clear();

		ts.assertEqual(0, array.size());
		ts.assertTrue(array.empty());
		ts.assertEqual(array.begin(), array.end());

		for (natl::Size i = 0; i < operationCount; i++) {
			array.insert((operationCount - i) * 7, value_type(i));
		}

		ts.assertEqual(operationCount, array.size());

		natl::Size finalIterCount = 0;

		for (auto iter = array.begin(); iter != array.end(); ++iter) {
			finalIterCount++;
		}

		ts.assertEqual(operationCount, finalIterCount);
	}
	NATL_REGISTER_TEST(largeScaleStressTest);
};

struct PagedSparseArrayTestSuiteDefault : public PagedSparseArrayTestSuite<natl::PagedSparseArray<natl::Size>> {};
NATL_REGISTER_TESTS(PagedSparseArrayTestSuiteDefault, PagedSparseArrayTestSuiteTests);

struct PagedSparseArrayTestSuitePageSize8 : public PagedSparseArrayTestSuite<natl::PagedSparseArray<natl::Size, 8>> {};
NATL_REGISTER_TESTS(PagedSparseArrayTestSuitePageSize8, PagedSparseArrayTestSuiteTests);

struct SmallPagedSparseArrayTestSuite : public PagedSparseArrayTestSuite<natl::SmallPagedSparseArray<natl::Size, 8, 8>> {};
NATL_REGISTER_TESTS(SmallPagedSparseArrayTestSuite, PagedSparseArrayTestSuiteTests);

NATL_TESTS_MAIN();

