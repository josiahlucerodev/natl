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
		constexpr const Ascii* c_str() const noexcept { return stringStorage; }
		constexpr Size size() const noexcept { return StringSize; }
	};

	template <Size Number>
		requires (Number > 0)
	constexpr TemplateStringLiteral<Number> makeStringLiteral(const Ascii(&str)[Number]) noexcept {
		return TemplateStringLiteral<Number>(str);
	}

	template <TemplateStringLiteral StringL>
	struct StringLiteral {
	private:
		static constexpr const Ascii* string = &StringL.stringStorage[0];
		static constexpr Size stringSize = StringL.stringSize;
		static constexpr ConstAsciiStringView stringView = string;
	public:
		static constexpr Size size() noexcept { return stringSize; }
		static constexpr Size length() noexcept { return size(); }
		static constexpr const Ascii* c_str() noexcept { return string; }
		static constexpr const Ascii* data() noexcept { return string; }
		static constexpr ConstAsciiStringView toStringView() noexcept { return stringView; }
	};

	template<typename Type>
	struct IsStringLiteral : FalseType {};
	template<TemplateStringLiteral TStringL>
	struct IsStringLiteral<StringLiteral<TStringL>> : TrueType {};

	template<typename Type>
	constexpr inline Bool IsStringLiteralV = IsStringLiteral<Type>::value;

}