#pragma once 

//own
#include "basicTypes.h"
#include "preprocessor.h"
#include "../fundamental/tuple.h"

//interface 
namespace natl {
	constexpr static inline Size MaxStructMemberNumberEvaluated = 20;

	namespace impl {
		struct UC { //UniversallyCastable
			template<typename Type>
			constexpr operator Type && () const noexcept;
		};

		template<typename Type>
		struct StructMemberCountV;

#define NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(N, N2) \
		template<typename Type> \
			requires(requires() { { Type{ NATL_REPEAT_WITH_COMMAS(N, UC{}) } }; } && !requires() { { Type{ NATL_REPEAT_WITH_COMMAS(N2, UC{}) } }; }) \
		struct StructMemberCountV<Type> { \
			constexpr static Size value = N; \
		}

		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(1, 2);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(2, 3);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(3, 4);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(4, 5);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(5, 6);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(6, 7);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(7, 8);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(8, 9);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(9, 10);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(10, 11);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(11, 12);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(12, 13);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(13, 14);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(14, 15);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(15, 16);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(16, 17);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(17, 18);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(18, 19);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(19, 20);
		NATL_IMPL_STRUCT_MEMBER_COUNT_BASE(20, 21);

#undef NATL_IMPL_STRUCT_MEMBER_COUNT_IMPL_2D
#undef NATL_IMPL_STRUCT_MEMBER_COUNT_IMPL_1D
	}


	template<typename Type>
	constexpr Size StructMemberCount = impl::StructMemberCountV<Type>::value;

	namespace impl {
		template<Size Index, Size MemberCount>
		struct GetStructMemberImpl;

