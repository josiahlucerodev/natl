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
		v_default = 0,
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

	struct DummySerializer {
		using allocator_type = DefaultAllocator<Ascii>;
		using code_point_type = Ascii;
		using container_type = void;

		constexpr static inline Size smallBufferSize = 0;
		constexpr static inline SerializeFlags flag = SerializeFlags::v_default;
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
	struct SerializeVariantComponent {
		using variant_type = VariantType;
		using parent_type = variant_type;
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

	template<typename Type> struct IsSerializeVariantComponentV : FalseType {};
	template<typename VariantType, Size Index> struct IsSerializeVariantComponentV<SerializeVariantComponent<VariantType, Index>> : TrueType {};
	template<typename Type> constexpr inline Bool IsSerializeVariantComponent = IsSerializeVariantComponentV<Type>::value;
	template<typename Type> concept IsSerializeVariantComponentC = IsSerializeVariantComponent<Type>;

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
	template<typename VariantType, Size Index> struct IsSerializeComponentV<SerializeVariantComponent<VariantType, Index>> : TrueType {};

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

	template<typename Serializer>
	concept IsSerializerC = requires(Serializer & serializer, const ConstAsciiStringView & name) {
		typename Serializer::custom_write_flag_type;
		{
			serializer.template beginWriteNamed<
				SerializeWriteFlag::v_default,
				DefaultCustomSerializeWriteFlag<Serializer>,
				SerializeGlobalComponent, SerializeI8>(name)
		} -> IsSameC<void>;
		{
			serializer.template endWriteNamed<
				SerializeWriteFlag::v_default,
				DefaultCustomSerializeWriteFlag<Serializer>,
				SerializeGlobalComponent>()
		} -> IsSameC<void>;
		{
			serializer.template writeValue<
				SerializeWriteFlag::v_default,
				DefaultCustomSerializeWriteFlag<Serializer>,
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
			return[]<Size... Indices>(IndexSequence<Indices...>) {
				return TypePack<SerializeStructComponent<StructType, Indices>...>{};
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
	using SerializeErrorResult = Conditional<IsSameC<SerializeErrorOf<Serializer, Type>, void>, void, Option<SerializeErrorOf<Serializer, Type>>>;

	template<typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
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
		&& IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
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
		serializer.template beginWriteNamed<Flags, CustomFlags, SerializeComponentType, as_type>(name);
		serializer.template writeValue<Flags, CustomFlags, SerializeComponentType>();

		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWriteNamed<Flags, CustomFlags, SerializeComponentType>();
		} else {
			SerializeErrorResult<Serializer, Type> result =
				serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
					serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWriteNamed<Flags, CustomFlags, SerializeComponentType>();
			return result;
		}
	}

	template<typename SerilizeType, typename Serializer, SerializeWriteFlag Flags, CustomSerializeWriteFlag<Serializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename... SerializeArgs>
		requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializableC<Decay<Type>>
	&& IsSerializeTypeC<SerilizeType>&& CanSerializeC<Serializer, Decay<Type>>
		&& IsSameC<SerilizeType, typename Serialize<Decay<Type>>::as_type>)
		constexpr SerializeErrorResult<Serializer, Type> serializeWriteNamedMatch(
			Serializer& serializer,
			const ConstAsciiStringView& name,
			const Type& serializeValue,
			SerializeArgs&&... serializeArgs) noexcept {
		using decayed_serialize_type = Serialize<Decay<Type>>;
		using as_type = decayed_serialize_type::as_type;
		serializer.template beginWriteNamed<Flags, CustomFlags, SerializeComponentType, as_type>(name);
		serializer.template writeValue<Flags, CustomFlags, SerializeComponentType>();

		if constexpr (IsSameC<SerializeErrorResult<Serializer, Type>, void>) {
			serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
				serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWriteNamed<Flags, CustomFlags, SerializeComponentType>();
		} else {
			SerializeErrorResult<Serializer, Type> result =
				serializeWrite<Serializer, Flags, CustomFlags, SerializeComponentType, Type, SerializeArgs...>(
					serializer, serializeValue, forward<SerializeArgs>(serializeArgs)...);
			serializer.template endWriteNamed<Flags, CustomFlags, SerializeComponentType>();
			return result;
		}
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
		v_default = 0,
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
		readIsOptionalNull,
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
		case DeserializeErrorLocation::readIsOptionalNull:
			return "readIsOptionalNull";
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
			const ConstAsciiStringView&, const ConstAsciiStringView&,
			const DeserializeErrorLocation&, const DeserializeErrorFlag&) noexcept {
		}
		constexpr Bool hasMessage() noexcept { return false; }
		constexpr DummyDeserializeErrorHandler& addSource(
			const ConstAsciiStringView&, const ConstAsciiStringView&) noexcept {
			return *this;
		}
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
				DeserializeReadFlag::v_default,
				DefaultCustomDeserializeReadFlag<Deserializer>,
				SerializeGlobalComponent, i8>(globalScope, name)
		} -> IsSameC<Expect<DeserializeInfo<Deserializer, SerializeI8>, StandardDeserializeError<Deserializer>>>;
		{
			deserializer.template endReadNamed<
				DeserializeReadFlag::v_default,
				DefaultCustomDeserializeReadFlag<Deserializer>,
				SerializeGlobalComponent>(declref<DeserializeInfo<Deserializer, i8>>())
		} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
		{
			deserializer.template begin<DeserializeReadFlag::v_default,
				DefaultCustomDeserializeReadFlag<Deserializer>>()
		} -> IsSameC<Expect<DeserializeInfoGlobal<Deserializer>, StandardDeserializeError<Deserializer>>>;
		{
			deserializer.template end<DeserializeReadFlag::v_default,
				DefaultCustomDeserializeReadFlag<Deserializer>>(
					declref<DeserializeInfoGlobal<Deserializer>>()
					)
		} -> IsSameC<Option<StandardDeserializeError<Deserializer>>>;
	};
	template<typename Type> constexpr inline Bool IsDeserializer = IsDeserializerC<Type>;
	template<typename Type> struct IsDeserializerV : BoolConstant<IsDeserializerC<Type>> {};

	//deserialize read
	template<typename Deserializer, typename Type>
	struct CanDeserializeV : FalseType {};
	template<typename Deserializer, typename Type>
	constexpr inline Bool CanDeserialize = CanDeserializeV<Deserialize, Type>::value;
	template<typename Serializer, typename SerializeType>

	//TODO 
	concept CanDeserializeC = true;

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentSerializeType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializeTypeC<ParentSerializeType>
			&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeRead(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentSerializeType>& parent,
			DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "deserializeRead";

		Type value;
		auto readValueError = type_deserialize::template read<Deserializer, Flags, CustomFlags, SerializeComponentType>(
			deserializer, parent, value, forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return unexpected(readValueError.value().addSource(sourceName, ""));
		}
		return value;
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>
	&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
		&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			DeserializerArgs&&... deserializerArgs) {
		return deserializeRead<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, forward<DeserializerArgs>(deserializerArgs)...);
	}


	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>
	&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDst(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		using type_deserialize = Deserialize<Decay<Type>>;
		constexpr ConstAsciiStringView sourceName = "deserializeReadToDst";

		auto readValueError = type_deserialize::template read<Deserializer, Flags, CustomFlags, SerializeComponentType>(
			deserializer, parent, dst, forward<DeserializerArgs>(deserializerArgs)...);
		if (readValueError.hasValue()) {
			return readValueError.value().addSource(sourceName, "");
		}
		return {};
	}

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>
	&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
		&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadToDstMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadToDstMatch<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
	&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamed(
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

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
	&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
		&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Expect<Type, DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			DeserializerArgs&&... deserializerArgs
		) noexcept {
		return deserializeReadNamed<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, forward<DeserializerArgs>(deserializerArgs)...);
	}

	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
	&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDst(
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

	template<typename SerilizeType, typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentType, typename... DeserializerArgs>
		requires(IsDeserializableC<Decay<Type>>&& IsSerializeComponentC<SerializeComponentType>
	&& CanDeserializeC<Deserializer, typename Deserialize<Decay<Type>>::as_type>
		&& IsSameC<SerilizeType, typename Deserialize<Decay<Type>>::as_type>)
		constexpr Option<DeserializeErrorOf<Deserializer, Type>> deserializeReadNamedToDstMatch(
			Deserializer& deserializer,
			typename Deserializer::template deserialize_info<ParentType>& parent,
			const ConstAsciiStringView& name,
			Type& dst,
			DeserializerArgs&&... deserializerArgs) noexcept {
		return deserializeReadNamedToDst<Deserializer, Flags, CustomFlags, SerializeComponentType, Type, ParentType, DeserializerArgs...>(
			deserializer, parent, name, dst, forward<DeserializerArgs>(deserializerArgs)...);
	}

	//deserialize skip 
	template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
		typename SerializeComponentType, typename Type, typename ParentSerializeType, typename... DeserializerArgs>
		requires(IsDeserializableC<Type>&& IsSerializeComponentC<SerializeComponentType>&& IsSerializeTypeC<ParentSerializeType>)
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