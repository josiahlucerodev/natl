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

//@export
namespace natl {
	template<typename... DataTypes>
	struct VariantStorage {
		//this is need for the other function to parse
		template<Size vIndex, typename ReturnType> constexpr ReturnType& getRef() = delete;
	};

#ifdef _MSC_VER
#pragma warning(disable: 4624)
#endif //_MSC_VER

	template<typename FirstDataType, typename... RestDataTypes>
	struct VariantStorage<FirstDataType, RestDataTypes...> {
	public:
		//constexpr VariantStorage() {};
		constexpr ~VariantStorage() = default;

	public:
		static constexpr size_t _Size = 1 + sizeof...(RestDataTypes);
		union {
			FirstDataType data;
			VariantStorage<RestDataTypes...> trailingData;
		};

		template<Size vIndex, typename... Args>
			requires(vIndex < 1 + sizeof...(RestDataTypes))
		constexpr void construct(Args&&... args) noexcept {
			if constexpr (vIndex == 0) {
				natl::construct<FirstDataType>(&data, natl::forward<Args>(args)...);
			} else {
				natl::construct<VariantStorage<RestDataTypes...>>(&trailingData);
				trailingData.template construct<vIndex - 1>(natl::forward<Args>(args)...);
			}
		}

		template<Size vIndex>
			requires(vIndex < 1 + sizeof...(RestDataTypes))
		constexpr void deconstruct() noexcept {
			if constexpr (vIndex == 0) {
				natl::deconstruct(&data);
			} else {
				trailingData.template deconstruct<vIndex - 1>();
				natl::deconstruct<VariantStorage<RestDataTypes...>>(&trailingData);
			}
		}

		template<Size vIndex, typename ReturnType>
		constexpr ReturnType& getRef() noexcept {
			if constexpr (vIndex == 0) {
				return data;
			} else {
				if constexpr (sizeof...(RestDataTypes) > 0) {
					return trailingData.template getRef<vIndex - 1, ReturnType>();
				}
			}
		}

		template<Size vIndex, typename ReturnType>
		constexpr const ReturnType& getRef() const noexcept {
			if constexpr (vIndex == 0) {
				return data;
			} else {
				if constexpr (sizeof...(RestDataTypes) > 0) {
					return trailingData.template getRef<vIndex - 1, ReturnType>();
				}
			}
		}
	};

#ifdef _MSC_VER
#pragma warning(default: 4624)
#endif

	namespace impl {
		template<typename LhsStringLiteralType, typename RhsStringLiteralType>
		struct StringLiteralCompare {
			constexpr static Bool value = ConstAsciiStringView(LhsStringLiteralType::c_str()) == ConstAsciiStringView(RhsStringLiteralType::c_str());
		};

		template<typename StringLiteralFindType, typename... SearchStringLiteralTypes>
		using FindIndexOfStringLiteral =
			TemplatePackFindIndexOfTypeCompare<
			StringLiteralCompare,
			StringLiteralFindType,
			SearchStringLiteralTypes...
			>;
	}

	// --- Variant --- //
	template<typename... DataTypes>
	struct Variant {
	public:
		constexpr static Size numberOfVariants = sizeof...(DataTypes);
		constexpr static Size emptyVariantValue = 0;
		using types = TypePack<DataTypes...>;
		using elements = types;

		template <natl::Size vIndex>
		constexpr static natl::Bool isValidIndex = vIndex < numberOfVariants;
		template <typename Type>
		constexpr static natl::Bool isValidType = TemplatePackFindIndexOfType<Type, DataTypes...>::value != IndexNotFound::value;

	private:
		Size variantIndex;

		constexpr static Size byteStorageSize = TemplatePackMaxSizeOfTypes<DataTypes...>::value;
		union {
			alignas(DataTypes...) Byte byteStorage[byteStorageSize];
			VariantStorage<DataTypes...> recursiveStorage;
		};

	public:
		//constructor
		constexpr Variant() noexcept : variantIndex(emptyVariantValue) {
			if (isConstantEvaluated()) {
				natl::construct(&recursiveStorage);
			}
		}

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

		template<Size vIndex>
		constexpr VariantCopyConstructFunction getCopyConstructFunction() noexcept {
			return [](Variant& variant, const Variant& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, DataTypes...>::type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyConstRefConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template construct<vIndex>(
					other.recursiveStorage.template getRef<vIndex, ElementType>()
				);
			};
		}

