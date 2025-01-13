#pragma once 

//own
#include "typeTraits.h"
#include "typePack.h"
#include "stringLiteral.h"
#include "characterTest.h"
#include "../fundamental/option.h"
#include "../container/array.h"
#include "../container/string.h"
#include "../processing/format.h"

//interface 
namespace natl {
	template<typename EnumType>
		requires(IsEnumC<EnumType>)
	consteval ConstAsciiStringView getNameOfEnum() noexcept {
#if defined(NATL_COMPILER_EMSCRIPTEN) || defined(NATL_COMPILER_GCC)
		return getTypeInfo<EnumType>().name();
#elif defined(NATL_COMPILER_MSVC)
		return getTypeInfo<EnumType>().name().substr(5);
#else 
		static_assert("natl: getNameOfEnum() not implemented");
#endif
	}

	template<auto Value>
		requires(IsEnumC<decltype(Value)>)
	consteval auto getRawEnumValueName() noexcept {
#if defined(NATL_COMPILER_EMSCRIPTEN)
		//prefix: "auto natl::getRawEnumValueName() [Value = "
		//suffix: "}"
	return ConstAsciiStringView(__PRETTY_FUNCTION__).substr(42).removeSuffix(1);
#elif defined(NATL_COMPILER_GCC)
		//prefix: "consteval auto natl::getRawEnumValueName() [with auto Value = "
		//suffix: "]"
		return ConstAsciiStringView(__PRETTY_FUNCTION__).substr(62).removeSuffix(1);
#elif defined(NATL_COMPILER_MSVC)
		//prefix: "auto __cdecl natl::getRawEnumValueName<"
		//suffix: ">(void) noexcept"
		return ConstAsciiStringView(__FUNCSIG__).substr(39).removeSuffix(16);
#else 
	static_assert("natl: getRawEnumValueName() not implemented");
#endif
	}

	template<auto Value>
		requires(IsEnumC<decltype(Value)>)
	consteval ConstAsciiStringView getNameOfEnumValue() noexcept {
#if defined(NATL_COMPILER_EMSCRIPTEN) || defined(NATL_COMPILER_GCC) || defined(NATL_COMPILER_MSVC)
		constexpr ConstAsciiStringView enumTypeName = getNameOfEnum<Decay<decltype(Value)>>();
		constexpr ConstAsciiStringView enumValueName = getRawEnumValueName<Value>();
		if (enumValueName.size() > enumTypeName.size() + 1 && enumValueName[enumTypeName.size() + 1] == ':') {
			return enumValueName.substr(enumTypeName.size() + 2);
		}
		return enumValueName;
#else 
	static_assert("natl: getNameOfEnumValue() not implemented");
#endif
	}

	template<typename EnumType, natl::UnderlyingType<EnumType> Value>
		requires(IsEnumC<EnumType>)
	consteval Bool testIfValidEnumValue() noexcept {
#if defined(NATL_COMPILER_EMSCRIPTEN) || defined(NATL_COMPILER_GCC) || defined(NATL_COMPILER_MSVC)
		constexpr ConstAsciiStringView enumValueName = getRawEnumValueName<static_cast<EnumType>(Value)>();
		if (enumValueName.isEmpty() || enumValueName[0] == '(') {
			return false;
		}
		return true;
#else 
		static_assert("natl: testIfValidEnumValue() not implemented");
#endif
	}

	enum struct EnumValueFlag {
		unknown,
		sequential,
		bitFlags,
		custom,
	};

	template<TemplateStringLiteral Name, typename EnumType, EnumType Value>
		requires(IsEnumC<EnumType>)
	struct EnumValueInfoType {
		using enum_type = EnumType;
		using underlying_type = UnderlyingType<EnumType>;
		constexpr static TemplateStringLiteral name = Name;
		constexpr static EnumType value = Value;
	};

