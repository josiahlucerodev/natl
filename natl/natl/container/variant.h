#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/dataMovement.h"
#include "../util/typePack.h"
#include "../util/stringLiteral.h"
#include "../util/error.h"
#include "stringView.h"
#include "array.h"
#include "string.h"

//interface 
namespace natl {
	struct BaseNamedElement {};
	template<TemplateStringLiteral InputName, typename DataType>
	struct NamedElement {
		constexpr static TemplateStringLiteral name = InputName;
		using NameType = decltype(name);
		using value_type = DataType;
		constexpr operator BaseNamedElement() const noexcept { return BaseNamedElement(); };
	};
	
	struct BaseVariantAssign {};
	template<TemplateStringLiteral InputName, typename DataType>
	struct VariantAssign {
		constexpr static TemplateStringLiteral name = InputName;
		using NameType = decltype(name);
		using value_type = DataType;
		const DataType& data;
		constexpr VariantAssign(const DataType& dataIn) noexcept : data(dataIn) {}
		constexpr operator BaseVariantAssign() const noexcept { return BaseVariantAssign(); };
	};

	struct BaseVariantAssignMove {};
	template<TemplateStringLiteral InputName, typename DataType>
	struct VariantAssignMove {
		constexpr static TemplateStringLiteral name = InputName;
		using NameType = decltype(name);
		using value_type = DataType;
		DataType&& data;
		constexpr VariantAssignMove(DataType&& dataIn) noexcept : data(forward<DataType>(dataIn)) {}
		constexpr operator BaseVariantAssignMove() const noexcept { return BaseVariantAssignMove(); };
	};

	template<typename... DataTypes>
	struct VariantStorage {
		//this is need for the other function to parse
		template<Size Index, typename ReturnType> constexpr ReturnType& getRef() = delete;
	};

	namespace impl {
		template<typename LhsStringLiteralType, typename RhsStringLiteralType>
		struct StringLiteralCompare {
			constexpr static Bool value = ConstAsciiStringView(LhsStringLiteralType::c_str()) == ConstAsciiStringView(RhsStringLiteralType::c_str());
		};

		template<typename StringLiteralFindType, typename... SearchStringLiteralTypes>
		using FindIndexofStringLiteral =
			TemplatePackFindIndexOfTypeCompare<
			StringLiteralCompare,
			StringLiteralFindType,
			SearchStringLiteralTypes...
		>;
	}

#ifdef _MSC_VER
#pragma warning(disable: 4624)
#endif //_MSC_VER

	template<typename FirstDataType, typename... RestDataTypes>
	struct VariantStorage<FirstDataType, RestDataTypes...> {
	public:
		constexpr VariantStorage() = default;
		constexpr ~VariantStorage() = default;
	public:
		static constexpr size_t _Size = 1 + sizeof...(RestDataTypes);
		union {
			FirstDataType data;
			VariantStorage<RestDataTypes...> trailingData;
		};

		template<Size Index, typename ReturnType> 
		constexpr ReturnType& getRef(int x = 0) noexcept {
			if constexpr (Index == 0) {
				return data;
			} else {
				if constexpr (sizeof...(RestDataTypes) > 0) {
					return trailingData.template getRef<Index - 1, ReturnType>(x);
				}
			}
		}

		template<Size Index, typename ReturnType>
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
		requires((IsConvertibleC<Elements, BaseNamedElement> && ...))
	struct Variant {
	public:
		constexpr static Size numberOfVariants = sizeof...(Elements);
		constexpr static Size emptyVariantValue = 0;
		using named_elements = TypePack<Elements...>;
		using types = TypePack<typename Elements::value_type...>;
		using elements = TypePack<typename Elements::value_type...>;


	private:
		Size variantIndex;

		constexpr static Size byteStorageSize = TemplatePackMaxSizeOfTypes<typename Elements::value_type...>::value;
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
				using ElementType = typename TemplatePackNthElement<Index, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyConstRefConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				natl::construct<ElementType>(
					addressof<ElementType>(variant.recursiveStorage.template getRef<Index, ElementType>()),
					other.recursiveStorage.template getRef<Index, ElementType>()
				);
			};
		}

		using VariantMoveConstructFunction = void(*)(Variant&, Variant&&);

