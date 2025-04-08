
//natl
#include <natl/util/algorithm.h>
#include <natl/util/test.h>
#include <natl/container/dynArray.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlAlgorithmTest";

constexpr natl::DynArray<natl::String> getSortTestStringValues() noexcept {
	return {
		"57", "59", "20", "79", "21", "89", "75", "42", "80", "99",
		"53", "0", "15", "3", "88", "40", "29", "98", "76", "68",
		"90", "28", "52", "12", "63", "11", "7", "66", "96", "70",
		"84", "4", "72", "77", "50", "61", "32", "44", "74", "81",
		"43", "30", "9", "16", "18", "2", "86", "83", "36", "19",
		"49", "31", "39", "95", "62", "35", "37", "87", "24", "22",
		"10", "34", "33", "71", "58", "55", "14", "47", "13", "85",
		"100", "8", "91", "5", "73", "45", "60", "94", "67", "82",
		"46", "69", "65", "92", "64", "38", "26", "23", "97", "17",
		"25", "56", "6", "1", "78", "41", "54", "48", "93", "51", "27"
	};
}

constexpr natl::DynArray<natl::String> getSortExpectedStringValues() noexcept {
	return {
		"0", "1", "10", "100", "11", "12", "13", "14", "15", "16",
		"17", "18", "19", "2", "20", "21", "22", "23", "24", "25",
		"26", "27", "28", "29", "3", "30", "31", "32", "33", "34",
		"35", "36", "37", "38", "39", "4", "40", "41", "42", "43",
		"44", "45", "46", "47", "48", "49", "5", "50", "51", "52",
		"53", "54", "55", "56", "57", "58", "59", "6", "60", "61",
		"62", "63", "64", "65", "66", "67", "68", "69", "7", "70",
		"71", "72", "73", "74", "75", "76", "77", "78", "79", "8",
		"80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
		"9", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99"
	};
}

constexpr natl::DynArray<natl::Size> getSortTestIntValues() noexcept {
	return {
		57, 59, 20, 79, 21, 89, 75, 42, 80, 99,
		53, 0, 15, 3, 88, 40, 29, 98, 76, 68,
		90, 28, 52, 12, 63, 11, 7, 66, 96, 70,
		84, 4, 72, 77, 50, 61, 32, 44, 74, 81,
		43, 30, 9, 16, 18, 2, 86, 83, 36, 19,
		49, 31, 39, 95, 62, 35, 37, 87, 24, 22,
		10, 34, 33, 71, 58, 55, 14, 47, 13, 85,
		100, 8, 91, 5, 73, 45, 60, 94, 67, 82,
		46, 69, 65, 92, 64, 38, 26, 23, 97, 17,
		25, 56, 6, 1, 78, 41, 54, 48, 93, 51, 27
	};
}

constexpr natl::DynArray<natl::Size> getSortExpectedIntValues() noexcept {
	return {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
		40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
		50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
		60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
		70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
		80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
		90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100
	};
}



constexpr void checkSortedStringValue(natl::Test& test, const natl::DynArray<natl::String>& values) noexcept {
	natl::DynArray<natl::String> expectedStringValues = getSortExpectedStringValues();
	natl::testAssertEquals(test, expectedStringValues.size(), values.size(), "sorted string values size");
	if (expectedStringValues.size() != values.size()) {
		return;
	}

	for (const natl::Size i : natl::Repeat(values.size())) {
		natl::testAssertEquals(test, expectedStringValues[i], values[i], "sorted string value in order");
	}
}
constexpr void checkSortedIntValue(natl::Test& test, const natl::DynArray<natl::Size>& values) noexcept {
	natl::DynArray<natl::Size> expectedIntValues = getSortExpectedIntValues();
	natl::testAssertEquals(test, expectedIntValues.size(), values.size(), "sorted int values size");
	if (expectedIntValues.size() != values.size()) {
		return;
	}

	for (const natl::Size i : natl::Repeat(values.size())) {
		natl::testAssertEquals(test, expectedIntValues[i], values[i], "sorted int value in order");
	}
}

constexpr natl::Bool insertionSortTests() noexcept {
	natl::Test test(natlTestFrom, "insertion sort tests", natl::TestType::leaf);

	natl::DynArray<natl::String> stringValues = getSortTestStringValues();
	natl::insertionSort(stringValues.begin(), stringValues.end());
	checkSortedStringValue(test, stringValues);

	natl::DynArray<natl::Size> intValues = getSortTestIntValues();
	natl::insertionSort(intValues.begin(), intValues.end());
	checkSortedIntValue(test, intValues);

	return test;
}

constexpr natl::Bool quickSortTests() noexcept {
	natl::Test test(natlTestFrom, "quick sort tests", natl::TestType::leaf);

	natl::DynArray<natl::String> stringValues = getSortTestStringValues();
	natl::quickSort(stringValues.begin(), stringValues.end());
	checkSortedStringValue(test, stringValues);

	natl::DynArray<natl::Size> intValues = getSortTestIntValues();
	natl::quickSort(intValues.begin(), intValues.end());
	checkSortedIntValue(test, intValues);

	return test;
}

constexpr natl::Bool heapSortTests() noexcept {
	natl::Test test(natlTestFrom, "heap sort tests", natl::TestType::leaf);

	natl::DynArray<natl::String> stringValues = getSortTestStringValues();
	natl::heapSort(stringValues.begin(), stringValues.end());
	checkSortedStringValue(test, stringValues);

	natl::DynArray<natl::Size> intValues = getSortTestIntValues();
	natl::heapSort(intValues.begin(), intValues.end());
	checkSortedIntValue(test, intValues);

	return test;
}

constexpr natl::Bool introSortTests() noexcept {
	natl::Test test(natlTestFrom, "intro sort tests", natl::TestType::leaf);

	natl::DynArray<natl::String> stringValues = getSortTestStringValues();
	natl::introSort(stringValues.begin(), stringValues.end());
	checkSortedStringValue(test, stringValues);

	natl::DynArray<natl::Size> intValues = getSortTestIntValues();
	natl::introSort(intValues.begin(), intValues.end());
	checkSortedIntValue(test, intValues);

	return test;
}

natl::Bool tests() {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, insertionSortTests());
	natl::subTestAssert(test, quickSortTests());
	natl::subTestAssert(test, heapSortTests());
	natl::subTestAssert(test, introSortTests());
	return test;
}

int main() {
	tests();
}