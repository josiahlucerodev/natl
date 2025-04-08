# Serialization

## Serialize API 

```cpp
template<> struct Serialize<MyType> {
	using as_type = ...;
	using type = MyType;
	template<typename Serializer> using error_type = void;

	template<typename Serializer, SerializeWriteFlag Flags,
		CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
		requires(IsSerializerC<Serializer> && IsSerializeComponentC<SerializeComponentType>)
	constexpr static void write(Serializer& serializer, const type& value) noexcept {
		...
	}
};
```

### Example
```cpp 
template<> struct Serialize<i8> {
	using as_type = SerializeI8;
	using type = i8;
	template<typename Serializer> using error_type = void;

	template<typename Serializer, SerializeWriteFlag Flags,
		CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
		requires(IsSerializerC<Serializer> && IsSerializeComponentC<SerializeComponentType> && CanSerializeI8C<Serializer>)
	constexpr static void write(Serializer& serializer, const i8 value) noexcept {
		serializer.template writeI8<Flags, CustomFlags, SerializeComponentType>(value);
	}
};
```



## Deserialize API 
```cpp
template<> struct Deserialize<MyType> {
	using as_type = ...;
	using type = MyType;
	constexpr static ConstAsciiStringView sourceName = "Deserialize<MyType>::read";
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
		requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType>)
	constexpr static Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			type& dst) noexcept {
		...
	}
};
```

### Example
```cpp 
template<> struct Deserialize<i8> {
	using as_type = SerializeI8;
	using type = i8;
	constexpr static ConstAsciiStringView sourceName = "Deserialize<i8>::read";
	template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
		requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> && CanDeserializeI8C<Deserializer>)
	constexpr static Option<error_type<Deserializer>>
		read(Deserializer& deserializer,
			typename Deserializer::template deserialize_info<as_type>& info,
			i8& dst) noexcept {
		auto valueExpect = deserializer.template readI8<Flags, CustomFlags, SerializeComponentType>(info);
		if (valueExpect.hasValue()) {
			dst = valueExpect.value();
			return {};
		} else {
			return valueExpect.error().addSource(sourceName, "");
		}
	}
};
```


## Serializer API 

```cpp
template<
	natl::Size smallBufferByteSize = 1024,
	natl::SerializeFlags serializeFlags = natl::SerializeFlags::pretty,
	typename Alloc = natl::DefaultAllocator<natl::Ascii>>
	requires(natl::IsAllocatorC<Alloc>)
struct Serializer {
private:
	using allocator_type;
	using code_point_type;
	using container_type;
	using custom_write_flag_type;
	constexpr static custom_write_flag_type defaultCustomWriteFlag;
	constexpr static inline natl::Size smallBufferSize = smallBufferByteSize;
	constexpr static inline natl::SerializeFlags flag = serializeFlags;
public:
	constexpr Serializer() noexcept {}
	constexpr void begin() noexcept {}
	constexpr void end() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, typename SerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteNamed(const natl::ConstAsciiStringView name) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteNamed() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeValue() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteOptional() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteOptional() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeEmptyOptional() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeI8(const natl::i8 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeI16(const natl::i16 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeI32(const natl::i32 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeI64(const natl::i64 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeUI8(const natl::ui8 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeUI16(const natl::ui16 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeUI32(const natl::ui32 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeUI64(const natl::ui64 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeF32(const natl::f32 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeF64(const natl::f64 value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeBool(const natl::Bool value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeChar(const natl::Char value) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeStr(const natl::Ascii* str, const natl::Size size) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeStr(const natl::ConstAsciiStringView str) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeFile(const natl::ConstAsciiStringView fileName, const natl::ConstArrayView<natl::Byte> data) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeBlob(const natl::ConstArrayView<natl::Byte> data) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename BaseSerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsEnumBaseSerializeTypeC<BaseSerializeType>)
	constexpr void writeEnum(
		const natl::BasicSerializeTypeToType<BaseSerializeType> intValue,
		const natl::ConstAsciiStringView& strValue) noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteFixedArray() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteFixedArray() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteFixedArrayElement() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteFixedArrayElement() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeEmptyArray() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteArray(natl::Size) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteArray() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteArrayElement() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteArrayElement() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeEmptyDic() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteDic(natl::Size) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteDic() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteDicElement() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteDicElement() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeDicKey() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeDicValue() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void beginWriteStruct() noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteStruct() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void writeEmptyVariant() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename VariantSerializeType, natl::Size Index>
		requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeVariantC<VariantSerializeType>)
	constexpr void beginWriteVariant(const natl::ConstAsciiStringView& strValue) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename VariantSerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteVariant() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename IndexIDSizeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsBuiltInIntegerC<IndexIDSizeType>)
	constexpr void beginWriteJumpTable(const natl::ConstAsciiStringView name) noexcept {}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr void endWriteTable() noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
	constexpr natl::SerializeJumpSaveId writeJumpTableElement(const natl::SerializeJumpId<IdNumberType>& element) noexcept {}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
	constexpr natl::SerializeJumpLocation writeJumpLocation(const natl::SerializeJumpId<IdNumberType>& element,
		const natl::SerializeJumpSaveId& saveId) noexcept {}
};
```

