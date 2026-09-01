//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/typePack.h>
#include <natl/util/match.h>

enum class MatchTypeTestKind {
	i8,
	i64,
	f32,
	f64
};

template<MatchTypeTestKind Value, typename ValueType>
struct MatchTypeTestElement {
	constexpr static MatchTypeTestKind value = Value;
	using value_type = ValueType;
};

using MatchTypeI8 = MatchTypeTestElement<MatchTypeTestKind::i8, natl::i8>;
using MatchTypeI64 = MatchTypeTestElement<MatchTypeTestKind::i64, natl::i64>;
using MatchTypeF32 = MatchTypeTestElement<MatchTypeTestKind::f32, natl::f32>;
using MatchTypeF64 = MatchTypeTestElement<MatchTypeTestKind::f64, natl::f64>;

struct MatchTypeTestSuite : public natl::TestSuite<MatchTypeTestSuite> {};

template<typename test_suite_type = MatchTypeTestSuite>
struct MatchTypeTestSuiteTests {
	constexpr static void matchI8(test_suite_type& ts) noexcept {
		MatchTypeTestKind kind = MatchTypeTestKind::i8;
		natl::Bool called = false;

		const natl::Bool matched = natl::matchType<MatchTypeTestKind, MatchTypeI8, MatchTypeI64, MatchTypeF32, MatchTypeF64>(
			kind,
			[&]<typename Type>() constexpr noexcept {
			if constexpr (natl::IsSameC<Type, natl::i8>) {
				called = true;
			}
		}
		);

		ts.assertEqual(true, matched);
		ts.assertEqual(true, called);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchI8);

	constexpr static void matchI64(test_suite_type& ts) noexcept {
		MatchTypeTestKind kind = MatchTypeTestKind::i64;
		natl::Bool called = false;

		const natl::Bool matched = natl::matchType<MatchTypeTestKind,
			MatchTypeI8,
			MatchTypeI64,
			MatchTypeF32,
			MatchTypeF64
		>(
			kind,
			[&]<typename Type>() constexpr noexcept {
			if constexpr (natl::IsSameC<Type, natl::i64>) {
				called = true;
			}
		}
		);

		ts.assertEqual(true, matched);
		ts.assertEqual(true, called);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchI64);

	constexpr static void matchF32(test_suite_type& ts) noexcept {
		MatchTypeTestKind kind = MatchTypeTestKind::f32;
		natl::Bool called = false;

		const natl::Bool matched = natl::matchType<MatchTypeTestKind, MatchTypeI8, MatchTypeI64, MatchTypeF32, MatchTypeF64>(
			kind,
			[&]<typename Type>() constexpr noexcept {
			if constexpr (natl::IsSameC<Type, natl::f32>) {
				called = true;
			}
		}
		);

		ts.assertEqual(true, matched);
		ts.assertEqual(true, called);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchF32);

	constexpr static void matchF64(test_suite_type& ts) noexcept {
		MatchTypeTestKind kind = MatchTypeTestKind::f64;
		natl::Bool called = false;

		const natl::Bool matched = natl::matchType<MatchTypeTestKind, MatchTypeI8, MatchTypeI64, MatchTypeF32, MatchTypeF64 >(
			kind,
			[&]<typename Type>() constexpr noexcept {
			if constexpr (natl::IsSameC<Type, natl::f64>) {
				called = true;
			}
		}
		);

		ts.assertEqual(true, matched);
		ts.assertEqual(true, called);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchF64);

	constexpr static void matchOnlyCallsOne(test_suite_type& ts) noexcept {
		MatchTypeTestKind kind = MatchTypeTestKind::f32;
		natl::Size numberOfCalls = 0;

		const natl::Bool matched = natl::matchType<MatchTypeTestKind,
			MatchTypeI8, MatchTypeI64, MatchTypeF32,
			MatchTypeF64
		>(
			kind,
			[&]<typename Type>() constexpr noexcept {
			numberOfCalls++;
		}
		);

		ts.assertEqual(true, matched);
		ts.assertEqual(natl::Size(1), numberOfCalls);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchOnlyCallsOne);

	constexpr static void matchValueType(test_suite_type& ts) noexcept {
		MatchTypeTestKind kind = MatchTypeTestKind::i64;
		natl::Size typeSize = 0;

		const natl::Bool matched = natl::matchType<MatchTypeTestKind, MatchTypeI8, MatchTypeI64, MatchTypeF32, MatchTypeF64>(
			kind,
			[&]<typename Type>() constexpr noexcept {
			typeSize = sizeof(Type);
		}
		);

		ts.assertEqual(true, matched);
		ts.assertEqual(natl::Size(sizeof(natl::i64)), typeSize);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchValueType);

	constexpr static void matchCanUseSelectedType(test_suite_type& ts) noexcept {
		MatchTypeTestKind kind = MatchTypeTestKind::f64;
		natl::f64 result = 0.0;

		const natl::Bool matched = natl::matchType<MatchTypeTestKind, MatchTypeI8, MatchTypeI64, MatchTypeF32, MatchTypeF64>(
			kind,
			[&]<typename Type>() constexpr noexcept {
				Type value = Type(42);
				result = static_cast<natl::f64>(value);
			}
		);

		ts.assertEqual(true, matched);
		ts.assertEqual(natl::f64(42), result);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchCanUseSelectedType);

	constexpr static void matchDescriptorValues(test_suite_type& ts) noexcept {
		ts.assertEqual(MatchTypeTestKind::i8, MatchTypeI8::value);
		ts.assertEqual(MatchTypeTestKind::i64, MatchTypeI64::value);
		ts.assertEqual(MatchTypeTestKind::f32, MatchTypeF32::value);
		ts.assertEqual(MatchTypeTestKind::f64, MatchTypeF64::value);
	}
	NATL_REGISTER_TEST_CONSTEXPR(matchDescriptorValues);
};

NATL_REGISTER_STD_TESTS(MatchTypeTestSuite);

NATL_TESTS_MAIN();
