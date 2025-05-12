
#pragma once 

//own
#include "base.h"
#include "jump.h"
#include "../container/smallDynArray.h"
#include "../container/variant.h"
#include "../util/TypeInfo.h"

//interface 
namespace natl {
	template<
		template<Size, SerializeFlags, typename, typename> typename Serializer,		
		natl::Size smallBufferByteSize = 1024,
		natl::SerializeFlags serializeFlags = natl::SerializeFlags::pretty,
		typename SerializeErrorHandler = natl::DummySerializeErrorHandler,
		typename Alloc = natl::DefaultAllocator>
		requires(IsAllocatorC<Alloc> && IsSerializerC<Serializer<smallBufferByteSize, serializeFlags, SerializeErrorHandler, Alloc>> 
			&& natl::IsSerializeErrorHandlerC<SerializeErrorHandler>)
	struct SerializerEnhancedError {
	public:
		using serializer = Serializer<smallBufferByteSize, serializeFlags, SerializeErrorHandler, Alloc>;
		using allocator_type = serializer::allocator_type;
		using code_point_type = serializer::code_point_type;
		using container_type = serializer::container_type;
		using custom_write_flag_type = serializer::custom_write_flag_type;
		using serialize_error_handler = serializer::serialize_error_handler;
		constexpr static inline custom_write_flag_type defaultCustomWriteFlag = serializer::defaultCustomWriteFlag;
		constexpr static inline Size smallBufferSize = serializer::smallBufferSize;
		constexpr static inline SerializeFlags flags = serializer::flags;

	private:
		struct WriteState {
			enum struct Type {
				global,
				i8,
				i16,
				i32,
				i64,
				ui8,
				ui16,
				ui32,
				ui64,
				f32,
				f64,
				cChar,
				str,
				file,
				blob,
				op,
				cEnum,
				farray,
				array,
				dic,
				cStruct,
				variant,
				jumpTable,
			};

			struct OpInfo {
				TypeInfo elementInfo;
			};

			struct EnumInfo {
				TypeInfo baseInfo;
			};

			struct FarrayInfo {
				Size size;
				Size index;
				TypeInfo elementInfo;
			};

			struct ArrayInfo {
				Size size;
				Size index;
				TypeInfo elementInfo;
			};

			struct DicInfo {
				Size size;
				Size index;
				Bool beganWritingKey;
				TypeInfo keyInfo;
				TypeInfo valueInfo;
			};

			struct StructInfo {
				Size memberIndex;
				SmallDynArray<TypeInfo, 8> members;
			};

			struct VariantInfo {
				SmallDynArray<TypeInfo, 8> types;
			};

			struct JumpTableInfo {
				TypeInfo numberInfo;
			};

			using MemberInfo = Variant<
				NamedElement<"op", OpInfo>,
				NamedElement<"enum", EnumInfo>,
				NamedElement<"farray", FarrayInfo>,
				NamedElement<"array", ArrayInfo>,
				NamedElement<"dic", DicInfo>,
				NamedElement<"struct", StructInfo>,
				NamedElement<"variant", VariantInfo>,
				NamedElement<"jumpTable", JumpTableInfo>
			>;

			Type fromType;
			Type type;
			TypeInfo typeInfo;
			Bool named;
			MemberInfo memberInfo;
			Bool writeValueCalled;
			Bool beganWritingValue;
			Bool endedWritingValue;
		};

		Bool activeSerialization;
		SmallDynArray<WriteState, 16> stateStack;
		serializer ser;


	public:
		constexpr SerializerEnhancedError() noexcept : activeSerialization(false), stateStack(), ser() {}
		constexpr ~SerializerEnhancedError() noexcept {}

