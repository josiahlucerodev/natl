#pragma once

//std
#include <iostream>
#include <string>

//own
#include "basicTypes.h"

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
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = true;
		constexpr static bool triviallyDestructible = true;
		constexpr static bool triviallyConstRefConstructedable = true;
		constexpr static bool triviallyMoveConstructedable = true;
		constexpr static bool triviallyMoveAssignable = true;
		constexpr static bool triviallyConstRefAssignable = true;

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