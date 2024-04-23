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

#define NATL_TEST_ASSERT(from, condition, message) \
    if (!(condition)) { \
        std::cerr << "natl: " << from << "Test Assertion failed: " << message << "\n"; \
        return false; \
    }


	inline void runTest(Bool(*func)(int), const std::string& testName) noexcept {
		std::cout << "Natl Test: " << testName << "\n";
		func(rand());
	}
}