//natl
#include <natl/test.h>
#include <natl/string.h>

template<class StringType>
class StringTest {
public:
     static natl::Bool testConstructionAndAccess() {
        StringType emptyStr;
        NATL_TEST_ASSERT(typeid(StringType).name() ,emptyStr == "", "Default construction failed");

        const char* cStr = "Test String";
        StringType fromCStr(cStr);
        NATL_TEST_ASSERT(typeid(StringType).name() ,fromCStr == cStr, "Construction from C-string failed");

        char fillChar = 'X';
        natl::Size size = 5;
        StringType filledStr(size, fillChar);
        NATL_TEST_ASSERT(typeid(StringType).name() ,filledStr == "XXXXX", "Construction with size and character failed");

        StringType str = "Hello, World!";
        NATL_TEST_ASSERT(typeid(StringType).name() ,str[0] == 'H', "Accessing characters failed");
        NATL_TEST_ASSERT(typeid(StringType).name() ,str.at(7) == 'W', "Accessing characters with at() failed");

        NATL_TEST_ASSERT(typeid(StringType).name() ,str.size() == 13, "String size mismatch");

        // Test string larger than SSO
        StringType largeStr(300, 'A');
        NATL_TEST_ASSERT(typeid(StringType).name() ,largeStr.size() == 300, "Large string construction failed");

        return true;
    }

     static natl::Bool testModification() {
        StringType str = "Hello, World!";

        // Test appending
        str += " Welcome!";
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == "Hello, World! Welcome!", "Appending failed");

