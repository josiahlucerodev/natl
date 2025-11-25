#pragma once 

//own 
#include "../util/stringLiteral.h"
#include "../util/strongType.h"
#include "../container/array.h"
#include "../processing/format.h"

//@export
namespace natl {
	Bool enablePrintExtendedColor() noexcept;
	
	enum struct PrintColor : Size {
		black,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		white,
		standard,
	};

	enum struct PrintForegroundColor : Size {
		black = static_cast<Size>(PrintColor::black),
		red = static_cast<Size>(PrintColor::red),
		green = static_cast<Size>(PrintColor::green),
		yellow = static_cast<Size>(PrintColor::yellow),
		blue = static_cast<Size>(PrintColor::blue),
		magenta = static_cast<Size>(PrintColor::magenta),
		cyan = static_cast<Size>(PrintColor::cyan),
		white = static_cast<Size>(PrintColor::white),
		standard = static_cast<Size>(PrintColor::standard),
	};

	enum struct PrintBackgroundColor : Size {
		black = static_cast<Size>(PrintColor::black),
		red = static_cast<Size>(PrintColor::red),
		green = static_cast<Size>(PrintColor::green),
		yellow = static_cast<Size>(PrintColor::yellow),
		blue = static_cast<Size>(PrintColor::blue),
		magenta = static_cast<Size>(PrintColor::magenta),
		cyan = static_cast<Size>(PrintColor::cyan),
		white = static_cast<Size>(PrintColor::white),
		standard = static_cast<Size>(PrintColor::standard),
	};

	struct PrintExtendedColor {
		natl::ui8 r;
		natl::ui8 g;
		natl::ui8 b;
		constexpr PrintExtendedColor() noexcept
			: r(0), g(0), b(0) {};
		constexpr PrintExtendedColor(
			const natl::ui8 rIn,
			const natl::ui8 gIn,
			const natl::ui8 bIn) noexcept :
			r(rIn), g(gIn), b(bIn) {};
	};

	struct PrintExtendedForegroundColor : public PrintExtendedColor {
	public:
		using PrintExtendedColor::PrintExtendedColor;
	};
	struct PrintExtendedBackgroundColor : public PrintExtendedColor {
	public:
		using PrintExtendedColor::PrintExtendedColor;
	};

	struct PrintDefaultColor {};
	struct PrintDefaultForegroundColor {};
	struct PrintDefaultBackgroundColor {};
	struct PrintAllDefaultColor {};

	struct PrintColorString {
	public:
		Size stringSize;
		Array<Ascii, 20> stringData;
		constexpr Size size() const noexcept { return stringSize; }
		constexpr Size length() const noexcept { return size(); }
		constexpr const Ascii* data() const noexcept { return stringData.data(); }
		constexpr const Ascii* c_str() const noexcept { return data(); }
	};

	enum struct PrintColorAttribute {
		foreground,
		background,
		standard = foreground
	};

