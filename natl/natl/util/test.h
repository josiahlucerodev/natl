#pragma once

//own
#include "../util/basicTypes.h"
#include "../system/printFormatted.h"
#include "../system/printColor.h"
#include "../fundamental/option.h"

//interface
namespace natl {
	struct TestTypeStdOut {
	public:
		//constructor
		TestTypeStdOut() noexcept { natl::print("default constructed"); }
		TestTypeStdOut(TestTypeStdOut&) noexcept { natl::println("ref constructed"); }
		TestTypeStdOut(const TestTypeStdOut&) noexcept { natl::println("const ref constructed"); }
		TestTypeStdOut(TestTypeStdOut&&) noexcept { natl::println("moved constructed"); }
		TestTypeStdOut(const TestTypeStdOut&&) noexcept { natl::println("const moved constructed"); }

		//destructor 
		~TestTypeStdOut() noexcept { natl::println("destructed\n"); }

		//util
		constexpr TestTypeStdOut& self() { return *this; }

		//assignment 
		TestTypeStdOut& operator=(TestTypeStdOut&) { natl::println("ref assigned"); return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut&) { natl::println("const ref assigned"); return self(); }
		TestTypeStdOut& operator=(TestTypeStdOut&&) noexcept { natl::println("moved assigned"); return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut&&) noexcept { natl::println("const moved assigned"); return self(); }
	};
	//custom 
	struct TestTypeCustomTrivialStdOut {
		ui8 addressableData;
	public:
		//constructor
		TestTypeCustomTrivialStdOut() noexcept : addressableData(0) { natl::println("default constructed"); }
		TestTypeCustomTrivialStdOut(TestTypeCustomTrivialStdOut&) noexcept : addressableData(0) { natl::println("ref constructed"); }
		TestTypeCustomTrivialStdOut(const TestTypeCustomTrivialStdOut&) noexcept : addressableData(0) { natl::println("const ref constructed"); }
		TestTypeCustomTrivialStdOut(TestTypeCustomTrivialStdOut&&) noexcept : addressableData(0) { natl::println("moved constructed"); }
		TestTypeCustomTrivialStdOut(const TestTypeCustomTrivialStdOut&&) noexcept : addressableData(0) { natl::println("const moved constructed"); }

		//destructor 
		~TestTypeCustomTrivialStdOut() noexcept { natl::println("destructed"); }

		//util
		constexpr TestTypeCustomTrivialStdOut& self() { return *this; }

		//assignment 
		TestTypeCustomTrivialStdOut& operator=(TestTypeCustomTrivialStdOut&) { natl::println("ref assigned"); return self(); }
		TestTypeCustomTrivialStdOut& operator=(const TestTypeCustomTrivialStdOut&) { natl::println("const ref assigned"); return self(); }
		TestTypeCustomTrivialStdOut& operator=(TestTypeCustomTrivialStdOut&&) noexcept { natl::println("moved assigned"); return self(); }
		TestTypeCustomTrivialStdOut& operator=(const TestTypeCustomTrivialStdOut&&) noexcept { natl::println("const moved assigned"); return self(); }

		//element access 
		const ui8* getAddress() const noexcept { return &addressableData; }
	};

	template<>
	struct IsTriviallyCompareableV<TestTypeCustomTrivialStdOut>
		: TrueType {};

	template<>
	struct IsTriviallyRelocatableV<TestTypeCustomTrivialStdOut>
		: TrueType {};
	template<>
	struct IsTriviallyConstructibleV<TestTypeCustomTrivialStdOut>
		: TrueType {};
	template<>
	struct IsTriviallyDestructibleV<TestTypeCustomTrivialStdOut>
		: TrueType {};

	template<>
	struct IsTriviallyConstRefConstructibleV<TestTypeCustomTrivialStdOut>
		: TrueType {};
	template<>
	struct IsTriviallyMoveConstructibleV<TestTypeCustomTrivialStdOut>
		: TrueType {};

	template<>
	struct IsTriviallyConstRefAssignableV<TestTypeCustomTrivialStdOut>
		: TrueType {};
	template<>
	struct IsTriviallyMoveAssignableV<TestTypeCustomTrivialStdOut>
		: TrueType {};

	enum struct TestType {
		root,
		leaf,
		node,
	};

	struct Test {
		ConstStringView from;
		String name;
		Size successCount;
		Size failureCount;
		TestType testType;
		Bool leafPrintedNewLine;

		constexpr Test() noexcept = default;
		constexpr Test(const ConstStringView fromIn, const String& nameIn, const TestType testTypeIn) noexcept
			: from(), name(), successCount(), failureCount() {
			successCount = 0;
			failureCount = 0;
			testType = testTypeIn;
			leafPrintedNewLine = false;
			if (!isConstantEvaluated()) {
				from = fromIn;
				name = nameIn;
				if (testType == TestType::root) {
					natl::printlnf("natl: ", from, " test ", name);
				} else if (testType == TestType::node) {
					natl::printlnfc("natl: ", from, " test ", PrintColor::yellow, name);
				} else {
					natl::printf("natl: ", from, " test ", name);
				}
			}
		}
		constexpr ~Test() noexcept {
			if (!isConstantEvaluated()) {
				if (testType == TestType::leaf) {
					if (noFailures()) {
						natl::printlnfc(": ", PrintColor::green, successCount, " tests passed");
					} else {
						natl::printlnfc("natl: ", from, " test ", name, ": ", 
							PrintColor::green, successCount, " tests passed ", 
							PrintColor::red, failureCount, " tests failed");
					}
				} 
			}
		}
		constexpr void testIfShouldPrint() noexcept {
			if (!isConstantEvaluated()) {
				if (testType == TestType::leaf) {
					if (failureCount > 0 && !leafPrintedNewLine) {
						natl::printf(FormatNewLine{});
						leafPrintedNewLine = true;
					}
				}
			}
		}
		constexpr void passedTest() noexcept { 
			successCount += 1; 
			testIfShouldPrint();
		}
		constexpr void failedTest() noexcept { 
			failureCount += 1; 
			testIfShouldPrint();
		}
		constexpr Bool noFailures() noexcept { return failureCount == 0; }
		constexpr operator Bool() noexcept { return noFailures(); }
	};

