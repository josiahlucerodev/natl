#pragma once

//own
#include "../util/basicTypes.h"
#include "../system/printFormatted.h"
#include "../system/printColor.h"
#include "../system/timer.h"

//interface
namespace natl {
	struct TestTimer {
		ConstAsciiStringView testFrom;
		Timer timer;
		TestTimer(ConstAsciiStringView testFromIn) noexcept : testFrom(testFromIn) {}
		~TestTimer() noexcept {
			natl::printlnf("natl: ", testFrom, " time: ", timer.getMillisecondsInt());
		}
	};


	class TestTypeStdOut {
	public:
		//constructor
		TestTypeStdOut() noexcept { std::cout << "default constructed\n"; }
		TestTypeStdOut(TestTypeStdOut&) noexcept { std::cout << "ref constructed\n"; }
		TestTypeStdOut(const TestTypeStdOut&) noexcept { std::cout << "const ref constructed\n"; }
		TestTypeStdOut(TestTypeStdOut&&) noexcept { std::cout << "moved constructed\n"; }
		TestTypeStdOut(const TestTypeStdOut&&) noexcept { std::cout << "const moved constructed\n"; }

		//destructor 
		~TestTypeStdOut() noexcept { std::cout << "destructed\n"; }

		//util
		constexpr TestTypeStdOut& self() { return *this; }

		//assignment 
		TestTypeStdOut& operator=(TestTypeStdOut&) { std::cout << "ref assigned\n"; return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut&) { std::cout << "const ref assigned\n"; return self(); }
		TestTypeStdOut& operator=(TestTypeStdOut&&) noexcept { std::cout << "moved assigned\n"; return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut&&) noexcept { std::cout << "const moved assigned\n"; return self(); }
	};
	//custom 
	class TestTypeCustomTrivialStdOut {
		ui8 addressableData;
	public:
		//movement info  
		constexpr static Bool triviallyRelocatable = true;
		constexpr static Bool triviallyDefaultConstructible = true;
		constexpr static Bool triviallyCompareable = true;
		constexpr static Bool triviallyDestructible = true;
		constexpr static Bool triviallyConstRefConstructedable = true;
		constexpr static Bool triviallyMoveConstructedable = true;
		constexpr static Bool triviallyMoveAssignable = true;
		constexpr static Bool triviallyConstRefAssignable = true;

		//constructor
		TestTypeCustomTrivialStdOut() noexcept : addressableData(0) { std::cout << "default constructed\n"; }
		TestTypeCustomTrivialStdOut(TestTypeCustomTrivialStdOut&) noexcept : addressableData(0) { std::cout << "ref constructed\n"; }
		TestTypeCustomTrivialStdOut(const TestTypeCustomTrivialStdOut&) noexcept : addressableData(0) { std::cout << "const ref constructed\n"; }
		TestTypeCustomTrivialStdOut(TestTypeCustomTrivialStdOut&&) noexcept : addressableData(0) { std::cout << "moved constructed\n"; }
		TestTypeCustomTrivialStdOut(const TestTypeCustomTrivialStdOut&&) noexcept : addressableData(0) { std::cout << "const moved constructed\n"; }

		//destructor 
		~TestTypeCustomTrivialStdOut() noexcept { std::cout << "destructed\n"; }

		//util
		constexpr TestTypeCustomTrivialStdOut& self() { return *this; }

		//assignment 
		TestTypeCustomTrivialStdOut& operator=(TestTypeCustomTrivialStdOut&) { std::cout << "ref assigned\n"; return self(); }
		TestTypeCustomTrivialStdOut& operator=(const TestTypeCustomTrivialStdOut&) { std::cout << "const ref assigned\n"; return self(); }
		TestTypeCustomTrivialStdOut& operator=(TestTypeCustomTrivialStdOut&&) noexcept { std::cout << "moved assigned\n"; return self(); }
		TestTypeCustomTrivialStdOut& operator=(const TestTypeCustomTrivialStdOut&&) noexcept { std::cout << "const moved assigned\n"; return self(); }

		//element access 
		const ui8* getAddress() const noexcept { return &addressableData; }
	};

	enum class TestType {
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
						natl::printlnfc("natl: ", from, " test ", name, ": ", PrintColor::green, successCount, " tests passed ", PrintColor::red, failureCount, " tests failed");
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
				natl::printlnfc("natl: ", test.from, natl::PrintColor::red, " test failed: ", assertArgs...);
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