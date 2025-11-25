#pragma once

//@begin_non_modules
//natl
#define NATL_DISABLE_CONSTEXPR_TESTS
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/stringConvert.h>

//@export
namespace impl {
	template<typename IntHashSetType, typename StringHashSetType>
	struct BaseHashSetTestSuite : public natl::TestSuite<BaseHashSetTestSuite<IntHashSetType, StringHashSetType>> {
		using int_hash_set_type = IntHashSetType;
		using string_hash_set_type = StringHashSetType;
	};

	constexpr inline natl::Size maxCount = 1000;

	template<typename test_suite_type = BaseHashSetTestSuite<natl::PersistentHashSet<natl::Size>, natl::PersistentHashSet<natl::String>>>
	struct BaseHashSetTestSuiteTests {
		using int_hash_set_type = typename test_suite_type::int_hash_set_type;
		using string_hash_set_type = typename test_suite_type::string_hash_set_type;

		constexpr static natl::Size getIterationCount() noexcept {
			if (natl::isConstantEvaluated()) {
				return 10;
			} else {
				return maxCount;
			}
		}

		constexpr static int_hash_set_type generateTestHashSetInt() noexcept {
			int_hash_set_type map;
			for (natl::Size i : natl::Repeat(getIterationCount())) {
				map.insert(i);
			}
			return map;
		}

		constexpr static string_hash_set_type generateTestHashSetString() noexcept {
			string_hash_set_type map;
			for (natl::Size i : natl::Repeat(getIterationCount())) {
				map.insert(natl::intToStringDecimal(i));
			}
			return map;
		}

		constexpr static void insertIntTest(test_suite_type& ts) noexcept {
			natl::Size count = getIterationCount();
			auto intSet = generateTestHashSetInt();
			natl::Array<natl::Bool, maxCount> vistedMap;
			for (natl::Size i : natl::Repeat(count)) {
				vistedMap[i] = false;
			}
			ts.assertEqual(count, intSet.size());

			for (auto& value : intSet) {
				ts.assertGt(count, value);

				if (value >= count) {
					continue;
				}

				ts.assertFalse(vistedMap[value]);
				vistedMap[value] = true;
			}

			for (natl::Size i : natl::Repeat(count)) {
				ts.assertTrue(vistedMap[i]);
			}
		}
		NATL_REGISTER_TEST_CONSTEXPR(insertIntTest);

		constexpr static void insertStringTest(test_suite_type& ts) noexcept {
			natl::Size count = getIterationCount();
			auto stringSet = generateTestHashSetString();
			natl::Array<natl::Bool, maxCount> vistedMap;
			for (natl::Size i : natl::Repeat(count)) {
				vistedMap[i] = false;
			}
			ts.assertEqual(count, stringSet.size());


			for (auto& value : stringSet) {
				auto intValueExpect = natl::stringDecimalToInt<natl::Size>(value.toStringView());
				ts.assertTrue(intValueExpect.hasValue());
				if (!intValueExpect.hasValue()) {
					continue;
				}
				auto intValue = intValueExpect.value();
				ts.assertGt(count, intValue);

				if (intValue >= count) {
					continue;
				}

				ts.assertFalse(vistedMap[intValue]);
				vistedMap[intValue] = true;
			}

			for (natl::Size i : natl::Repeat(count)) {
				ts.assertTrue(vistedMap[i]);
			}
		}
		NATL_REGISTER_TEST_CONSTEXPR(insertStringTest);

		constexpr static void formatTest(test_suite_type& ts) noexcept {
			auto testSet = generateTestHashSetInt();
			ts.assertFalse(natl::sformat(
				natl::formatArg(testSet,
					natl::formatValue(natl::IntFormat::binary)
				)
			).isEmpty());

			ts.assertFalse(natl::sformat(
				natl::formatArg(testSet,
					natl::formatValue(natl::IntFormat::binary)
				)
			).isEmpty());

			ts.assertFalse(natl::sformat(
				natl::formatArgText<"v: hex">(testSet)
			).isEmpty());
		}
		NATL_REGISTER_TEST_CONSTEXPR(formatTest);
	};
}
