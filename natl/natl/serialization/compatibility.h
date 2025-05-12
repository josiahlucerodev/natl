#pragma once 

//own
#include "base.h"
#include "basicSerializations.h"
#include "basicCompatibility.h"
#include "../container/variant.h"
#include "../container/string.h"
#include "../container/flatHashMap.h"
#include "../container/dynArray.h"
#include "../container/smallDynArray.h"

//interface 
namespace natl {
	//can serialize 
	template<typename Serializer>
	concept CanSerializeStrC = IsSerializerC<Serializer>
		&& requires(Serializer& serializer,
			const Ascii* rawStr, const Size size,
			const ConstAsciiStringView str) {
				{
					serializer.template writeStr<
						SerializeWriteFlag::none,
						DefaultCustomSerializeWriteFlag<Serializer>,
						SerializeGlobalComponent<natl::ConstAsciiStringView>>(rawStr, size)
				} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
				{
					serializer.template writeStr<
						SerializeWriteFlag::none,
						DefaultCustomSerializeWriteFlag<Serializer>,
						SerializeGlobalComponent<natl::ConstAsciiStringView>>(str)
				} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeStr = CanSerializeStrC<Serializer>;
	template<typename Serializer> struct CanSerializeStrV : BoolConstant<CanSerializeStrC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeFileC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ConstAsciiStringView & fileName, const ArrayView<const Byte>&data) {
			{ true };
			/* TODO
			{
				serializer.template writeFile<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<>>(fileName, data)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			*/ 
	};
	template<typename Serializer> constexpr inline Bool CanSerializeFile = CanSerializeFileC<Serializer>;
	template<typename Serializer> struct CanSerializeFileV : BoolConstant<CanSerializeFileC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeBlobC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer,
			const ArrayView<const Byte>&data) {
			{ true };
			/* TODO
			{
				serializer.template writeBlob<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(data)
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			*/
	};

	template<typename Serializer> constexpr inline Bool CanSerializeBlob = CanSerializeBlobC<Serializer>;
	template<typename Serializer> struct CanSerializeBlobV : BoolConstant<CanSerializeBlobC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeOptionalC = IsSerializerC<Serializer>
		&& requires(Serializer& serializer) {
			{
				serializer.template beginWriteOptional<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<Option<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteOptional<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<Option<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template writeEmptyOptional<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<Option<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeOptional = CanSerializeOptionalC<Serializer>;
	template<typename Serializer> struct CanSerializeOptionalV : BoolConstant<CanSerializeOptionalC<Serializer>> {};

	namespace impl {
		enum struct CanSerializeEnumTestEnum {};
	}

	template<typename Serializer>
	concept CanSerializeEnumC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template writeEnum<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeEnumTestEnum>,
					SerializeUI64>(
						declval<Size>(),
						declval<ConstAsciiStringView>())
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeEnum = CanSerializeEnumC<Serializer>;
	template<typename Serializer> struct CanSerializeEnumV : BoolConstant<CanSerializeEnumC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeFixedArrayC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template beginWriteFixedArray<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ArrayView<i8>>, 10>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteFixedArray<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ArrayView<i8>>, 10>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template beginWriteFixedArrayElement<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeFArrayComponent<ArrayView<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteFixedArrayElement<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeFArrayComponent<ArrayView<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeFixedArray = CanSerializeFixedArrayC<Serializer>;
	template<typename Serializer> struct CanSerializeFixedArrayV : BoolConstant<CanSerializeFixedArrayC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeArrayC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template writeEmptyArray<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ArrayView<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template beginWriteArray<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ArrayView<i8>>>(declval<Size>())
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteArray<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<ArrayView<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template beginWriteArrayElement<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeArrayComponent<ArrayView<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteArrayElement<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeArrayComponent<ArrayView<i8>>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeArray = CanSerializeArrayC<Serializer>;
	template<typename Serializer> struct CanSerializeArrayV : BoolConstant<CanSerializeArrayC<Serializer>> {};

	namespace impl {
		struct CanSerializeDicTestDic {};
	}

	template<typename Serializer>
	concept CanSerializeDicC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template writeEmptyDic<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeDicTestDic>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template beginWriteDic<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeDicTestDic>>(declval<Size>())
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteDic<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeDicTestDic>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template beginWriteDicElement<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeDicTestDic>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteDicElement<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeDicTestDic>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template writeDicKey<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeDicKeyComponent<impl::CanSerializeDicTestDic>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template writeDicValue<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeDicValueComponent<impl::CanSerializeDicTestDic>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeDic = CanSerializeDicC<Serializer>;
	template<typename Serializer> struct CanSerializeDicV : BoolConstant<CanSerializeDicC<Serializer>> {};

	namespace impl {
		struct CanSerializeStructTestStruct {};
	}

	template<typename Serializer>
	concept CanSerializeStructC = IsSerializerC<Serializer>
		&& requires(Serializer& serializer) {
			{
				serializer.template beginWriteStruct<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeStructTestStruct>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteStruct<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeStructTestStruct>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeStruct = CanSerializeStructC<Serializer>;
	template<typename Serializer> struct CanSerializeStructV : BoolConstant<CanSerializeStructC<Serializer>> {};

	template<> struct Serialize<ui8> {
		using as_type = SerializeUI8;
		using type = ui8;
		template<typename Serializer> using error_type = StandardSerializeError<Serializer>;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(CanSerializeUI8C<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const ui8 value) noexcept {
			return serializer.template writeUI8<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	namespace impl {
		struct CanSerializeVariantTestVariant {};
	}

	template<typename Serializer>
	concept CanSerializeVariantC = IsSerializerC<Serializer>
		&& requires(Serializer& serializer) {
			{
				serializer.template beginWriteVariant<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeVariantTestVariant>,
					SerializeVariant<ui8, ui8>, 0>(
						declval<ConstAsciiStringView>())
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template endWriteVariant<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeVariantTestVariant>,
					SerializeVariant<ui8, ui8>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
			{
				serializer.template writeEmptyVariant<
					SerializeWriteFlag::none,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent<impl::CanSerializeVariantTestVariant>,
					SerializeVariant<ui8, ui8>>()
			} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeVariant = CanSerializeVariantC<Serializer>;
	template<typename Serializer> struct CanSerializeVariantV : BoolConstant<CanSerializeVariantC<Serializer>> {};

	template<typename Serializer>
	concept IsFullSerializerC = IsSerializerC<Serializer>
		&& CanSerializeI8C<Serializer>
		&& CanSerializeI16C<Serializer>
		&& CanSerializeI32C<Serializer>
		&& CanSerializeI64C<Serializer>
		&& CanSerializeUI8C<Serializer>
		&& CanSerializeUI16C<Serializer>
		&& CanSerializeUI32C<Serializer>
		&& CanSerializeUI64C<Serializer>
		&& CanSerializeF32C<Serializer>
		&& CanSerializeF64C<Serializer>
		&& CanSerializeBoolC<Serializer>
		&& CanSerializeCharC<Serializer>
		&& CanSerializeStrC<Serializer>
		&& CanSerializeFileC<Serializer>
		&& CanSerializeBlobC<Serializer>
		&& CanSerializeOptionalC<Serializer>
		&& CanSerializeEnumC<Serializer>
		&& CanSerializeFixedArrayC<Serializer>
		&& CanSerializeArrayC<Serializer>
		&& CanSerializeDicC<Serializer>
		&& CanSerializeStructC<Serializer>
		&& CanSerializeVariantC<Serializer>;
	template<typename Serializer> struct IsFullSerializerV : BoolConstant<IsFullSerializerC<Serializer>> {};
	template<typename Serializer> constexpr inline Bool IsFullSerializer = IsFullSerializerC<Serializer>;

	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI8> : BoolConstant<CanSerializeI8C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI16> : BoolConstant<CanSerializeI16C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI32> : BoolConstant<CanSerializeI32C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeI64> : BoolConstant<CanSerializeI64C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI8> : BoolConstant<CanSerializeUI8C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI16> : BoolConstant<CanSerializeUI16C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI32> : BoolConstant<CanSerializeUI32C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeUI64> : BoolConstant<CanSerializeUI64C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeF32> : BoolConstant<CanSerializeF32C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeF64> : BoolConstant<CanSerializeF64C<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeBool> : BoolConstant<CanSerializeBoolC<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeChar> : BoolConstant<CanSerializeCharC<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeStr> : BoolConstant<CanSerializeStrC<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeFile> : BoolConstant<CanSerializeFileC<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeBlob> : BoolConstant<CanSerializeBlobC<Serializer>> {};

	template<typename Serializer, typename SerializeType>
	struct CanSerializeV<Serializer, SerializeOptional<SerializeType>>
		: BoolConstant<CanSerializeOptionalC<SerializeType> && CanSerializeC<Serializer, SerializeType>> {
	};
	template<typename Serializer, typename BaseSerializeType>
	struct CanSerializeV<Serializer, SerializeEnum<BaseSerializeType>>
		: BoolConstant<CanSerializeEnumC<Serializer>&& CanSerializeC<Serializer, BaseSerializeType>> {
	};
	template<typename Serializer, typename ElementType, Size Number>
	struct CanSerializeV<Serializer, SerializeFixedArray<ElementType, Number>>
		: BoolConstant<CanSerializeFixedArrayC<Serializer>&& CanSerializeC<Serializer, ElementType>> {
	};
	template<typename Serializer, typename ElementType>
	struct CanSerializeV<Serializer, SerializeArray<ElementType>>
		: BoolConstant<CanSerializeArrayC<Serializer>&& CanSerializeC<Serializer, ElementType>> {
	};
	template<typename Serializer, typename KeyType, typename ValueType>
	struct CanSerializeV<Serializer, SerializeDic<KeyType, ValueType>>
		: BoolConstant<CanSerializeDicC<Serializer>&& CanSerializeC<Serializer, KeyType>&& CanSerializeC<Serializer, ValueType>> {
	};
	template<typename Serializer, typename... MemberTypes>
	struct CanSerializeV<Serializer, SerializeStruct<MemberTypes...>>
		: BoolConstant<CanSerializeStructC<Serializer> && (CanSerializeC<Serializer, MemberTypes> && ...)> {
	};
	template<typename Serializer, typename IndexType, typename... Types>
	struct CanSerializeV<Serializer, SerializeVariant<IndexType, Types...>>
		: BoolConstant<CanSerializeVariantC<Serializer> && (CanSerializeC<Serializer, Types> && ...)> {
	};

	//can deserialize
	template<typename Deserializer>
	concept CanDeserializeStrC = IsDeserializerC<Deserializer> &&
		requires(Deserializer & deserializer, String & dst) {
			{
				deserializer.template readStr<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<natl::ConstAsciiStringView>, String>(
						declref<DeserializeInfo<Deserializer, SerializeStr>>(), dst)
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeStr = CanDeserializeStrC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeStrV : BoolConstant<CanDeserializeStrC<Deserializer>> {};

	namespace impl {
		struct CanDeserializeFileTestFile {};
	}

	template<typename Deserializer>
	concept CanDeserializeFileC = IsDeserializerC<Deserializer> &&
		requires(Deserializer& deserializer, String& filename, String& contentDst) {
			{
				deserializer.template readFile<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeFileTestFile>, String, String>(
						declref<DeserializeInfo<Deserializer, SerializeFile>>(), 
						filename, contentDst)
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeFile = CanDeserializeFileC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeFileV : BoolConstant<CanDeserializeFileC<Deserializer>> {};

	namespace impl {
		struct CanDeserializeBlobTestFile {};
	}

	template<typename Deserializer>
	concept CanDeserializeBlobC = IsDeserializerC<Deserializer> &&
		requires(Deserializer& deserializer, String& contentDst) {
			{
				deserializer.template readBlob<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeBlobTestFile>, String>(
						declref<DeserializeInfo<Deserializer, SerializeBlob>>(),
						contentDst)
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeBlob = CanDeserializeBlobC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeBlobV : BoolConstant<CanDeserializeBlobC<Deserializer>> {};

	template<typename Deserializer>
	concept CanDeserializeOptionalC = IsDeserializerC<Deserializer> &&
		requires(Deserializer& deserializer) {
			{
				deserializer.template readIsEmptyOptional<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<Option<i8>>, i8>(
						declref<DeserializeInfo<Deserializer, SerializeOptional<i8>>>())
			} -> IsSameC<Expect<Bool, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template beginReadOptional<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<Option<i8>>, i8>(
						declref<DeserializeInfo<Deserializer, SerializeOptional<i8>>>())
			} -> IsSameC<Expect<DeserializeInfo<Deserializer, i8>, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadOptional<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<Option<i8>>, i8>(
						declref<DeserializeInfo<Deserializer, SerializeOptional<i8>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeOptional = CanDeserializeOptionalC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeOptionalV : BoolConstant<CanDeserializeOptionalC<Deserializer>> {};

	namespace impl {
		enum struct SerializeTestEnum : i64 {};
		constexpr Option<i64> testEnumToString(const ConstAsciiStringView&) noexcept;
	}

	template<> struct Serialize<impl::SerializeTestEnum> {
		using as_type = SerializeEnum<i64>;
		using type = impl::SerializeTestEnum;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
		constexpr static void write(Serializer& serializer, const type value) noexcept {};
	};

	template<> struct Deserialize<impl::SerializeTestEnum> {
		using as_type = SerializeEnum<i64>;
		using type = impl::SerializeTestEnum;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<impl::SerializeTestEnum>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				type& dst) noexcept {
			return OptionEmpty{};
		}
	};

	template<typename Deserializer>
	concept CanDeserializeEnumC = IsDeserializerC<Deserializer> &&
		requires(Deserializer& deserializer) {
			{
				deserializer.template readEnum<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::SerializeTestEnum>, i64, decltype(impl::testEnumToString)>(
						declref<DeserializeInfo<Deserializer, SerializeEnum<i64>>>(), 
						impl::testEnumToString)
			} -> IsSameC<Expect<i64, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeEnum = CanDeserializeEnumC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeEnumV : BoolConstant<CanDeserializeEnumC<Deserializer>> {};

	namespace impl {
		struct CanDeserializeFixedArrayTestArray {};
	}

	template<typename Deserializer>
	concept CanDeserializeFixedArrayC = IsDeserializerC<Deserializer> &&
		requires(Deserializer & deserializer) {
			{
				deserializer.template beginReadFixedArray<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeFixedArrayTestArray>, i64, 10>(
						declref<DeserializeInfo<Deserializer, SerializeFixedArray<i64, 10>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadFixedArray<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeFixedArrayTestArray>, i64, 10>(
						declref<DeserializeInfo<Deserializer, SerializeFixedArray<i64, 10>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template beginReadFixedArrayElement<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeFixedArrayTestArray>, i64, 10>(
						declref<DeserializeInfo<Deserializer, SerializeFixedArray<i64, 10>>>())
			} -> IsSameC<Expect<DeserializeInfo<Deserializer, SerializeI64>, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadFixedArrayElement<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeFixedArrayTestArray>, i64, 10>(
						declref<DeserializeInfo<Deserializer, SerializeI64>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeFixedArray = CanDeserializeFixedArrayC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeFixedArrayV : BoolConstant<CanDeserializeFixedArrayC<Deserializer>> {};

	namespace impl {
		struct CanDeserializeArrayTestArray {};
	}

	template<typename Deserializer>
	concept CanDeserializeArrayC = IsDeserializerC<Deserializer> &&
		requires(Deserializer & deserializer) {
			{
				deserializer.template beginReadArray<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeArrayTestArray>, i64>(
						declref<DeserializeInfo<Deserializer, SerializeArray<i64>>>())
			} -> IsSameC<Expect<Size, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadArray<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeArrayTestArray>, i64>(
						declref<DeserializeInfo<Deserializer, SerializeArray<i64>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadEmptyArray<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeArrayTestArray>, i64>(
						declref<DeserializeInfo<Deserializer, SerializeArray<i64>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template beginReadArrayElement<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeArrayTestArray>, i64>(
						declref<DeserializeInfo<Deserializer, SerializeArray<i64>>>())
			} -> IsSameC<Expect<DeserializeInfo<Deserializer, SerializeI64>, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadArrayElement<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeArrayTestArray>, i64>(
						declref<DeserializeInfo<Deserializer, SerializeI64>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeArray = CanDeserializeArrayC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeArrayV : BoolConstant<CanDeserializeArrayC<Deserializer>> {};

	namespace impl {
		struct CanDeserializeDicTestDic {};
	}

	template<typename Deserializer>
	concept CanDeserializeDicC = IsDeserializerC<Deserializer> &&
		requires(Deserializer & deserializer) {
			{
				deserializer.template beginReadDic<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeDicTestDic>, i64, i64>(
						declref<DeserializeInfo<Deserializer, SerializeDic<i64, i64>>>())
			} -> IsSameC<Expect<Size, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadDic<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeDicTestDic>, i64, i64>(
						declref<DeserializeInfo<Deserializer, SerializeDic<i64, i64>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadEmptyDic<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeDicTestDic>, i64, i64>(
						declref<DeserializeInfo<Deserializer, SerializeDic<i64, i64>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template beginReadDicElement<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeDicTestDic>, i64, i64>(
						declref<DeserializeInfo<Deserializer, SerializeDic<i64, i64>>>())
			} -> IsSameC<Expect<DeserializeInfo<Deserializer, natl::SerializeDicElement<i64, i64>>, 
				StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadDicElement<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeDicTestDic>, i64, i64>(
						declref<DeserializeInfo<Deserializer, natl::SerializeDicElement<i64, i64>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template readDicKey<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeDicTestDic>, i64, i64>(
						declref<DeserializeInfo<Deserializer, natl::SerializeDicElement<i64, i64>>>())
			} -> IsSameC<Expect<DeserializeInfo<Deserializer, SerializeI64>, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template readDicValue<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeDicTestDic>, i64, i64>(
						declref<DeserializeInfo<Deserializer, natl::SerializeDicElement<i64, i64>>>())
			} -> IsSameC<Expect<DeserializeInfo<Deserializer, SerializeI64>, StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeDic = CanDeserializeDicC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeDicV : BoolConstant<CanDeserializeDicC<Deserializer>> {};

	namespace impl {
		struct CanDeserializeStructTestStruct {};
	}

	template<typename Deserializer>
	concept CanDeserializeStructC = IsDeserializerC<Deserializer> &&
		requires(Deserializer & deserializer) {
			{
				deserializer.template beginReadStruct<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeStructTestStruct>, i64, i64, i64>(
						declref<DeserializeInfo<Deserializer, SerializeStruct<i64, i64, i64>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadStruct<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeStructTestStruct>, i64, i64, i64>(
						declref<DeserializeInfo<Deserializer, SerializeStruct<i64, i64, i64>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeStruct = CanDeserializeStructC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeStructV : BoolConstant<CanDeserializeStructC<Deserializer>> {};

	namespace impl {
		struct CanDeserializeVariantTestVaraint {};
		constexpr Option<i64> testStringToIndexVariant(const ConstAsciiStringView&) noexcept;
	}

	template<typename Deserializer>
	concept CanDeserializeVariantC = IsDeserializerC<Deserializer> &&
		requires(Deserializer& deserializer) {
			{
				deserializer.template readIsEmptyVariant<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeVariantTestVaraint>, i64, Char, i8>(
						declref<DeserializeInfo<Deserializer, SerializeVariant<i64, Char, i8>>>())
			} -> IsSameC<Expect<natl::Bool, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template beginReadVariantGetIndex<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeVariantTestVaraint>, decltype(impl::testStringToIndexVariant), i64, Char, i8>(
						declref<DeserializeInfo<Deserializer, SerializeVariant<i64, Char, i8>>>(), impl::testStringToIndexVariant)
			} -> IsSameC<Expect<i64, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template beginReadVariantOfType<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeVariantTestVaraint>, i8, i64, Char, i8>(
						declref<DeserializeInfo<Deserializer, SerializeVariant<i64, Char, i8>>>())
			} -> IsSameC<Expect<DeserializeInfo<Deserializer, SerializeI8>, StandardDeserializeError<Deserializer>>>;
			{
				deserializer.template endReadVariant<
					DeserializeReadFlag::none,
					DefaultCustomDeserializeReadFlag<Deserializer>,
					SerializeGlobalComponent<impl::CanDeserializeVariantTestVaraint>, i64, Char, i8>(
						declref<DeserializeInfo<Deserializer, SerializeVariant<i64, Char, i8>>>())
			} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Deserializer> constexpr inline Bool CanDeserializeVariant = CanDeserializeVariantC<Deserializer>;
	template<typename Deserializer> struct CanDeserializeVariantV : BoolConstant<CanDeserializeVariantC<Deserializer>> {};

	template<typename Deserializer> constexpr inline Bool CanDeserializeF64 = CanDeserializeF64C<Deserializer>;
	template<typename Deserializer>
	concept IsFullDeserializerC = IsDeserializerC<Deserializer>
		&& CanDeserializeI8C<Deserializer>
		&& CanDeserializeI16C<Deserializer>
		&& CanDeserializeI32C<Deserializer>
		&& CanDeserializeI64C<Deserializer>
		&& CanDeserializeUI8C<Deserializer>
		&& CanDeserializeUI16C<Deserializer>
		&& CanDeserializeUI32C<Deserializer>
		&& CanDeserializeUI64C<Deserializer>
		&& CanDeserializeF32C<Deserializer>
		&& CanDeserializeF64C<Deserializer>
		&& CanDeserializeBoolC<Deserializer>
		&& CanDeserializeCharC<Deserializer>
		&& CanDeserializeStrC<Deserializer>
		&& CanDeserializeFileC<Deserializer>
		&& CanDeserializeBlobC<Deserializer>
		&& CanDeserializeOptionalC<Deserializer>
		&& CanDeserializeEnumC<Deserializer>
		&& CanDeserializeFixedArrayC<Deserializer>
		&& CanDeserializeArrayC<Deserializer>
		&& CanDeserializeDicC<Deserializer>
		&& CanDeserializeStructC<Deserializer>
		&& CanDeserializeVariantC<Deserializer>;

	template<typename Deserializer> struct IsFullDeserializerV : BoolConstant<IsFullDeserializerC<Deserializer>> {};
	template<typename Deserializer> constexpr inline Bool IsFullDeserializerV = IsFullDeserializerC<Deserializer>;

	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeI8> : BoolConstant<CanDeserializeI8C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeI16> : BoolConstant<CanDeserializeI16C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeI32> : BoolConstant<CanDeserializeI32C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeI64> : BoolConstant<CanDeserializeI64C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeUI8> : BoolConstant<CanDeserializeUI8C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeUI16> : BoolConstant<CanDeserializeUI16C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeUI32> : BoolConstant<CanDeserializeUI32C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeUI64> : BoolConstant<CanDeserializeUI64C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeF32> : BoolConstant<CanDeserializeF32C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeF64> : BoolConstant<CanDeserializeF64C<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeBool> : BoolConstant<CanDeserializeBoolC<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeChar> : BoolConstant<CanDeserializeCharC<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeStr> : BoolConstant<CanDeserializeStrC<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeFile> : BoolConstant<CanDeserializeFileC<Deserializer>> {};
	template<typename Deserializer>
	struct CanDeserializeV<Deserializer, SerializeBlob> : BoolConstant<CanDeserializeBlobC<Deserializer>> {};

	template<typename Deserializer, typename SerializeType>
	struct CanDeserializeV<Deserializer, SerializeOptional<SerializeType>>
		: BoolConstant<CanDeserializeOptionalC<SerializeType>&& CanDeserializeC<Deserializer, SerializeType>> {
	};
	template<typename Deserializer, typename BaseSerializeType>
	struct CanDeserializeV<Deserializer, SerializeEnum<BaseSerializeType>>
		: BoolConstant<CanDeserializeEnumC<Deserializer>&& CanDeserializeC<Deserializer, BaseSerializeType>> {
	};
	template<typename Deserializer, typename ElementType, Size Number>
	struct CanDeserializeV<Deserializer, SerializeFixedArray<ElementType, Number>>
		: BoolConstant<CanDeserializeFixedArrayC<Deserializer>&& CanDeserializeC<Deserializer, ElementType>> {
	};
	template<typename Deserializer, typename ElementType>
	struct CanDeserializeV<Deserializer, SerializeArray<ElementType>>
		: BoolConstant<CanDeserializeArrayC<Deserializer>&& CanDeserializeC<Deserializer, ElementType>> {
	};
	template<typename Deserializer, typename KeyType, typename ValueType>
	struct CanDeserializeV<Deserializer, SerializeDic<KeyType, ValueType>>
		: BoolConstant<CanDeserializeDicC<Deserializer>&& CanDeserializeC<Deserializer, KeyType>&& CanDeserializeC<Deserializer, ValueType>> {
	};
	template<typename Deserializer, typename... MemberTypes>
	struct CanDeserializeV<Deserializer, SerializeStruct<MemberTypes...>>
		: BoolConstant<CanDeserializeStructC<Deserializer> && (CanDeserializeC<Deserializer, MemberTypes> && ...)> {
	};
	template<typename Deserializer, typename IndexType, typename... Types>
	struct CanDeserializeV<Deserializer, SerializeVariant<IndexType, Types...>>
		: BoolConstant<CanDeserializeVariantC<Deserializer> && (CanDeserializeC<Deserializer, Types> && ...)> {
	};

}