# Serialization

## Serialize API 

```cpp
template<> struct Serialize<MyType> {
	using as_type = ...;
	using type = MyType;
	constexpr static ConstAsciiStringView sourceName = "Serialize<MyType>::write";
	template<typename Serializer> using error_type = StandardSerializeError<Serializer>;;

	template<typename Serializer, SerializeWriteFlag Flags,
		CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
		requires(IsSerializerC<Serializer> && IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr static natl::Option<error_type<Serializer>> write(Serializer& serializer, const type& value) noexcept {
		...
	}
};
```

### Example
```cpp 
template<> struct Serialize<i8> {
	using as_type = SerializeI8;
	using type = i8;
	template<typename Serializer> using error_type = StandardSerializeError<Serializer>;

	template<typename Serializer, SerializeWriteFlag Flags,
		CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeI8C<Serializer>)
	[[nodiscard]] constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const i8 value) noexcept {
		return serializer.template writeI8<Flags, CustomFlags, SerializeComponentType>(value);
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
	[[nodiscard]] constexpr static Option<error_type<Deserializer>>
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
	typename SerializeErrorHandler = natl::DummySerializeErrorHandler,
	typename Alloc = natl::DefaultAllocator>
	requires(natl::IsAllocatorC<Alloc>)
struct Serializer {
private:
	using allocator_type;
	using code_point_type;
	using container_type;
	using custom_write_flag_type = natl::SerializeWriteFlag;
	using serialize_error_handler = SerializeErrorHandler;
	constexpr static custom_write_flag_type defaultCustomWriteFlag = natl::SerializeWriteFlag::none;
	constexpr static inline natl::Size smallBufferSize = smallBufferByteSize;
	constexpr static inline natl::SerializeFlags flag = serializeFlags;

public:
	[[nodiscard]] constexpr natl::ArrayView<code_point_type> output() noexcept {
			return storage.toArrayView();
	}

	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> begin() noexcept {
		return natl::OptionEmpty{};
	}
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> end() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, typename SerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteNamed(const natl::ConstAsciiStringView name) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteNamed() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeValue() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI8(const natl::i8 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI16(const natl::i16 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI32(const natl::i32 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI64(const natl::i64 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI8(const natl::ui8 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI16(const natl::ui16 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI32(const natl::ui32 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI64(const natl::ui64 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF32(const natl::f32 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF64(const natl::f64 value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBool(const natl::Bool value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeChar(const natl::Char value) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::Ascii* str, const natl::Size size) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::ConstAsciiStringView str) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeFile(const natl::ConstAsciiStringView fileName, const natl::ConstArrayView<natl::Byte> data) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBlob(const natl::ConstArrayView<natl::Byte> data) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename BaseSerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsEnumBaseSerializeTypeC<BaseSerializeType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEnum(
		const natl::BasicSerializeTypeToType<BaseSerializeType> intValue,
		const natl::ConstAsciiStringView& strValue) noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteOptional() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteOptional() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyOptional() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, natl::Size FarraySize>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArray() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, natl::Size FarraySize>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArray() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArrayElement() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArrayElement() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyArray() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArray(natl::Size) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArray() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeArrayComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArrayElement() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeArrayComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArrayElement() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyDic() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDic(natl::Size) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDic() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDicElement() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDicElement() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeDicKeyComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicKey() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeDicValueComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicValue() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteStruct() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteStruct() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename VariantSerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyVariant() noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename VariantSerializeType, natl::Size Index>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteVariant(const natl::ConstAsciiStringView& strValue) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename VariantSerializeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteVariant() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>
			&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
			&& natl::IsBuiltInIntegerC<IdNumberType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteJumpTable(const natl::ConstAsciiStringView name) noexcept {
		return natl::OptionEmpty{};
	}
	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeGlobalComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteTable() noexcept {
		return natl::OptionEmpty{};
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>
			&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
			&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
	[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpSaveId, SerializeErrorHandler> writeJumpTableElement(const natl::SerializeJumpId<IdNumberType>& element) noexcept {
		return natl::SerializeJumpSaveId(0);
	}

	template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
		typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>
			&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
			&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
	[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpLocation, SerializeErrorHandler> writeJumpLocation(const natl::SerializeJumpId<IdNumberType>& element,
		const natl::SerializeJumpSaveId& saveId) noexcept {
		return natl::SerializeJumpLocation(0);
	}
};
```

