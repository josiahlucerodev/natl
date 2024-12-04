#pragma once 

//own
#include "../util/compilerDependent.h"
#include "../container/stringView.h"
#include "../container/arrayView.h"
#include "../fundamental/expect.h"

//interface 
namespace natl {
	enum class SerializeFlag {
		pretty,
		compress,
	};


	struct DummySerializer {
		using allocator_type = DefaultAllocator<natl::Ascii>;
		using code_point_type = natl::Ascii;
		using container_type = void;

		constexpr static inline natl::Size smallBufferSize = 0;
		constexpr static inline natl::SerializeFlag flag = natl::SerializeFlag::compress;
	};

	template<typename Type> struct Serialize;

	template<typename Type> concept IsSerializableC = requires() {
		typename Serialize<Decay<Type>>;
		typename Serialize<Decay<Type>>::as_type;
		typename Serialize<Decay<Type>>::type;
		typename Serialize<Decay<Type>>::template error_type<DummySerializer>;
	};
	template<typename Type> struct IsSerializableV : BoolConstant<IsSerializableC<Type>> {};
	template<typename Type> constexpr Bool IsSerializable = IsSerializableV<Type>::value;

	template<typename SerializeType>
		requires(IsSerializableC<SerializeType>)
	using SerializeTypeOf = Serialize<SerializeType>::as_type;

	template<typename Type> struct IsSerializeTypeV : FalseType {};
	template<typename Type> constexpr inline Bool IsSerializeType = IsSerializeTypeV<Type>::value;
	template<typename Type> concept IsSerializeTypeC = IsSerializeType<Type>;

	template<typename Type> struct IsEnumBaseSerializeTypeV : FalseType {};
	template<typename Type> constexpr inline Bool IsEnumBaseSerializeType = IsEnumBaseSerializeTypeV<Type>::value;
	template<typename Type> concept IsEnumBaseSerializeTypeC = IsEnumBaseSerializeType<Type>;

	template<typename Type> struct IsVariantIndexSerializeTypeV : FalseType {};
	template<typename Type> constexpr inline Bool IsVariantIndexSerializeType = IsVariantIndexSerializeTypeV<Type>::value;
	template<typename Type> concept IsVariantIndexSerializeTypeC = IsVariantIndexSerializeType<Type>;

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

	template<typename Type>
		requires(IsSerializableC<Type>)
	struct SerializeOptional {
		/*todo*/ using type = Type;
		using serialize_type = SerializeTypeOf<Type>;
	};

	template<typename BaseType>
		requires(IsSerializableC<BaseType>)
	struct SerializeEnum {
		/*todo*/ using base_type = BaseType;
		using base_serialize_type = SerializeTypeOf<BaseType>;
	};

	template<typename ElementType, Size Number>
		requires(IsSerializableC<ElementType>)
	struct SerializeFixedArray {
		/*todo*/ using element_type = ElementType;
		using element_serialize_type = SerializeTypeOf<ElementType>;
		constexpr static inline Size size = Number;
	};

	template<typename ElementType>
		requires(IsSerializableC<ElementType>)
	struct SerializeArray {
		/*todo*/ using element_type = ElementType;
		using element_serialize_type = SerializeTypeOf<ElementType>;
	};

	template<typename KeyType, typename ValueType>
		requires(IsSerializableC<KeyType> && IsSerializableC<ValueType>)
	struct SerializeDic {
		/*todo*/ using key_type = KeyType;
		using key_serialize_type = SerializeTypeOf<KeyType>;
		/*todo*/ using value_type = ValueType;
		using value_serialize_type = SerializeTypeOf<ValueType>;
	};

	template<typename KeyType, typename ValueType>
		requires(IsSerializableC<KeyType>&& IsSerializableC<ValueType>)
	struct SerializeDicElement {
		/*todo*/ using key_type = KeyType;
		using key_serialize_type = SerializeTypeOf<KeyType>;
		/*todo*/ using value_type = ValueType;
		using value_serialize_type = SerializeTypeOf<ValueType>;
	};

	template<typename... MemberTypes>
		requires(IsSerializableC<MemberTypes> && ...)
	struct SerializeStruct { 
		/*todo*/ using members = TypePack<MemberTypes...>; 
		using members_serialize_types = TypePack<SerializeTypeOf<MemberTypes>...>;
	};

	template<typename IndexType, typename... Types>
		requires(IsSerializableC<IndexType> 
			&& IsVariantIndexSerializeTypeC<SerializeTypeOf<IndexType>> 
			&& (IsSerializableC<Types> && ...))
	struct SerializeVariant { 
		/*todo*/ using types = TypePack<Types...>; 
		using serialize_types = TypePack<SerializeTypeOf<Types>...>;
		/*todo*/ using index_type = IndexType;
		using index_serialize_type = SerializeTypeOf<IndexType>;
	};