	private:
		constexpr static natl::ConstAsciiStringView writeStateTypeToString(const typename WriteState::Type type) noexcept {
			switch (type) {
			case WriteState::Type::i8: return "i8";
			case WriteState::Type::i16: return "i16";
			case WriteState::Type::i32: return "i32";
			case WriteState::Type::i64: return "i64";
			case WriteState::Type::ui8: return "ui8";
			case WriteState::Type::ui16: return "ui16";
			case WriteState::Type::ui32: return "ui32";
			case WriteState::Type::ui64: return "ui64";
			case WriteState::Type::f32: return "f32";
			case WriteState::Type::f64: return "f64";
			case WriteState::Type::cChar: return "cChar";
			case WriteState::Type::str: return "str";
			case WriteState::Type::file: return "file";
			case WriteState::Type::blob: return "blob";
			case WriteState::Type::op: return "op";
			case WriteState::Type::cEnum: return "cEnum";
			case WriteState::Type::farray: return "farray";
			case WriteState::Type::array: return "array";
			case WriteState::Type::dic: return "dic";
			case WriteState::Type::cStruct: return "cStruct";
			case WriteState::Type::variant: return "variant";
			case WriteState::Type::jumpTable: return "jumpTable";
			default:
				unreachable();
			}
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkActive(const natl::SerializeErrorLocation& errorLocation) noexcept {
			if (activeSerialization != true) {
				return SerializeErrorHandler("serialization has not began", errorLocation, natl::SerializeErrorFlag::inactive);
			}
			return natl::OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkActiveWrite(
			const natl::ConstAsciiStringView& from, const natl::SerializeErrorLocation& errorLocation) noexcept {
			auto activeError = checkActive(errorLocation);
			if (activeError.hasValue()) { return activeError.value(); }

			if (stateStack.isEmpty()) {
				return SerializeErrorHandler(natl::sformat("cannot write from global context when calling ", from),
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			return natl::OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkActiveAndWrite(
			const natl::ConstAsciiStringView& from,
			typename WriteState::Type expectedType,
			const natl::SerializeErrorLocation& errorLocation) noexcept {
			auto activeError = checkActive(errorLocation);
			if (activeError.hasValue()) { return activeError.value(); }

			if (stateStack.isEmpty()) {
				return SerializeErrorHandler(natl::sformat("cannot write from global context when calling ", from), 
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			WriteState& write = stateStack.back();
			if (write.type != expectedType) {
				return SerializeErrorHandler(natl::sformat("expected to write value of type of ", writeStateTypeToString(expectedType),
					" but called ", from, " writing type of ", writeStateTypeToString(write.type)),
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			return natl::OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkBeginWrite(
			const natl::ConstAsciiStringView& from,
			const WriteState::Type& expectedType,
			const natl::SerializeErrorLocation& errorLocation) noexcept {
			
			auto activeNamedError = checkActiveAndWrite(from, expectedType, errorLocation);
			if (activeNamedError.hasValue()) { return activeNamedError.value(); }

			WriteState& write = stateStack.back();
			if (write.beganWritingValue != false) {
				return SerializeErrorHandler(natl::sformat(from, " has already been called in context and cannot be called until current element write is complete"),
					errorLocation, natl::SerializeErrorFlag::duplicateCall);
			}

			if (write.endedWritingValue != false) {
				return SerializeErrorHandler(natl::sformat(from, " has already been called and the current write context is complete but a new element write context has to be called"),
					errorLocation, natl::SerializeErrorFlag::duplicateCall);
			}

			write.beganWritingValue = true;

			return OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkEndWriteElement(
			const natl::ConstAsciiStringView& from,
			const WriteState::Type& expectedType,
			const natl::SerializeErrorLocation& errorLocation) noexcept {
			auto activeNamedError = checkActiveWrite(from, errorLocation);
			if (activeNamedError.hasValue()) { return activeNamedError.value(); }

			WriteState& write = stateStack.back();
			if (write.beganWritingValue != true) {
				return SerializeErrorHandler(natl::sformat(from, " cannot be called until child element has been written"),
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			if (write.endedWritingValue != true) {
				return SerializeErrorHandler(natl::sformat(from, " cannot be called until child element has been completely written"),
					errorLocation, natl::SerializeErrorFlag::duplicateCall);
			}

			return OptionEmpty{};
		}
		
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkEndWrite(
			const natl::ConstAsciiStringView& from,
			const WriteState::Type& expectedType,
			const natl::SerializeErrorLocation& errorLocation) noexcept {

			auto activeNamedError = checkActiveAndWrite(from, expectedType, errorLocation);
			if (activeNamedError.hasValue()) { return activeNamedError.value(); }

			WriteState& write = stateStack.back();
			if (write.beganWritingValue != true) {
				return SerializeErrorHandler(natl::sformat(from, " cannot be called unless matching begin function has started the writing context of type"),
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			if (write.endedWritingValue != false) {
				return SerializeErrorHandler(natl::sformat(from, " has already been called and cannot be called again until the end of writing context of matching type"),
					errorLocation, natl::SerializeErrorFlag::duplicateCall);
			}
			write.endedWritingValue = true;

			return OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkEndWriteContainer(
			const natl::ConstAsciiStringView& from, const Size size, const Size index,
			const natl::SerializeErrorLocation& errorLocation) noexcept {

			if (index != size) {
				return SerializeErrorHandler(natl::sformat(from, " was called early and the container has not been filled due to expecting a ",
					" a size of ", size, " but was at size of ", index),
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			return OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkBeginWriteContainerElement(
			const natl::ConstAsciiStringView& from, const Size size, const Size index,
			const natl::SerializeErrorLocation& errorLocation) noexcept {

			if (not (index < size)) {
				return SerializeErrorHandler(natl::sformat(from, " cannot be called as to many element have been written to container due to ",
					" expected size of ", size, " but was ended at size + 1"),
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			return OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkWritingValue(
			const natl::ConstAsciiStringView& from,
			const natl::SerializeErrorLocation& errorLocation) noexcept {

			WriteState& write = stateStack.back();
			if (write.beganWritingValue != true) {
				return SerializeErrorHandler(natl::sformat(from, " cannot be called unless matching begin function has started the writing context of type"),
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			if (write.endedWritingValue != false) {
				return SerializeErrorHandler(natl::sformat(from, " has already been called and cannot be called again until the end of writing context of matching type"),
					errorLocation, natl::SerializeErrorFlag::duplicateCall);
			}

			return OptionEmpty{};
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkBasicType(
			const natl::ConstAsciiStringView& from,
			typename WriteState::Type expectedType,
			const natl::SerializeErrorLocation& errorLocation) noexcept {

			WriteState& write = stateStack.back();
			write.beganWritingValue = true;

			auto activeNamedError = checkActiveAndWrite(from, expectedType, errorLocation);
			if (activeNamedError.hasValue()) { return activeNamedError.value(); }

			auto writingValueError = checkWritingValue(from, errorLocation);
			if (writingValueError.hasValue()) { return writingValueError.value(); }

			write.endedWritingValue = true;
			return OptionEmpty{};
		}

		template<typename IsExpectedTypeInfoFunctor>
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkBeginWriteElement(
			const natl::ConstAsciiStringView& from,
			typename WriteState::Type expectedType,
			const natl::SerializeErrorLocation& errorLocation,
			IsExpectedTypeInfoFunctor isExpectedTypeInfoFunctor) {
			
			auto activeNamedError = checkActiveAndWrite(from, expectedType, errorLocation);
			if (activeNamedError.hasValue()) { return activeNamedError.value(); }

			auto writingValueError = checkWritingValue(from, errorLocation);
			if (writingValueError.hasValue()) { return writingValueError.value(); }

			WriteState& write = stateStack.back();
			if (not isExpectedTypeInfoFunctor(write)) {
				return SerializeErrorHandler(sformat("non-match element type during call to ", from),
					errorLocation, natl::SerializeErrorFlag::wrongType);
			}

			return OptionEmpty{};
		}

		template<typename IsExpectedTypeInfoFunctor>
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> checkWriteEmpty(
			const natl::ConstAsciiStringView& from,
			typename WriteState::Type expectedType,
			const natl::SerializeErrorLocation& errorLocation,
			IsExpectedTypeInfoFunctor isExpectedTypeInfoFunctor) {

			auto activeNamedError = checkActiveAndWrite(from, expectedType, errorLocation);

			WriteState& write = stateStack.back();
			write.beganWritingValue = true;

			auto beginError = checkBeginWriteElement(from, expectedType, errorLocation, isExpectedTypeInfoFunctor);
			if (beginError.hasValue()) { return beginError.value(); }

			write.endedWritingValue = true;

			return natl::OptionEmpty{};
		}

		constexpr WriteState newWriteState(Bool named, typename WriteState::Type fromType) {
			WriteState write{};
			write.fromType = fromType;
			write.writeValueCalled = false;
			write.beganWritingValue = false;
			write.endedWritingValue = false;
			write.named = named;

			return write;
		}

		template<typename SerializeType>
		constexpr void pushNewWrite(Bool named, typename WriteState::Type fromType) noexcept {

			WriteState write{};
			write.fromType = fromType;
			write.typeInfo = getTypeInfo<SerializeType>();
			write.writeValueCalled = false;
			write.beganWritingValue = false;
			write.endedWritingValue = false;
			write.named = named;

			if constexpr (IsSerializeI8C<SerializeType>) {
				write.type = WriteState::Type::i8;
			} else if constexpr (IsSerializeI16C<SerializeType>) {
				write.type = WriteState::Type::i16;
			} else if constexpr (IsSerializeI32C<SerializeType>) {
				write.type = WriteState::Type::i32;
			} else if constexpr (IsSerializeI64C<SerializeType>) {
				write.type = WriteState::Type::i64;
			} else if constexpr (IsSerializeUI8C<SerializeType>) {
				write.type = WriteState::Type::ui8;
			} else if constexpr (IsSerializeUI16C<SerializeType>) {
				write.type = WriteState::Type::ui16;
			} else if constexpr (IsSerializeUI32C<SerializeType>) {
				write.type = WriteState::Type::ui32;
			} else if constexpr (IsSerializeUI64C<SerializeType>) {
				write.type = WriteState::Type::ui64;
			} else if constexpr (IsSerializeCharC<SerializeType>) {
				write.type = WriteState::Type::cChar;
			} else if constexpr (IsSerializeStrC<SerializeType>) {
				write.type = WriteState::Type::str;
			} else if constexpr (IsSerializeFileC<SerializeType>) {
				write.type = WriteState::Type::file;
			} else if constexpr (IsSerializeBlobC<SerializeType>) {
				write.type = WriteState::Type::blob;
			} else if constexpr (IsSerializeOptionalC<SerializeType>) {
				write.type = WriteState::Type::op;

				typename WriteState::OpInfo opInfo;
				opInfo.elementInfo = getTypeInfo<SerializeType>();

				write.memberInfo.assign<"op">(opInfo);
			} else if constexpr (IsSerializeEnumC<SerializeType>) {
				write.type = WriteState::Type::cEnum;

				typename WriteState::EnumInfo enumInfo;
				enumInfo.baseInfo = getTypeInfo<typename SerializeType::base_serialize_type>();

				write.memberInfo.assign<"enum">(enumInfo);
			} else if constexpr (IsSerializeFixedArrayC<SerializeType>) {
				write.type = WriteState::Type::farray;

				typename WriteState::FarrayInfo farrayInfo;
				farrayInfo.size = SerializeType::size;
				farrayInfo.index = 0;
				farrayInfo.elementInfo = getTypeInfo<typename SerializeType::element_serialize_type>();

				write.memberInfo.assign<"farray">(farrayInfo);
			} else if constexpr (IsSerializeArrayC<SerializeType>) {
				write.type = WriteState::Type::array;

				typename WriteState::ArrayInfo arrayInfo;
				arrayInfo.size = 0;
				arrayInfo.index = 0;
				arrayInfo.elementInfo = getTypeInfo<typename SerializeType::element_serialize_type>();

				write.memberInfo.assign<"array">(arrayInfo);
			} else if constexpr (IsSerializeDicC<SerializeType>) {
				write.type = WriteState::Type::dic;

				typename WriteState::DicInfo dicInfo;
				dicInfo.size = 0;
				dicInfo.index = 0;
				dicInfo.beganWritingKey = false;
				dicInfo.keyInfo = getTypeInfo<typename SerializeType::key_serialize_type>();
				dicInfo.valueInfo = getTypeInfo<typename SerializeType::value_serialize_type>();

				write.memberInfo.assign<"dic">(dicInfo);
			} else if constexpr (IsSerializeStructC<SerializeType>) {
				write.type = WriteState::Type::cStruct;

				typename WriteState::StructInfo structInfo;
				structInfo.memberIndex = 0;
				structInfo.members.resize(SerializeType::members_serialize_types::size);

				[&] <Size... Indices> (IndexSequence<Indices...>) {
					auto setMember = [&]<Size Index>() {
						structInfo.members[Index] = getTypeInfo<SerializeType::members_serialize_types::template at<Index>>();
					};
					(setMember.operator() < Indices > (), ...);
				}(MakeIndexSequence<SerializeType::members_serialize_types::size>{});

				write.memberInfo.assign<"struct">(structInfo);
			} else if constexpr (IsSerializeVariantC<SerializeType>) {
				write.type = WriteState::Type::variant;

				typename WriteState::VariantInfo variantInfo;
				variantInfo.types.resize(SerializeType::serialize_types::size);

				[&] <Size... Indices> (IndexSequence<Indices...>) {
					auto setType = [&]<Size Index>() {
						variantInfo.types[Index] = getTypeInfo<SerializeType::serialize_types::template at<Index>>();
					};
					(setType.operator() < Indices > (), ...);
				}(MakeIndexSequence<SerializeType::serialize_types::size>{});

				write.memberInfo.assign<"variant">(variantInfo);
			} else {
				static_assert(AlwaysFalse<SerializeType>, "natl: natl::SerializerEnhancedError::beginWriteNamed unknown SerializeType");
			}

			stateStack.pushBack(write);
		}

	public:
		[[nodiscard]] constexpr natl::ConstAsciiStringView output() noexcept {
			return ser.output();
		}

		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> begin() noexcept {
			constexpr natl::SerializeErrorLocation errorLocation = natl::SerializeErrorLocation::begin;
			if (not stateStack.isEmpty()) {
				return SerializeErrorHandler("serialization has already began", errorLocation, natl::SerializeErrorFlag::duplicateCall);
			}

			activeSerialization = true;

			return natl::OptionEmpty{};
		}
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> end() noexcept {
			constexpr natl::SerializeErrorLocation errorLocation = natl::SerializeErrorLocation::end;
			auto activeError = checkActive(errorLocation);
			if (activeError.hasValue()) { return activeError.value(); }

			if (not stateStack.isEmpty()) {
				return SerializeErrorHandler("serialization ended early", errorLocation, natl::SerializeErrorFlag::earlyEnd);
			}

			activeSerialization = false;

			return natl::OptionEmpty{};
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, typename SerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteNamed(const natl::ConstAsciiStringView name) noexcept {
			constexpr natl::SerializeErrorLocation errorLocation = natl::SerializeErrorLocation::beginWriteNamed;
			auto activeError = checkActive(errorLocation);
			if (activeError.hasValue()) { return activeError.value(); }

			TypeInfo typeInfo = getTypeInfo<SerializeType>();
			typename WriteState::Type fromType = WriteState::Type::global;

			if (not stateStack.isEmpty()) {
				WriteState& write = stateStack.back();

				if (write.type != WriteState::Type::cStruct) {
					return SerializeErrorHandler("can only write named element in global scope or in scope of struct",
						errorLocation, natl::SerializeErrorFlag::invalidState);
				}

				typename WriteState::StructInfo& structInfo = write.memberInfo.get<"struct">();
				if (not isInRange(structInfo.members, structInfo.memberIndex)) {
					return SerializeErrorHandler(natl::sformat("writing extra member to struct which only has ", structInfo.members.size(), " members"),
						errorLocation, natl::SerializeErrorFlag::invalidState);
				}

				if (structInfo.members[structInfo.memberIndex] != typeInfo) {
					return SerializeErrorHandler("writing member of struct with a non-matching type",
						errorLocation, natl::SerializeErrorFlag::wrongType);
				}

				structInfo.memberIndex += 1;
				fromType = write.type;
			}

			pushNewWrite<SerializeType>(true, fromType);
			
			return ser.beginWriteNamed<Flags, CustomFlags, SerializeComponentType, SerializeType>(name);
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteNamed() noexcept {
			constexpr natl::SerializeErrorLocation errorLocation = natl::SerializeErrorLocation::endWriteNamed;

			auto activeError = checkActive(errorLocation);
			if (activeError.hasValue()) { return activeError.value(); }

			WriteState& write = stateStack.back();
			if (not write.named) {
				return SerializeErrorHandler("can only call endWriteNamed after beginWriteNamed/writeValue and when in named write context",
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}
			
			if (write.endedWritingValue == false) {
				return SerializeErrorHandler("a value has not been written and cannot call endWriteNamed until a value has been written",
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			if (write.writeValueCalled == false) {
				return SerializeErrorHandler("cannot call endWriteNamed until writeValue is called and write element is written",
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			stateStack.popBack();

			return ser.endWriteNamed<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeValue() noexcept {
			constexpr natl::SerializeErrorLocation errorLocation = natl::SerializeErrorLocation::writeValue;
			
			auto activeError = checkActive(errorLocation);
			if (activeError.hasValue()) { return activeError.value(); }

			WriteState& write = stateStack.back();
			if (not write.named) {
				return SerializeErrorHandler("can only call writeValue after beginWriteNamed and and when in named write context",
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			if (write.beganWritingValue != false) {
				return SerializeErrorHandler("cannot write value of write element until after writeValue is called",
					errorLocation, natl::SerializeErrorFlag::invalidState);
			}

			if (write.writeValueCalled != false) {
				return SerializeErrorHandler("writeValue called multiple times",
					errorLocation, natl::SerializeErrorFlag::duplicateCall);
			}

			write.writeValueCalled = true;

			return ser.writeValue<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI8(const natl::i8 value) noexcept {
			auto error = checkBasicType("writeI8", WriteState::Type::i8, SerializeErrorLocation::writeI8);
			if (error.hasValue()) { return error.value(); }
			return ser.writeI8<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI16(const natl::i16 value) noexcept {
			auto error = checkBasicType("writeI16", WriteState::Type::i16, SerializeErrorLocation::writeI16);
			if (error.hasValue()) { return error.value(); }
			return ser.writeI16<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI32(const natl::i32 value) noexcept {
			auto error = checkBasicType("writeI32", WriteState::Type::i32, SerializeErrorLocation::writeI32);
			if (error.hasValue()) { return error.value(); }
			return ser.writeI32<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeI64(const natl::i64 value) noexcept {
			auto error = checkBasicType("writeI64", WriteState::Type::i64, SerializeErrorLocation::writeI64);
			if (error.hasValue()) { return error.value(); }
			return ser.writeI64<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI8(const natl::ui8 value) noexcept {
			auto error = checkBasicType("writeUI8", WriteState::Type::ui8, SerializeErrorLocation::writeUI8);
			if (error.hasValue()) { return error.value(); }
			return ser.writeUI8<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI16(const natl::ui16 value) noexcept {
			auto error = checkBasicType("writeUI16", WriteState::Type::ui16, SerializeErrorLocation::writeUI16);
			if (error.hasValue()) { return error.value(); }
			return ser.writeUI16<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI32(const natl::ui32 value) noexcept {
			auto error = checkBasicType("writeUI32", WriteState::Type::ui32, SerializeErrorLocation::writeUI32);
			if (error.hasValue()) { return error.value(); }
			return ser.writeUI32<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeUI64(const natl::ui64 value) noexcept {
			auto error = checkBasicType("writeUI64", WriteState::Type::ui64, SerializeErrorLocation::writeUI64);
			if (error.hasValue()) { return error.value(); }
			return ser.writeUI64<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF32(const natl::f32 value) noexcept {
			auto error = checkBasicType("writeF32", WriteState::Type::f32, SerializeErrorLocation::writeF32);
			if (error.hasValue()) { return error.value(); }
			return ser.writeF32<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeF64(const natl::f64 value) noexcept {
			auto error = checkBasicType("writeF64", WriteState::Type::f64, SerializeErrorLocation::writeF64);
			if (error.hasValue()) { return error.value(); }
			return ser.writeF64<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBool(const natl::Bool value) noexcept {
			auto error = checkBasicType("writeBool", WriteState::Type::cBool, SerializeErrorLocation::writeBool);
			if (error.hasValue()) { return error.value(); }
			return ser.writeBool<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeChar(const natl::Char value) noexcept {
			auto error = checkBasicType("writeChar", WriteState::Type::cChar, SerializeErrorLocation::writeChar);
			if (error.hasValue()) { return error.value(); }
			return ser.writeChar<Flags, CustomFlags, SerializeComponentType>(value);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::Ascii* str, const natl::Size size) noexcept {
			auto error = checkBasicType("writeStr", WriteState::Type::str, SerializeErrorLocation::writeStr);
			if (error.hasValue()) { return error.value(); }
			return ser.writeStr<Flags, CustomFlags, SerializeComponentType>(str, size);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeStr(const natl::ConstAsciiStringView str) noexcept {
			auto error = checkBasicType("writeStr", WriteState::Type::str, SerializeErrorLocation::writeStr);
			if (error.hasValue()) { return error.value(); }
			return ser.writeStr<Flags, CustomFlags, SerializeComponentType>(str);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeFile(const natl::ConstAsciiStringView fileName, const natl::ConstArrayView<natl::Byte> data) noexcept {
			auto error = checkBasicType("writeFile", WriteState::Type::file, SerializeErrorLocation::writeFile);
			if (error.hasValue()) { return error.value(); }
			return ser.writeFile<Flags, CustomFlags, SerializeComponentType>(fileName, data);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeBlob(const natl::ConstArrayView<natl::Byte> data) noexcept {
			auto error = checkBasicType("writeBlob", WriteState::Type::blob, SerializeErrorLocation::writeBlob);
			if (error.hasValue()) { return error.value(); }
			return ser.writeBlob<Flags, CustomFlags, SerializeComponentType>(data);
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteOptional() noexcept {
			auto beginError = checkBeginWrite("beginWriteOptional", WriteState::Type::op, natl::SerializeErrorLocation::beginWriteOptional);
			if (beginError.hasValue()) { return beginError.value(); }

			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using element_type = serialize_type::type;
			pushNewWrite<SerializeTypeOf<element_type>>(false, WriteState::Type::op);

			return ser.beginWriteOptional<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteOptional() noexcept {
			auto endElementError = checkEndWriteElement("endWriteOptional", WriteState::Type::op, natl::SerializeErrorLocation::endWriteOptional);
			if (endElementError.hasValue()) { return endElementError.value(); }
			stateStack.popBack();

			auto endError = checkEndWrite("endWriteOptional", WriteState::Type::op, natl::SerializeErrorLocation::endWriteOptional);
			if (endError.hasValue()) { return endError.value(); }
			return ser.endWriteOptional<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyOptional() noexcept {
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using element_type = serialize_type::type;

			auto emptyError = checkWriteEmpty("writeEmptyOptional", WriteState::Type::op
				, natl::SerializeErrorLocation::writeEmptyOptional
				, [](const WriteState& write) {
					const typename WriteState::OpInfo& opInfo = write.memberInfo.get<"op">();
					return opInfo.elementInfo == getTypeInfo<SerializeTypeOf<element_type>>();
				}
			);
			if (emptyError.hasValue()) { return emptyError.value(); }

			return ser.writeEmptyOptional<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename BaseSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsEnumBaseSerializeTypeC<BaseSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEnum(
			const natl::BasicSerializeTypeToType<BaseSerializeType> intValue,
			const natl::ConstAsciiStringView& strValue) noexcept {
			auto error = checkBasicType("writeEnum", WriteState::Type::cEnum, SerializeErrorLocation::writeEnum);
			if (error.hasValue()) { return error.value(); }
			return ser.writeEnum<Flags, CustomFlags, SerializeComponentType, BaseSerializeType>(intValue, strValue);
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, Size FarraySize>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArray() noexcept {
			auto beginError = checkBeginWrite("beginWriteFixedArray", WriteState::Type::farray, natl::SerializeErrorLocation::beginWriteFixedArray);
			if (beginError.hasValue()) { return beginError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::FarrayInfo& farrayInfo = write.memberInfo.get<"farray">();
			farrayInfo.size = FarraySize;
			farrayInfo.index = 0;

			return ser.beginWriteFixedArray<Flags, CustomFlags, SerializeComponentType, FarraySize>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, Size FarraySize>
			requires(natl::IsSerializeComponentC<SerializeComponentType>&& natl::IsSerializeFixedArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArray() noexcept {
			constexpr natl::SerializeErrorLocation errorLocation = natl::SerializeErrorLocation::endWriteFixedArray;
			auto endError = checkEndWrite("endWriteFixedArray", WriteState::Type::farray, errorLocation);
			if (endError.hasValue()) { return endError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::FarrayInfo& farrayInfo = write.memberInfo.get<"farray">();
			auto endContainerError = checkEndWriteContainer("endWriteFixedArray", farrayInfo.size, farrayInfo.index, errorLocation);
			if (endContainerError.hasValue()) { return endContainerError.value(); }

			return ser.endWriteFixedArray<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteFixedArrayElement() noexcept {
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using element_type = serialize_type::element_type;
			auto beginError = checkBeginWriteElement("beginWriteFixedArrayElement", WriteState::Type::farray, 
				natl::SerializeErrorLocation::beginWriteFixedArrayElement,
				[](const WriteState& write) {
					WriteState& write = stateStack.back();
					typename WriteState::FarrayInfo& farrayInfo = write.memberInfo.get<"farray">();
					return farrayInfo.elementInfo == getTypeInfo<SerializeTypeOf<element_type>>();
				}
			);
			if (beginError.hasValue()) { return beginError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::FarrayInfo& farrayInfo = write.memberInfo.get<"farray">();
			auto beingContainerError = checkBeginWriteContainerElement("beginWriteFixedArrayElement",
				farrayInfo.size, farrayInfo.index, natl::SerializeErrorLocation::beginWriteFixedArrayElement);
			if (beingContainerError.hasValue()) { return beingContainerError.value(); }
			farrayInfo.index++;

			pushNewWrite<element_type>(false, WriteState::Type::farray);

			return ser.beginWriteFixedArrayElement<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteFixedArrayElement() noexcept {
			auto endElementError = checkEndWriteElement("endWriteFixedArrayElement", WriteState::Type::farray, natl::SerializeErrorLocation::endWriteFixedArrayElement);
			if (endElementError.hasValue()) { return endElementError.value(); }
			stateStack.popBack();
			return ser.endWriteFixedArrayElement<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyArray() noexcept {
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using element_type = serialize_type::element_type;

			auto emptyError = checkWriteEmpty("writeEmptyArray", WriteState::Type::array
				, natl::SerializeErrorLocation::writeEmptyArray
				, [&](const WriteState& write) {
					const typename WriteState::ArrayInfo& arrayInfo = write.memberInfo.get<"array">();
					return arrayInfo.elementInfo == getTypeInfo<SerializeTypeOf<element_type>>();
				}
			);
			if (emptyError.hasValue()) { return emptyError.value(); }

			return ser.writeEmptyArray<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArray(natl::Size size) noexcept {
			auto beginError = checkBeginWrite("beginWriteArray", WriteState::Type::array, 
				natl::SerializeErrorLocation::beginWriteArray);
			if (beginError.hasValue()) { return beginError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::ArrayInfo& arrayInfo = write.memberInfo.get<"array">();
			arrayInfo.size = size;
			arrayInfo.index = 0;

			return ser.beginWriteArray<Flags, CustomFlags, SerializeComponentType>(size);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeArrayComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArray() noexcept {
			auto endError = checkEndWrite("endWriteArray", WriteState::Type::array, 
				natl::SerializeErrorLocation::endWriteArray);
			if (endError.hasValue()) { return endError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::ArrayInfo& arrayInfo = write.memberInfo.get<"array">();
			auto endContainerError = checkEndWriteContainer("endWriteArray", arrayInfo.size, arrayInfo.index,
				natl::SerializeErrorLocation::endWriteArray);
			if (endContainerError.hasValue()) { return endContainerError.value(); }

			return ser.endWriteArray<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteArrayElement() noexcept {
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using element_type = serialize_type::element_type;
			auto beginError = checkBeginWriteElement("beginWriteArrayElement", WriteState::Type::array,
				natl::SerializeErrorLocation::beginWriteArrayElement,
				[&](const WriteState& write) {
					const typename WriteState::ArrayInfo& arrayInfo = write.memberInfo.get<"array">();
					return arrayInfo.elementInfo == getTypeInfo<SerializeTypeOf<element_type>>();
				}
			);
			if (beginError.hasValue()) { return beginError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::ArrayInfo& arrayInfo = write.memberInfo.get<"array">();
			auto beingContainerError = checkBeginWriteContainerElement("beginWriteArrayElement",
				arrayInfo.size, arrayInfo.index, natl::SerializeErrorLocation::beginWriteArrayElement);
			if (beingContainerError.hasValue()) { return beingContainerError.value(); }
			arrayInfo.index++;

			pushNewWrite<SerializeTypeOf<element_type>>(false, WriteState::Type::array);

			return ser.beginWriteArrayElement<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteArrayElement() noexcept {
			auto endElementError = checkEndWriteElement("endWriteArrayElement", WriteState::Type::array, natl::SerializeErrorLocation::endWriteArrayElement);
			if (endElementError.hasValue()) { return endElementError.value(); }
			stateStack.popBack();
			return ser.endWriteArrayElement<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyDic() noexcept {

			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using key_serialize_type = serialize_type::key_serialize_type;
			using value_serialize_type = serialize_type::value_serialize_type;

			auto emptyError = checkWriteEmpty("writeEmptyDic", WriteState::Type::dic
				, natl::SerializeErrorLocation::writeEmptyDic
				, [&](const WriteState& write) {
					const typename WriteState::DicInfo& dicInfo = write.memberInfo.get<"dic">();
					return dicInfo.keyInfo == getTypeInfo<key_serialize_type>()
						&& dicInfo.valueInfo == getTypeInfo<value_serialize_type>();
				}
			);
			if (emptyError.hasValue()) { return emptyError.value(); }

			return ser.writeEmptyDic<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDic(natl::Size size) noexcept {
			auto beginError = checkBeginWrite("beginWriteDic", WriteState::Type::dic,
				natl::SerializeErrorLocation::beginWriteDic);
			if (beginError.hasValue()) { return beginError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::DicInfo& dicInfo = write.memberInfo.get<"dic">();
			dicInfo.size = size;
			dicInfo.index = 0;
			dicInfo.beganWritingKey = false;

			return ser.beginWriteDic<Flags, CustomFlags, SerializeComponentType>(size);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDic() noexcept {
			auto endError = checkEndWrite("endWriteDic", WriteState::Type::dic,
				natl::SerializeErrorLocation::endWriteDic);
			if (endError.hasValue()) { return endError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::DicInfo& dicInfo = write.memberInfo.get<"dic">();
			auto endContainerError = checkEndWriteContainer("endWriteDic", dicInfo.size, dicInfo.index,
				natl::SerializeErrorLocation::endWriteDic);
			if (endContainerError.hasValue()) { return endContainerError.value(); }

			return ser.endWriteDic<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteDicElement() noexcept {
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using key_serialize_type = serialize_type::key_serialize_type;
			using value_serialize_type = serialize_type::value_serialize_type;

			auto beginError = checkBeginWriteElement("beginWriteDicElement", WriteState::Type::dic,
				natl::SerializeErrorLocation::beginWriteArrayElement,
				[&](const WriteState& write) {
					const typename WriteState::DicInfo& dicInfo = write.memberInfo.get<"dic">();
					return dicInfo.keyInfo == getTypeInfo<key_serialize_type>()
						&& dicInfo.valueInfo == getTypeInfo<value_serialize_type>();
				}
			);
			if (beginError.hasValue()) { return beginError.value(); }

			return ser.beginWriteArrayElement<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteDicElement() noexcept {
			//end write dic 
			auto endElementError = checkEndWriteElement("endWriteDicElement", WriteState::Type::dic, 
				natl::SerializeErrorLocation::endWriteDicElement);
			if (endElementError.hasValue()) { return endElementError.value(); }
			stateStack.popBack();

			return endWriteDicElement<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeDicKeyComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicKey() noexcept {
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using key_serialize_type = serialize_type::key_serialize_type;

			//begin key element
			WriteState& write = stateStack.back();
			typename WriteState::DicInfo& dicInfo = write.memberInfo.get<"dic">();
			auto beingKeyError = checkBeginWriteContainerElement("writeDicKey",
				dicInfo.size, dicInfo.index, natl::SerializeErrorLocation::writeDicKey);
			if (beingKeyError.hasValue()) { return beingKeyError.value(); }

			pushNewWrite<key_serialize_type>(false, WriteState::Type::dic);
			dicInfo.beganWritingKey = true;

			return writeDicKey<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeDicValueComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeDicValue() noexcept {
			
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using key_serialize_type = serialize_type::key_serialize_type;
			using value_serialize_type = serialize_type::value_serialize_type;

			//end key element 
			auto endKeyError = checkEndWriteElement("writeDicValue", WriteState::Type::dic, 
				natl::SerializeErrorLocation::writeDicValue);
			if (endKeyError.hasValue()) { return endKeyError.value(); }
			stateStack.popBack();

			WriteState& write = stateStack.back();
			typename WriteState::DicInfo& dicInfo = write.memberInfo.get<"dic">();
			if (dicInfo.beganWritingKey != true) {
				return SerializeErrorHandler("cannot call writeDicValue until key has been written",
					natl::SerializeErrorLocation::writeDicValue, natl::SerializeErrorFlag::invalidState);
			}


			//begin value element
			auto beingValueError = checkBeginWriteContainerElement("writeDicValue",
				dicInfo.size, dicInfo.index, natl::SerializeErrorLocation::writeDicValue);
			if (beingValueError.hasValue()) { return beingValueError.value(); }
			dicInfo.index++;

			pushNewWrite<value_serialize_type>(false, WriteState::Type::dic);

			return writeDicValue<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteStruct() noexcept {
			auto beginError = checkBeginWrite("beginWriteStruct", WriteState::Type::cStruct, 
				natl::SerializeErrorLocation::beginWriteStruct);
			if (beginError.hasValue()) { return beginError.value(); }

			return ser.beginWriteStruct<Flags, CustomFlags, SerializeComponentType>();
		}
		//TODO add member check in writeNamed 
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteStruct() noexcept {
			auto endError = checkEndWrite("endWriteStruct", WriteState::Type::cStruct, natl::SerializeErrorLocation::endWriteStruct);
			if (endError.hasValue()) { return endError.value(); }
			return ser.endWriteStruct<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> writeEmptyVariant() noexcept {
			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;

			auto emptyError = checkWriteEmpty("writeEmptyVariant", WriteState::Type::variant
				, natl::SerializeErrorLocation::writeEmptyVariant
				, [](const WriteState& write) {
					return true;
				}
			);
			if (emptyError.hasValue()) { return emptyError.value(); }

			return ser.writeEmptyVariant<Flags, CustomFlags, SerializeComponentType>();
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType, natl::Size Index>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeVariantC<VariantSerializeType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteVariant(const natl::ConstAsciiStringView& strValue) noexcept {
			auto beginError = checkBeginWrite("beginWriteVariant", WriteState::Type::variant, 
				natl::SerializeErrorLocation::beginWriteVariant);
			if (beginError.hasValue()) { return beginError.value(); }

			using serialize_type = SerializeTypeOf<typename SerializeComponentType::type>;
			using serialize_element_type = serialize_type::serialize_types::template at<Index>;
			pushNewWrite<serialize_element_type>(false, WriteState::Type::variant);

			return ser.beginWriteVariant<Flags, CustomFlags, SerializeComponentType, VariantSerializeType, Index>(strValue);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename VariantSerializeType>
			requires(natl::IsSerializeComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteVariant() noexcept {

			auto endElementError = checkEndWriteElement("endWriteVariant", WriteState::Type::variant, 
				natl::SerializeErrorLocation::endWriteVariant);
			if (endElementError.hasValue()) { return endElementError.value(); }
			stateStack.popBack();

			auto endError = checkEndWrite("endWriteVariant", WriteState::Type::variant, 
				natl::SerializeErrorLocation::endWriteVariant);
			if (endError.hasValue()) { return endError.value(); }

			return ser.endWriteVariant<Flags, CustomFlags, SerializeComponentType, VariantSerializeType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> 
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> beginWriteJumpTable(const natl::ConstAsciiStringView name) noexcept {
			auto activeNamedError = checkActiveAndWrite("beginWriteJumpTable", WriteState::Type::jumpTable,
				natl::SerializeErrorLocation::beginWriteJumpTable);
			if (activeNamedError.hasValue()) { return activeNamedError.value(); }

			if (stateStack.size() != 0) {
				return SerializeErrorHandler(natl::sformat("cannot write jump table outside of global scope"),
					natl::SerializeErrorLocation::beginWriteJumpTable, natl::SerializeErrorFlag::invalidState);
			}

			typename WriteState::JumpTableInfo jumpTableInfo;
			jumpTableInfo.numberInfo = getTypeInfo<IdNumberType>();

			WriteState write = newWriteState(false, WriteState::Type::jumpTable);
			write.memberInfo.assign<"jumpTable">(jumpTableInfo);
			stateStack.push(write);
			write.beganWritingValue = true;

			return ser.beginWriteJumpTable<Flags, CustomFlags, SerializeComponentType, IdNumberType>(name);
		}
		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> && natl::IsSerializeGlobalComponentC<SerializeComponentType>)
		[[nodiscard]] constexpr natl::Option<SerializeErrorHandler> endWriteTable() noexcept {
			auto endError = checkEndWrite("endWriteTable", WriteState::Type::jumpTable,
				natl::SerializeErrorLocation::endWriteTable);
			if (endError.hasValue()) { return endError.value(); }

			stateStack.popBack();

			return ser.endWriteTable<Flags, CustomFlags, SerializeComponentType>();
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType, 
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> 
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpSaveId, SerializeErrorHandler> writeJumpTableElement(const natl::SerializeJumpId<IdNumberType>& element) noexcept {
			auto activeNamedError = checkActiveAndWrite("writeJumpTableElement", WriteState::Type::jumpTable, 
				SerializeErrorLocation::writeJumpTableElement);
			if (activeNamedError.hasValue()) { return activeNamedError.value(); }

			WriteState& write = stateStack.back();
			typename WriteState::JumpTableInfo& jumpTableInfo = write.memberInfo.get<"jumpTable">();
			if (jumpTableInfo.numberInfo != getTypeInfo<IdNumberType>()) {
				return SerializeErrorHandler(natl::sformat("cannot write jump table elemnent with different IdNumberType"),
					natl::SerializeErrorFlag::beginWriteJumpTable, natl::SerializeErrorFlag::wrongType);
			}

			return ser.writeJumpTableElement<Flags, CustomFlags, SerializeComponentType, IdNumberType>(element);
		}

		template<natl::SerializeWriteFlag Flags, custom_write_flag_type CustomFlags, typename SerializeComponentType,
			typename IdNumberType>
			requires(natl::IsSerializeComponentC<SerializeComponentType> 
				&& natl::IsSerializeGlobalComponentC<SerializeComponentType>
				&& natl::IsBuiltInUnsignedIntegerC<IdNumberType>)
		[[nodiscard]] constexpr natl::Expect<natl::SerializeJumpLocation, SerializeErrorHandler> writeJumpLocation(
			const natl::SerializeJumpId<IdNumberType>& element,
			const natl::SerializeJumpSaveId& saveId) noexcept {
			return ser.writeJumpLocation<Flags, CustomFlags, SerializeComponentType, IdNumberType>(element, saveId);
		}
	};
}