## Deserializer API 

```cpp
template<natl::DeserializeFlags deserializeFlags = natl::DeserializeFlags::shortenError,
	template<typename> typename DeserializeInfo = natl::DummyDeserializeElementInfo, 
	typename DeserializeErrorHandler = natl::DummyDeserializeErrorHandler,
	typename Alloc = natl::DefaultAllocator>
struct Deserializer {
public:
	using allocator_type = Alloc;
	using code_point_type = natl::Byte;
	template<typename SerializeType>
	using deserialize_info = DeserializeInfo<SerializeType>;
	using deserialize_error_handler = DeserializeErrorHandler;
	using error_message_string_type = natl::String256;
	using custom_read_flag_type = natl::CustomDeserializerReadFlagNone;
	constexpr static inline natl::DeserializeFlags flags = deserializeFlags;
	constexpr static inline custom_read_flag_type defaultCustomReadFlag = custom_read_flag_type::none;

public:
	[[nodiscard]] constexpr natl::Bool addSource(const natl::ConstArrayView<code_point_type>& newSource) noexcept {}

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
		readIsEmptyOptional(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename SerilizeType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<SerilizeType>, DeserializeErrorHandler>
		beginReadOptional(DeserializeInfo<natl::SerializeOptional<SerilizeType>>& info) noexcept {}
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
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> beginReadFixedArray(
		DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> endReadFixedArray(
		DeserializeInfo<natl::SerializeFixedArray<ElementType, Number>>& info) noexcept {}
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename ElementType, natl::Size Number>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)

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
			StringToVariatIndexFunctor&& stringToIndex) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename ElementType, typename IndexSize, typename... VariantTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::TemplatePackHasElementC<natl::IsSameV, ElementType, VariantTypes...>)
	[[nodiscard]] constexpr natl::Expect<DeserializeInfo<natl::SerializeTypeOf<ElementType>>, DeserializeErrorHandler>
		beginReadVariantOfType(DeserializeInfo<natl::SerializeVariant<IndexSize, VariantTypes...>>& variant) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename IndexSize, typename... VariantTypes>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler>
		endReadVariant(DeserializeInfo<natl::SerializeVariant<IndexSize, VariantTypes...>>& info) noexcept {}

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
		DeserializeInfo<ParentType>& info, natl::SerializeJumpTableInfo<IdNumberType>) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, 
		typename IdNumberType, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpInfo<IdNumberType>, DeserializeErrorHandler> readJumpTableElement(
		DeserializeInfo<ParentType>& info) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> readJumpLocation(
		const natl::SerializeJumpTableInfo<IdNumberType>& jumpTableInfo,
		natl::SerializeJumpInfo<IdNumberType>& jumpInfo) noexcept {}

	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType, typename IdNumberType>
		requires(natl::IsSerializeComponentC<SerializeComponentType>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> jumpTo(
		const natl::SerializeJumpTableInfo<IdNumberType>& jumpTableInfo, 
		natl::SerializeJumpInfo<IdNumberType>& jumpInfo,
		const natl::Bool searchKnownLocations = true) noexcept {}

	//skip 
	template<natl::DeserializeReadFlag Flags, custom_read_flag_type CustomFlags, typename SerializeComponentType,
		typename Type, typename ParentType>
		requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsDeserializableC<Type>)
	[[nodiscard]] constexpr natl::Option<DeserializeErrorHandler> skip(
		DeserializeInfo<ParentType>& parent, const natl::ConstAsciiStringView name) noexcept {}
};
```

