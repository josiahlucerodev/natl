#pragma once 

//own
#include "basicTypes.h"
#include "stringLiteral.h"
#include "typeTraits.h"
#include "typePack.h"
#include "tuple.h"
#include "stringConvert.h"
#include "characterTest.h"
#include "iteration.h"
#include "strongType.h"
#include "typeInfo.h"

//interface
namespace natl {
	template<typename Type, typename CharType = Ascii>
	struct  Formatter;

	template<typename OutputIter>
	class FormatOutputIter {
	public:
		using value_type = OutputIter::value_type;
		using reference = OutputIter::reference;
		using const_reference = OutputIter::const_reference;
		using pointer = OutputIter::pointer;
		using const_pointer = OutputIter::const_pointer;
		using difference_type = OutputIter::difference_type;
		using size_type = OutputIter::size_type;

		using iterator_category = std::output_iterator_tag;
	private:
		OutputIter outputIter;
		Size internalColumnNumber;
		Size internalLineNumber;
	public:
		//constructor
		constexpr FormatOutputIter() noexcept {}
		constexpr FormatOutputIter(OutputIter outputIterIn) noexcept
			: outputIter(outputIterIn), internalColumnNumber(0), internalLineNumber(0) {}

		//destructor 
		constexpr ~FormatOutputIter() noexcept = default;

		//util 
		constexpr FormatOutputIter& self() { return *this; }
		constexpr const FormatOutputIter& self() const { return *this; }

		//modifiers 
		constexpr void reserve(const size_type newCapacity) noexcept {
			if constexpr (HasReserve<OutputIter>) {
				outputIter.reserve(newCapacity);
			}
		}

		//iterator operation 
		constexpr Size lineNumber() const noexcept {
			return internalLineNumber;
		}
		constexpr Size columnNumber() const noexcept {
			return internalColumnNumber;
		}
		constexpr OutputIter getOutputIter() noexcept {
			return outputIter;
		}
		constexpr FormatOutputIter& operator=(const value_type& value) noexcept {
			if (value == '\n') {
				internalLineNumber += 1;
				internalColumnNumber = 0;
			} else {
				internalColumnNumber += 1;
			}

			outputIter = value;
			return self();
		}

		//no op 
		constexpr FormatOutputIter& operator*() noexcept { return self(); }
		constexpr FormatOutputIter& operator++() noexcept { return self(); }
		constexpr FormatOutputIter& operator++(int) noexcept { return self(); }
	};

	template<typename ArgType, 
		typename TemplateFlagsTypePack, 
		typename ArgFlagTypesTypePack = TypePack<>,
		typename ArgFlagsStorageTuple = Tuple<>>
	class FormatArgFlags {
	public:
		using template_flags = TemplateFlagsTypePack;
		using arg_flags = ArgFlagTypesTypePack;
		using arg_type = ArgType;
		using arg_flags_storage_tuple = ArgFlagsStorageTuple;
	private:
		ArgType arg;
		ArgFlagsStorageTuple argFlagsStorage;
	public:
		constexpr FormatArgFlags(
			ArgType&& argIn,
			ArgFlagsStorageTuple&& argFlagsStorageIn = ArgFlagsStorageTuple()) :
			arg(natl::forward<ArgType>(argIn)),
			argFlagsStorage(natl::forward<ArgFlagsStorageTuple>(argFlagsStorageIn)) {}
		constexpr arg_type&& getArg() noexcept { return natl::forward<ArgType>(arg); }
		constexpr ArgFlagsStorageTuple& getArgFlags() noexcept { return argFlagsStorage; }
	};

	template<typename... FlagTypes>
	constexpr auto formatArg(auto arg, auto... flagArgs) noexcept {
		using arg_type = decltype(arg);
		using arg_flags = TypePack<decltype(flagArgs)...>;
		using template_flags = TypePack<FlagTypes...>;
		using arg_flags_storage_tuple = Tuple<decltype(flagArgs)...>;

		using format_arg_flags = FormatArgFlags<
			arg_type,
			template_flags,
			arg_flags,
			arg_flags_storage_tuple>;
		return format_arg_flags(
			natl::forward<arg_type>(arg), 
			natl::forward<arg_flags_storage_tuple>(arg_flags_storage_tuple(flagArgs...)));
	}

	template<TemplateStringLiteral textFlag>
	constexpr auto fromatTextFlag() noexcept { return StringLiteral<textFlag>{}; }

	template<TemplateStringLiteral... textFlags>
	constexpr auto formatArgText(auto arg, auto... flagArgs) noexcept {
		using arg_type = decltype(arg);
		using template_flags = TypePack<StringLiteral<textFlags>...>;
		using arg_flags = TypePack<decltype(flagArgs)...>;
		using arg_flags_storage_tuple = Tuple<decltype(flagArgs)...>;
		using format_arg_flags = FormatArgFlags<
			arg_type,
			template_flags,
			arg_flags,
			arg_flags_storage_tuple>;
		return format_arg_flags(
			natl::forward<arg_type>(arg),
			natl::forward<arg_flags_storage_tuple>(arg_flags_storage_tuple(flagArgs...)));
	}

	template<typename Type>
	struct IsFormatArgFlagsT : FalseType {};
	template<typename ArgType, typename... FlagTypes>
	struct IsFormatArgFlagsT<FormatArgFlags<ArgType, FlagTypes...>> : TrueType {};

