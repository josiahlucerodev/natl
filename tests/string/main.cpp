
//natl
#include <natl/util/test.h>
#include <natl/container/string.h>
#include <natl/system/timer.h>

#define NATL_TEST_FROM "NatlStringTest"

template<class StringType>
class StringTest {
	natl::ConstAsciiStringView stringTypeName;
public:
	StringTest(natl::ConstAsciiStringView stringTypeNameIn) : stringTypeName(stringTypeNameIn) {}

	natl::Bool testConstructionAndAccess() {
		natl::String testName = stringTypeName;
		testName += " testConstructionAndAccess";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType emptyStr;
		NATL_TEST_ASSERT(typeid(StringType).name(), emptyStr == "", "Default construction failed");

		const char* cStr = "Test String";
		StringType fromCStr(cStr);
		NATL_TEST_ASSERT(typeid(StringType).name(), fromCStr == cStr, "Construction from C-string failed");

		char fillChar = 'X';
		natl::Size size = 5;
		StringType filledStr(size, fillChar);
		NATL_TEST_ASSERT(typeid(StringType).name(), filledStr == "XXXXX", "Construction with size and character failed");

		StringType str = "Hello, World!";
		NATL_TEST_ASSERT(typeid(StringType).name(), str[0] == 'H', "Accessing characters failed");
		NATL_TEST_ASSERT(typeid(StringType).name(), str.at(7) == 'W', "Accessing characters with at() failed");

		NATL_TEST_ASSERT(typeid(StringType).name(), str.size() == 13, "String size mismatch");

		// Test string larger than SSO
		StringType largeStr(300, 'A');
		NATL_TEST_ASSERT(typeid(StringType).name(), largeStr.size() == 300, "Large string construction failed");

		return test;
	}

	natl::Bool testModification() {
		natl::String testName = stringTypeName;
		testName += " testModification";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str = "Hello, World!";

		// Test appending
		str += " Welcome!";
		NATL_TEST_ASSERT(typeid(StringType).name(), str == "Hello, World! Welcome!", "Appending failed");

		// Test erasing
		str.erase(7, 7);  // Erase "World! "
		NATL_TEST_ASSERT(typeid(StringType).name(), str == "Hello, Welcome!", "Erasing failed");

		// Test inserting
		str.insert(5, " Beautiful");
		NATL_TEST_ASSERT(typeid(StringType).name(), str == "Hello Beautiful, Welcome!", "Inserting failed");

		// Test replacing
		str.replace(6, 9, "Universe");
		NATL_TEST_ASSERT(typeid(StringType).name(), str == "Hello Universe, Welcome!", "Replacing failed");

		// Test clearing
		str.clear();
		NATL_TEST_ASSERT(typeid(StringType).name(), str.empty(), "Clearing failed");

		// Test reserve
		str.reserve(20);
		NATL_TEST_ASSERT(typeid(StringType).name(), str.capacity() >= 20, "Reserving capacity failed");

		// Test resizing from small to large
		str.resize(1000, 'X');
		NATL_TEST_ASSERT(typeid(StringType).name(), str.size() == 1000 && str[7] == 'X', "Resizing from small to large failed");

		return test;
	}

	natl::Bool testComparison() {
		natl::String testName = stringTypeName;
		testName += " testComparison";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str = "Hello, World! Welcome!";
		StringType sameStr = "Hello, World! Welcome!";
		NATL_TEST_ASSERT(typeid(StringType).name(), str == sameStr, "Equality comparison failed");

		StringType differentStr = "Different String";
		NATL_TEST_ASSERT(typeid(StringType).name(), str != differentStr, "Inequality comparison failed");

		NATL_TEST_ASSERT(typeid(StringType).name(), differentStr < str, "Less than comparison failed");

		// Test comparison operators
		NATL_TEST_ASSERT(typeid(StringType).name(), str >= sameStr, "Greater than or equal to comparison failed");

		return test;
	}

	natl::Bool testConcatenation() {
		natl::String testName = stringTypeName;
		testName += " testConcatenation";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str1 = "Hello, ";
		StringType str2 = "World!";
		StringType result = str1 + str2;

		NATL_TEST_ASSERT(typeid(StringType).name(), result == "Hello, World!", "String concatenation failed");

		return test;
	}

	natl::Bool testSubstring() {
		natl::String testName = stringTypeName;
		testName += " testSubstring";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str = "Hello, World!";

		// Extract substring
		StringType subStr = str.substr(7, 5);  // "World"
		NATL_TEST_ASSERT(typeid(StringType).name(), subStr == "World", "Substring extraction failed");

		// Replace substring
		str.replace(7, 5, "Universe");
		NATL_TEST_ASSERT(typeid(StringType).name(), str == "Hello, Universe!", "Substring replacement failed");

		// Test finding the last occurrence of a substring
		natl::Size lastFound = str.rfind("e");
		NATL_TEST_ASSERT(typeid(StringType).name(), lastFound == 14, "Last occurrence of substring not found");

		return test;
	}

