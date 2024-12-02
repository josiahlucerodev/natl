
//natl
#include <natl/util/test.h>
#include <natl/container/string.h>
#include <natl/system/timer.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlStringTest";

template<class StringType>
class StringTest {
	natl::ConstAsciiStringView stringTypeName;
public:
	StringTest(natl::ConstAsciiStringView stringTypeNameIn) : stringTypeName(stringTypeNameIn) {}

	natl::Bool testConstructionAndAccess() {
		natl::String testName = stringTypeName;
		testName += " testConstructionAndAccess";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType emptyStr;
		natl::testAssert(test, typeid(StringType).name(), emptyStr == "", "Default construction failed");

		const char* cStr = "Test String";
		StringType fromCStr(cStr);
		natl::testAssert(test, typeid(StringType).name(), fromCStr == cStr, "Construction from C-string failed");

		char fillChar = 'X';
		natl::Size size = 5;
		StringType filledStr(size, fillChar);
		natl::testAssert(test, typeid(StringType).name(), filledStr == "XXXXX", "Construction with size and character failed");

		StringType str = "Hello, World!";
		natl::testAssert(test, typeid(StringType).name(), str[0] == 'H', "Accessing characters failed");
		natl::testAssert(test, typeid(StringType).name(), str.at(7) == 'W', "Accessing characters with at() failed");

		natl::testAssert(test, typeid(StringType).name(), str.size() == 13, "String size mismatch");

		// Test string larger than SSO
		StringType largeStr(300, 'A');
		natl::testAssert(test, typeid(StringType).name(), largeStr.size() == 300, "Large string construction failed");

		return test;
	}

	natl::Bool testModification() {
		natl::String testName = stringTypeName;
		testName += " testModification";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str = "Hello, World!";

		// Test appending
		str += " Welcome!";
		natl::testAssert(test, typeid(StringType).name(), str == "Hello, World! Welcome!", "Appending failed");

		// Test erasing
		str.erase(7, 7);  // Erase "World! "
		natl::testAssert(test, typeid(StringType).name(), str == "Hello, Welcome!", "Erasing failed");

		// Test inserting
		str.insert(5, " Beautiful");
		natl::testAssert(test, typeid(StringType).name(), str == "Hello Beautiful, Welcome!", "Inserting failed");

		// Test replacing
		str.replace(6, 9, "Universe");
		natl::testAssert(test, typeid(StringType).name(), str == "Hello Universe, Welcome!", "Replacing failed");

		// Test clearing
		str.clear();
		natl::testAssert(test, typeid(StringType).name(), str.empty(), "Clearing failed");

		// Test reserve
		str.reserve(20);
		natl::testAssert(test, typeid(StringType).name(), str.capacity() >= 20, "Reserving capacity failed");

		// Test resizing from small to large
		str.resize(1000, 'X');
		natl::testAssert(test, typeid(StringType).name(), str.size() == 1000 && str[7] == 'X', "Resizing from small to large failed");

		return test;
	}

	natl::Bool testComparison() {
		natl::String testName = stringTypeName;
		testName += " testComparison";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str = "Hello, World! Welcome!";
		StringType sameStr = "Hello, World! Welcome!";
		natl::testAssert(test, typeid(StringType).name(), str == sameStr, "Equality comparison failed");

		StringType differentStr = "Different String";
		natl::testAssert(test, typeid(StringType).name(), str != differentStr, "Inequality comparison failed");

		natl::testAssert(test, typeid(StringType).name(), differentStr < str, "Less than comparison failed");

		// Test comparison operators
		natl::testAssert(test, typeid(StringType).name(), str >= sameStr, "Greater than or equal to comparison failed");

		return test;
	}

	natl::Bool testConcatenation() {
		natl::String testName = stringTypeName;
		testName += " testConcatenation";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str1 = "Hello, ";
		StringType str2 = "World!";
		StringType result = str1 + str2;

		natl::testAssert(test, typeid(StringType).name(), result == "Hello, World!", "String concatenation failed");

		return test;
	}

	natl::Bool testSubstring() {
		natl::String testName = stringTypeName;
		testName += " testSubstring";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str = "Hello, World!";

		// Extract substring
		StringType subStr = str.substr(7, 5);  // "World"
		natl::testAssert(test, typeid(StringType).name(), subStr == "World", "Substring extraction failed");

		// Replace substring
		str.replace(7, 5, "Universe");
		natl::testAssert(test, typeid(StringType).name(), str == "Hello, Universe!", "Substring replacement failed");

		// Test finding the last occurrence of a substring
		natl::Size lastFound = str.rfind("e");
		natl::testAssert(test, typeid(StringType).name(), lastFound == 14, "Last occurrence of substring not found");

		return test;
	}

