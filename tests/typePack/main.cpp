
//natl
#include <natl/printFormatted.h>
#include <natl/typePack.h>
#include <natl/typeInfo.h>

using TestTypePack = natl::TypePack<natl::i8, natl::i16, natl::i32, natl::i64>;

static_assert(natl::IsSame<natl::TypePackTake<2, TestTypePack>, natl::TypePack<natl::i8, natl::i16>>);


template<typename Type>
struct IsLessThanInt32V {
	constexpr static natl::Bool value = natl::IsSame<Type, natl::i8> || natl::IsSame<Type, natl::i16>;
};
using TypePackFilteredTestType = natl::TypePackFilter<IsLessThanInt32V, TestTypePack>; 
static_assert(natl::IsSame<TypePackFilteredTestType, natl::TypePack<natl::i8, natl::i16>>);

using TypePackUniformTestType = natl::TypePackUniform<3, TestTypePack>;
static_assert(natl::IsSame<TypePackUniformTestType, natl::TypePack<TestTypePack, TestTypePack, TestTypePack>>);

static_assert(natl::packAt<1>(2, 3, 4) == 3);

int main() noexcept {}