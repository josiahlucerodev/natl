#pragma once 

//own
#include "basicTypes.h"
#include "dataMovement.h"
#include "peramaterPackOperations.h"
#include "stringView.h"
#include "array.h"

//interface 
namespace natl {
	template <Size Number>
	struct StringLiteral {
		char stringStorage[Number];
		constexpr StringLiteral(const char(&str)[Number]) { uninitializedCopyCountNoOverlap<const char*>(str, stringStorage, Number); }
		constexpr const char* c_str() const noexcept { return stringStorage; }
		constexpr Size size() const noexcept { return Number; }
	};

	template <Size Number>
		requires (Number > 0)
	constexpr StringLiteral<Number> makeStringLiteral(const char(&str)[Number]) noexcept {
		return StringLiteral<Number>(str);
	}

	template <StringLiteral name>
	struct StringLiteralType {
		static constexpr const char name[name.size()] = name.stringStorage;
	};

	struct BaseNamedElement {};
	template<StringLiteral InputName, class DataType>
	struct NamedElement {
		constexpr static StringLiteral name = InputName;
		using NameType = decltype(name);
		using value_type = DataType;
		constexpr operator BaseNamedElement() const noexcept { return BaseNamedElement(); };
	};

	class BaseVariantAssign {};
	template<StringLiteral InputName, class DataType>
	struct VariantAssign {
		constexpr static StringLiteral name = InputName;
		using NameType = decltype(name);
		using value_type = DataType;
		const DataType& data;
		constexpr VariantAssign(const DataType& data) noexcept : data(data) {}
		constexpr operator BaseVariantAssign() const noexcept { return BaseVariantAssign(); };
	};

	class BaseVariantAssignMove {};
	template<StringLiteral InputName, class DataType>
	struct VariantAssignMove {
		constexpr static StringLiteral name = InputName;
		using NameType = decltype(name);
		using value_type = DataType;
		DataType&& data;
		constexpr VariantAssignMove(DataType&& data) noexcept : data(forward<DataType>(data)) {}
		constexpr operator BaseVariantAssignMove() const noexcept { return BaseVariantAssignMove(); };
	};

	template <
		StringLiteral Name1 = "", class DataType1 = i8,
		StringLiteral Name2 = "", class DataType2 = i8,
		StringLiteral Name3 = "", class DataType3 = i8>
	class VariantT {
		union {
			DataType1 value1;

		};
	
	};

	template<class... DataTypes>
	class VariantStorage {
		//this is need for the other function to parse
		template<Size Index, class ReturnType> constexpr ReturnType& getRef() = delete;
	};

#ifdef _MSC_VER
#pragma warning(disable: 4624)
#endif _MSC_VER

	template<class FirstDataType, class... RestDataTypes>
	class VariantStorage<FirstDataType, RestDataTypes...> {
	public:
		constexpr VariantStorage() = default;
		constexpr ~VariantStorage() = default;
	public:
		static constexpr size_t _Size = 1 + sizeof...(RestDataTypes);
		union {
			FirstDataType data;
			VariantStorage<RestDataTypes...> trailingData;
		};

		template<Size Index, class ReturnType> 
		constexpr ReturnType& getRef(int x = 0) noexcept {
			if constexpr (Index == 0) {
				return data;
			} else {
				if constexpr (sizeof...(RestDataTypes) > 0) {
					return trailingData.template getRef<Index - 1, ReturnType>(x);
				}
			}
		}
	};

#ifdef _MSC_VER
#pragma warning(default: 4624)
#endif

	struct DidNotFindVariantType {};
	struct VariantTypeAtIndexNotConstructable {};

	template<typename... Elements>
		requires((std::is_convertible_v<Elements, BaseNamedElement> && ...))
	class Variant {
		constexpr static Size numberOfVariants = sizeof...(Elements);
		constexpr static Size emptyVariantValue = 0;

		//movement info  
		constexpr static bool triviallyRelocatable = (IsTriviallyRelocatable<typename Elements::value_type> && ...);
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = (IsTriviallyDestructible<typename Elements::value_type> && ...);
		constexpr static bool triviallyConstRefConstructedable = (IsTriviallyConstRefConstructible<typename Elements::value_type> && ...) && triviallyDestructible;
		constexpr static bool triviallyMoveConstructedable = (IsTriviallyMoveConstructible<typename Elements::value_type> && ...) && triviallyDestructible;

	private:
		Size variantIndex;
		union {
			alignas(Elements...) Byte byteStorage[ParameterPackMaxSizeOfTypes<typename Elements::value_type...>::value];
			VariantStorage<typename Elements::value_type...> recursiveStorage;
		};
	public:
		constexpr Variant() noexcept : variantIndex(emptyVariantValue) {}
		constexpr Variant(const Variant& other) noexcept {
			//if (other.variantIndex == emptyVariantValue) {
			//
			//}
		}
		constexpr ~Variant() noexcept {
			destoryValue();
		}

		//util 
		constexpr Variant& self() noexcept { return *this; }
		constexpr const Variant& self() const noexcept { return *this; }



	private:
		using VariantDestructFunction = void(*)(Variant&);