## Deserializer API 

```cpp
template<natl::DeserializeFlags deserializeFlags, template<typename> typename DeserializeInfo, typename DeserializeErrorHandler>
struct Deserializer {
public:
	template<typename SerializeType>
	using deserialize_info = DeserializeInfo<SerializeType>;
	using deserialize_error_handler = DeserializeErrorHandler;
	using error_message_string_type = natl::String256;
	using custom_read_flag_type = natl::CustomDeserializerReadFlagNone;
	constexpr static inline natl::DeserializeFlags flags = deserializeFlags;
	constexpr static inline custom_read_flag_type defaultCustomReadFlag = custom_read_flag_type::none;
public:
	[[nodiscard]] constexpr natl::Bool addSource(const natl::ConstArrayView<natl::Char>& newSource) noexcept {}

	template<typename Serializer>
		requires(natl::IsSerializerC<Serializer>)
	constexpr void serializeErrors(Serializer& serializer) noexcept {}
	template<typename Serializer, natl::SerializeWriteFlag Flags,
		natl::CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeMemberType>
	constexpr void serializeSourceProccessState(Serializer& serializer) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags>
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::DeserializeGlobalScope>, DeserializeErrorHandler> begin() noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags>
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> end(
		DeserializeInfo<natl::DeserializeGlobalScope> info) noexcept {}		

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename Type, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<Type>>, DeserializeErrorHandler> beginReadNamed(
		DeserializeInfo<ParentType>& parrent, const natl::ConstAsciiStringView name) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadNamed(DeserializeInfo<SerializeType>& info) noexcept {}


	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::i8, DeserializeErrorHandler> readI8(DeserializeInfo<natl::SerializeI8>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::i16, DeserializeErrorHandler> readI16(DeserializeInfo<natl::SerializeI16>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::i32, DeserializeErrorHandler> readI32(DeserializeInfo<natl::SerializeI32>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::i64, DeserializeErrorHandler> readI64(DeserializeInfo<natl::SerializeI64>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::ui8, DeserializeErrorHandler> readUI8(DeserializeInfo<natl::SerializeUI8>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::ui16, DeserializeErrorHandler> readUI16(DeserializeInfo<natl::SerializeUI16>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::ui32, DeserializeErrorHandler> readUI32(DeserializeInfo<natl::SerializeUI32>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::ui64, DeserializeErrorHandler> readUI64(DeserializeInfo<natl::SerializeUI64>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::f32, DeserializeErrorHandler> readF32(DeserializeInfo<natl::SerializeF32>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::f64, DeserializeErrorHandler> readF64(DeserializeInfo<natl::SerializeF64>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::Bool, DeserializeErrorHandler> readBool(DeserializeInfo<natl::SerializeBool>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::Ascii, DeserializeErrorHandler> readChar(DeserializeInfo<natl::SerializeChar>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename StringDstType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readStr(DeserializeInfo<natl::SerializeStr>& info, StringDstType& dst) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename FileNameDstType, typename FileContentDstType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
		readFile(DeserializeInfo<natl::SerializeFile>& info, FileNameDstType& filenameDst, FileContentDstType& fileContentDst) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename DataDstType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readBlob(DeserializeInfo<natl::SerializeBlob>& info, DataDstType& dst) noexcept {}

	//optional
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerilizeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::Bool, DeserializeErrorHandler>
		readIsOptionalNull(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerilizeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<SerilizeType>, DeserializeErrorHandler>
		beginReadOptional(const natl::Bool isNull, DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerilizeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
		endReadOptional(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {}

	//enum
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename BaseType, typename Functor>
		requires(natl::IsSerializableC<BaseType> && natl::IsSerializeComponentC<SerializeComponentType>
			&& natl::IsStringToSerializeFlagConvertFunctor<Functor, BaseType>)
	[[nodiscard]] constexpr natl::Expect<natl::BasicSerializeTypeToType<natl::SerializeTypeOf<BaseType>>, DeserializeErrorHandler>
		readEnum(DeserializeInfo<natl::SerializeEnum<BaseType>>& info, Functor&& stringToEnum) noexcept {}

	//fixed array
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadFixedArray(
		DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadFixedArray(
		DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyFixedArray(
		DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler> beginReadFixedArrayElement(
		DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename ElementType, natl::Size Number, typename ElementSerializeType = natl::SerializeTypeOf<ElementType>>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadFixedArrayElement(DeserializeInfo<ElementSerializeType>& info) {}

	//array
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadArray(
		DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadArray(
		DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyArray(
		DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler> beginReadArrayElement(
		DeserializeInfo<natl::SerializeArray<ElementType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType,
		typename ElementSerializeType = natl::SerializeTypeOf<ElementType>>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadArrayElement(DeserializeInfo<ElementSerializeType>& info) {}

	//dic
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename KeyType, typename ValueType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::Size, DeserializeErrorHandler> beginReadDic(
		DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename KeyType, typename ValueType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadDic(
		DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename KeyType, typename ValueType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadEmptyDic(
		DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename KeyType, typename ValueType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>, DeserializeErrorHandler> beginReadDicElement(
		DeserializeInfo<natl::SerializeDic<KeyType, ValueType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename KeyType, typename ValueType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadDicElement(
		DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename KeyType, typename ValueType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<KeyType>>, DeserializeErrorHandler> readDicKey(
		DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename KeyType, typename ValueType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ValueType>>, DeserializeErrorHandler> readDicValue(
		DeserializeInfo<natl::SerializeDicElement<KeyType, ValueType>>& info) noexcept {}

	//struct
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename... SerializeMemberTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> beginReadStruct(
		DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>> info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename... SerializeMemberTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadStruct(
		DeserializeInfo<natl::SerializeStruct<SerializeMemberTypes...>>& info) {}

	//variant
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename IndexSerializeType, typename... VariantSerializeTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	constexpr natl::Expect<natl::Bool, DeserializeErrorHandler> 
		readIsEmptyVariant(DeserializeInfo<natl::SerializeVariant<IndexSerializeType, VariantSerializeTypes...>>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename StringToVariatIndexFunctor, typename IndexType, typename... VariantTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType> 
			&& natl::IsStringToSerializeVariantIndexConvertFunctor<StringToVariatIndexFunctor, IndexType>)
	[[nodiscard]] constexpr natl::Expect<IndexType, DeserializeErrorHandler>
		beginReadVariantGetIndex(
			DeserializeInfo<natl::SerializeVariant<IndexType, VariantTypes...>>& info,
			const natl::Bool isEmpty,
			StringToVariatIndexFunctor&& stringToIndex) {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename ElementType, typename IndexSize, typename... VariantTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::TemplatePackHasElementC<natl::IsSameV, ElementType, VariantTypes...>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler>
		beginReadVariantOfType(DeserializeInfo<natl::SerializeVariant<IndexSize, VariantTypes...>>& variant) {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename IndexSize, typename... VariantTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
		endReadVariant(DeserializeInfo<natl::SerializeVariant<IndexSize, VariantTypes...>>& info) {}

	//jump
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename IdNumberType, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpTableInfo<IdNumberType>, DeserializeErrorHandler> beginReadJumpTable(
		DeserializeInfo<ParentType>& info,
		const natl::ConstAsciiStringView& name) noexcept {}
		
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename IdNumberType, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadJumpTable(
		DeserializeInfo<ParentType>& info, natl::SerializeJumpTableInfo<IdNumberType>) {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename IdNumberType, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpInfo<IdNumberType>, DeserializeErrorHandler> readJumpTableElement(
		DeserializeInfo<ParentType>& info) {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readJumpLocation(
		const natl::SerializeJumpTableInfo<IdNumberType>& jumpTableInfo,
		natl::SerializeJumpInfo<IdNumberType>& jumpInfo) {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> jumpTo(
		const natl::SerializeJumpTableInfo<IdNumberType>& jumpTableInfo, 
		natl::SerializeJumpInfo<IdNumberType>& jumpInfo,
		const natl::Bool searchKnownLocations = true) {}

	//skip 
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename Type, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsDeserializableC<Type>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> skip(
		DeserializeInfo<ParentType>& parent, const natl::ConstAsciiStringView name) noexcept {}
};
```

