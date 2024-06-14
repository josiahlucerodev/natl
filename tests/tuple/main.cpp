
//natl
#include <natl/fundamental/tuple.h>
#include <natl/container/string.h>
#include <natl/processing/format.h>

static_assert(natl::format<natl::String256>(natl::formatArgText<"0: FULL", "1: s">(natl::Tuple<natl::Bool, natl::Bool>(false, true))) == "{FALSE, t}");

constexpr natl::Size addFunction(natl::Size a, natl::Size b) noexcept {
	return a + b;
}

constexpr natl::Size constexprTest(const natl::i64 number) noexcept {
	using namespace natl::literals;
	[[maybe_unused]] natl::Tuple<natl::i64, natl::i64> tuple(3, 2);
	[[maybe_unused]] auto tuple2 = natl::makeTuple(3, number);
	[[maybe_unused]] natl::Bool tupleEqualTestValue = tuple == tuple2;
	[[maybe_unused]] natl::Size tupleSize = natl::TupleSize<decltype(tuple2)>;
	//[[maybe_unused]] natl::Size addFunctionValue = natl::callFunctionWithTuple(addFunction, tuple2);
	[[maybe_unused]] auto threeWayReturn = tuple <=> tuple2;
	[[maybe_unused]] auto h = natl::hashValue(tuple2.get<1>());
	[[maybe_unused]] auto tupleHash = tuple.hash();
	return static_cast<natl::Size>(tuple2.get<1_size>());
}

int main() {
	[[maybe_unused]] constexpr natl::Size testValue1 = constexprTest(2);
	[[maybe_unused]] natl::Size testValue2 = constexprTest(2);
}