		using VariantMoveConstructFunction = void(*)(Variant&, Variant&&);

		template<Size vIndex>
		constexpr VariantMoveConstructFunction getMoveConstructFunction() noexcept {
			return [](Variant& variant, Variant&& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, DataTypes...>::type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyMoveConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template construct<vIndex>(
					move(other.recursiveStorage.template getRef<vIndex, ElementType>())
				);
			};
		}

		constexpr void constRefConstruct(const Variant& other) {
			variantIndex = other.variantIndex;

			VariantCopyConstructFunction copyFunctions[numberOfVariants] = { getCopyConstructFunction<TemplatePackFindIndexOfType<DataTypes, DataTypes...>::value>()... };
			VariantCopyConstructFunction& copyFunction = copyFunctions[other.getIndex()];

			if (isConstantEvaluated()) {
				copyFunction(self(), other);
			} else {
				if constexpr (!IsTriviallyConstRefConstructibleC<Variant<DataTypes...>>) {
					copyFunction(self(), other);
				} else {
					uninitializedCopyInternalStorage(byteStorage, other.byteStorage);
				}
			}
		}

		constexpr void moveConstruct(Variant&& other) noexcept {
			variantIndex = other.variantIndex;

			VariantMoveConstructFunction moveFunctions[numberOfVariants] = { getMoveConstructFunction<TemplatePackFindIndexOfType<DataTypes, DataTypes...>::value>()... };
			VariantMoveConstructFunction& moveFunction = moveFunctions[other.getIndex()];

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

		constexpr void defaultConstruct() noexcept {
			variantIndex = emptyVariantValue;
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

		template<typename DataType>
			requires(isValidType<DataType>)
		constexpr Variant(const DataType& value) noexcept {
			defaultConstruct();
			assign<getIndexOf<DataType>()>(value);
		}

		template<typename DataType>
			requires(isValidType<DataType>)
		constexpr Variant(DataType&& value) noexcept {
			defaultConstruct();
			assign<getIndexOf<DataType>()>(forward<DataType>(value));
		}

		constexpr ~Variant() noexcept {
			destoryValue();
		}

		//util
		constexpr Variant& self() noexcept { return *this; }
		constexpr const Variant& self() const noexcept { return *this; }

	private:
		using VariantDestructFunction = void(*)(Variant&);

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr VariantDestructFunction getDestructionFunction() noexcept {
			return [](Variant& variant) {
				using ElementType = typename TemplatePackNthElement<vIndex, DataTypes...>::type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyDestructible<ElementType>) {
						return;
					}
				}

				variant.recursiveStorage.template deconstruct<vIndex>();
			};
		}
		constexpr void actuallyDestoryValue() noexcept {
			VariantDestructFunction destructFunctions[numberOfVariants] = { getDestructionFunction<TemplatePackFindIndexOfType<DataTypes, DataTypes...>::value>()... };
			if (variantIndex != emptyVariantValue) [[likely]] {
				VariantDestructFunction& destructFunction = destructFunctions[getIndex()];
				destructFunction(self());
			}
		}
		constexpr void destoryValue() noexcept {
			if (isConstantEvaluated() || !IsTriviallyDestructibleC<Variant>) {
				actuallyDestoryValue();
			}
		}

		//assignment
	private:
		using VariantCopyFunction = void(*)(Variant&, const Variant&);

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr VariantCopyFunction getCopyFunction() noexcept {
			return [](Variant& variant, const Variant& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, DataTypes...>::type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyConstRefAssignable<ElementType>) {
						copyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template getRef<vIndex, ElementType>() = other.recursiveStorage.template getRef<vIndex, ElementType>();
			};
		}
		using VariantMoveFunction = void(*)(Variant&, Variant&&);

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr VariantMoveFunction getMoveFunction() noexcept {
			return [](Variant& variant, Variant&& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, DataTypes...>::type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyMoveAssignable<ElementType>) {
						copyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template getRef<vIndex, ElementType>() = move(other.recursiveStorage.template getRef<vIndex, ElementType>());
			};
		}

	public:
		constexpr Variant& operator=(const Variant& other) noexcept {
			return assign(other);
		}


		constexpr Variant& operator=(Variant&& other) noexcept {
			return assign(forward<Variant>(other));
		}

