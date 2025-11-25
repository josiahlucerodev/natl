#pragma once 

//own
#include "basicTypes.h"
#include "dataMovement.h"
#include "../container/stringView.h"

//@export
namespace natl {
	template <Size StringSize>
	struct TemplateStringLiteral {
		Ascii stringStorage[StringSize];
		constexpr static Size stringSize = StringSize;
		constexpr TemplateStringLiteral() noexcept = default;
		constexpr ~TemplateStringLiteral() noexcept = default;
		constexpr TemplateStringLiteral(const Ascii(&str)[StringSize]) noexcept { 
			uninitializedCopyCountNoOverlap<const Ascii*>(str, stringStorage, StringSize); 
		}
		template<typename StringPtrType>
			requires(IsSame<StringPtrType, const Ascii*>)
		constexpr TemplateStringLiteral(StringPtrType str) noexcept { 
			uninitializedCopyCountNoOverlap<const Ascii*>(str, stringStorage, StringSize);
			//TODO
			//addEndNullTerminator();
		}
		constexpr TemplateStringLiteral(const ConstAsciiStringView& str) noexcept { 
			uninitializedCopyCountNoOverlap<const Ascii*>(str.data(), stringStorage, str.size());
			addEndNullTerminator();
		}

		constexpr const Ascii* c_str() const noexcept { return stringStorage; }
		constexpr const Ascii* data() const noexcept { return stringStorage; }
		constexpr Ascii* data() noexcept { return stringStorage; }
		constexpr Size size() const noexcept { return StringSize - 1; }
		constexpr operator AsciiStringView() noexcept {
			return AsciiStringView(data(), size());
		}
		constexpr operator ConstAsciiStringView() const noexcept {
			return ConstAsciiStringView(data(), size());
		}
		constexpr AsciiStringView toStringView() noexcept {
			return AsciiStringView(data(), size());
		}
		constexpr ConstAsciiStringView toStringView() const noexcept {
			return ConstAsciiStringView(data(), size());
		}

	private:
		constexpr void addEndNullTerminator() {
			stringStorage[StringSize - 1] = '\n';
		}
	};

	namespace impl {
		template<typename TemplateStringLiteralType, TemplateStringLiteral TemplatStringLiteralArg>
		constexpr void concatTemplateStringLiteralArg(TemplateStringLiteralType& storage, natl::Size& index) noexcept {
			for (natl::Size i = 0; i < TemplatStringLiteralArg.size() - 1; i++, index++) {
				storage.stringStorage[index] = TemplatStringLiteralArg.c_str()[i];
			}
		}
	}
	
	template<TemplateStringLiteral... TemplatStringLiterals>
	constexpr TemplateStringLiteral<(TemplatStringLiterals.size() + ...) - sizeof...(TemplatStringLiterals) + 1 > concatTemplateStringLiterals() noexcept {
		using concat_string_literal_type = TemplateStringLiteral<(TemplatStringLiterals.size() + ...) - sizeof...(TemplatStringLiterals) + 1>;
		natl::Size index = 0;
		concat_string_literal_type stringLiteral{};
		(impl::concatTemplateStringLiteralArg<concat_string_literal_type, TemplatStringLiterals>(stringLiteral, index), ...);
		return stringLiteral;
	}

	template <TemplateStringLiteral StringL>
	struct StringLiteral {
		using size_type = Size;
		static constexpr size_type npos = size_type(-1);
		constexpr static TemplateStringLiteral value = StringL;
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

		
		template<TemplateStringLiteral... StringLiteralsRhs>
		using ConcatV = StringLiteral<concatTemplateStringLiterals<StringL, StringLiteralsRhs...>()>;
		template<TemplateStringLiteral... StringLiteralsRhs>
		constexpr static TemplateStringLiteral Concat = ConcatV<StringLiteralsRhs...>::value;
	};

	template<typename Type> struct IsStringLiteralV : FalseType {};
	template<TemplateStringLiteral TStringL> struct IsStringLiteralV<StringLiteral<TStringL>> : TrueType {};
	template<typename Type> constexpr inline Bool IsStringLiteral = IsStringLiteralV<Type>::value;
	template<typename Type> concept IsStringLiteralC = IsStringLiteralV<Type>::value;

	template<auto> struct AsStringLiteralT {};
	template<auto Value> using AsStringLiteral = AsStringLiteralT<Value>::type;

	template<ConstAsciiStringView Name>
	consteval inline TemplateStringLiteral<Name.size() + 1> makeTemplateStringLiteral() noexcept {
		return TemplateStringLiteral<Name.size() + 1>(Name);
	};
}
