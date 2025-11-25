#pragma once 

//own
#include "typePack.h"
#include "reflection.h"
#include "uninitialized.h"
#include "tuple.h"

//@export
namespace natl {
	template<typename Derived> 
	struct CRTPBase {
		constexpr Derived& base() & noexcept {
			return static_cast<Derived&>(*this);
		}
		constexpr const Derived& base() const& noexcept {
			return static_cast<const Derived&>(*this);
		}
		constexpr Derived&& base() && noexcept {
			return static_cast<Derived&&>(*this);
		}
		constexpr const Derived&& base() const&& noexcept {
			return static_cast<const Derived&&>(*this);
		}
	};

	template<typename Type>
	concept IsTypeMixinCompatibleC = HasStructMemberCountC<Type> && HasGetStructMembersC<Type>;
	template<typename Type> struct IsTypeMixinCompatibleV : BoolConstant<IsTypeMixinCompatibleC<Type>> {};
	template<typename Type> inline constexpr Bool IsTypeMixinCompatible = IsTypeMixinCompatibleC<Type>;

	template<typename Type>
	concept IsMixinC = requires() {
		typename Type::dependicies;
		IsTypePackC<typename Type::dependicies>;
		//{ Type::template isValid<i8> } -> ConvertibleTo<Bool>;
	};
	template<typename Type> struct IsMixinV : BoolConstant<IsMixinC<Type>> {};
	template<typename Type> inline constexpr Bool IsMixin = IsMixinC<Type>;

	template<typename Type> 
	struct AggregateConstructorMixin {
	public:
		using dependicies = TypePack<>;
		template<typename TestType>
		constexpr static inline Bool isValid = IsTypeMixinCompatibleC<TestType>;
		
		template<typename... ConstructArgs>
			requires(sizeof...(ConstructArgs) == StructMemberCount<Type>)
		constexpr static Type construct(ConstructArgs&&... args) noexcept requires(isValid<Type>) {
			if (isConstantEvaluated()) {
				Type value{};
				auto assignment = []<typename LhsType, typename RhsType>(LhsType&& lhs, RhsType&& rhs) -> void {
					lhs = natl::forward<RhsType>(rhs);
				};
				auto members = getStructMembers(value);
				[&] <Size... Indices>(IndexSequence<Indices...>) {
					(assignment(members.template get<Indices>(), forward<ConstructArgs>(args)), ...);
				}(MakeIndexSequence<StructMemberCount<Type>>{});
				return value;
			} else {
				natl::UninitializedValue<Type> value;
				auto members = getStructMembers(value.value());
				[&] <Size... Indices>(IndexSequence<Indices...>) {
					(natl::construct(&(members.template get<Indices>()), forward<ConstructArgs>(args)), ...);
				}(MakeIndexSequence<StructMemberCount<Type>>{});
				return value.value();
			}
		};
		template<typename... ConstructArgs>
			requires(sizeof...(ConstructArgs) == StructMemberCount<Type>)
		constexpr static Type create(ConstructArgs&&... args) noexcept requires(isValid<Type>) {
			return construct<ConstructArgs...>(forward<ConstructArgs>(args)...);
		}
	};

	template<typename Type>
	struct EqualityMixin {
	private:
		template<typename MemberType>
		struct MemberRequirement {
			constexpr static inline Bool value = IsEqualTestable<MemberType>;
		};

	public:
		using dependicies = TypePack<>;
		template<typename TestType>
		constexpr static inline Bool isValid = IsTypeMixinCompatibleC<TestType> && TypePackElementsAreC<MemberRequirement, StructMemberTypes<TestType>>;