	template<> struct IsEnumBaseSerializeTypeV<SerializeI8> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeI16> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeI32> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeI64> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI8> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI16> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI32> : TrueType {};
	template<> struct IsEnumBaseSerializeTypeV<SerializeUI64> : TrueType {};

	template<> struct IsVariantIndexSerializeTypeV<SerializeI8> : TrueType {};
	template<> struct IsVariantIndexSerializeTypeV<SerializeI16> : TrueType {};
	template<> struct IsVariantIndexSerializeTypeV<SerializeI32> : TrueType {};
	template<> struct IsVariantIndexSerializeTypeV<SerializeI64> : TrueType {};
	template<> struct IsVariantIndexSerializeTypeV<SerializeUI8> : TrueType {};
	template<> struct IsVariantIndexSerializeTypeV<SerializeUI16> : TrueType {};
	template<> struct IsVariantIndexSerializeTypeV<SerializeUI32> : TrueType {};
	template<> struct IsVariantIndexSerializeTypeV<SerializeUI64> : TrueType {};

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

	template<typename Type> 
	struct IsSerializeTypeV<SerializeOptional<Type>> : TrueType {};
	template<typename BaseType> 
	struct IsSerializeTypeV<SerializeEnum<BaseType>> : TrueType {};
	template<typename ElementType, Size Number> 
	struct IsSerializeTypeV<SerializeFixedArray<ElementType, Number>> : TrueType {};
	template<typename ElementType> 
	struct IsSerializeTypeV<SerializeArray<ElementType>> : TrueType {};
	template<typename KeyType, typename ValueType> 
	struct IsSerializeTypeV<SerializeDic<KeyType, ValueType>> : TrueType {};
	template<typename... MemberTypes> 
	struct IsSerializeTypeV<SerializeStruct<MemberTypes...>> : TrueType {};
	template<typename IndexType, typename... Types> 
	struct IsSerializeTypeV<SerializeVariant<IndexType, Types...>> : TrueType {};

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
	template<typename IndexType, typename... Types> struct IsSerializeVariantV<SerializeVariant<IndexType, Types...>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeVariant = IsSerializeVariantV<Type>::value;
	template<typename Type> concept IsSerializeVariantC = IsSerializeVariant<Type>;

	template<typename Serilizer>
	concept IsSerializerC = requires(Serilizer & serilizer, const ConstAsciiStringView& name) {
		{ serilizer.template beginWrite<SerializeI8>(name) } -> IsSameC<void>;
		{ serilizer.endWrite() } -> IsSameC<void>;
		{ serilizer.writeValue() } -> IsSameC<void>;
		{ serilizer.begin() } -> IsSameC<void>;
		{ serilizer.end() } -> IsSameC<void>;
	};

	template<typename Functor, typename EnumIntegerType>
	concept IsStringToSerializeFlagConvertFunctor = requires(Functor functor, const ConstAsciiStringView& value) {
		{ functor(value) } -> IsSameC<Option<EnumIntegerType>>;
	};
	template<typename Functor, typename IndexIntegerType>
	concept IsStringToSerializeVariantIndexConvertFunctor = requires(Functor functor, const ConstAsciiStringView& str) {
		{ functor(str) } -> IsSameC<Option<IndexIntegerType>>;
	};

	template<typename Serializer, typename SerializeType>
	struct CanSerializeV : FalseType {};
	template<typename Serializer, typename SerializeType>
	constexpr Bool CanSerialize = CanSerializeV<Serializer, SerializeType>::value;
	template<typename Serializer, typename SerializeType>
	concept CanSerializeC = CanSerialize<Serializer, SerializeType>;

	template<typename Serializer, typename Type>
	using SerializeErrorOf = Serialize<Decay<Type>>::template error_type<Serializer>;

	template<typename Serializer, typename Type>
	using SerializeErrorResult = Conditional<IsSameC<SerializeErrorOf<Serializer, Type>, void>, void, Option<SerializeErrorOf<Serializer, Type>>>;

