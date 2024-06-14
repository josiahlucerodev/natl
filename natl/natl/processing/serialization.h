#pragma once 

//own
#include "../util/compilerDependent.h"
#include "../util/stringLiteral.h"
#include "../container/variant.h"
#include "../container/string.h"
#include "../container/batchPool.h"

//interface 
namespace natl {
	enum class SerializationOptionalType : Bool {
		False = false,
		True = true,
	};

	enum class SerializationBasicType : i8 {
		dt_i8, dt_i16, dt_i32, dt_i64,
		dt_ui8, dt_ui16, dt_ui32, dt_ui64,
		dt_size,
		dt_f32, dt_f64,
		dt_ascci_str,
	};

	enum class SerializationTypeFlag : i8 {
		dt_i8, dt_i16, dt_i32, dt_i64,
		dt_ui8, dt_ui16, dt_ui32, dt_ui64,
		dt_size,
		dt_f32, dt_f64,
		dt_ascci_str,
		dt_array,
		dt_dic,
		dt_struct,
		dt_tstruct,
		dt_structType,
		dt_variant,
		dt_variantType,
		dt_enum,
		dt_enumType,
	};

	constexpr SerializationTypeFlag serializationBasicTypeToFlag(const SerializationBasicType basicType) noexcept {
		switch (basicType) {
		case SerializationBasicType::dt_i8:
			return SerializationTypeFlag::dt_i8;
		case SerializationBasicType::dt_i16:
			return SerializationTypeFlag::dt_i16;
		case SerializationBasicType::dt_i32:
			return SerializationTypeFlag::dt_i32;
		case SerializationBasicType::dt_i64:
			return SerializationTypeFlag::dt_i64;
		case SerializationBasicType::dt_ui8:
			return SerializationTypeFlag::dt_ui8;
		case SerializationBasicType::dt_ui16:
			return SerializationTypeFlag::dt_ui16;
		case SerializationBasicType::dt_ui32:
			return SerializationTypeFlag::dt_ui32;
		case SerializationBasicType::dt_ui64:
			return SerializationTypeFlag::dt_ui64;
		case SerializationBasicType::dt_size:
			return SerializationTypeFlag::dt_size;
		case SerializationBasicType::dt_f32:
			return SerializationTypeFlag::dt_f32;
		case SerializationBasicType::dt_f64:
			return SerializationTypeFlag::dt_f64;
		case SerializationBasicType::dt_ascci_str:
			return SerializationTypeFlag::dt_ascci_str;
		default:
			unreachable();
		}
	}

	struct SerializationStructType;
	struct SerializationVariantType;
	struct SerializationEnumType;

	enum class SerializationTypeCategory : i8 {
		basicType,
		structType, 
		enumType,
		variantType,
	};

	struct SerializationBaseTypeElementVariant {
		union {
			SerializationTypeFlag basicType;
			const SerializationVariantType* variantType;
			const SerializationEnumType* enumType;
			const SerializationStructType* structType;
		};

		constexpr SerializationBaseTypeElementVariant(const SerializationTypeFlag basicTypeIn) noexcept 
			: basicType(basicTypeIn) {}
		constexpr SerializationBaseTypeElementVariant(const SerializationStructType* structTypeIn) noexcept
			: structType(structTypeIn) {}
		constexpr SerializationBaseTypeElementVariant(const SerializationEnumType* enumTypeIn) noexcept
			: enumType(enumTypeIn) {}
		constexpr SerializationBaseTypeElementVariant(const SerializationVariantType* variantTypeIn) noexcept
			: variantType(variantTypeIn) {}

		constexpr SerializationBaseTypeElementVariant(
			const SerializationTypeCategory& typeCategory,
			const SerializationBaseTypeElementVariant& other) {
			switch (typeCategory) {
			case SerializationTypeCategory::basicType:
				basicType = other.basicType;
				break;
			case SerializationTypeCategory::structType:
				structType = other.structType;
				break;
			case SerializationTypeCategory::enumType:
				enumType = other.enumType;
				break;
			case SerializationTypeCategory::variantType:
				variantType = other.variantType;
				break;
			default:
				unreachable();
			}
		}
	};