	natl::Bool testCharacterManipulation() {
		natl::String testName = stringTypeName;
		testName += " testCharacterManipulation";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str = "Hello, World";

		// Test pushing back and popping back
		str.pushBack('!');
		natl::testAssert(test, typeid(StringType).name(), str.back() == '!', "Pushing back failed");
		str.pop_back();
		natl::testAssert(test, typeid(StringType).name(), str.back() == 'd', "Popping back failed");
		str.pushBack('!');
		natl::testAssert(test, typeid(StringType).name(), str.back() == '!', "Pushing back failed");

		// Test changing character at a specific position
		str[7] = 'M';
		natl::testAssert(test, typeid(StringType).name(), str == "Hello, Morld!", "Changing character failed");

		// Test swapping characters
		natl::swap(str[0], str[7]);
		natl::testAssert(test, typeid(StringType).name(), str == "Mello, Horld!", "Swapping characters failed");

		return test;
	}

	natl::Bool testIterators() {
		natl::String testName = stringTypeName;
		testName += " testIterators";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str = "Hello, World!";

		// Test iterating forward
		StringType forwardStr;
		for (auto it = str.begin(); it != str.end(); ++it) {
			forwardStr.pushBack(*it);
		}
		natl::testAssert(test, typeid(StringType).name(), forwardStr == str, "Forward iteration failed");

		// Test iterating backward
		StringType backwardStr;
		for (auto it = str.rbegin(); it != str.rend(); ++it) {
			backwardStr.pushBack(*it);
		}
		natl::testAssert(test, typeid(StringType).name(), backwardStr == "!dlroW ,olleH", "Backward iteration failed");

		return test;
	}

	natl::Bool testCStringConversion() {
		natl::String testName = stringTypeName;
		testName += " testCStringConversion";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		if constexpr (natl::IsSameC<typename StringType::value_type, char>) {
			StringType str = "Hello, World!";

			// Test conversion to C-style string
			const char* cStr = str.c_str();
			natl::testAssert(test, typeid(StringType).name(), strcmp(cStr, "Hello, World!") == 0, "C-string conversion failed");
		}

		return test;
	}

	natl::Bool testSwap() {
		natl::String testName = stringTypeName;
		testName += " testSwap";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str1 = "First String";
		StringType str2 = "Second String";

		// Test swapping strings
		swap<StringType>(str1, str2);

		natl::testAssert(test, typeid(StringType).name(), str1 == "Second String" && str2 == "First String", "Swapping strings failed");

		return test;
	}

	natl::Bool testShrinkToFit() {
		natl::String testName = stringTypeName;
		testName += " testShrinkToFit";
		natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

		StringType str;
		str.resize(10000, 'A');
		natl::Size initialCapacity = str.capacity();

		str.resize(10, 'A');

		// Shrink to fit
		str.shrink_to_fit();

		natl::testAssert(test, typeid(StringType).name(), str.capacity() < initialCapacity, "Shrink to fit size 100 failed");

		// Increase size to trigger capacity increase
		str.resize(12000, 'A');
		initialCapacity = str.capacity();

		str.resize(50, 'A');

		// Shrink to fit
		str.shrink_to_fit();

		natl::testAssert(test, typeid(StringType).name(), str.capacity() < initialCapacity, "Shrink to fit size 1000 failed");

		return test;
	}

	natl::Bool test() {
		natl::Test test(natlTestFrom, stringTypeName, natl::TestType::node);
		natl::subTestAssert(test, testConstructionAndAccess());
		natl::subTestAssert(test, testModification());
		natl::subTestAssert(test, testComparison());
		natl::subTestAssert(test, testConcatenation());
		natl::subTestAssert(test, testSubstring());
		natl::subTestAssert(test, testCharacterManipulation());
		natl::subTestAssert(test, testIterators());
		natl::subTestAssert(test, testCStringConversion());
		natl::subTestAssert(test, testSwap());
		natl::subTestAssert(test, testShrinkToFit());
		return test;
	}

};

template<typename StringTestType>
natl::Bool stringTypeTest(natl::ConstAsciiStringView name) noexcept {
	StringTestType stringTest(name);
	return stringTest.test();
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::AsciiString32>>("natl::AsciiString32"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::AsciiString64>>("natl::AsciiString64"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::AsciiString96>>("natl::AsciiString96"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::AsciiString128>>("natl::AsciiString128"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::AsciiString256>>("natl::AsciiString256"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::Utf32String32>>("natl::Utf32String32"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::Utf32String64>>("natl::Utf32String64"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::Utf32String96>>("natl::Utf32String96"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::Utf32String128>>("natl::Utf32String128"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::Utf32String256>>("natl::Utf32String256"));
	natl::subTestAssert(test, stringTypeTest<StringTest<natl::Utf32String512>>("natl::Utf32String512"));
	return test;
}

int main() {
	tests();
}