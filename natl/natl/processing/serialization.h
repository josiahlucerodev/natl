#pragma once 

//own
#include "../util/compilerDependent.h"
#include "../util/stringLiteral.h"
#include "../container/variant.h"
#include "../container/string.h"
#include "../container/batchPool.h"

//interface 
namespace natl {
	enum class SerializeFlag {
		pretty,
		compress,
	};

	enum class SerializeID {
		none = 0,
		name = 1 << 0,
		type = 1 << 1,
	};

	constexpr SerializeID operator&(const SerializeID& lhs, const SerializeID& rhs) noexcept {
		return natl::fromUnderlying<SerializeID>(toUnderlying<SerializeID>(lhs) & toUnderlying<SerializeID>(rhs));
	}
	constexpr SerializeID operator|(const SerializeID& lhs, const SerializeID& rhs) noexcept {
		return natl::fromUnderlying<SerializeID>(toUnderlying<SerializeID>(lhs) | toUnderlying<SerializeID>(rhs));
	}
	constexpr SerializeID& operator&=(SerializeID& lhs, const SerializeID& rhs) noexcept {
		lhs = lhs & rhs;
		return lhs;
	}
	constexpr SerializeID& operator|=(SerializeID& lhs, const SerializeID& rhs) noexcept {
		lhs = lhs | rhs;
		return lhs;
	}

	template<typename IntegerType> 
		requires(IsBuiltInIntegerC<IntegerType>)
	struct SerializeNumberTag {
		using number_type = IntegerType;
		IntegerType number;
		constexpr SerializeNumberTag(IntegerType numberIn) noexcept : number(numberIn) {}
	};
	
	struct SerializeNameTag {
		ConstAsciiStringView name;
	};

	template<typename Type> struct IsSerializeNumberTagV : FalseType {};
	template<typename IntegerType> struct IsSerializeNumberTagV<SerializeNumberTag<IntegerType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeNumberTag = IsSerializeNumberTagV<Type>::value;
	template<typename Type> concept IsSerializeNumberTagC = IsSerializeNumberTag<Type>;

	struct SerializeCharType {};
	struct SerializeStrType {};
	struct SerializeFlagType {};
	struct SerializeStructType {};
	struct SerializeFileType {};
	struct SerializeBlobType {};
	template<typename SerializeType>
	struct SerializeOptionalType {};
	template<typename ElementType>
	struct SerializeArrayType {};
	template<typename KeyType, typename ValueType>
	struct SerializeDicType {};


	template<typename Type> struct IsSerializeCharTypeV : FalseType {};
	template<> struct IsSerializeCharTypeV<SerializeCharType> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeCharType = IsSerializeCharTypeV<Type>::value;
	template<typename Type> concept IsSerializeCharTypeC = IsSerializeCharType<Type>;

	template<typename Type> struct IsSerializeStrTypeV : FalseType {};
	template<> struct IsSerializeStrTypeV<SerializeStrType> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeStrType = IsSerializeStrTypeV<Type>::value;
	template<typename Type> concept IsSerializeStrTypeC = IsSerializeStrType<Type>;

	template<typename Type> struct IsSerializeFlagTypeV : FalseType {};
	template<> struct IsSerializeFlagTypeV<SerializeFlagType> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeFlagType = IsSerializeFlagTypeV<Type>::value;
	template<typename Type> concept IsSerializeFlagTypeC = IsSerializeFlagType<Type>;
	
	template<typename Type> struct IsSerializeStructTypeV : FalseType {};
	template<> struct IsSerializeStructTypeV<SerializeStructType> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeStructType = IsSerializeStructTypeV<Type>::value;
	template<typename Type> concept IsSerializeStructTypeC = IsSerializeStructType<Type>;

	template<typename Type> struct IsSerializeFileTypeV : FalseType {};
	template<> struct IsSerializeFileTypeV<SerializeFileType> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeFileType = IsSerializeFileTypeV<Type>::value;
	template<typename Type> concept IsSerializeFileTypeC = IsSerializeFileType<Type>;
	
	template<typename Type> struct IsSerializeBlobTypeV : FalseType {};
	template<> struct IsSerializeBlobTypeV<SerializeBlobType> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeBlobType = IsSerializeBlobTypeV<Type>::value;
	template<typename Type> concept IsSerializeBlobTypeC = IsSerializeBlobType<Type>;
	
	template<typename Type> struct IsSerializeOptionalTypeV : FalseType {};
	template<typename SerializeType> struct IsSerializeOptionalTypeV<SerializeOptionalType<SerializeType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeOptionalType = IsSerializeOptionalTypeV<Type>::value;
	template<typename Type> concept IsSerializeOptionalTypeC = IsSerializeOptionalType<Type>;