	namespace impl {
		template<typename ArgType, typename CharType, typename... FlagTypes>
		struct CreateFormatterWithTemplateFlagsImpl {
			using formatter = void;
		};

		template<typename ArgType, typename CharType, typename... FlagTypes>
		struct CreateFormatterWithTemplateFlagsImpl<ArgType, CharType, TypePack<FlagTypes...>> {
			using formatter = Formatter<ArgType, CharType>::template WithTemplateFlags<FlagTypes...>;
		};
		template<typename ArgType, typename CharType>
		struct CreateFormatterWithTemplateFlagsImpl<ArgType, CharType, TypePack<>> {
			using formatter = Formatter<ArgType, CharType>;
		};

		template<typename ArgType, typename CharType, typename FlagsTypePack>
			requires(IsTypePack<FlagsTypePack>)
		using CreateFormatterWithTemplateFlags = CreateFormatterWithTemplateFlagsImpl<ArgType, CharType, FlagsTypePack>::formatter;

		template<typename OutputIter, typename ArgType, typename FormatterType, typename StorageTuple, Size... Indices>
		constexpr void formatToArgCallFormatImpl(OutputIter& outputIter, ArgType&& arg, StorageTuple&& storageTuple, IndexSequence<Indices...>) noexcept {
			outputIter = FormatterType::format(outputIter, arg, natl::forward<TupleElement<Indices, StorageTuple>>(storageTuple.template get<Indices>())...);
		}
		template<typename OutputIter, typename ArgType, typename FormatterType, typename StorageTuple, Size numberOfFormatArgs>
		constexpr void formatToArgCallFormat(OutputIter& outputIter, ArgType&& arg, StorageTuple&& storageTuple) noexcept {
			formatToArgCallFormatImpl<OutputIter, ArgType, FormatterType, StorageTuple>(outputIter, natl::forward<ArgType>(arg), natl::forward<StorageTuple>(storageTuple), MakeIndexSequence<numberOfFormatArgs>{});
		}

		template<typename OutputIter, typename ArgType, typename CharType>
		constexpr void formatToArgLevel(OutputIter& outputIter, ArgType&& arg) noexcept {
			if constexpr (IsFormatArgFlagsT<ArgType>::value) {
				using FormatArgType = ArgType;
				using arg_flags_storage_tuple = FormatArgType::arg_flags_storage_tuple;
				using arg_type = FormatArgType::arg_type;

				if constexpr (ArgType::template_flags::size > 0) {
					using formatter_type = CreateFormatterWithTemplateFlags<typename FormatArgType::arg_type, CharType, typename FormatArgType::template_flags>;
					formatToArgCallFormat<OutputIter, arg_type, formatter_type, arg_flags_storage_tuple, TupleSize<arg_flags_storage_tuple>>(
							outputIter, natl::forward<arg_type>(arg.getArg()), natl::forward<arg_flags_storage_tuple>(arg.getArgFlags()));
				} else {
					using formatter_type = Formatter<RemoveConctVolatile<arg_type>, CharType>;
					formatToArgCallFormat<OutputIter, arg_type, formatter_type, arg_flags_storage_tuple, TupleSize<arg_flags_storage_tuple>>(
						outputIter, natl::forward<arg_type>(arg.getArg()), natl::forward<arg_flags_storage_tuple>(arg.getArgFlags()));
				}
			} else {
				using arg_flags_storage_tuple = Tuple<>;
				using formatter_type = Formatter<RemoveConctVolatile<RemoveReference<ArgType>>, CharType>;
				formatToArgCallFormat<OutputIter, ArgType, formatter_type, arg_flags_storage_tuple, 0>(
					outputIter, natl::forward<ArgType>(arg), natl::forward<arg_flags_storage_tuple>(arg_flags_storage_tuple()));
			}
		}
	}

	template<typename ArgType, typename CharType>
	concept Formattable = requires(ArgType&& arg, 
		FormatOutputIter<BackInsertIterator<BaseStringByteSize<CharType, 32>>> outputIter) {
		{ impl::formatToArgLevel<decltype(outputIter), ArgType, CharType>(outputIter, natl::forward<ArgType>(arg)) };
	};

	template<typename OutputIter, typename... ArgTypes>
		requires(Formattable<ArgTypes, Ascii> && ...)
	constexpr OutputIter formatTo(OutputIter outputIter, ArgTypes&&... args) noexcept {
		using format_output_iter = FormatOutputIter<OutputIter>;
		format_output_iter formatOutputIter = format_output_iter(outputIter);
		(impl::formatToArgLevel<format_output_iter, ArgTypes, Ascii>(formatOutputIter, natl::forward<ArgTypes>(args)), ...);
		return formatOutputIter.getOutputIter();
	}

	template<typename DynStringType, typename... ArgTypes>
		requires(Formattable<ArgTypes, Ascii> && ...)
	constexpr DynStringType format(ArgTypes&&... args) noexcept {
		DynStringType outputString;
		natl::BackInsertIterator<DynStringType> outputIter = natl::backInserter(outputString);
		natl::formatTo<natl::BackInsertIterator<DynStringType>>(outputIter, natl::forward<ArgTypes>(args)...);
		return outputString;
	}

	template<typename... ArgTypes>
		requires(Formattable<ArgTypes, Ascii> && ...)
	constexpr String sFormat(ArgTypes&&... args) noexcept {
		return natl::format<String>(natl::forward<ArgTypes>(args)...);
	}

