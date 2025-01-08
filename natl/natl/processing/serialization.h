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
	enum class SerializeFlag {
		v_default = 0,
		pretty = 1 << 0,
		fullTypes = 1 << 1,
	};

	constexpr inline SerializeFlag operator|(const SerializeFlag lhs, const SerializeFlag rhs) noexcept {
		using underlying_type = natl::UnderlyingType<SerializeFlag>; 
		return static_cast<SerializeFlag>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs));
	} constexpr inline SerializeFlag operator&(const SerializeFlag lhs, const SerializeFlag rhs) noexcept {
		using underlying_type = natl::UnderlyingType<SerializeFlag>; 
		return static_cast<SerializeFlag>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs));
	};

	struct DummySerializer {
		using allocator_type = DefaultAllocator<natl::Ascii>;
		using code_point_type = natl::Ascii;
		using container_type = void;

		constexpr static inline natl::Size smallBufferSize = 0;
		constexpr static inline natl::SerializeFlag flag = natl::SerializeFlag::v_default;
	};

	template<typename Type> 
	struct Serialize;

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
	struct SerializeGlobalComponent {};

	template<typename Type>
		requires(IsSerializableC<Type>)
	struct SerializeOptionalComponent {
		using type = Type;
	};

	template<typename ArrayType>
		requires(IsSerializableC<ArrayType>)
	struct SerializeFArrayComponent {
		using array_type = ArrayType;
		using parent_type = array_type;
	};
	template<typename ArrayType>
		requires(IsSerializableC<ArrayType>)
	struct SerializeArrayComponent {
		using array_type = ArrayType;
	};

	template<typename DicType>
		requires(IsSerializableC<DicType>)
	struct SerializeDicKeyComponent {
		using dic_type = DicType;
		using parent_type = dic_type;
	};
	template<typename DicType>
		requires(IsSerializableC<DicType>)
	struct SerializeDicValueComponent {
		using dic_type = DicType;
		using parent_type = dic_type;
	};

	template<typename StructType, Size Index>
		requires(IsSerializableC<StructType>)
	struct SerializeStructComponent {
		using struct_type = StructType;
		constexpr static inline Size index = Index;
	};

	template<typename VariantType, Size Index>
		requires(IsSerializableC<VariantType>)
	struct SerializeVaraintComponent {
		using varaint_type = VariantType;
		using parent_type = varaint_type;
		constexpr static inline Size index = Index;
	};

	template<typename Type> struct IsSerializeGlobalComponentV : FalseType {};
	template<> struct IsSerializeGlobalComponentV<SerializeGlobalComponent> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeGlobalComponent = IsSerializeGlobalComponentV<Type>::value;
	template<typename Type> concept IsSerializeGlobalComponentC = IsSerializeGlobalComponent<Type>;

	template<typename Type> struct IsSerializeOptionalComponentV : FalseType {};
	template<typename Type> struct IsSerializeOptionalComponentV<SerializeOptionalComponent<Type>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeOptionalComponent = IsSerializeOptionalComponentV<Type>::value;
	template<typename Type> concept IsSerializeOptionalComponentC = IsSerializeOptionalComponent<Type>;

	template<typename Type> struct IsSerializeFArrayComponentV : FalseType {};
	template<typename ArrayType> struct IsSerializeFArrayComponentV<SerializeFArrayComponent<ArrayType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeFArrayComponent = IsSerializeFArrayComponentV<Type>::value;
	template<typename Type> concept IsSerializeFArrayComponentC = IsSerializeFArrayComponent<Type>;

	template<typename Type> struct IsSerializeArrayComponentV : FalseType {};
	template<typename ArrayType> struct IsSerializeArrayComponentV<SerializeArrayComponent<ArrayType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeArrayComponent = IsSerializeArrayComponentV<Type>::value;
	template<typename Type> concept IsSerializeArrayComponentC = IsSerializeArrayComponent<Type>;

	template<typename Type> struct IsSerializeDicKeyComponentV : FalseType {};
	template<typename DicType> struct IsSerializeDicKeyComponentV<SerializeDicKeyComponent<DicType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeDicKeyComponent = IsSerializeDicKeyComponentV<Type>::value;
	template<typename Type> concept IsSerializeDicKeyComponentC = IsSerializeDicKeyComponent<Type>;

	template<typename Type> struct IsSerializeDicValueComponentV : FalseType {};
	template<typename DicType> struct IsSerializeDicValueComponentV<SerializeDicValueComponent<DicType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeDicValueComponent = IsSerializeDicValueComponentV<Type>::value;
	template<typename Type> concept IsSerializeDicValueComponentC = IsSerializeDicValueComponent<Type>;

	template<typename Type> struct IsSerializeStructComponentV : FalseType {};
	template<typename StructType, Size Index> struct IsSerializeStructComponentV<SerializeStructComponent<StructType, Index>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeStructComponent = IsSerializeStructComponentV<Type>::value;
	template<typename Type> concept IsSerializeStructComponentC = IsSerializeStructComponent<Type>;

	template<typename Type> struct IsSerializeVaraintComponentV : FalseType {};
	template<typename VariantType, Size Index> struct IsSerializeVaraintComponentV<SerializeVaraintComponent<VariantType, Index>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeVaraintComponent = IsSerializeVaraintComponentV<Type>::value;
	template<typename Type> concept IsSerializeVaraintComponentC = IsSerializeVaraintComponent<Type>;

	template<typename Type> struct IsSerializeComponentV : FalseType {};
	template<typename Type> constexpr inline Bool IsSerializeComponent = IsSerializeComponentV<Type>::value;
	template<typename Type> concept IsSerializeComponentC = IsSerializeComponent<Type>;

	template<> struct IsSerializeComponentV<SerializeGlobalComponent> : TrueType {};
	template<typename Type> struct IsSerializeComponentV<SerializeOptionalComponent<Type>> : TrueType {};
	template<typename ArrayType> struct IsSerializeComponentV<SerializeFArrayComponent<ArrayType>> : TrueType {};
	template<typename ArrayType> struct IsSerializeComponentV<SerializeArrayComponent<ArrayType>> : TrueType {};
	template<typename DicType> struct IsSerializeComponentV<SerializeDicKeyComponent<DicType>> : TrueType {};
	template<typename DicType> struct IsSerializeComponentV<SerializeDicValueComponent<DicType>> : TrueType {};
	template<typename StructType, Size Index> struct IsSerializeComponentV<SerializeStructComponent<StructType, Index>> : TrueType {};
	template<typename VariantType, Size Index> struct IsSerializeComponentV<SerializeVaraintComponent<VariantType, Index>> : TrueType {};

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
		requires(IsSerializableC<KeyType> && IsSerializableC<ValueType>)
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

	template<typename Type> struct IsSerializeJumpTableV : FalseType {};
	template<typename IdNumberType> struct IsSerializeJumpTableV<SerializeJumpTable<IdNumberType>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeJumpTable = IsSerializeJumpTableV<Type>::value;
	template<typename Type> concept IsSerializeJumpTableC = IsSerializeJumpTable<Type>;

	template<typename Serializer>
	using CustomSerializeWriteFlag = Serializer::custom_write_flag_type;

	template<typename Serializer>
	constexpr  CustomSerializeWriteFlag<Serializer> DefaultCustomSerializeWriteFlag = Serializer::defaultCustomWriteFlag;

	enum class SerializeWriteFlag {
		v_default = 0,
		fullname,
	};

	template<typename Serilizer>
	concept IsSerializerC = requires(Serilizer & serializer, const ConstAsciiStringView& name) {
		typename Serilizer::custom_write_flag_type;
		{ serializer.template beginWrite<
			SerializeWriteFlag::v_default,
			DefaultCustomSerializeWriteFlag<Serilizer>,
			SerializeGlobalComponent, SerializeI8>(name)
		} -> IsSameC<void>;
		{
			serializer.template endWrite<
				SerializeWriteFlag::v_default,
				DefaultCustomSerializeWriteFlag<Serilizer>,
				SerializeGlobalComponent>()
		} -> IsSameC<void>;
		{
			serializer.template writeValue<
				SerializeWriteFlag::v_default,
				DefaultCustomSerializeWriteFlag<Serilizer>,
				SerializeGlobalComponent>()
		} -> IsSameC<void>;
		{ serializer.begin() } -> IsSameC<void>;
		{ serializer.end() } -> IsSameC<void>;
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
			return[]<Size... Indices>(natl::IndexSequence<Indices...>) {
				return TypePack<SerializeStructComponent<StructType, Indices>...>{};
			}(natl::MakeIndexSequence<SerializeType::members::size>{});
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
	//TODO concept CanSerializeC = CanSerialize<Serializer, SerializeType>;
	concept CanSerializeC = true;

	template<typename Serializer, typename Type>
	using SerializeErrorOf = Serialize<Decay<Type>>::template error_type<Serializer>;

	template<typename Serializer, typename Type>
	using SerializeErrorResult = Conditional<IsSameC<SerializeErrorOf<Serializer, Type>, void>, void, Option<SerializeErrorOf<Serializer, Type>>>;

	template<typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer> && IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
	&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWrite(Serializer& serializer, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			Serialize<Decay<Type>>::template write<Serializer, Flags, CustomFlags, SerializeComponentType>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		} else {
			return Serialize<Decay<Type>>::template write<Serializer, Flags, CustomFlags, SerializeComponentType>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		}
	}

	template<typename SerilizeType, typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
	&& IsSerializeTypeC<SerilizeType>&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>
		&& natl::IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWriteMatch(
			Serializer& serializer, const Type& serializeValue, SerializeArgs&&... serializeArgs) noexcept {
		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		} else {
			return serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
		}
	}

	template<typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
			&& CanSerializeC<Serializer, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamed(
			Serializer& serializer,
			const ConstAsciiStringView& name,
			const Type& serializeValue,
			SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using as_type = decayed_serialize_type::as_type;
		serializer.template beginWrite<Flags, CustomFlags, SerializeComponentType, as_type>(name);
		serializer.template writeValue<Flags, CustomFlags, SerializeComponentType>();

		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWrite<Flags, CustomFlags, SerializeComponentType>();
		} else {
			SerializeErrorResult<Serializer, Type> result =
				serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
					serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWrite<Flags, CustomFlags, SerializeComponentType>();
			return result;
		}
	}

	template<typename SerilizeType, typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
	&& IsSerializeTypeC<SerilizeType>&& CanSerializeC<Serializer, Decay<Type>>
		&& natl::IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedMatch(
			Serializer& serializer,
			const ConstAsciiStringView& name,
			const Type& serializeValue,
			SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using as_type = decayed_serialize_type::as_type;
		serializer.template beginWrite<Flags, CustomFlags, SerializeComponentType, as_type>(name);
		serializer.template writeValue<Flags, CustomFlags, SerializeComponentType>();

		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWrite<Flags, CustomFlags, SerializeComponentType>();
		} else {
			SerializeErrorResult<Serializer, Type> result =
				serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
					serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWrite<Flags, CustomFlags, SerializeComponentType>();
			return result;
		}
	}

	//can serialize 
	template<typename Serializer, typename SerializeType>
		requires(IsSerializableC<SerializeType>)
	struct CanSerializeV<Serializer, SerializeType> : CanSerializeV<Serializer, SerializeTypeOf<SerializeType>> {};

	template<typename Serializer>
	concept CanSerializeI8C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i8 value) {
			{
				serializer.template writeI8<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI8 = CanSerializeI8C<Serializer>;
	template<typename Serializer> struct CanSerializeI8V : BoolConstant<CanSerializeI8C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeI16C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i16 value) {
			{
				serializer.template writeI16<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI16 = CanSerializeI16C<Serializer>;
	template<typename Serializer> struct CanSerializeI16V : BoolConstant<CanSerializeI16C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeI32C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i32 value) {
			{
				serializer.template writeI32<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI32 = CanSerializeI32C<Serializer>;
	template<typename Serializer> struct CanSerializeI32V : BoolConstant<CanSerializeI32C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeI64C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const i64 value) {
			{
				serializer.template writeI64<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeI64 = CanSerializeI64C<Serializer>;
	template<typename Serializer> struct CanSerializeI64V : BoolConstant<CanSerializeI64C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI8C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui8 value) {
			{
				serializer.template writeUI8<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI8 = CanSerializeUI8C<Serializer>;
	template<typename Serializer> struct CanSerializeUI8V : BoolConstant<CanSerializeUI8C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI16C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui16 value) {
			{
				serializer.template writeUI16<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI16 = CanSerializeUI16C<Serializer>;
	template<typename Serializer> struct CanSerializeUI16V : BoolConstant<CanSerializeUI16C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI32C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui32 value) {
			{
				serializer.template writeUI32<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI32 = CanSerializeUI32C<Serializer>;
	template<typename Serializer> struct CanSerializeUI32V : BoolConstant<CanSerializeUI32C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeUI64C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ui64 value) {
			{
				serializer.template writeUI64<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeUI64 = CanSerializeUI64C<Serializer>;
	template<typename Serializer> struct CanSerializeUI64V : BoolConstant<CanSerializeUI64C<Serializer>> {};

	template<typename Serializer> concept CanSerializeSizeC = CanSerializeUI64C<Serializer>;
	template<typename Serializer> constexpr inline Bool CanSerializeSize = CanSerializeSizeC<Serializer>;
	template<typename Serializer> struct CanSerializeSizeV : BoolConstant<CanSerializeSizeC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeF32C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const f32 value) {
			{
				serializer.template writeF32<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeF32 = CanSerializeF32C<Serializer>;
	template<typename Serializer> struct CanSerializeF32V : BoolConstant<CanSerializeF32C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeF64C = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const f64 value) {
			{
				serializer.template writeF64<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeF64 = CanSerializeF64C<Serializer>;
	template<typename Serializer> struct CanSerializeF64V : BoolConstant<CanSerializeF64C<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeBoolC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const Bool value) {
			{
				serializer.template writeBool<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeBool = CanSerializeBoolC<Serializer>;
	template<typename Serializer> struct CanSerializeBoolV : BoolConstant<CanSerializeBoolC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeCharC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const Ascii value) {
			{
				serializer.template writeChar<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(value)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeChar = CanSerializeCharC<Serializer>;
	template<typename Serializer> struct CanSerializeCharV : BoolConstant<CanSerializeCharC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeStrC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer,
			const Ascii* rawStr, const Size size,
			const ConstAsciiStringView str) {
				{
					serializer.template writeStr<
						SerializeWriteFlag::v_default,
						DefaultCustomSerializeWriteFlag<Serializer>,
						SerializeGlobalComponent>(rawStr, size)
				} -> IsSameC<void>;
				{
					serializer.template writeStr<
						SerializeWriteFlag::v_default,
						DefaultCustomSerializeWriteFlag<Serializer>,
						SerializeGlobalComponent>(str)
				} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeStr = CanSerializeStrC<Serializer>;
	template<typename Serializer> struct CanSerializeStrV : BoolConstant<CanSerializeStrC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeFileC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer, const ConstAsciiStringView & fileName, const ArrayView<const Byte>&data) {
			{
				serializer.template writeFile<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(fileName, data)
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeFile = CanSerializeFileC<Serializer>;
	template<typename Serializer> struct CanSerializeFileV : BoolConstant<CanSerializeFileC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeBlobC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer,
			const ArrayView<const Byte>&data) {
				{
					serializer.template writeBlob<
						SerializeWriteFlag::v_default,
						DefaultCustomSerializeWriteFlag<Serializer>,
						SerializeGlobalComponent>(data)
				} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeBlob = CanSerializeBlobC<Serializer>;
	template<typename Serializer> struct CanSerializeBlobV : BoolConstant<CanSerializeBlobC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeOptionalC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template beginWriteOptional<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template emdWriteOptional<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template writeNullOptional<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeOptional = CanSerializeOptionalC<Serializer>;
	template<typename Serializer> struct CanSerializeOptionalV : BoolConstant<CanSerializeOptionalC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeEnumC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template writeEnum<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent,
					SerializeUI64>(
						declval<Size>(),
						declval<ConstAsciiStringView>())
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeEnum = CanSerializeEnumC<Serializer>;
	template<typename Serializer> struct CanSerializeEnumV : BoolConstant<CanSerializeEnumC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeFixedArrayC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template beginWriteFixedArray<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template endWriteFixedArray<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template beginWriteFixedArrayElement<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template endWriteFixedArrayElement<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeFixedArray = CanSerializeFixedArrayC<Serializer>;
	template<typename Serializer> struct CanSerializeFixedArrayV : BoolConstant<CanSerializeFixedArrayC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeArrayC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template writeEmptyArray<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template beginWriteArray<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(declval<natl::Size>())
			} -> IsSameC<void>;
			{
				serializer.template endWriteArray<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template beginWriteArrayElement<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template endWriteArrayElement<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeArray = CanSerializeArrayC<Serializer>;
	template<typename Serializer> struct CanSerializeArrayV : BoolConstant<CanSerializeArrayC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeDicC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template writeEmptyDic<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template beginWriteDic<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>(declval<natl::Size>())
			} -> IsSameC<void>;
			{
				serializer.template endWriteDic<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template beginWriteDicElement<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template endWriteDicElement<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template writeDicValue<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeDic = CanSerializeDicC<Serializer>;
	template<typename Serializer> struct CanSerializeDicV : BoolConstant<CanSerializeDicC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeStructC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template beginWriteStruct<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template endWriteStruct<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeStruct = CanSerializeStructC<Serializer>;
	template<typename Serializer> struct CanSerializeStructV : BoolConstant<CanSerializeStructC<Serializer>> {};

	template<typename Serializer>
	concept CanSerializeVariantC = IsSerializerC<Serializer>
		&& requires(Serializer & serializer) {
			{
				serializer.template beginWriteVariant<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent,
					SerializeVariant<natl::ui8, natl::ui8>, 0>(
						declval<ConstAsciiStringView>())
			} -> IsSameC<void>;
			{
				serializer.template endWriteStruct<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
			{
				serializer.template writeEmptyVariant<
					SerializeWriteFlag::v_default,
					DefaultCustomSerializeWriteFlag<Serializer>,
					SerializeGlobalComponent>()
			} -> IsSameC<void>;
	};
	template<typename Serializer> constexpr inline Bool CanSerializeVariant = CanSerializeVariantC<Serializer>;
	template<typename Serializer> struct CanSerializeVariantV : BoolConstant<CanSerializeVariantC<Serializer>> {};

	template<typename Serializer>
	concept IsFullSerializer = IsSerializerC<Serializer>
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
		&& CanSerializeChar<Serializer>
		&& CanSerializeStrC<Serializer>
		&& CanSerializeOptionalC<Serializer>
		&& CanSerializeEnumC<Serializer>
		&& CanSerializeFixedArrayC<Serializer>
		&& CanSerializeArrayC<Serializer>
		&& CanSerializeDicC<Serializer>
		&& CanSerializeStructC<Serializer>
		&& CanSerializeVariantC<Serializer>;

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
	struct CanSerializeV<Serializer, SerializeChar> : BoolConstant<CanSerializeChar<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeStr> : BoolConstant<CanSerializeStrC<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeFile> : BoolConstant<CanSerializeFileC<Serializer>> {};
	template<typename Serializer>
	struct CanSerializeV<Serializer, SerializeBlob> : BoolConstant<CanSerializeBlobC<Serializer>> {};

	template<typename Serializer, typename SerializeType>
	struct CanSerializeV<Serializer, SerializeOptional<SerializeType>>
		: BoolConstant<CanSerializeOptionalC<SerializeType>&& CanSerialize<Serializer, SerializeType>> {
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

	//serialize specialization
	template<> struct Serialize<i8> {
		using as_type = SerializeI8;
		using type = i8;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags, 
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeI8C<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const i8 value) noexcept {
			serializer.template writeI8<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<i16> {
		using as_type = SerializeI16;
		using type = i16;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeI16C<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const i16 value) noexcept {
			serializer.template writeI16<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<i32> {
		using as_type = SerializeI32;
		using type = i32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeI32C<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const i32 value) noexcept {
			serializer.template writeI32<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<i64> {
		using as_type = SerializeI64;
		using type = i64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeI64C<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const i64 value) noexcept {
			serializer.template writeI64<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<ui8> {
		using as_type = SerializeUI8;
		using type = ui8;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeUI8C<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const ui8 value) noexcept {
			serializer.template writeUI8<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<ui16> {
		using as_type = SerializeUI16;
		using type = ui16;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeUI16C<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const ui16 value) noexcept {
			serializer.template writeUI16<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<ui32> {
		using as_type = SerializeUI32;
		using type = ui32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeUI32C<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const ui32 value) noexcept {
			serializer.template writeUI32<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<ui64> {
		using as_type = SerializeUI64;
		using type = ui64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeUI64C<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const ui64 value) noexcept {
			serializer.template writeUI64<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<f32> {
		using as_type = SerializeF32;
		using type = f32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeF32C<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const f32 value) noexcept {
			serializer.template writeF32<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<f64> {
		using as_type = SerializeF64;
		using type = f64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeF64C<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const f64 value) noexcept {
			serializer.template writeF64<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<Bool> {
		using as_type = SerializeBool;
		using type = Bool;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeBoolC<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const Bool value) noexcept {
			serializer.template writeBool<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<Ascii> {
		using as_type = SerializeChar;
		using type = Ascii;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeChar<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const Ascii value) noexcept {
			serializer.template writeChar<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<char*> {
		using as_type = SerializeStr;
		using type = Ascii*;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeStrC<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const char* str) noexcept {
			serializer.template writeStr<Flags, CustomFlags, SerializeComponentType>(str);
		}
	};
	template<> struct Serialize<AsciiStringView> {
		using as_type = SerializeStr;
		using type = AsciiStringView;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeStrC<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const AsciiStringView str) noexcept {
			serializer.template writeStr<Flags, CustomFlags, SerializeComponentType>(str);
		}
	};
	template<> struct Serialize<ConstAsciiStringView> {
		using as_type = SerializeStr;
		using type = ConstAsciiStringView;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(natl::CanSerializeStrC<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const ConstAsciiStringView str) noexcept {
			serializer.template writeStr<Flags, CustomFlags, SerializeComponentType>(str);
		}
	};

	template<class ElementType>
		requires(IsSerializableC<Decay<ElementType>>)
	struct Serialize<ArrayView<ElementType>> {
		using as_type = SerializeArray<Decay<ElementType>>;
		using type = ArrayView<ElementType>;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType,
			typename... ElementSerializeArgs>
			requires(natl::CanSerializeArrayC<Serializer> && IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const type& arrayView, ElementSerializeArgs&&... elementSerializeArgs) noexcept {
			if (arrayView.isEmpty()) {
				serializer.template writeEmptyArray<Flags, CustomFlags, SerializeComponentType>();
			} else {
				serializer.template beginWriteArray<Flags, CustomFlags, SerializeComponentType>(arrayView.size());
				for (Size i = 0; i < arrayView.size(); i++) {
					using array_member = SerializeArrayComponent<type>;
					serializer.template beginWriteArrayElement<Flags, CustomFlags, array_member>();
					serializeWrite<Serializer, Flags, CustomFlags, array_member>(
						serializer, arrayView[i], forward<ElementSerializeArgs>(elementSerializeArgs)...);
					serializer.template endWriteArrayElement<Flags, CustomFlags, array_member>();
				}
				serializer.template endWriteArray<Flags, CustomFlags, SerializeComponentType>();
			}
		}
	};

	template<typename DataType> struct Serialize<Option<DataType>> {
		using as_type = SerializeOptional<DataType>;
		using type = Option<DataType>;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType, 
			typename... DeserializerArgs>
			requires(natl::CanSerializeStrC<Serializer>&& IsSerializeComponentC<SerializeComponentType>)
		constexpr static void write(Serializer& serializer, const type& value, DeserializerArgs&&... deserializerArgs) noexcept {
			if (!value.hasValue()) {
				serializer.template writeEmptyOptional<Flags, CustomFlags, SerializeComponentType>();
			} else {
				using optional_component = SerializeOptionalComponent<type>;
				serializer.template beginWriteOptional<Flags, CustomFlags, SerializeComponentType>();
				serializeWrite<Serializer, Flags, CustomFlags, optional_component>(
					serializer, value.value(), natl::forward<DeserializerArgs>(deserializerArgs)...);
				serializer.template endWriteOptional<Flags, CustomFlags, SerializeComponentType>();
			}
		}
	};

	//deserialize 
	enum class CustomDeserializerReadFlagNone {
		none = 0
	};

	template<typename Deserializer>
	using CustomDeserializeReadFlag = Deserializer::custom_read_flag_type;

	template<typename Deserializer>
	constexpr inline CustomDeserializeReadFlag<Deserializer> DefaultCustomDeserializeReadFlag = Deserializer::defaultCustomReadFlag;

	enum class DeserializeReadFlag {
		v_default = 0,
		fullname,
	};

	enum class ErrorHandlingFlag {
		shorten,
		full,
	};

	struct DeserializeGlobalScope {};
	template<> struct IsSerializeTypeV<DeserializeGlobalScope> : TrueType {};

	enum class DeserializeErrorFlag {
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
		beginReadOptional,
		endReadOptional,
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
		case DeserializeErrorLocation::readIsOptionalNull:
			return "readIsOptionalNull";
		case DeserializeErrorLocation::beginReadOptional:
			return "beginReadOptional";
		case DeserializeErrorLocation::endReadOptional:
			return "endReadOptional";
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

	template<typename Type>
	concept HasRegularDeserializeNameC = IsDeserilizableC<Type> && requires() {
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

	//deserialize read
	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentSerializeType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type> && IsSerializeComponentC<SerializeComponentType> && IsSerializeTypeC<ParentSerializeType>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeRead(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
		DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "natl::deserializeRead";

		Type value;
		auto readValueError = type_deserialize::template read<Deserializer, Flags, CustomFlags, SerializeComponentType>(
			deserializer, parent, value, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return unexpected(readValueError.value().addSource(sourceName, ""));
		}
		return value;
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type> && IsSerializeComponentC<SerializeComponentType> && natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		DeserializerArgs&&... deserializerArgs) {
		return deserializeRead<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}


	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type> && IsSerializeComponentC<SerializeComponentType>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDst(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "natl::deserializeReadToDst";

		auto readValueError = type_deserialize::template read<Deserializer, Flags, CustomFlags, SerializeComponentType>(
			deserializer, parent, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return readValueError.value().addSource(sourceName, "");
		}
		return {};
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type> && IsSerializeComponentC<SerializeComponentType> && natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDstMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadToDstMatch<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>> && IsSerializeComponentC<SerializeComponentType>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamed(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		DeserializerArgs&&... deserializerArgs
	) noexcept {
		constexpr ConstAsciiStringView sourceName = "natl::deserializeReadNamed";

		auto readNamedInfoExpect = deserializer.template beginReadNamed<Flags, CustomFlags, SerializeComponentType, Type, ParentType>(parent, name);
		if (readNamedInfoExpect.hasError()) {
			return unexpected(readNamedInfoExpect.error().addSource(sourceName, name));
		}
		auto readNamedInfo = readNamedInfoExpect.value();

		auto valueExpect = deserializeRead<Deserializer, Flags, CustomFlags, SerializeComponentType, 
			Type, typename decltype(readNamedInfo)::serialize_type, DeserializerArgs...>(
			deserializer, readNamedInfo, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (valueExpect.hasError()) {
			return unexpected(valueExpect.error().addSource(sourceName, name));
		}

		auto endReadNameError = deserializer.template endReadNamed<Flags, CustomFlags, SerializeComponentType, SerializeTypeOf<Type>>(readNamedInfo);
		if (endReadNameError.hasValue()) {
			return unexpected(endReadNameError.value().addSource(sourceName, name));
		}

		return valueExpect.value();
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>> && IsSerializeComponentC<SerializeComponentType> && natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		DeserializerArgs&&... deserializerArgs
	) noexcept {
		return deserializeReadNamed<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>> && IsSerializeComponentC<SerializeComponentType>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDst(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		constexpr ConstAsciiStringView sourceName = "natl::deserializeReadNamedToDst";

		auto readNamedInfoExpect = deserializer.template beginReadNamed<Flags, CustomFlags, SerializeComponentType, Type, ParentType>(parent, name);
		if (readNamedInfoExpect.hasError()) {
			return readNamedInfoExpect.error().addSource(sourceName, name);
		}
		auto readNamedInfo = readNamedInfoExpect.value();

		auto error = deserializeReadToDst<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, typename decltype(readNamedInfo)::serialize_type, DeserializerArgs...>(
			deserializer, readNamedInfo, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
		if (error.hasValue()) {
			return error.value().addSource(sourceName, name);
		}

		auto endReadNameError = deserializer.template endReadNamed<Flags, CustomFlags, SerializeComponentType, SerializeTypeOf<Type>>(readNamedInfo);
		if (endReadNameError.hasValue()) {
			return endReadNameError.value().addSource(sourceName, name);
		}

		return {};
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Decay<Type>> && IsSerializeComponentC<SerializeComponentType> && natl::IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDstMatch(
		Deserializer& deserializer,
		typename Deserializer::template deserialize_info<ParentType>& parent,
		const ConstAsciiStringView& name,
		Type& dst,
		DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, dst, natl::forward<DeserializerArgs>(deserializerArgs)...);
	}

	//deserialize skip 
	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentSerializeType, typename... DeserializerArgs>
		requires(IsDeserilizableC<Type> && IsSerializeComponentC<SerializeComponentType> && IsSerializeTypeC<ParentSerializeType>)
	constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeSkip(
		Deserializer& deserializer, 
		typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
		const ConstAsciiStringView& name) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "natl::deserializeSkip";
		
		auto skipError = deserializer.template skip<Flags, CustomFlags, SerializeComponentType, Type, ParentSerializeType>(parent, name);
		if (skipError.hasValue()) {
			return skipError.value().addSource(sourceName, name);
		}

		return OptionEmpty{};
	}

	//deserialize specialization
	template<> struct Deserialize<i8> {
		using as_type = SerializeI8;
		using type = i8;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<i8>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
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


	template<> struct Deserialize<i16> {
		using as_type = SerializeI16;
		using type = i16;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<i16>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i16& dst) noexcept {
			auto valueExpect = deserializer.template readI16<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i32& dst) noexcept {
			auto valueExpect = deserializer.template readI32<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i64& dst) noexcept {
			auto valueExpect = deserializer.template readI64<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui8& dst) noexcept {
			auto valueExpect = deserializer.template readUI8<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui16& dst) noexcept {
			auto valueExpect = deserializer.template readUI16<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui32& dst) noexcept {
			auto valueExpect = deserializer.template readUI32<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui64& dst) noexcept {
			auto valueExpect = deserializer.template readUI64<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				f32& dst) noexcept {
			auto valueExpect = deserializer.template readF32<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				f64& dst) noexcept {
			auto valueExpect = deserializer.template readF64<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				Bool& dst) noexcept {
			auto valueExpect = deserializer.template readBool<Flags, CustomFlags, SerializeComponentType>(info);
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

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				Ascii& dst) noexcept {
			auto valueExpect = deserializer.template readChar<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<typename Type> struct Deserialize<Option<Type>> {
		using as_type = SerializeChar;
		using type = Option<Type>;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<Ascii>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, \
			typename SerializeComponentType, typename... DeserializerArgs>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				Ascii& dst,
				DeserializerArgs&&... deserializerArgs) noexcept {
			auto isNullExpect = deserializer.template readIsOptionalNull
				<Flags, CustomFlags, SerializeComponentType>(info);
			if (isNullExpect.hasError()) {
				return isNullExpect.error().addSource(sourceName, "");
			}
			if (isNullExpect.value()) {
				return natl::OptionEmpty{};
			}

			auto optionalInfoExpect = deserializer.template beginReadOptional
				<Flags, CustomFlags, SerializeComponentType>(isNullExpect.value(), info);
			if (optionalInfoExpect.hasError()) {
				return optionalInfoExpect.error().addSource(sourceName, "");
			}
			auto valueInfo = optionalInfoExpect.value();

			using optional_component = SerializeOptionalComponent<type>;
			auto valueExpect = deserializeRead<Deserializer, Flags, CustomFlags, optional_component, Type>(
				deserializer, valueInfo, natl::forward<DeserializerArgs>(deserializerArgs)...);
			if (valueExpect.hasError()) {
				return valueExpect.error().addSource(sourceName, "");
			}
			dst = valueExpect.value();

			auto endReadError = deserializer.template endReadOptional
				<Flags, CustomFlags, SerializeComponentType>(info);
			if (endReadError.hasError()) {
				return endReadError.value().addSource(sourceName, "");
			}

			return natl::OptionEmpty{};
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

		template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename DerializeInfoType>
		constexpr void chainDeserializeReadHelper(
			Option<ErrorType>&, const natl::ConstAsciiStringView&, Deserializer&,
			DerializeInfoType&) {}

		template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename DerializeInfoType,
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

			chainDeserializeReadHelper<ErrorType, Deserializer, Flags, CustomFlags, 
				SerializeComponentType, DerializeInfoType, OtherReadFunctors...>(
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


	template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename DerializeInfoType, typename... ReadFunctors>
		requires(IsSerializeComponentC<SerializeComponentType> && (impl::IsDeserializeReadFunctor<ErrorType, Deserializer, DerializeInfoType, ReadFunctors> && ...))
	constexpr Option<ErrorType> chainDeserializeRead(const natl::ConstAsciiStringView& source, Deserializer& deserializer,
		DerializeInfoType& info, ReadFunctors&&... readFunctors) noexcept {
		Option<ErrorType> error = OptionEmpty{};
		impl::chainDeserializeReadHelper<ErrorType, Deserializer, Flags, CustomFlags, SerializeComponentType, DerializeInfoType, ReadFunctors...>(
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

	template<typename ErrorType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, 
		typename SerializeComponentType, typename DerializeInfoType, typename... ReadFunctors>
		requires(IsSerializeComponentC<SerializeComponentType> && (impl::IsDeserializeReadFunctorWithArgs<ErrorType, Deserializer, DerializeInfoType, ReadFunctors> && ...))
	constexpr Option<ErrorType> deserializeReadStruct(const natl::ConstAsciiStringView& source, Deserializer& deserializer,
		DerializeInfoType& info, ReadFunctors&&... readFunctors) {
		auto structInfoExpect = deserializer.template beginReadStruct<Flags, CustomFlags, SerializeComponentType>(info);
		if (structInfoExpect.hasError()) {
			return structInfoExpect.error().addSource(source, "");
		}
		auto structInfo = structInfoExpect.value();

		auto memberError = chainDeserializeRead<ErrorType, Deserializer, Flags, CustomFlags, SerializeComponentType, decltype(structInfo), ReadFunctors...>(
			source, deserializer, structInfo, forward<ReadFunctors>(readFunctors)...);
		if(memberError.hasValue()) {
			return memberError.value();
		}

		auto structEndError = deserializer.template endReadStruct<Flags, CustomFlags, SerializeComponentType>(structInfo);
		if (structEndError.hasValue()) {
			return structEndError.value().addSource(source, "");
		}
		return {};
	}
}