		/*
			for i in range(20 + 1):
				print("#undef NATL_IMPL_GET_STRUCT_MEMBER_" + str(i))
			for i in range(20 + 2):
				print("#define NATL_IMPL_GET_STRUCT_MEMBER_" + str(i) + " NATL_IMPL_GET_STRUCT_MEMBER_" + str(i - 1) + " NATL_IMPL_GET_STRUCT_MEMBER_BASE(" + str(i - 1) + ")")
			for i in range(20 + 1):
				print("template<Size Index> struct ", end="")
				print("GetStructMemberImpl<Index, " + str(i) + "> {")
				print("\tconstexpr static auto&& get(auto&& value) noexcept {")
				print("\t\tauto&& [",  end="")
				for j in range(i):
					print("m" + str(j), end="")
					if j != i - 1:
						print(", ", end ="")
				print("] = value;")
				print("\t\tNATL_IMPL_GET_STRUCT_MEMBER_" + str(i))
				print("\t}")
				print("};")
		*/

#define NATL_IMPL_GET_STRUCT_MEMBER_BASE(INDEX) else if constexpr(Index == INDEX) { return m##INDEX; }
#define NATL_IMPL_GET_STRUCT_MEMBER_1 if constexpr(Index == 0) { return m0; }
#define NATL_IMPL_GET_STRUCT_MEMBER_2 NATL_IMPL_GET_STRUCT_MEMBER_1 NATL_IMPL_GET_STRUCT_MEMBER_BASE(1)
#define NATL_IMPL_GET_STRUCT_MEMBER_3 NATL_IMPL_GET_STRUCT_MEMBER_2 NATL_IMPL_GET_STRUCT_MEMBER_BASE(2)
#define NATL_IMPL_GET_STRUCT_MEMBER_4 NATL_IMPL_GET_STRUCT_MEMBER_3 NATL_IMPL_GET_STRUCT_MEMBER_BASE(3)
#define NATL_IMPL_GET_STRUCT_MEMBER_5 NATL_IMPL_GET_STRUCT_MEMBER_4 NATL_IMPL_GET_STRUCT_MEMBER_BASE(4)
#define NATL_IMPL_GET_STRUCT_MEMBER_6 NATL_IMPL_GET_STRUCT_MEMBER_5 NATL_IMPL_GET_STRUCT_MEMBER_BASE(5)
#define NATL_IMPL_GET_STRUCT_MEMBER_7 NATL_IMPL_GET_STRUCT_MEMBER_6 NATL_IMPL_GET_STRUCT_MEMBER_BASE(6)
#define NATL_IMPL_GET_STRUCT_MEMBER_8 NATL_IMPL_GET_STRUCT_MEMBER_7 NATL_IMPL_GET_STRUCT_MEMBER_BASE(7)
#define NATL_IMPL_GET_STRUCT_MEMBER_9 NATL_IMPL_GET_STRUCT_MEMBER_8 NATL_IMPL_GET_STRUCT_MEMBER_BASE(8)
#define NATL_IMPL_GET_STRUCT_MEMBER_10 NATL_IMPL_GET_STRUCT_MEMBER_9 NATL_IMPL_GET_STRUCT_MEMBER_BASE(9)
#define NATL_IMPL_GET_STRUCT_MEMBER_11 NATL_IMPL_GET_STRUCT_MEMBER_10 NATL_IMPL_GET_STRUCT_MEMBER_BASE(10)
#define NATL_IMPL_GET_STRUCT_MEMBER_12 NATL_IMPL_GET_STRUCT_MEMBER_11 NATL_IMPL_GET_STRUCT_MEMBER_BASE(11)
#define NATL_IMPL_GET_STRUCT_MEMBER_13 NATL_IMPL_GET_STRUCT_MEMBER_12 NATL_IMPL_GET_STRUCT_MEMBER_BASE(12)
#define NATL_IMPL_GET_STRUCT_MEMBER_14 NATL_IMPL_GET_STRUCT_MEMBER_13 NATL_IMPL_GET_STRUCT_MEMBER_BASE(13)
#define NATL_IMPL_GET_STRUCT_MEMBER_15 NATL_IMPL_GET_STRUCT_MEMBER_14 NATL_IMPL_GET_STRUCT_MEMBER_BASE(14)
#define NATL_IMPL_GET_STRUCT_MEMBER_16 NATL_IMPL_GET_STRUCT_MEMBER_15 NATL_IMPL_GET_STRUCT_MEMBER_BASE(15)
#define NATL_IMPL_GET_STRUCT_MEMBER_17 NATL_IMPL_GET_STRUCT_MEMBER_16 NATL_IMPL_GET_STRUCT_MEMBER_BASE(16)
#define NATL_IMPL_GET_STRUCT_MEMBER_18 NATL_IMPL_GET_STRUCT_MEMBER_17 NATL_IMPL_GET_STRUCT_MEMBER_BASE(17)
#define NATL_IMPL_GET_STRUCT_MEMBER_19 NATL_IMPL_GET_STRUCT_MEMBER_18 NATL_IMPL_GET_STRUCT_MEMBER_BASE(18)
#define NATL_IMPL_GET_STRUCT_MEMBER_20 NATL_IMPL_GET_STRUCT_MEMBER_19 NATL_IMPL_GET_STRUCT_MEMBER_BASE(19)
#define NATL_IMPL_GET_STRUCT_MEMBER_21 NATL_IMPL_GET_STRUCT_MEMBER_20 NATL_IMPL_GET_STRUCT_MEMBER_BASE(20)

		template<Size Index> struct GetStructMemberImpl<Index, 1> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_1
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 2> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_2
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 3> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_3
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 4> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_4
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 5> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_5
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 6> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_6
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 7> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_7
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 8> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_8
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 9> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_9
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 10> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_10
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 11> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_11
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 12> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_12
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 13> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_13
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 14> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_14
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 15> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_15
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 16> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_16
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 17> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_17
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 18> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_18
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 19> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_19
			}
		};
		template<Size Index> struct GetStructMemberImpl<Index, 20> {
			constexpr static auto&& get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19] = value;
				NATL_IMPL_GET_STRUCT_MEMBER_20
			}
		};