		template<typename DataType>
			requires(isValidType<DataType>)
		constexpr Variant& operator=(DataType&& value) noexcept {
			return assign<getIndexOf<DataType>()>(forward<DataType>(value));
		}

		template<typename DataType>
			requires(isValidType<DataType>)
		constexpr Variant& operator=(const DataType& value) noexcept {
			return assign<getIndexOf<DataType>()>(value);
		}

		constexpr Variant& assign(const Variant& other) noexcept {
			if (other.variantIndex == emptyVariantValue) {
				destoryValue();
				variantIndex = 0;
			} else if (variantIndex == other.variantIndex) {
				VariantCopyFunction copyFunctions[numberOfVariants] = { getCopyFunction<TemplatePackFindIndexOfType<DataTypes, DataTypes...>::value>()... };
				VariantCopyFunction& copyFunction = copyFunctions[other.getIndex()];

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


		constexpr Variant& assign(Variant&& other) noexcept {
			if (other.variantIndex == emptyVariantValue) {
				destoryValue();
				variantIndex = emptyVariantValue;
			} else if (variantIndex == other.variantIndex) {
				VariantMoveFunction moveFunctions[numberOfVariants] = { getMoveFunction<TemplatePackFindIndexOfType<DataTypes, DataTypes...>::value>()... };
				VariantMoveFunction& moveFunction = moveFunctions[other.getIndex()];

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

		template<Size vIndex, typename DataType>
			requires(isValidIndex<vIndex>)
		constexpr Variant& assign(const DataType& value) noexcept {
			using variant_type_at_index = TemplatePackNthElement<vIndex, DataTypes...>::type;

			if (variantIndex == vIndex + 1) {
				if (isConstantEvaluated()) {
					recursiveStorage.template getRef<vIndex, variant_type_at_index>() = value;
				} else {
					*reinterpret_cast<variant_type_at_index*>(byteStorage) = value;
				}
			} else {
				destoryValue();
				if (isConstantEvaluated()) {
					recursiveStorage.template construct<vIndex>(value);
				} else {
					natl::construct<variant_type_at_index>(
						reinterpret_cast<variant_type_at_index*>(byteStorage),
						value
					);
				}
			}
			variantIndex = vIndex + 1;
			return self();
		}

		template<Size vIndex, typename DataType>
			requires(isValidIndex<vIndex>)
		constexpr Variant& assign(DataType&& value) noexcept {
			using variant_type_at_index = TemplatePackNthElement<vIndex, DataTypes...>::type;

			if (variantIndex == vIndex + 1) {
				if (isConstantEvaluated()) {
					recursiveStorage.template getRef<vIndex, variant_type_at_index>() = forward<DataType>(value);
				} else {
					*reinterpret_cast<variant_type_at_index*>(byteStorage) = forward<DataType>(value);
				}
			} else {
				destoryValue();
				if (isConstantEvaluated()) {
					recursiveStorage.template construct<vIndex>(forward<DataType>(value));
				} else {
					natl::construct<variant_type_at_index>(
						reinterpret_cast<variant_type_at_index*>(byteStorage),
						forward<DataType>(value)
					);
				}
			}
			variantIndex = vIndex + 1;
			return self();
		}

		template<typename DataType>
			requires(isValidType<DataType>)
		constexpr Variant& assign(const DataType& value) noexcept {
			return assign<getIndexOf<DataType>()>(value);
		}

		template<typename DataType>
			requires(isValidType<DataType>)
		constexpr Variant& assign(DataType&& value) noexcept {
			return assign<getIndexOf<DataType>()>(forward<DataType>(value));
		}

		constexpr void reset() noexcept {
			destoryValue();
			variantIndex = emptyVariantValue;
		}

		template<typename Type>
			requires(isValidType<Type>)
		constexpr static Size getIndexOf() noexcept {
			return TemplatePackFindIndexOfType<Type, DataTypes...>::value;
		}

	private:
		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr void testValidIndex() const noexcept {
			if (variantIndex != vIndex + 1) [[unlikely]] {
				natl::fatalError("variant index violation");
			}
		}

	public:
		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr auto& get() noexcept {
			testValidIndex<vIndex>();
			using variant_type_at_index = TemplatePackNthElement<vIndex, DataTypes...>::type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<vIndex, variant_type_at_index>();
			} else {
				return *reinterpret_cast<variant_type_at_index*>(byteStorage);
			}
		}

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr const auto& get() const noexcept {
			testValidIndex<vIndex>();
			using variant_type_at_index = TemplatePackNthElement<vIndex, DataTypes...>::type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<vIndex, variant_type_at_index>();
			} else {
				return *reinterpret_cast<const variant_type_at_index*>(byteStorage);
			}
		}

		constexpr Size getIndex() const noexcept {
			return variantIndex - 1;
		}

		constexpr Bool doesNotHaveValue() const noexcept {
			return variantIndex == emptyVariantValue;
		}
		constexpr Bool hasValue() const noexcept {
			return variantIndex != emptyVariantValue;
		}

		constexpr static Size getIndexOfEmpty() noexcept {
			return Size(0) - 1;
		}

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr natl::Bool isIndex() const noexcept {
			return variantIndex == vIndex + 1;
		}

		template<typename Type>
			requires(isValidType<Type>)
		constexpr natl::Bool isValue() const noexcept {
			constexpr Size index = TemplatePackFindIndexOfType<Type, DataTypes...>::value;
			return variantIndex == index + 1;
		}
	};

	template<typename... DataTypes>
	struct IsTriviallyRelocatableV<Variant<DataTypes...>>
		: BoolConstant<(IsTriviallyRelocatableC<DataTypes> && ...)> {
	};
	template<typename... DataTypes>
	struct IsTriviallyConstructibleV<Variant<DataTypes...>>
		: TrueType {
	};
	template<typename... DataTypes>
	struct IsTriviallyDestructibleV<Variant<DataTypes...>>
		: BoolConstant<(IsTriviallyDestructibleC<DataTypes> && ...)> {
	};

	template<typename... DataTypes>
	struct IsTriviallyConstRefConstructibleV<Variant<DataTypes...>>
		: BoolConstant<(IsTriviallyConstRefConstructibleC<DataTypes> && ...)
		&& IsTriviallyDestructibleC<Variant<DataTypes...>>> {
	};
	template<typename... DataTypes>
	struct IsTriviallyMoveConstructibleV<Variant<DataTypes...>>
		: BoolConstant<(IsTriviallyMoveConstructibleC<DataTypes> && ...)
		&& IsTriviallyDestructibleC<Variant<DataTypes...>>> {
	};

	template<typename... DataTypes>
	struct IsTriviallyConstRefAssignableV<Variant<DataTypes...>>
		: BoolConstant<(IsTriviallyConstRefAssignableC<DataTypes> && ...)
		&& IsTriviallyDestructibleC<Variant<DataTypes...>>> {
	};
	template<typename... DataTypes>
	struct IsTriviallyMoveAssignableV<Variant<DataTypes...>>
		: BoolConstant<(IsTriviallyMoveAssignableC<DataTypes> && ...)
		&& IsTriviallyDestructibleC<Variant<DataTypes...>>> {
	};

	// --- Named Variant --- //

	struct BaseNamedElement {};
	template<TemplateStringLiteral InputName, typename DataType>
	struct NamedElement {
		constexpr static TemplateStringLiteral vName = InputName;
		using NameType = decltype(vName);
		using value_type = DataType;
		constexpr operator BaseNamedElement() const noexcept { return BaseNamedElement(); };
	};

	struct BaseVariantAssign {};
	template<TemplateStringLiteral InputName, typename DataType>
	struct VariantAssign {
		constexpr static TemplateStringLiteral vName = InputName;
		using NameType = decltype(vName);
		using value_type = DataType;
		const DataType& data;
		constexpr VariantAssign(const DataType& dataIn) noexcept : data(dataIn) {}
		constexpr operator BaseVariantAssign() const noexcept { return BaseVariantAssign(); };
	};

	struct BaseVariantAssignMove {};
	template<TemplateStringLiteral InputName, typename DataType>
	struct VariantAssignMove {
		constexpr static TemplateStringLiteral vName = InputName;
		using NameType = decltype(vName);
		using value_type = DataType;
		DataType&& data;
		constexpr VariantAssignMove(DataType&& dataIn) noexcept : data(forward<DataType>(dataIn)) {}
		constexpr operator BaseVariantAssignMove() const noexcept { return BaseVariantAssignMove(); };
	};

	struct DidNotFindVariantType {};
	struct VariantTypeAtIndexNotConstructable {};

	template<typename... Elements>
		requires((IsConvertibleC<Elements, BaseNamedElement> && ...))
	struct NamedVariant {
	public:
		constexpr static Size numberOfVariants = sizeof...(Elements);
		constexpr static Size emptyVariantValue = 0;
		using named_elements = TypePack<Elements...>;
		using types = TypePack<typename Elements::value_type...>;
		using elements = TypePack<typename Elements::value_type...>;

		template <natl::Size vIndex>
		constexpr static natl::Bool isValidIndex = vIndex < numberOfVariants;
		template<TemplateStringLiteral vName>
		constexpr static natl::Bool isValidName = impl::FindIndexOfStringLiteral<StringLiteral<vName>, StringLiteral<Elements::vName>...>::value != IndexNotFound::value;

	private:
		Size variantIndex;

		constexpr static Size byteStorageSize = TemplatePackMaxSizeOfTypes<typename Elements::value_type...>::value;
		union {
			alignas(Elements...) Byte byteStorage[byteStorageSize];
			VariantStorage<typename Elements::value_type...> recursiveStorage;
		};
	public:
		//constructor
		constexpr NamedVariant() noexcept : variantIndex(emptyVariantValue) {
			if (isConstantEvaluated()) {
				natl::construct(&recursiveStorage);
			}
		}


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
		using VariantCopyConstructFunction = void(*)(NamedVariant&, const NamedVariant&);

		template<Size vIndex>
		constexpr VariantCopyConstructFunction getCopyConstructFunction() noexcept {
			return [](NamedVariant& variant, const NamedVariant& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyConstRefConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template construct<vIndex>(
					other.recursiveStorage.template getRef<vIndex, ElementType>()
				);
			};
		}

		using VariantMoveConstructFunction = void(*)(NamedVariant&, NamedVariant&&);

		template<Size vIndex>
		constexpr VariantMoveConstructFunction getMoveConstructFunction() noexcept {
			return [](NamedVariant& variant, NamedVariant&& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyMoveConstructible<ElementType>) {
						uninitializedCopyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template construct<vIndex>(
					move(other.recursiveStorage.template getRef<vIndex, ElementType>())
				);
			};
		}

		constexpr void constRefConstruct(const NamedVariant& other) {
			variantIndex = other.variantIndex;

			VariantCopyConstructFunction copyFunctions[numberOfVariants] = { getCopyConstructFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
			VariantCopyConstructFunction& copyFunction = copyFunctions[other.getIndex()];

			if (isConstantEvaluated()) {
				copyFunction(self(), other);
			} else {
				if constexpr (!IsTriviallyConstRefConstructibleC<NamedVariant<Elements...>>) {
					copyFunction(self(), other);
				} else {
					uninitializedCopyInternalStorage(byteStorage, other.byteStorage);
				}
			}
		}

		constexpr void moveConstruct(NamedVariant&& other) noexcept {
			variantIndex = other.variantIndex;

			VariantMoveConstructFunction moveFunctions[numberOfVariants] = { getMoveConstructFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
			VariantMoveConstructFunction& moveFunction = moveFunctions[other.getIndex()];

			if (isConstantEvaluated()) {
				moveFunction(self(), forward<NamedVariant>(other));
			} else {
				if constexpr (!IsTriviallyMoveConstructibleC<NamedVariant>) {
					moveFunction(self(), forward<NamedVariant>(other));
				} else {
					uninitializedCopyInternalStorage(byteStorage, other.byteStorage);
				}
			}
		}

	public:
		constexpr NamedVariant(const NamedVariant& other) noexcept {
			if (!other.hasValue()) {
				variantIndex = 0;
				return;
			}
			constRefConstruct(other);
		}

		constexpr NamedVariant(NamedVariant&& other) noexcept {
			if (!other.hasValue()) {
				variantIndex = 0;
				return;
			}
			moveConstruct(forward<NamedVariant>(other));
		}

		constexpr ~NamedVariant() noexcept {
			destoryValue();
		}

		//util
		constexpr NamedVariant& self() noexcept { return *this; }
		constexpr const NamedVariant& self() const noexcept { return *this; }



	private:
		using VariantDestructFunction = void(*)(NamedVariant&);

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr VariantDestructFunction getDestructionFunction() noexcept {
			return [](NamedVariant& variant) {
				using ElementType = typename TemplatePackNthElement<vIndex, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyDestructible<ElementType>) {
						return;
					}
				}

				variant.recursiveStorage.template deconstruct<vIndex>();
			};
		}
		constexpr void actuallyDestoryValue() noexcept {
			VariantDestructFunction destructFunctions[numberOfVariants] = { getDestructionFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
			if (variantIndex != emptyVariantValue) [[likely]] {
				VariantDestructFunction& destructFunction = destructFunctions[getIndex()];
				destructFunction(self());
			}
		}
		constexpr void destoryValue() noexcept {
			if (isConstantEvaluated() || !IsTriviallyDestructibleC<NamedVariant>) {
				actuallyDestoryValue();
			}
		}

	public:
		template<TemplateStringLiteral vFindName>
			requires(isValidName<vFindName>)
		constexpr static Size getIndexOf() noexcept {
			return impl::FindIndexOfStringLiteral<StringLiteral<vFindName>, StringLiteral<Elements::vName>...>::value;
		}

		template<TemplateStringLiteral vFindName>
			requires(isValidName<vFindName>)
		constexpr Size getIndexOf_NotStatic() const noexcept {
			return impl::FindIndexOfStringLiteral<StringLiteral<vFindName>, StringLiteral<Elements::vName>...>::value;
		}

		//assignment
	private:
		using VariantCopyFunction = void(*)(NamedVariant&, const NamedVariant&);

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr VariantCopyFunction getCopyFunction() noexcept {
			return [](NamedVariant& variant, const NamedVariant& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyConstRefAssignable<ElementType>) {
						copyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template getRef<vIndex, ElementType>() = other.recursiveStorage.template getRef<vIndex, ElementType>();
			};
		}
		using VariantMoveFunction = void(*)(NamedVariant&, NamedVariant&&);

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr VariantMoveFunction getMoveFunction() noexcept {
			return [](NamedVariant& variant, NamedVariant&& other) {
				using ElementType = typename TemplatePackNthElement<vIndex, Elements...>::type::value_type;

				if (!isConstantEvaluated()) {
					if constexpr (IsTriviallyMoveAssignable<ElementType>) {
						copyInternalStorage(variant.byteStorage, other.byteStorage);
						return;
					}
				}

				variant.recursiveStorage.template getRef<vIndex, ElementType>() = move(other.recursiveStorage.template getRef<vIndex, ElementType>());
			};
		}

	public:
		constexpr NamedVariant& operator=(const NamedVariant& other) noexcept {
			if (other.variantIndex == emptyVariantValue) {
				destoryValue();
				variantIndex = 0;
			} else if (variantIndex == other.variantIndex) {
				VariantCopyFunction copyFunctions[numberOfVariants] = { getCopyFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
				VariantCopyFunction& copyFunction = copyFunctions[other.getIndex()];

				if (isConstantEvaluated()) {
					copyFunction(self(), other);
				} else {
					if constexpr (!IsTriviallyConstRefConstructibleC<NamedVariant>) {
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

		constexpr NamedVariant& operator=(NamedVariant&& other) noexcept {
			if (other.variantIndex == emptyVariantValue) {
				destoryValue();
				variantIndex = emptyVariantValue;
			} else if (variantIndex == other.variantIndex) {
				VariantMoveFunction moveFunctions[numberOfVariants] = { getMoveFunction<TemplatePackFindIndexOfType<Elements, Elements...>::value>()... };
				VariantMoveFunction& moveFunction = moveFunctions[other.getIndex()];

				if (isConstantEvaluated()) {
					moveFunction(self(), forward<NamedVariant>(other));
				} else {
					if constexpr (!IsTriviallyMoveConstructibleC<NamedVariant>) {
						moveFunction(self(), forward<NamedVariant>(other));
					} else {
						copyInternalStorage(byteStorage, other.byteStorage);
					}
				}
			} else {
				destoryValue();
				moveConstruct(forward<NamedVariant>(other));
			}
			return self();
		}

		template<Size vIndex, typename DataType>
			requires(isValidIndex<vIndex>)
		constexpr NamedVariant& assign(const DataType& value) noexcept {
			using variant_type_at_index = TemplatePackNthElement<vIndex, Elements...>::type::value_type;

			if (variantIndex == vIndex + 1) {
				if (isConstantEvaluated()) {
					recursiveStorage.template getRef<vIndex, variant_type_at_index>() = value;
				} else {
					*reinterpret_cast<variant_type_at_index*>(byteStorage) = value;
				}
			} else {
				destoryValue();
				if (isConstantEvaluated()) {
					recursiveStorage.template construct<vIndex>(value);
				} else {
					natl::construct<variant_type_at_index>(
						reinterpret_cast<variant_type_at_index*>(byteStorage),
						value
					);
				}
			}
			variantIndex = vIndex + 1;
			return self();
		}

		template<Size vIndex, typename DataType>
			requires(isValidIndex<vIndex>)
		constexpr NamedVariant& assign(DataType&& value) noexcept {
			using variant_type_at_index = TemplatePackNthElement<vIndex, Elements...>::type::value_type;

			if (variantIndex == vIndex + 1) {
				if (isConstantEvaluated()) {
					recursiveStorage.template getRef<vIndex, variant_type_at_index>() = forward<DataType>(value);
				} else {
					*reinterpret_cast<variant_type_at_index*>(byteStorage) = forward<DataType>(value);
				}
			} else {
				destoryValue();
				if (isConstantEvaluated()) {
					recursiveStorage.template construct<vIndex>(forward<DataType>(value));
				} else {
					natl::construct<variant_type_at_index>(
						reinterpret_cast<variant_type_at_index*>(byteStorage),
						forward<DataType>(value)
					);
				}
			}
			variantIndex = vIndex + 1;
			return self();
		}

		template<TemplateStringLiteral vName, typename DataType>
			requires(isValidName<vName>)
		constexpr NamedVariant& assign(const DataType& value) noexcept {
			constexpr Size index = impl::FindIndexOfStringLiteral<StringLiteral<vName>, StringLiteral<Elements::vName>...>::value;
			using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;

			if constexpr (IsConstructibleC<VariantTypeAtIndex, DataType>) {
				if (variantIndex == index + 1) {
					if (isConstantEvaluated()) {
						recursiveStorage.template getRef<index, VariantTypeAtIndex>() = value;
					} else {
						*reinterpret_cast<VariantTypeAtIndex*>(byteStorage) = value;
					}
				} else {
					destoryValue();
					if (isConstantEvaluated()) {
						recursiveStorage.template construct<index>(value);
					} else {
						natl::construct<VariantTypeAtIndex>(
							reinterpret_cast<VariantTypeAtIndex*>(byteStorage),
							value
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
			return self();
		}

		template<TemplateStringLiteral vName, typename DataType>
			requires(isValidName<vName>)
		constexpr NamedVariant& assign(DataType&& value) noexcept {
			constexpr Size index = impl::FindIndexOfStringLiteral<StringLiteral<vName>, StringLiteral<Elements::vName>...>::value;
			using decayed_data_type = Decay<DataType>;
			using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;

			if constexpr (IsConstructibleC<VariantTypeAtIndex, DataType>) {
				if (variantIndex == index + 1) {
					if (isConstantEvaluated()) {
						recursiveStorage.template getRef<index, VariantTypeAtIndex>() = forward<DataType>(value);
					} else {
						*reinterpret_cast<VariantTypeAtIndex*>(byteStorage) = forward<DataType>(value);
					}
				} else {
					destoryValue();
					if (isConstantEvaluated()) {
						recursiveStorage.template construct<index>(forward<DataType>(value));
					} else {
						natl::construct<VariantTypeAtIndex>(
							reinterpret_cast<VariantTypeAtIndex*>(byteStorage),
							forward<DataType>(value)
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
		}

		constexpr void reset() noexcept {
			destoryValue();
			variantIndex = emptyVariantValue;
		}

	private:
		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr void testValidIndex() const noexcept {
			if (variantIndex != vIndex + 1) [[unlikely]] {
				natl::fatalError("variant index violation");
			}
		}

	public:
		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr auto& get() noexcept {
			testValidIndex<vIndex>();
			using variant_type_at_index = TemplatePackNthElement<vIndex, Elements...>::type::value_type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<vIndex, variant_type_at_index>();
			} else {
				return *reinterpret_cast<variant_type_at_index*>(byteStorage);
			}
		}

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr const auto& get() const noexcept {
			testValidIndex<vIndex>();
			using variant_type_at_index = TemplatePackNthElement<vIndex, Elements...>::type::value_type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<vIndex, variant_type_at_index>();
			} else {
				return *reinterpret_cast<const variant_type_at_index*>(byteStorage);
			}
		}

		template<TemplateStringLiteral vName>
			requires(isValidName<vName>)
		constexpr auto& get() noexcept {
			constexpr Size index = impl::FindIndexOfStringLiteral<StringLiteral<vName>, StringLiteral<Elements::vName>...>::value;
			testValidIndex<index>();

			using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<index, VariantTypeAtIndex>();
			} else {
				return *reinterpret_cast<VariantTypeAtIndex*>(byteStorage);
			}
		}

		template<TemplateStringLiteral vName>
			requires(isValidName<vName>)
		constexpr const auto& get() const noexcept {
			constexpr Size index = impl::FindIndexOfStringLiteral<StringLiteral<vName>, StringLiteral<Elements::vName>...>::value;
			testValidIndex<index>();
			using VariantTypeAtIndex = typename TemplatePackNthElement<index, Elements...>::type::value_type;
			if (isConstantEvaluated()) {
				return recursiveStorage.template getRef<index, VariantTypeAtIndex>();
			} else {
				return *reinterpret_cast<const VariantTypeAtIndex*>(byteStorage);
			}
		}

		constexpr Bool doesNotHaveValue() const noexcept {
			return variantIndex == emptyVariantValue;
		}
		constexpr Bool hasValue() const noexcept {
			return variantIndex != emptyVariantValue;
		}
		constexpr Size getIndex() const noexcept {
			return variantIndex - 1;
		}

		constexpr static Size getIndexOfEmpty() noexcept {
			return Size(0) - 1;
		}

		template<Size vIndex>
			requires(isValidIndex<vIndex>)
		constexpr natl::Bool isIndex() const noexcept {
			return variantIndex == vIndex + 1;
		}

		template<TemplateStringLiteral vName>
		constexpr Bool isValue() const noexcept {
			return variantIndex == (getIndexOf<vName>() + 1);
		}

		//special
	private:
		using test_element_str_function = Bool(*)(const ConstAsciiStringView&);
		template<Size vIndex, typename Element>
		constexpr static test_element_str_function getTestElementStrFunction() noexcept {
			return [](const ConstAsciiStringView& str) -> Bool {
				return str == Element::vName;
			};
		}

	public:
		constexpr static Option<Size> stringToIndexStatic(const ConstAsciiStringView& str) noexcept {
			return[&] <Size... vIndices>(IndexSequence<vIndices...>) -> Option<Size> {
				test_element_str_function testFunctions[sizeof...(Elements)] = {
					getTestElementStrFunction<vIndices, Elements>()...
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
			return[&] <Size... vIndices>(IndexSequence<vIndices...>) -> Option<Size> {
				test_element_str_function testFunctions[sizeof...(Elements)] = {
					getTestElementStrFunction<vIndices, Elements>()...
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
	struct IsTriviallyRelocatableV<NamedVariant<Elements...>>
		: BoolConstant<(IsTriviallyRelocatableC<typename Elements::value_type> && ...)> {
	};
	template<typename... Elements>
	struct IsTriviallyConstructibleV<NamedVariant<Elements...>>
		: TrueType {
	};
	template<typename... Elements>
	struct IsTriviallyDestructibleV<NamedVariant<Elements...>>
		: BoolConstant<(IsTriviallyDestructibleC<typename Elements::value_type> && ...)> {
	};

	template<typename... Elements>
	struct IsTriviallyConstRefConstructibleV<NamedVariant<Elements...>>
		: BoolConstant<(IsTriviallyConstRefConstructibleC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<NamedVariant<Elements...>>> {
	};
	template<typename... Elements>
	struct IsTriviallyMoveConstructibleV<NamedVariant<Elements...>>
		: BoolConstant<(IsTriviallyMoveConstructibleC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<NamedVariant<Elements...>>> {
	};

	template<typename... Elements>
	struct IsTriviallyConstRefAssignableV<NamedVariant<Elements...>>
		: BoolConstant<(IsTriviallyConstRefAssignableC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<NamedVariant<Elements...>>> {
	};
	template<typename... Elements>
	struct IsTriviallyMoveAssignableV<NamedVariant<Elements...>>
		: BoolConstant<(IsTriviallyMoveAssignableC<typename Elements::value_type> && ...)
		&& IsTriviallyDestructibleC<NamedVariant<Elements...>>> {
	};

}