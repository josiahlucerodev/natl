//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
#include <natl/util/preprocessor.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/reflection.h>

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

struct ReflectionTestSuite : public natl::TestSuite<ReflectionTestSuite> {};

template<typename test_suite_type = ReflectionTestSuite>
struct ReflectionTestSuiteTests {
	constexpr static void structMemberCountTest(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Size(1), natl::StructMemberCount<StructMembers1>);
		ts.assertEqual(natl::Size(2), natl::StructMemberCount<StructMembers2>);
		ts.assertEqual(natl::Size(3), natl::StructMemberCount<StructMembers3>);
		ts.assertEqual(natl::Size(4), natl::StructMemberCount<StructMembers4>);
		ts.assertEqual(natl::Size(5), natl::StructMemberCount<StructMembers5>);
		ts.assertEqual(natl::Size(6), natl::StructMemberCount<StructMembers6>);
		ts.assertEqual(natl::Size(7), natl::StructMemberCount<StructMembers7>);
		ts.assertEqual(natl::Size(8), natl::StructMemberCount<StructMembers8>);
		ts.assertEqual(natl::Size(9), natl::StructMemberCount<StructMembers9>);
		ts.assertEqual(natl::Size(10), natl::StructMemberCount<StructMembers10>);
		ts.assertEqual(natl::Size(11), natl::StructMemberCount<StructMembers11>);
		ts.assertEqual(natl::Size(12), natl::StructMemberCount<StructMembers12>);
		ts.assertEqual(natl::Size(13), natl::StructMemberCount<StructMembers13>);
		ts.assertEqual(natl::Size(14), natl::StructMemberCount<StructMembers14>);
		ts.assertEqual(natl::Size(15), natl::StructMemberCount<StructMembers15>);
		ts.assertEqual(natl::Size(16), natl::StructMemberCount<StructMembers16>);
		ts.assertEqual(natl::Size(17), natl::StructMemberCount<StructMembers17>);
		ts.assertEqual(natl::Size(18), natl::StructMemberCount<StructMembers18>);
		ts.assertEqual(natl::Size(19), natl::StructMemberCount<StructMembers19>);
		ts.assertEqual(natl::Size(20), natl::StructMemberCount<StructMembers20>);
	}
	NATL_REGISTER_TEST_CONSTEXPR(structMemberCountTest);

	//SMT = struct member type 
