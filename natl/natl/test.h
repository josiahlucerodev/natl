#pragma once

//std
#include <iostream>

//own
#include "stringView.h"
#include "string.h"

//interface
namespace natl {
	class TestTypeStdOut {
	public:
		TestTypeStdOut(const TestTypeStdOut&) { std::cout << "const ref constructed\n"; }
		TestTypeStdOut(TestTypeStdOut&) { std::cout << "ref constructed\n"; }
		TestTypeStdOut(TestTypeStdOut&&) { std::cout << "moved\n"; }
		TestTypeStdOut(const TestTypeStdOut&&) { std::cout << "const moved\n"; }
		TestTypeStdOut() { std::cout << "constructed\n"; }
		~TestTypeStdOut() { std::cout << "destructed\n"; }
	};

	struct TestTypeMoved {};
	struct TestTypeConstructed {};
	struct TestTypeDestructed {};

	class TestTypeThrow {
		TestTypeThrow(TestTypeThrow&&) { throw TestTypeMoved(); }
		TestTypeThrow() { throw TestTypeConstructed(); }
		~TestTypeThrow() noexcept(false) { throw TestTypeDestructed(); }
	};

	class TestTypeCondition {
	public:
		bool constructed;
		constexpr TestTypeCondition() { constructed = true; }
		constexpr ~TestTypeCondition() { constructed = false; }
	};

	class TestType {};

	class TestInvaildValue {
		String message;
	public:
		TestInvaildValue(const StringView& message) : message(message) {}
		TestInvaildValue(const String& message) : message(message) {}
		TestInvaildValue(const char* message) : message(message) {}
	public:
		StringView getMessageView() const { return static_cast<StringView>(message); };
		String& getMessage() { return message; };
		const String& getMessage() const { return message; };
	};

	template<class Type>
	void testVaildateValue(Type value, Type correctValue, const StringView& message) {
		if (value != correctValue) {
			throw TestInvaildValue(message);
		}
	}

	template<class Type>
	void testVaildateValue(Type value, Type correctValue, const String& message) {
		if (value != correctValue) {
			throw TestInvaildValue(message);
		}
	}

	template<class Type>
	void testVaildateValue(Type value, Type correctValue, const char* message) {
		if (value != correctValue) {
			throw TestInvaildValue(message);
		}
	}

	void runTest(bool(*func)(int), const std::size_t repeatCount, const String& testName) {
		std::cout << "Test " << testName << "\n";
		std::size_t failCount = 0;
		for (std::size_t i = 0; i < repeatCount; i++) {
			try {
				func(i);
			} catch (const TestInvaildValue& invaildValue) {
				std::cout << "iteration " << i << " failed: invaild value " << invaildValue.getMessageView().cStr() << "\n";
				failCount++;
			}
		}
		std::cout << testName << " complete ";
		if (failCount == 0) {
			std::cout << "success";
		} else {
			std::cout << "failed " << failCount << " times";
		}
		std::cout << "\n";
	}

	void testStart(const String& testName) {
		std::cout << testName << "\n";
	}
}