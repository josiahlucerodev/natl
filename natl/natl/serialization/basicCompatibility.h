#pragma once 

//own
#include "base.h"

//@export
namespace natl {
	//can serialize 
	template<typename Serializer, typename SerializeType>
		requires(IsSerializableC<SerializeType>)
	struct CanSerializeV<Serializer, SerializeType> : CanSerializeV<Serializer, SerializeTypeOf<SerializeType>> {};

	template<typename Serializer>
	concept CanSerializeI8C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const i8 value) {
			{
				serializer.template writeI8<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<i8>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI8 = CanSerializeI8C<Serializer>;
	template<typename Serializer> struct CanSerializeI8V : BoolConstant<CanSerializeI8C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeI16C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const i16 value) {
			{
				serializer.template writeI16<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<i16>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI16 = CanSerializeI16C<Serializer>;
	template<typename Serializer> struct CanSerializeI16V : BoolConstant<CanSerializeI16C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeI32C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const i32 value) {
			{
				serializer.template writeI32<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<i32>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI32 = CanSerializeI32C<Serializer>;
	template<typename Serializer> struct CanSerializeI32V : BoolConstant<CanSerializeI32C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeI64C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const i64 value) {
			{
				serializer.template writeI64<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<i64>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI64 = CanSerializeI64C<Serializer>;
	template<typename Serializer> struct CanSerializeI64V : BoolConstant<CanSerializeI64C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI8C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const ui8 value) {
			{
				serializer.template writeUI8<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ui8>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI8 = CanSerializeUI8C<Serializer>;
	template<typename Serializer> struct CanSerializeUI8V : BoolConstant<CanSerializeUI8C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI16C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const ui16 value) {
			{
				serializer.template writeUI16<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ui16>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI16 = CanSerializeUI16C<Serializer>;
	template<typename Serializer> struct CanSerializeUI16V : BoolConstant<CanSerializeUI16C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI32C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const ui32 value) {
			{
				serializer.template writeUI32<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ui32>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI32 = CanSerializeUI32C<Serializer>;
	template<typename Serializer> struct CanSerializeUI32V : BoolConstant<CanSerializeUI32C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI64C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const ui64 value) {
			{
				serializer.template writeUI64<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ui64>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI64 = CanSerializeUI64C<Serializer>;
	template<typename Serializer> struct CanSerializeUI64V : BoolConstant<CanSerializeUI64C<Serializer>> {};

	template<typename Serializer> concept CanSerializeSizeC = CanSerializeUI64C<Serializer>;
	template<typename Serializer> constexpr inline Bool CanSerializeSize = CanSerializeSizeC<Serializer>;
	template<typename Serializer> struct CanSerializeSizeV : BoolConstant<CanSerializeSizeC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeF32C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const f32 value) {
			{
				serializer.template writeF32<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<f32>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeF32 = CanSerializeF32C<Serializer>;
	template<typename Serializer> struct CanSerializeF32V : BoolConstant<CanSerializeF32C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeF64C = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const f64 value) {
			{
				serializer.template writeF64<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<f64>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeF64 = CanSerializeF64C<Serializer>;
	template<typename Serializer> struct CanSerializeF64V : BoolConstant<CanSerializeF64C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeBoolC = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const Bool value) {
			{
				serializer.template writeBool<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<Bool>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeBool = CanSerializeBoolC<Serializer>;
	template<typename Serializer> struct CanSerializeBoolV : BoolConstant<CanSerializeBoolC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeCharC = IsSerializerC<Serializer>
		&& requires(Serializer& serializer, const Ascii value) {
			{
				serializer.template writeChar<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<Ascii>>(value)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeChar = CanSerializeCharC<Serializer>;
	template<typename Serializer> struct CanSerializeCharV : BoolConstant<CanSerializeCharC<Serializer>> {};

	//can deserialize 
	template<typename Deserializer, typename Type>
		requires(IsDeserializableC<Type>)
	struct CanDeserializeV<Deserializer, Type> : CanDeserializeV<Deserializer, DeserializeTypeOf<Type>> {};

	template<typename Deserializer>
	concept CanDeserializeI8C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readI8<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<i8>>(
						declref<DeserializeInfo<Deserializer, SerializeI8>>())
			} -> IsSameC<Expect<i8, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeI8 = CanDeserializeI8C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeI8V : BoolConstant<CanDeserializeI8C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeI16C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readI16<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<i16>>(
						declref<DeserializeInfo<Deserializer, SerializeI16>>())
			} -> IsSameC<Expect<i16, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeI16 = CanDeserializeI16C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeI16V : BoolConstant<CanDeserializeI16C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeI32C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readI32<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<i32>>(
						declref<DeserializeInfo<Deserializer, SerializeI32>>())
			} -> IsSameC<Expect<i32, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeI32 = CanDeserializeI32C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeI32V : BoolConstant<CanDeserializeI32C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeI64C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readI64<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<i64>>(
						declref<DeserializeInfo<Deserializer, SerializeI64>>())
			} -> IsSameC<Expect<i64, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeI64 = CanDeserializeI64C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeI64V : BoolConstant<CanDeserializeI64C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeUI8C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readUI8<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<ui8>>(
						declref<DeserializeInfo<Deserializer, SerializeUI8>>())
			} -> IsSameC<Expect<ui8, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeUI8 = CanDeserializeUI8C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeUI8V : BoolConstant<CanDeserializeUI8C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeUI16C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readUI16<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<ui16>>(
						declref<DeserializeInfo<Deserializer, SerializeUI16>>())
			} -> IsSameC<Expect<ui16, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeUI16 = CanDeserializeUI16C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeUI16V : BoolConstant<CanDeserializeUI16C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeUI32C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readUI32<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<ui32>>(
						declref<DeserializeInfo<Deserializer, SerializeUI32>>())
			} -> IsSameC<Expect<ui32, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeUI32 = CanDeserializeUI32C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeUI32V : BoolConstant<CanDeserializeUI32C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeUI64C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readUI64<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<ui64>>(
						declref<DeserializeInfo<Deserializer, SerializeUI64>>())
			} -> IsSameC<Expect<ui64, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeUI64 = CanDeserializeUI64C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeUI64V : BoolConstant<CanDeserializeUI64C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeF32C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readF32<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<f32>>(
						declref<DeserializeInfo<Deserializer, SerializeF32>>())
			} -> IsSameC<Expect<f32, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeF32 = CanDeserializeF32C<Deserializer>;
	template<typename Deserializer> struct CanDeserializeF32V : BoolConstant<CanDeserializeF32C<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeF64C = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readF64<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<f64>>(
						declref<DeserializeInfo<Deserializer, SerializeF64>>())
			} -> IsSameC<Expect<f64, StandardDeserializeError<Deserializer>>>;
	};

	template<typename Deserializer>
	concept CanDeserializeBoolC = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readBool<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<Bool>>(
						declref<DeserializeInfo<Deserializer, SerializeBool>>())
			} -> IsSameC<Expect<Bool, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeBool = CanDeserializeBoolC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeBoolV : BoolConstant<CanDeserializeBoolC<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeCharC = IsDeserializerC<Deserializer>
		&& requires(Deserializer & deserializer) {
			{
				deserializer.template readChar<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<Char>>(
						declref<DeserializeInfo<Deserializer, SerializeChar>>())
			} -> IsSameC<Expect<Char, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeChar = CanDeserializeCharC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeCharV : BoolConstant<CanDeserializeCharC<Deserializer>> {};
}