	natl::Bool testCharacterManipulation() {
		natl::String testName = stringTypeName;
		testName += " testCharacterManipulation";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str = "Hello, World";

		// Test pushing back and popping back
		str.push_back('!');
		NATL_TEST_ASSERT(typeid(StringType).name(), str.back() == '!', "Pushing back failed");
		str.pop_back();
		NATL_TEST_ASSERT(typeid(StringType).name(), str.back() == 'd', "Popping back failed");
		str.push_back('!');
		NATL_TEST_ASSERT(typeid(StringType).name(), str.back() == '!', "Pushing back failed");

		// Test changing character at a specific position
		str[7] = 'M';
		NATL_TEST_ASSERT(typeid(StringType).name(), str == "Hello, Morld!", "Changing character failed");

		// Test swapping characters
		natl::swap(str[0], str[7]);
		NATL_TEST_ASSERT(typeid(StringType).name(), str == "Mello, Horld!", "Swapping characters failed");

		return test;
	}

	natl::Bool testIterators() {
		natl::String testName = stringTypeName;
		testName += " testIterators";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str = "Hello, World!";

		// Test iterating forward
		StringType forwardStr;
		for (auto it = str.begin(); it != str.end(); ++it) {
			forwardStr.push_back(*it);
		}
		NATL_TEST_ASSERT(typeid(StringType).name(), forwardStr == str, "Forward iteration failed");

		// Test iterating backward
		StringType backwardStr;
		for (auto it = str.rbegin(); it != str.rend(); ++it) {
			backwardStr.push_back(*it);
		}
		NATL_TEST_ASSERT(typeid(StringType).name(), backwardStr == "!dlroW ,olleH", "Backward iteration failed");

		return test;
	}

	natl::Bool testCStringConversion() {
		natl::String testName = stringTypeName;
		testName += " testCStringConversion";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		if constexpr (std::is_same_v<typename StringType::value_type, char>) {
			StringType str = "Hello, World!";

			// Test conversion to C-style string
			const char* cStr = str.c_str();
			NATL_TEST_ASSERT(typeid(StringType).name(), strcmp(cStr, "Hello, World!") == 0, "C-string conversion failed");
		}

		return test;
	}

	natl::Bool testSwap() {
		natl::String testName = stringTypeName;
		testName += " testSwap";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str1 = "First String";
		StringType str2 = "Second String";

		// Test swapping strings
		swap<StringType>(str1, str2);

		NATL_TEST_ASSERT(typeid(StringType).name(), str1 == "Second String" && str2 == "First String", "Swapping strings failed");

		return test;
	}

	natl::Bool testShrinkToFit() {
		natl::String testName = stringTypeName;
		testName += " testShrinkToFit";
		natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

		StringType str;
		str.resize(10000, 'A');
		natl::Size initialCapacity = str.capacity();

		str.resize(10, 'A');

		// Shrink to fit
		str.shrink_to_fit();

		NATL_TEST_ASSERT(typeid(StringType).name(), str.capacity() < initialCapacity, "Shrink to fit size 100 failed");

		// Increase size to trigger capacity increase
		str.resize(12000, 'A');
		initialCapacity = str.capacity();

		str.resize(50, 'A');

		// Shrink to fit
		str.shrink_to_fit();

		NATL_TEST_ASSERT(typeid(StringType).name(), str.capacity() < initialCapacity, "Shrink to fit size 1000 failed");

		return test;
	}

	natl::Bool test() {
		natl::Test test(NATL_TEST_FROM, stringTypeName, natl::TestType::node);
		NATL_SUB_TEST_ASSERT(testConstructionAndAccess());
		NATL_SUB_TEST_ASSERT(testModification());
		NATL_SUB_TEST_ASSERT(testComparison());
		NATL_SUB_TEST_ASSERT(testConcatenation());
		NATL_SUB_TEST_ASSERT(testSubstring());
		NATL_SUB_TEST_ASSERT(testCharacterManipulation());
		NATL_SUB_TEST_ASSERT(testIterators());
		NATL_SUB_TEST_ASSERT(testCStringConversion());
		NATL_SUB_TEST_ASSERT(testSwap());
		NATL_SUB_TEST_ASSERT(testShrinkToFit());
		return test;
	}

};

template<typename StringTestType>
natl::Bool stringTypeTest(natl::ConstAsciiStringView name) noexcept {
	StringTestType stringTest(name);
	return stringTest.test();
}

natl::Bool tests() noexcept {
	natl::Test test(NATL_TEST_FROM, "all", natl::TestType::root);
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::AsciiString32>>("natl::AsciiString32"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::AsciiString64>>("natl::AsciiString64"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::AsciiString96>>("natl::AsciiString96"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::AsciiString128>>("natl::AsciiString128"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::AsciiString256>>("natl::AsciiString256"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::Utf32String32>>("natl::Utf32String32"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::Utf32String64>>("natl::Utf32String64"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::Utf32String96>>("natl::Utf32String96"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::Utf32String128>>("natl::Utf32String128"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::Utf32String256>>("natl::Utf32String256"));
	NATL_SUB_TEST_ASSERT(stringTypeTest<StringTest<natl::Utf32String512>>("natl::Utf32String512"));
	return test;
}

int main() {
	natl::Timer timer;
	tests();
	natl::printlnf("natl: ", NATL_TEST_FROM, " time: ", timer.getMillisecondsInt());
}