#define NATL_TEST_SMT_1(StructName) ts.assertEqual(natl::getNameOfType<natl::String>(), natl::getNameOfType<natl::StructMemberType<0, StructName>>()); 
#define NATL_TEST_SMT_2(StructName) ts.assertEqual(natl::getNameOfType<natl::i8>(), natl::getNameOfType<natl::StructMemberType<1, StructName>>()); NATL_TEST_SMT_1(StructName)
#define NATL_TEST_SMT_3(StructName) ts.assertEqual(natl::getNameOfType<natl::i16>(), natl::getNameOfType<natl::StructMemberType<2, StructName>>()); NATL_TEST_SMT_2(StructName)
#define NATL_TEST_SMT_4(StructName) ts.assertEqual(natl::getNameOfType<natl::i32>(), natl::getNameOfType<natl::StructMemberType<3, StructName>>()); NATL_TEST_SMT_3(StructName)
#define NATL_TEST_SMT_5(StructName) ts.assertEqual(natl::getNameOfType<natl::i64>(), natl::getNameOfType<natl::StructMemberType<4, StructName>>()); NATL_TEST_SMT_4(StructName)
#define NATL_TEST_SMT_6(StructName) ts.assertEqual(natl::getNameOfType<natl::String256>(), natl::getNameOfType<natl::StructMemberType<5, StructName>>()); NATL_TEST_SMT_5(StructName)
#define NATL_TEST_SMT_7(StructName) ts.assertEqual(natl::getNameOfType<natl::ui8>(), natl::getNameOfType<natl::StructMemberType<6, StructName>>()); NATL_TEST_SMT_6(StructName)
#define NATL_TEST_SMT_8(StructName) ts.assertEqual(natl::getNameOfType<natl::ui16>(), natl::getNameOfType<natl::StructMemberType<7, StructName>>()); NATL_TEST_SMT_7(StructName)
#define NATL_TEST_SMT_9(StructName) ts.assertEqual(natl::getNameOfType<natl::ui32>(), natl::getNameOfType<natl::StructMemberType<8, StructName>>()); NATL_TEST_SMT_8(StructName)
#define NATL_TEST_SMT_10(StructName) ts.assertEqual(natl::getNameOfType<natl::ui64>(), natl::getNameOfType<natl::StructMemberType<9, StructName>>()); NATL_TEST_SMT_9(StructName)
#define NATL_TEST_SMT_11(StructName) ts.assertEqual(natl::getNameOfType<natl::String>(), natl::getNameOfType<natl::StructMemberType<10, StructName>>()); NATL_TEST_SMT_10(StructName)
#define NATL_TEST_SMT_12(StructName) ts.assertEqual(natl::getNameOfType<natl::i8>(), natl::getNameOfType<natl::StructMemberType<11, StructName>>()); NATL_TEST_SMT_11(StructName)
#define NATL_TEST_SMT_13(StructName) ts.assertEqual(natl::getNameOfType<natl::i16>(), natl::getNameOfType<natl::StructMemberType<12, StructName>>()); NATL_TEST_SMT_12(StructName)
#define NATL_TEST_SMT_14(StructName) ts.assertEqual(natl::getNameOfType<natl::i32>(), natl::getNameOfType<natl::StructMemberType<13, StructName>>()); NATL_TEST_SMT_13(StructName)
#define NATL_TEST_SMT_15(StructName) ts.assertEqual(natl::getNameOfType<natl::i64>(), natl::getNameOfType<natl::StructMemberType<14, StructName>>()); NATL_TEST_SMT_14(StructName)
#define NATL_TEST_SMT_16(StructName) ts.assertEqual(natl::getNameOfType<natl::String256>(), natl::getNameOfType<natl::StructMemberType<15, StructName>>()); NATL_TEST_SMT_15(StructName)
#define NATL_TEST_SMT_17(StructName) ts.assertEqual(natl::getNameOfType<natl::ui8>(), natl::getNameOfType<natl::StructMemberType<16, StructName>>()); NATL_TEST_SMT_16(StructName)
#define NATL_TEST_SMT_18(StructName) ts.assertEqual(natl::getNameOfType<natl::ui16>(), natl::getNameOfType<natl::StructMemberType<17, StructName>>()); NATL_TEST_SMT_17(StructName)
#define NATL_TEST_SMT_19(StructName) ts.assertEqual(natl::getNameOfType<natl::ui32>(), natl::getNameOfType<natl::StructMemberType<18, StructName>>()); NATL_TEST_SMT_18(StructName)
#define NATL_TEST_SMT_20(StructName) ts.assertEqual(natl::getNameOfType<natl::ui64>(), natl::getNameOfType<natl::StructMemberType<19, StructName>>()); NATL_TEST_SMT_19(StructName)

	constexpr static void structMemberTypeTest(test_suite_type& ts) noexcept {
		NATL_TEST_SMT_1(StructMembers1);
		NATL_TEST_SMT_2(StructMembers2);
		NATL_TEST_SMT_3(StructMembers3);
		NATL_TEST_SMT_4(StructMembers4);
		NATL_TEST_SMT_5(StructMembers5);
		NATL_TEST_SMT_6(StructMembers6);
		NATL_TEST_SMT_7(StructMembers7);
		NATL_TEST_SMT_8(StructMembers8);
		NATL_TEST_SMT_9(StructMembers9);
		NATL_TEST_SMT_10(StructMembers10);
		NATL_TEST_SMT_11(StructMembers11);
		NATL_TEST_SMT_12(StructMembers12);
		NATL_TEST_SMT_13(StructMembers13);
		NATL_TEST_SMT_14(StructMembers14);
		NATL_TEST_SMT_15(StructMembers15);
		NATL_TEST_SMT_16(StructMembers16);
		NATL_TEST_SMT_17(StructMembers17);
		NATL_TEST_SMT_18(StructMembers18);
		NATL_TEST_SMT_19(StructMembers19);
		NATL_TEST_SMT_20(StructMembers20);
	}
	NATL_REGISTER_TEST_CONSTEXPR(structMemberTypeTest);

	//SMN = struct member name
