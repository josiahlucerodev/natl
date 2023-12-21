#pragma once 

//own
#include "stringLiteral.h"
#include "string.h"
#include "batchPool.h"
#include "variant.h"

//interface 
namespace natl {
	enum class SchemaType : i8 {
		dt_i8, dt_i16, dt_i32, dt_i64,
		dt_ui8, dt_ui16, dt_ui32, dt_ui64,
		dt_size,
		dt_f32, dt_f64,
		dt_ascci_str,
		dt_array,
		dt_dic,
		dt_variant,
		dt_schema,
	};

	struct Schema;

	struct SchemaArrayInfo {
		SchemaType arrayElementType;
	};
	struct  SchemaDicInfo {
		SchemaType keyType;
		SchemaType valueType;
	};

	struct SchemaElement {
		String name;
		union SchemaElementData {
			const Schema* schema;
			SchemaArrayInfo arrayInfo;
			SchemaDicInfo dicInfo;

			constexpr SchemaElementData() : schema(nullptr) {}
			constexpr SchemaElementData(const Schema* schema) noexcept : schema(schema) {}
			constexpr SchemaElementData(const SchemaArrayInfo& arrayInfo) noexcept : arrayInfo(arrayInfo) {}
			constexpr SchemaElementData(const SchemaDicInfo& dicInfo) noexcept : dicInfo(dicInfo) {}
		} data;
		SchemaType type;

		constexpr SchemaElement() noexcept = default;
		constexpr SchemaElement(const StringView& name, const SchemaType type) noexcept : name(name), type(type), data() {}
		constexpr SchemaElement(const StringView& name, const Schema* schema) noexcept : name(name), type(SchemaType::dt_schema), data(schema) {}
	};

	struct Schema {
		String name;
		SchemaType type;
		SmallDynArray<SchemaElement, 8> elements;

		constexpr Schema() noexcept = default;
		constexpr Schema(const StringView& name, std::initializer_list<SchemaElement> elements) noexcept : name(name), type(SchemaType::dt_schema), elements(elements) {};
	};

	struct SerlizationElement;

	struct LargeSerlizationElement {
		struct SerlizationDicElement {
			SerlizationElement* key;
			SerlizationElement* value;
		};

		using LargeElementVariant = Variant<
			NamedElement<"schema", Schema>,
			NamedElement<"array", SmallDynArray<SerlizationElement*, (sizeof(Schema) / sizeof(SerlizationElement*)) - 3 >>,
			NamedElement<"ascci_str", BaseStringByteSize<AssciCode, sizeof(Schema)>>,
			NamedElement<"dic", SmallDynArray<SerlizationDicElement, (sizeof(Schema) / sizeof(SerlizationDicElement)) - 3 >>,
			NamedElement<"struct", SmallDynArray<SerlizationElement*, (sizeof(Schema) / sizeof(SerlizationElement*)) - 3 >>
		>;

		LargeElementVariant data;
	};

	struct SerlizationElement {
		using ElementVariant = Variant<
			NamedElement<"i8", i8>,
			NamedElement<"i16", i16>,
			NamedElement<"i32", i32>,
			NamedElement<"i64", i64>,

			NamedElement<"ui8", ui8>,
			NamedElement<"ui16", ui16>,
			NamedElement<"ui32", ui32>,
			NamedElement<"ui64", ui64>,

			NamedElement<"f32", f32>,
			NamedElement<"f64", f64>,
			NamedElement<"large", LargeSerlizationElement>,
		>;

		StringView name;
		ElementVariant data;
	};

	struct DeserlizationInterfaceBase {
		BatchPool<SerlizationElement> smallElementStorage;
		BatchPool<LargeSerlizationElement> largeElementStorage;

		constexpr void clear() noexcept {
			smallElementStorage.clear();
			largeElementStorage.clear();
		}
	};

	/*
	template<class SerlizationFormat>
	concept SerlizationInterface = requires(SerlizationFormat serializer) { 
		{ serializer.startElement(StringView(), i8()) } -> std::same_as<void>;
		{ serializer.endElement() } -> std::same_as<void>;
	};
	*/
}