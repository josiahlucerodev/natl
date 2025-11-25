//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/colony.h>

struct ColonyTestSuite : public natl::TestSuite<ColonyTestSuite> {};

template<typename test_suite_type = ColonyTestSuite>
struct ColonyTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::Colony<natl::Size> colony(natl::ColonyLimits(30, 300));

		for (natl::Size index : natl::Repeat(100)) {
			colony.insert(index);
		}
		ts.assertEqual(100, colony.size());

		colony.shrink();
		ts.assertEqual(100, colony.size());

		auto iter = colony.getIteratorAtIndex(1).value();
		for ([[maybe_unused]] natl::Size index : natl::Repeat(3)) {
			colony.erase(iter);
			iter++;
		}
		iter = colony.getIteratorAtIndex(5).value();
		for ([[maybe_unused]] natl::Size index : natl::Repeat(2)) {
			colony.erase(iter);
			iter++;
		}
		colony.erase(colony.getIteratorAtIndex(4).value());

		natl::Colony<natl::Size> colony2 = natl::move(colony);
		ts.assertTrue(colony.isEmpty());
		ts.assertLt(0, colony2.size());

		colony2.reshape(natl::ColonyLimits(30, 50), 2);
		ts.assertLt(0, colony2.size());
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(ColonyTestSuite);

NATL_TESTS_MAIN();