	template<typename Type> struct IsSerializeArrayTypeV : FalseType {};
	template<typename ElementType> struct IsSerializeArrayTypeV<SerializeArrayType<ElementType>> : FalseType {};
	template<typename Type> constexpr inline Bool IsSerializeArrayType = IsSerializeArrayTypeV<Type>::value;
	template<typename Type> concept IsSerializeArrayTypeC = IsSerializeArrayType<Type>;

	template<typename Type> struct IsSerializeDicTypeV : FalseType {};
	template<typename KeyType, typename ValueType> struct IsSerializeDicTypeV<SerializeDicType<KeyType, ValueType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeDicType = IsSerializeDicTypeV<Type>::value;
	template<typename Type> concept IsSerializeDicTypeC = IsSerializeDicType<Type>;

	template<typename Type> struct Serialize;

	template<typename SerializeType> struct IsSerializableV : BoolConstant<requires() { typename Serialize<SerializeType>::serialize_as_type; }> {};
	template<> struct IsSerializableV<SerializeCharType> : TrueType {};
	template<> struct IsSerializableV<SerializeStrType> : TrueType {};
	template<> struct IsSerializableV<SerializeFlagType> : TrueType {};
	template<> struct IsSerializableV<SerializeStructType> : TrueType {};
	template<> struct IsSerializableV<SerializeFileType> : TrueType {};
	template<> struct IsSerializableV<SerializeBlobType> : TrueType {};
	template<typename SerializeType> struct IsSerializableV<SerializeOptionalType<SerializeType>> : TrueType {};
	template<typename ElementType> struct IsSerializableV<SerializeArrayType<ElementType>> : TrueType {};
	template<typename KeyType, typename ValueType> struct IsSerializableV<SerializeDicType<KeyType, ValueType>> : TrueType {};

	template<typename SerializeType> constexpr Bool IsSerializable = IsSerializableV<SerializeType>::value;
	template<typename SerializeType> concept IsSerializableC = IsSerializable<SerializeType>;

	template<typename Serializer>
	concept IsSerializerC = true;

	template<typename Functor>
	concept IsToFlagConvertFunctor = requires(Functor functor, const natl::Size value) {
		{ functor(value) } -> IsStringViewLike<Ascii>;
	};

