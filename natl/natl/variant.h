#pragma once 

//own
#include "basicTypes.h"
#include "dataMovement.h"
#include "peramaterPackOperations.h"
#include "stringView.h"
#include "stringLiteral.h"
#include "array.h"

//interface 
namespace natl {
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
#endif //_MSC_VER

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

		template<Size Index, class ReturnType>
		constexpr const ReturnType& getRef(int x = 0) const noexcept {
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

		constexpr static Size byteStorageSize = ParameterPackMaxSizeOfTypes<typename Elements::value_type...>::value;
		union {
			alignas(Elements...) Byte byteStorage[byteStorageSize];
			VariantStorage<typename Elements::value_type...> recursiveStorage;
		};
	public:
		//constructor
		constexpr Variant() noexcept : variantIndex(emptyVariantValue), byteStorage{} {}


	private:
		static void uninitializedCopyInternalStorage(Byte* dst, const Byte* src) noexcept {
			const natl::Byte* copySrcFirst = src;
			const natl::Byte* copySrcLast = copySrcFirst + byteStorageSize;
			uninitializedCopyNoOverlap<const natl::Byte*, natl::Byte*>(copySrcFirst, copySrcLast, dst);
		}
		static void copyInternalStorage(Byte* dst, const Byte* src) noexcept {
			const natl::Byte* copySrcFirst = src;
			const natl::Byte* copySrcLast = copySrcFirst + byteStorageSize;
			copyNoOverlap<const natl::Byte*, natl::Byte*>(copySrcFirst, copySrcLast, dst);
		}
		using VariantCopyConstructFunction = void(*)(Variant&, const Variant&);

		template<Size Index>
		constexpr VariantCopyConstructFunction getCopyConstructFunction() noexcept {
			return [](Variant& variant, const Variant& other) {
				using ElementType = typename ParameterPackNthElement<Index, Elements...>::type::value_type;

				if (!std::is_constant_evaluated()) {
					if constexpr (IsTriviallyConstRefConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				std::construct_at<ElementType>(
					addressof<ElementType>(variant.recursiveStorage.template getRef<Index, ElementType>()),
					other.recursiveStorage.template getRef<Index, ElementType>()
				);
			};
		}

		using VariantMoveConstructFunction = void(*)(Variant&, Variant&&);

		template<Size Index>
		constexpr VariantMoveConstructFunction getMoveConstructFunction() noexcept {
			return [](Variant& variant, Variant&& other) {
				using ElementType = typename ParameterPackNthElement<Index, Elements...>::type::value_type;

				if (!std::is_constant_evaluated()) {
					if constexpr (IsTriviallyMoveConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				std::construct_at<ElementType, ElementType>(
					addressof<ElementType>(variant.recursiveStorage.template getRef<Index, ElementType>()), 
					move<ElementType>(other.recursiveStorage.template getRef<Index, ElementType>())
				);
			};
		}

		constexpr void constRefConstruct(const Variant& other) {
			variantIndex = other.variantIndex;

			VariantCopyConstructFunction copyFunctions[numberOfVariants] = { getCopyConstructFunction<ParameterPackFindIndexOfType<Elements, Elements...>::value>()... };
			VariantCopyConstructFunction& copyFunction = copyFunctions[other.getIndex() - 1];

			if (std::is_constant_evaluated()) {
				copyFunction(self(), other);
			}
			else {
				if constexpr (!triviallyConstRefConstructedable) {
					copyFunction(self(), other);
				}
				else {
					uninitializedCopyInternalStorage(byteStorage, other.byteStorage);
				}
			}
		}

		constexpr void moveConstruct(Variant&& other) noexcept {
			variantIndex = other.variantIndex;

			VariantMoveConstructFunction moveFunctions[numberOfVariants] = { getMoveConstructFunction<ParameterPackFindIndexOfType<Elements, Elements...>::value>()... };
			VariantMoveConstructFunction& moveFunction = moveFunctions[other.getIndex() - 1];

			if (std::is_constant_evaluated()) {
				moveFunction(self(), forward<Variant>(other));
			} else {
				if constexpr (!triviallyMoveConstructedable) {
					moveFunction(self(), forward<Variant>(other));
				} else {
					uninitializedCopyInternalStorage(byteStorage, other.byteStorage);
				}
			}
		}

	public:
		constexpr Variant(const Variant& other) noexcept {
			if (!other.hasValue()) {
				variantIndex = 0;
			}
			constRefConstruct(other);
		}

		constexpr Variant(Variant&& other) noexcept {
			if (!other.hasValue()) {
				variantIndex = 0;
			}
			moveConstruct(other);
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
		constexpr VariantDestructFunction getDestructionFunction() noexcept {
			return [](Variant& variant) {
				using ElementType = typename ParameterPackNthElement<Index, Elements...>::type::value_type;

				if (!std::is_constant_evaluated()) {
					if constexpr (IsTriviallyDestructible<ElementType>) {
						return;
					}
				}

				std::destroy_at<ElementType>(natl::addressof<ElementType>(variant.recursiveStorage.template getRef<Index, ElementType>()));
			};
		}
		constexpr void actuallyDestoryValue() noexcept {
			VariantDestructFunction destructFunctions[numberOfVariants] = { getDestructionFunction<ParameterPackFindIndexOfType<Elements, Elements...>::value>()... };
			if (variantIndex != emptyVariantValue) [[likely]] {
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
			return index + 1;
		}

		template<StringLiteral FindName>
		constexpr Size getIndexOf_NotStatic() const noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<FindName>, StringLiteralType<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - getIndex() - could not find varaint element with name");
			return index + 1;
		}

		//assignment
	private:
		using VariantCopyFunction = void(*)(Variant&, const Variant&);

		template<Size Index>
		constexpr VariantCopyFunction getCopyFunction() noexcept {
			return [](Variant& variant, const Variant& other) {
				using ElementType = typename ParameterPackNthElement<Index, Elements...>::type::value_type;

				if (!std::is_constant_evaluated()) {
					if constexpr (IsTriviallyConstRefAssignable<ElementType>) {
						copyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template getRef<Index, ElementType>() = other.recursiveStorage.template getRef<Index, ElementType>();
			};
		}
		using VariantMoveFunction = void(*)(Variant&, Variant&&);

		template<Size Index>
		constexpr VariantMoveFunction getMoveFunction() noexcept {
			return [](Variant& variant, Variant&& other) {
				using ElementType = typename ParameterPackNthElement<Index, Elements...>::type::value_type;

				if (!std::is_constant_evaluated()) {
					if constexpr (IsTriviallyMoveAssignable<ElementType>) {
						copyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template getRef<Index, ElementType>() = move<ElementType>(other.recursiveStorage.template getRef<Index, ElementType>());
			};
		}
	public:

		constexpr Variant& operator=(const Variant& other) noexcept {
			if (other.variantIndex == emptyVariantValue) {
				destoryValue();
				other.variantIndex = 0;
			} else if (other.variantIndex == other.variantIndex) {
				VariantCopyFunction copyFunctions[numberOfVariants] = { getCopyFunction<ParameterPackFindIndexOfType<Elements, Elements...>::value>()... };
				VariantCopyFunction& copyFunction = copyFunctions[other.getIndex() - 1];

				if (std::is_constant_evaluated()) {
					copyFunction(self(), other);
				} else {
					if constexpr (!triviallyConstRefConstructedable) {
						copyFunction(self(), other);
					} else {
						copyInternalStorage(byteStorage, other.byteStorage);
					}
				}
			} else {
				destoryValue();
				constRefConstruct(other);
			}
			return self();
		}


		Variant& operator=(Variant&& other) noexcept {
			if (other.variantIndex == emptyVariantValue) {
				destoryValue();
				other.variantIndex = 0;
			} else if (other.variantIndex == other.variantIndex) {
				VariantMoveFunction moveFunctions[numberOfVariants] = { getMoveFunction<ParameterPackFindIndexOfType<Elements, Elements...>::value>()... };
				VariantMoveFunction& moveFunction = moveFunctions[other.getIndex() - 1];

				if (std::is_constant_evaluated()) {
					moveFunction(self(), forward<Variant>(other));
				} else {
					if constexpr (!triviallyMoveConstructedable) {
						moveFunction(self(), forward<Variant>(other));
					} else {
						copyInternalStorage(byteStorage, other.byteStorage);
					}
				}
			} else {
				destoryValue();
				moveConstruct(other);
			}
			return self();
		}

		template<StringLiteral name, typename DataType>
		constexpr Variant& assign(DataType&& value) noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<name>, StringLiteralType<Elements::name>...>::value;
			if constexpr (index != IndexNotFound::value) {
				using VariantTypeAtIndex = typename ParameterPackNthElement<index, Elements...>::type::value_type;

				if constexpr (std::is_constructible_v<VariantTypeAtIndex, DataType>) {
					if (variantIndex == index) {
						if (std::is_constant_evaluated()) {
							recursiveStorage.template getRef<index, VariantTypeAtIndex>() = forward<DataType>(value);
						} else {
							*reinterpret_cast<DataType*>(byteStorage) = forward<DataType>(value);
						}
					} else {
						destoryValue();
						if (std::is_constant_evaluated()) {
							std::construct_at<VariantTypeAtIndex, VariantTypeAtIndex>(
								&recursiveStorage.template getRef<index, VariantTypeAtIndex>(),
								forward<DataType>(value)
							);
						} else {
							std::construct_at<VariantTypeAtIndex, VariantTypeAtIndex>(
								reinterpret_cast<DataType*>(byteStorage),
								forward<DataType>(value)
							);
						}
					}
					variantIndex = index + 1;
					return self();
				} else {
					static_assert(std::is_constructible_v<VariantTypeAtIndex, DataType>, "natl: variant error - assign() - variant type at index not construtable with provided assign type");
					[[maybe_unused]] DataType assignTypeTemp{};
					[[maybe_unused]] VariantTypeAtIndexNotConstructable variantData = assignTypeTemp;
					return self();
				}
			} else {
				static_assert(index != IndexNotFound::value, "natl: variant error - assign() - could not find varaint element with name");
				[[maybe_unused]] DataType assignTypeTemp{};
				[[maybe_unused]] DidNotFindVariantType variantData = assignTypeTemp;
				return self();
			}
		}
		template<StringLiteral name, typename DataType>
		constexpr Variant& assign(const DataType& value) noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<name>, StringLiteralType<Elements::name>...>::value;
			if constexpr (index != IndexNotFound::value) {
				using VariantTypeAtIndex = typename ParameterPackNthElement<index, Elements...>::type::value_type;

				if constexpr (std::is_constructible_v<VariantTypeAtIndex, DataType>) {
					if (variantIndex == index) {
						if (std::is_constant_evaluated()) {
							recursiveStorage.template getRef<index, VariantTypeAtIndex>() = forward<DataType>(value);
						} else {
							*reinterpret_cast<DataType*>(byteStorage) = forward<DataType>(value);
						}
					} else {
						destoryValue();
						if (std::is_constant_evaluated()) {
							std::construct_at<VariantTypeAtIndex, VariantTypeAtIndex>(
								&recursiveStorage.template getRef<index, VariantTypeAtIndex>(),
								forward<DataType>(value)
							);
						} else {
							std::construct_at<VariantTypeAtIndex, VariantTypeAtIndex>(
								reinterpret_cast<DataType*>(byteStorage),
								forward<DataType>(value)
							);
						}
					}
					variantIndex = index + 1;
					return self();
				} else {
					static_assert(std::is_constructible_v<VariantTypeAtIndex, DataType>, "natl: variant error - assign() - variant type at index not construtable with provided assign type");
					[[maybe_unused]] DataType assignTypeTemp{};
					[[maybe_unused]] VariantTypeAtIndexNotConstructable variantData = assignTypeTemp;
					return self();
				}
			} else {
				static_assert(index != IndexNotFound::value, "natl: variant error - assign() - could not find varaint element with name");
				[[maybe_unused]] DataType assignTypeTemp{};
				[[maybe_unused]] DidNotFindVariantType variantData = assignTypeTemp;
				return self();
			}
			return self();
		}
		private:
		template<StringLiteral name>
		using TypeAtIndexOfName = typename ParameterPackNthElement<getIndexOf<name>() - 1, Elements...>::type::value_type;
		
		template<Size Index>
		constexpr void testValidIndex() const noexcept {
			if (variantIndex != Index + 1) [[unlikely]] {
				std::cout << "variant index violation\n";
				std::abort();
			}
		}

		public:

		template<StringLiteral name>
		constexpr auto& get() noexcept {
			constexpr Size index = ParameterPackStringLiteralFindIndex<StringLiteralType<name>, StringLiteralType<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - get() - could not find varaint element with name");

			if constexpr (index != IndexNotFound::value) {
				testValidIndex<index>();

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
				testValidIndex<index>();

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

		constexpr bool doesnotHaveValue() const noexcept {
			return variantIndex == emptyVariantValue;
		}
		constexpr bool hasValue() const noexcept {
			return variantIndex != emptyVariantValue;
		}
		constexpr Size getIndex() const noexcept {
			return variantIndex;
		}

		template<StringLiteral name>
		constexpr bool isValue() const noexcept {
			return variantIndex == getIndexOf<name>();
		}
	};
}