	constexpr PrintColorString printColorToString(
		const PrintColor printColor,
		const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {

		PrintColorString string{};
		string.stringSize = 5;

		string.stringData[0] = '\x1b';
		string.stringData[1] = '[';

		if (colorAttribute == PrintColorAttribute::foreground) {
			string.stringData[2] = '3';
		} else {
			string.stringData[2] = '4';
		}

		switch (printColor) {
		case PrintColor::black:
			string.stringData[3] = '0';
			break;
		case PrintColor::red:
			string.stringData[3] = '1';
			break;
		case PrintColor::green:
			string.stringData[3] = '2';
			break;
		case PrintColor::yellow:
			string.stringData[3] = '3';
			break;
		case PrintColor::blue:
			string.stringData[3] = '4';
			break;
		case PrintColor::magenta:
			string.stringData[3] = '5';
			break;
		case PrintColor::cyan:
			string.stringData[3] = '6';
			break;
		case PrintColor::white:
			string.stringData[3] = '7';
			break;
		case PrintColor::standard:
			string.stringData[3] = '9';
			break;
		default:
			unreachable();
		}

		string.stringData[4] = 'm';
		string.stringData[5] = '\0';

		return string;
	}

	constexpr PrintColor convertToPrintColor(const PrintForegroundColor printColor) noexcept {
		return fromUnderlying<PrintColor>(toUnderlying<PrintForegroundColor>(printColor));
	}
	constexpr PrintColor convertToPrintColor(const PrintBackgroundColor printColor) noexcept {
		return fromUnderlying<PrintColor>(toUnderlying<PrintBackgroundColor>(printColor));
	}

	constexpr PrintColorString printColorToString(const PrintForegroundColor printColor) noexcept {
		return printColorToString(convertToPrintColor(printColor), PrintColorAttribute::foreground);
	}
	constexpr PrintColorString printColorToString(const PrintBackgroundColor printColor) noexcept {
		return printColorToString(convertToPrintColor(printColor), PrintColorAttribute::background);
	}

	namespace impl {
		template<Bool AddEndSeimColon>
		constexpr void printColorChannelToString(Size& index, PrintColorString& string, const natl::ui8 value) noexcept {
			if (value == 0) {
				if constexpr (AddEndSeimColon) {
					string.stringData[index + 0] = ';';
					index += 1;
				}
			} else if (value > 99) {
				string.stringData[index + 0] = '0' + static_cast<Ascii>(value / 100);
				string.stringData[index + 1] = '0' + static_cast<Ascii>((value / 10) % 10);
				string.stringData[index + 2] = '0' + static_cast<Ascii>(value % 10);
				if constexpr (AddEndSeimColon) {
					string.stringData[index + 3] = ';';
					index += 4;
				} else {
					index += 3;
				}
			} else if (value > 9) {
				string.stringData[index + 0] = '0' + static_cast<Ascii>(value / 10);
				string.stringData[index + 1] = '0' + static_cast<Ascii>(value % 10);
				if constexpr (AddEndSeimColon) {
					string.stringData[index + 2] = ';';
					index += 3;
				} else {
					index += 2;
				}
			} else {
				string.stringData[index + 0] = '0' + static_cast<Ascii>(value % 10);

				if constexpr (AddEndSeimColon) {
					string.stringData[index + 1] = ';';
					index += 2;
				} else {
					index += 1;
				}
			}
		};
	}

	constexpr PrintColorString printColorToString(
		const PrintExtendedColor printColor, 
		const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		Size index{};
		PrintColorString string{};

		string.stringData[0] = '\x1b';
		string.stringData[1] = '[';
		if (colorAttribute == PrintColorAttribute::foreground) {
			string.stringData[2] = '3';
			string.stringData[3] = '8';
		} else {
			string.stringData[2] = '4';
			string.stringData[3] = '8';
		}
		string.stringData[4] = ';';
		string.stringData[5] = '2';
		string.stringData[6] = ';';
		index = 7;

		impl::printColorChannelToString<true>(index, string, printColor.r);
		impl::printColorChannelToString<true>(index, string, printColor.g);
		impl::printColorChannelToString<false>(index, string, printColor.b);

		string.stringData[index + 0] = 'm';
		string.stringData[index + 1] = '\0';

		string.stringSize = index + 2;
		return string;
	}

	constexpr PrintColorString printColorToString(const PrintExtendedForegroundColor printColor) noexcept {
		return printColorToString(static_cast<PrintExtendedColor>(printColor), PrintColorAttribute::foreground);
	}
	constexpr PrintColorString printColorToString(const PrintExtendedBackgroundColor printColor) noexcept {
		return printColorToString(static_cast<PrintExtendedColor>(printColor), PrintColorAttribute::background);
	}
	constexpr PrintColorString printColorToString(
		PrintDefaultColor, 
		const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		PrintColorString string{};
		string.stringSize = 5;
		string.stringData[0] = '\x1b';
		string.stringData[1] = '[';

		if (colorAttribute == PrintColorAttribute::foreground) {
			string.stringData[2] = '3';
			string.stringData[3] = '9';
		} else {
			string.stringData[2] = '4';
			string.stringData[3] = '9';
		}

		string.stringData[4] = 'm';
		string.stringData[5] = '\0';
		return string;
	}
	constexpr PrintColorString printColorToString(PrintDefaultForegroundColor) noexcept {
		return printColorToString(PrintDefaultColor{}, PrintColorAttribute::foreground);
	}
	constexpr PrintColorString printColorToString(PrintDefaultBackgroundColor) noexcept {
		return printColorToString(PrintDefaultColor{}, PrintColorAttribute::background);
	}

	constexpr PrintColorString printColorToString(PrintAllDefaultColor) noexcept {
		PrintColorString string{};
		string.stringSize = 4;
		string.stringData[0] = '\x1b';
		string.stringData[1] = '[';
		string.stringData[2] = '0';
		string.stringData[3] = 'm';
		string.stringData[4] = '\0';
		return string;
	}

	namespace impl {
		inline PrintColorString processPrintColorString(const PrintColorString& colorString) noexcept {
			if constexpr(getPlatformType() == ProgramPlatformType::unixPlatform || 
				getPlatformType() == ProgramPlatformType::windowsPlatform) {
				return colorString;
			} else {
				PrintColorString emptyColorString;
				emptyColorString.stringSize = 0;
				emptyColorString.stringData[0] = '\0';
				return emptyColorString;
			} 
		}

		inline Bool printColorString(const PrintColorString& colorString) noexcept {
			PrintColorString outputColorString = processPrintColorString(colorString); 
			return print(outputColorString.data(), outputColorString.size());
		}
	}

	inline Bool setPrintColor(const PrintColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		PrintColorString colorString = printColorToString(printColor, colorAttribute);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintBackgroundColor printColor) noexcept {
		PrintColorString colorString = printColorToString(printColor);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintForegroundColor printColor) noexcept {
		PrintColorString colorString = printColorToString(printColor);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintExtendedColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		PrintColorString colorString = printColorToString(printColor, colorAttribute);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintExtendedForegroundColor printColor) noexcept {
		PrintColorString colorString = printColorToString(printColor);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintExtendedBackgroundColor printColor) noexcept {
		PrintColorString colorString = printColorToString(printColor);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintDefaultColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		PrintColorString colorString = printColorToString(printColor, colorAttribute);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintDefaultForegroundColor printColor) noexcept {
		PrintColorString colorString = printColorToString(printColor);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintDefaultBackgroundColor printColor) noexcept {
		PrintColorString colorString = printColorToString(printColor);
		return impl::printColorString(colorString);
	}
	inline Bool setPrintColor(const PrintAllDefaultColor printColor) noexcept {
		PrintColorString colorString = printColorToString(printColor);
		return impl::printColorString(colorString);
	}

	inline Bool printc(const Ascii* string, const Size size, const PrintColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		setPrintColor(printColor, colorAttribute);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintBackgroundColor printColor) noexcept {
		setPrintColor(printColor);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintForegroundColor printColor) noexcept {
		setPrintColor(printColor);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintExtendedColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		setPrintColor(printColor, colorAttribute);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintExtendedForegroundColor printColor) noexcept {
		setPrintColor(printColor);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintExtendedBackgroundColor printColor) noexcept {
		setPrintColor(printColor);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintDefaultColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		setPrintColor(printColor, colorAttribute);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintDefaultForegroundColor printColor) noexcept {
		setPrintColor(printColor);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintDefaultBackgroundColor printColor) noexcept {
		setPrintColor(printColor);
		const Bool result = print(string, size);
		setPrintColor(PrintAllDefaultColor{});
		return result;
	}
	inline Bool printc(const Ascii* string, const Size size, const PrintAllDefaultColor printColor) noexcept {
		setPrintColor(printColor);
		const Bool result = print(string, size);
		return result;
	}

	inline Bool printc(const Ascii* string, const PrintColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printc(string, cstringLength(string), printColor, colorAttribute);
	}
	inline Bool printc(const Ascii* string, const PrintBackgroundColor printColor) noexcept {
		return printc(string, cstringLength(string), printColor);
	}
	inline Bool printc(const Ascii* string, const PrintForegroundColor printColor) noexcept {
		return printc(string, cstringLength(string), printColor);
	}
	inline Bool printc(const Ascii* string, const PrintExtendedColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printc(string, cstringLength(string), printColor, colorAttribute);
	}
	inline Bool printc(const Ascii* string, const PrintExtendedForegroundColor printColor) noexcept {
		return printc(string, cstringLength(string), printColor);
	}
	inline Bool printc(const Ascii* string, const PrintExtendedBackgroundColor printColor) noexcept {
		return printc(string, cstringLength(string), printColor);
	}
	inline Bool printc(const Ascii* string, const PrintDefaultColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printc(string, cstringLength(string), printColor, colorAttribute);
	}
	inline Bool printc(const Ascii* string, const PrintDefaultForegroundColor printColor) noexcept {
		return printc(string, cstringLength(string), printColor);
	}
	inline Bool printc(const Ascii* string, const PrintDefaultBackgroundColor printColor) noexcept {
		return printc(string, cstringLength(string), printColor);
	}
	inline Bool printc(const Ascii* string, const PrintAllDefaultColor printColor) noexcept {
		return printc(string, cstringLength(string), printColor);
	}

	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor, colorAttribute);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintBackgroundColor printColor) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintForegroundColor printColor) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintExtendedColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor, colorAttribute);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintExtendedForegroundColor printColor) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintExtendedBackgroundColor printColor) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintDefaultColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor, colorAttribute);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintDefaultForegroundColor printColor) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintDefaultBackgroundColor printColor) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printc(const StringViewLike stringViewLike, const PrintAllDefaultColor printColor) noexcept {
		return printc(stringViewLike.data(), stringViewLike.size(), printColor);
	}

	inline Bool printlnc(const Ascii* string, const Size size, const PrintColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		const Bool result = printc(string, size, printColor, colorAttribute);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintBackgroundColor printColor) noexcept {
		const Bool result = printc(string, size, printColor);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintForegroundColor printColor) noexcept {
		const Bool result = printc(string, size, printColor);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintExtendedColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		const Bool result = printc(string, size, printColor, colorAttribute);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintExtendedForegroundColor printColor) noexcept {
		const Bool result = printc(string, size, printColor);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintExtendedBackgroundColor printColor) noexcept {
		const Bool result = printc(string, size, printColor);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintDefaultColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		const Bool result = printc(string, size, printColor, colorAttribute);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintDefaultForegroundColor printColor) noexcept {
		const Bool result = printc(string, size, printColor);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintDefaultBackgroundColor printColor) noexcept {
		const Bool result = printc(string, size, printColor);
		print("\n");
		return result;
	}
	inline Bool printlnc(const Ascii* string, const Size size, const PrintAllDefaultColor printColor) noexcept {
		const Bool result = printc(string, size, printColor);
		print("\n");
		return result;
	}

	inline Bool printlnc(const Ascii* string, const PrintColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printlnc(string, cstringLength(string), printColor, colorAttribute);
	}
	inline Bool printlnc(const Ascii* string, const PrintBackgroundColor printColor) noexcept {
		return printlnc(string, cstringLength(string), printColor);
	}
	inline Bool printlnc(const Ascii* string, const PrintForegroundColor printColor) noexcept {
		return printlnc(string, cstringLength(string), printColor);
	}
	inline Bool printlnc(const Ascii* string, const PrintExtendedColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printlnc(string, cstringLength(string), printColor, colorAttribute);
	}
	inline Bool printlnc(const Ascii* string, const PrintExtendedForegroundColor printColor) noexcept {
		return printlnc(string, cstringLength(string), printColor);
	}
	inline Bool printlnc(const Ascii* string, const PrintExtendedBackgroundColor printColor) noexcept {
		return printlnc(string, cstringLength(string), printColor);
	}
	inline Bool printlnc(const Ascii* string, const PrintDefaultColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printlnc(string, cstringLength(string), printColor, colorAttribute);
	}
	inline Bool printlnc(const Ascii* string, const PrintDefaultForegroundColor printColor) noexcept {
		return printlnc(string, cstringLength(string), printColor);
	}
	inline Bool printlnc(const Ascii* string, const PrintDefaultBackgroundColor printColor) noexcept {
		return printlnc(string, cstringLength(string), printColor);
	}
	inline Bool printlnc(const Ascii* string, const PrintAllDefaultColor printColor) noexcept {
		return printlnc(string, cstringLength(string), printColor);
	}

	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor, colorAttribute);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintBackgroundColor printColor) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintForegroundColor printColor) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintExtendedColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor, colorAttribute);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintExtendedForegroundColor printColor) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintExtendedBackgroundColor printColor) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintDefaultColor printColor, const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor, colorAttribute);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintDefaultForegroundColor printColor) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintDefaultBackgroundColor printColor) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor);
	}
	template<typename StringViewLike>
		requires(IsStringViewLike<StringViewLike, Ascii>)
	inline Bool printlnc(const StringViewLike stringViewLike, const PrintAllDefaultColor printColor) noexcept {
		return printlnc(stringViewLike.data(), stringViewLike.size(), printColor);
	}

	namespace impl {
		template<auto PrintColorValue>
		struct PrintAsStringLiteralImpl {
			constexpr static PrintColorString printColorString = printColorToString(PrintColorValue);
			constexpr static auto templateStringLiteral = TemplateStringLiteral(printColorString.stringData.dataStorage);
			using type = StringLiteral<templateStringLiteral>;
		};
	}

	template<PrintColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};
	template<PrintBackgroundColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};
	template<PrintForegroundColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};

	template<PrintExtendedColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};
	template<PrintExtendedForegroundColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};
	template<PrintExtendedBackgroundColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};

	template<PrintDefaultColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};
	template<PrintDefaultForegroundColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};
	template<PrintDefaultBackgroundColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};
	template<PrintAllDefaultColor PrintColorValue>
	struct AsStringLiteralT<PrintColorValue> : impl::PrintAsStringLiteralImpl<PrintColorValue> {};

	template<>
	struct Formatter<PrintColorString, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const PrintColorString colorString) noexcept {
			const PrintColorString outputColorString = impl::processPrintColorString(colorString);
			const ConstAsciiStringView colorStringView(outputColorString.c_str(), outputColorString.length());
			for (const Ascii character : colorStringView) {
				outputIter = character;
			}
			return outputIter;
		}
	};

	struct FormatForegroundFlag {};
	struct FormatBackgroundFlag {};

	namespace impl {
		struct PrintColorAttributeTemplateFlag {
		public:
			template<typename TemplateFlag>
			constexpr static void handelTemplateFlag(PrintColorAttribute& colorAttribute) noexcept {
				if constexpr (IsStringLiteral<TemplateFlag>) {
					constexpr ConstAsciiStringView tflagName = TemplateFlag::toStringView();
					if constexpr (tflagName == "b") {
						colorAttribute = PrintColorAttribute::background;
					} else if constexpr (tflagName == "f") {
						colorAttribute = PrintColorAttribute::foreground;
					} else if constexpr (tflagName == "background") {
						colorAttribute = PrintColorAttribute::background;
					} else if constexpr (tflagName == "foreground") {
						colorAttribute = PrintColorAttribute::foreground;
					} else {
						unreachable();
					}
				} else {
					if constexpr (IsSame<TemplateFlag, FormatBackgroundFlag>) {
						colorAttribute = PrintColorAttribute::background;
					} else if constexpr (IsSame<TemplateFlag, FormatForegroundFlag>) {
						colorAttribute = PrintColorAttribute::foreground;
					} else {
						unreachable();
					}
				}
			}
		};
	}

	template<>
	struct Formatter<PrintColor, Ascii> {
		template<typename OutputIter>
		constexpr static void universalFormat(
			OutputIter& outputIter,
			const PrintColor printColor,
			const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
			const PrintColorString colorString = printColorToString(printColor, colorAttribute);
			outputIter = Formatter<PrintColorString, Ascii>::format<OutputIter>(outputIter, colorString);
		}

		template<typename... TemplateFlags>
		struct WithTemplateFlags : impl::PrintColorAttributeTemplateFlag {
		public:
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const PrintColor printColor) noexcept {
				PrintColorAttribute colorAttribute;
				(handelTemplateFlag<TemplateFlags>(colorAttribute), ...);
				universalFormat<OutputIter>(outputIter, printColor, colorAttribute);
				return outputIter;
			}
		};

		template<typename OutputIter>
		constexpr static OutputIter format(
			OutputIter outputIter,
			const PrintColor printColor,
			const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
			universalFormat<OutputIter>(outputIter, printColor, colorAttribute);
			return outputIter;
		}
	};

	template<>
	struct Formatter<PrintForegroundColor, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const PrintForegroundColor printColor) noexcept {
			return Formatter<PrintColor, Ascii>::format<OutputIter>(
				outputIter,
				static_cast<PrintColor>(printColor),
				PrintColorAttribute::foreground);
		}
	};

	template<>
	struct Formatter<PrintBackgroundColor, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const PrintBackgroundColor printColor) noexcept {
			return Formatter<PrintColor, Ascii>::format<OutputIter>(
				outputIter,
				static_cast<PrintColor>(printColor),
				PrintColorAttribute::background);
		}
	};

	template<>
	struct Formatter<PrintExtendedColor, Ascii> {
		template<typename OutputIter>
		constexpr static void universalFormat(
			OutputIter& outputIter,
			const PrintExtendedColor printColor,
			const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
			const PrintColorString colorString = printColorToString(printColor, colorAttribute);
			outputIter = Formatter<PrintColorString, Ascii>::format<OutputIter>(outputIter, colorString);
		}

		template<typename... TemplateFlags>
		struct WithTemplateFlags : impl::PrintColorAttributeTemplateFlag {
		public:
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, const PrintExtendedColor printColor) noexcept {
				PrintColorAttribute colorAttribute;
				(handelTemplateFlag<TemplateFlags>(colorAttribute), ...);
				universalFormat<OutputIter>(outputIter, printColor, colorAttribute);
				return outputIter;
			}
		};

		template<typename OutputIter>
		constexpr static OutputIter format(
			OutputIter outputIter,
			const PrintExtendedColor printColor,
			const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
			universalFormat<OutputIter>(outputIter, printColor, colorAttribute);
			return outputIter;
		}
	};

	template<>
	struct Formatter<PrintExtendedForegroundColor, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const PrintExtendedForegroundColor printColor) noexcept {
			return Formatter<PrintExtendedColor, Ascii>::format<OutputIter>(
				outputIter, 
				static_cast<PrintExtendedColor>(printColor),
				PrintColorAttribute::foreground);
		}
	};

	template<>
	struct Formatter<PrintExtendedBackgroundColor, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const PrintExtendedBackgroundColor printColor) noexcept {
			return Formatter<PrintExtendedColor, Ascii>::format<OutputIter>(
				outputIter, 
				static_cast<PrintExtendedColor>(printColor), 
				PrintColorAttribute::background);
		}
	};


	template<>
	struct Formatter<PrintDefaultColor, Ascii> {
		template<typename OutputIter>
		constexpr static void universalFormat(
			OutputIter& outputIter,
			const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
			const PrintColorString colorString = printColorToString(PrintDefaultColor{}, colorAttribute);
			outputIter = Formatter<PrintColorString, Ascii>::format<OutputIter>(outputIter, colorString);
		}

		template<typename... TemplateFlags>
		struct WithTemplateFlags : impl::PrintColorAttributeTemplateFlag {
		public:
			template<typename OutputIter>
			constexpr static OutputIter format(OutputIter outputIter, PrintDefaultColor) noexcept {
				PrintColorAttribute colorAttribute;
				(handelTemplateFlag<TemplateFlags>(colorAttribute), ...);
				universalFormat<OutputIter>(outputIter, colorAttribute);
				return outputIter;
			}
		};

		template<typename OutputIter>
		constexpr static OutputIter format(
			OutputIter outputIter, 
			PrintDefaultColor,
			const PrintColorAttribute colorAttribute = PrintColorAttribute::standard) noexcept {
			universalFormat<OutputIter>(outputIter, colorAttribute);
			return outputIter;
		}
	};

	template<>
	struct Formatter<PrintDefaultForegroundColor, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, PrintDefaultForegroundColor) noexcept {
			return Formatter<PrintDefaultColor, Ascii>::format<OutputIter>(
				outputIter,
				PrintDefaultColor{},
				PrintColorAttribute::foreground);
		}
	};

	template<>
	struct Formatter<PrintDefaultBackgroundColor, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, PrintDefaultBackgroundColor) noexcept {
			return Formatter<PrintDefaultColor, Ascii>::format<OutputIter>(
				outputIter,
				PrintDefaultColor{},
				PrintColorAttribute::background);
		}
	};

	template<>
	struct Formatter<PrintAllDefaultColor, Ascii> {
		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, PrintAllDefaultColor) noexcept {
			const PrintColorString colorString = printColorToString(PrintAllDefaultColor{});
			return Formatter<PrintColorString, Ascii>::format<OutputIter>(outputIter, colorString);
		}
	};
}