	template<typename... AssertArgs>
	constexpr void testAssert(Test& test, Bool condition, AssertArgs&&... assertArgs) noexcept {
		if (condition) {
			test.passedTest();
		} else {
			if (natl::isConstantEvaluated()) {
				natl::constantEvaluatedError();
			} else {
				test.failedTest();
				natl::printlnfc("natl: ", test.from, natl::PrintColor::red, 
					" test failed: ", forward<AssertArgs>(assertArgs)...);
			}
		}
	}

	template<typename... AssertArgs>
	constexpr void testAssertFail(Test& test, AssertArgs&&... assertArgs) noexcept {
		testAssert<AssertArgs...>(test, false, natl::forward<AssertArgs>(assertArgs)...);
	}


	template<typename LhsType, typename RhsType, typename... AssertArgs>
		requires(requires(const LhsType& expected, const RhsType& actual) { {expected == actual} -> IsConvertibleC<Bool>; } )
	constexpr void testAssertEquals(Test& test, const LhsType& expected, const RhsType& actual, AssertArgs&&... assertArgs) noexcept {
		if (expected == actual) {
			test.passedTest();
		} else {
			if (natl::isConstantEvaluated()) {
				natl::constantEvaluatedError();
			} else {
				test.failedTest();
				if constexpr(IsFormattableC<Decay<LhsType>, natl::Ascii> && IsFormattableC<Decay<RhsType>, natl::Ascii>) {
					natl::printlnfc("natl: ", test.from, natl::PrintColor::red, 
						" test failed: ", forward<AssertArgs>(assertArgs)...,
						" expected: ", expected, ", actual: ", actual);
				} else {
					natl::printlnfc("natl: ", test.from, natl::PrintColor::red, 
						" test failed: ", forward<AssertArgs>(assertArgs)...);
				}
			}
		}
	}

	template<typename LhsType, typename RhsType, typename... AssertArgs>
		requires(requires(const LhsType& expected, const RhsType& actual) { { expected == actual } -> IsConvertibleC<Bool>; })
	constexpr void testAssertNotEquals(Test& test, const LhsType& expected, const RhsType& actual, AssertArgs&&... assertArgs) noexcept {
		if (expected != actual) {
			test.passedTest();
		} else {
			if (natl::isConstantEvaluated()) {
				natl::constantEvaluatedError();
			} else {
				test.failedTest();
				if constexpr (IsFormattableC<Decay<LhsType>, natl::Ascii> && IsFormattableC<Decay<RhsType>, natl::Ascii>) {
					natl::printlnfc("natl: ", test.from, natl::PrintColor::red,
						" test failed: ", forward<AssertArgs>(assertArgs)...,
						" expected: ", expected, ", actual: ", actual);
				} else {
					natl::printlnfc("natl: ", test.from, natl::PrintColor::red,
						" test failed: ", forward<AssertArgs>(assertArgs)...);
				}
			}
		}
	}

	template<typename LhsType, typename RhsType, typename... AssertArgs>
		requires(requires(const LhsType& expected, const RhsType& actual) { { expected == actual } -> IsConvertibleC<Bool>; })
	constexpr void testOptionAssertEquals(Test& test, const LhsType& expected, const Option<RhsType>& actual, AssertArgs&&... assertArgs) noexcept {
		if (actual.hasValue() && (expected == actual.value())) {
			test.passedTest();
		} else {
			if (natl::isConstantEvaluated()) {
				natl::constantEvaluatedError();
			}
			else {
				test.failedTest();

				if constexpr (IsFormattableC<Decay<LhsType>, natl::Ascii> && IsFormattableC<Decay<RhsType>, natl::Ascii>) {
					if (actual.hasValue()) {
						natl::printlnfc("natl: ", test.from, natl::PrintColor::red,
							" test failed: ", forward<AssertArgs>(assertArgs)...,
							" expected: ", expected, ", actual: ", actual);
					} else {
						natl::printlnfc("natl: ", test.from, natl::PrintColor::red,
							" test failed: ", forward<AssertArgs>(assertArgs)...,
							" expected: ", expected, ", actual: null");
					}
				} else {
					if (actual.hasValue()) {
						natl::printlnfc("natl: ", test.from, natl::PrintColor::red,
							" test failed: ", forward<AssertArgs>(assertArgs)...);
					} else {
						natl::printlnfc("natl: ", test.from, natl::PrintColor::red,
							" test failed: ", forward<AssertArgs>(assertArgs)..., " actual: null");
					}
				}
			}
		}
	}

	

	template<typename... AssertArgs>
	constexpr void subTestAssert(Test& test, Bool condition) noexcept {
		if (condition) {
			test.passedTest(); 
		} else {
			test.failedTest();
		}
	}
}