	struct FormatColumn {
		Size columnNumber;
		Ascii fillCharacter;

		//constructor
		constexpr FormatColumn() noexcept : columnNumber(0), fillCharacter() {}
		constexpr FormatColumn(const Size columnNumberIn, const Ascii fillCharacterIn = ' ') noexcept :
			columnNumber(columnNumberIn), fillCharacter(fillCharacterIn) {}

		//destructor
		constexpr ~FormatColumn() noexcept = default;
	};


	template<typename CharType>
	struct Formatter<FormatColumn, CharType> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const FormatColumn formatColumn) noexcept {
			if (outputIter.columnNumber() < formatColumn.columnNumber) {
				const Size columnDifference = formatColumn.columnNumber - outputIter.columnNumber();
				for (Size i = 0; i < columnDifference; i++) {
					outputIter = formatColumn.fillCharacter;
				}
			}
			return outputIter;
		}
	};

	struct FormatNewLine {};

	template<typename CharType>
	struct Formatter<FormatNewLine, CharType> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, FormatNewLine) noexcept {
			outputIter = '\n';
			return outputIter;
		}
	};

	template<>
	struct Formatter<Ascii, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const Ascii character) noexcept {
			outputIter += character;
			return outputIter;
		}
	};

	template<>
	struct Formatter<Ascii*, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const Ascii* str) noexcept {
			if (str == nullptr) {
				return outputIter;
			}

			Ascii character = *str;
			while (character != '\0') {
				outputIter += character;
				str += 1;
				character = *str;
			}

			return outputIter;
		}
	};

	template<Size StringSize>
	struct Formatter<Ascii[StringSize], Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const Ascii* str) noexcept {
			for (Size i = 0; i < StringSize - 1; i++) {
				outputIter = str[i];
			}
			return outputIter;
		}
	};

	template<>
	struct Formatter<BaseStringView<Ascii>, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const ConstAsciiStringView stringView) noexcept {
			for (const Ascii character : stringView) {
				outputIter = character;
			}
			return outputIter;
		}
	};
	template<>
	struct Formatter<BaseStringView<const Ascii>, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const ConstAsciiStringView stringView) noexcept {
			for (const Ascii character : stringView) {
				outputIter = character;
			}
			return outputIter;
		}
	};
	template<
		Size bufferSize,
		typename Alloc,
		Bool EnableDynAllocation,
		Bool EnableIncreasedSmallBufferSize>
	struct Formatter<BaseString<Ascii, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const ConstAsciiStringView stringView) noexcept {
			for (const Ascii character : stringView) {
				outputIter = character;
			}
			return outputIter;
		}
	};

	template<TemplateStringLiteral TStringL, typename CharType>
	struct Formatter<StringLiteral<TStringL>, CharType> {
		using value_type = StringLiteral<TStringL>;
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, value_type = value_type{}) noexcept {
			outputIter = Formatter<ConstAsciiStringView, CharType>::format<OutputIter>(outputIter, value_type::toStringView());
			return outputIter;
		}
	};

	enum class BoolFormat {
		fullLowercase,
		fullFirstUppercase,
		fullAllUppercase,
		shorthandLowercase,
		shorthandUppercase,

		full = fullLowercase,
		shorthand = shorthandLowercase,
		standard = full
	};

	struct FormatBoolFullLowercaseFlag {};
	struct FormatBoolFullFirstUppercaseFlag {};
	struct FormatBoolFullAllUppercaseFlag {};
	struct FormatBoolShorthandLowercaseFlag {};
	struct FormatBoolShorthandUppercaseFlag {};

	using FormatBoolFullFlag = FormatBoolFullLowercaseFlag;
	using FormatBoolShorthandFlag = FormatBoolShorthandLowercaseFlag;
	using FormatBoolStandardFlag = FormatBoolFullFlag;

	template<typename CharType>
	struct Formatter<Bool, CharType> {
		using value_type = Bool;

		template<typename OutputIter>
		constexpr static void universalFormat(OutputIter& outputIter, const Bool booleanValue, const BoolFormat boolFormat) noexcept {
			switch (boolFormat) {
			case BoolFormat::fullLowercase:
				if (booleanValue) {
					outputIter = 't';
					outputIter = 'r';
					outputIter = 'u';
					outputIter = 'e';
				} else {
					outputIter = 'f';
					outputIter = 'a';
					outputIter = 'l';
					outputIter = 's';
					outputIter = 'e';
				}
				break;
			case BoolFormat::fullFirstUppercase:
				if (booleanValue) {
					outputIter = 'T';
					outputIter = 'r';
					outputIter = 'u';
					outputIter = 'e';
				} else {
					outputIter = 'F';
					outputIter = 'a';
					outputIter = 'l';
					outputIter = 's';
					outputIter = 'e';
				}
				break;
			case BoolFormat::fullAllUppercase:
				if (booleanValue) {
					outputIter = 'T';
					outputIter = 'R';
					outputIter = 'U';
					outputIter = 'E';
				} else {
					outputIter = 'F';
					outputIter = 'A';
					outputIter = 'L';
					outputIter = 'S';
					outputIter = 'E';
				}
				break;
			case BoolFormat::shorthandLowercase:
				if (booleanValue) {
					outputIter = 't';
				} else {
					outputIter = 'f';
				}
				break;
			case BoolFormat::shorthandUppercase:
				if (booleanValue) {
					outputIter = 'T';
				} else {
					outputIter = 'F';
				}
				break;
			default:
				unreachable();
			}
		}

		template<typename... TemplateFlags>
		class WithTemplateFlags {
		public:
			template<typename TemplateFlag>
			constexpr static void handelTemplateFlag(BoolFormat& boolFormat) noexcept {
				if constexpr (IsStringLiteralV<TemplateFlag>) {
					constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
					if constexpr (tflagName == "shorthand" || tflagName == "s") {
						boolFormat = BoolFormat::shorthand;
					} else if constexpr (tflagName == "Full") {
						boolFormat = BoolFormat::fullFirstUppercase;
					} else if constexpr (tflagName == "FULL" || tflagName == "F") {
						boolFormat = BoolFormat::fullAllUppercase;
					} else if constexpr (tflagName == "Shorthand" || tflagName == "S") {
						boolFormat = BoolFormat::shorthandUppercase;
					} else if constexpr (tflagName == "full" || tflagName == "f") {
						boolFormat = BoolFormat::fullLowercase;
					} else {
						unreachable();
					}
				} else {
					if constexpr (IsSame<TemplateFlag, FormatBoolShorthandLowercaseFlag>) {
						boolFormat = BoolFormat::shorthand;
					} else if constexpr (IsSame<TemplateFlag, FormatBoolFullFirstUppercaseFlag>) {
						boolFormat = BoolFormat::fullFirstUppercase;
					} else if constexpr (IsSame<TemplateFlag, FormatBoolFullAllUppercaseFlag>) {
						boolFormat = BoolFormat::fullAllUppercase;
					} else if constexpr (IsSame<TemplateFlag, FormatBoolShorthandUppercaseFlag>) {
						boolFormat = BoolFormat::shorthandUppercase;
					} else if constexpr (IsSame<TemplateFlag, FormatBoolFullLowercaseFlag>) {
						boolFormat = BoolFormat::fullLowercase;
					} else {
						unreachable();
					}
				}
			}

		public:
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const Bool booleanValue, BoolFormat boolFormat = BoolFormat::standard) noexcept {
				(handelTemplateFlag<TemplateFlags>(boolFormat), ...);
				universalFormat<OutputIter>(outputIter, booleanValue, boolFormat);
				return outputIter;
			}
		};

		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const Bool booleanValue, BoolFormat boolFormat = BoolFormat::standard) noexcept {
			universalFormat<OutputIter>(outputIter, booleanValue, boolFormat);
			return outputIter;
		}
	};

	enum class IntFormat {
		decimal,
		hexadecimal,
		binary,
		standard = decimal,
	};

	struct FormatIntHexadecimalFlag {};
	struct FormatIntBinaryFlag {};
	struct FormatIntDecimalFlag {};

	using FormatIntHexFlag = FormatIntHexadecimalFlag;
	using FormatIntBinFlag = FormatIntBinaryFlag;
	using FormatIntDecFlag = FormatIntDecimalFlag;

	namespace impl {
		template<typename InteagerType, typename CharType>
		struct IntegerFormatter {
		public:
			using value_type = InteagerType;

			template<typename OutputIter>
			constexpr static void formatHexadecimal(OutputIter& outputIter, const value_type number) noexcept {
				natl::String numberAsString = intToStringHexadecimal(number);
				outputIter = copyNoOverlap<natl::String::const_iterator, OutputIter>(
					numberAsString.cbegin(),
					numberAsString.cend(),
					outputIter);
			}
			template<typename OutputIter>
			constexpr static void formatBinary(OutputIter& outputIter, const value_type number) noexcept {
				natl::String numberAsString = intToStringBinary(number);
				outputIter = copyNoOverlap<natl::String::const_iterator, OutputIter>(
					numberAsString.cbegin(),
					numberAsString.cend(),
					outputIter);
			}
			template<typename OutputIter>
			constexpr static void formatDecimal(OutputIter& outputIter, const value_type number) noexcept {
				natl::String numberAsString = intToStringDecimal(number);
				outputIter = copyNoOverlap<natl::String::const_iterator, OutputIter>(
					numberAsString.cbegin(),
					numberAsString.cend(),
					outputIter);
			}

			template<typename OutputIter>
			constexpr static void univeralFormat(OutputIter& outputIter, const value_type number, const IntFormat intFormat) noexcept {
				switch (intFormat) {
				case IntFormat::hexadecimal:
					formatHexadecimal<OutputIter>(outputIter, number);
					break;
				case IntFormat::binary:
					formatBinary<OutputIter>(outputIter, number);
					break;
				case IntFormat::decimal:
					formatDecimal<OutputIter>(outputIter, number);
					break;
				default:
					unreachable();
				}
			}

			template<typename... TemplateFlags>
			class WithTemplateFlags {
			public:

				template<typename TemplateFlag>
				constexpr static void handelTemplateFlag(IntFormat& intFormat) noexcept {
					if constexpr (IsStringLiteralV<TemplateFlag>) {
						constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();

						if constexpr (tflagName == "hexadecimal" || tflagName == "hex") {
							intFormat = IntFormat::hexadecimal;
						} else if constexpr (tflagName == "binary" || tflagName == "bin") {
							intFormat = IntFormat::binary;
						} else if constexpr (tflagName == "decimal" || tflagName == "dec") {
							intFormat = IntFormat::decimal;
						} else {
							unreachable();
						}
					} else {
						if constexpr (IsSame<TemplateFlag, FormatIntHexadecimalFlag>) {
							intFormat = IntFormat::hexadecimal;
						} else if constexpr (IsSame<TemplateFlag, FormatIntBinaryFlag>) {
							intFormat = IntFormat::binary;
						} else if constexpr (IsSame<TemplateFlag, FormatIntDecimalFlag>) {
							intFormat = IntFormat::decimal;
						} else {
							unreachable();
						}
					}
				}

			public:
				template<typename OutputIter>
				constexpr static OutputIter format(OutputIter outputIter, const value_type number, IntFormat intFormat = IntFormat::standard) noexcept {
					(handelTemplateFlag<TemplateFlags>(intFormat), ...);
					univeralFormat<OutputIter>(outputIter, number, intFormat);
					return outputIter;
				}
			};

		public:
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const value_type number, IntFormat intFormat = IntFormat::standard) noexcept {
				univeralFormat<OutputIter>(outputIter, number, intFormat);
				return outputIter;
			}
		};
	}

	template<typename CharType> 
	struct Formatter<i8, CharType> : impl::IntegerFormatter<i8, CharType> {};
	template<typename CharType> 
	struct Formatter<i16, CharType> : impl::IntegerFormatter<i16, CharType> {};
	template<typename CharType> 
	struct Formatter<i32, CharType> : impl::IntegerFormatter<i32, CharType> {};
	template<typename CharType> 
	struct Formatter<i64, CharType> : impl::IntegerFormatter<i64, CharType> {};

	template<typename CharType>
	struct Formatter<ui8, CharType> : impl::IntegerFormatter<ui8, CharType> {};
	template<typename CharType>
	struct Formatter<ui16, CharType> : impl::IntegerFormatter<ui16, CharType> {};
	template<typename CharType>
	struct Formatter<ui32, CharType> : impl::IntegerFormatter<ui32, CharType> {};
	template<typename CharType>
	struct Formatter<ui64, CharType> : impl::IntegerFormatter<ui64, CharType> {};

	enum class FloatFormat {
		standard,
	};

	struct FormatFloatStandardFlag {};
	
	namespace impl {
		struct FormatFloatPrecisionTag {};
	}
	using FormatFloatPrecision = StrongType<Size, impl::FormatFloatPrecisionTag>;
	template<Size PrecisionValue>
	using FormatFloatPrecisionType = TypeValue<FormatFloatPrecision(PrecisionValue)>;

	namespace impl {
		template<typename FormatArgType>
		concept IsValidFloatFormatArgType =
			IsSame<Decay<FormatArgType>, FloatFormat>
			|| IsSame<Decay<FormatArgType>, FormatFloatPrecision>;
	}

	namespace impl {
		template<typename FloatType, typename CharType>
		struct FloatFormatter {
		public:
			using value_type = FloatType;
			
			template<typename OutputIter>
			constexpr static void formatStandard(OutputIter& outputIter, 
				const value_type number, 
				const Size precision) noexcept {
				natl::String numberAsString = floatToStringDecimal(number, precision);
				outputIter = copyNoOverlap<natl::String::const_iterator, OutputIter>(
					numberAsString.cbegin(),
					numberAsString.cend(),
					outputIter);
			}

			template<typename OutputIter>
			constexpr static void universalFormat(OutputIter& outputIter, 
				const value_type number, 
				const Size precision,
				const FloatFormat floatFormat) noexcept {

				switch (floatFormat) {
				case FloatFormat::standard:
					formatStandard(outputIter, number, precision);
					break;
				default:
					unreachable();
				}
			}

			template<typename... TemplateFlags>
			class WithTemplateFlags {
			public:
				template<typename TemplateFlag>
				constexpr static void handelTemplateFlag(Size& precision, FloatFormat& floatFormat) noexcept {
					if constexpr (IsStringLiteralV<TemplateFlag>) {
						constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();

						if constexpr (tflagName.starts_with("p: ")) {
							constexpr Size newPrecision = stringDecimalToInt(tflagName.substr(3));
							precision = newPrecision;
						} else if constexpr (tflagName == "standard") {
							floatFormat = FloatFormat::standard;
						} else {
							unreachable();
						}
					} else {
						if constexpr (IsSame<TemplateFlag, FormatFloatStandardFlag>) {
							floatFormat = FloatFormat::standard;
						} else {
							unreachable();
						}
					}
				}

			public:

				template<typename FormatArgType>
				constexpr static void handelFormatArg(FormatArgType&& formatArg, Size& precision, FloatFormat& floatFormat) noexcept {
					if constexpr (IsSame<Decay<FormatArgType>, FloatFormat>) {
						floatFormat = formatArg;
					} else if constexpr(IsSame<Decay<FormatArgType>, FormatFloatPrecision>) {
						precision = formatArg.value();
					} else {
						unreachable();
					}
				}

				template<typename OutputIter, typename... FormatArgTypes>
					requires(impl::IsValidFloatFormatArgType<FormatArgTypes> && ...)
				constexpr static OutputIter format(OutputIter outputIter, const value_type number, FormatArgTypes&&... formatArgs) noexcept {
					Size precision = Limits<Size>::max();
					FloatFormat floatFormat = FloatFormat::standard;
					(handelTemplateFlag<TemplateFlags>(precision, floatFormat), ...);
					(handelFormatArg<FormatArgTypes>(forward<FormatArgTypes>(formatArgs), precision, floatFormat), ...);
					universalFormat<OutputIter>(outputIter, number, precision, floatFormat);
					return outputIter;
				}
			};

			template<typename OutputIter, typename... FormatArgTypes>
				requires(impl::IsValidFloatFormatArgType<FormatArgTypes> && ...)
			constexpr static OutputIter format(OutputIter outputIter, const value_type number, FormatArgTypes&&... formatArgs) noexcept {
				Size precision = Limits<Size>::max();
				FloatFormat floatFormat = FloatFormat::standard;
				(handelFormatArg<FormatArgTypes>(forward<FormatArgTypes>(formatArgs), precision, floatFormat), ...);
				universalFormat<OutputIter>(outputIter, number, precision, floatFormat);
				return outputIter;
			}
		};
	}

	template<typename CharType>
	struct Formatter<f32, CharType> : impl::FloatFormatter<f32, CharType> {};
	template<typename CharType>
	struct Formatter<f64, CharType> : impl::FloatFormatter<f64, CharType> {};

	template<Size Index, typename... TemplateFlags>
	struct FormatElement {
		using template_flags = TypePack<TemplateFlags...>;
	};
	

	namespace impl {
		template<typename>
		struct IsFormatElementV : FalseType {};
		template<Size Index, typename... TemplateFlags>
		struct IsFormatElementV<FormatElement<Index, TemplateFlags...>> : TrueType {};
		template<typename Type>
		constexpr inline Bool IsFormatElement = IsFormatElementV<Type>::value;

		template<Size Index, typename... ArgTypes>
		struct FormatElementArg {
			constexpr static Size index = Index;
			using arg_types = TypePack<ArgTypes...>;
			using storage_tuple_type = Tuple<ArgTypes&&...>;
		public:
			storage_tuple_type argStorage;
		};

		template<typename>
		struct IsFormatElementArgV : FalseType {};
		template<Size Index, typename... ArgTypes>
		struct IsFormatElementArgV<FormatElementArg<Index, ArgTypes...>> : TrueType {};
		template<typename Type>
		constexpr inline Bool IsFormatElementArg = IsFormatElementArgV<Type>::value;
	}

	template<typename... DataTypes, typename CharType>
		requires(Formattable<DataTypes, CharType> && ...)
	struct Formatter<Tuple<DataTypes...>, CharType> {
		using value_type = Tuple<DataTypes...>;
		constexpr static Size tupleArgSize = sizeof...(DataTypes);

		template<Size Index, typename... FormatArgTypes>
		struct HandelFormatArgT {
			template<Size Index, typename FormatArgType>
			struct IndexAndFormatArgPair {
				constexpr static Bool index = Index;
				using format_arg_type = FormatArgType;
			};

			template<typename>
			struct GeneratePairedTypePackT;

			template<Size... Indices>
			struct GeneratePairedTypePackT<IndexSequence<Indices...>> {
				using type = TypePack<IndexAndFormatArgPair<Indices, FormatArgTypes>...>;
			};

			using index_and_arg_pairs = GeneratePairedTypePackT<MakeIndexSequence<tupleArgSize>>;

			template<typename IndexAndArgPairType>
			struct IsArgOfIndexV {
				consteval static Bool testType() noexcept {
					if constexpr (impl::IsFormatElementArg<typename IndexAndArgPairType::format_arg_type>) {
						return IndexAndArgPairType::format_arg_type::index == Index;
					} else {
						return true;
					}
				}

				constexpr static Bool value = testType();
			};

			using filitered_format_arg_flags = TemplatePackFilter<IsArgOfIndexV, index_and_arg_pairs>;
			using type = filitered_format_arg_flags;
		};

	public:
		template<typename... TemplateFlags>
		class WithTemplateFlags {

			template<Size Index, typename... TemplateFlags>
			struct HandelTemplateFlags {

				template<typename TemplateFlag>
				struct IsFlagOfIndexV {
					consteval static Bool testTemplateFlag() noexcept {
						if constexpr (impl::IsFormatElementArg<TemplateFlag>) {
							return TemplateFlag::index == Index;
						} else if constexpr (IsStringLiteralV<TemplateFlag>) {
							constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
							if constexpr (tflagName.size() > 3 && tflagName[1] == ':' &&  tflagName[0] - '0' == Index) {
								return true;
							} else {
								return false;
							}
						} else {
							return true;
						}
					}

					constexpr static Bool value = testTemplateFlag();
				};

				using filitered_template_flags = TemplatePackFilter<IsFlagOfIndexV, TemplateFlags...>;

				template<typename TemplateFlag>
				struct ReduceTemplateFlagT {
					using type = TypePack<TemplateFlag>;
				};

				template<typename TemplateFlag>
					requires(impl::IsFormatElementArg<TemplateFlag>)
				struct ReduceTemplateFlagT<TemplateFlag> {
					using type = TemplateFlag::template_flags;
				};

				template<typename TemplateFlag>
					requires(IsStringLiteralV<TemplateFlag>)
				struct ReduceTemplateFlagT<TemplateFlag> {
					consteval static auto getReduceType() noexcept {
						constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
						if constexpr (tflagName.size() > 3 && tflagName[1] == ':' && tflagName[0] - '0' == Index) {
							return typename TemplateFlag::template Substr<3>{};
						} else {
							return TemplateFlag{};
						}
					}
					using type = decltype(getReduceType());
				};

				using reduced_template_flags = TypePackTransform<ReduceTemplateFlagT, filitered_template_flags>;

				template<typename TypePack> 
				struct TypePackExtractTemplateFlagsT;

				template<typename... TemplateFlagTypePacks>
				struct TypePackExtractTemplateFlagsT<TypePack<TemplateFlagTypePacks...>> {
					using type = TypePackMergeBlend<TemplateFlagTypePacks...>;
				};

				using extracted_template_flags = TypePackExtractTemplateFlagsT<reduced_template_flags>::type;
				using type = extracted_template_flags;
			};

			template<Size Index>
			struct FormatTupleElement {
				using template_flags = HandelTemplateFlags<Index, TemplateFlags...>::type;
				
				template<typename OutputIter, typename ArgType,typename... FormatArgTypes>
				constexpr static void baseFormat(OutputIter& outputIter, ArgType&& arg, FormatArgTypes&&... formatArgs) noexcept {
					if constexpr (Index > 0) {
						outputIter = ',';
						outputIter = ' ';
					}

					if constexpr (template_flags::size == 0) {
						outputIter = Formatter<Decay<ArgType>>::format(
							outputIter,
							natl::forward<ArgType>(arg),
							natl::forward<FormatArgTypes>(formatArgs)...);
					} else {
						using formatter = impl::CreateFormatterWithTemplateFlags<Decay<ArgType>, CharType, template_flags>;
						outputIter = formatter::format(
							outputIter,
							natl::forward<ArgType>(arg),
							natl::forward<FormatArgTypes>(formatArgs)...);
					}
				}

				template<typename OutputIter, typename FormatArgsStorageTuple, typename ArgType,
					typename IndicesTypePack, Size... Indices>
				constexpr static void formatWithArgsImpl(
					OutputIter& outputIter, FormatArgsStorageTuple& formatArgsStorageTuple,
					ArgType&& arg, IndexSequence<Indices...>) noexcept {

					baseFormat(outputIter,
						natl::forward<ArgType>(arg),
						formatArgsStorageTuple.template get<TypePackNthElement<Indices, IndicesTypePack>::index>()...
					);
				}

				template<typename OutputIter, typename FormatArgsStorageTuple,
					typename ArgType, typename... FormatArgTypes>
				constexpr static void formatWithArgs(
					OutputIter& outputIter,
					FormatArgsStorageTuple& formatArgsStorageTuple, ArgType&& arg) noexcept {


					using format_arg_indices = HandelFormatArgT<Index, FormatArgTypes...>;
					if constexpr (format_arg_indices::size == 0) {
						baseFormat<OutputIter, ArgType>(outputIter, natl::forward<ArgType>(arg));
					} else {
						formatWithArgsImpl<OutputIter, FormatArgsStorageTuple, ArgType, format_arg_indices>(
							outputIter,
							formatArgsStorageTuple,
							natl::forward<ArgType>(arg),
							MakeIndexSequence<format_arg_indices::size>{});
					}
				}
			};

			template<typename... FormatArgTypes>
			struct FormatElementsWithArgs {
				template<typename OutputIter, typename FormatArgsStorageTuple, Size... Indices>
				constexpr static void format(
					OutputIter outputIter, const value_type& tuple,
					FormatArgsStorageTuple& formatArgsStorageTuple, IndexSequence<Indices...>) noexcept {

					(formatWithArgs<
						OutputIter,
						FormatArgsStorageTuple,
						TupleElement<Indices, value_type>,
						FormatArgTypes...>(
							outputIter,
							formatArgsStorageTuple,
							tuple.template get<Indices>()) && ...);
				}
			};

			template<typename OutputIter, Size... Indices>
			constexpr static void formatElements(OutputIter outputIter, const value_type& tuple, IndexSequence<Indices...>) noexcept {
				(FormatTupleElement<Indices>::baseFormat(outputIter, tuple.template get<Indices>()), ...);
			}

		public:
			template<typename OutputIter, typename... FormatArgTypes>
			constexpr static OutputIter format(OutputIter outputIter, const value_type& tuple, FormatArgTypes&&... formatArgs) noexcept {
				outputIter = '{';

				if constexpr (sizeof...(FormatArgTypes) == 0) {
					formatElements(
						outputIter,
						tuple,
						MakeIndexSequence<tupleArgSize>{});
				} else {
					using format_args_storage_tuple = Tuple<FormatArgTypes&&...>;
					format_args_storage_tuple formatArgsStorageTuple(natl::forward<FormatArgTypes>(formatArgs)...);
					FormatElementsWithArgs<FormatArgTypes...>::format(
						outputIter,
						tuple,
						formatArgsStorageTuple,
						MakeIndexSequence<tupleArgSize>{});
				}

				outputIter = '}';

				return outputIter;
			}
		};

		

		template<typename OutputIter, typename ArgType, typename... FormatArgTypes>
		constexpr static void baseElementFormat(
			OutputIter& outputIter, ArgType&& arg, FormatArgTypes&&... formatArgs) noexcept {
			outputIter = Formatter<Decay<ArgType>>::format(
				outputIter, natl::forward<ArgType>(arg), natl::forward<FormatArgTypes>(formatArgs)...);
		}

		template<typename OutputIter, typename FormatArgsStorageTuple, typename ArgType,
			typename IndicesTypePack, Size... Indices>
		constexpr static void formatWithArgsImpl(
			OutputIter& outputIter, FormatArgsStorageTuple& formatArgsStorageTuple,
			ArgType&& arg, IndexSequence<Indices...>) noexcept {

			baseElementFormat(outputIter,
				natl::forward<ArgType>(arg),
				formatArgsStorageTuple.template get<TypePackNthElement<Indices, IndicesTypePack>::index>()...
			);
		}

		template<Size Index, typename OutputIter, typename FormatArgsStorageTuple,
			typename ArgType, typename... FormatArgTypes>
		constexpr static void formatWithArgs(
			ValueIdentityV<Index>, OutputIter& outputIter,
			FormatArgsStorageTuple& formatArgsStorageTuple, ArgType&& arg) noexcept {

			if constexpr (Index > 0) {
				outputIter = ',';
				outputIter = ' ';
			}

			using format_arg_indices = HandelFormatArgT<Index, FormatArgTypes...>;
			if constexpr (format_arg_indices::size == 0) {
				baseElementFormat<OutputIter, ArgType>(outputIter, natl::forward<ArgType>(arg));
			} else {
				formatWithArgsImpl<OutputIter, FormatArgsStorageTuple, ArgType, format_arg_indices>(
					outputIter, formatArgsStorageTuple,
					natl::forward<ArgType>(arg), MakeIndexSequence<format_arg_indices::size>{});
			}
		}

		template<typename... FormatArgTypes>
		struct FormatElementsWithArgs {
			template<typename OutputIter, typename FormatArgsStorageTuple, Size... Indices>
			constexpr static void format(
				OutputIter outputIter, const value_type& tuple,
				FormatArgsStorageTuple& formatArgsStorageTuple, IndexSequence<Indices...>) noexcept {

				(formatWithArgs(ValueIdentityV<Indices>{}, outputIter, formatArgsStorageTuple, tuple.template get<Indices>()) && ...);
			}
		};

		template<Size Index, typename OutputIter, typename ArgType, typename... FormatArgTypes>
		constexpr static void formatElementsImpl(ValueIdentityV<Index>, OutputIter& outputIter,
			ArgType&& arg, FormatArgTypes&&... formatArgs) noexcept {

			if constexpr (Index > 0) {
				outputIter = ',';
				outputIter = ' ';
			}

			outputIter = Formatter<Decay<ArgType>>::format(
				outputIter, natl::forward<ArgType>(arg), natl::forward<FormatArgTypes>(formatArgs)...);
		}

		template<typename OutputIter, Size... Indices>
		constexpr static void formatElements(
			OutputIter outputIter,
			const value_type& tuple,
			IndexSequence<Indices...>) noexcept {

			(formatElementsImpl(ValueIdentityV<Indices>{}, outputIter, tuple.template get<Indices>()), ...);
		}

	public:
		template<typename OutputIter, typename... FormatArgTypes>
			requires(impl::IsFormatElementArg<FormatArgTypes> && ...)
		constexpr static OutputIter format(OutputIter outputIter, const value_type& tuple, FormatArgTypes&&... formatArgs) noexcept {

			outputIter = '{';

			if constexpr (sizeof...(FormatArgTypes) == 0) {
				formatElements(
					outputIter,
					tuple,
					MakeIndexSequence<tupleArgSize>{});
			} else {
				using format_args_storage_tuple = Tuple<FormatArgTypes&&...>;
				format_args_storage_tuple formatArgsStorageTuple(natl::forward<FormatArgTypes>(formatArgs)...);
				FormatElementsWithArgs<FormatArgTypes...>::format(
					outputIter,
					tuple,
					formatArgsStorageTuple,
					MakeIndexSequence<tupleArgSize>{});
			}

			outputIter = '}';

			return outputIter;
		}

	};
	//format(formatArgText<"1: standard", "2: fish", "t", FormatElement<1, >>(tuple, formatElement()));

	template<typename CharType>
	struct Formatter<TypeInfo, CharType> {
		using value_type = TypeInfo;
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const TypeInfo typeInfo = TypeInfo{}) noexcept {
			outputIter = '<';
			outputIter = 'h';
			outputIter = ':';
			outputIter = ' ';
			outputIter = Formatter<natl::Size, CharType>::format<OutputIter>(outputIter, typeInfo.hashCode());
			outputIter = ',';
			outputIter = ' ';
			outputIter = 'n';
			outputIter = ':';
			outputIter = ' ';
			outputIter = Formatter<ConstAsciiStringView, CharType>::format<OutputIter>(outputIter, typeInfo.name());
			outputIter = '>';
			return outputIter;
		}
	};

	template<typename... Types, typename CharType>
	struct Formatter<TypePack<Types...>, CharType> {
		using value_type = TypePack<Types...>;

		template<typename OutputIter, typename Type, Size Index>
		constexpr static void formatType(OutputIter& outputIter) noexcept {
			if constexpr (Index > 0) {
				outputIter = ',';
				outputIter = ' ';
			}
			outputIter = Formatter<TypeInfo, CharType>::format<OutputIter>(outputIter, getTypeInfo<Type>());
		}

		template<typename OutputIter, Size... Indices>
		constexpr static void formatTypes(OutputIter& outputIter, IndexSequence<Indices...>) noexcept {
			outputIter = '{';
			(formatType<OutputIter, Types, Indices>(outputIter), ...);
			outputIter = '}';
		}

		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, value_type = value_type{}) noexcept {
			formatTypes(outputIter, MakeIndexSequence<value_type::size>{});
			return outputIter;
		}
	};
}