#pragma once 

//own
#include "base.h"
#include "../container/smallDynArray.h"
#include "../container/variant.h"
#include "../util/TypeInfo.h"

//interface 
namespace natl {
	template<
		template<Size, SerializeFlags, typename> typename Serializer,
		Size smallBufferByteSize = 1024,
		SerializeFlags serializeFlags = SerializeFlags::pretty,
		typename Alloc = DefaultAllocatorByte>
		requires(IsAllocatorC<Alloc> && IsSerializerC<Serializer<smallBufferByteSize, serializeFlags, Alloc>>)
	struct SerializerEnhancedError {
	private:
		using serializer = Serializer<smallBufferByteSize, serializeFlags, Alloc>;
		using allocator_type = serializer::allocator_type;
		using code_point_type = serializer::code_point_type;
		using container_type = serializer::container_type;
		using custom_write_flag_type = serializer::custom_write_flag_type;
		constexpr static custom_write_flag_type defaultCustomWriteFlag = serializer::defaultCustomWriteFlag;
		constexpr static inline Size smallBufferSize = serializer::smallBufferByteSize;
		constexpr static inline SerializeFlags flag = serializer::serializeFlags;
	public:
		constexpr SerializerEnhancedError() noexcept {}
		constexpr ~SerializerEnhancedError() noexcept {}

	private:
		struct State {
			using Info = Variant<
				NamedElement<"begin", natl::Empty>
			>;

			TypeInfo type;
			Info info;
		};

		SmallDynArray<State, 16> stateStack;
	public:
		constexpr void begin() noexcept {}
		constexpr void end() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, typename SerializeType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteNamed(const ConstAsciiStringView name) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteNamed() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeValue() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteOptional() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteOptional() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeEmptyOptional() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeI8(const i8 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeI16(const i16 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeI32(const i32 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeI64(const i64 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeUI8(const ui8 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeUI16(const ui16 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeUI32(const ui32 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeUI64(const ui64 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeF32(const f32 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeF64(const f64 value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeBool(const Bool value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeChar(const Char value) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeStr(const Ascii* str, const Size size) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeStr(const ConstAsciiStringView str) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeFile(const ConstAsciiStringView fileName, const ConstArrayView<Byte> data) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeBlob(const ConstArrayView<Byte> data) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename BaseSerializeType>
			requires(IsSerializeComponentC<SerializeComponentType>&& IsEnumBaseSerializeTypeC<BaseSerializeType>)
		constexpr void writeEnum(
			const BasicSerializeTypeToType<BaseSerializeType> intValue,
			const ConstAsciiStringView& strValue) noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteFixedArray() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteFixedArray() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteFixedArrayElement() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteFixedArrayElement() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeEmptyArray() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteArray(Size) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteArray() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteArrayElement() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteArrayElement() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeEmptyDic() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteDic(Size) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteDic() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteDicElement() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteDicElement() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeDicKey() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeDicValue() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void beginWriteStruct() noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteStruct() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void writeEmptyVariant() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType, Size Index>
			requires(IsSerializeComponentC<SerializeComponentType>&& IsSerializeVariantC<VariantSerializeType>)
		constexpr void beginWriteVariant(const ConstAsciiStringView& strValue) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteVariant() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IndexIDSizeType>
			requires(IsSerializeComponentC<SerializeComponentType>&& IsBuiltInIntegerC<IndexIDSizeType>)
		constexpr void beginWriteJumpTable(const ConstAsciiStringView name) noexcept {}
		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(IsSerializeComponentC<SerializeComponentType>)
		constexpr void endWriteTable() noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(IsSerializeComponentC<SerializeComponentType>&& IsBuiltInUnsignedIntegerC<IdNumberType>)
		constexpr SerializeJumpSaveId writeJumpTableElement(const SerializeJumpId<IdNumberType>& element) noexcept {}

		template<SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(IsSerializeComponentC<SerializeComponentType>&& IsBuiltInUnsignedIntegerC<IdNumberType>)
		constexpr SerializeJumpLocation writeJumpLocation(const SerializeJumpId<IdNumberType>& element,
			const SerializeJumpSaveId& saveId) noexcept {}
	};
}