
//natl
#include <natl/util/test.h>
#include <natl/util/stateful.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlStatefulTest";

NATL_DEFINE_STATEFUL_COUNTER(TestCounter)
constexpr natl::Bool statefulCounterTest() noexcept {
	natl::Test test(natlTestFrom, "stateful counter", natl::TestType::leaf);
	natl::testAssertEquals(test, natl::Size(0), TestCounter<>, " 0 index");
	natl::testAssertEquals(test, natl::Size(1), TestCounter<>, " 1 index");
	natl::testAssertEquals(test, natl::Size(2), TestCounter<>, " 2 index");
	natl::testAssertEquals(test, natl::Size(3), TestCounter<>, " 3 index");
	natl::testAssertEquals(test, natl::Size(4), TestCounter<>, " 4 index");
	return test;
}
//static_assert(statefulCounterTest());

NATL_DEFINE_STATEFUL_TYPEPACK(TestTypePack)
constexpr natl::Bool statefulTypePackTest() noexcept {
	natl::Test test(natlTestFrom, "stateful type pack", natl::TestType::leaf);
	natl::testAssertEquals(test, natl::Size(0), TestTypePack<>::size, "size 0");
	TestTypePackAppend<natl::i8>();
	natl::testAssertEquals(test, natl::Size(1), TestTypePack<>::size, "size 1");
	natl::testAssertEquals(test, natl::getNameOfType<natl::TypePack<natl::i8>>(), natl::getNameOfType<TestTypePack<>>(), "expected type at size 1");
	TestTypePackAppend<natl::i16>();
	natl::testAssertEquals(test, natl::Size(2), TestTypePack<>::size, "size 2");
	natl::testAssertEquals(test, natl::getNameOfType<natl::TypePack<natl::i8, natl::i16>>(), natl::getNameOfType<TestTypePack<>>(), "expected type at size 2");
	TestTypePackAppend<natl::i32>();
	natl::testAssertEquals(test, natl::Size(3), TestTypePack<>::size, "size 3");
	natl::testAssertEquals(test, natl::getNameOfType<natl::TypePack<natl::i8, natl::i16, natl::i32>>(), natl::getNameOfType<TestTypePack<>>(), "expected type at size 3");
	TestTypePackAppend<natl::i64>();
	natl::testAssertEquals(test, natl::Size(4), TestTypePack<>::size, "size 4");
	natl::testAssertEquals(test, natl::getNameOfType<natl::TypePack<natl::i8, natl::i16, natl::i32, natl::i64>>(), natl::getNameOfType<TestTypePack<>>(), "expected type at size 4");
	return test;
}
//static_assert(statefulTypePackTest());

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, statefulCounterTest());
	natl::subTestAssert(test, statefulTypePackTest());
	return test;
}

int main() {
	tests();
}

/*
NATL_DEFINE_STATEFUL_IMPL_START
template<class...> struct TypePack {};

namespace detail {
	template<auto> struct nth { constexpr auto friend get(nth); constexpr auto friend get(nth); };
	template<auto N, class T> struct set { constexpr auto friend get(nth<N>) { return T{}; } };
	template<class T, template<class...> class TList, class... Ts> constexpr auto append(TList<Ts...>) -> TList<Ts..., T>;
} // namespace detail

template<class T, auto N = 0, auto unique = [] {} >
consteval auto append() {
	if constexpr (requires { get(detail::nth<N>{}); }) {
		append<T, N + 1, unique>();
	} else if constexpr (N == 0) {
		void(detail::set<N, TypePack<T>>{});
	} else {
		void(detail::set < N, decltype(detail::append<T>(get(detail::nth<N - 1>{}))) > {});
	}
}

template<auto unique = [] {}, auto N = 0 >
consteval auto get_list() {
	if constexpr (requires { get(detail::nth<N>{}); }) {
		return get_list<unique, N + 1>();
	} else if constexpr (N == 0) {
		return TypePack{};
	} else {
		return get(detail::nth<N - 1>{});
	}
}
NATL_DEFINE_STATEFUL_IMPL_END

#include <typeinfo>

int main() {
	natl::printlnf(natl::getNameOfType<decltype(get_list())>());
	append<int>();
	natl::printlnf(natl::getNameOfType<decltype(get_list())>());
	append<float>();
	natl::printlnf(natl::getNameOfType<decltype(get_list())>());
}
*/