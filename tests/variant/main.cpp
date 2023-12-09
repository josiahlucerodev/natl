
//natl
#include <natl/variant.h>
#include <natl/dynArray.h>


int main() {
	//using VariantType = natl::Variant<
	//	natl::NamedElement<"int", int>,
	//	natl::NamedElement<"double", double>,
	//	natl::NamedElement<"size", natl::Size>,
	//	natl::NamedElement<"dynArray", natl::DynArray<int>>
	//>;
	//
	//natl::DynArray<int> intArray(10, 1);
	//natl::VariantAssign<"dynArray", natl::DynArray<int>> assign(intArray);
	//VariantType testVariant;
	//testVariant = natl::VariantAssignMove<"dynArray", natl::DynArray<int>>(natl::move(intArray));
	//
	//testVariant.assign<"dynArray">(natl::move(intArray));
	//natl::Option<natl::DynArray<int>*> intArrayPtr = testVariant.get<"dynArray">();
	//
	//
	//natl::DynArray<int>& intArrayRef = testVariant.get<"dynArray">();
	//if (testVariant.isValue<"dynArray">()) {
	//	std::cout << "dynArray";
	//}


	return 0;
}