#undef NATL_IMPL_GET_STRUCT_MEMBER_0
#undef NATL_IMPL_GET_STRUCT_MEMBER_1
#undef NATL_IMPL_GET_STRUCT_MEMBER_2
#undef NATL_IMPL_GET_STRUCT_MEMBER_3
#undef NATL_IMPL_GET_STRUCT_MEMBER_4
#undef NATL_IMPL_GET_STRUCT_MEMBER_5
#undef NATL_IMPL_GET_STRUCT_MEMBER_6
#undef NATL_IMPL_GET_STRUCT_MEMBER_7
#undef NATL_IMPL_GET_STRUCT_MEMBER_8
#undef NATL_IMPL_GET_STRUCT_MEMBER_9
#undef NATL_IMPL_GET_STRUCT_MEMBER_10
#undef NATL_IMPL_GET_STRUCT_MEMBER_11
#undef NATL_IMPL_GET_STRUCT_MEMBER_12
#undef NATL_IMPL_GET_STRUCT_MEMBER_13
#undef NATL_IMPL_GET_STRUCT_MEMBER_14
#undef NATL_IMPL_GET_STRUCT_MEMBER_15
#undef NATL_IMPL_GET_STRUCT_MEMBER_16
#undef NATL_IMPL_GET_STRUCT_MEMBER_17
#undef NATL_IMPL_GET_STRUCT_MEMBER_18
#undef NATL_IMPL_GET_STRUCT_MEMBER_19
#undef NATL_IMPL_GET_STRUCT_MEMBER_20
	}

	template<Size Index, typename Type>
		requires(IsStructC<Decay<Type>> && StructMemberCount<Type> <= MaxStructMemberNumberEvaluated && Index < StructMemberCount<Type>)
	constexpr auto&& getStructMember(Type&& value) noexcept {
		return impl::GetStructMemberImpl<Index, StructMemberCount<Type>>::get(value);
	}

	template<typename Type, Size Index>
		requires(IsStructC<Decay<Type>> && StructMemberCount<Type> <= MaxStructMemberNumberEvaluated && Index < StructMemberCount<Type>)
	using StructMemberType = Decay<decltype(impl::GetStructMemberImpl<Index, StructMemberCount<Type>>::get(declval<Type>()))>;

	namespace impl {
		template<Size Index>
		struct GetStructMembersImpl;

		/*autofor i in range(1, 21):
				print("template<> struct GetStructMembersImpl<" + str(i) + ">
				print("\tconstexpr static decltype(auto) get(auto&& value) noexcept {")
				for j in range(i):
					print("m" + str(j), end="")
					if j != i - 1:
						print(", ", end ="")
				print("] = value;")
				print("\t\treturn tie(", end="")
				for j in range(i):
					print("m" + str(j), end="")
					if j != i - 1:
						print(", ", end ="")
				print(");")
				print("\t}")
				print("};")
		*/

		template<> struct GetStructMembersImpl<1> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0] = value;
				return tie(m0);
			}
		};
		template<> struct GetStructMembersImpl<2> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1] = value;
				return tie(m0, m1);
			}
		};
		template<> struct GetStructMembersImpl<3> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2] = value;
				return tie(m0, m1, m2);
			}
		};
		template<> struct GetStructMembersImpl<4> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3] = value;
				return tie(m0, m1, m2, m3);
			}
		};
		template<> struct GetStructMembersImpl<5> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4] = value;
				return tie(m0, m1, m2, m3, m4);
			}
		};
		template<> struct GetStructMembersImpl<6> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5] = value;
				return tie(m0, m1, m2, m3, m4, m5);
			}
		};
		template<> struct GetStructMembersImpl<7> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6);
			}
		};
		template<> struct GetStructMembersImpl<8> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7);
			}
		};
		template<> struct GetStructMembersImpl<9> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8);
			}
		};
		template<> struct GetStructMembersImpl<10> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9);
			}
		};
		template<> struct GetStructMembersImpl<11> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10);
			}
		};
		template<> struct GetStructMembersImpl<12> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11);
			}
		};
		template<> struct GetStructMembersImpl<13> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12);
			}
		};
		template<> struct GetStructMembersImpl<14> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13);
			}
		};
		template<> struct GetStructMembersImpl<15> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14);
			}
		};
		template<> struct GetStructMembersImpl<16> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15);
			}
		};
		template<> struct GetStructMembersImpl<17> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16);
			}
		};
		template<> struct GetStructMembersImpl<18> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17);
			}
		};
		template<> struct GetStructMembersImpl<19> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18);
			}
		};
		template<> struct GetStructMembersImpl<20> {
			constexpr static decltype(auto) get(auto&& value) noexcept {
				auto&& [m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19] = value;
				return tie(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19);
			}
		};
	}

	template<typename Type>
		requires(IsStructC<Decay<Type>> && StructMemberCount<Type> <= MaxStructMemberNumberEvaluated)
	constexpr decltype(auto) getStructMembers(Type&& value) noexcept {
		return impl::GetStructMembersImpl<StructMemberCount<Type>>::get(value);
	}
	template<typename Type>
		requires(StructMemberCount<Type> <= MaxStructMemberNumberEvaluated)
	using StructMemberTypes =
		TypePackTransform<
			RemoveReferenceT,
			typename decltype(impl::GetStructMembersImpl<StructMemberCount<Type>>::get(declval<Type>()))::value_types>;

	template<typename Type>
	[[maybe_unused]] extern const Type external;

	namespace impl {
		template<typename Type, auto V>
		consteval auto getStructMemberNameImpl() noexcept {
#if defined(NATL_COMPILER_EMSCRIPTEN)
			//prefix: "auto natl::impl::getStructMemberNameImpl() [Type = " + struct_name + ", V = &external."
			//info: member_name
			//suffix "]"
			return ConstAsciiStringView(__PRETTY_FUNCTION__)
				.removePrefix(
					cstringLength("auto natl::impl::getStructMemberNameImpl() [Type = ")
					+ getNameOfType<Decay<Type>>().size() + cstringLength(", V = &external."))
				.removeSuffix(cstringLength("]"));
#elif defined(NATL_COMPILER_GCC)
		//prefix: "consteval auto natl::impl::getStructMemberNameImpl() [with Type = " + struct_name + "; auto V = (&natl::external<"
		// + struct_name + ">." + struct_name + "::" 
		//info: member_name
		//suffix "]"
		constexpr Size structName = getNameOfType<Decay<Type>>().size();
		return ConstAsciiStringView(__PRETTY_FUNCTION__)
			.removePrefix(
				cstringLength("consteval auto natl::impl::getStructMemberNameImpl() [with Type = ")
				+ structName + cstringLength("; auto V = (& natl::external<")  
				+ structName + cstringLength(">.") + structName + cstringLength("::"))
			.removeSuffix(cstringLength(")]"));
#elif defined(NATL_COMPILER_MSVC)
			//prefix: "auto __cdecl natl::impl::getStructMemberNameImpl<" + "struct" or "struct" + struct_name + 
			// ",&natl::external<" + "struct" or "struct" + struct_name + ">->"
			//info: member_name 
			//suffix: ">(void) noexcept"
			constexpr Size structInfo = getNameOfType<Decay<Type>>().size();
			return ConstAsciiStringView(__FUNCSIG__)
				.removePrefix(
					cstringLength("auto __cdecl natl::impl::getStructMemberNameImpl<")
					+ structInfo + cstringLength(",&natl::external<")
					+ structInfo + cstringLength(">->"))
				.removeSuffix(cstringLength(">(void) noexcept"));
#else
			static_assert(AlwaysFalse<Type>, "natl: getStructMemberName() not implemented");
#endif
		}
	}

#if defined(NATL_COMPILER_EMSCRIPTEN)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-var-template"
#endif

	template<typename Type, Size Index>
		requires(IsStructC<Decay<Type>>&& StructMemberCount<Type> <= MaxStructMemberNumberEvaluated && Index < StructMemberCount<Type>)
	consteval natl::ConstAsciiStringView getStructMemberName() noexcept {
		return impl::getStructMemberNameImpl<Type, &getStructMember<Index>(external<Type>)>();
	}

#if defined(NATL_COMPILER_EMSCRIPTEN)
#pragma GCC diagnostic pop
#endif

	template<typename Type>
		requires(IsStructC<Decay<Type>>&& StructMemberCount<Type> <= MaxStructMemberNumberEvaluated)
	consteval natl::Array<natl::ConstAsciiStringView, StructMemberCount<Type>> getStructMemberNames() noexcept {
		return[] <natl::Size... Indices> (natl::IndexSequence<Indices...>) {
			return natl::Array<natl::ConstAsciiStringView, StructMemberCount<Type>> {
				getStructMemberName<Type, Indices>()...
			};
		}(MakeIndexSequence<StructMemberCount<Type>>{});
	}	
}