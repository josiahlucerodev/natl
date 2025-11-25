//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/string.h>
#include <natl/system/timer.h>

template<typename StringType>
struct StringTestSuite : public natl::TestSuite<StringTestSuite<StringType>> {
	using string_type = StringType;
};

template<typename test_suite_type = StringTestSuite<natl::String>>
struct StringTestSuiteTests {
	using string_type = test_suite_type::string_type;
	using char_type = test_suite_type::string_type::value_type;

	constexpr static void constructionAndAccessTest(test_suite_type& ts) noexcept {
		string_type emptyStr;
		ts.assertEqual("", emptyStr);

		const char* cStr = "Test String";
		string_type fromCStr(cStr);
		ts.assertEqual(cStr, fromCStr);

		char fillChar = 'X';
		natl::Size size = 5;
		string_type filledStr(size, fillChar);
		ts.assertEqual("XXXXX", filledStr);

		string_type str = "Hello, World!";
		ts.assertEqual(char_type('H'), str[0]);
		ts.assertEqual(char_type('W'), str.at(7));

		ts.assertEqual(13, str.size());

		string_type largeStr(300, char_type('A'));
		ts.assertEqual(300, largeStr.size());
	}
	NATL_REGISTER_TEST_CONSTEXPR(constructionAndAccessTest);

	constexpr static void modificationTest(test_suite_type& ts) noexcept {
		string_type str = "Hello, World!";

		str += " Welcome!";
		ts.assertEqual("Hello, World! Welcome!", str);

		str.erase(7, 7); 
		ts.assertEqual("Hello, Welcome!", str);

		str.insert(5, " Beautiful");
		ts.assertEqual("Hello Beautiful, Welcome!", str);

		str.replace(6, 9, "Universe");
		ts.assertEqual("Hello Universe, Welcome!", str);

		str.clear();
		ts.assertTrue(str.empty());

		str.reserve(20);
		ts.assertLt(20, str.capacity());

		str.resize(1000, 'X');
		ts.assertEqual(1000, str.size());
	}
	NATL_REGISTER_TEST_CONSTEXPR(modificationTest);

	constexpr static void comparisonTest(test_suite_type& ts) noexcept {
		string_type str = "Hello, World! Welcome!";
		string_type sameStr = "Hello, World! Welcome!";
		ts.assertEqual(str, sameStr);

		string_type differentStr = "Different String";
		ts.assertNotEqual(str, differentStr);

		ts.assertLt(differentStr, str);
		ts.assertGe(str, sameStr);
	}
	NATL_REGISTER_TEST_CONSTEXPR(comparisonTest);

	constexpr static void concatenationTest(test_suite_type& ts) noexcept {
		string_type str1 = "Hello, ";
		string_type str2 = "World!";

		ts.assertEqual("Hello, World!", str1 + str2);
	}
	NATL_REGISTER_TEST_CONSTEXPR(concatenationTest);

	constexpr static void substringTest(test_suite_type& ts) noexcept {
		string_type str = "Hello, World!";

		string_type subStr = str.substr(7, 5);
		ts.assertEqual("World", subStr);

		// Replace substring
		str.replace(7, 5, "Universe");
		ts.assertEqual("Hello, Universe!", str);

		natl::Size lastFound = str.rfind("e");
		ts.assertEqual(14, lastFound);
	}
	NATL_REGISTER_TEST_CONSTEXPR(substringTest);

	constexpr static void characterManipulationTest(test_suite_type& ts) noexcept {
		string_type str = "Hello, World";

		str.pushBack('!');
		ts.assertEqual(char_type('!'), str.back());
		str.popBack();
		ts.assertEqual(char_type('d'), str.back());
		str.pushBack(char_type('!'));
		ts.assertEqual(char_type('!'), str.back());

		str[7] = 'M';
		ts.assertEqual("Hello, Morld!", str);

		natl::swap(str[0], str[7]);
		ts.assertEqual("Mello, Horld!", str);
	}
	NATL_REGISTER_TEST_CONSTEXPR(characterManipulationTest);