	template<TemplateStringLiteral Name, typename EnumType, EnumType Value>
	struct Formatter<EnumValueInfoType<Name, EnumType, Value>> {
		using value_type = EnumValueInfoType<Name, EnumType, Value>;
		using formattable_type_info = void;

		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, value_type = value_type{}) noexcept {
			rawFormat<OutputIter>(outputIter, Name.toStringView());
			rawFormat<OutputIter>(outputIter, '(');
			rawFormat<OutputIter>(outputIter, toUnderlying<EnumType>(Value));
			rawFormat<OutputIter>(outputIter, ')');
			return outputIter;
		}
	};

	template<typename EnumType>
		requires(IsEnumC<EnumType>)
	struct EnumValueInfo {
		using enum_type = EnumType;
		using underlying_type = UnderlyingType<EnumType>;
		ConstAsciiStringView name;
		EnumType value;
	};

	template<typename EnumType>
	struct Formatter<EnumValueInfo<EnumType>> {
		using value_type = EnumValueInfo<EnumType>;

		template<typename OutputIter>
		constexpr static OutputIter format(OutputIter outputIter, const value_type& value) noexcept {
			rawFormat<OutputIter>(outputIter, value.name);
			rawFormat<OutputIter>(outputIter, '(');
			rawFormat<OutputIter>(outputIter, value.value);
			rawFormat<OutputIter>(outputIter, ')');
			return outputIter;
		}
	};

	template<typename EnumType>
	struct StandardEnumTraitInfo {
		using underlying_type = UnderlyingType<EnumType>;
		underlying_type startValue;
		EnumValueFlag enumValueFlag;
	};

	template<typename EnumType>
	consteval StandardEnumTraitInfo<EnumType> getStandardEnumTraitInfo() noexcept {
		if constexpr (!testIfValidEnumValue<EnumType, 0>()) {
			if constexpr (!testIfValidEnumValue<EnumType, 1>()) {
				return StandardEnumTraitInfo<EnumType>{0, EnumValueFlag::unknown};
			} else if constexpr (!testIfValidEnumValue<EnumType, 2>()) {
				return StandardEnumTraitInfo<EnumType>{1, EnumValueFlag::sequential};
			} else if constexpr (!testIfValidEnumValue<EnumType, 3>()) {
				if constexpr (!testIfValidEnumValue<EnumType, 4>()) {
					return StandardEnumTraitInfo<EnumType>{1, EnumValueFlag::sequential};
				} else {
					return StandardEnumTraitInfo<EnumType>{1, EnumValueFlag::bitFlags};
				}
			} else {
				return StandardEnumTraitInfo<EnumType>{1, EnumValueFlag::sequential};
			}
		}

		if constexpr (!testIfValidEnumValue<EnumType, 1>()) {
			return StandardEnumTraitInfo<EnumType>{0, EnumValueFlag::sequential};
		} else if constexpr (!testIfValidEnumValue<EnumType, 2>()) {
			return StandardEnumTraitInfo<EnumType>{0, EnumValueFlag::sequential};
		} else if constexpr (!testIfValidEnumValue<EnumType, 3>()) {
			if constexpr (!testIfValidEnumValue<EnumType, 4>()) {
				return StandardEnumTraitInfo<EnumType>{0, EnumValueFlag::sequential};
			} else {
				return StandardEnumTraitInfo<EnumType>{0, EnumValueFlag::bitFlags};
			}
		} else {
			return StandardEnumTraitInfo<EnumType>{0, EnumValueFlag::sequential};
		}
	}

	//traits
	template<typename EnumType>
		requires(IsEnumC<EnumType>)
	struct EnumTraits {
		constexpr inline static StandardEnumTraitInfo<EnumType> standardEnumTraitInfo = getStandardEnumTraitInfo<EnumType>();
		using enum_type = EnumType;
		using underlying_type = UnderlyingType<EnumType>;
		constexpr inline static underlying_type startValue = standardEnumTraitInfo.startValue;
		constexpr inline static EnumValueFlag enumValueFlag = standardEnumTraitInfo.enumValueFlag;
		using unique_values = TypePack<>;
	};

	template<typename EnumType>
	concept IsSequentialEnumC = IsEnumC<EnumType> && EnumTraits<EnumType>::enumValueFlag == EnumValueFlag::sequential;
	template<typename EnumType> constexpr inline Bool IsSequentialEnum = IsSequentialEnumC<EnumType>;
	template<typename EnumType> struct IsSequentialEnumV : BoolConstant<IsSequentialEnumC<EnumType>> {};

	template<typename EnumType>
	concept IsBitFlagsEnumC = IsEnumC<EnumType> && EnumTraits<EnumType>::enumValueFlag == EnumValueFlag::bitFlags;
	template<typename EnumType> constexpr inline Bool IsBitFlagsEnum = IsBitFlagsEnumC<EnumType>;
	template<typename EnumType> struct IsBitFlagsEnumV : BoolConstant<IsBitFlagsEnumC<EnumType>> {};

	template<typename EnumType>
	concept EnumTraitHasAllValuesC = IsEnumC<EnumType> && requires() {
		typename EnumTraits<EnumType>::all_values;
	};
	template<typename EnumType> constexpr inline Bool EnumTraitHasAllValues = EnumTraitHasAllValuesC<EnumType>;
	template<typename EnumType> struct EnumTraitHasAllValuesV : BoolConstant<EnumTraitHasAllValuesC<EnumType>> {};

	//sequential enum value in range
	namespace impl {
		template<typename EnumType, EnumType Start, EnumType End>
			requires(IsEnumC<EnumType> && (Start < End))
		consteval Size getNumberOfSequentialEnumInRange() noexcept {
			return[]<Size... Indices> (IndexSequence<Indices...>) -> Size {
				return (static_cast<Size>(testIfValidEnumValue<EnumType, toUnderlying<EnumType>(Start) + Indices>()) + ...);
			}(MakeIndexSequence<(toUnderlying<EnumType>(End) - toUnderlying<EnumType>(Start)) + 1>{});
		}

		template<typename EnumType, EnumType Value>
			requires(IsEnumC<EnumType>)
		consteval auto getEnumValueOfElementInRange() noexcept {
			return EnumValueInfoType<
				NATL_MAKE_TEMPLATE_STRING_LITERAL_FROM_STRING_VIEW(getNameOfEnumValue<Value>()), 
				EnumType, Value>{};
		}

		template<typename EnumType, EnumType Start, EnumType End>
			requires(IsEnumC<EnumType> && (Start < End))
		consteval auto getSequentialEnumsInRange() noexcept {
			return[]<Size... Indices>(IndexSequence<Indices...>) {
				constexpr auto startUnderlying = toUnderlying<EnumType>(Start);
				using all_elements = TypePack<
					decltype(getEnumValueOfElementInRange<EnumType, fromUnderlying<EnumType>(startUnderlying + Indices)>())...>;
				return TypePackRemoveElementsIf<IsVoidV, all_elements>{};
			}(MakeIndexSequence<(toUnderlying<EnumType>(End) - toUnderlying<EnumType>(Start)) + 1>());
		}
	}

	template<typename EnumType, EnumType Start, EnumType End>
		requires(IsEnumC<EnumType> && (Start < End))
	using SequentialEnumValuesInRange = decltype(impl::getSequentialEnumsInRange<EnumType, Start, End>());

	//bit flag enum value in range
	namespace impl {

#ifdef NATL_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4293)
#endif // NATL_COMPILER_MSVC

		//how it works, who knows 
		template<typename EnumType, EnumType Start, Size Index>
		constexpr UnderlyingType<EnumType> generateBitFlagEnumTestValue =
			Index == 0 
				? (toUnderlying<EnumType>(Start))
				: (toUnderlying<EnumType>(Start) == 0 
					? (1 << (Index - 1))
					: (toUnderlying<EnumType>(Start) << Index)
				);

#ifdef NATL_COMPILER_MSVC
#pragma warning(pop)
#endif // NATL_COMPILER_MSVC

		template<typename EnumType, EnumType Start, EnumType End>
			requires(IsEnumC<EnumType> && (Start < End)
			&& (isPowerOf2(toUnderlying<EnumType>(Start)) || toUnderlying<EnumType>(Start) == 0)
			&& (isPowerOf2(toUnderlying<EnumType>(End)) || toUnderlying<EnumType>(End) == 0))
			consteval Size getNumberOfBitFlagEnumInRange() noexcept {
			using underlying_type = UnderlyingType<EnumType>;
			constexpr underlying_type startIndex = bitscanForward(toUnderlying<EnumType>(Start));
			constexpr underlying_type endIndex = bitscanForward(toUnderlying<EnumType>(End));
			return[]<Size... Indices> (IndexSequence<Indices...>) -> Size {
				return (static_cast<Size>(testIfValidEnumValue<EnumType, generateBitFlagEnumTestValue<EnumType, Start, Indices>>()) + ...);
			}(MakeIndexSequence<(endIndex - startIndex) + 1>{});
		}

		template<typename EnumType, UnderlyingType<EnumType> Base, Size StartIndex, Size EndIndex>
			requires(IsEnumC<EnumType> && (StartIndex < EndIndex))
		consteval auto getBitFlagEnumsInIndexRange() noexcept {
			return[]<Size... Indices>(IndexSequence<Indices...>) {
				using all_elements = TypePack<
					decltype(
						getEnumValueOfElementInRange<
						EnumType,
						fromUnderlying<EnumType>(generateBitFlagEnumTestValue<EnumType, fromUnderlying<EnumType>(Base), Indices + StartIndex>)
						>()
					)...
				>;
				return TypePackRemoveElementsIf<IsVoidV, all_elements>{};
			}(MakeIndexSequence<(EndIndex - StartIndex) + 1>());
		}

		template<typename EnumType, EnumType Start, EnumType End>
			requires(IsEnumC<EnumType> && (Start < End))
		consteval auto getBitFlagEnumsInRange() noexcept {
			using underlying_type = UnderlyingType<EnumType>;
			constexpr underlying_type startIndex = bitscanForward(toUnderlying<EnumType>(Start));
			constexpr underlying_type endIndex = bitscanForward(toUnderlying<EnumType>(End));
			return[]<Size... Indices>(IndexSequence<Indices...>) {
				using all_elements = TypePack<
					decltype(
						getEnumValueOfElementInRange<
							EnumType,
							fromUnderlying<EnumType>(generateBitFlagEnumTestValue<EnumType, Start, Indices>)
						>()
					)...
				>;
				return TypePackRemoveElementsIf<IsVoidV, all_elements>{};
			}(MakeIndexSequence<(endIndex - startIndex) + 1>());
		}
	}

	template<typename EnumType, EnumType Start, EnumType End>
		requires(IsEnumC<EnumType> && (Start < End)
			&& (isPowerOf2(toUnderlying<EnumType>(Start)) || toUnderlying<EnumType>(Start) == 0)
			&& (isPowerOf2(toUnderlying<EnumType>(End)) || toUnderlying<EnumType>(End) == 0))
	using BitFlagEnumValuesInRange = decltype(impl::getBitFlagEnumsInRange<EnumType, Start, End>());

	template<typename EnumType, UnderlyingType<EnumType> Base, Size StartIndex, Size EndIndex>
		requires(IsEnumC<EnumType> && (Base == 0 || Base == 1) && (StartIndex < EndIndex))
	using BitFlagEnumValuesInIndexRange = decltype(impl::getBitFlagEnumsInIndexRange<EnumType, Base, StartIndex, EndIndex>());


	//default enum evaluation 
	namespace impl {
		template<typename EnumType, Size Count>
		consteval Size countDefaultSequentialEnumValues() noexcept {
			if constexpr (testIfValidEnumValue<EnumType, Count>()) {
				return countDefaultSequentialEnumValues<EnumType, Count + 1>();
			}
			return Count;
		}

		template<typename EnumType, Size Count>
		consteval Size countDefaultBitFlagEnumValues() noexcept {
			constexpr Size index = Count;
			constexpr UnderlyingType<EnumType> value = generateBitFlagEnumTestValue<EnumType, 
				fromUnderlying<EnumType>(EnumTraits<EnumType>::startValue), index>;

			if constexpr (testIfValidEnumValue<EnumType, value>()) {
				return countDefaultBitFlagEnumValues<EnumType, Count + 1>();
			}
			return Count;
		}
	}

	template<typename EnumType>
		requires(IsEnumC<EnumType> && !EnumTraitHasAllValuesC<EnumType>
	&& (IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType>))
		consteval Size getDefaultNumberOfEnumValue() noexcept {
		if constexpr (IsSequentialEnumC<EnumType>) {
			return impl::countDefaultSequentialEnumValues<EnumType, EnumTraits<EnumType>::startValue>();
		} else {
			return impl::countDefaultBitFlagEnumValues<EnumType, EnumTraits<EnumType>::startValue>();
		}
	}

	template<typename EnumType>
		requires(IsEnumC<EnumType> &&
	((IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType>)
		|| EnumTraitHasAllValuesC<EnumType>))
		consteval Size getNumberOfEnumValues() noexcept {
		if constexpr (EnumTraitHasAllValuesC<EnumType>) {
			return EnumTraits<EnumType>::all_values::size;
		} else {
			return getDefaultNumberOfEnumValue<EnumType>() + EnumTraits<EnumType>::unique_values::size;
		}
	}

	namespace impl {
		template<typename EnumType, Size Index, Size Value = EnumTraits<EnumType>::startValue + Index>
		using DefaultSequentialEnumValue = EnumValueInfoType<
			NATL_MAKE_TEMPLATE_STRING_LITERAL_FROM_STRING_VIEW(getNameOfEnumValue<static_cast<EnumType>(Value)>()),
			EnumType, fromUnderlying<EnumType>(Value)>;

		template<typename EnumType, Size Index, UnderlyingType<EnumType> Value =
			generateBitFlagEnumTestValue<EnumType, fromUnderlying<EnumType>(EnumTraits<EnumType>::startValue), Index>>
		using DefaultBitFlagEnumValue = EnumValueInfoType<
			NATL_MAKE_TEMPLATE_STRING_LITERAL_FROM_STRING_VIEW(getNameOfEnumValue<static_cast<EnumType>(Value)>()),
			EnumType, fromUnderlying<EnumType>(Value)>;

		template<typename EnumType>
		consteval auto getDefaultEnumValues() noexcept {
			constexpr Size numOfEnumValues = getDefaultNumberOfEnumValue<EnumType>();
			return[]<Size... Indices>(IndexSequence<Indices...>) {
				if constexpr (IsSequentialEnumC<EnumType>) {
					return TypePack<impl::DefaultSequentialEnumValue<EnumType, Indices>...>{};
				} else {
					return TypePack<impl::DefaultBitFlagEnumValue<EnumType, Indices>...>{};
				}
			}(MakeIndexSequence<numOfEnumValues>());
		}

	}

	template<typename EnumType>
		requires(IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType>)
	using DefaultEnumValues = decltype(impl::getDefaultEnumValues<EnumType>());

	//get enum valus
	namespace impl {
		template<typename EnumType>
			requires((IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType>) || EnumTraitHasAllValuesC<EnumType>)
		consteval auto getEnumValues() noexcept {
			if constexpr (EnumTraitHasAllValuesC<EnumType>) {
				return typename EnumTraits<EnumType>::all_values{};
			} else {
				return TypePackMerge<DefaultEnumValues<EnumType>, typename EnumTraits<EnumType>::unique_values>{};
			}
		}
	}

	template<typename EnumType>
		requires(IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType> || EnumTraitHasAllValuesC<EnumType>)
	using EnumValues = decltype(impl::getEnumValues<EnumType>());

	template<typename EnumType>
	concept HasEnumValuesC =
		(IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType> || EnumTraitHasAllValuesC<EnumType>)
		&& requires() { typename EnumValues<EnumType>; };
	template<typename EnumType> constexpr inline Bool HasEnumValues = HasEnumValuesC<EnumType>;
	template<typename EnumType> struct HasEnumValuesV : BoolConstant<HasEnumValuesC<EnumType>> {};

	//enum value as array
	namespace impl {
		template<typename EnumType, typename EnumInfo>
		constexpr EnumValueInfo<EnumType> getEnumValuesAsArrayElement() noexcept {
			EnumValueInfo<EnumType> enumInfo;
			enumInfo.name = getNameOfEnumValue<static_cast<EnumType>(EnumInfo::value)>();
			enumInfo.value = EnumInfo::value;
			return enumInfo;
		}
	}

	template<typename EnumType>
		requires(IsEnumC<EnumType>&& HasEnumValuesC<EnumType>
	&& (IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType>))
		consteval natl::Array<EnumValueInfo<EnumType>, getNumberOfEnumValues<EnumType>()>
		getEnumValuesAsArray() noexcept {
		using enum_info_types = EnumValues<EnumType>;

		return[]<Size... Indices>(IndexSequence<Indices...>) {
			natl::Array<EnumValueInfo<EnumType>, getNumberOfEnumValues<EnumType>()> result = {
				impl::getEnumValuesAsArrayElement<EnumType, typename enum_info_types::template at<Indices>>()...
			};
			return result;
		}(MakeIndexSequence<enum_info_types::size>{});
	}

	//enum operations 
	template<typename EnumType>
		requires(IsEnumC<EnumType>&& HasEnumValuesC<EnumType>
		&& (IsSequentialEnumC<EnumType> || IsBitFlagsEnumC<EnumType>))
	struct EnumOperations {
		template<typename DynStringType>
		constexpr static natl::Bool toStringToDst(DynStringType& dst, const EnumType& value) noexcept {

			constexpr auto arrayOfEnumValues = getEnumValuesAsArray<EnumType>();
			if constexpr (IsSequentialEnumC<EnumType>) {
				for (Size i = 0; i < arrayOfEnumValues.size(); i++) {
					if (arrayOfEnumValues[i].value == value) {
						dst = arrayOfEnumValues[i].name;
						return true;
					}
				}
				return false;
			} else {
				constexpr natl::ConstAsciiStringView seperator = " | ";
				for (Size i = 0; i < arrayOfEnumValues.size(); i++) {
					if (toUnderlying<EnumType>(arrayOfEnumValues[i].value) == 0) {
						if (toUnderlying<EnumType>(value) == 0) {
							dst += arrayOfEnumValues[i].name;
							return true;
						}
					}
				}

				Bool foundValue = false;
				for (Size i = 0; i < arrayOfEnumValues.size(); i++) {
					if (toUnderlying<EnumType>(value) & toUnderlying<EnumType>(arrayOfEnumValues[i].value)) {
						if (foundValue) {
							dst += seperator;
						}
						dst += arrayOfEnumValues[i].name;
						foundValue = true;
					} 
				}

				return foundValue;
			}
		}

		template<typename DynStringType>
		constexpr static natl::Option<EnumType> fromString(const DynStringType& src) noexcept {
			constexpr auto arrayOfEnumValues = getEnumValuesAsArray<EnumType>();
			if constexpr (IsSequentialEnumC<EnumType>) {
				for (Size i = 0; i < arrayOfEnumValues.size(); i++) {
					if (arrayOfEnumValues[i].name == src) {
						return arrayOfEnumValues[i].value;
					}
				}
				return OptionEmpty{};
			} else {
				auto srcSubstr = [&](Size start, Size size) {
					return ConstAsciiStringView(src.data() + start, size);
				};
				Bool valueFound = false;
				UnderlyingType<EnumType> value = 0;

				Size start = 0;
				Size end = 0;

				constexpr natl::ConstAsciiStringView seperator = "|";
				auto skipSpaces = [&]() {
					Bool foundSpace = false;
					while ((end <= src.size()) && isSpace(*(src.data() + end))) {
						end++;
						foundSpace = true;
					}

					if (foundSpace) {
						start = end;
						end++;
					}
					return foundSpace;
				};

				skipSpaces();
				while (end <= src.size()) {
					for (Size i = 0; i < arrayOfEnumValues.size(); i++) {
						if (arrayOfEnumValues[i].name == srcSubstr(start, end - start)) {
							value |= toUnderlying<EnumType>(arrayOfEnumValues[i].value);
							valueFound = true;

							start = end;
							break;
						}
					}

					skipSpaces();

					if (((end + seperator.size()) <= src.size())) {
						if (srcSubstr(end, seperator.size()) == seperator) {
							end += seperator.size();
							start = end;
						}
					}

					if (!skipSpaces()) {
						end++;
					}
				}

				if (valueFound == false) {
					return OptionEmpty{};
				}

				return fromUnderlying<EnumType>(value);
			}
		}
	};

	template<typename EnumType> 
	concept HasEnumOperationsC = requires() {
		typename EnumOperations<EnumType>;
	};
	template<typename EnumType> constexpr inline Bool HasEnumOperations = HasEnumOperationsC<EnumType>;
	template<typename EnumType> struct HasEnumOperationsV : BoolConstant<HasEnumOperationsC<EnumType>> {};

	template<typename EnumType, typename DynStringType = natl::String256>
		requires(HasEnumOperationsC<EnumType>)
	constexpr DynStringType enumToStringToDst(DynStringType& dst, const EnumType& value) noexcept {
		return EnumOperations<EnumType>::template toString<DynStringType>(dst, value);
	}

	template<typename EnumType, typename DynStringType = natl::String256>
		requires(HasEnumOperationsC<EnumType>)
	constexpr DynStringType enumToString(const EnumType& value) noexcept {
		DynStringType result;
		EnumOperations<EnumType>::template toStringToDst<DynStringType>(result, value);
		return result;
	}

	template<typename EnumType, typename DynStringType = ConstAsciiStringView>
		requires(HasEnumOperationsC<EnumType>)
	constexpr static natl::Option<EnumType> enumFromString(const DynStringType& src) {
		return EnumOperations<EnumType>::template fromString<DynStringType>(src);
	}

	template<typename EnumType>
		requires(IsEnumC<EnumType>)
	constexpr natl::Bool bitFlagTest(const EnumType& value) noexcept {
		using underlying_type = natl::UnderlyingType<EnumType>;
		return static_cast<underlying_type>(value) != underlying_type(0);
	}

	template<typename EnumType, typename... EnumArgs>
		requires(IsEnumC<EnumType> && (IsSameC<EnumType, Decay<EnumArgs>> && ...))
	constexpr natl::Bool bitFlagTestAnd(const EnumType& value, const EnumArgs&... tests) noexcept {
		using underlying_type = natl::UnderlyingType<EnumType>;
		return (static_cast<underlying_type>(value) & (static_cast<underlying_type>(tests) | ...)) != underlying_type(0);
	}
}

#define NATL_ADD_BIT_FLAG_OPERATIONS(EnumName) \
constexpr inline EnumName operator|(const EnumName lhs, const EnumName rhs) noexcept { \
	using underlying_type = natl::UnderlyingType<EnumName>; \
	return static_cast<EnumName>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs)); \
} \
constexpr inline EnumName operator&(const EnumName lhs, const EnumName rhs) noexcept { \
	using underlying_type = natl::UnderlyingType<EnumName>; \
	return static_cast<EnumName>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs)); \
} 


