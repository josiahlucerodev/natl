#pragma once

//@begin_non_modules
//natl
#define NATL_DISABLE_CONSTEXPR_TESTS
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>

//@export
namespace impl {
	template<typename IntHashMapType, typename StringHashMapType>
	struct BaseHashMapTestSuite : public natl::TestSuite<BaseHashMapTestSuite<IntHashMapType, StringHashMapType>> {
		using int_hash_map_type = IntHashMapType;
		using string_hash_map_type = StringHashMapType;
	};

	constexpr inline natl::Size maxCount = 1000;

	template<typename test_suite_type>
	struct BaseHashMapTestSuiteTests {
		using int_hash_map_type = typename test_suite_type::int_hash_map_type;
		using string_hash_map_type = typename test_suite_type::string_hash_map_type;

		constexpr static natl::Size getIterationCount() noexcept {
			if (natl::isConstantEvaluated()) {
				return 10;
			} else {
				return maxCount;
			}
		}

		constexpr static int_hash_map_type generateTestHashMapInt() noexcept {
			int_hash_map_type map;
			for (natl::Size i : natl::Repeat(getIterationCount())) {
				map.insert(i, i);
			}
			return map;
		}

		constexpr static string_hash_map_type generateTestHashMapString() noexcept {
			string_hash_map_type map;
			for (natl::Size i : natl::Repeat(getIterationCount())) {
				map.insert(natl::intToStringDecimal(i), natl::intToStringDecimal(i));
			}
			return map;
		}

		constexpr static void insertIntTest(test_suite_type& ts) noexcept {
			natl::Size count = getIterationCount();
			auto intMap = generateTestHashMapInt();
			natl::Array<natl::Bool, maxCount> vistedMap;
			for (natl::Size i : natl::Repeat(count)) {
				vistedMap[i] = false;
			}
			ts.assertEqual(count, intMap.size());

			for (auto [key, value] : intMap) {
				ts.assertEqual(key, value);
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
			auto stringMap = generateTestHashMapString();
			natl::Array<natl::Bool, maxCount> vistedMap;
			for (natl::Size i : natl::Repeat(count)) {
				vistedMap[i] = false;
			}
			ts.assertEqual(count, stringMap.size());


			for (const auto& [key, value] : stringMap) {
				auto intValueExpect = natl::stringDecimalToInt<natl::Size>(value.toStringView());
				ts.assertTrue(intValueExpect.hasValue());
				ts.assertEqual(key, value);
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
			auto testMap = generateTestHashMapInt();
			ts.assertFalse(natl::sformat(
				natl::formatArg(testMap,
					natl::formatKey(natl::IntFormat::hexadecimal)
				)
			).isEmpty());

			ts.assertFalse(natl::sformat(
				natl::formatArg(testMap,
					natl::formatKey(natl::IntFormat::hexadecimal),
					natl::formatValue(natl::IntFormat::binary)
				)
			).isEmpty());

			ts.assertFalse(natl::sformat(
				natl::formatArgText<"v: hex", "k: hex">(testMap)
			).isEmpty());
		}
		NATL_REGISTER_TEST_CONSTEXPR(formatTest);
	};
}