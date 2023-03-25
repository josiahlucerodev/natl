/*header
#pragma once
//own
#include "pch.h"
endHeader*/

//module
export module natl.test;
//std
import std;
//endModule

//interface
namespace natl {
	class TestInvaildValue {
		std::string message;
	public:
		TestInvaildValue(const std::string_view& message) : message(message) {}
		TestInvaildValue(const std::string& message) : message(message) {}
		TestInvaildValue(const char* message) : message(message) {}
	public:
		std::string_view getMessageView() const { return message; };
		std::string& getMessage() { return message; };
		const std::string& getMessage() const { return message; };
	};

	template<class Type>
	void testVaildateValue(Type value, Type correctValue, const std::string_view& message) {
		if (value != correctValue) {
			throw TestInvaildValue(message);
		}
	}

	template<class Type>
	void testVaildateValue(Type value, Type correctValue, const std::string& message) {
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

	void runTest(bool(*func)(int), const std::size_t repeatCount, const std::string& testName) {
		std::cout << "Test " << testName << "\n";
		std::size_t failCount = 0;
		for (std::size_t i = 0; i < repeatCount; i++) {
			try {
				func(i);
			} catch (const TestInvaildValue& invaildValue) {
				std::cout << "iteration " << i << " failed: invaild value " << invaildValue.getMessageView() << "\n";
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

	void testStart(const std::string& testName) {
		std::cout << testName << "\n";
	}
}