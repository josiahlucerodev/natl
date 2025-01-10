
//natl
#include <natl/util/test.h>
#include <natl/util/reflection.h>
#include <natl/util/preprocessor.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlReflectionTest";


struct StructMembers1 {
	natl::String m1;
};
struct StructMembers2 {
	natl::String m1; natl::i8 m2;
};
struct StructMembers3 {
	natl::String m1; natl::i8 m2; natl::i16 m3;
};
struct StructMembers4 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4;
};
struct StructMembers5 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
};
struct StructMembers6 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6;
};
struct StructMembers7 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7;
};
struct StructMembers8 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8;
};
struct StructMembers9 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9;
};
struct StructMembers10 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
};
struct StructMembers11 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11;
};
struct StructMembers12 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12;
};
struct StructMembers13 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13;
};
struct StructMembers14 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13; natl::i32 m14;
};
struct StructMembers15 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13; natl::i32 m14; natl::i64 m15;
};
struct StructMembers16 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13; natl::i32 m14; natl::i64 m15;
	natl::String256 m16;
};
struct StructMembers17 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13; natl::i32 m14; natl::i64 m15;
	natl::String256 m16; natl::ui8 m17;
};
struct StructMembers18 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13; natl::i32 m14; natl::i64 m15;
	natl::String256 m16; natl::ui8 m17; natl::ui16 m18;
};
struct StructMembers19 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13; natl::i32 m14; natl::i64 m15;
	natl::String256 m16; natl::ui8 m17; natl::ui16 m18; natl::ui32 m19;
};
struct StructMembers20 {
	natl::String m1; natl::i8 m2; natl::i16 m3; natl::i32 m4; natl::i64 m5;
	natl::String256 m6; natl::ui8 m7; natl::ui16 m8; natl::ui32 m9; natl::ui64 m10;
	natl::String m11; natl::i8 m12; natl::i16 m13; natl::i32 m14; natl::i64 m15;
	natl::String256 m16; natl::ui8 m17; natl::ui16 m18; natl::ui32 m19; natl::ui64 m20;
};

constexpr natl::Bool structMemberCountTest() noexcept {
	natl::Test test(natlTestFrom, "struct member count", natl::TestType::leaf);
	natl::testAssertEquals(test, natl::Size(1), natl::StructMemberCount<StructMembers1>, "member count of StructMembers1");
	natl::testAssertEquals(test, natl::Size(2), natl::StructMemberCount<StructMembers2>, "member count of StructMembers2");
	natl::testAssertEquals(test, natl::Size(3), natl::StructMemberCount<StructMembers3>, "member count of StructMembers3");
	natl::testAssertEquals(test, natl::Size(4), natl::StructMemberCount<StructMembers4>, "member count of StructMembers4");
	natl::testAssertEquals(test, natl::Size(5), natl::StructMemberCount<StructMembers5>, "member count of StructMembers5");
	natl::testAssertEquals(test, natl::Size(6), natl::StructMemberCount<StructMembers6>, "member count of StructMembers6");
	natl::testAssertEquals(test, natl::Size(7), natl::StructMemberCount<StructMembers7>, "member count of StructMembers7");
	natl::testAssertEquals(test, natl::Size(8), natl::StructMemberCount<StructMembers8>, "member count of StructMembers8");
	natl::testAssertEquals(test, natl::Size(9), natl::StructMemberCount<StructMembers9>, "member count of StructMembers9");
	natl::testAssertEquals(test, natl::Size(10), natl::StructMemberCount<StructMembers10>, "member count of StructMembers10");
	natl::testAssertEquals(test, natl::Size(11), natl::StructMemberCount<StructMembers11>, "member count of StructMembers11");
	natl::testAssertEquals(test, natl::Size(12), natl::StructMemberCount<StructMembers12>, "member count of StructMembers12");
	natl::testAssertEquals(test, natl::Size(13), natl::StructMemberCount<StructMembers13>, "member count of StructMembers13");
	natl::testAssertEquals(test, natl::Size(14), natl::StructMemberCount<StructMembers14>, "member count of StructMembers14");
	natl::testAssertEquals(test, natl::Size(15), natl::StructMemberCount<StructMembers15>, "member count of StructMembers15");
	natl::testAssertEquals(test, natl::Size(16), natl::StructMemberCount<StructMembers16>, "member count of StructMembers16");
	natl::testAssertEquals(test, natl::Size(17), natl::StructMemberCount<StructMembers17>, "member count of StructMembers17");
	natl::testAssertEquals(test, natl::Size(18), natl::StructMemberCount<StructMembers18>, "member count of StructMembers18");
	natl::testAssertEquals(test, natl::Size(19), natl::StructMemberCount<StructMembers19>, "member count of StructMembers19");
	natl::testAssertEquals(test, natl::Size(20), natl::StructMemberCount<StructMembers20>, "member count of StructMembers20");
	return test;
}

