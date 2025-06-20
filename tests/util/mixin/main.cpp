
//natl
#include <natl/util/test.h>
#include <natl/util/mixin.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlMixinTest";

struct StructMembers1 : 
	natl::AggregateConstructorMixin<StructMembers1>,
	natl::ComparableMixin<StructMembers1>, natl::HashMixin<StructMembers1> {
	natl::String m1;
};

static_assert(natl::IsMixinC<natl::AggregateConstructorMixin<StructMembers1>>);
static_assert(natl::IsMixinC<natl::EqualityMixin<StructMembers1>>);
static_assert(natl::IsMixinC<natl::HashMixin<StructMembers1>>);

struct StructMembers2 : 
	natl::AggregateConstructorMixin<StructMembers2>,
	natl::ComparableMixin<StructMembers2>, natl::HashMixin<StructMembers2> {
	natl::String m1; natl::i8 m2;
};
struct StructMembers3 : 
	natl::AggregateConstructorMixin<StructMembers3>,
	natl::ComparableMixin<StructMembers3>, natl::HashMixin<StructMembers3> {
	natl::String m1; natl::i8 m2; natl::i16 m3;
};
struct StructMembers4 : 
	natl::AggregateConstructorMixin<StructMembers4>,
	natl::ComparableMixin<StructMembers4>, natl::HashMixin<StructMembers4> {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4;
};

constexpr natl::Bool equalityMixinTests() noexcept {

	natl::Test test(natlTestFrom, "equality mixin", natl::TestType::leaf);
	{
		StructMembers1 sm1_1 = StructMembers1::construct("1");
		StructMembers1 sm1_2 = StructMembers1::construct("1");
		StructMembers1 sm1_3 = StructMembers1::construct("2");
		natl::testAssert(test, sm1_1 == sm1_2);
		natl::testAssert(test, sm1_1 != sm1_3);
	}

	{
		StructMembers2 sm2_1 = StructMembers2::construct("1", 2);
		StructMembers2 sm2_2 = StructMembers2::create("1", 2);
		StructMembers2 sm2_3 = StructMembers2::create("1", 3);
		natl::testAssert(test, sm2_1 == sm2_2);
		natl::testAssert(test, sm2_1 != sm2_3);
	}

	{
		StructMembers3 sm3_1 = StructMembers3::construct("1", 2, 3);
		StructMembers3 sm3_2 = StructMembers3::construct("1", 2, 3);
		StructMembers3 sm3_3 = StructMembers3::construct("1", 2, 4);
		natl::testAssert(test, sm3_1 == sm3_2);
		natl::testAssert(test, sm3_1 != sm3_3);
	}

	{
		StructMembers4 sm4_1 = StructMembers4::construct("1", 2, 3, 4);
		StructMembers4 sm4_2 = StructMembers4::construct("1", 2, 3, 4);
		StructMembers4 sm4_3 = StructMembers4::construct("1", 2, 3, 5);
		natl::testAssert(test, sm4_1 == sm4_2);
		natl::testAssert(test, sm4_1 != sm4_3);
	}

	return test;
}
static_assert(equalityMixinTests());

constexpr natl::Bool hashMixinTests() noexcept {
	natl::Test test(natlTestFrom, "hash mixin", natl::TestType::leaf);

	StructMembers1 sm1 = StructMembers1::construct("1");
	StructMembers2 sm2 = StructMembers2::construct("1", 2);
	StructMembers3 sm3 = StructMembers3::construct("1", 2, 3);
	StructMembers4 sm4 = StructMembers4::construct("1", 2, 3, 4);

	natl::testAssertNotEquals(test, 0, sm1.hash());
	natl::testAssertNotEquals(test, 0, sm2.hash());
	natl::testAssertNotEquals(test, 0, sm3.hash());
	natl::testAssertNotEquals(test, 0, sm4.hash());

	natl::testAssertNotEquals(test, 0, natl::hashValue(sm1));
	natl::testAssertNotEquals(test, 0, natl::hashValue(sm2));
	natl::testAssertNotEquals(test, 0, natl::hashValue(sm3));
	natl::testAssertNotEquals(test, 0, natl::hashValue(sm4));

	natl::testAssertEquals(test, sm1.hash(), natl::hashValue(sm1));
	natl::testAssertEquals(test, sm2.hash(), natl::hashValue(sm2));
	natl::testAssertEquals(test, sm3.hash(), natl::hashValue(sm3));
	natl::testAssertEquals(test, sm4.hash(), natl::hashValue(sm4));

	return test;
}
static_assert(hashMixinTests());

natl::Bool orderingMixinTests() noexcept {
	natl::Test test(natlTestFrom, "hash mixin", natl::TestType::leaf);

	auto preformTests = [&] <typename Type>(const Type& sm1, const Type& sm2) {
		const Type sm3 = sm1;
		natl::testAssert(test, sm1 == sm3, "equal");
		natl::testAssert(test, not (sm1 == sm2), "not (equal)");
		natl::testAssert(test, sm1 != sm2, "notequal");
		natl::testAssert(test, not (sm1 != sm3), "not (notequal)");

		natl::testAssert(test, sm1 < sm2, "less than");
		natl::testAssert(test, not (sm2 < sm1), "not (less than)");
		natl::testAssert(test, sm1 <= sm1, "less than equal self");
		natl::testAssert(test, sm1 <= sm3, "less than equal");
		natl::testAssert(test, not (sm2 <= sm1), "not (less than equal)");

		natl::testAssert(test, sm2 > sm1, "greater than");
		natl::testAssert(test, not (sm1 > sm2), "not (greater than)");
		natl::testAssert(test, sm2 >= sm2, "greater than equal");
		natl::testAssert(test, sm2 >= sm3, "greater than equal");
		natl::testAssert(test, not (sm1 >= sm2), "not (greater than equal)");
	};

	preformTests(StructMembers1::construct("1"), StructMembers1::construct("2"));
	preformTests(StructMembers2::construct("1", 2), StructMembers2::construct("1", 3));
	preformTests(StructMembers3::construct("1", 2, 3), StructMembers3::construct("1", 2, 4));
	preformTests(StructMembers4::construct("1", 2, 3, 4), StructMembers4::construct("1", 2, 3, 5));

	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, equalityMixinTests());
	natl::subTestAssert(test, hashMixinTests());
	natl::subTestAssert(test, orderingMixinTests());
	return test;
}

int main() {
	tests();
}