#define NATL_TEST_SMN_1(StructName) ts.assertEqual("m1", natl::getStructMemberName<StructName, 0>()); 
#define NATL_TEST_SMN_2(StructName) ts.assertEqual("m2", natl::getStructMemberName<StructName, 1>()); NATL_TEST_SMN_1(StructName)
#define NATL_TEST_SMN_3(StructName) ts.assertEqual("m3", natl::getStructMemberName<StructName, 2>()); NATL_TEST_SMN_2(StructName)
#define NATL_TEST_SMN_4(StructName) ts.assertEqual("m4", natl::getStructMemberName<StructName, 3>()); NATL_TEST_SMN_3(StructName)
#define NATL_TEST_SMN_5(StructName) ts.assertEqual("m5", natl::getStructMemberName<StructName, 4>()); NATL_TEST_SMN_4(StructName)
#define NATL_TEST_SMN_6(StructName) ts.assertEqual("m6", natl::getStructMemberName<StructName, 5>()); NATL_TEST_SMN_5(StructName)
#define NATL_TEST_SMN_7(StructName) ts.assertEqual("m7", natl::getStructMemberName<StructName, 6>()); NATL_TEST_SMN_6(StructName)
#define NATL_TEST_SMN_8(StructName) ts.assertEqual("m8", natl::getStructMemberName<StructName, 7>()); NATL_TEST_SMN_7(StructName)
#define NATL_TEST_SMN_9(StructName) ts.assertEqual("m9", natl::getStructMemberName<StructName, 8>()); NATL_TEST_SMN_8(StructName)
#define NATL_TEST_SMN_10(StructName) ts.assertEqual("m10", natl::getStructMemberName<StructName, 9>()); NATL_TEST_SMN_9(StructName)
#define NATL_TEST_SMN_11(StructName) ts.assertEqual("m11", natl::getStructMemberName<StructName, 10>()); NATL_TEST_SMN_10(StructName)
#define NATL_TEST_SMN_12(StructName) ts.assertEqual("m12", natl::getStructMemberName<StructName, 11>()); NATL_TEST_SMN_11(StructName)
#define NATL_TEST_SMN_13(StructName) ts.assertEqual("m13", natl::getStructMemberName<StructName, 12>()); NATL_TEST_SMN_12(StructName)
#define NATL_TEST_SMN_14(StructName) ts.assertEqual("m14", natl::getStructMemberName<StructName, 13>()); NATL_TEST_SMN_13(StructName)
#define NATL_TEST_SMN_15(StructName) ts.assertEqual("m15", natl::getStructMemberName<StructName, 14>()); NATL_TEST_SMN_14(StructName)
#define NATL_TEST_SMN_16(StructName) ts.assertEqual("m16", natl::getStructMemberName<StructName, 15>()); NATL_TEST_SMN_15(StructName)
#define NATL_TEST_SMN_17(StructName) ts.assertEqual("m17", natl::getStructMemberName<StructName, 16>()); NATL_TEST_SMN_16(StructName)
#define NATL_TEST_SMN_18(StructName) ts.assertEqual("m18", natl::getStructMemberName<StructName, 17>()); NATL_TEST_SMN_17(StructName)
#define NATL_TEST_SMN_19(StructName) ts.assertEqual("m19", natl::getStructMemberName<StructName, 18>()); NATL_TEST_SMN_18(StructName)
#define NATL_TEST_SMN_20(StructName) ts.assertEqual("m20", natl::getStructMemberName<StructName, 19>()); NATL_TEST_SMN_19(StructName)

	constexpr static void structMemberNameTest(test_suite_type& ts) noexcept {
		NATL_TEST_SMN_1(StructMembers1);
		NATL_TEST_SMN_2(StructMembers2);
		NATL_TEST_SMN_3(StructMembers3);
		NATL_TEST_SMN_4(StructMembers4);
		NATL_TEST_SMN_5(StructMembers5);
		NATL_TEST_SMN_6(StructMembers6);
		NATL_TEST_SMN_7(StructMembers7);
		NATL_TEST_SMN_8(StructMembers8);
		NATL_TEST_SMN_9(StructMembers9);
		NATL_TEST_SMN_10(StructMembers10);
		NATL_TEST_SMN_11(StructMembers11);
		NATL_TEST_SMN_12(StructMembers12);
		NATL_TEST_SMN_13(StructMembers13);
		NATL_TEST_SMN_14(StructMembers14);
		NATL_TEST_SMN_15(StructMembers15);
		NATL_TEST_SMN_16(StructMembers16);
		NATL_TEST_SMN_17(StructMembers17);
		NATL_TEST_SMN_18(StructMembers18);
		NATL_TEST_SMN_19(StructMembers19);
		NATL_TEST_SMN_20(StructMembers20);
	}
	NATL_REGISTER_TEST_CONSTEXPR(structMemberNameTest);
};

NATL_REGISTER_STD_TESTS(ReflectionTestSuite);

NATL_TESTS_MAIN();