//static_assert(structMemberCountTest());

//SMT = struct member type 
#define NATL_TEST_SMT_1(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::String>(), natl::getNameOfType<natl::StructMemberType<StructName, 0>>(), "struct member type 0 " #StructName); 
#define NATL_TEST_SMT_2(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i8>(), natl::getNameOfType<natl::StructMemberType<StructName, 1>>(), "struct member type 1 " #StructName); NATL_TEST_SMT_1(StructName)
#define NATL_TEST_SMT_3(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i16>(), natl::getNameOfType<natl::StructMemberType<StructName, 2>>(), "struct member type 2 " #StructName); NATL_TEST_SMT_2(StructName)
#define NATL_TEST_SMT_4(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i32>(), natl::getNameOfType<natl::StructMemberType<StructName, 3>>(), "struct member type 3 " #StructName); NATL_TEST_SMT_3(StructName)
#define NATL_TEST_SMT_5(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i64>(), natl::getNameOfType<natl::StructMemberType<StructName, 4>>(), "struct member type 4 " #StructName); NATL_TEST_SMT_4(StructName)
#define NATL_TEST_SMT_6(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::String256>(), natl::getNameOfType<natl::StructMemberType<StructName, 5>>(), "struct member type 5 " #StructName); NATL_TEST_SMT_5(StructName)
#define NATL_TEST_SMT_7(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui8>(), natl::getNameOfType<natl::StructMemberType<StructName, 6>>(), "struct member type 6 " #StructName); NATL_TEST_SMT_6(StructName)
#define NATL_TEST_SMT_8(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui16>(), natl::getNameOfType<natl::StructMemberType<StructName, 7>>(), "struct member type 7 " #StructName); NATL_TEST_SMT_7(StructName)
#define NATL_TEST_SMT_9(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui32>(), natl::getNameOfType<natl::StructMemberType<StructName, 8>>(), "struct member type 8 " #StructName); NATL_TEST_SMT_8(StructName)
#define NATL_TEST_SMT_10(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui64>(), natl::getNameOfType<natl::StructMemberType<StructName, 9>>(), "struct member type 9 " #StructName); NATL_TEST_SMT_9(StructName)
#define NATL_TEST_SMT_11(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::String>(), natl::getNameOfType<natl::StructMemberType<StructName, 10>>(), "struct member type 10 " #StructName); NATL_TEST_SMT_10(StructName)
#define NATL_TEST_SMT_12(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i8>(), natl::getNameOfType<natl::StructMemberType<StructName, 11>>(), "struct member type 11 " #StructName); NATL_TEST_SMT_11(StructName)
#define NATL_TEST_SMT_13(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i16>(), natl::getNameOfType<natl::StructMemberType<StructName, 12>>(), "struct member type 12 " #StructName); NATL_TEST_SMT_12(StructName)
#define NATL_TEST_SMT_14(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i32>(), natl::getNameOfType<natl::StructMemberType<StructName, 13>>(), "struct member type 13 " #StructName); NATL_TEST_SMT_13(StructName)
#define NATL_TEST_SMT_15(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::i64>(), natl::getNameOfType<natl::StructMemberType<StructName, 14>>(), "struct member type 14 " #StructName); NATL_TEST_SMT_14(StructName)
#define NATL_TEST_SMT_16(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::String256>(), natl::getNameOfType<natl::StructMemberType<StructName, 15>>(), "struct member type 15 " #StructName); NATL_TEST_SMT_15(StructName)
#define NATL_TEST_SMT_17(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui8>(), natl::getNameOfType<natl::StructMemberType<StructName, 16>>(), "struct member type 16 " #StructName); NATL_TEST_SMT_16(StructName)
#define NATL_TEST_SMT_18(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui16>(), natl::getNameOfType<natl::StructMemberType<StructName, 17>>(), "struct member type 17 " #StructName); NATL_TEST_SMT_17(StructName)
#define NATL_TEST_SMT_19(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui32>(), natl::getNameOfType<natl::StructMemberType<StructName, 18>>(), "struct member type 18 " #StructName); NATL_TEST_SMT_18(StructName)
#define NATL_TEST_SMT_20(StructName) natl::testAssertEquals(test, natl::getNameOfType<natl::ui64>(), natl::getNameOfType<natl::StructMemberType<StructName, 19>>(), "struct member type 19 " #StructName); NATL_TEST_SMT_19(StructName)

