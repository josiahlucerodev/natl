#pragma once 

//own
#include "basicTypes.h"
#include "stringLiteral.h"
#include "typeTraits.h"
#include "typePack.h"
#include "tuple.h"
#include "stringConvert.h"
#include "characterTest.h"

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
		using ArgType = decltype(arg);
		using ArgFlagsStorageTuple = Tuple<decltype(flagArgs)...>;
		using FormatArgFlagsType = FormatArgFlags<
			ArgType,
			TypePack<FlagTypes...>,
			TypePack<decltype(flagArgs)...>,
			ArgFlagsStorageTuple>;
		return FormatArgFlagsType(natl::forward<ArgType>(arg), natl::forward<ArgFlagsStorageTuple>(ArgFlagsStorageTuple(flagArgs...)));
	}

	template<TemplateStringLiteral textFlag>
	constexpr auto fromatTextFlag() noexcept { return StringLiteral<textFlag>{}; }

	template<TemplateStringLiteral... textFlags>
	constexpr auto formatArgText(auto arg) noexcept {
		using ArgType = decltype(arg);
		return FormatArgFlags<ArgType, TypePack<StringLiteral<textFlags>...>>(natl::forward<ArgType>(arg));
	}

	template<typename Type>
	struct IsFormatArgFlags : FalseType {};
	template<typename ArgType, typename... FlagTypes>
	struct IsFormatArgFlags<FormatArgFlags<ArgType, FlagTypes...>> : TrueType {};

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
			if constexpr (IsFormatArgFlags<ArgType>::value) {
				using FormatArgType = ArgType;
				using formatter_type = CreateFormatterWithTemplateFlags<typename FormatArgType::arg_type, CharType, typename FormatArgType::template_flags>;
				using arg_flags_storage_tuple = FormatArgType::arg_flags_storage_tuple;
				using arg_type = FormatArgType::arg_type;
				formatToArgCallFormat<OutputIter, arg_type, formatter_type, arg_flags_storage_tuple, FormatArgType::arg_flags::size>(
						outputIter, natl::forward<arg_type>(arg.getArg()), natl::forward<arg_flags_storage_tuple>(arg.getArgFlags()));
			} else {
				using arg_flags_storage_tuple = Tuple<>;
				using formatter_type = Formatter<RemoveReferenceT<ArgType>, CharType>;
				formatToArgCallFormat<OutputIter, ArgType, formatter_type, arg_flags_storage_tuple, 0>(
					outputIter, natl::forward<ArgType>(arg), natl::forward<arg_flags_storage_tuple>(arg_flags_storage_tuple()));
			}
		}
	}

	template<typename OutputIter, typename... ArgTypes>
	constexpr OutputIter formatTo(OutputIter outputIter, ArgTypes&&... args) noexcept {
		using format_output_iter = FormatOutputIter<OutputIter>;
		format_output_iter formatOutputIter = format_output_iter(outputIter);
		(impl::formatToArgLevel<format_output_iter, ArgTypes, Ascii>(formatOutputIter, natl::forward<ArgTypes>(args)), ...);
		return formatOutputIter.getOutputIter();
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

	template<Size StringSize>
	struct Formatter<const Ascii[StringSize], Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const Ascii* str) noexcept {
			for (Size i = 0; i < StringSize; i++) {
				outputIter = str[i];
			}
			return outputIter;
		}
	};

	enum class FormatIntFlag {
		decimal,
		hexadecimal,
		binary,
		defaultFormat = decimal,
	};

	struct FormatIntHexadecimalFlag {};
	struct FormatIntBinaryFlag {};
	struct FormatIntDecimalFlag {};

	using FormatIntHexFlag = FormatIntHexadecimalFlag;
	using FormatIntBinFlag = FormatIntBinaryFlag;
	using FormatIntDecFlag = FormatIntDecimalFlag;

	namespace impl {
		template<typename InteagerType, typename CharType>
		class IntegerFormatter {
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
			constexpr static void formatWithFlag(OutputIter& outputIter, const value_type number, const FormatIntFlag flag) noexcept {
				switch (flag) {
				case FormatIntFlag::hexadecimal:
					formatHexadecimal<OutputIter>(outputIter, number);
				case FormatIntFlag::binary:
					formatBinary<OutputIter>(outputIter, number);
				case FormatIntFlag::decimal:
					formatDecimal<OutputIter>(outputIter, number);
				default:
					unreachable();
				}
			}

			template<typename... TemplateFlags>
			class WithTemplateFlags {
			public:
				constexpr static Size reserveAmount = 32;

				template<typename OutputIter, typename TemplateFlag, typename... RestTemplateFlags>
				constexpr static void handelFlags(OutputIter& outputIter, const value_type number) noexcept {
					if constexpr (IsStringLiteralV<TemplateFlag>) {
						constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();

						if constexpr (tflagName == "hexadecimal" || tflagName == "hex") {
							formatHexadecimal<OutputIter>(outputIter, number);
						} else if constexpr (tflagName == "binary" || tflagName == "bin") {
							formatBinary<OutputIter>(outputIter, number);
						} else if constexpr (tflagName == "decimal" || tflagName == "dec") {
							formatDecimal<OutputIter>(outputIter, number);
						} else {
							unreachable();
						}
					}

					if constexpr (sizeof...(RestTemplateFlags) != 0) {
						handelFlags<OutputIter, RestTemplateFlags...>(outputIter, number);
					}
				}

			public:
				template<typename OutputIter>
				constexpr static OutputIter format(OutputIter outputIter, const value_type number) noexcept {
					handelFlags<OutputIter, TemplateFlags...>(outputIter, number);
					return outputIter;
				}
			};

		public:
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const value_type number, const FormatIntFlag flag = FormatIntFlag::defaultFormat) noexcept {
				return formatWithFlag<OutputIter>(outputIter, number, flag);
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
}