	struct SerializationTypeElementsInfo {
		SerializationOptionalType isOptionalFlag;
		SerializationTypeCategory typeCategory;
		SerializationBaseTypeElementVariant typeVariant;

		constexpr SerializationTypeElementsInfo(
			const SerializationBasicType basicType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeCategory(SerializationTypeCategory::basicType), typeVariant(serializationBasicTypeToFlag(basicType)) {}

		constexpr SerializationTypeElementsInfo(
			const SerializationStructType* structType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeCategory(SerializationTypeCategory::structType), typeVariant(structType) {}

		constexpr SerializationTypeElementsInfo(
			const SerializationEnumType* enumType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeCategory(SerializationTypeCategory::enumType), typeVariant(enumType) {}

		constexpr SerializationTypeElementsInfo(
			const SerializationVariantType* variantType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeCategory(SerializationTypeCategory::variantType), typeVariant(variantType) {}
	};

	struct SerializationArrayInfo {
		SerializationOptionalType isElementOptionalFlag;
		SerializationTypeCategory elementTypeCategory;
		SerializationBaseTypeElementVariant elementTypeVariant;

		constexpr SerializationArrayInfo(const SerializationTypeElementsInfo& elementsInfo) noexcept 
		: isElementOptionalFlag(elementsInfo.isOptionalFlag), elementTypeCategory(elementsInfo.typeCategory),
			elementTypeVariant(elementsInfo.typeCategory, elementsInfo.typeVariant) {}
	};

	struct SerializationDicInfo {
		SerializationOptionalType isValueOptionalFlag;

		SerializationTypeCategory keyTypeCategory;
		SerializationTypeCategory valueTypeCategory;
		SerializationBaseTypeElementVariant keyTypeVariant;
		SerializationBaseTypeElementVariant valueTypeVariant;

		constexpr SerializationDicInfo(
			const SerializationTypeElementsInfo& keyElementInfo, 
			const SerializationTypeElementsInfo& valueElementInfo) noexcept
			: isValueOptionalFlag(valueElementInfo.isOptionalFlag), 
			keyTypeCategory(keyElementInfo.typeCategory),
			valueTypeCategory(valueElementInfo.typeCategory),
			keyTypeVariant(keyElementInfo.typeCategory, keyElementInfo.typeVariant),
			valueTypeVariant(valueElementInfo.typeCategory, valueElementInfo.typeVariant) {}
	};

	struct SerializationType {

		SerializationOptionalType isOptionalFlag;
		SerializationTypeFlag typeFlag;

		union SchemaElementData {
			const SerializationStructType* structType;
			const SerializationEnumType* enumType;
			const SerializationVariantType* variantType;

			SerializationArrayInfo arrayInfo;
			SerializationDicInfo dicInfo;


			constexpr SchemaElementData() : structType(nullptr) {}
			constexpr SchemaElementData(const SerializationStructType* structTypeIn) noexcept : structType(structTypeIn) {}
			constexpr SchemaElementData(const SerializationEnumType* enumTypeIn) noexcept : enumType(enumTypeIn) {}
			constexpr SchemaElementData(const SerializationVariantType* variantTypeIn) noexcept : variantType(variantTypeIn) {}
			constexpr SchemaElementData(const SerializationArrayInfo& arrayInfoIn) noexcept : arrayInfo(arrayInfoIn) {}
			constexpr SchemaElementData(const SerializationDicInfo& dicInfoIn) noexcept : dicInfo(dicInfoIn) {}
		} data;

		constexpr SerializationType() noexcept = default;
		constexpr SerializationType(
			const SerializationBasicType basicType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeFlag(serializationBasicTypeToFlag(basicType)), data() {}

		constexpr SerializationType(
			const SerializationStructType* type,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeFlag(SerializationTypeFlag::dt_structType), data(type) {}

		constexpr SerializationType(
			const SerializationEnumType* enumType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeFlag(SerializationTypeFlag::dt_enumType), data(enumType) {}

		constexpr SerializationType(
			const SerializationVariantType* variantType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			isOptionalFlag(optionalFlag), typeFlag(SerializationTypeFlag::dt_variantType), data(variantType) {}
	};

	struct SerializationStructTypeElement {
		String name;
		SerializationType type;

		constexpr SerializationStructTypeElement() noexcept = default;
		constexpr SerializationStructTypeElement(
			const StringView& nameIn, 
			const SerializationBasicType basicType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			name(nameIn), type(basicType, optionalFlag) {}


		constexpr SerializationStructTypeElement(
			const StringView& nameIn, 
			const SerializationStructType* structType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			name(nameIn), type(structType, optionalFlag) {}

		constexpr SerializationStructTypeElement(
			const StringView& nameIn, 
			const SerializationEnumType* enumType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			name(nameIn), type(enumType, optionalFlag) {}

		constexpr SerializationStructTypeElement(
			const StringView& nameIn, 
			const SerializationVariantType* variantType,
			const SerializationOptionalType& optionalFlag = SerializationOptionalType::False) noexcept :
			name(nameIn), type(variantType, optionalFlag) {}
	};

	struct SerializationStructType {
		String name;
		SmallDynArray<SerializationStructTypeElement, 7> elements;

		constexpr SerializationStructType() noexcept = default;
		/*
		template<typename... SerializationStructTypeElementArgs>
			requires(IsSame<SerializationStructTypeElement, SerializationStructTypeElementArgs> && ...)
		constexpr SerializationStructType(const StringView& name, SerializationStructTypeElementArgs&&... elements) noexcept : name(name), elements(elements...) {};
		*/
		constexpr SerializationStructType(const StringView& nameIn, std::initializer_list<SerializationStructTypeElement> elementsIn) noexcept : name(nameIn), elements(elementsIn) {};
	};

	struct SerializationVariantType {
		String name;
		SmallDynArray<SerializationType, 7> types;

		constexpr SerializationVariantType() noexcept = default;
		constexpr SerializationVariantType(const StringView& nameIn, std::initializer_list<SerializationType> typesIn) noexcept : name(nameIn), types(typesIn) {};
	};

	struct SerializationEnumType {
		String name;
		SmallDynArray<String, 15> nameValues;
	};

	struct SerializationElement;
		
	struct LargeSerializationElement {
		using LargeElementVariant = Variant<
			NamedElement<"enum_type", SerializationEnumType>,
			NamedElement<"variant_type", SerializationVariantType>,
			NamedElement<"struct_type", SerializationStructType>
		>;

		LargeElementVariant data;
	};

	struct MediumSerializationElement {

		constexpr static Size commonByteSize = 120;

		struct SerializationDicElement {
			SerializationElement* key;
			SerializationElement* value;
		};

		using SerializationArrayData = SmallDynArray<SerializationElement*, ((commonByteSize - 24) / sizeof(SerializationElement*))>;
		using SerializationDicData = SmallDynArray<SerializationElement*, ((commonByteSize - 24) / sizeof(SerializationElement*))>;
		using SerializationAssicStringData = BaseStringByteSize<Ascii, commonByteSize >;
		using SerializationStructData = SmallDynArray<SerializationElement*, ((commonByteSize - 24) / sizeof(SerializationElement*))>;
		using SerializationTStructData = SmallDynArray<SerializationElement*, ((commonByteSize - 24) / sizeof(SerializationElement*))>;

		using MediumElementVariant = Variant<
			NamedElement<"array", SerializationArrayData>,
			NamedElement<"ascciStr", SerializationAssicStringData>,
			NamedElement<"dic", SerializationDicData>,
			NamedElement<"struct", SerializationStructData>,
			NamedElement<"tstruct", SerializationTStructData>
		>;

		MediumElementVariant data;
	};

	struct SerializationElement {
		using ElementVariant = Variant<
			NamedElement<"i8", i8>,
			NamedElement<"i16", i16>,
			NamedElement<"i32", i32>,
			NamedElement<"i64", i64>,

			NamedElement<"ui8", ui8>,
			NamedElement<"ui16", ui16>,
			NamedElement<"ui32", ui32>,
			NamedElement<"ui64", ui64>,

			NamedElement<"size", Size>,

			NamedElement<"f32", f32>,
			NamedElement<"f64", f64>,


			NamedElement<"enum", Size>,
			NamedElement<"variant", SerializationElement*>,
			NamedElement<"medium", MediumSerializationElement*>,
			NamedElement<"large", LargeSerializationElement*>
		>;

		StringView name;
		ElementVariant data;
	};

	struct DeserlizationInterfaceBase {
		BatchPool<SerializationElement> smallElementStorage;
		BatchPool<MediumSerializationElement> mediumElementStorage;
		BatchPool<LargeSerializationElement> largeElementStorage;

		constexpr void clear() noexcept {
			smallElementStorage.clear();
			mediumElementStorage.clear();
			largeElementStorage.clear();
		}
	};

	namespace impl {
		template<class Serializer>
		concept DoesSerializerHaveInt8 = requires(Serializer serializer, const i8& value) {
			{ serializer.serialize_i8(value) } -> SameAs<void>; 
			{ serializer.serializeNull_i8() } -> SameAs<void>; 
		};
		template<class Serializer>
		concept DoesSerializerHaveInt16 = requires(Serializer serializer, const i16& value) {
				{ serializer.serialize_i16(value) } -> SameAs<void>;
				{ serializer.serializeNull_i16() } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializerHaveInt32 = requires(Serializer serializer, const i32 & value) {
			{ serializer.serialize_i32(value) } -> SameAs<void>;
			{ serializer.serializeNull_i32() } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializerHaveInt64 = requires(Serializer serializer, const i64 & value) {
			{ serializer.serialize_i64(value) } -> SameAs<void>;
			{ serializer.serializeNull_i64() } -> SameAs<void>;
		};

		template<class Serializer>
		concept DoesSerializerHaveUInt8 = requires(Serializer serializer, const ui8 & value) {
			{ serializer.serialize_ui8(value) } -> SameAs<void>;
			{ serializer.serializeNull_ui8() } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializerHaveUInt16 = requires(Serializer serializer, const ui16 & value) {
			{ serializer.serialize_ui16(value) } -> SameAs<void>;
			{ serializer.serializeNull_ui16() } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializerHaveUInt32 = requires(Serializer serializer, const ui32 & value) {
			{ serializer.serialize_ui32(value) } -> SameAs<void>;
			{ serializer.serializeNull_ui32() } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializerHaveUInt64 = requires(Serializer serializer, const ui64 & value) {
			{ serializer.serialize_ui64(value) } -> SameAs<void>;
			{ serializer.serializeNull_ui64() } -> SameAs<void>;
		};

		template<class Serializer>
		concept DoesSerializerHaveFloat32 = requires(Serializer serializer, const f32 & value) {
			{ serializer.serialize_f32(value) } -> SameAs<void>;
			{ serializer.serializeNull_f32() } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializerHaveFloat64 = requires(Serializer serializer, const f64 & value) {
			{ serializer.serialize_f64(value) } -> SameAs<void>;
			{ serializer.serializeNull_f64() } -> SameAs<void>;
		};

		template<class Serializer>concept DoesSerializerHaveAssicStr = requires(Serializer serializer,
			const ConstAsciiStringView & value) {
				{ serializer.serializeAssicStr(value) } -> SameAs<void>;
				{ serializer.serializeNullAssicStr() } -> SameAs<void>;
		};

		template<class Serializer>
		concept DoesSerializeHaveTStruct = requires(Serializer serializer, const SerializationStructType& structType) {
				{ serializer.serializeStructType(structType) } -> SameAs<void>;
				{ serializer.serializeNamedTstructInfo(structType) } -> SameAs<void>;
				{ serializer.serializeTypedStruct(structType) } -> SameAs<void>;
				{ serializer.endSerializeTypedStruct() } -> SameAs<void>;
				{ serializer.serializeNullTypedStruct(structType) } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializeHaveStruct = requires(Serializer serializer) {
				{ serializer.serializeStruct() } -> SameAs<void>;
				{ serializer.endSerializeStruct() } -> SameAs<void>;
				{ serializer.serializeNullStruct() } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializeHaveDic = requires(Serializer serializer, const natl::SerializationDicInfo& dicInfo) {
				{ serializer.serializeNamedDicInfo(dicInfo) } -> SameAs<void>;
				{ serializer.serializeDic(dicInfo) } -> SameAs<void>;
				{ serializer.endSerializeDic() } -> SameAs<void>;
				{ serializer.serializeDicKey() } -> SameAs<void>;
				{ serializer.serializeDicValue() } -> SameAs<void>;
				{ serializer.serializeNullDic(dicInfo) } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializeHaveArray = requires(Serializer serializer, const natl::SerializationArrayInfo& arrayInfo) {
				{ serializer.serializeNamedArrayInfo(arrayInfo) } ->SameAs<void>;
				{ serializer.serializeNextArrayElement() } ->SameAs<void>;
				{ serializer.serializeArray(arrayInfo) } -> SameAs<void>;
				{ serializer.endSerializeArray() } -> SameAs<void>;
				{ serializer.serializeNullArray(arrayInfo) } -> SameAs<void>;
		};
		template<class Serializer>
		concept DoesSerializeHaveVariant = requires(
			Serializer serializer,
			const SerializationVariantType& variantType,
			const SerializationType& variantElementType) {
				{ serializer.serializeNamedVariantInfo(variantType, variantElementType) } -> SameAs<void>;
				{ serializer.serializeVariantType(variantType) } -> SameAs<void>;
				{ serializer.serializeVariant(variantType, variantElementType) } -> SameAs<void>;
				{ serializer.endSerializeVariant() } -> SameAs<void>;
				{ serializer.serializeNullVariant(variantType) } -> SameAs<void>;
		};
		
		template<class Serializer>
		concept DoesSerializeHaveEnum = requires(Serializer serializer,
			const natl::SerializationEnumType& enumType,
			const natl::Size nameIndex) {
				{ serializer.serializeEnumType(enumType) } -> SameAs<void>;
				{ serializer.serializeNamedEnumInfo(enumType) } -> SameAs<void>;
				{ serializer.serializeEnum(enumType, nameIndex) } -> SameAs<void>;
				{ serializer.serializeNullEnum(enumType) } -> SameAs<void>;
		};	

		template<class Serializer>
		concept DoesSerializeHaveBaseSerializeUtil = requires(
			Serializer serializer, 
			const Size& bytes, 
			const ConstAsciiStringView& name) {
				{ serializer.serializeBegin(bytes) } -> SameAs<void>;
				{ serializer.serializeEnd() } -> SameAs<void>;
				{ serializer. template serializeNamedElement<SerializationTypeFlag::dt_i8, SerializationOptionalType::True>(name) } -> SameAs<void>;
		};
	}

	template<class Serializer>
	concept IsCompleteSerializer =
		impl::DoesSerializeHaveBaseSerializeUtil<Serializer> &&
		impl::DoesSerializerHaveInt8<Serializer> &&
		impl::DoesSerializerHaveInt16<Serializer> &&
		impl::DoesSerializerHaveInt32<Serializer> &&
		impl::DoesSerializerHaveInt64<Serializer> &&
		impl::DoesSerializerHaveUInt8<Serializer> &&
		impl::DoesSerializerHaveUInt16<Serializer> &&
		impl::DoesSerializerHaveUInt32<Serializer> &&
		impl::DoesSerializerHaveUInt64<Serializer> &&
		impl::DoesSerializerHaveFloat32<Serializer> &&
		impl::DoesSerializerHaveFloat64<Serializer> &&
		impl::DoesSerializerHaveAssicStr<Serializer> && 
		impl::DoesSerializeHaveTStruct<Serializer> &&
		impl::DoesSerializeHaveStruct<Serializer> &&
		impl::DoesSerializeHaveDic<Serializer> &&
		impl::DoesSerializeHaveArray<Serializer> &&
		impl::DoesSerializeHaveEnum<Serializer> &&
		impl::DoesSerializeHaveVariant<Serializer>;
}