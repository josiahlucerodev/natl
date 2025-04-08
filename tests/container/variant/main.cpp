
//natl
#include <natl/container/variant.h>
#include <natl/container/dynArray.h>
#include <natl/util/test.h>


int main() {
	using VariantTestType = natl::Variant<
			natl::NamedElement<"int", int>,
			natl::NamedElement<"test type", natl::TestTypeStdOut>,
			natl::NamedElement<"test1 type", double>,
			natl::NamedElement<"test2 type", float>,
			natl::NamedElement<"test3 type", short>,
			natl::NamedElement<"test4 type", int>
	>;

	VariantTestType testValue;
	testValue.assign<"test type">(natl::TestTypeStdOut());
	testValue.assign<"int">(6);

	return testValue.get<"int">();
}