		friend constexpr natl::Bool operator==(const Type& lhs, const Type& rhs) noexcept requires(isValid<Type>) {
			auto equalTest = []<typename MemberType>(const MemberType& lhs, const MemberType& rhs) -> Bool {
				return lhs == rhs;
			};
			return [&]<Size... Indices>(IndexSequence<Indices...>) -> Bool {
				auto lhsMembers = getStructMembers(lhs);
				auto rhsMembers = getStructMembers(rhs);
				return (equalTest(lhsMembers.template get<Indices>(), rhsMembers.template get<Indices>()) && ...);
			}(MakeIndexSequence<StructMemberCount<Type>>{});
		}
		friend constexpr natl::Bool operator!=(const Type& lhs, const Type& rhs) noexcept requires(isValid<Type>) {
			auto notEqualTest = []<typename MemberType>(const MemberType& lhs, const MemberType& rhs) -> Bool {
				return lhs != rhs;
			};
			return[&]<Size... Indices>(IndexSequence<Indices...>) -> Bool {
				auto lhsMembers = getStructMembers(lhs);
				auto rhsMembers = getStructMembers(rhs);
				return (notEqualTest(lhsMembers.template get<Indices>(), rhsMembers.template get<Indices>()) || ...);
			}(MakeIndexSequence<StructMemberCount<Type>>{});
		}
	};

	template<typename Type>
	struct OrderingMixin {
	private:
		template<typename MemberType>
		struct MemberRequirement {
			constexpr static inline Bool value = IsLessThanTestable<MemberType> && IsEqualTestable<MemberType>;
		};
		template<typename MemberType>
		struct ThreeWayRequirement {
			constexpr static inline Bool value = IsThreeWayTestable<MemberType>;
		};

	public:
		using dependicies = TypePack<>;
		template<typename TestType>
		constexpr static inline Bool isValid = IsTypeMixinCompatibleC<Type> && TypePackElementsAreC<MemberRequirement, StructMemberTypes<Type>>;

		friend constexpr natl::Bool operator<(const Type& lhs, const Type& rhs) noexcept requires(isValid<Type>) {
			return getStructMembers(lhs) < getStructMembers(rhs);
		}
		friend constexpr natl::Bool operator<=(const Type& lhs, const Type& rhs) noexcept requires(isValid<Type>) {
			return getStructMembers(lhs) <= getStructMembers(rhs);
		}
		friend constexpr natl::Bool operator>(const Type& lhs, const Type& rhs) noexcept requires(isValid<Type>) {
			return getStructMembers(lhs) > getStructMembers(rhs);
		}
		friend constexpr natl::Bool operator>=(const Type& lhs, const Type& rhs) noexcept requires(isValid<Type>) {
			return getStructMembers(lhs) >= getStructMembers(rhs);
		}
		/*
		friend constexpr static auto operator<=>(const Type& lhs, const Type& rhs) noexcept 
			requires(isValid<Type> && TypePackElementsAreC<ThreeWayRequirement, StructMemberTypes<Type>>) {
			auto lhsMembers = getStructMembers(lhs);
			auto rhsMembers = getStructMembers(rhs);
			return lhs <=> rhs;
		}
		*/
	};

	template<typename Type>
	struct ComparableMixin : EqualityMixin<Type>, OrderingMixin<Type> {
	public:
		using dependicies = TypePack<EqualityMixin<Type>, OrderingMixin<Type>>;
		template<typename TestType>
		constexpr static inline Bool isValid = EqualityMixin<Type>::template isValid<TestType> 
			&& OrderingMixin<Type>::template isValid<TestType>;
	};

	template<typename Type> 
	struct HashMixin : CRTPBase<Type> {
		template<typename MemberType>
		struct MemberRequirement {
			constexpr static inline Bool value = IsHashableC<MemberType>;
		};

	public:
		using dependicies = TypePack<>;
		template<typename TestType>
		constexpr static inline Bool isValid = TypePackElementsAreC<MemberRequirement, StructMemberTypes<TestType>>;

		template<typename Hasher>
			requires(IsHasherC<Hasher>)
		constexpr void hashAppend(Hasher& hasher) const noexcept requires(isValid<Type>) {
			return [&]<Size... Indices>(IndexSequence<Indices...>) {
				auto members = getStructMembers(this->base());
				(natl::hashAppend<Hasher>(hasher, members.template get<Indices>()), ...);
			}(MakeIndexSequence<StructMemberCount<Type>>{});
		}

		constexpr natl::Size hash() const noexcept requires(isValid<Type>) {
			return [&]<Size... Indices>(IndexSequence<Indices...>) -> Size {
				StandardHasher hasher{};
				auto members = getStructMembers(this->base());
				(natl::hashAppend<StandardHasher>(hasher, members.template get<Indices>()), ...);
				return hasher.hashCode();
			}(MakeIndexSequence<StructMemberCount<Type>>{});
		}
	};

}