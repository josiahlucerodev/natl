#pragma once

//std
#include <iostream>
#include <string>

//interface
namespace natl {
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
		TestTypeStdOut& operator=(TestTypeStdOut& other) { std::cout << "ref assigned\n"; return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut& other) { std::cout << "const ref assigned\n"; return self(); }
		TestTypeStdOut& operator=(TestTypeStdOut&&) noexcept { std::cout << "moved assigned\n"; return self(); }
		TestTypeStdOut& operator=(const TestTypeStdOut&&) noexcept { std::cout << "const moved assigned\n"; return self(); }
	};

#define NATL_TEST_ASSERT(from, condition, message) \
    if (!(condition)) { \
        std::cerr << "natl: " << from << "Test Assertion failed: " << message << "\n"; \
        return false; \
    }


	void runTest(bool(*func)(int), const std::string& testName) {
		std::cout << "Natl Test: " << testName << "\n";
		func(rand());
	}
}