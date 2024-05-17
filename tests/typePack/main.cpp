
//natl
#include <natl/printFormatted.h>
#include <natl/typePack.h>
#include <natl/typeInfo.h>

using TestTypePack = natl::TypePack<natl::i8, natl::i16, natl::i32, natl::i64>;

static_assert(natl::IsSame<natl::TypePackTake<2, TestTypePack>, natl::TypePack<natl::i8, natl::i16>>);


int main() noexcept {
}