		template<Size Index>
		constexpr VariantDestructFunction getDestructionFunction() {
			return [](Variant& variant) {
				using ElementType = typename ParameterPackNthElement<Index, Elements...>::type::value_type;
				std::destroy_at<ElementType>(natl::addressof<ElementType>(variant.recursiveStorage.template getRef<Index, ElementType>()));
			};
		}
		constexpr void actuallyDestoryValue() noexcept {
			VariantDestructFunction destructFunctions[numberOfVariants] = { getDestructionFunction<ParameterPackFindIndexOfType<Elements, Elements...>::value>()... };
			if (variantIndex != emptyVariantValue) {
				VariantDestructFunction& destructFunction = destructFunctions[variantIndex - 1];
				destructFunction(self());
			}
		}
		constexpr void destoryValue() noexcept {
			if (std::is_constant_evaluated()) {
				actuallyDestoryValue();
			} else {
				if (!triviallyDestructible) {
					actuallyDestoryValue();
				}
			}
		}
	public:
		template<StringLiteral FindName> 
		constexpr static Size getIndexOf() noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<FindName>, StringLiteralType<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - getIndex() - could not find varaint element with name");
			return index;
		}

		template<StringLiteral FindName>
		constexpr Size getIndexOf_NotStatic() const noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<FindName>, StringLiteralType<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - getIndex() - could not find varaint element with name");
			return index;
		}


		template<typename AssignInfo>
				requires(std::is_convertible_v<AssignInfo, BaseVariantAssign> || std::is_convertible_v<AssignInfo, BaseVariantAssignMove>)
		Variant& operator= (const AssignInfo& assignInfo) noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<AssignInfo::name>, StringLiteralType<Elements::name>...>::value;
			if constexpr (index != IndexNotFound::value) {
				using VariantTypeAtIndex = typename ParameterPackNthElement<index, Elements...>::type::value_type;
				using AssignInfoDataType = AssignInfo::value_type;

				if constexpr (std::is_constructible_v<VariantTypeAtIndex, AssignInfoDataType>) {
					destoryValue();
					variantIndex = index + 1;

					if constexpr (std::is_convertible_v<AssignInfo, BaseVariantAssign>) {
						if (std::is_constant_evaluated()) {
							std::construct_at<VariantTypeAtIndex>(&recursiveStorage.template getRef<index, VariantTypeAtIndex>(), assignInfo.data);
						} else {
							::new (static_cast<void*>(byteStorage)) VariantTypeAtIndex(assignInfo.data);
						}
					} else {
						if (std::is_constant_evaluated()) {
							std::construct_at<VariantTypeAtIndex>(&recursiveStorage.template getRef<index, VariantTypeAtIndex>(), forward<AssignInfoDataType>(assignInfo.data));
						} else {
							::new (static_cast<void*>(byteStorage)) VariantTypeAtIndex(forward<AssignInfoDataType>(assignInfo.data));
						}
					}


					return self();
				} else {
					static_assert(std::is_constructible_v<VariantTypeAtIndex, AssignInfoDataType>, "natl: variant error - assign() - variant type at index not construtable with provided assign type");
					[[maybe_unused]] typename AssignInfo::DataType assignTypeTemp{};
					[[maybe_unused]] VariantTypeAtIndexNotConstructable variantData = assignTypeTemp;
					return self();
				}
			} else {
				static_assert(index != IndexNotFound::value, "natl: variant error - assign() - could not find varaint element with name");
				[[maybe_unused]] typename AssignInfo::DataType assignTypeTemp{};
				[[maybe_unused]] DidNotFindVariantType variantData = assignTypeTemp;
				return self();
			}
		}

		template<StringLiteral name, typename DataType>
		constexpr Variant assign(DataType&& value) noexcept {
			self() = VariantAssignMove<name, DataType>(forward<DataType>(value));
			return self();
		}
		template<StringLiteral name, typename DataType>
		constexpr Variant assign(const DataType& value) noexcept {
			self() = VariantAssign<name, DataType>(value);
			return self();
		}

		template<StringLiteral name>
		using TypeAtIndexOfName = typename ParameterPackNthElement<getIndexOf<name>, Elements...>::type::value_type;
		
		template<StringLiteral name>
		constexpr auto& get() noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<name>, StringLiteralType<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - get() - could not find varaint element with name");

			if constexpr (index != IndexNotFound::value) {
				using VariantTypeAtIndex = typename ParameterPackNthElement<index, Elements...>::type::value_type;
				if (std::is_constant_evaluated()) {
					return recursiveStorage.template getRef<index, VariantTypeAtIndex>();
				} else {
					return *reinterpret_cast<VariantTypeAtIndex*>(byteStorage);
				}
			}
		}

		template<StringLiteral name>
		constexpr const auto& get() const noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<name>, StringLiteralType<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - get() - could not find varaint element with name");

			if constexpr (index != IndexNotFound::value) {
				using VariantTypeAtIndex = typename ParameterPackNthElement<index, Elements...>::type::value_type;
				if (std::is_constant_evaluated()) {
					return recursiveStorage.template getRef<index, VariantTypeAtIndex>();
				} else {
					return *reinterpret_cast<const VariantTypeAtIndex*>(byteStorage);
				}
			}
		}
		template<StringLiteral name>
		constexpr static Size getIndexFunction() noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<name>, StringLiteralType<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - get() - could not find varaint element with name");
			return index;
		}

		constexpr bool hasValue() const noexcept {
			return variantIndex != emptyVariantValue;
		}
		constexpr Size getIndex() const noexcept {
			return variantIndex;
		}

		template<StringLiteral name>
		constexpr bool isValue() const noexcept {
			return variantIndex - 1 == getIndexOf<name>();
		}
	};
}