		template<Size Index>
		constexpr VariantMoveConstructFunction getMoveConstructFunction() noexcept {
			return [](Variant& variant, Variant&& other) {
				using ElementType = typename TemplatePackNthElement<Index, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyMoveConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				natl::construct<ElementType, ElementType>(
					addressof<ElementType>(variant.recursiveStorage.template getRef<Index, ElementType>()), 
					move(other.recursiveStorage.template getRef<Index, ElementType>())
				);
			};
		}

		constexpr void constRefConstruct(const Variant& other) {
			variantIndex = other.variantIndex;

			VariantCopyConstructFunction copyFunctions[numberOfVariants] = { getCopyConstructFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
			VariantCopyConstructFunction& copyFunction = copyFunctions[other.getIndex() - 1];

			if (isConstantEvaluated()) {
				copyFunction(self(), other);
			}
			else {
				if constexpr (!IsTriviallyConstRefConstructibleC<Variant<Elements...>>) {
					copyFunction(self(), other);
				} else {
					uninitializedCopyInternalStorage(byteStorage, other.byteStorage);
				}
			}
		}

		constexpr void moveConstruct(Variant&& other) noexcept {
			variantIndex = other.variantIndex;

			VariantMoveConstructFunction moveFunctions[numberOfVariants] = { getMoveConstructFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
			VariantMoveConstructFunction& moveFunction = moveFunctions[other.getIndex() - 1];

			if (isConstantEvaluated()) {
				moveFunction(self(), forward<Variant>(other));
			} else {
				if constexpr (!IsTriviallyMoveConstructibleC<Variant>) {
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
				return;
			}
			constRefConstruct(other);
		}

		constexpr Variant(Variant&& other) noexcept {
			if (!other.hasValue()) {
				variantIndex = 0;
				return;
			}
			moveConstruct(forward<Variant>(other));
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
				using ElementType = typename TemplatePackNthElement<Index, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyDestructible<ElementType>) {
						return;
					}
				}

				natl::deconstruct<ElementType>(natl::addressof<ElementType>(variant.recursiveStorage.template getRef<Index, ElementType>()));
			};
		}
		constexpr void actuallyDestoryValue() noexcept {
			VariantDestructFunction destructFunctions[numberOfVariants] = { getDestructionFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
			if (variantIndex != emptyVariantValue) [[likely]] {
				VariantDestructFunction& destructFunction = destructFunctions[variantIndex - 1];
				destructFunction(self());
			}
		}
		constexpr void destoryValue() noexcept {
			if (isConstantEvaluated() || !IsTriviallyDestructibleC<Variant>) {
				actuallyDestoryValue();
			}
		}

	public:
		constexpr static Size getIndexOfEmpty() noexcept { return 0; }

		template<TemplateStringLiteral FindName> 
		constexpr static Size getIndexOf() noexcept {
			constexpr Size index = impl::FindIndexofStringLiteral<StringLiteral<FindName>, StringLiteral<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - getIndex() - could not find variant element with name");
			return index + 1;
		}

		template<TemplateStringLiteral FindName>
		constexpr Size getIndexOf_NotStatic() const noexcept {
			constexpr Size index = impl::FindIndexofStringLiteral<StringLiteral<FindName>, StringLiteral<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - getIndex() - could not find variant element with name");
			return index + 1;
		}

		//assignment
	private:
		using VariantCopyFunction = void(*)(Variant&, const Variant&);

		template<Size Index>
		constexpr VariantCopyFunction getCopyFunction() noexcept {
			return [](Variant& variant, const Variant& other) {
				using ElementType = typename TemplatePackNthElement<Index, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
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
				using ElementType = typename TemplatePackNthElement<Index, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyMoveAssignable<ElementType>) {
						copyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template getRef<Index, ElementType>() = move(other.recursiveStorage.template getRef<Index, ElementType>());
			};
		}
	public:

		constexpr Variant& operator=(const Variant& other) noexcept {
			if (other.variantIndex == emptyVariantValue) {
				destoryValue();
				variantIndex = 0;
			} else if (variantIndex == other.variantIndex) {
				VariantCopyFunction copyFunctions[numberOfVariants] = { getCopyFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
				VariantCopyFunction& copyFunction = copyFunctions[other.getIndex() - 1];

				if (isConstantEvaluated()) {
					copyFunction(self(), other);
				} else {
					if constexpr (!IsTriviallyConstRefConstructibleC<Variant>) {
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
			} else if (variantIndex == other.variantIndex) {
				VariantMoveFunction moveFunctions[numberOfVariants] = { getMoveFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
				VariantMoveFunction& moveFunction = moveFunctions[other.getIndex() - 1];

				if (isConstantEvaluated()) {
					moveFunction(self(), forward<Variant>(other));
				} else {
					if constexpr (!IsTriviallyMoveConstructibleC<Variant>) {
						moveFunction(self(), forward<Variant>(other));
					} else {
						copyInternalStorage(byteStorage, other.byteStorage);
					}
				}
			} else {
				destoryValue();
				moveConstruct(forward<Variant>(other));
			}
			return self();
		}

		template<Size Index, typename DataType>
		constexpr Variant& assign(DataType&& value) noexcept {
			using variant_type_at_index = TemplatePackNthElement<Index, Elements...>::type::value_type;

			if (variantIndex == Index + 1) {
				if (isConstantEvaluated()) {
					recursiveStorage.template getRef<Index, variant_type_at_index>() = forward<DataType>(value);
				} else {
					*reinterpret_cast<DataType*>(byteStorage) = forward<DataType>(value);
				}
			} else {
				destoryValue();
				if (isConstantEvaluated()) {
					natl::construct<variant_type_at_index, variant_type_at_index>(
						&recursiveStorage.template getRef<Index, variant_type_at_index>(),
						forward<DataType>(value)
					);
				} else {
					natl::construct<variant_type_at_index, variant_type_at_index>(
						reinterpret_cast<DataType*>(byteStorage),
						forward<DataType>(value)
					);
				}
			}
			variantIndex = Index + 1;
			return self();
		}

		template<Size Index, typename DataType>
		constexpr Variant& assign(const DataType& value) noexcept {
			using variant_type_at_index = TemplatePackNthElement<Index, Elements...>::type::value_type;

			if (variantIndex == Index + 1) {
				if (isConstantEvaluated()) {
					recursiveStorage.template getRef<Index, variant_type_at_index>() = forward<DataType>(value);
				} else {
					*reinterpret_cast<DataType*>(byteStorage) = forward<DataType>(value);
				}
			} else {
				destoryValue();
				if (isConstantEvaluated()) {
					natl::construct<variant_type_at_index, variant_type_at_index>(
						&recursiveStorage.template getRef<Index, variant_type_at_index>(),
						forward<DataType>(value)
					);
				} else {
					natl::construct<variant_type_at_index, variant_type_at_index>(
						reinterpret_cast<DataType*>(byteStorage),
						forward<DataType>(value)
					);
				}
			}
			variantIndex = Index + 1;
			return self();
		}

		template<TemplateStringLiteral name, typename DataType>
		constexpr Variant& assign(DataType&& value) noexcept {
			constexpr Size index = impl::FindIndexofStringLiteral<StringLiteral<name>, StringLiteral<Elements::name>...>::value;
			using decayed_data_type = Decay<DataType>;
			if constexpr (index != IndexNotFound::value) {
				using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;

				if constexpr (IsConstructibleC<VariantTypeAtIndex, DataType>) {
					if (variantIndex == index + 1) {
						if (isConstantEvaluated()) {
							recursiveStorage.template getRef<index, VariantTypeAtIndex>() = forward<DataType>(value);
						} else {
							*reinterpret_cast<decayed_data_type*>(byteStorage) = forward<DataType>(value);
						}
					} else {
						destoryValue();
						if (isConstantEvaluated()) {
							natl::construct<VariantTypeAtIndex, VariantTypeAtIndex>(
								&recursiveStorage.template getRef<index, VariantTypeAtIndex>(),
								forward<decayed_data_type>(value)
							);
						} else {
							natl::construct<VariantTypeAtIndex, VariantTypeAtIndex>(
								reinterpret_cast<decayed_data_type*>(byteStorage),
								forward<decayed_data_type>(value)
							);
						}
					}
					variantIndex = index + 1;
					return self();
				} else {
					static_assert(IsConstructibleC<VariantTypeAtIndex, DataType>, "natl: variant error - assign() - variant type at index not construtable with provided assign type");
					[[maybe_unused]] DataType assignTypeTemp{};
					[[maybe_unused]] VariantTypeAtIndexNotConstructable variantData = assignTypeTemp;
					return self();
				}
			} else {
				static_assert(index != IndexNotFound::value, "natl: variant error - assign() - could not find variant element with name");
				[[maybe_unused]] DataType assignTypeTemp{};
				[[maybe_unused]] DidNotFindVariantType variantData = assignTypeTemp;
				return self();
			}
		}
		template<TemplateStringLiteral name, typename DataType>
		constexpr Variant& assign(const DataType& value) noexcept {
			constexpr Size index = impl::FindIndexofStringLiteral<StringLiteral<name>, StringLiteral<Elements::name>...>::value;
			if constexpr (index != IndexNotFound::value) {
				using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;

				if constexpr (IsConstructibleC<VariantTypeAtIndex, DataType>) {
					if (variantIndex == index + 1) {
						if (isConstantEvaluated()) {
							recursiveStorage.template getRef<index, VariantTypeAtIndex>() = value;
						} else {
							*reinterpret_cast<DataType*>(byteStorage) = value;
						}
					} else {
						destoryValue();
						if (isConstantEvaluated()) {
							natl::construct<VariantTypeAtIndex, const VariantTypeAtIndex&>(
								&recursiveStorage.template getRef<index, VariantTypeAtIndex>(),
								natl::forward<const DataType&>(value)
							);
						} else {
							natl::construct<VariantTypeAtIndex, const VariantTypeAtIndex&>(
								reinterpret_cast<DataType*>(byteStorage),
								forward<const DataType&>(value)
							);
						}
					}
					variantIndex = index + 1;
					return self();
				} else {
					static_assert(IsConstructibleC<VariantTypeAtIndex, DataType>, "natl: variant error - assign() - variant type at index not construtable with provided assign type");
					[[maybe_unused]] DataType assignTypeTemp{};
					[[maybe_unused]] VariantTypeAtIndexNotConstructable variantData = assignTypeTemp;
					return self();
				}
			} else {
				static_assert(index != IndexNotFound::value, "natl: variant error - assign() - could not find variant element with name");
				[[maybe_unused]] DataType assignTypeTemp{};
				[[maybe_unused]] DidNotFindVariantType variantData = assignTypeTemp;
				return self();
			}
			return self();
		}
		private:
		template<TemplateStringLiteral name>
		using TypeAtIndexOfName = typename TemplatePackNthElement<getIndexOf<name>() - 1, Elements...>::type::value_type;
		
		template<Size Index>
		constexpr void testValidIndex() const noexcept {
			if (variantIndex != Index + 1) [[unlikely]] {
				natl::fatalError("variant index violation");
			}
		}

		public:
		template<Size Index>
		constexpr auto& get() noexcept {
			testValidIndex<Index>();
			using variant_type_at_index = TemplatePackNthElement<Index, Elements...>::type::value_type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<Index, variant_type_at_index>();
			} else {
				return *reinterpret_cast<variant_type_at_index*>(byteStorage);
			}
		}

		public:
		template<Size Index>
		constexpr const auto& get() const noexcept {
			testValidIndex<Index>();
			using variant_type_at_index = TemplatePackNthElement<Index, Elements...>::type::value_type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<Index, variant_type_at_index>();
			} else {
				return *reinterpret_cast<const variant_type_at_index*>(byteStorage);
			}
		}

		template<TemplateStringLiteral name>
		constexpr auto& get() noexcept {
			constexpr Size index = impl::FindIndexofStringLiteral<StringLiteral<name>, StringLiteral<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - get() - could not find variant element with name");

			if constexpr (index != IndexNotFound::value) {
				testValidIndex<index>();

				using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;
				if (isConstantEvaluated()) {
					return recursiveStorage.template getRef<index, VariantTypeAtIndex>();
				} else {
					return *reinterpret_cast<VariantTypeAtIndex*>(byteStorage);
				}
			}
		}

		template<TemplateStringLiteral name>
		constexpr const auto& get() const noexcept {
			constexpr Size index = impl::FindIndexofStringLiteral<StringLiteral<name>, StringLiteral<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - get() - could not find variant element with name");

			if constexpr (index != IndexNotFound::value) {
				testValidIndex<index>();

				using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;
				if (isConstantEvaluated()) {
					return recursiveStorage.template getRef<index, VariantTypeAtIndex>();
				} else {
					return *reinterpret_cast<const VariantTypeAtIndex*>(byteStorage);
				}
			}
		}
		template<TemplateStringLiteral name>
		constexpr static Size getIndexFunction() noexcept {
			constexpr Size index = impl::FindIndexofStringLiteral<StringLiteral<name>, StringLiteral<Elements::name>...>::value;
			static_assert(index != IndexNotFound::value, "natl: variant error - get() - could not find variant element with name");
			return index;
		}

		constexpr Bool doesNotHaveValue() const noexcept {
			return variantIndex == emptyVariantValue;
		}
		constexpr Bool hasValue() const noexcept {
			return variantIndex != emptyVariantValue;
		}
		constexpr Size getIndex() const noexcept {
			return variantIndex;
		}

		template<TemplateStringLiteral name>
		constexpr Bool isValue() const noexcept {
			return variantIndex == getIndexOf<name>();
		}

		//special 
	private:
		using test_element_str_function = Bool(*)(const ConstAsciiStringView&);
		template<Size Index, typename Element>
		constexpr static test_element_str_function getTestElementStrFunction() noexcept {
			return [](const ConstAsciiStringView& str) -> Bool {
				return str == Element::name;
			};
		}

		public:
		constexpr static Option<Size> stringToIndexStatic(const ConstAsciiStringView& str) noexcept {
			return[&] <Size... Indices>(IndexSequence<Indices...>) -> Option<Size> {
				test_element_str_function testFunctions[sizeof...(Elements)] = {
					getTestElementStrFunction<Indices, Elements>()...
				};

				for (Size i = 0; i < sizeof...(Elements); i++) {
					if (testFunctions[i](str)) {
						return i + 1;
					}
				}

				return {};
			}(MakeIndexSequence<sizeof...(Elements)>{});
		}

		constexpr Option<Size> stringToIndex(const ConstAsciiStringView& str) noexcept {
			return stringToIndexStatic(str);
		}

		constexpr static Option<Size> stringToIndexNotShiftedStatic(const ConstAsciiStringView& str) noexcept {
			return[&] <Size... Indices>(IndexSequence<Indices...>) -> Option<Size> {
				test_element_str_function testFunctions[sizeof...(Elements)] = {
					getTestElementStrFunction<Indices, Elements>()...
				};

				for (Size i = 0; i < sizeof...(Elements); i++) {
					if (testFunctions[i](str)) {
						return i;
					}
				}

				return {};
			}(MakeIndexSequence<sizeof...(Elements)>{});
		}

		constexpr Option<Size> stringToIndexNotShifted(const ConstAsciiStringView& str) noexcept {
			return stringToIndexNotShiftedStatic(str);
		}
	};

	template<typename... Elements>
	struct IsTriviallyRelocatableV<Variant<Elements...>>
		: BoolConstant<(IsTriviallyRelocatableC<typename Elements::value_type> && ...)> {};
	template<typename... Elements>
	struct IsTriviallyConstructibleV<Variant<Elements...>>
		: TrueType {};
	template<typename... Elements>
	struct IsTriviallyDestructibleV<Variant<Elements...>>
		: BoolConstant<(IsTriviallyDestructibleC<typename Elements::value_type> && ...)> {};

	template<typename... Elements>
	struct IsTriviallyConstRefConstructibleV<Variant<Elements...>>
		: BoolConstant<(IsTriviallyConstRefConstructibleC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<Variant<Elements...>>> {};
	template<typename... Elements>
	struct IsTriviallyMoveConstructibleV<Variant<Elements...>>
		: BoolConstant<(IsTriviallyMoveConstructibleC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<Variant<Elements...>>> {};

	template<typename... Elements>
	struct IsTriviallyConstRefAssignableV<Variant<Elements...>>
		: BoolConstant<(IsTriviallyConstRefAssignableC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<Variant<Elements...>>> {};
	template<typename... Elements>
	struct IsTriviallyMoveAssignableV<Variant<Elements...>>
		: BoolConstant<(IsTriviallyMoveAssignableC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<Variant<Elements...>>> {};
}