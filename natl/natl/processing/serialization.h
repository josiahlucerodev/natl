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

	template<typename Type> struct IsSerializeTypeV : FalseType {};
	template<typename Type> constexpr inline Bool IsSerializeType = IsSerializeTypeV<Type>::value;
	template<typename Type> concept IsSerializeTypeC = IsSerializeType<Type>;

	template<typename Type> struct IsEnumBaseSerializeTypeV : FalseType {};
	template<typename Type> constexpr inline Bool IsEnumBaseSerializeType = IsEnumBaseSerializeTypeV<Type>::value;
	template<typename Type> concept IsEnumBaseSerializeTypeC = IsEnumBaseSerializeType<Type>;

	struct SerializeI8 {};
	struct SerializeI16 {};
	struct SerializeI32 {};
	struct SerializeI64 {};
	struct SerializeUI8 {};
	struct SerializeUI16 {};
	struct SerializeUI32 {};
	struct SerializeUI64 {};
	struct SerializeF32 {};
	struct SerializeF64 {};
	struct SerializeBool {};
	struct SerializeChar {};
	struct SerializeStr {};
	struct SerializeFile {};
	struct SerializeBlob {};

	template<typename SerializeType>
		requires(IsSerializeTypeC<SerializeType>)
	struct SerializeOptional {};

	template<typename BaseSerializeType>
		requires(IsEnumBaseSerializeTypeC<BaseSerializeType>)
	struct SerializeEnum {
		using base_type = BaseSerializeType;
	};

	template<typename ElementType, Size Number>
		requires(IsSerializeTypeC<ElementType>)
	struct SerializeFixedArray {};

	template<typename ElementType>
		requires(IsSerializeTypeC<ElementType>)
	struct SerializeArray {};

	template<typename KeyType, typename ValueType>
		requires(IsSerializeTypeC<KeyType> && IsSerializeTypeC<ValueType>)
	struct SerializeDic {};

	template<typename KeyType, typename ValueType>
		requires(IsSerializeTypeC<KeyType>&& IsSerializeTypeC<ValueType>)
	struct SerializeDicElement {};

	template<typename... MemberTypes>
		requires(IsSerializeTypeC<MemberTypes> && ...)
	struct SerializeStruct { using member_types = TypePack<MemberTypes...>; };

	template<typename... Types>
		requires(IsSerializeTypeC<Types> && ...)
	struct SerializeVariant { using types = TypePack<Types...>; };

	template<> struct IsEnumBaseSerializeTypeV<SerializeI8> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeI16> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeI32> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeI64> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI8> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI16> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI32> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI64> : TrueType {};


	template<typename SerializeType> struct IsBasicSerializeTypeV : FalseType {};
	template<typename SerializeType> constexpr inline Bool IsBasicSerializeType = IsBasicSerializeTypeV<SerializeType>::value;
	template<typename SerializeType> concept IsBasicSerializeTypeC = IsBasicSerializeType<SerializeType>;
	template<> struct IsBasicSerializeTypeV<SerializeI8> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeI16> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeI32> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeI64> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeUI8> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeUI16> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeUI32> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeUI64> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeF32> : TrueType {};
	template<> struct IsBasicSerializeTypeV<SerializeF64> : TrueType {};

	template<typename SerializeType> 
		requires(IsBasicSerializeTypeC<SerializeType>)
	struct BasicSerializeTypeToTypeT;

	template<typename SerializeType> 
		requires(IsBasicSerializeTypeC<SerializeType>)
	using BasicSerializeTypeToType = BasicSerializeTypeToTypeT<SerializeType>::type;

	template<> struct BasicSerializeTypeToTypeT<SerializeI8> { using type = i8; };
	template<> struct BasicSerializeTypeToTypeT<SerializeI16> { using type = i16; };
	template<> struct BasicSerializeTypeToTypeT<SerializeI32> { using type = i32; };
	template<> struct BasicSerializeTypeToTypeT<SerializeI64> { using type = i64; };
	template<> struct BasicSerializeTypeToTypeT<SerializeUI8> { using type = ui8; };
	template<> struct BasicSerializeTypeToTypeT<SerializeUI16> { using type = ui16; };
	template<> struct BasicSerializeTypeToTypeT<SerializeUI32> { using type = ui32; };
	template<> struct BasicSerializeTypeToTypeT<SerializeUI64> { using type = ui64; };
	template<> struct BasicSerializeTypeToTypeT<SerializeF32> { using type = f32; };
	template<> struct BasicSerializeTypeToTypeT<SerializeF64> { using type = f64; };


	template<> struct IsSerializeTypeV<SerializeI8> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeI16> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeI32> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeI64> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeUI8> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeUI16> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeUI32> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeUI64> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeF32> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeF64> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeBool> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeChar> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeStr> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeFile> : TrueType {};
	template<> struct IsSerializeTypeV<SerializeBlob> : TrueType {};

	template<typename SerializeType> 
	struct IsSerializeTypeV<SerializeOptional<SerializeType>> : TrueType {};
	template<typename BaseSerializeType> 
	struct IsSerializeTypeV<SerializeEnum<BaseSerializeType>> : TrueType {};
	template<typename ElementType, Size Number> 
	struct IsSerializeTypeV<SerializeFixedArray<ElementType, Number>> : TrueType {};
	template<typename ElementType> 
	struct IsSerializeTypeV<SerializeArray<ElementType>> : TrueType {};
	template<typename KeyType, typename ValueType> 
	struct IsSerializeTypeV<SerializeDic<KeyType, ValueType>> : TrueType {};
	template<typename... MemberTypes> 
	struct IsSerializeTypeV<SerializeStruct<MemberTypes...>> : TrueType {};
	template<typename... Types> 
	struct IsSerializeTypeV<SerializeVariant<Types...>> : TrueType {};

	template<typename Type> struct IsSerializeI8V : FalseType {};
	template<> struct IsSerializeI8V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI8 = IsSerializeI8V<Type>::value;
	template<typename Type> concept IsSerializeI8C = IsSerializeI8<Type>;

	template<typename Type> struct IsSerializeI16V : FalseType {};
	template<> struct IsSerializeI16V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI16 = IsSerializeI16V<Type>::value;
	template<typename Type> concept IsSerializeI16C = IsSerializeI16<Type>;

	template<typename Type> struct IsSerializeI32V : FalseType {};
	template<> struct IsSerializeI32V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI32 = IsSerializeI32V<Type>::value;
	template<typename Type> concept IsSerializeI32C = IsSerializeI32<Type>;

	template<typename Type> struct IsSerializeI64V : FalseType {};
	template<> struct IsSerializeI64V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI64 = IsSerializeI64V<Type>::value;
	template<typename Type> concept IsSerializeI64C = IsSerializeI64<Type>;

	template<typename Type> struct IsSerializeUI8V : FalseType {};
	template<> struct IsSerializeUI8V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI8 = IsSerializeUI8V<Type>::value;
	template<typename Type> concept IsSerializeUI8C = IsSerializeUI8<Type>;

	template<typename Type> struct IsSerializeUI16V : FalseType {};
	template<> struct IsSerializeUI16V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI16 = IsSerializeUI16V<Type>::value;
	template<typename Type> concept IsSerializeUI16C = IsSerializeUI16<Type>;

	template<typename Type> struct IsSerializeUI32V : FalseType {};
	template<> struct IsSerializeUI32V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI32 = IsSerializeUI32V<Type>::value;
	template<typename Type> concept IsSerializeUI32C = IsSerializeUI32<Type>;

	template<typename Type> struct IsSerializeUI64V : FalseType {};
	template<> struct IsSerializeUI64V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI64 = IsSerializeUI64V<Type>::value;
	template<typename Type> concept IsSerializeUI64C = IsSerializeUI64<Type>;

	template<typename Type> struct IsSerializeF32V : FalseType {};
	template<> struct IsSerializeF32V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeF32 = IsSerializeF32V<Type>::value;
	template<typename Type> concept IsSerializeF32C = IsSerializeF32<Type>;

	template<typename Type> struct IsSerializeF64V : FalseType {};
	template<> struct IsSerializeF64V<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeF64 = IsSerializeF64V<Type>::value;
	template<typename Type> concept IsSerializeF64C = IsSerializeF64<Type>;

	template<typename Type> struct IsSerializeBoolV : FalseType {};
	template<> struct IsSerializeBoolV<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeBool = IsSerializeBoolV<Type>::value;
	template<typename Type> concept IsSerializeBoolC = IsSerializeBool<Type>;

	template<typename Type> struct IsSerializeCharV : FalseType {};
	template<> struct IsSerializeCharV<SerializeChar> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeChar = IsSerializeCharV<Type>::value;
	template<typename Type> concept IsSerializeCharC = IsSerializeChar<Type>;

	template<typename Type> struct IsSerializeStrV : FalseType {};
	template<> struct IsSerializeStrV<SerializeStr> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeStr = IsSerializeStrV<Type>::value;
	template<typename Type> concept IsSerializeStrC = IsSerializeStr<Type>;

	template<typename Type> struct IsSerializeFileV : FalseType {};
	template<> struct IsSerializeFileV<SerializeFile> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeFile = IsSerializeFileV<Type>::value;
	template<typename Type> concept IsSerializeFileC = IsSerializeFile<Type>;
	
	template<typename Type> struct IsSerializeBlobV : FalseType {};
	template<> struct IsSerializeBlobV<SerializeBlob> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeBlob = IsSerializeBlobV<Type>::value;
	template<typename Type> concept IsSerializeBlobC = IsSerializeBlob<Type>;
	
	template<typename Type> struct IsSerializeOptionalV : FalseType {};
	template<typename SerializeType> struct IsSerializeOptionalV<SerializeOptional<SerializeType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeOptional = IsSerializeOptionalV<Type>::value;
	template<typename Type> concept IsSerializeOptionalC = IsSerializeOptional<Type>;

	template<typename Type> struct IsSerializeEnumV : FalseType {};
	template<typename BaseSerializeType> struct IsSerializeEnumV<SerializeEnum<BaseSerializeType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeEnum = IsSerializeEnumV<Type>::value;
	template<typename Type> concept IsSerializeEnumC = IsSerializeEnum<Type>;

	template<typename Type> struct IsSerializeArrayV : FalseType {};
	template<typename ElementType> struct IsSerializeArrayV<SerializeArray<ElementType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeArray = IsSerializeArrayV<Type>::value;
	template<typename Type> concept IsSerializeArrayC = IsSerializeArray<Type>;

	template<typename Type> struct IsSerializeFixedArrayV : FalseType {};
	template<typename ElementType, Size Number> struct IsSerializeFixedArrayV<SerializeFixedArray<ElementType, Number>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeFixedArray = IsSerializeFixedArrayV<Type>::value;
	template<typename Type> concept IsSerializeFixedArrayC = IsSerializeFixedArray<Type>;

	template<typename Type> struct IsSerializeDicV : FalseType {};
	template<typename KeyType, typename ValueType> struct IsSerializeDicV<SerializeDic<KeyType, ValueType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeDic = IsSerializeDicV<Type>::value;
	template<typename Type> concept IsSerializeDicC = IsSerializeDic<Type>;

	template<typename Type> struct IsSerializeStructV : FalseType {};
	template<typename... MemberTypes> struct IsSerializeStructV<SerializeStruct<MemberTypes...>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeStruct = IsSerializeStructV<Type>::value;
	template<typename Type> concept IsSerializeStructC = IsSerializeStruct<Type>;

	template<typename Type> struct IsSerializeVariantV : FalseType {};
	template<typename... Types> struct IsSerializeVariantV<SerializeVariant<Types...>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeVariant = IsSerializeVariantV<Type>::value;
	template<typename Type> concept IsSerializeVariantC = IsSerializeVariant<Type>;

	template<typename Type> struct Serialize;

	template<typename Type> concept IsSerializableC = requires() { 
		typename Serialize<Type>::serialize_as_type;
		typename Serialize<Type>::type;
	};
	template<typename Type> struct IsSerializableV : BoolConstant<IsSerializableC<Type>> {};
	template<typename Type> constexpr Bool IsSerializable = IsSerializableV<Type>::value;

	template<typename Serilizer>
	concept IsSerializerC = requires(Serilizer & serilizer, const natl::ConstAsciiStringView& name) {
		{ serilizer.template beginWrite<natl::SerializeI8>(name) } -> natl::IsSameC<void>;
		{ serilizer.endWrite() } -> natl::IsSameC<void>;
		{ serilizer.writeValue() } -> natl::IsSameC<void>;
		{ serilizer.begin() } -> natl::IsSameC<void>;
		{ serilizer.end() } -> natl::IsSameC<void>;
	};

	template<typename Functor, typename EnumIntegerType>
	concept IsSerializeFlagToStringConvertFunctor = requires(Functor functor, const EnumIntegerType value) {
		{ functor(value) } -> IsStringViewLike<const Ascii>;
	};
	template<typename Functor, typename EnumIntegerType>
	concept IsStringToSerializeFlagConvertFunctor = requires(Functor functor, const natl::ConstAsciiStringView& value) {
		{ functor(value) } -> IsSameC<Option<EnumIntegerType>>;
	};

	template<typename SerializeType>
		requires(IsSerializableC<SerializeType>)
	using SerializeTypeOf = Serialize<SerializeType>::serialize_as_type;

	template<typename Serializer, typename SerializeType>
	struct CanSerializeV;
	template<typename Serializer, typename SerializeType>
	constexpr natl::Bool CanSerialize = CanSerializeV<Serializer, SerializeType>::value;
	template<typename Serializer, typename SerializeType>
	concept CanSerializeC = CanSerialize<Serializer, SerializeType>;

	template<typename Serializer>
	concept CanSerializeI8 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::i8 value) {
			{ serializer.writeI8(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeI16 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::i16 value) {
			{ serializer.writeI16(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeI32 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::i32 value) {
			{ serializer.writeI32(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeI64 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::i64 value) {
			{ serializer.writeI64(value) } -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeUI8 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::ui8 value) {
			{ serializer.writeUI8(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeUI16 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::ui16 value) {
			{ serializer.writeUI16(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeUI32 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::ui32 value) {
			{ serializer.writeUI32(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeUI64 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::ui64 value) {
			{ serializer.writeUI64(value) } -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeSize = CanSerializeUI64<Serializer>;

	template<typename Serializer>
	concept CanSerializeF32 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::f32 value) {
			{ serializer.writeF32(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeF64 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::f64 value) {
			{ serializer.writeF64(value) } -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeBool = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::Bool value) {
			{ serializer.writeBool(value) } -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeChar = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::Ascii value) {
			{ serializer.writeChar(value) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeStr = IsSerializerC<Serializer>
		&& requires(Serializer & serializer,
			const natl::Ascii * rawStr, const natl::Size size,
			const natl::ConstAsciiStringView str) {
				{ serializer.writeStr(rawStr, size) } -> natl::IsSameC<void>;
				{ serializer.writeStr(str) } -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeFile = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const natl::ConstAsciiStringView & fileName, const natl::ArrayView<const natl::Byte>&data) {
			{ serializer.writeFile(fileName, data) } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeBlob = IsSerializerC<Serializer>
		&& requires(Serializer & serializer,
			const natl::ArrayView<const natl::Byte>&data) {
				{ serializer.writeBlob(data) } -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeOptional = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.writeNull() } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeEnum = IsSerializerC<Serializer>
		&& requires(Serializer& serializer) {
			{
				serializer.template writeEnum<natl::Size>(
					natl::declval<natl::Size>(),
					[](const natl::Size&) -> natl::ConstAsciiStringView { return natl::declval<natl::ConstAsciiStringView>(); })
			} -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeFixedArray = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.beginWriteFixedArray() } -> natl::IsSameC<void>;
			{ serializer.endWriteFixedArray() } -> natl::IsSameC<void>;
			{ serializer.beginWriteFixedArrayElement() } -> natl::IsSameC<void>;
			{ serializer.endWriteFixedArrayElement() } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeArray = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.writeEmptyArray() } -> natl::IsSameC<void>;
			{ serializer.beginWriteArray() } -> natl::IsSameC<void>;
			{ serializer.endWriteArray() } -> natl::IsSameC<void>;
			{ serializer.beginWriteArrayElement() } -> natl::IsSameC<void>;
			{ serializer.endWriteArrayElement() } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeDic = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.writeEmptyDic() } -> natl::IsSameC<void>;
			{ serializer.beginWriteDic() } -> natl::IsSameC<void>;
			{ serializer.endWriteDic() } -> natl::IsSameC<void>;
			{ serializer.beginWriteDicElement() } -> natl::IsSameC<void>;
			{ serializer.endWriteDicElement() } -> natl::IsSameC<void>;
			{ serializer.writeDicValue() } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeStruct = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.beginWriteStruct() } -> natl::IsSameC<void>;
			{ serializer.endWriteStruct() } -> natl::IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeVariant = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.template beginWriteVariant<natl::SerializeI8>() } -> natl::IsSameC<void>;
			{ serializer.endWriteStruct() } -> natl::IsSameC<void>;
	};

	template<typename Serializer>
	concept IsFullSerializer = IsSerializerC<Serializer>
		&& CanSerializeI8<Serializer>
		&& CanSerializeI16<Serializer>
		&& CanSerializeI32<Serializer>
		&& CanSerializeI64<Serializer>
		&& CanSerializeUI8<Serializer>
		&& CanSerializeUI16<Serializer>
		&& CanSerializeUI32<Serializer>
		&& CanSerializeUI64<Serializer>
		&& CanSerializeF32<Serializer>
		&& CanSerializeF64<Serializer>
		&& CanSerializeBool<Serializer>
		&& CanSerializeChar<Serializer>
		&& CanSerializeStr<Serializer>
		&& CanSerializeOptional<Serializer>
		&& CanSerializeEnum<Serializer>
		&& CanSerializeFixedArray<Serializer>
		&& CanSerializeArray<Serializer>
		&& CanSerializeDic<Serializer>
		&& CanSerializeStruct<Serializer>
		&& CanSerializeVariant<Serializer>;

	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI8> : BoolConstant<CanSerializeI8<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI16> : BoolConstant<CanSerializeI16<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI32> : BoolConstant<CanSerializeI32<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI64> : BoolConstant<CanSerializeI64<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI8> : BoolConstant<CanSerializeUI8<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI16> : BoolConstant<CanSerializeUI16<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI32> : BoolConstant<CanSerializeUI32<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI64> : BoolConstant<CanSerializeUI64<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeF32> : BoolConstant<CanSerializeF32<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeF64> : BoolConstant<CanSerializeF64<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeBool> : BoolConstant<CanSerializeBool<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeChar> : BoolConstant<CanSerializeChar<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeStr> : BoolConstant<CanSerializeStr<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeFile> : BoolConstant<CanSerializeFile<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeBlob> : BoolConstant<CanSerializeBlob<Serializer>> {};

	template<typename Serializer, typename SerializeType>
	struct CanSerializeV<Serializer, SerializeOptional<SerializeType>>
		: BoolConstant<CanSerializeOptional<SerializeType> && CanSerialize<Serializer, SerializeType>> {};

	template<typename Serializer, typename BaseSerializeType>
	struct CanSerializeV<Serializer, SerializeEnum<BaseSerializeType>>
		: BoolConstant<CanSerializeEnum<Serializer> && CanSerializeC<Serializer, BaseSerializeType>> {};
	template<typename Serializer, typename ElementType, Size Number>
	struct CanSerializeV<Serializer, SerializeFixedArray<ElementType, Number>>
		: BoolConstant<CanSerializeFixedArray<Serializer>&& CanSerializeC<Serializer, ElementType>> {};
	template<typename Serializer, typename ElementType>
	struct CanSerializeV<Serializer, SerializeArray<ElementType>>
		: BoolConstant<CanSerializeArray<Serializer> && CanSerializeC<Serializer, ElementType>> {};
	template<typename Serializer, typename KeyType, typename ValueType>
	struct CanSerializeV<Serializer, SerializeDic<KeyType, ValueType>>
		: BoolConstant<CanSerializeDic<Serializer> && CanSerializeC<Serializer, KeyType> && CanSerializeC<Serializer, ValueType>> {};
	template<typename Serializer, typename... MemberTypes>
	struct CanSerializeV<Serializer, SerializeStruct<MemberTypes...>>
		: BoolConstant<CanSerializeStruct<Serializer> && (CanSerializeC<Serializer, MemberTypes> && ...)> {};
	template<typename Serializer, typename... Types>
	struct CanSerializeV<Serializer, SerializeVariant<Types...>>
		: BoolConstant<CanSerializeVariant<Serializer> && (CanSerializeC<Serializer, Types> && ...)> {};

	template<typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer> && IsSerializableC<Decay<Type>> && CanSerializeC<Serializer, typename Serialize<Decay<Type>>::serialize_as_type>)
	constexpr void serializeWrite(Serializer& serializer, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		Serialize<Decay<Type>>::template write<Serializer>(serializer, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer> && IsSerializableC<Decay<Type>>)
	constexpr void serializeWriteNamed(
		Serializer& serializer,
		const ConstAsciiStringView& name,
		const Type& serializeValue,
		SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using serialize_as_type = decayed_serialize_type::serialize_as_type;
		serializer.beginWrite<serialize_as_type>(name);
		serializer.writeValue();
		serializeWrite<Serializer, Type, SerializeArgs...>(serializer, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
		serializer.endWrite();
	}

	template<> struct Serialize<natl::i8> {
		using serialize_as_type = SerializeI8;
		using type = natl::i8;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i8 value) noexcept {
			serializer.writeI8(value);
		}
	};

	template<> struct Serialize<natl::i16> {
		using serialize_as_type = SerializeI16;
		using type = natl::i16;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i16 value) noexcept {
			serializer.writeI16(value);
		}
	};
	template<> struct Serialize<natl::i32> {
		using serialize_as_type = SerializeI32;
		using type = natl::i32;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i32 value) noexcept {
			serializer.writeI32(value);
		}
	};
	template<> struct Serialize<natl::i64> {
		using serialize_as_type = SerializeI64;
		using type = natl::i64;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::i64 value) noexcept {
			serializer.writeI64(value);
		}
	};

	template<> struct Serialize<natl::ui8> {
		using serialize_as_type = SerializeUI8;
		using type = natl::ui8;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui8 value) noexcept {
			serializer.writeUI8(value);
		}
	};
	template<> struct Serialize<natl::ui16> {
		using serialize_as_type = SerializeUI16;
		using type = natl::ui16;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui16 value) noexcept {
			serializer.writeUI16(value);
		}
	};
	template<> struct Serialize<natl::ui32> {
		using serialize_as_type = SerializeUI32;
		using type = natl::ui32;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui32 value) noexcept {
			serializer.writeUI32(value);
		}
	};
	template<> struct Serialize<natl::ui64> {
		using serialize_as_type = SerializeUI64;
		using type = natl::ui64;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ui64 value) noexcept {
			serializer.writeUI64(value);
		}
	};

	template<> struct Serialize<natl::f32> {
		using serialize_as_type = SerializeF32;
		using type = natl::f32;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::f32 value) noexcept {
			serializer.writeF32(value);
		}
	};
	template<> struct Serialize<natl::f64> {
		using serialize_as_type = SerializeF64;
		using type = natl::f64;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::f64 value) noexcept {
			serializer.writeF64(value);
		}
	};
	template<> struct Serialize<natl::Bool> {
		using serialize_as_type = SerializeBool;
		using type = natl::Bool;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::Bool value) noexcept {
			serializer.writeBool(value);
		}
	};

	template<> struct Serialize<natl::Ascii> {
		using serialize_as_type = SerializeChar;
		using type = natl::Ascii;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::Ascii value) noexcept {
			serializer.writeChar(value);
		}
	};

	template<> struct Serialize<char*> {
		using serialize_as_type = SerializeStr;
		using type = natl::Ascii*;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const char* str) noexcept {
			serializer.writeStr(str);
		}
	};
	template<> struct Serialize<natl::AsciiStringView> {
		using serialize_as_type = SerializeStr;
		using type = natl::AsciiStringView;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::AsciiStringView str) noexcept {
			serializer.writeStr(str);
		}
	};
	template<> struct Serialize<natl::ConstAsciiStringView> {
		using serialize_as_type = SerializeStr;
		using type = natl::ConstAsciiStringView;
		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const natl::ConstAsciiStringView str) noexcept {
			serializer.writeStr(str);
		}
	};

	//deserialize 
	struct DeserializeGlobalScope {};

	enum class DeserializeErrorFlag {
		endOfSource,
		wrongType,
		wrongFormatting,
		wrongName,
		valueParsing,
	};

	enum class DeserializeErrorLocation {
		beginReadNamed,
		endReadNamed,
		readI8,
		readI16,
		readI32,
		readI64,
		readUI8,
		readUI16,
		readUI32,
		readUI64,
		readF32,
		readF64,
		readBool,
		readChar,
		readStr,
		readFile,
		readBlob,
		readEnum,
		beginReadArray,
		endReadArray,
		beginReadArrayElement,
		endReadArrayElement,
		beginReadDic, 
		endReadDic,
		beginReadDicElement,
		endReadDicElement,
		readDicKey,
		readDicValue,
		beginReadStruct,
		endReadStruct,
		beginReadVariant,
		endReadVariant,
	};

	constexpr ConstAsciiStringView deserializeErrorLocationToString(const DeserializeErrorLocation errorLocation) noexcept {
		switch (errorLocation) {
		case DeserializeErrorLocation::beginReadNamed:
			return "beginReadNamed";
		case DeserializeErrorLocation::endReadNamed:
			return "endReadNamed";
		case DeserializeErrorLocation::readI8:
			return "readI8";
		case DeserializeErrorLocation::readI16:
			return "readI16";
		case DeserializeErrorLocation::readI32:
			return "readI32";
		case DeserializeErrorLocation::readI64:
			return "readI64";
		case DeserializeErrorLocation::readUI8:
			return "readUI8";
		case DeserializeErrorLocation::readUI16:
			return "readUI16";
		case DeserializeErrorLocation::readUI32:
			return "readUI32";
		case DeserializeErrorLocation::readUI64:
			return "readUI64";
		case DeserializeErrorLocation::readF32:
			return "readF32";
		case DeserializeErrorLocation::readF64:
			return "readF64";
		case DeserializeErrorLocation::readBool:
			return "readBool";
		case DeserializeErrorLocation::readChar:
			return "readChar";
		case DeserializeErrorLocation::readStr:
			return "readStr";
		case DeserializeErrorLocation::readFile:
			return "readFile";
		case DeserializeErrorLocation::readBlob:
			return "readBlob";
		case DeserializeErrorLocation::readEnum:
			return "readEnum";
		case DeserializeErrorLocation::beginReadArray:
			return "beginReadArray";
		case DeserializeErrorLocation::endReadArray:
			return "endReadArray";
		case DeserializeErrorLocation::beginReadArrayElement:
			return "beginReadArrayElement";
		case DeserializeErrorLocation::endReadArrayElement:
			return "endReadArrayElement";
		case DeserializeErrorLocation::beginReadDic:
			return "beginReadDic";
		case DeserializeErrorLocation::endReadDic:
			return "endReadDic";
		case DeserializeErrorLocation::beginReadDicElement:
			return "beginReadDicElement";
		case DeserializeErrorLocation::endReadDicElement:
			return "endReadDicElement";
		case DeserializeErrorLocation::readDicKey:
			return "readDicKey";
		case DeserializeErrorLocation::readDicValue:
			return "readDicValue";
		case DeserializeErrorLocation::beginReadStruct:
			return "beginReadStruct";
		case DeserializeErrorLocation::endReadStruct:
			return "endReadStruct";
		case DeserializeErrorLocation::beginReadVariant:
			return "beginReadVariant";
		case DeserializeErrorLocation::endReadVariant:
			return "endReadVariant";
		default:
			natl::unreachable();
		}
	}

	template<typename SerializeType>
	struct DummyDeserializeElementInfo {
	};

	struct DummyDeserializeErrorHandler {
		constexpr DummyDeserializeErrorHandler(
			const natl::ConstAsciiStringView& errorMessage, 
			const DeserializeErrorLocation errorLocation,
			const DeserializeErrorFlag& errorFlag) noexcept {}
		constexpr natl::Bool hasMessage() noexcept { return false; }
		constexpr natl::Size addSource(const natl::ConstAsciiStringView& name) noexcept { return 0; }
	};

	template<typename Type> struct Deserialize {};

	template<typename Type> concept IsDeserilizableC = requires() {
		typename Deserialize<Type>::deserialize_as_type;
		typename Deserialize<Type>::type;
	};
	template<typename Type> struct IsDeserilizableV : BoolConstant<IsDeserilizableC<Type>> {};
	template<typename Type> constexpr Bool IsDeserilizable = IsDeserilizableV<Type>::value;

	template<typename Type>
		requires(IsDeserilizableC<Type>)
	using DeserializeTypeOf = typename Deserialize<Type>::deserialize_as_type;

	template<typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type>)
	constexpr natl::Expect<Type, typename Deserializer::deserialize_error_handler> deserializeReadNamed(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		DeserializerArgs&&... deserializerArgs
	) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		using deserialize_as_type = type_deserialize::deserialize_as_type;

		auto readNamedInfoExpect = deserializer.template beginReadName<deserialize_as_type, ParentType>
			(parent, name, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (readNamedInfoExpect.doseNotHaveValue()) {
			return natl::unexpected(readNamedInfoExpect.error());
		}

		Type value;
		auto readValueError = type_deserialize::template read<Deserializer>(deserializer, readNamedInfoExpect, value);
		if (readValueError.hasValue()) {
			return natl::unexpected(readValueError.value());
		}

		auto endReadNameError = deserializer.endReadName<deserialize_as_type>(readNamedInfoExpect.value);
		if (endReadNameError.hasValue()) {
			return natl::unexpected(endReadNameError.value());
		}
		return value;
	}

	template<> struct Deserialize<natl::i8> {
		using deserialize_as_type = SerializeI8;
		using type = natl::i8;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::i8& dst) noexcept {
			auto valueExpect = deserializer.readI8();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::i16> {
		using deserialize_as_type = SerializeI16;
		using type = natl::i16;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::i16& dst) noexcept {
			auto valueExpect = deserializer.readI16();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::i32> {
		using deserialize_as_type = SerializeI8;
		using type = natl::i32;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::i32& dst) noexcept {
			auto valueExpect = deserializer.readI32();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::i64> {
		using deserialize_as_type = SerializeI64;
		using type = natl::i64;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::i64& dst) noexcept {
			auto valueExpect = deserializer.readI64();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::ui8> {
		using deserialize_as_type = SerializeUI8;
		using type = natl::ui8;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::ui8& dst) noexcept {
			auto valueExpect = deserializer.readUI8();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::ui16> {
		using deserialize_as_type = SerializeUI16;
		using type = natl::ui16;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::ui16& dst) noexcept {
			auto valueExpect = deserializer.readUI16();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::ui32> {
		using deserialize_as_type = SerializeUI32;
		using type = natl::ui32;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::ui32& dst) noexcept {
			auto valueExpect = deserializer.readUI32();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::ui64> {
		using deserialize_as_type = SerializeUI64;
		using type = natl::ui64;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::ui64& dst) noexcept {
			auto valueExpect = deserializer.readUI64();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::f32> {
		using deserialize_as_type = SerializeF32;
		using type = natl::f32;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::f32& dst) noexcept {
			auto valueExpect = deserializer.readF32();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::f64> {
		using deserialize_as_type = SerializeF64;
		using type = natl::f64;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::f64& dst) noexcept {
			auto valueExpect = deserializer.readF64();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::Bool> {
		using deserialize_as_type = SerializeBool;
		using type = natl::Bool;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::Bool& dst) noexcept {
			auto valueExpect = deserializer.readBool();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};

	template<> struct Deserialize<natl::Ascii> {
		using deserialize_as_type = SerializeChar;
		using type = natl::Ascii;

		template<typename Deserializer>
		constexpr static natl::Option<typename Deserializer::deserialize_error_handler>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<deserialize_as_type>& info,
				natl::Ascii& dst) noexcept {
			auto valueExpect = deserializer.readChar();
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error();
			}
		}
	};
}