constexpr natl::Bool structMemberTypeTest() noexcept {
	natl::Test test(natlTestFrom, "struct member type", natl::TestType::leaf);
	NATL_TEST_SMT_1(StructMembers1)
	NATL_TEST_SMT_2(StructMembers2)
	NATL_TEST_SMT_3(StructMembers3)
	NATL_TEST_SMT_4(StructMembers4)
	NATL_TEST_SMT_5(StructMembers5)
	NATL_TEST_SMT_6(StructMembers6)
	NATL_TEST_SMT_7(StructMembers7)
	NATL_TEST_SMT_8(StructMembers8)
	NATL_TEST_SMT_9(StructMembers9)
	NATL_TEST_SMT_10(StructMembers10)
	NATL_TEST_SMT_11(StructMembers11)
	NATL_TEST_SMT_12(StructMembers12)
	NATL_TEST_SMT_13(StructMembers13)
	NATL_TEST_SMT_14(StructMembers14)
	NATL_TEST_SMT_15(StructMembers15)
	NATL_TEST_SMT_16(StructMembers16)
	NATL_TEST_SMT_17(StructMembers17)
	NATL_TEST_SMT_18(StructMembers18)
	NATL_TEST_SMT_19(StructMembers19)
	NATL_TEST_SMT_20(StructMembers20)
	return test;
}

