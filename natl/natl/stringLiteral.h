#pragma once 

//own
#include "basicTypes.h"
#include "dataMovement.h"
#include "stringView.h"

//interface 
namespace natl {
	template <Size StringSize>
	struct TemplateStringLiteral {
		Ascii stringStorage[StringSize];
		constexpr static Size stringSize = StringSize;
		constexpr TemplateStringLiteral(const Ascii(&str)[StringSize]) { uninitializedCopyCountNoOverlap<const Ascii*>(str, stringStorage, StringSize); }
		template<typename StringPtrType>
			requires(IsSame<StringPtrType, const Ascii*>)
		constexpr TemplateStringLiteral(StringPtrType str) { uninitializedCopyCountNoOverlap<const Ascii*>(str, stringStorage, StringSize); }
		constexpr const Ascii* c_str() const noexcept { return stringStorage; }
		constexpr Size size() const noexcept { return StringSize; }
	};

	template <Size Number>
		requires (Number > 0)
	constexpr TemplateStringLiteral<Number> makeTStringLiteral(const Ascii(&str)[Number]) noexcept {
		return TemplateStringLiteral<Number>(str);
	}

	template <TemplateStringLiteral StringL>
	struct StringLiteral {
		using size_type = Size;
		static constexpr size_type npos = size_type(-1);
	public:
		constexpr static Ascii stringStorage[StringL.stringSize] = StringL.stringStorage;
		constexpr static const Ascii* string = &StringL.stringStorage[0];
		constexpr static size_type stringSize = StringL.stringSize;
		constexpr static ConstAsciiStringView stringView = string;
	public:
		constexpr static size_type size() noexcept { return stringSize; }
		constexpr static size_type length() noexcept { return size(); }
		constexpr static const Ascii* c_str() noexcept { return string; }
		constexpr static const Ascii* data() noexcept { return string; }
		constexpr static ConstAsciiStringView toStringView() noexcept { return stringView; }

		template<size_type Pos = 0, size_type Count = npos, size_type UsedCount = natl::min<size_type>(size() - Pos, Count)>
		using Substr = StringLiteral<TemplateStringLiteral<UsedCount>(c_str() + Pos)>;
	};

	template<typename Type>
	struct IsStringLiteral : FalseType {};
	template<TemplateStringLiteral TStringL>
	struct IsStringLiteral<StringLiteral<TStringL>> : TrueType {};

	template<typename Type>
	constexpr inline Bool IsStringLiteralV = IsStringLiteral<Type>::value;

	template<auto> struct AsStringLiteralT {};
	template<auto Value> using AsStringLiteral = AsStringLiteralT<Value>::type;
}