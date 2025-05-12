#pragma once 

//own
#include "../util/typeTraits.h"
#include "../util/compilerDependent.h"
#include "../container/stringView.h"
#include "../container/arrayView.h"
#include "../fundamental/expect.h"
#include "../fundamental/strongType.h"

//interface 
namespace natl {
	enum class SerializeFlags {
		none = 0,
		pretty = 1 << 0,
		fullTypes = 1 << 1,
	};

	constexpr inline SerializeFlags operator|(const SerializeFlags lhs, const SerializeFlags rhs) noexcept {
		using underlying_type = UnderlyingType<SerializeFlags>;
		return static_cast<SerializeFlags>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs));
	} constexpr inline SerializeFlags operator&(const SerializeFlags lhs, const SerializeFlags rhs) noexcept {
		using underlying_type = UnderlyingType<SerializeFlags>;
		return static_cast<SerializeFlags>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs));
	};

	//serialize error 
	template<typename Type>
	struct Serialize;

	template<typename Serializer>
	using StandardSerializeError = Serializer::serialize_error_handler;

	template<typename Serializer, typename Type> constexpr inline Bool IsStandardSerializeError = IsSameC<
		typename Decay<Serializer>::serialize_error_handler, Type>;
	template<typename Serializer, typename Type> struct IsStandardSerializeErrorV
		: BoolConstant<IsStandardSerializeError<Serializer, Type>> {
	};
	template<typename Serializer, typename Type> concept IsStandardSerializeErrorC = IsStandardSerializeError<Serializer, Type>;

	template<typename Serializer, typename ErrorType>
	struct SerializeError {
		using serializer = Serializer;
		using error_type = ErrorType;
		using serialize_error_handler = Serializer::serialize_error_handler;

		serialize_error_handler errorHandler;
		ErrorType errorType;

		constexpr Bool hasMessage() noexcept { return errorHandler.hasMessage(); }
		constexpr SerializeError& addSource(const ConstAsciiStringView& name) noexcept {
			errorHandler.addSource(name);
		}
	};

	template<typename Type> struct IsSerializeErrorV : FalseType {};
	template<typename Type> constexpr inline Bool IsSerializeError = IsSerializeErrorV<Type>::value;
	template<typename Type> concept IsSerializeErrorC = IsSerializeError<Type>;
	template<typename Serializer, typename ErrorType>
	struct IsSerializeErrorV<SerializeError<Serializer, ErrorType>> : TrueType {};

	template<typename Serializer, typename Type>
	using SerializeErrorOf = typename Serialize<Decay<Type>>::template error_type<Serializer>;
	template<typename Serializer, typename Type>
	using SerializeExpect = Expect<Type, SerializeErrorOf<Serializer, Type>>;

	enum struct SerializeErrorFlag {
		none = 0,
		inactive,
		duplicateCall,
		earlyEnd,
		invalidState,
		wrongType,
	};

	constexpr ConstAsciiStringView serializeErrorFlagToString(const SerializeErrorFlag errorFlag) noexcept {
		switch (errorFlag) {
		case SerializeErrorFlag::none:
			return "none";
		case SerializeErrorFlag::inactive:
			return "inactive";
		case SerializeErrorFlag::duplicateCall:
			return "duplicateCall";
		case SerializeErrorFlag::earlyEnd:
			return "earlyEnd";
		case SerializeErrorFlag::invalidState:
			return "invalidState";
		case SerializeErrorFlag::wrongType:
			return "wrongType";
		default:
			unreachable();
		}
	}

	enum struct SerializeErrorLocation {
		none = 0,
		begin,
		end,
		beginWriteNamed,
		endWriteNamed,
		writeValue,
		writeI8,
		writeI16,
		writeI32,
		writeI64,
		writeUI8,
		writeUI16,
		writeUI32,
		writeUI64,
		writeF32,
		writeF64,
		writeBool,
		writeChar,
		writeStr,
		writeFile,
		writeBlob,
		beginWriteOptional,
		endWriteOptional,
		writeEmptyOptional,
		writeEnum,
		beginWriteFixedArray,
		endWriteFixedArray,
		beginWriteFixedArrayElement,
		endWriteFixedArrayElement,
		writeEmptyArray,
		beginWriteArray,
		endWriteArray,
		beginWriteArrayElement,
		endWriteArrayElement,
		writeEmptyDic,
		beginWriteDic,
		endWriteDic,
		beginWriteDicElement,
		endWriteDicElement,
		writeDicKey,
		writeDicValue,
		beginWriteStruct,
		endWriteStruct,
		writeEmptyVariant,
		beginWriteVariant,
		endWriteVariant,
		beginWriteJumpTable,
		endWriteTable,
		writeJumpTableElement,
		writeJumpLocation,
	};

	constexpr ConstAsciiStringView deserializeErrorLocationToString(const SerializeErrorLocation errorLocation) noexcept {
		switch (errorLocation) {
		case SerializeErrorLocation::none:
			return "none";
		case SerializeErrorLocation::begin:
			return "begin";
		case SerializeErrorLocation::end:
			return "end";
		case SerializeErrorLocation::beginWriteNamed:
			return "beginWriteNamed";
		case SerializeErrorLocation::endWriteNamed:
			return "endWriteNamed";
		case SerializeErrorLocation::writeValue:
			return "writeValue";
		case SerializeErrorLocation::beginWriteOptional:
			return "beginWriteOptional";
		case SerializeErrorLocation::endWriteOptional:
			return "endWriteOptional";
		case SerializeErrorLocation::writeEmptyOptional:
			return "writeEmptyOptional";
		case SerializeErrorLocation::writeI8:
			return "writeI8";
		case SerializeErrorLocation::writeI16:
			return "writeI16";
		case SerializeErrorLocation::writeI32:
			return "writeI32";
		case SerializeErrorLocation::writeI64:
			return "writeI64";
		case SerializeErrorLocation::writeUI8:
			return "writeUI8";
		case SerializeErrorLocation::writeUI16:
			return "writeUI16";
		case SerializeErrorLocation::writeUI32:
			return "writeUI32";
		case SerializeErrorLocation::writeUI64:
			return "writeUI64";
		case SerializeErrorLocation::writeF32:
			return "writeF32";
		case SerializeErrorLocation::writeF64:
			return "writeF64";
		case SerializeErrorLocation::writeBool:
			return "writeBool";
		case SerializeErrorLocation::writeChar:
			return "writeChar";
		case SerializeErrorLocation::writeStr:
			return "writeStr";
		case SerializeErrorLocation::writeFile:
			return "writeFile";
		case SerializeErrorLocation::writeBlob:
			return "writeBlob";
		case SerializeErrorLocation::writeEnum:
			return "writeEnum";
		case SerializeErrorLocation::beginWriteFixedArray:
			return "beginWriteFixedArray";
		case SerializeErrorLocation::endWriteFixedArray:
			return "endWriteFixedArray";
		case SerializeErrorLocation::beginWriteFixedArrayElement:
			return "beginWriteFixedArrayElement";
		case SerializeErrorLocation::endWriteFixedArrayElement:
			return "endWriteFixedArrayElement";
		case SerializeErrorLocation::writeEmptyArray:
			return "writeEmptyArray";
		case SerializeErrorLocation::beginWriteArray:
			return "beginWriteArray";
		case SerializeErrorLocation::endWriteArray:
			return "endWriteArray";
		case SerializeErrorLocation::beginWriteArrayElement:
			return "beginWriteArrayElement";
		case SerializeErrorLocation::endWriteArrayElement:
			return "endWriteArrayElement";
		case SerializeErrorLocation::writeEmptyDic:
			return "writeEmptyDic";
		case SerializeErrorLocation::beginWriteDic:
			return "beginWriteDic";
		case SerializeErrorLocation::endWriteDic:
			return "endWriteDic";
		case SerializeErrorLocation::beginWriteDicElement:
			return "beginWriteDicElement";
		case SerializeErrorLocation::endWriteDicElement:
			return "endWriteDicElement";
		case SerializeErrorLocation::writeDicKey:
			return "writeDicKey";
		case SerializeErrorLocation::writeDicValue:
			return "writeDicValue";
		case SerializeErrorLocation::beginWriteStruct:
			return "beginWriteStruct";
		case SerializeErrorLocation::endWriteStruct:
			return "endWriteStruct";
		case SerializeErrorLocation::writeEmptyVariant:
			return "writeEmptyVariant";
		case SerializeErrorLocation::beginWriteVariant:
			return "beginWriteVariant";
		case SerializeErrorLocation::endWriteVariant:
			return "endWriteVariant";
		case SerializeErrorLocation::beginWriteJumpTable:
			return "beginWriteJumpTable";
		case SerializeErrorLocation::endWriteTable:
			return "endWriteTable";
		case SerializeErrorLocation::writeJumpTableElement:
			return "writeJumpTableElement";
		case SerializeErrorLocation::writeJumpLocation:
			return "writeJumpLocation";
		default:
			unreachable();
		}
	}

	struct DummySerializeErrorHandler {
		constexpr DummySerializeErrorHandler(const ConstAsciiStringView&,
			const SerializeErrorLocation&, const SerializeErrorFlag&) noexcept {
		}
		constexpr Bool hasMessage() const noexcept { return false; }
		constexpr DummySerializeErrorHandler& addSource(const ConstAsciiStringView&) noexcept { return *this; }
		template<typename DynStringType>
		constexpr void getMessageTo(DynStringType&) const noexcept {}
		template<typename DynStringType>
		constexpr DynStringType getMessage() const noexcept { return DynStringType(); }
		template<typename DynStringType>
		constexpr void getErrorMessageTo(DynStringType&) const noexcept {}
		template<typename DynStringType>
		constexpr DynStringType getErrorMessage() const noexcept { return DynStringType(); }
	};

	struct DummySerializer {
		using allocator_type = DefaultAllocator;
		using code_point_type = Ascii;
		using container_type = void;
		using serialize_error_handler = DummySerializeErrorHandler;

		constexpr static inline Size smallBufferSize = 0;
		constexpr static inline SerializeFlags flag = SerializeFlags::none;
	};

	//is serialize
	template<typename Type>
	concept IsSerializableC = requires() {
		typename Serialize<Decay<Type>>;
		typename Serialize<Decay<Type>>::type;
		typename Serialize<Decay<Type>>::as_type;
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

	//is serialize error handler 
	template<typename SerializerErrorHandlerType>
	concept IsSerializeErrorHandlerC = requires(
		const SerializerErrorHandlerType & errorHandler,
		SerializerErrorHandlerType & sourceErrorHandler,
		const ConstAsciiStringView & errorMessage,
		const SerializeErrorLocation & errorLocation,
		const SerializeErrorFlag & errorFlag,
		const ConstAsciiStringView & source) {
			{ SerializerErrorHandlerType(errorMessage, errorLocation, errorFlag) };
			{ errorHandler.hasMessage() } -> IsSameC<natl::Bool>;
			{ sourceErrorHandler.addSource(source) } -> IsSameC<SerializerErrorHandlerType&>;
			{ errorHandler.template getMessageTo<DummyString>(declref<DummyString>()) } -> IsSameC<void>;
			{ errorHandler.template getMessage<DummyString>() } -> IsConvertibleC<DummyString>;
			{ errorHandler.template getErrorMessageTo<DummyString>(declref<DummyString>()) } -> IsSameC<void>;
			{ errorHandler.template getErrorMessage<DummyString>() } -> IsConvertibleC<DummyString>;
	};
	template<typename Type> constexpr inline Bool IsSerializeErrorHandler = IsSerializeErrorHandlerC<Type>;
	template<typename Type> struct IsSerializeErrorHandlerV : BoolConstant<IsSerializeErrorHandlerC<Type>> {};
	static_assert(IsSerializeErrorHandlerC<DummySerializeErrorHandler>);

	//serialize name
	template<typename Type>
	concept HasRegularSerializeNameC = IsSerializableC<Type> && requires() {
		{ Serialize<Decay<Type>>::name } -> ConvertibleTo<ConstAsciiStringView>;
	};
	template<typename Type> constexpr inline Bool HasRegularSerializeName = HasRegularSerializeNameC<Type>;
	template<typename Type> struct HasRegularSerializeNameV : BoolConstant<HasRegularSerializeNameC<Type>> {};

	template<typename Type>
	constexpr inline ConstAsciiStringView RegularSerializeName = Serialize<Type>::name;

	template<typename Type>
	struct OverrideSerializeNameV {
		using not_specialized = void;
	};

	template<typename Type>
	concept HasOverrideSerializeNameC = !requires() {
		typename OverrideSerializeNameV<Type>::not_specialized;
	};
	template<typename Type> constexpr inline Bool HasOverrideSerializeName = HasOverrideSerializeNameC<Type>;
	template<typename Type> struct HasOverrideSerializeNameV : BoolConstant<HasOverrideSerializeNameC<Type>> {};

	template<typename Type>
	constexpr inline ConstAsciiStringView OverrideSerializeName = OverrideSerializeNameV<Type>::value;

	template<typename Type>
	concept HasSerializeNameC = HasRegularSerializeNameC<Type> || HasOverrideSerializeNameC<Type>;
	template<typename Type> constexpr inline Bool HasSerializeName = HasSerializeNameC<Type>;
	template<typename Type> struct HasSerializeNameV : BoolConstant<HasSerializeNameC<Type>> {};

	template<typename Type>
		requires(HasSerializeNameC<Type>)
	struct SerializeNameV {
		consteval static ConstAsciiStringView getName() noexcept {
			if constexpr (HasOverrideSerializeNameC<Type>) {
				return OverrideSerializeName<Type>;
			} else {
				return RegularSerializeName<Type>;
			}
		}
		constexpr static inline ConstAsciiStringView value = getName();
	};
	template<typename Type>
	constexpr inline ConstAsciiStringView SerializeName = SerializeNameV<Type>::value;

	template<typename Type> struct IsEnumBaseSerializeTypeV : FalseType {};
	template<typename Type> constexpr inline Bool IsEnumBaseSerializeType = IsEnumBaseSerializeTypeV<Type>::value;
	template<typename Type> concept IsEnumBaseSerializeTypeC = IsEnumBaseSerializeType<Type>;

	template<typename Type> struct IsVariantIndexSerializeTypeV : FalseType {};
	template<typename Type> constexpr inline Bool IsVariantIndexSerializeType = IsVariantIndexSerializeTypeV<Type>::value;
	template<typename Type> concept IsVariantIndexSerializeTypeC = IsVariantIndexSerializeType<Type>;

	//components
	template<typename Type>
	//requires(IsSerializableC<Type>)
	struct SerializeGlobalComponent {
		using type = Type;
	};

	template<typename Type>
	//requires(IsSerializableC<Type>)
	struct SerializeOptionalComponent {
		using type = Type;
		//using serialize_type = SerializeTypeOf<Type>;
	};

	template<typename Type>
	//requires(IsSerializableC<Type>)
	struct SerializeFArrayComponent {
		using type = Type;
		//using serialize_type = SerializeTypeOf<Type>;
	};

	template<typename Type>
	//requires(IsSerializableC<Type>)
	struct SerializeArrayComponent {
		using type = Type;
		//using serialize_type = SerializeTypeOf<Type>;
	};

	template<typename Type>
	//requires(IsSerializableC<Type>)
	struct SerializeDicKeyComponent {
		using type = Type;
		//using serialize_type = SerializeTypeOf<Type>;
	};

	template<typename Type>
	//requires(IsSerializableC<Type>)
	struct SerializeDicValueComponent {
		using type = Type;
		//using serialize_type = SerializeTypeOf<Type>;
	};

	template<typename StructType, typename MemberType, Size Index>
	//requires(IsSerializableC<Type>)
	struct SerializeStructComponent {
		using parent_type = StructType;
		using type = MemberType;
		//using serialize_type = SerializeTypeOf<Type>;
		constexpr static inline Size index = Index;
	};

	template<typename VariantType, typename ElementType, Size Index>
	//requires(IsSerializableC<Type>)
	struct SerializeVariantComponent {
		using parent_type = VariantType;
		using type = ElementType;
		//using serialize_type = SerializeTypeOf<Type>;
		constexpr static inline Size index = Index;
	};

	template<typename Type> struct IsSerializeGlobalComponentV : FalseType {};
	template<typename Type> struct IsSerializeGlobalComponentV<SerializeGlobalComponent<Type>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeGlobalComponent = IsSerializeGlobalComponentV<Type>::value;
	template<typename Type> concept IsSerializeGlobalComponentC = IsSerializeGlobalComponent<Type>;

	template<typename Type> struct IsSerializeOptionalComponentV : FalseType {};
	template<typename Type> struct IsSerializeOptionalComponentV<SerializeOptionalComponent<Type>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeOptionalComponent = IsSerializeOptionalComponentV<Type>::value;
	template<typename Type> concept IsSerializeOptionalComponentC = IsSerializeOptionalComponent<Type>;

	template<typename Type> struct IsSerializeFixedArrayComponentV : FalseType {};
	template<typename Type> struct IsSerializeFixedArrayComponentV<SerializeFArrayComponent<Type>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeFixedArrayComponent = IsSerializeFixedArrayComponentV<Type>::value;
	template<typename Type> concept IsSerializeFixedArrayComponentC = IsSerializeFixedArrayComponent<Type>;

	template<typename Type> struct IsSerializeArrayComponentV : FalseType {};
	template<typename Type> struct IsSerializeArrayComponentV<SerializeArrayComponent<Type>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeArrayComponent = IsSerializeArrayComponentV<Type>::value;
	template<typename Type> concept IsSerializeArrayComponentC = IsSerializeArrayComponent<Type>;

	template<typename Type> struct IsSerializeDicKeyComponentV : FalseType {};
	template<typename Type> struct IsSerializeDicKeyComponentV<SerializeDicKeyComponent<Type>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeDicKeyComponent = IsSerializeDicKeyComponentV<Type>::value;
	template<typename Type> concept IsSerializeDicKeyComponentC = IsSerializeDicKeyComponent<Type>;

	template<typename Type> struct IsSerializeDicValueComponentV : FalseType {};
	template<typename Type> struct IsSerializeDicValueComponentV<SerializeDicValueComponent<Type>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeDicValueComponent = IsSerializeDicValueComponentV<Type>::value;
	template<typename Type> concept IsSerializeDicValueComponentC = IsSerializeDicValueComponent<Type>;

	template<typename Type> struct IsSerializeStructComponentV : FalseType {};
	template<typename StructType, typename MemberType, Size Index>
	struct IsSerializeStructComponentV<SerializeStructComponent<StructType, MemberType, Index>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeStructComponent = IsSerializeStructComponentV<Type>::value;
	template<typename Type> concept IsSerializeStructComponentC = IsSerializeStructComponent<Type>;

	template<typename Type> struct IsSerializeVariantComponentV : FalseType {};
	template<typename VariantType, typename ElementType, Size Index>
	struct IsSerializeVariantComponentV<SerializeVariantComponent<VariantType, ElementType, Index>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeVariantComponent = IsSerializeVariantComponentV<Type>::value;
	template<typename Type> concept IsSerializeVariantComponentC = IsSerializeVariantComponent<Type>;

	template<typename Type> struct IsSerializeComponentV : FalseType {};
	template<typename Type> constexpr inline Bool IsSerializeComponent = IsSerializeComponentV<Type>::value;
	template<typename Type> concept IsSerializeComponentC = IsSerializeComponent<Type>;

	template<typename Type> struct IsSerializeComponentV<SerializeGlobalComponent<Type>> : TrueType {};
	template<typename Type> struct IsSerializeComponentV<SerializeOptionalComponent<Type>> : TrueType {};
	template<typename Type> struct IsSerializeComponentV<SerializeFArrayComponent<Type>> : TrueType {};
	template<typename Type> struct IsSerializeComponentV<SerializeArrayComponent<Type>> : TrueType {};
	template<typename Type> struct IsSerializeComponentV<SerializeDicKeyComponent<Type>> : TrueType {};
	template<typename Type> struct IsSerializeComponentV<SerializeDicValueComponent<Type>> : TrueType {};
	template<typename StructType, typename MemberType, Size Index>
	struct IsSerializeComponentV<SerializeStructComponent<StructType, MemberType, Index>> : TrueType {};
	template<typename VariantType, typename ElementType, Size Index>
	struct IsSerializeComponentV<SerializeVariantComponent<VariantType, ElementType, Index>> : TrueType {};

	//types
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
		using type = Type;
		using serialize_type = SerializeTypeOf<Type>;
	};

	template<typename BaseType>
		requires(IsSerializableC<BaseType>)
	struct SerializeEnum {
		using base_type = BaseType;
		using base_serialize_type = SerializeTypeOf<BaseType>;
	};

	template<typename ElementType, Size Number>
		requires(IsSerializableC<ElementType>)
	struct SerializeFixedArray {
		using element_type = ElementType;
		using element_serialize_type = SerializeTypeOf<ElementType>;
		constexpr static inline Size size = Number;
	};

	template<typename ElementType>
		requires(IsSerializableC<ElementType>)
	struct SerializeArray {
		using element_type = ElementType;
		using element_serialize_type = SerializeTypeOf<ElementType>;
	};

	template<typename KeyType, typename ValueType>
		requires(IsSerializableC<KeyType>&& IsSerializableC<ValueType>)
	struct SerializeDic {
		using key_type = KeyType;
		using key_serialize_type = SerializeTypeOf<KeyType>;
		using value_type = ValueType;
		using value_serialize_type = SerializeTypeOf<ValueType>;
	};

	template<typename KeyType, typename ValueType>
		requires(IsSerializableC<KeyType>&& IsSerializableC<ValueType>)
	struct SerializeDicElement {
		using key_type = KeyType;
		using key_serialize_type = SerializeTypeOf<KeyType>;
		using value_type = ValueType;
		using value_serialize_type = SerializeTypeOf<ValueType>;
	};

	template<typename... MemberTypes>
		requires(IsSerializableC<MemberTypes> && ...)
	struct SerializeStruct {
		using members = TypePack<MemberTypes...>;
		using members_serialize_types = TypePack<SerializeTypeOf<MemberTypes>...>;
	};

	template<typename IndexType, typename... Types>
		requires(IsSerializableC<IndexType>
	&& IsVariantIndexSerializeTypeC<SerializeTypeOf<IndexType>>
		&& (IsSerializableC<Types> && ...))
		struct SerializeVariant {
		using types = TypePack<Types...>;
		using serialize_types = TypePack<SerializeTypeOf<Types>...>;
		using index_type = IndexType;
		using index_serialize_type = SerializeTypeOf<IndexType>;
	};

	template<typename IdNumberType>
		requires(IsBuiltInUnsignedIntegerC<IdNumberType>)
	struct SerializeJumpTable {
		using id_number_type = IdNumberType;
	};



	//type traits
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
	template<typename IdNumberType>
	struct IsSerializeTypeV<SerializeJumpTable<IdNumberType>> : TrueType {};

	template<typename Type> struct IsSerializeI8V : FalseType {};
	template<> struct IsSerializeI8V<SerializeI8> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI8 = IsSerializeI8V<Type>::value;
	template<typename Type> concept IsSerializeI8C = IsSerializeI8<Type>;

	template<typename Type> struct IsSerializeI16V : FalseType {};
	template<> struct IsSerializeI16V<SerializeI16> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI16 = IsSerializeI16V<Type>::value;
	template<typename Type> concept IsSerializeI16C = IsSerializeI16<Type>;

	template<typename Type> struct IsSerializeI32V : FalseType {};
	template<> struct IsSerializeI32V<SerializeI32> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI32 = IsSerializeI32V<Type>::value;
	template<typename Type> concept IsSerializeI32C = IsSerializeI32<Type>;

	template<typename Type> struct IsSerializeI64V : FalseType {};
	template<> struct IsSerializeI64V<SerializeI64> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeI64 = IsSerializeI64V<Type>::value;
	template<typename Type> concept IsSerializeI64C = IsSerializeI64<Type>;

	template<typename Type> struct IsSerializeUI8V : FalseType {};
	template<> struct IsSerializeUI8V<SerializeUI8> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI8 = IsSerializeUI8V<Type>::value;
	template<typename Type> concept IsSerializeUI8C = IsSerializeUI8<Type>;

	template<typename Type> struct IsSerializeUI16V : FalseType {};
	template<> struct IsSerializeUI16V<SerializeUI16> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI16 = IsSerializeUI16V<Type>::value;
	template<typename Type> concept IsSerializeUI16C = IsSerializeUI16<Type>;

	template<typename Type> struct IsSerializeUI32V : FalseType {};
	template<> struct IsSerializeUI32V<SerializeUI32> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI32 = IsSerializeUI32V<Type>::value;
	template<typename Type> concept IsSerializeUI32C = IsSerializeUI32<Type>;

	template<typename Type> struct IsSerializeUI64V : FalseType {};
	template<> struct IsSerializeUI64V<SerializeUI64> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeUI64 = IsSerializeUI64V<Type>::value;
	template<typename Type> concept IsSerializeUI64C = IsSerializeUI64<Type>;

	template<typename Type> struct IsSerializeF32V : FalseType {};
	template<> struct IsSerializeF32V<SerializeF32> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeF32 = IsSerializeF32V<Type>::value;
	template<typename Type> concept IsSerializeF32C = IsSerializeF32<Type>;

	template<typename Type> struct IsSerializeF64V : FalseType {};
	template<> struct IsSerializeF64V<SerializeF64> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeF64 = IsSerializeF64V<Type>::value;
	template<typename Type> concept IsSerializeF64C = IsSerializeF64<Type>;

	template<typename Type> struct IsSerializeBoolV : FalseType {};
	template<> struct IsSerializeBoolV<SerializeBool> : TrueType {};
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

	template<typename Type> struct IsSerializeJumpTableV : FalseType {};
	template<typename IdNumberType> struct IsSerializeJumpTableV<SerializeJumpTable<IdNumberType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeJumpTable = IsSerializeJumpTableV<Type>::value;
	template<typename Type> concept IsSerializeJumpTableC = IsSerializeJumpTable<Type>;

	template<typename Serializer>
	using CustomSerializeWriteFlag = Serializer::custom_write_flag_type;

	template<typename Serializer>
	constexpr  CustomSerializeWriteFlag<Serializer> DefaultCustomSerializeWriteFlag = Serializer::defaultCustomWriteFlag;

	enum class SerializeWriteFlag {
		none = 0,
		fullname,
	};

	template<typename Serializer>
	concept IsSerializerC = requires(Serializer & serializer, const ConstAsciiStringView & name) {
		typename Serializer::custom_write_flag_type;
		{
			serializer.template beginWriteNamed<
				SerializeWriteFlag::none,
				DefaultCustomSerializeWriteFlag<Serializer>,
				SerializeGlobalComponent<i8>, SerializeI8>(name)
		} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
		{
			serializer.template endWriteNamed<
				SerializeWriteFlag::none,
				DefaultCustomSerializeWriteFlag<Serializer>,
				SerializeGlobalComponent<i8>>()
		} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
		{
			serializer.template writeValue<
				SerializeWriteFlag::none,
				DefaultCustomSerializeWriteFlag<Serializer>,
				SerializeGlobalComponent<i8>>()
		} -> IsSameC<Option<StandardSerializeError<Serializer>>>;
		{ serializer.begin() } -> IsSameC<Option<StandardSerializeError<Serializer>>>;
		{ serializer.end() } -> IsSameC<Option<StandardSerializeError<Serializer>>>;
	};
	template<typename Type> constexpr inline Bool IsSerializer = IsSerializerC<Type>;
	template<typename Type> struct IsSerializerV : BoolConstant<IsSerializer<Type>> {};

	template<typename Functor, typename EnumIntegerType>
	concept IsStringToSerializeFlagConvertFunctor = requires(Functor functor, const ConstAsciiStringView & value) {
		{ functor(value) } -> IsSameC<Option<EnumIntegerType>>;
	};
	template<typename Functor, typename IndexIntegerType>
	concept IsStringToSerializeVariantIndexConvertFunctor = requires(Functor functor, const ConstAsciiStringView & str) {
		{ functor(str) } -> IsSameC<Option<IndexIntegerType>>;
	};

	namespace impl {
		template<typename StructType, typename SerializeType>
			requires(IsSerializeStructC<SerializeType>)
		consteval static auto generateStructMemberComponentsType() noexcept {
			return[]<Size... Indices>(IndexSequence<Indices...>) {
				return TypePack<SerializeStructComponent<StructType, SerializeType::members::template at<Indices>, Indices>...>{};
			}(MakeIndexSequence<SerializeType::members::size>{});
		}
	}

	template<typename StructType, typename SerializeType>
	using SerializeStructMemberComponents = decltype(impl::generateStructMemberComponentsType<StructType, SerializeType>());

	//serialize write
	template<typename Serializer, typename SerializeType>
	struct CanSerializeV : FalseType {};
	template<typename Serializer, typename SerializeType>
	constexpr Bool CanSerialize = CanSerializeV<Serializer, SerializeType>::value;
	template<typename Serializer, typename SerializeType>
	concept CanSerializeC = CanSerialize<Serializer, SerializeType>;

	template<typename Serializer, typename Type>
	using SerializeErrorOf = Serialize<Decay<Type>>::template error_type<Serializer>;

	template<typename Serializer, typename Type>
	using SerializeErrorResult = Option<SerializeErrorOf<Serializer, Type>>;

	template<typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
			&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWrite(Serializer& serializer, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		return Serialize<Decay<Type>>::template write<Serializer, Flags, CustomFlags, SerializeComponentType>(
			serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
			&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamed(
			Serializer& serializer,
			const ConstAsciiStringView& name,
			const Type& serializeValue,
			SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using as_type = decayed_serialize_type::as_type;

		auto beginError = serializer.template beginWriteNamed<Flags, CustomFlags, SerializeComponentType, as_type>(name);
		if (beginError.hasValue()) { return beginError.value(); }

		auto valueError = serializer.template writeValue<Flags, CustomFlags, SerializeComponentType>();
		if (valueError.hasValue()) { return valueError.value(); }

		SerializeErrorResult<Serializer, Type> result = serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
			serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		if (result.hasValue()) { return result.value(); };

		auto endError = serializer.template endWriteNamed<Flags, CustomFlags, SerializeComponentType>();
		if (endError.hasValue()) { return endError.value(); }
		return natl::OptionEmpty{};
	}

	template<typename Serializer, typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
			&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedDefault(
			Serializer& serializer, const ConstAsciiStringView& name,
			const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		return serializeWriteNamed<Serializer, natl::SerializeWriteFlag::none, natl::DefaultCustomSerializeWriteFlag<Serializer>, SerializeComponentType, Type, SerializeArgs...>
			(serializer, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer> && IsSerializableC<Decay<Type>> && CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedDefaultGlobal(
			Serializer& serializer, const ConstAsciiStringView& name, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		return serializeWriteNamed<Serializer, natl::SerializeWriteFlag::none, natl::DefaultCustomSerializeWriteFlag<Serializer>, SerializeGlobalComponent<Type>, Type, SerializeArgs...>
			(serializer, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	//serialize write match 
	template<typename SerilizeType, typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
			&& IsSerializeTypeC<SerilizeType>&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWriteMatch(
			Serializer& serializer, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		return serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
			serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename SerilizeType, typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
			&& IsSerializeTypeC<SerilizeType> && CanSerializeC<Serializer, Decay<Type>>
			&& IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedMatch(
			Serializer& serializer,
			const ConstAsciiStringView& name,
			const Type& serializeValue,
			SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using as_type = decayed_serialize_type::as_type;

		auto beginError = serializer.template beginWriteNamed<Flags, CustomFlags, SerializeComponentType, as_type>(name);
		if (beginError.hasValue()) { return beginError.value(); }

		auto valueError = serializer.template writeValue<Flags, CustomFlags, SerializeComponentType>();
		if (valueError.hasValue()) { return valueError.value(); }

		SerializeErrorResult<Serializer, Type> result = serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
			serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		if (result.hasValue()) { return result.value(); };

		auto endError = serializer.template endWriteNamed<Flags, CustomFlags, SerializeComponentType>();
		if (endError.hasValue()) { return endError.value(); }
		return natl::OptionEmpty{};
	}

	template<typename SerilizeType, typename Serializer, typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
			&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>
			&& IsSerializeTypeC<SerilizeType> && IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedDefaultMatch(
			Serializer& serializer, const ConstAsciiStringView& name,
			const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		return serializeWriteNamed<Serializer, natl::SerializeWriteFlag::none, natl::DefaultCustomSerializeWriteFlag<Serializer>, SerializeComponentType, Type, SerializeArgs...>
			(serializer, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	template<typename SerilizeType, typename Serializer, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer> && IsSerializableC<Decay<Type>>
			&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>
			&& IsSerializeTypeC<SerilizeType>&& IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedDefaultGlobalMatch(
			Serializer& serializer, const ConstAsciiStringView& name,
			const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		return serializeWriteNamed<Serializer, natl::SerializeWriteFlag::none, natl::DefaultCustomSerializeWriteFlag<Serializer>, SerializeGlobalComponent<Type>, Type, SerializeArgs...>
			(serializer, name, serializeValue, natl::forward<SerializeArgs>(serializeArgs)...);
	}

	//deserialize 
	enum class CustomDeserializerReadFlagNone {
		none = 0
	};

	template<typename Deserializer>
	using CustomDeserializeReadFlag = Deserializer::custom_read_flag_type;

	template<typename Deserializer>
	constexpr inline CustomDeserializeReadFlag<Deserializer> DefaultCustomDeserializeReadFlag = Deserializer::defaultCustomReadFlag;

	enum class DeserializeReadFlag {
		none = 0,
		fullname,
	};

	enum class DeserializeFlags {
		shortenError,
	};

	struct DeserializeGlobalScope {};
	template<> struct IsSerializeTypeV<DeserializeGlobalScope> : TrueType {};

	enum struct DeserializeErrorFlag {
		none = 0,
		endOfSource,
		wrongType,
		wrongFormatting,
		wrongName,
		valueParsing,
		jump,
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
		case DeserializeErrorFlag::jump:
			return "jump";
		default:
			unreachable();
		}
	}

	enum struct DeserializeErrorLocation {
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
		readIsEmptyOptional,
		beginReadOptional,
		endReadOptional,
		readEnum,
		beginReadFixedArray,
		endReadFixedArray,
		endReadEmptyFixedArray,
		beginReadFixedArrayElement,
		endReadFixedArrayElement,
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
		beginReadJumpTable,
		endReadJumpTable,
		readJumpTableElement,
		readJumpLocation,
		jumpTo,
		skip,
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
		case DeserializeErrorLocation::readIsEmptyOptional:
			return "readIsEmptyOptional";
		case DeserializeErrorLocation::beginReadOptional:
			return "beginReadOptional";
		case DeserializeErrorLocation::endReadOptional:
			return "endReadOptional";
		case DeserializeErrorLocation::readEnum:
			return "readEnum";
		case DeserializeErrorLocation::beginReadFixedArray:
			return "beginReadFixedArray";
		case DeserializeErrorLocation::endReadFixedArray:
			return "endReadFixedArray";
		case DeserializeErrorLocation::beginReadFixedArrayElement:
			return "beginReadFixedArrayElement";
		case DeserializeErrorLocation::endReadFixedArrayElement:
			return "endReadFixedArrayElement";
		case DeserializeErrorLocation::endReadEmptyFixedArray:
			return "endReadEmptyFixedArray";
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
		case DeserializeErrorLocation::beginReadJumpTable:
			return "beginReadJumpTable";
		case DeserializeErrorLocation::endReadJumpTable:
			return "endReadJumpTable";
		case DeserializeErrorLocation::readJumpTableElement:
			return "readJumpTableElement";
		case DeserializeErrorLocation::readJumpLocation:
			return "readJumpLocation";
		case DeserializeErrorLocation::jumpTo:
			return "jumpTo";
		case DeserializeErrorLocation::skip:
			return "skip";
		default:
			unreachable();
		}
	}

	template<typename SerializeType>
	struct DummyDeserializeElementInfo {
		using serialize_type = SerializeType;
	};

	struct DummyDeserializeErrorHandler {
		constexpr DummyDeserializeErrorHandler(
			const ConstAsciiStringView&,
			const ConstAsciiStringView& locationDetails,
			const DeserializeErrorLocation&,
			const DeserializeErrorFlag&) noexcept {
		}
		constexpr Bool hasMessage() const noexcept { return false; }
		constexpr DummyDeserializeErrorHandler& addSource(const ConstAsciiStringView&) noexcept { 
			return *this; 
		}
		constexpr DummyDeserializeErrorHandler& addSource(
			const ConstAsciiStringView&, const ConstAsciiStringView&) noexcept {
			return *this;
		}

		template<typename DynStringType>
		constexpr void getMessageTo(DynStringType&) const noexcept {}
		template<typename DynStringType>
		constexpr DynStringType getMessage() const noexcept { return DynStringType(); }
		template<typename DynStringType>
		constexpr void getErrorMessageTo(DynStringType&) const noexcept {}
		template<typename DynStringType>
		constexpr DynStringType getErrorMessage() const noexcept { return DynStringType(); }
	};

	struct DummyDeserializer {
		template<typename SerializeType>
		using deserialize_info = DummyDeserializeElementInfo<SerializeType>;
		using deserialize_error_handler = DummyDeserializeErrorHandler;
		using error_message_string_type = void;
	};

	//is serialize error handler 
	template<typename DeserializerErrorHandlerType>
	concept IsDeserializeErrorHandlerC = requires(
		const DeserializerErrorHandlerType& errorHandler,
		DeserializerErrorHandlerType& sourceErrorHandler,
		const ConstAsciiStringView& errorMessage,
		const ConstAsciiStringView& locationDetails,
		const DeserializeErrorLocation & errorLocation,
		const DeserializeErrorFlag& errorFlag,
		const ConstAsciiStringView& source,
		const ConstAsciiStringView& element) {
			{ DeserializerErrorHandlerType(errorMessage, locationDetails, errorLocation, errorFlag) };
			{ errorHandler.hasMessage() } -> IsSameC<natl::Bool>;
			{ sourceErrorHandler.addSource(source) } -> IsSameC<DeserializerErrorHandlerType&>;
			{ sourceErrorHandler.addSource(source, element) } -> IsSameC<DeserializerErrorHandlerType&>;
			{ errorHandler.template getMessageTo<DummyString>(declref<DummyString>()) } -> IsSameC<void>;
			{ errorHandler.template getMessage<DummyString>() } -> IsConvertibleC<DummyString>;
	};
	template<typename Type> constexpr inline Bool IsDeserializeErrorHandler = IsDeserializeErrorHandlerC<Type>;
	template<typename Type> struct IsDeserializeErrorHandlerV : BoolConstant<IsDeserializeErrorHandlerC<Type>> {};
	static_assert(IsDeserializeErrorHandlerC<DummyDeserializeErrorHandler>);

	template<typename Type> struct Deserialize {};

	template<typename Type> concept IsDeserializableC = requires() {
		typename Deserialize<Decay<Type>>;
		typename Deserialize<Decay<Type>>::as_type;
		typename Deserialize<Decay<Type>>::type;
		typename Deserialize<Decay<Type>>::template error_type<DummyDeserializer>;
	};
	template<typename Type> struct IsDeserializableV : BoolConstant<IsDeserializableC<Type>> {};
	template<typename Type> constexpr Bool IsDeserializable = IsDeserializableV<Type>::value;

	template<typename Type>
		requires(IsDeserializableC<Type>)
	using DeserializeTypeOf = typename Deserialize<Type>::as_type;

	//deserialize name
	template<typename Type>
	concept HasRegularDeserializeNameC = IsDeserializableC<Type> && requires() {
		{ Deserialize<Decay<Type>>::name } -> ConvertibleTo<ConstAsciiStringView>;
	};
	template<typename Type> constexpr inline Bool HasRegularDeserializeName = HasRegularDeserializeNameC<Type>;
	template<typename Type> struct HasRegularDeserializeNameV : BoolConstant<HasRegularDeserializeNameC<Type>> {};

	template<typename Type>
	constexpr inline ConstAsciiStringView RegularDeserializeName = Deserialize<Type>::name;

	template<typename Type>
	struct OverrideDeserializeNameV {
		using not_specialized = void;
	};

	template<typename Type>
	concept HasOverrideDeserializeNameC = !requires() {
		typename OverrideDeserializeNameV<Type>::not_specialized;
	};
	template<typename Type> constexpr inline Bool HasOverrideDeserializeName = HasOverrideDeserializeNameC<Type>;
	template<typename Type> struct HasOverrideDeserializeNameV : BoolConstant<HasOverrideDeserializeNameC<Type>> {};

	template<typename Type>
	constexpr inline ConstAsciiStringView OverrideDeserializeName = OverrideDeserializeNameV<Type>::value;

	template<typename Type>
	concept HasDeserializeNameC = HasRegularDeserializeNameC<Type> || HasOverrideDeserializeNameC<Type>;
	template<typename Type> constexpr inline Bool HasDeserializeName = HasDeserializeNameC<Type>;
	template<typename Type> struct HasDeserializeNameV : BoolConstant<HasDeserializeNameC<Type>> {};

	template<typename Type>
		requires(HasDeserializeNameC<Type>)
	struct DeserializeNameV {
		consteval static ConstAsciiStringView getName() noexcept {
			if constexpr (HasOverrideDeserializeNameC<Type>) {
				return OverrideDeserializeName<Type>;
			} else {
				return RegularDeserializeName<Type>;
			}
		}
		constexpr static inline ConstAsciiStringView value = getName();
	};

	template<typename Type>
	constexpr inline ConstAsciiStringView DeserializeName = DeserializeNameV<Type>::value;

	//deserialize info
	template<typename Deserializer, typename SerializeType>
	concept HasDeserializeInfoC = requires() {
		typename Deserializer::template deserialize_info<SerializeType>;
	};
	template<typename Deserializer, typename SerializeType>
	constexpr inline Bool HasDeserializeInfo = HasDeserializeInfoC<Deserializer, SerializeType>;
	template<typename Deserializer, typename SerializeType>
	struct HasDeserializeInfoV : BoolConstant<HasDeserializeInfoC<Deserializer, SerializeType>> {};

	template<typename Deserializer, typename SerializeType>
	using DeserializeInfo = typename Deserializer::template deserialize_info<SerializeType>;
	template<typename Deserializer, typename Type>
	using DeserializeInfoOf = typename Deserializer::template deserialize_info<DeserializeTypeOf<Type>>;

	template<typename Deserializer>
	concept HasDeserializeInfoGlobalC = HasDeserializeInfoC<Deserializer, DeserializeGlobalScope>;
	template<typename Deserializer>
	constexpr inline Bool HasDeserializeInfoGlobal = HasDeserializeInfoGlobalC<Deserializer>;
	template<typename Deserializer>
	struct HasDeserializeInfoGlobalV : BoolConstant<HasDeserializeInfoGlobalC<Deserializer>> {};

	template<typename Deserializer>
	using DeserializeInfoGlobal = DeserializeInfo<Deserializer, DeserializeGlobalScope>;

	//deserialize error
	template<typename Deserializer>
	using StandardDeserializeError = Deserializer::deserialize_error_handler;

	template<typename Deserializer, typename Type> constexpr inline Bool IsStandardDeserializeError = IsSameC<
		typename Decay<Deserializer>::deserialize_error_handler, Type>;
	template<typename Deserializer, typename Type> struct IsStandardDeserializeErrorV
		: BoolConstant<IsStandardDeserializeError<Deserializer, Type>> {
	};
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
	template<typename Deserializer, typename Type>
	using DeserializeExpect = Expect<Type, DeserializeErrorOf<Deserializer, Type>>;

	template<typename Deserializer>
	concept IsDeserializerC = HasDeserializeInfoGlobalC<Deserializer> && requires(Deserializer & deserializer,
		DeserializeInfoGlobal<Deserializer>&globalScope,
		const ConstAsciiStringView & name) {
		typename Deserializer::custom_read_flag_type;
		{
			deserializer.template beginReadNamed<
				DeserializeReadFlag::none,
				DefaultCustomDeserializeReadFlag<Deserializer>,
				SerializeGlobalComponent<i8>, i8>(globalScope, name)
		} -> IsSameC<Expect<DeserializeInfo<Deserializer, SerializeI8>, StandardDeserializeError<Deserializer>>>;
		{
			deserializer.template endReadNamed<
				DeserializeReadFlag::none,
				DefaultCustomDeserializeReadFlag<Deserializer>,
				SerializeGlobalComponent<i8>>(declref<DeserializeInfo<Deserializer, i8>>())
		} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
		{
			deserializer.template begin<DeserializeReadFlag::none,
				DefaultCustomDeserializeReadFlag<Deserializer>>()
		} -> IsSameC<Expect<DeserializeInfoGlobal<Deserializer>, StandardDeserializeError<Deserializer>>>;
		{
			deserializer.template end<DeserializeReadFlag::none,
				DefaultCustomDeserializeReadFlag<Deserializer>>(
					declref<DeserializeInfoGlobal<Deserializer>>()
					)
		} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Type> constexpr inline Bool IsDeserializer = IsDeserializerC<Type>;
	template<typename Type> struct IsDeserializerV : BoolConstant<IsDeserializerC<Type>> {};

	template<typename Deserializer, typename Type>
	struct CanDeserializeV : FalseType {};
	template<typename Deserializer, typename Type>
	constexpr inline Bool CanDeserialize = CanDeserializeV<Deserialize, Type>::value;
	template<typename Serializer, typename SerializeType>

	//TODO 
	concept CanDeserializeC = true;

	//deserialize read
	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentSerializeType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializeTypeC<ParentSerializeType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeRead(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
			DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "deserializeRead";

		Type value;
		auto readValueError = type_deserialize::template read<Deserializer, Flags, CustomFlags, SerializeComponentType>(
			deserializer, parent, value, forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return unexpected(readValueError.value().addSource(sourceName));
		}
		return value;
	}
	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamed(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			DeserializerArgs&&... deserializerArgs
		) noexcept {
		constexpr ConstAsciiStringView sourceName = "deserializeReadNamed";

		auto readNamedInfoExpect = deserializer.template beginReadNamed<Flags, CustomFlags, SerializeComponentType, Type, ParentType>(parent, name);
		if (readNamedInfoExpect.hasError()) {
			return unexpected(readNamedInfoExpect.error().addSource(sourceName, name));
		}
		auto readNamedInfo = readNamedInfoExpect.value();

		auto valueExpect = deserializeRead<Deserializer, Flags, CustomFlags, SerializeComponentType,
			Type, typename decltype(readNamedInfo)::serialize_type, DeserializerArgs...>(
				deserializer, readNamedInfo, forward<DeserializerArgs>(deserializerArgs)...);
		if (valueExpect.hasError()) {
			return unexpected(valueExpect.error().addSource(sourceName, name));
		}

		auto endReadNameError = deserializer.template endReadNamed<Flags, CustomFlags, SerializeComponentType, SerializeTypeOf<Type>>(readNamedInfo);
		if (endReadNameError.hasValue()) {
			return unexpected(endReadNameError.value().addSource(sourceName, name));
		}

		return valueExpect.value();
	}
	template<typename Deserializer, typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefault(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamed<Deserializer, DeserializeReadFlag::none, DefaultCustomDeserializeReadFlag<Deserializer>, 
			SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>> && CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefaultGlobal(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamed<Deserializer, DeserializeReadFlag::none, DefaultCustomDeserializeReadFlag<Deserializer>,
			SerializeGlobalComponent<Type>, Type, ParentType, DeserializerArgs...>(
				deserializer, parent, name, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}
	
	//deserialize read match
	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			DeserializerArgs&&... deserializerArgs) {
		return deserializeRead<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			DeserializerArgs&&... deserializerArgs
		) noexcept {
		return deserializeReadNamed<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename SerilizeType, typename Deserializer, typename SerializeComponentType, 
		typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefaultMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			DeserializerArgs&&... deserializerArgs
		) noexcept {
		return deserializeReadNamedDefault<Deserializer, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename SerilizeType, typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>> && CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefaultGlobalMatch(
			Deserializer& deserializer, typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name, DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedDefault<Deserializer, SerializeGlobalComponent<Type>, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, forward<DeserializerArgs>(deserializerArgs)...);
	}

	//deserilize read to dst
	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDst(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "deserializeReadToDst";

		auto readValueError = type_deserialize::template read<Deserializer, Flags, CustomFlags, SerializeComponentType>(
			deserializer, parent, dst, forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return readValueError.value().addSource(sourceName);
		}
		return {};
	}

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDst(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		constexpr ConstAsciiStringView sourceName = "deserializeReadNamedToDst";

		auto readNamedInfoExpect = deserializer.template beginReadNamed<Flags, CustomFlags, SerializeComponentType, Type, ParentType>(parent, name);
		if (readNamedInfoExpect.hasError()) {
			return readNamedInfoExpect.error().addSource(sourceName, name);
		}
		auto readNamedInfo = readNamedInfoExpect.value();

		auto error = deserializeReadToDst<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, typename decltype(readNamedInfo)::serialize_type, DeserializerArgs...>(
			deserializer, readNamedInfo, dst, forward<DeserializerArgs>(deserializerArgs)...);
		if (error.hasValue()) {
			return error.value().addSource(sourceName, name);
		}

		auto endReadNameError = deserializer.template endReadNamed<Flags, CustomFlags, SerializeComponentType, SerializeTypeOf<Type>>(readNamedInfo);
		if (endReadNameError.hasValue()) {
			return endReadNameError.value().addSource(sourceName, name);
		}

		return {};
	}

	template<typename Deserializer, typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefaultToDst(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, DeserializeReadFlag::none, DefaultCustomDeserializeReadFlag<Deserializer>, 
			SerializeComponentType, Type, ParentType, DeserializerArgs...>
			(deserializer, parent, name, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}
	template<typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>> && CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefaultGlobalToDst(
			Deserializer& deserializer, typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name, Type& dst, DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, DeserializeReadFlag::none, DefaultCustomDeserializeReadFlag<Deserializer>, 
			SerializeGlobalComponent<Type>, Type, ParentType, DeserializerArgs...>
			(deserializer, parent, name, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}

	//deserialize read to dst match
	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDstMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadToDstMatch<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDstMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename SerilizeType, typename Deserializer, typename SerializeComponentType, 
		typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefaultToDstMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, DeserializeReadFlag::none, 
			DefaultCustomDeserializeReadFlag<Deserializer>, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
				deserializer, parent, name, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}


	template<typename SerilizeType, typename Deserializer, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>> && CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
			&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	[[nodiscard]] constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedDefaultToDstMatch(
			Deserializer& deserializer, typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name, Type& dst, DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, DeserializeReadFlag::none,
			DefaultCustomDeserializeReadFlag<Deserializer>, SerializeGlobalComponent<Type>, Type, ParentType, DeserializerArgs...>(
				deserializer, parent, name, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}

	//deserialize skip 
	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentSerializeType>
		requires(IsDeserializableC<Type> && IsSerializeComponentC<SerializeComponentType>&& IsSerializeTypeC<ParentSerializeType>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeSkip(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
		const ConstAsciiStringView& name) noexcept {
		//using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "deserializeSkip";

		auto skipError = deserializer.template skip<Flags, CustomFlags, SerializeComponentType, Type, ParentSerializeType>(parent, name);
		if (skipError.hasValue()) {
			return skipError.value().addSource(sourceName, name);
		}

		return OptionEmpty{};
	}

	template<typename Deserializer, typename SerializeComponentType, typename Type, typename ParentSerializeType>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializeTypeC<ParentSerializeType>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeSkipDefault(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
		const ConstAsciiStringView& name) noexcept {
		return deserializeSkip<Deserializer, DeserializeReadFlag::none, DefaultCustomDeserializeReadFlag<Deserializer>,
			SerializeComponentType, Type, ParentSerializeType>(
			deserializer, parent, name);
	}

	template<typename Deserializer, typename Type, typename ParentSerializeType>
		requires(IsDeserializableC<Type> && IsSerializeTypeC<ParentSerializeType>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeSkipDefault(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
		const ConstAsciiStringView& name) noexcept {
		return deserializeSkip<Deserializer, DeserializeReadFlag::none, DefaultCustomDeserializeReadFlag<Deserializer>,
			SerializeGlobalComponent<Type>, Type, ParentSerializeType>(
				deserializer, parent, name);
	}

	namespace impl {
		template<typename ErrorType, typename Deserializer, typename DerializeInfoType, typename ReadFunctor>
		concept IsDeserializeReadFunctorWithArgs = requires(Option<ErrorType> error, Deserializer & deserializer,
			DerializeInfoType & info, ReadFunctor && readFunctor) {
				{ error = forward<ReadFunctor>(readFunctor)(deserializer, info) };
		};

		template<typename ErrorType, typename ReadFunctor>
		concept  IsDeserializeReadFunctorWithoutArgs = requires(Option<ErrorType> error, ReadFunctor && readFunctor) {
			{ error = forward<ReadFunctor>(readFunctor)() };
		};

		template<typename ErrorType, typename Deserializer, typename DerializeInfoType, typename ReadFunctor>
		concept IsDeserializeReadFunctor =
			IsDeserializeReadFunctorWithArgs<ErrorType, Deserializer, DerializeInfoType, ReadFunctor> ||
			IsDeserializeReadFunctorWithoutArgs<ErrorType, ReadFunctor>;

		template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename DerializeInfoType>
		constexpr void chainDeserializeReadHelper(
			Option<ErrorType>&, const ConstAsciiStringView&, Deserializer&,
			DerializeInfoType&) {
		}

		template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename DerializeInfoType,
			typename ReadFunctor, typename... OtherReadFunctors>
		constexpr void chainDeserializeReadHelper(
			Option<ErrorType>& error, const ConstAsciiStringView& source, Deserializer& deserializer,
			DerializeInfoType& info, ReadFunctor&& readFunctor, OtherReadFunctors&&... otherReadFunctors) noexcept {
			if constexpr (IsDeserializeReadFunctorWithArgs<ErrorType, Deserializer, DerializeInfoType, ReadFunctor>) {
				error = forward<ReadFunctor>(readFunctor)(deserializer, info);
			} else {
				error = forward<ReadFunctor>(readFunctor)();
			}

			if (error.hasValue()) {
				error.value().addSource(source, "");
				return;
			}

			chainDeserializeReadHelper<ErrorType, Deserializer, Flags, CustomFlags,
				SerializeComponentType, DerializeInfoType, OtherReadFunctors...>(
					error, source, deserializer, info,
					forward<OtherReadFunctors>(otherReadFunctors)...);
		}

		template<typename ErrorType>
		constexpr void chainDeserializeReadNoArgsHelper(
			Option<ErrorType>&, const ConstAsciiStringView&) {
		};

		template<typename ErrorType, typename ReadFunctor, typename... OtherReadFunctors>
		constexpr void chainDeserializeReadNoArgsHelper(
			Option<ErrorType>& error, const ConstAsciiStringView& source,
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


	template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename DerializeInfoType, typename... ReadFunctors>
		requires(IsSerializeComponentC<SerializeComponentType> && (impl::IsDeserializeReadFunctor<ErrorType, Deserializer, DerializeInfoType, ReadFunctors> && ...))
	constexpr Option<ErrorType> chainDeserializeRead(const ConstAsciiStringView& source, Deserializer& deserializer,
		DerializeInfoType& info, ReadFunctors&&... readFunctors) noexcept {
		Option<ErrorType> error = OptionEmpty{};
		impl::chainDeserializeReadHelper<ErrorType, Deserializer, Flags, CustomFlags, SerializeComponentType, DerializeInfoType, ReadFunctors...>(
			error, source, deserializer, info, forward<ReadFunctors>(readFunctors)...);
		return error;
	}

	template<typename ErrorType, typename... ReadFunctors>
		requires(impl::IsDeserializeReadFunctorWithoutArgs<ErrorType, ReadFunctors> && ...)
	constexpr Option<ErrorType> chainDeserializeReadNoArgs(
		const ConstAsciiStringView& source, ReadFunctors&&... readFunctors) noexcept {
		Option<ErrorType> error = OptionEmpty{};
		impl::chainDeserializeReadNoArgsHelper<ErrorType, ReadFunctors...>(
			error, source, forward<ReadFunctors>(readFunctors)...);
		return error;
	}

	template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename DerializeInfoType, typename... ReadFunctors>
		requires(IsSerializeComponentC<SerializeComponentType> && (impl::IsDeserializeReadFunctorWithArgs<ErrorType, Deserializer, DerializeInfoType, ReadFunctors> && ...))
	constexpr Option<ErrorType> deserializeReadStruct(const ConstAsciiStringView& source, Deserializer& deserializer,
		DerializeInfoType& info, ReadFunctors&&... readFunctors) {
		auto beginStructError = deserializer.template beginReadStruct<Flags, CustomFlags, SerializeComponentType>(info);
		if (beginStructError.hasValue()) {
			return beginStructError.value().addSource(source, "");
		}
		auto& structInfo = info;

		auto memberError = chainDeserializeRead<ErrorType, Deserializer, Flags, CustomFlags, SerializeComponentType, decltype(structInfo), ReadFunctors...>(
			source, deserializer, structInfo, forward<ReadFunctors>(readFunctors)...);
		if (memberError.hasValue()) {
			return memberError.value();
		}

		auto structEndError = deserializer.template endReadStruct<Flags, CustomFlags, SerializeComponentType>(structInfo);
		if (structEndError.hasValue()) {
			return structEndError.value().addSource(source, "");
		}
		return {};
	}
}