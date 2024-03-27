
//natl
#include <natl/dataMovement.h>
#include <natl/smallDynArray.h>
#include <natl/dynArray.h>
#include <natl/string.h>
#include <natl/heapArray.h>


constexpr char compileTimeTest() {
	natl::DynArray<char> charArray(100, 5);
	natl::SmallDynArray<char, 10> smallCharArray;
	natl::String charString = charArray.getAlloctionMoveAdapater();
	natl::HeapArray<char> heapArray;
	charArray = charString.getAlloctionMoveAdapater();
	charString = charArray.getAlloctionMoveAdapater();
	smallCharArray = charString.getAlloctionMoveAdapater();
	charArray = smallCharArray.getAlloctionMoveAdapater();
	smallCharArray = charArray.getAlloctionMoveAdapater();
	charString = smallCharArray.getAlloctionMoveAdapater();
	heapArray = charString.getAlloctionMoveAdapater();
	charString = heapArray.getAlloctionMoveAdapater();

	return charString.at(5);
}

int main() {
	[[maybe_unused]] constexpr char testValue = compileTimeTest();
	[[maybe_unused]] char testValue2 = compileTimeTest();
}