	constexpr static void iteratorTest(test_suite_type& ts) noexcept {
		string_type str = "Hello, World!";

		string_type forwardStr;
		for (auto it = str.begin(); it != str.end(); ++it) {
			forwardStr.pushBack(*it);
		}
		ts.assertEqual(str, forwardStr);

		string_type backwardStr;
		for (auto it = str.rbegin(); it != str.rend(); ++it) {
			backwardStr.pushBack(*it);
		}
		ts.assertEqual("!dlroW ,olleH", backwardStr);
	}
	NATL_REGISTER_TEST_CONSTEXPR(iteratorTest);

	constexpr static void cStringConversionTest(test_suite_type& ts) noexcept {
		if constexpr (natl::IsSameC<typename string_type::value_type, char>) {
			string_type str = "Hello, World!";

			const char* cStr = str.c_str();
			ts.assertEqual("Hello, World!", natl::ConstStringView(cStr));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(cStringConversionTest);

	constexpr static void swapTest(test_suite_type& ts) noexcept {
		string_type str1 = "First String";
		string_type str2 = "Second String";

		swap<string_type>(str1, str2);

		ts.assertEqual(str1, "Second String");
		ts.assertEqual(str2, "First String");
	}
	NATL_REGISTER_TEST_CONSTEXPR(swapTest);

	constexpr static void shrinkToFitTest(test_suite_type& ts) noexcept {
		string_type str;
		str.resize(10000, 'A');
		natl::Size initialCapacity = str.capacity();

		str.resize(10, 'A');

		str.shrink_to_fit();

		ts.assertLt(str.capacity(), initialCapacity);

		str.resize(12000, 'A');
		initialCapacity = str.capacity();

		str.resize(50, 'A');

		str.shrink_to_fit();
		ts.assertLt(str.capacity(), initialCapacity);
	}
	NATL_REGISTER_TEST_CONSTEXPR(shrinkToFitTest);
};

struct  AsciiString32TestSuite : public StringTestSuite<natl::AsciiString32> {};
NATL_REGISTER_TESTS(AsciiString32TestSuite, StringTestSuiteTests);

struct  AsciiString64TestSuite : public StringTestSuite<natl::AsciiString64> {};
NATL_REGISTER_TESTS(AsciiString64TestSuite, StringTestSuiteTests);

struct  AsciiString96TestSuite : public StringTestSuite<natl::AsciiString96> {};
NATL_REGISTER_TESTS(AsciiString96TestSuite, StringTestSuiteTests);

struct  AsciiString128TestSuite : public StringTestSuite<natl::AsciiString128> {};
NATL_REGISTER_TESTS(AsciiString128TestSuite, StringTestSuiteTests);

struct  AsciiString256TestSuite : public StringTestSuite<natl::AsciiString256> {};
NATL_REGISTER_TESTS(AsciiString256TestSuite, StringTestSuiteTests);

struct  Utf32String32TestSuite : public StringTestSuite<natl::Utf32String32> {};
NATL_REGISTER_TESTS(Utf32String32TestSuite, StringTestSuiteTests);

struct  Utf32String64TestSuite : public StringTestSuite<natl::Utf32String64> {};
NATL_REGISTER_TESTS(Utf32String64TestSuite, StringTestSuiteTests);

struct  Utf32String96TestSuite : public StringTestSuite<natl::Utf32String96> {};
NATL_REGISTER_TESTS(Utf32String96TestSuite, StringTestSuiteTests);

struct  Utf32String128TestSuite : public StringTestSuite<natl::Utf32String128> {};
NATL_REGISTER_TESTS(Utf32String128TestSuite, StringTestSuiteTests);

struct  Utf32String256TestSuite : public StringTestSuite<natl::Utf32String256> {};
NATL_REGISTER_TESTS(Utf32String256TestSuite, StringTestSuiteTests);

struct  Utf32String512TestSuite : public StringTestSuite<natl::Utf32String512> {};
NATL_REGISTER_TESTS(Utf32String512TestSuite, StringTestSuiteTests)

NATL_TESTS_MAIN();