	template<typename Serializer, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWrite(Serializer& serializer, const SerializeType& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		Serialize<Decay<SerializeType>>::template write<Serializer>(serializer, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}


	//ID
	template<typename Serializer, SerializeID ID, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteIDNamed(
		Serializer& serializer,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		using decayed_serialize_type = Serialize<Decay<SerializeType>>;
		using serialize_as_type = decayed_serialize_type::serialize_as_type;

		serializer.beginWrite<ID>(name);
		serializer.template as<serialize_as_type, ID>();
		serializer.writeValue();
		decayed_serialize_type::template write<Serializer>(serializer, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
		serializer.endWrite();
	}

	template<typename Serializer, SerializeID ID, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteIDNamedWithNameTag(
		Serializer& serializer,
		SerializeNameTag& serializeNameTag,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		using decayed_serialize_type = Serialize<Decay<SerializeType>>;
		using serialize_as_type = decayed_serialize_type::serialize_as_type;

		serializer.template beginWrite<ID>(name, serializeNameTag);
		serializer.template as<serialize_as_type, ID>();
		serializer.writeValue();
		decayed_serialize_type::template write<Serializer>(serializer, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
		serializer.endWrite();
	}

	template<typename Serializer, SerializeID ID, typename SerializeNumberTagType, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeNumberTagC<SerializeNumberTagType>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteIDNamedWithNumberTag(
		Serializer& serializer,
		const SerializeNumberTagType& serializeNumberTag,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		using decayed_serialize_type = Serialize<Decay<SerializeType>>;
		using serialize_as_type = decayed_serialize_type::serialize_as_type;

		serializer.template beginWrite<SerializeID::none, SerializeNumberTagType>(name, serializeNumberTag);
		serializer.template as<serialize_as_type, SerializeID::none>();
		serializer.writeValue();
		decayed_serialize_type::template write<Serializer>(serializer, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
		serializer.endWrite();
	}

	template<typename Serializer, SerializeID ID, typename SerializeNumberTagType, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeNumberTagC<SerializeNumberTagType>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteIDNamedWithNumberTagAndNumberTag(
		Serializer& serializer,
		SerializeNameTag& serializeNameTag,
		const SerializeNumberTagType& serializeNumberTag,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		using decayed_serialize_type = Serialize<Decay<SerializeType>>;
		using serialize_as_type = decayed_serialize_type::serialize_as_type;

		serializer.template beginWrite<ID, SerializeNumberTagType>(name, serializeNameTag, serializeNumberTag);
		serializer.template as<serialize_as_type, ID>();
		serializer.writeValue();
		decayed_serialize_type::template write<Serializer>(serializer, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
		serializer.endWrite();
	}

	template<typename Serializer, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteNamed(
		Serializer& serializer,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		serializeWriteIDNamed<Serializer, SerializeID::none, SerializeType, SerializeArgs...>(
			serializer, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename Serializer, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteNamedWithNameTag(
		Serializer& serializer,
		SerializeNameTag& serializeNameTag,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		serializeWriteIDNamedWithNameTag<Serializer, SerializeID::none, SerializeType, SerializeArgs...>(
			serializer, serializeNameTag, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename Serializer, typename SerializeNumberTagType, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer> && IsSerializeNumberTagC<SerializeNumberTagType> && IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteNamedWithNumberTag(
		Serializer& serializer,
		const SerializeNumberTagType& serializeNumberTag,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		serializeWriteIDNamedWithNumberTag<Serializer, SerializeID::none, SerializeNumberTagType, SerializeType, SerializeArgs...>(
			serializer, serializeNumberTag, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename Serializer, typename SerializeNumberTagType, typename SerializeType, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeNumberTagC<SerializeNumberTagType>&& IsSerializableC<Decay<SerializeType>>)
	constexpr void serializeWriteNamedWithNumberTagAndNumberTag(
		Serializer& serializer,
		SerializeNameTag& serializeNameTag,
		const SerializeNumberTagType& serializeNumberTag,
		const ConstAsciiStringView& name,
		const SerializeType& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {

		serializeWriteIDNamedWithNumberTag<Serializer, SerializeID::none, SerializeNumberTagType, SerializeType, SerializeArgs...>(
			serializer, serializeNameTag, serializeNumberTag, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}


	template<> struct Serialize<natl::i8> {
		using serialize_as_type = natl::i8;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i8 value) noexcept {
			serializer.writeI8(value);
		}
	};

	template<> struct Serialize<natl::i16> {
		using serialize_as_type = natl::i16;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i16 value) noexcept {
			serializer.writeI16(value);
		}
	};
	template<> struct Serialize<natl::i32> {
		using serialize_as_type = natl::i32;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i32 value) noexcept {
			serializer.writeI32(value);
		}
	};
	template<> struct Serialize<natl::i64> {
		using serialize_as_type = natl::i64;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i64 value) noexcept {
			serializer.writeI64(value);
		}
	};

	template<> struct Serialize<natl::ui8> {
		using serialize_as_type = natl::ui8;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui8 value) noexcept {
			serializer.writeUI8(value);
		}
	};
	template<> struct Serialize<natl::ui16> {
		using serialize_as_type = natl::ui16;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui16 value) noexcept {
			serializer.writeUI16(value);
		}
	};
	template<> struct Serialize<natl::ui32> {
		using serialize_as_type = natl::ui32;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui32 value) noexcept {
			serializer.writeUI32(value);
		}
	};
	template<> struct Serialize<natl::ui64> {
		using serialize_as_type = natl::ui64;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui64 value) noexcept {
			serializer.writeUI64(value);
		}
	};

	template<> struct Serialize<natl::f32> {
		using serialize_as_type = natl::f32;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::f32 value) noexcept {
			serializer.writeF32(value);
		}
	};
	template<> struct Serialize<natl::f64> {
		using serialize_as_type = natl::f64;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::f64 value) noexcept {
			serializer.writeF64(value);
		}
	};

	template<> struct Serialize<natl::Ascii> {
		using serialize_as_type = SerializeCharType;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::Ascii value) noexcept {
			serializer.writeChar(value);
		}
	};

	template<> struct Serialize<char*> {
		using serialize_as_type = SerializeStrType;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const char* str) noexcept {
			serializer.writeStr(str);
		}
	};
	template<> struct Serialize<natl::AsciiStringView> {
		using serialize_as_type = SerializeStrType;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::AsciiStringView str) noexcept {
			serializer.writeStr(str);
		}
	};
	template<> struct Serialize<natl::ConstAsciiStringView> {
		using serialize_as_type = SerializeStrType;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ConstAsciiStringView str) noexcept {
			serializer.writeStr(str);
		}
	};
}