//SMN = struct member name
#define NATL_TEST_SMN_1(StructName) natl::testAssertEquals(test, "m1", natl::getStructMemberName<StructName, 0>(), "struct member name 0 " #StructName); 
#define NATL_TEST_SMN_2(StructName) natl::testAssertEquals(test, "m2", natl::getStructMemberName<StructName, 1>(), "struct member name 1 " #StructName); NATL_TEST_SMN_1(StructName)
#define NATL_TEST_SMN_3(StructName) natl::testAssertEquals(test, "m3", natl::getStructMemberName<StructName, 2>(), "struct member name 2 " #StructName); NATL_TEST_SMN_2(StructName)
#define NATL_TEST_SMN_4(StructName) natl::testAssertEquals(test, "m4", natl::getStructMemberName<StructName, 3>(), "struct member name 3 " #StructName); NATL_TEST_SMN_3(StructName)
#define NATL_TEST_SMN_5(StructName) natl::testAssertEquals(test, "m5", natl::getStructMemberName<StructName, 4>(), "struct member name 4 " #StructName); NATL_TEST_SMN_4(StructName)
#define NATL_TEST_SMN_6(StructName) natl::testAssertEquals(test, "m6", natl::getStructMemberName<StructName, 5>(), "struct member name 5 " #StructName); NATL_TEST_SMN_5(StructName)
#define NATL_TEST_SMN_7(StructName) natl::testAssertEquals(test, "m7", natl::getStructMemberName<StructName, 6>(), "struct member name 6 " #StructName); NATL_TEST_SMN_6(StructName)
#define NATL_TEST_SMN_8(StructName) natl::testAssertEquals(test, "m8", natl::getStructMemberName<StructName, 7>(), "struct member name 7 " #StructName); NATL_TEST_SMN_7(StructName)
#define NATL_TEST_SMN_9(StructName) natl::testAssertEquals(test, "m9", natl::getStructMemberName<StructName, 8>(), "struct member name 8 " #StructName); NATL_TEST_SMN_8(StructName)
#define NATL_TEST_SMN_10(StructName) natl::testAssertEquals(test, "m10", natl::getStructMemberName<StructName, 9>(), "struct member name 9 " #StructName); NATL_TEST_SMN_9(StructName)
#define NATL_TEST_SMN_11(StructName) natl::testAssertEquals(test, "m11", natl::getStructMemberName<StructName, 10>(), "struct member name 10 " #StructName); NATL_TEST_SMN_10(StructName)
#define NATL_TEST_SMN_12(StructName) natl::testAssertEquals(test, "m12", natl::getStructMemberName<StructName, 11>(), "struct member name 11 " #StructName); NATL_TEST_SMN_11(StructName)
#define NATL_TEST_SMN_13(StructName) natl::testAssertEquals(test, "m13", natl::getStructMemberName<StructName, 12>(), "struct member name 12 " #StructName); NATL_TEST_SMN_12(StructName)
#define NATL_TEST_SMN_14(StructName) natl::testAssertEquals(test, "m14", natl::getStructMemberName<StructName, 13>(), "struct member name 13 " #StructName); NATL_TEST_SMN_13(StructName)
#define NATL_TEST_SMN_15(StructName) natl::testAssertEquals(test, "m15", natl::getStructMemberName<StructName, 14>(), "struct member name 14 " #StructName); NATL_TEST_SMN_14(StructName)
#define NATL_TEST_SMN_16(StructName) natl::testAssertEquals(test, "m16", natl::getStructMemberName<StructName, 15>(), "struct member name 15 " #StructName); NATL_TEST_SMN_15(StructName)
#define NATL_TEST_SMN_17(StructName) natl::testAssertEquals(test, "m17", natl::getStructMemberName<StructName, 16>(), "struct member name 16 " #StructName); NATL_TEST_SMN_16(StructName)
#define NATL_TEST_SMN_18(StructName) natl::testAssertEquals(test, "m18", natl::getStructMemberName<StructName, 17>(), "struct member name 17 " #StructName); NATL_TEST_SMN_17(StructName)
#define NATL_TEST_SMN_19(StructName) natl::testAssertEquals(test, "m19", natl::getStructMemberName<StructName, 18>(), "struct member name 18 " #StructName); NATL_TEST_SMN_18(StructName)
#define NATL_TEST_SMN_20(StructName) natl::testAssertEquals(test, "m20", natl::getStructMemberName<StructName, 19>(), "struct member name 19 " #StructName); NATL_TEST_SMN_19(StructName)

constexpr natl::Bool structMemberNameTest() noexcept {
	natl::Test test(natlTestFrom, "struct member name", natl::TestType::leaf);
	NATL_TEST_SMN_1(StructMembers1)
	NATL_TEST_SMN_2(StructMembers2)
	NATL_TEST_SMN_3(StructMembers3)
	NATL_TEST_SMN_4(StructMembers4)
	NATL_TEST_SMN_5(StructMembers5)
	NATL_TEST_SMN_6(StructMembers6)
	NATL_TEST_SMN_7(StructMembers7)
	NATL_TEST_SMN_8(StructMembers8)
	NATL_TEST_SMN_9(StructMembers9)
	NATL_TEST_SMN_10(StructMembers10)
	NATL_TEST_SMN_11(StructMembers11)
	NATL_TEST_SMN_12(StructMembers12)
	NATL_TEST_SMN_13(StructMembers13)
	NATL_TEST_SMN_14(StructMembers14)
	NATL_TEST_SMN_15(StructMembers15)
	NATL_TEST_SMN_16(StructMembers16)
	NATL_TEST_SMN_17(StructMembers17)
	NATL_TEST_SMN_18(StructMembers18)
	NATL_TEST_SMN_19(StructMembers19)
	NATL_TEST_SMN_20(StructMembers20)
	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, structMemberCountTest());
	natl::subTestAssert(test, structMemberTypeTest());
	natl::subTestAssert(test, structMemberNameTest());
	return test;
}

int main() {
	tests();
}