	template<typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer> 
			&& IsSerializableC<Decay<Type>> 
			&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
	constexpr SerializeErrorResult<Serializer, Type> serializeWrite(Serializer& serializer, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			Serialize<Decay<Type>>::template write<Serializer>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		} else {
			return Serialize<Decay<Type>>::template write<Serializer>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		}
	}

	template<typename SerilizeType, typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>
	&& IsSerializableC<Decay<Type>>
		&& IsSerializeTypeC<SerilizeType>
		&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>
		&& natl::IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWriteMatch(
			Serializer& serializer, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		} else {
			return serializeWrite<Serializer, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		}
	}

	template<typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>
		&& IsSerializableC<Decay<Type>>
		&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamed(
			Serializer& serializer,
			const ConstAsciiStringView& name,
			const Type& serializeValue,
			SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using as_type = decayed_serialize_type::as_type;
		serializer.template beginWrite<as_type>(name);
		serializer.writeValue();

		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.endWrite();
		} else {
			SerializeErrorResult<Serializer, Type> result =
				serializeWrite<Serializer, Type, SerializeArgs...>(
					serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.endWrite();
			return result;
		}
	}

	template<typename SerilizeType, typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>
		&& IsSerializableC<Decay<Type>>
		&& IsSerializeTypeC<SerilizeType>
		&& CanSerializeC<Serializer, Decay<Type>>
		&& natl::IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedMatch(
			Serializer& serializer,
			const ConstAsciiStringView& name,
			const Type& serializeValue,
			SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using as_type = decayed_serialize_type::as_type;
		serializer.template beginWrite<as_type>(name);
		serializer.writeValue();

		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.endWrite();
		} else {
			SerializeErrorResult<Serializer, Type> result =
				serializeWrite<Serializer, Type, SerializeArgs...>(
					serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.endWrite();
			return result;
		}
	}

	template<> struct Serialize<i8> {
		using as_type = SerializeI8;
		using type = i8;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const i8 value) noexcept {
			serializer.writeI8(value);
		}
	};

	template<> struct Serialize<i16> {
		using as_type = SerializeI16;
		using type = i16;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const i16 value) noexcept {
			serializer.writeI16(value);
		}
	};
	template<> struct Serialize<i32> {
		using as_type = SerializeI32;
		using type = i32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const i32 value) noexcept {
			serializer.writeI32(value);
		}
	};
	template<> struct Serialize<i64> {
		using as_type = SerializeI64;
		using type = i64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const i64 value) noexcept {
			serializer.writeI64(value);
		}
	};

	template<> struct Serialize<ui8> {
		using as_type = SerializeUI8;
		using type = ui8;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const ui8 value) noexcept {
			serializer.writeUI8(value);
		}
	};
	template<> struct Serialize<ui16> {
		using as_type = SerializeUI16;
		using type = ui16;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const ui16 value) noexcept {
			serializer.writeUI16(value);
		}
	};
	template<> struct Serialize<ui32> {
		using as_type = SerializeUI32;
		using type = ui32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const ui32 value) noexcept {
			serializer.writeUI32(value);
		}
	};
	template<> struct Serialize<ui64> {
		using as_type = SerializeUI64;
		using type = ui64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const ui64 value) noexcept {
			serializer.writeUI64(value);
		}
	};

	template<> struct Serialize<f32> {
		using as_type = SerializeF32;
		using type = f32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const f32 value) noexcept {
			serializer.writeF32(value);
		}
	};
	template<> struct Serialize<f64> {
		using as_type = SerializeF64;
		using type = f64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const f64 value) noexcept {
			serializer.writeF64(value);
		}
	};
	template<> struct Serialize<Bool> {
		using as_type = SerializeBool;
		using type = Bool;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const Bool value) noexcept {
			serializer.writeBool(value);
		}
	};

	template<> struct Serialize<Ascii> {
		using as_type = SerializeChar;
		using type = Ascii;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const Ascii value) noexcept {
			serializer.writeChar(value);
		}
	};

	template<> struct Serialize<char*> {
		using as_type = SerializeStr;
		using type = Ascii*;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const char* str) noexcept {
			serializer.writeStr(str);
		}
	};
	template<> struct Serialize<AsciiStringView> {
		using as_type = SerializeStr;
		using type = AsciiStringView;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const AsciiStringView str) noexcept {
			serializer.writeStr(str);
		}
	};
	template<> struct Serialize<ConstAsciiStringView> {
		using as_type = SerializeStr;
		using type = ConstAsciiStringView;
		template<typename Serializer> using error_type = void;

		template<typename Serializer>
		constexpr static void write(Serializer& serializer, const ConstAsciiStringView str) noexcept {
			serializer.writeStr(str);
		}
	};

	template<class ElementType>
		requires(IsSerializableC<Decay<ElementType>>)
	struct Serialize<ArrayView<ElementType>> {
		using as_type = SerializeArray<Decay<ElementType>>;
		using type = ArrayView<ElementType>;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, typename... ElementSerializeArgs>
		constexpr static void write(Serializer& serializer, const type& arrayView, ElementSerializeArgs&&... elementSerializeArgs) noexcept {
			if (arrayView.isEmpty()) {
				serializer.writeEmptyArray();
			} else {
				serializer.beginWriteArray();
				for (Size i = 0; i < arrayView.size(); i++) {
					serializer.beginWriteArrayElement();
					serializeWrite(serializer, arrayView[i], forward<ElementSerializeArgs>(elementSerializeArgs)...);
					serializer.endWriteArrayElement();
				}
				serializer.endWriteArray();
			}
		}
	};

	template<typename Serializer, typename SerializeType>
		requires(IsSerializableC<SerializeType>)
	struct CanSerializeV<Serializer, SerializeType> : CanSerializeV<Serializer, SerializeTypeOf<SerializeType>> {};

	template<typename Serializer>
	concept CanSerializeI8 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i8 value) {
			{ serializer.writeI8(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeI16 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i16 value) {
			{ serializer.writeI16(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeI32 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i32 value) {
			{ serializer.writeI32(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeI64 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i64 value) {
			{ serializer.writeI64(value) } -> IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeUI8 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui8 value) {
			{ serializer.writeUI8(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeUI16 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui16 value) {
			{ serializer.writeUI16(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeUI32 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui32 value) {
			{ serializer.writeUI32(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeUI64 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui64 value) {
			{ serializer.writeUI64(value) } -> IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeSize = CanSerializeUI64<Serializer>;

	template<typename Serializer>
	concept CanSerializeF32 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const f32 value) {
			{ serializer.writeF32(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeF64 = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const f64 value) {
			{ serializer.writeF64(value) } -> IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeBool = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const Bool value) {
			{ serializer.writeBool(value) } -> IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeChar = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const Ascii value) {
			{ serializer.writeChar(value) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeStr = IsSerializerC<Serializer>
		&& requires(Serializer & serializer,
			const Ascii * rawStr, const Size size,
			const ConstAsciiStringView str) {
				{ serializer.writeStr(rawStr, size) } -> IsSameC<void>;
				{ serializer.writeStr(str) } -> IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeFile = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ConstAsciiStringView & fileName, const ArrayView<const Byte>&data) {
			{ serializer.writeFile(fileName, data) } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeBlob = IsSerializerC<Serializer>
		&& requires(Serializer & serializer,
			const ArrayView<const Byte>&data) {
				{ serializer.writeBlob(data) } -> IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeOptional = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.writeNull() } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeEnum = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template writeEnum<SerializeUI64>(
					declval<Size>(),
					declval<ConstAsciiStringView>())
			} -> IsSameC<void>;
	};

	template<typename Serializer>
	concept CanSerializeFixedArray = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.beginWriteFixedArray() } -> IsSameC<void>;
			{ serializer.endWriteFixedArray() } -> IsSameC<void>;
			{ serializer.beginWriteFixedArrayElement() } -> IsSameC<void>;
			{ serializer.endWriteFixedArrayElement() } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeArray = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.writeEmptyArray() } -> IsSameC<void>;
			{ serializer.beginWriteArray() } -> IsSameC<void>;
			{ serializer.endWriteArray() } -> IsSameC<void>;
			{ serializer.beginWriteArrayElement() } -> IsSameC<void>;
			{ serializer.endWriteArrayElement() } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeDic = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.writeEmptyDic() } -> IsSameC<void>;
			{ serializer.beginWriteDic() } -> IsSameC<void>;
			{ serializer.endWriteDic() } -> IsSameC<void>;
			{ serializer.beginWriteDicElement() } -> IsSameC<void>;
			{ serializer.endWriteDicElement() } -> IsSameC<void>;
			{ serializer.writeDicValue() } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeStruct = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{ serializer.beginWriteStruct() } -> IsSameC<void>;
			{ serializer.endWriteStruct() } -> IsSameC<void>;
	};
	template<typename Serializer>
	concept CanSerializeVariant = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template beginWriteVariant<SerializeVariant<natl::ui8, natl::ui8>, 0>(
					declval<ConstAsciiStringView>())
			} -> IsSameC<void>;
			{ serializer.endWriteStruct() } -> IsSameC<void>;
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
		: BoolConstant<CanSerializeOptional<SerializeType>&& CanSerialize<Serializer, SerializeType>> {
	};

	template<typename Serializer, typename BaseSerializeType>
	struct CanSerializeV<Serializer, SerializeEnum<BaseSerializeType>>
		: BoolConstant<CanSerializeEnum<Serializer>&& CanSerializeC<Serializer, BaseSerializeType>> {
	};
	template<typename Serializer, typename ElementType, Size Number>
	struct CanSerializeV<Serializer, SerializeFixedArray<ElementType, Number>>
		: BoolConstant<CanSerializeFixedArray<Serializer>&& CanSerializeC<Serializer, ElementType>> {
	};
	template<typename Serializer, typename ElementType>
	struct CanSerializeV<Serializer, SerializeArray<ElementType>>
		: BoolConstant<CanSerializeArray<Serializer>&& CanSerializeC<Serializer, ElementType>> {
	};
	template<typename Serializer, typename KeyType, typename ValueType>
	struct CanSerializeV<Serializer, SerializeDic<KeyType, ValueType>>
		: BoolConstant<CanSerializeDic<Serializer>&& CanSerializeC<Serializer, KeyType>&& CanSerializeC<Serializer, ValueType>> {
	};
	template<typename Serializer, typename... MemberTypes>
	struct CanSerializeV<Serializer, SerializeStruct<MemberTypes...>>
		: BoolConstant<CanSerializeStruct<Serializer> && (CanSerializeC<Serializer, MemberTypes> && ...)> {
	};
	template<typename Serializer, typename IndexType, typename... Types>
	struct CanSerializeV<Serializer, SerializeVariant<IndexType, Types...>>
		: BoolConstant<CanSerializeVariant<Serializer> && (CanSerializeC<Serializer, Types> && ...)> {
	};

	//deserialize 
	enum class ErrorHandlingFlag {
		shorten,
		full,
	};

	struct DeserializeGlobalScope {};

	enum class DeserializeErrorFlag {
		none = 0,
		endOfSource,
		wrongType,
		wrongFormatting,
		wrongName,
		valueParsing,
	};

	constexpr ConstAsciiStringView deserializeErrorFlagToString(const DeserializeErrorFlag errorFlag) noexcept {
		switch (errorFlag) {
		case DeserializeErrorFlag::none:
			return "none";
		case DeserializeErrorFlag::endOfSource:
			return "end of source";
		case DeserializeErrorFlag::wrongType:
			return "wrong type";
		case DeserializeErrorFlag::wrongFormatting:
			return "wrong formatting";
		case DeserializeErrorFlag::wrongName:
			return "wrong name";
		case DeserializeErrorFlag::valueParsing:
			return "value parsing";
		default:
			natl::unreachable();
		}
	}

	enum class DeserializeErrorLocation {
		none = 0,
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
		readIsOptionalNull,
		readAsOptional,
		readEnum,
		beginReadArray,
		endReadArray,
		endReadEmptyArray,
		beginReadArrayElement,
		endReadArrayElement,
		beginReadDic, 
		endReadDic,
		endReadEmptyDic,
		beginReadDicElement,
		endReadDicElement,
		readDicKey,
		readDicValue,
		beginReadStruct,
		endReadStruct,
		readIsEmptyVariant,
		beginReadVariantGetIndex,
		beginReadVariantOfType,
		endReadVariant,
	};

	constexpr ConstAsciiStringView deserializeErrorLocationToString(const DeserializeErrorLocation errorLocation) noexcept {
		switch (errorLocation) {
		case DeserializeErrorLocation::none:
			return "none";
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
		case DeserializeErrorLocation::readIsOptionalNull:
			return "readIsOptionalNull";
		case DeserializeErrorLocation::readAsOptional:
			return "readAsOptional";
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
		case DeserializeErrorLocation::endReadEmptyArray:
			return "endReadEmptyArray";
		case DeserializeErrorLocation::beginReadDic:
			return "beginReadDic";
		case DeserializeErrorLocation::endReadDic:
			return "endReadDic";
		case DeserializeErrorLocation::beginReadDicElement:
			return "beginReadDicElement";
		case DeserializeErrorLocation::endReadDicElement:
			return "endReadDicElement";
		case DeserializeErrorLocation::endReadEmptyDic:
			return "endReadEmptyDic";
		case DeserializeErrorLocation::readDicKey:
			return "readDicKey";
		case DeserializeErrorLocation::readDicValue:
			return "readDicValue";
		case DeserializeErrorLocation::beginReadStruct:
			return "beginReadStruct";
		case DeserializeErrorLocation::endReadStruct:
			return "endReadStruct";
		case DeserializeErrorLocation::readIsEmptyVariant:
			return "readIsEmptyVariant";
		case DeserializeErrorLocation::beginReadVariantGetIndex:
			return "beginReadVariantGetIndex";
		case DeserializeErrorLocation::beginReadVariantOfType:
			return "beginReadVariantOfType";
		case DeserializeErrorLocation::endReadVariant:
			return "endReadVariant";
		default:
			natl::unreachable();
		}
	}

	template<typename SerializeType>
	struct DummyDeserializeElementInfo {
		using serialize_type = SerializeType;
	};

	struct DummyDeserializeErrorHandler {
		constexpr DummyDeserializeErrorHandler(
			const ConstAsciiStringView&,  const ConstAsciiStringView&,
			const DeserializeErrorLocation&, const DeserializeErrorFlag&) noexcept {}
		constexpr Bool hasMessage() noexcept { return false; }
		constexpr DummyDeserializeErrorHandler& addSource(
			const ConstAsciiStringView&, const ConstAsciiStringView&) noexcept { return *this; }
		template<typename DynStringType>
		constexpr void getErrorMessage(DynStringType&) {}
	};

	struct DummyDeserializer {
		template<typename SerializeType>
		using deserialize_info = DummyDeserializeElementInfo<SerializeType>;
		using deserialize_error_handler = DummyDeserializeErrorHandler;
		using error_message_string_type = void;
	};

	template<typename Type> struct Deserialize {};

	template<typename Type> concept IsDeserilizableC = requires() {
		typename Deserialize<Decay<Type>>;
		typename Deserialize<Decay<Type>>::as_type;
		typename Deserialize<Decay<Type>>::type;
		typename Deserialize<Decay<Type>>::template error_type<DummyDeserializer>;
	};
	template<typename Type> struct IsDeserilizableV : BoolConstant<IsDeserilizableC<Type>> {};
	template<typename Type> constexpr Bool IsDeserilizable = IsDeserilizableV<Type>::value;

	template<typename Type>
		requires(IsDeserilizableC<Type>)
	using DeserializeTypeOf = typename Deserialize<Type>::as_type;

	template<typename Deserializer>
	using StandardDeserializeError = Deserializer::deserialize_error_handler;

	template<typename Deserializer, typename Type> constexpr inline Bool IsStandardDeserializeError = IsSameC<
		typename Decay<Deserializer>::deserialize_error_handler, Type>;
	template<typename Deserializer, typename Type> struct IsStandardDeserializeErrorV 
		: BoolConstant<IsStandardDeserializeError<Deserializer, Type>> {};
	template<typename Deserializer, typename Type> concept IsStandardDeserializeErrorC = IsStandardDeserializeError<Deserializer, Type>;

	template<typename Deserializer, typename ErrorType>
	struct DeserializeError {
		using deserializer = Deserializer;
		using error_type = ErrorType;
		using deserialize_error_handler = Deserializer::deserialize_error_handler;

		deserialize_error_handler errorHandler;
		ErrorType errorType;

		constexpr Bool hasMessage() noexcept { return errorHandler.hasMessage(); }
		constexpr DeserializeError& addSource(const ConstAsciiStringView& name) noexcept {
			errorHandler.addSource(name);
		}
	};

	template<typename Type> struct IsDeserializeErrorV : FalseType {};
	template<typename Type> constexpr inline Bool IsDeserializeError = IsDeserializeErrorV<Type>::value;
	template<typename Type> concept IsDeserializeErrorC = IsDeserializeError<Type>;
	template<typename Deserializer, typename ErrorType> 
	struct IsDeserializeErrorV<DeserializeError<Deserializer, ErrorType>> : TrueType {};

	template<typename Deserializer, typename Type>
	using DeserializeErrorOf = typename Deserialize<Decay<Type>>::template error_type<Deserializer>;

	template<typename Deserializer, typename Type, typename ParentSerializeType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type> && IsSerializeTypeC<ParentSerializeType>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeRead(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
		DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "natl::deserializeRead";

		Type value;
		auto readValueError = type_deserialize::template read<Deserializer>(
			deserializer, parent, value, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return unexpected(readValueError.value().addSource(sourceName, ""));
		}
		return value;
	}

	template<typename SerilizeType, typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type>&& natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		DeserializerArgs&&... deserializerArgs) {
		return deserializeRead<Deserializer, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}


	template<typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDst(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "natl::deserializeReadToDst";

		auto readValueError = type_deserialize::template read<Deserializer>(
			deserializer, parent, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return readValueError.value().addSource(sourceName, "");
		}
		return {};
	}

	template<typename SerilizeType, typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type>&& natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDstMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadToDstMatch<Deserializer, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamed(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		DeserializerArgs&&... deserializerArgs
	) noexcept {
		constexpr ConstAsciiStringView sourceName = "natl::deserializeReadNamed";

		auto readNamedInfoExpect = deserializer.template beginReadNamed<Type, ParentType>(parent, name);
		if (readNamedInfoExpect.hasError()) {
			return unexpected(readNamedInfoExpect.error().addSource(sourceName, name));
		}
		auto readNamedInfo = readNamedInfoExpect.value();

		auto valueExpect = deserializeRead<Deserializer, Type, typename decltype(readNamedInfo)::serialize_type, DeserializerArgs...>(
			deserializer, readNamedInfo, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (valueExpect.hasError()) {
			return unexpected(valueExpect.error().addSource(sourceName, name));
		}

		auto endReadNameError = deserializer.template endReadNamed<SerializeTypeOf<Type>>(readNamedInfo);
		if (endReadNameError.hasValue()) {
			return unexpected(endReadNameError.value().addSource(sourceName, name));
		}

		return valueExpect.value();
	}

	template<typename SerilizeType, typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>>&& natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		DeserializerArgs&&... deserializerArgs
	) noexcept {
		return deserializeReadNamed<Deserializer, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDst(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		constexpr ConstAsciiStringView sourceName = "natl::deserializeReadNamedToDst";

		auto readNamedInfoExpect = deserializer.template beginReadNamed<Type, ParentType>(parent, name);
		if (readNamedInfoExpect.hasError()) {
			return readNamedInfoExpect.error().addSource(sourceName, name);
		}
		auto readNamedInfo = readNamedInfoExpect.value();

		auto error = deserializeReadToDst<Deserializer, Type, typename decltype(readNamedInfo)::serialize_type, DeserializerArgs...>(
			deserializer, readNamedInfo, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (error.hasValue()) {
			return error.value().addSource(sourceName, name);
		}

		auto endReadNameError = deserializer.template endReadNamed<SerializeTypeOf<Type>>(readNamedInfo);
		if (endReadNameError.hasValue()) {
			return endReadNameError.value().addSource(sourceName, name);
		}

		return {};
	}

	template<typename SerilizeType, typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>> && natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDstMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<> struct Deserialize<i8> {
		using as_type = SerializeI8;
		using type = i8;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<i8>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i8& dst) noexcept {
			auto valueExpect = deserializer.readI8(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<i16> {
		using as_type = SerializeI16;
		using type = i16;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<i16>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i16& dst) noexcept {
			auto valueExpect = deserializer.readI16(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<i32> {
		using as_type = SerializeI8;
		using type = i32;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<i32>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i32& dst) noexcept {
			auto valueExpect = deserializer.readI32(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<i64> {
		using as_type = SerializeI64;
		using type = i64;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<i64>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i64& dst) noexcept {
			auto valueExpect = deserializer.readI64(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui8> {
		using as_type = SerializeUI8;
		using type = ui8;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<ui8>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui8& dst) noexcept {
			auto valueExpect = deserializer.readUI8(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui16> {
		using as_type = SerializeUI16;
		using type = ui16;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<ui16>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui16& dst) noexcept {
			auto valueExpect = deserializer.readUI16(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui32> {
		using as_type = SerializeUI32;
		using type = ui32;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<ui32>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui32& dst) noexcept {
			auto valueExpect = deserializer.readUI32(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui64> {
		using as_type = SerializeUI64;
		using type = ui64;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<ui64>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui64& dst) noexcept {
			auto valueExpect = deserializer.readUI64(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<f32> {
		using as_type = SerializeF32;
		using type = f32;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<f32>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				f32& dst) noexcept {
			auto valueExpect = deserializer.readF32(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<f64> {
		using as_type = SerializeF64;
		using type = f64;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<f64>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				f64& dst) noexcept {
			auto valueExpect = deserializer.readF64(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<Bool> {
		using as_type = SerializeBool;
		using type = Bool;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<Bool>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				Bool& dst) noexcept {
			auto valueExpect = deserializer.readBool(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<Ascii> {
		using as_type = SerializeChar;
		using type = Ascii;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<Ascii>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				Ascii& dst) noexcept {
			auto valueExpect = deserializer.readChar(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	namespace impl {
		template<typename ErrorType, typename Deserializer, typename DerializeInfoType, typename ReadFunctor>
		concept IsDeserializeReadFunctorWithArgs = requires(natl::Option<ErrorType> error, Deserializer& deserializer,
			DerializeInfoType& info, ReadFunctor&& readFunctor) {
			{ error = forward<ReadFunctor>(readFunctor)(deserializer, info) };
		};

		template<typename ErrorType, typename ReadFunctor>
		concept  IsDeserializeReadFunctorWithoutArgs = requires(natl::Option<ErrorType> error, ReadFunctor&& readFunctor) {
				{ error = forward<ReadFunctor>(readFunctor)() };
		};

		template<typename ErrorType, typename Deserializer, typename DerializeInfoType, typename ReadFunctor>
		concept IsDeserializeReadFunctor = 
			IsDeserializeReadFunctorWithArgs<ErrorType, Deserializer, DerializeInfoType, ReadFunctor> ||
			IsDeserializeReadFunctorWithoutArgs<ErrorType, ReadFunctor>;

		template<typename ErrorType, typename Deserializer, typename DerializeInfoType>
		constexpr void chainDeserializeReadHelper(
			Option<ErrorType>&, const natl::ConstAsciiStringView&, Deserializer&,
			DerializeInfoType&) {}

		template<typename ErrorType, typename Deserializer, typename DerializeInfoType,
			typename ReadFunctor, typename... OtherReadFunctors>
		constexpr void chainDeserializeReadHelper(
			Option<ErrorType>& error, const natl::ConstAsciiStringView& source, Deserializer& deserializer,
			DerializeInfoType& info, ReadFunctor&& readFunctor, OtherReadFunctors&&... otherReadFunctors) noexcept {
			if constexpr(IsDeserializeReadFunctorWithArgs<ErrorType, Deserializer, DerializeInfoType, ReadFunctor>) {
				error = forward<ReadFunctor>(readFunctor)(deserializer, info);
			} else {
				error = forward<ReadFunctor>(readFunctor)();
			}

			if (error.hasValue()) {
				error.value().addSource(source, "");
				return;
			}

			chainDeserializeReadHelper<ErrorType, Deserializer, DerializeInfoType, OtherReadFunctors...>(
				error, source, deserializer, info,
				forward<OtherReadFunctors>(otherReadFunctors)...);
		}

		template<typename ErrorType>
		constexpr void chainDeserializeReadNoArgsHelper(
			Option<ErrorType>&, const natl::ConstAsciiStringView&) {};

		template<typename ErrorType, typename ReadFunctor, typename... OtherReadFunctors>
		constexpr void chainDeserializeReadNoArgsHelper(
			Option<ErrorType>& error, const natl::ConstAsciiStringView& source,
			ReadFunctor&& readFunctor, OtherReadFunctors&&... otherReadFunctors) noexcept {
			error = forward<ReadFunctor>(readFunctor)();

			if (error.hasValue()) {
				error.value().addSource(source, "");
				return;
			}

			chainDeserializeReadNoArgsHelper<ErrorType, OtherReadFunctors...>(
				error, source, forward<OtherReadFunctors>(otherReadFunctors)...);
		}
	}


	template<typename ErrorType, typename Deserializer, typename DerializeInfoType, typename... ReadFunctors>
		requires(impl::IsDeserializeReadFunctor<ErrorType, Deserializer, DerializeInfoType, ReadFunctors> && ...)
	constexpr Option<ErrorType> chainDeserializeRead(const natl::ConstAsciiStringView& source, Deserializer& deserializer,
		DerializeInfoType& info, ReadFunctors&&... readFunctors) noexcept {
		Option<ErrorType> error = OptionEmpty{};
		impl::chainDeserializeReadHelper<ErrorType, Deserializer, DerializeInfoType, ReadFunctors...>(
			error, source, deserializer, info, forward<ReadFunctors>(readFunctors)...);
		return error;
	}

	template<typename ErrorType, typename... ReadFunctors>
		requires(impl::IsDeserializeReadFunctorWithoutArgs<ErrorType, ReadFunctors> && ...)
	constexpr Option<ErrorType> chainDeserializeReadNoArgs(
		const natl::ConstAsciiStringView& source, ReadFunctors&&... readFunctors) noexcept {
		Option<ErrorType> error = OptionEmpty{};
		impl::chainDeserializeReadNoArgsHelper<ErrorType, ReadFunctors...>(
			error, source, forward<ReadFunctors>(readFunctors)...);
		return error;
	}

	template<typename ErrorType, typename Deserializer, typename DerializeInfoType, typename... ReadFunctors>
		requires(impl::IsDeserializeReadFunctorWithArgs<ErrorType, Deserializer, DerializeInfoType, ReadFunctors> && ...)
	constexpr Option<ErrorType> deserializeReadStruct(const natl::ConstAsciiStringView& source, Deserializer& deserializer,
		DerializeInfoType& info, ReadFunctors&&... readFunctors) {
		auto structInfoExpect = deserializer.beginReadStruct(info);
		if (structInfoExpect.hasError()) {
			return structInfoExpect.error().addSource(source, "");
		}
		auto structInfo = structInfoExpect.value();

		auto memberError = chainDeserializeRead<ErrorType, Deserializer, decltype(structInfo), ReadFunctors...>(
			source, deserializer, structInfo, forward<ReadFunctors>(readFunctors)...);
		if(memberError.hasValue()) {
			return memberError.value();
		}

		auto structEndError = deserializer.endReadStruct(structInfo);
		if (structEndError.hasValue()) {
			return structEndError.value().addSource(source, "");
		}
		return {};
	}
}