        // Test erasing
        str.erase(7, 7);  // Erase "World! "
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == "Hello, Welcome!", "Erasing failed");

        // Test inserting
        str.insert(5, " Beautiful");
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == "Hello Beautiful, Welcome!", "Inserting failed");

        // Test replacing
        str.replace(6, 9, "Universe");
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == "Hello Universe, Welcome!", "Replacing failed");

        // Test clearing
        str.clear();
        NATL_TEST_ASSERT(typeid(StringType).name() ,str.empty(), "Clearing failed");

        // Test reserve
        str.reserve(20);
        NATL_TEST_ASSERT(typeid(StringType).name() ,str.capacity() >= 20, "Reserving capacity failed");

        // Test resizing from small to large
        str.resize(1000, 'X');
        NATL_TEST_ASSERT(typeid(StringType).name() ,str.size() == 1000 && str[7] == 'X', "Resizing from small to large failed");

        return true;
    }

     static natl::Bool testComparison() {
        StringType str = "Hello, World! Welcome!";
        StringType sameStr = "Hello, World! Welcome!";
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == sameStr, "Equality comparison failed");

        StringType differentStr = "Different String";
        NATL_TEST_ASSERT(typeid(StringType).name() ,str != differentStr, "Inequality comparison failed");

        NATL_TEST_ASSERT(typeid(StringType).name() ,differentStr < str, "Less than comparison failed");

        // Test comparison operators
        NATL_TEST_ASSERT(typeid(StringType).name() ,str >= sameStr, "Greater than or equal to comparison failed");

        return true;
    }

     static natl::Bool testConcatenation() {
        StringType str1 = "Hello, ";
        StringType str2 = "World!";
        StringType result = str1 + str2;

        NATL_TEST_ASSERT(typeid(StringType).name() ,result == "Hello, World!", "String concatenation failed");

        return true;
    }

     static natl::Bool testSubstring() {
        StringType str = "Hello, World!";

        // Extract substring
        StringType subStr = str.substr(7, 5);  // "World"
        NATL_TEST_ASSERT(typeid(StringType).name() ,subStr == "World", "Substring extraction failed");

        // Replace substring
        str.replace(7, 5, "Universe");
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == "Hello, Universe!", "Substring replacement failed");

        // Test finding the last occurrence of a substring
        natl::Size lastFound = str.rfind("e");
        NATL_TEST_ASSERT(typeid(StringType).name() ,lastFound == 14, "Last occurrence of substring not found");

        return true;
    }

     static natl::Bool testCharacterManipulation() {
        StringType str = "Hello, World";

        // Test pushing back and popping back
        str.push_back('!');
        NATL_TEST_ASSERT(typeid(StringType).name() ,str.back() == '!', "Pushing back failed");
        str.pop_back();
        NATL_TEST_ASSERT(typeid(StringType).name() ,str.back() == 'd', "Popping back failed");
        str.push_back('!');
        NATL_TEST_ASSERT(typeid(StringType).name(), str.back() == '!', "Pushing back failed");

        // Test changing character at a specific position
        str[7] = 'M';
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == "Hello, Morld!", "Changing character failed");

        // Test swapping characters
        natl::swap(str[0], str[7]);
        NATL_TEST_ASSERT(typeid(StringType).name() ,str == "Mello, Horld!", "Swapping characters failed");

        return true;
    }

     static natl::Bool testIterators() {
        StringType str = "Hello, World!";

        // Test iterating forward
        StringType forwardStr;
        for (auto it = str.begin(); it != str.end(); ++it) {
            forwardStr.push_back(*it);
        }
        NATL_TEST_ASSERT(typeid(StringType).name() ,forwardStr == str, "Forward iteration failed");

        // Test iterating backward
        StringType backwardStr;
        for (auto it = str.rbegin(); it != str.rend(); ++it) {
            backwardStr.push_back(*it);
        }
        NATL_TEST_ASSERT(typeid(StringType).name() ,backwardStr == "!dlroW ,olleH", "Backward iteration failed");

        return true;
    }

     static natl::Bool testCStringConversion() {
        if constexpr (std::is_same_v<typename StringType::value_type, char>) {
            StringType str = "Hello, World!";

            // Test conversion to C-style string
            const char* cStr = str.c_str();
            NATL_TEST_ASSERT(typeid(StringType).name() ,strcmp(cStr, "Hello, World!") == 0, "C-string conversion failed");
        }

        return true;
    }

     static natl::Bool testSwap() {
        StringType str1 = "First String";
        StringType str2 = "Second String";

        // Test swapping strings
        swap<StringType>(str1, str2);

        NATL_TEST_ASSERT(typeid(StringType).name() , str1 == "Second String" && str2 == "First String", "Swapping strings failed");

        return true;
    }

     static natl::Bool testShrinkToFit() {
        StringType str;
        str.resize(10000, 'A');
        natl::Size initialCapacity = str.capacity();

        str.resize(10, 'A');

        // Shrink to fit
        str.shrink_to_fit();

        NATL_TEST_ASSERT(typeid(StringType).name() ,str.capacity() < initialCapacity, "Shrink to fit size 100 failed");

        // Increase size to trigger capacity increase
        str.resize(12000, 'A');
        initialCapacity = str.capacity();

        str.resize(50, 'A');

        // Shrink to fit
        str.shrink_to_fit();

        NATL_TEST_ASSERT(typeid(StringType).name() , str.capacity() < initialCapacity, "Shrink to fit size 1000 failed");

        return true;
    }

     static natl::Bool test(int) {
         natl::Bool allTestsPassed = true;

        allTestsPassed &= testConstructionAndAccess();
        allTestsPassed &= testModification();
        allTestsPassed &= testComparison();
        allTestsPassed &= testConcatenation();
        allTestsPassed &= testSubstring();
        allTestsPassed &= testCharacterManipulation();
        allTestsPassed &= testIterators();
        allTestsPassed &= testCStringConversion();
        allTestsPassed &= testSwap();
        allTestsPassed &= testShrinkToFit();

        if (allTestsPassed) {
            std::cout << "All tests passed successfully!" << std::endl;
        } else {
            std::cerr << "Some tests failed." << std::endl;
        }

        return allTestsPassed;
    }

};


int main() {
    natl::runTest(StringTest<natl::AsciiString32>::test, "natl::AsciiString32");
    natl::runTest(StringTest<natl::AsciiString64>::test, "natl::AsciiString64");
    natl::runTest(StringTest<natl::AsciiString96>::test, "natl::AsciiString96");
    natl::runTest(StringTest<natl::AsciiString128>::test, "natl::AsciiString128");
    natl::runTest(StringTest<natl::AsciiString256>::test, "natl::AsciiString256");

    natl::runTest(StringTest<natl::Utf32String32>::test, "natl::Utf32String32"); 
    natl::runTest(StringTest<natl::Utf32String64>::test, "natl::Utf32String64"); 
    natl::runTest(StringTest<natl::Utf32String96>::test, "natl::Utf32String96"); 
    natl::runTest(StringTest<natl::Utf32String128>::test, "natl::Utf32String128"); 
    natl::runTest(StringTest<natl::Utf32String256>::test, "natl::Utf32String256"); 
    natl::runTest(StringTest<natl::Utf32String512>::test, "natl::Utf32String512"); 
}