
//natl
#include <natl/util/typeInfo.h>
#include <natl/system/print.h>


int main() {
	constexpr natl::TypeInfo typeInfo = natl::getTypeInfo<natl::Size>();
	constexpr natl::ConstAsciiStringView nameOfType = typeInfo.name();
	natl::println(nameOfType.c_str(), nameOfType.size());
}