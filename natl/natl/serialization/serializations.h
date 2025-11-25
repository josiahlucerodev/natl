#pragma once 

//own
#include "base.h"
#include "compatibility.h"
#include "../container/variant.h"
#include "../container/string.h"
#include "../container/flatHashMap.h"
#include "../container/dynArray.h"
#include "../container/smallDynArray.h"

//@export
namespace natl {
	template<typename CharType, Size bufferSize, typename Alloc,
		Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
		requires(IsAllocatorC<Alloc>)
	struct Serialize<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>> {
		using as_type = SerializeStr;
		using type = BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>;
		template<typename Serializer> using error_type = StandardSerializeError<Serializer>;
		constexpr static ConstAsciiStringView sourceName = "natl::Serialize<natl::BaseString<...>>::write";

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeStrC<Serializer>)
		[[nodiscard]] constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const type& str) noexcept {
			auto error = serializer.template writeStr<Flags, CustomFlags, SerializeComponentType>(str.toStringView());
			if (error.hasValue()) { return error.value().addSource(sourceName); }
			return natl::OptionEmpty{};
		}
	};


	template<typename CharType, Size bufferSize, typename Alloc,
		Bool EnableDynAllocation, Bool EnableIncreasedSmallBufferSize>
		requires(IsAllocatorC<Alloc>)
	struct Deserialize<BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>> {
		using as_type = SerializeStr;
		using type = BaseString<CharType, bufferSize, Alloc, EnableDynAllocation, EnableIncreasedSmallBufferSize>;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<natl::BaseString<...>>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags,
			CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>&& CanDeserializeStrC<Deserializer>)
		[[nodiscard]] constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				type& dst) noexcept {
			auto readError = deserializer.template readStr<Flags, CustomFlags, SerializeComponentType, type>(info, dst);
			if (readError.hasValue()) {
				return readError.value().addSource(sourceName, "");
			}
			return {};
		}
	};

	template<typename DataType> struct Serialize<Option<DataType>> {
		using as_type = SerializeOptional<DataType>;
		using type = Option<DataType>;
		template<typename Serializer> using error_type = StandardSerializeError<Serializer>;
		constexpr static ConstAsciiStringView sourceName = "natl::Serialize<natl::Option<...>>::write";

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType,
			typename... DeserializerArgs>
			requires(CanSerializeStrC<Serializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanDeserializeOptionalC<Serializer>&& CanSerializeC<Serializer, DataType>)
		[[nodiscard]] constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const type& value, DeserializerArgs&&... deserializerArgs) noexcept {
			if (!value.hasValue()) {
				auto error = serializer.template writeEmptyOptional<Flags, CustomFlags, SerializeComponentType>();
				if (error.hasValue()) { return error.value().addSource(sourceName); }
			} else {
				using optional_component = SerializeOptionalComponent<type>;

				auto beginError = serializer.template beginWriteOptional<Flags, CustomFlags, SerializeComponentType>();
				if (beginError.hasValue()) { return beginError.value().addSource(sourceName); }

				auto error = serializeWrite<Serializer, Flags, CustomFlags, optional_component>(
					serializer, value.value(), forward<DeserializerArgs>(deserializerArgs)...);
				if (error.hasValue()) { return error.value().addSource(sourceName); }

				auto endError = serializer.template endWriteOptional<Flags, CustomFlags, SerializeComponentType>();
				if (endError.hasValue()) { return endError.value().addSource(sourceName); }
			}
			return natl::OptionEmpty{};
		}
	};

	template<typename DataType> struct Deserialize<Option<DataType>> {
		using as_type = SerializeChar;
		using type = Option<DataType>;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<natl::Option<...>>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename... DeserializerArgs>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanDeserializeOptionalC<Deserializer>&& CanDeserializeC<Deserializer, DataType>)
		[[nodiscard]] constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				type& dst,
				DeserializerArgs&&... deserializerArgs) noexcept {
			auto isNullExpect = deserializer.template readIsEmptyOptional
				<Flags, CustomFlags, SerializeComponentType>(info);
			if (isNullExpect.hasError()) {
				return isNullExpect.error().addSource(sourceName, "");
			}
			if (isNullExpect.value()) {
				return OptionEmpty{};
			}

			auto optionalInfoExpect = deserializer.template beginReadOptional
				<Flags, CustomFlags, SerializeComponentType>(info);
			if (optionalInfoExpect.hasError()) {
				return optionalInfoExpect.error().addSource(sourceName, "");
			}
			auto valueInfo = optionalInfoExpect.value();

			using optional_component = SerializeOptionalComponent<DataType>;
			auto valueExpect = deserializeRead<Deserializer, Flags, CustomFlags, optional_component, DataType>(
				deserializer, valueInfo, forward<DeserializerArgs>(deserializerArgs)...);
			if (valueExpect.hasError()) {
				return valueExpect.error().addSource(sourceName, "");
			}
			dst = valueExpect.value();

			auto endReadError = deserializer.template endReadOptional
				<Flags, CustomFlags, SerializeComponentType>(info);
			if (endReadError.hasError()) {
				return endReadError.value().addSource(sourceName, "");
			}

			return OptionEmpty{};
		}
	};

	template<typename... Elements>
		requires(IsSerializableC<Decay<typename Elements::value_type>> && ...)
	struct Serialize<Variant<Elements...>> {
		using as_type = SerializeVariant<ui64, Decay<typename Elements::value_type>...>;
		using type = Variant<Elements...>;
		template<typename Serializer> using error_type = StandardSerializeError<Serializer>;
		constexpr static ConstAsciiStringView sourceName = "natl::Serialize<natl::Variant<...>>::write";

		template<typename Serializer>
		using VariantSerializeFunction = Option<error_type<Serializer>>(*)(Serializer&, const type&);

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType,
			typename Element, Size Index>
		constexpr static VariantSerializeFunction<Serializer> getSerializeFunction() noexcept {
			return [](Serializer& serializer, const type& value) -> Option<error_type<Serializer>> {
				using variant_member = SerializeVariantComponent<type, typename type::elements::template at<Index>,Index>;

				auto beginError = serializer.template beginWriteVariant<Flags, CustomFlags, SerializeComponentType, as_type, Index>(Element::name);
				if (beginError.hasValue()) { return beginError.value().addSource(sourceName); }
				
				auto error = Serialize<Decay<typename Element::value_type>>::template write<Serializer, Flags, CustomFlags, variant_member>(serializer,
					value.template get<Index>()
				);
				if (error.hasValue()) { return error.value().addSource(sourceName); }
				
				auto endError = serializer.template endWriteVariant<Flags, CustomFlags, SerializeComponentType, as_type>();
				if (endError.hasValue()) { return endError.value().addSource(sourceName); }
				return natl::OptionEmpty{};
			};
		}

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanSerializeVariantC<Serializer> && (CanSerializeC<Serializer, typename Elements::value_type> && ...))
		[[nodiscard]] constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const type& value) noexcept {
			return [&] <Size... Indices>(IndexSequence<Indices...>) -> Option<error_type<Serializer>> {
				VariantSerializeFunction<Serializer> serializeFunctions[sizeof...(Elements)] = {
					getSerializeFunction<Serializer, Flags, CustomFlags, SerializeComponentType, Elements, Indices>()...
				};

				if (value.hasValue()) {
					auto valueError = serializeFunctions[value.getIndex() - 1](serializer, value);
					if (valueError.hasValue()) { return valueError; }
				} else {
					auto emptyError = serializer.template writeEmptyVariant<Flags, CustomFlags, SerializeComponentType, as_type>();
					if (emptyError.hasValue()) { return emptyError.value().addSource(sourceName); }
				}

				return natl::OptionEmpty{};
			}(MakeIndexSequence<sizeof...(Elements)>{});
		}
	};

	template<typename... Elements>
		requires(IsDeserializableC<Decay<typename Elements::value_type>> && ...)
	struct Deserialize<Variant<Elements...>> {
		using as_type = SerializeTypeOf<Variant<Elements...>>;
		using type = Variant<Elements...>;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<natl::Variant<...>>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer>
		using VariantDeserializeFunction = Option<typename Deserializer::deserialize_error_handler>(*)
			(Deserializer&, typename Deserializer::template deserialize_info<as_type>&, type&);

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename Element, Size Index>
		constexpr static VariantDeserializeFunction<Deserializer> getDeserializeFunction() noexcept {
			return [](Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& variantInfo,
				type& dst) -> Option<error_type<Deserializer>> {
				using element_type = typename Element::value_type;
				using element_serialize_type = SerializeTypeOf<element_type>;

				auto variantElementExpect = deserializer.template beginReadVariantOfType<
					Flags, CustomFlags, SerializeComponentType, element_type>(variantInfo);
				if (variantElementExpect.hasError()) {
					return variantElementExpect.error();
				}
				auto variantElementInfo = variantElementExpect.value();

				using variant_component = SerializeVariantComponent<type, typename type::elements::template at<Index>, Index>;
				auto expectValue = deserializeReadMatch<element_serialize_type, Deserializer,
					Flags, CustomFlags, variant_component, element_type>(
						deserializer, variantElementInfo);
				if (expectValue.hasError()) {
					return expectValue.error();
				}

				dst.template assign<Index>(move(expectValue.value()));

				return deserializer.template endReadVariant<
					Flags, CustomFlags, SerializeComponentType>(variantInfo);
			};
		}

		template<typename Deserializer, DeserializeReadFlag Flags,
			CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanDeserializeVariantC<Deserializer> && (CanDeserializeC<Deserializer, typename Elements::value_type> && ...))
		[[nodiscard]] constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& variantInfo,
				type& dst) noexcept {
			auto isEmptyExpect = deserializer.template readIsEmptyVariant<Flags, CustomFlags, SerializeComponentType>(variantInfo);
			if (isEmptyExpect.hasError()) {
				return isEmptyExpect.error().addSource(sourceName, "");
			}

			const Bool isEmpty = isEmptyExpect.value();
			if (isEmpty == true) {
				dst = type{};
				return {};
			}

			auto variantIndexExpect = deserializer.template beginReadVariantGetIndex<Flags, CustomFlags, SerializeComponentType>(
				variantInfo,
				type::stringToIndexNotShiftedStatic);
			if (variantIndexExpect.hasError()) {
				return variantIndexExpect.error().addSource(sourceName, "");
			}

			const Size variantIndex = static_cast<Size>(variantIndexExpect.value());
			if (variantIndex > sizeof...(Elements)) {
				String256 errorLocationDetails = "";
				deserializer.getLocationDetail(errorLocationDetails);
				return error_type<Deserializer>("variant index out of range", errorLocationDetails, DeserializeErrorLocation::none, DeserializeErrorFlag::none)
					.addSource(sourceName, "");
			}

			auto valueError = [&]<Size... Indices>(IndexSequence<Indices...>) -> Option<error_type<Deserializer>> {
				VariantDeserializeFunction<Deserializer> deserializeFunctions[sizeof...(Elements)] = {
					getDeserializeFunction<Deserializer, Flags, CustomFlags, SerializeComponentType, Elements, Indices>()...
				};

				auto variantOfTypeError = deserializeFunctions[variantIndex](deserializer, variantInfo, dst);
				if (variantOfTypeError.hasValue()) {
					return variantOfTypeError.value().addSource(sourceName, "");
				}

				return {};
			}(MakeIndexSequence<sizeof...(Elements)>{});

			if (valueError.hasValue()) {
				return valueError.value();
			}

			return {};
		}
	};

	template<template<typename> typename DynamicArrayType, typename KeyType, typename ValueType, typename Hash, typename Compare>
		requires(IsSerializableC<Decay<KeyType>>&& IsSerializableC<Decay<ValueType>>)
	struct Serialize<impl::BaseFlatHashMap<DynamicArrayType, KeyType, ValueType, Hash, Compare>> {
		using as_type = SerializeDic<KeyType, ValueType>;
		using type = impl::BaseFlatHashMap<DynamicArrayType, KeyType, ValueType, Hash, Compare>;
		template<typename Serializer> using error_type = StandardSerializeError<Serializer>;
		constexpr static ConstAsciiStringView sourceName = "natl::Serialize<natl::BaseFlatHashMap<...>>::write";

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType,
			typename... KeySerializerArgs, typename... ValueSerializerArgs>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanSerializeDicC<Serializer>&& CanSerializeC<Serializer, KeyType>&& CanSerializeC<Serializer, ValueType>)
		[[nodiscard]] constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const type& dic,
				const FuncArgs<KeySerializerArgs...>& keySerializerArgs = {},
				const FuncArgs<ValueSerializerArgs...>& valueSerializerArgs = {}) noexcept {
			if (dic.size() == 0) {
				auto emptyError = serializer.template writeEmptyDic<Flags, CustomFlags, SerializeComponentType>();
				if (emptyError.hasValue()) { return emptyError.value().addSource(sourceName); }
				return natl::OptionEmpty{};
			}

			auto beginError = serializer.template beginWriteDic<Flags, CustomFlags, SerializeComponentType>(dic.size());
			if (beginError.hasValue()) { return beginError.value().addSource(sourceName); }

			for (auto&& [key, value] : dic) {
				auto beginElementError = serializer.template beginWriteDicElement<Flags, CustomFlags, SerializeComponentType>();
				if (beginElementError.hasValue()) { return beginElementError.value().addSource(sourceName); }

				using dic_key_member = SerializeDicKeyComponent<type>;
				auto keyError = serializer.template writeDicKey<Flags, CustomFlags, dic_key_member>();
				if (keyError.hasValue()) { return keyError.value().addSource(sourceName); }
				
				auto keyWriteError = [&] <Size... Indices> (IndexSequence<Indices...>) {
					return serializeWrite<Serializer, Flags, CustomFlags, dic_key_member, KeyType, KeySerializerArgs...>(
						serializer, key, forward<KeySerializerArgs>(keySerializerArgs.template get<Indices>())...);
				}(MakeIndexSequence<sizeof...(KeySerializerArgs)>{});
				if (keyWriteError.hasValue()) { return keyWriteError.value().addSource(sourceName); }

				using dic_value_member = SerializeDicValueComponent<type>;
				auto valueError = serializer.template writeDicValue<Flags, CustomFlags, dic_value_member>();
				if (valueError.hasValue()) { return valueError.value().addSource(sourceName); }
				
				auto valueWriteError = [&] <Size... Indices> (IndexSequence<Indices...>) {
					return serializeWrite<Serializer, Flags, CustomFlags, dic_value_member, KeyType, ValueSerializerArgs...>(
						serializer, value, forward<ValueSerializerArgs>(valueSerializerArgs.template get<Indices>())...);
				}(MakeIndexSequence<sizeof...(ValueSerializerArgs)>{});
				if (valueWriteError.hasValue()) { return valueWriteError.value().addSource(sourceName); }

				auto endElementError = serializer.template endWriteDicElement<Flags, CustomFlags, SerializeComponentType>();
				if (endElementError.hasValue()) { return endElementError.value().addSource(sourceName); }
			}
			
			auto endError = serializer.template endWriteDic<Flags, CustomFlags, SerializeComponentType>();
			if (endError.hasValue()) { return endError.value().addSource(sourceName); }
			return natl::OptionEmpty{};
		}
	};


	template<template<typename> typename DynamicArrayType, typename KeyType, typename ValueType, typename Hash, typename Compare>
		requires(IsSerializableC<Decay<KeyType>>&& IsSerializableC<Decay<ValueType>>)
	struct Deserialize<impl::BaseFlatHashMap<DynamicArrayType, KeyType, ValueType, Hash, Compare>> {
		using type = impl::BaseFlatHashMap<DynamicArrayType, KeyType, ValueType, Hash, Compare>;
		using as_type = SerializeTypeOf<type>;
		constexpr static ConstAsciiStringView sourceName = "natl::Deserialize<natl::BaseFlatHashMap<...>>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags,
			CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType,
			typename... KeyDeserializerArgs, typename... ValueDeserializerArgs>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanDeserializeDicC<Deserializer>&& CanDeserializeC<Deserializer, KeyType>&& CanDeserializeC<Deserializer, ValueType>)
		[[nodiscard]] constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				type& dst,
				const FuncArgs<KeyDeserializerArgs...>& keyDeserializerArgs = {},
				const FuncArgs<ValueDeserializerArgs...>& valueDeserializerArgs = {}) noexcept {

			auto dicSizeExpect = deserializer.template beginReadDic<Flags, CustomFlags, SerializeComponentType>(info);
			if (dicSizeExpect.hasError()) {
				return dicSizeExpect.error().addSource(sourceName, "");
			}
			Size dicSize = dicSizeExpect.value();

			if (dicSize == 0) {
				auto endDicError = deserializer.template endReadEmptyDic<Flags, CustomFlags, SerializeComponentType>(info);
				if (endDicError.hasValue()) {
					return endDicError.value().addSource(sourceName, "");
				}
				return OptionEmpty{};
			}

			dst.reserve(dicSize);
			for (Size i = 0; i < dicSize; i++) {
				auto dicElementExpect = deserializer.template beginReadDicElement<Flags, CustomFlags, SerializeComponentType>(info);
				if (dicElementExpect.hasError()) {
					return dicElementExpect.error().addSource(sourceName, "");
				}
				auto dicElement = dicElementExpect.value();

				auto dicKeyExpect = deserializer.template readDicKey<Flags, CustomFlags, SerializeComponentType>(dicElement);
				if (dicKeyExpect.hasError()) {
					return dicKeyExpect.error().addSource(sourceName, "");
				}
				auto dicKey = dicKeyExpect.value();

				using key_component = SerializeDicKeyComponent<type>;
				auto keyExpect = [&] <Size... Indices> (IndexSequence<Indices...>) {
					return deserializeRead<Deserializer, Flags, CustomFlags, key_component, KeyType>(deserializer, dicKey,
						forward<KeyDeserializerArgs>(keyDeserializerArgs.template get<Indices>())...);
				}(MakeIndexSequence<sizeof...(KeyDeserializerArgs)>{});
				if (keyExpect.hasError()) {
					return keyExpect.error().addSource(sourceName, "");
				}
				auto key = keyExpect.value();

				auto dicValueExpect = deserializer.template readDicValue<Flags, CustomFlags, SerializeComponentType>(dicElement);
				if (dicValueExpect.hasError()) {
					return dicValueExpect.error().addSource(sourceName, "");
				}
				auto dicValue = dicValueExpect.value();

				using value_component = SerializeDicKeyComponent<type>;
				auto valueExpect = [&] <Size... Indices> (IndexSequence<Indices...>) {
					return deserializeRead<Deserializer, Flags, CustomFlags, value_component, KeyType>(deserializer, dicValue,
						forward<ValueDeserializerArgs>(valueDeserializerArgs.template get<Indices>())...);
				}(MakeIndexSequence<sizeof...(ValueDeserializerArgs)>{});
				if (valueExpect.hasError()) {
					return valueExpect.error().addSource(sourceName, "");
				}
				auto value = valueExpect.value();

				auto dicElementEndError = deserializer.template endReadDicElement<Flags, CustomFlags, SerializeComponentType>(dicElement);
				if (dicElementEndError.hasValue()) {
					return dicElementEndError.value().addSource(sourceName, "");
				}

				dst.insert(move(key), move(value));
			}

			auto endDicError = deserializer.template endReadDic<Flags, CustomFlags, SerializeComponentType>(info);
			if (endDicError.hasValue()) {
				return endDicError.value().addSource(sourceName, "");
			}
			return {};
		}
	};

	template<typename DataType, typename Alloc>
		requires(IsSerializableC<Decay<DataType>>)
	struct Serialize<DynArray<DataType, Alloc>> {
		using as_type = SerializeArray<Decay<DataType>>;
		using type = DynArray<DataType, Alloc>;
		template<typename Serializer> using error_type = StandardSerializeError<Serializer>;
		constexpr static ConstAsciiStringView sourceName = "natl::Serialize<natl::DynArray<...>>::write";

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType,
			typename... ElementSerializeArgs>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanSerializeArrayC<Serializer>&& CanSerializeC<Serializer, DataType>)
		[[nodiscard]] constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const type& array, ElementSerializeArgs&&... elementSerializeArgs) noexcept {
			if (array.isEmpty()) {
				auto emptyError = serializer.template writeEmptyArray<Flags, CustomFlags, SerializeComponentType>();
				if (emptyError.hasValue()) { return emptyError.value().addSource(sourceName); }
			} else {
				auto beginError = serializer.template beginWriteArray<Flags, CustomFlags, SerializeComponentType>(array.size());
				if (beginError.hasValue()) { return beginError.value().addSource(sourceName); }
				
				for (Size i = 0; i < array.size(); i++) {
					using array_member = SerializeArrayComponent<type>;

					auto beginElementError = serializer.template beginWriteArrayElement<Flags, CustomFlags, array_member>();
					if (beginElementError.hasValue()) { return beginElementError.value().addSource(sourceName); }

					auto elementWriteError = serializeWrite<Serializer, Flags, CustomFlags, array_member>(
						serializer, array[i], forward<ElementSerializeArgs>(elementSerializeArgs)...);
					if (elementWriteError.hasValue()) { return elementWriteError.value().addSource(sourceName); }

					auto endElementError = serializer.template endWriteArrayElement<Flags, CustomFlags, array_member>();
					if (endElementError.hasValue()) { return endElementError.value().addSource(sourceName); }
				}

				auto endError = serializer.template endWriteArray<Flags, CustomFlags, SerializeComponentType>();
				if (endError.hasValue()) { return endError.value().addSource(sourceName); }
			}
			return natl::OptionEmpty{};
		}
	};

	template<typename DataType, typename Alloc>
		requires(IsSerializableC<Decay<DataType>>)
	struct Deserialize<DynArray<DataType, Alloc>> {
		using deserialize_element_as_type = SerializeTypeOf<Decay<DataType>>;
		using as_type = SerializeArray<Decay<DataType>>;
		using type = DynArray<DataType, Alloc>;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<DynArray<...>>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename... DeserializerArgs>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>
				&& CanDeserializeArrayC<Deserializer>&& CanDeserializeC<Deserializer, DataType>)
		[[nodiscard]] constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				type& dst,
				DeserializerArgs&&... deserializerArgs) noexcept {

			auto arraySizeExpect = deserializer.template beginReadArray<Flags, CustomFlags, SerializeComponentType>(info);
			if (arraySizeExpect.hasError()) {
				return arraySizeExpect.error().addSource(sourceName, "");
			}
			Size arraySize = arraySizeExpect.value();

			if (arraySize == 0) {
				auto endArrayError = deserializer.template endReadEmptyArray<Flags, CustomFlags, SerializeComponentType>(info);
				if (endArrayError.hasValue()) {
					return endArrayError.value().addSource(sourceName, "");
				}
				return {};
			}

			dst.resize(arraySize);
			for (Size index = 0; index < arraySize; index++) {
				auto arrayElementExpect = deserializer.template beginReadArrayElement<Flags, CustomFlags, SerializeComponentType>(info);
				if (arrayElementExpect.hasError()) {
					return arrayElementExpect.error().addSource(sourceName, "");
				}
				auto arrayElement = arrayElementExpect.value();

				using array_component = SerializeArrayComponent<type>;
				auto expectValue = deserializeRead<Deserializer, Flags, CustomFlags, array_component, DataType>(
					deserializer, arrayElement, forward<DeserializerArgs>(deserializerArgs)...);
				if (expectValue.hasError()) {
					dst.resize(index);
					return expectValue.error().addSource(sourceName, "");
				}
				dst[index] = expectValue.value();

				auto arrayElementEndError = deserializer.template endReadArrayElement<Flags, CustomFlags, SerializeComponentType, type>(arrayElement);
				if (arrayElementEndError.hasValue()) {
					return arrayElementEndError.value().addSource(sourceName, "");
				}
			}

			auto endArrayError = deserializer.template endReadArray<Flags, CustomFlags, SerializeComponentType>(info);
			if (endArrayError.hasValue()) {
				return endArrayError.value().addSource(sourceName, "");
			}

			return {};
		}
	};

	template<typename DataType, Size bufferSize, typename allocator_type>
	struct Serialize<SmallDynArray<DataType, bufferSize, allocator_type>> {
		using as_type = SerializeArray<SerializeTypeOf<DataType>>;
		using type = SmallDynArray<DataType, bufferSize, allocator_type>;
		template<typename Serializer> using error_type = StandardSerializeError<Serializer>;
		constexpr static ConstAsciiStringView sourceName = "natl::Serialize<natl::SmallDynArray<...>>::write";

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType,
			typename... ElementSerializeArgs>
			requires(IsSerializerC<Serializer> && IsSerializeComponentC<SerializeComponentType> 
				&& CanSerializeArrayC<Serializer> && CanSerializeC<Serializer, DataType>)
		[[nodiscard]] constexpr static Option<error_type<Serializer>> write(Serializer& serializer, const type& array, ElementSerializeArgs&&... elementSerializeArgs) noexcept {
			if (array.isEmpty()) {
				auto emptyError = serializer.template writeEmptyArray<Flags, CustomFlags, SerializeComponentType>();
				if (emptyError.hasValue()) { return emptyError.value().addSource(sourceName); }
			} else {
				auto beginError = serializer.template beginWriteArray<Flags, CustomFlags, SerializeComponentType>(array.size());
				if (beginError.hasValue()) { return beginError.value().addSource(sourceName); }

				for (Size i = 0; i < array.size(); i++) {
					using array_member = SerializeArrayComponent<type>;

					auto beginElementError = serializer.template beginWriteArrayElement<Flags, CustomFlags, array_member>();
					if (beginElementError.hasValue()) { return beginElementError.value().addSource(sourceName); }

					auto elementWriteError = serializeWrite<Serializer, Flags, CustomFlags, array_member>(
						serializer, array[i], forward<ElementSerializeArgs>(elementSerializeArgs)...);
					if (elementWriteError.hasValue()) { return elementWriteError.value().addSource(sourceName); }

					auto endElementError = serializer.template endWriteArrayElement<Flags, CustomFlags, array_member>();
					if (endElementError.hasValue()) { return endElementError.value().addSource(sourceName); }
				}

				auto endError = serializer.template endWriteArray<Flags, CustomFlags, SerializeComponentType>();
				if (endError.hasValue()) { return endError.value().addSource(sourceName); }
			}
			return natl::OptionEmpty{};
		}
	};

	template<typename DataType, Size bufferSize, typename allocator_type>
		requires(IsSerializableC<Decay<DataType>>)
	struct Deserialize<SmallDynArray<DataType, bufferSize, allocator_type>> {
		using element_as_type = SerializeTypeOf<Decay<DataType>>;
		using as_type = SerializeArray<element_as_type>;
		using type = SmallDynArray<DataType, bufferSize, allocator_type>;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<SmallDynArray<...>>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags,
			typename SerializeComponentType, typename... DeserializerArgs>
			requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> 
				&& CanDeserializeArrayC<Deserializer> && CanDeserializeC<Deserializer, DataType>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				type& dst,
				DeserializerArgs&&... deserializerArgs) noexcept {

			auto arraySizeExpect = deserializer.template beginReadArray<Flags, CustomFlags, SerializeComponentType>(info);
			if (arraySizeExpect.hasError()) {
				return arraySizeExpect.error().addSource(sourceName, "");
			}
			Size arraySize = arraySizeExpect.value();

			if (arraySize == 0) {
				auto endArrayError = deserializer.template endReadEmptyArray<Flags, CustomFlags, SerializeComponentType>(info);
				if (endArrayError.hasValue()) {
					return endArrayError.value().addSource(sourceName, "");
				}
				return {};
			}

			dst.resize(arraySize);
			for (Size index = 0; index < arraySize; index++) {
				auto arrayElementExpect = deserializer.template beginReadArrayElement<Flags, CustomFlags, SerializeComponentType>(info);
				if (arrayElementExpect.hasError()) {
					return arrayElementExpect.error().addSource(sourceName, "");
				}
				auto arrayElement = arrayElementExpect.value();

				using array_component = SerializeArrayComponent<type>;
				auto expectValue = deserializeRead<Deserializer, Flags, CustomFlags, array_component, DataType>(
					deserializer, arrayElement, forward<DeserializerArgs>(deserializerArgs)...);
				if (expectValue.hasError()) {
					dst.resize(index);
					return expectValue.error().addSource(sourceName, "");
				}
				dst[index] = expectValue.value();

				auto arrayElementEndError = deserializer.template endReadArrayElement<Flags, CustomFlags, SerializeComponentType>(arrayElement);
				if (arrayElementEndError.hasValue()) {
					return arrayElementEndError.value().addSource(sourceName, "");
				}
			}

			auto endArrayError = deserializer.template endReadArray<Flags, CustomFlags, SerializeComponentType>(info);
			if (endArrayError.hasValue()) {
				return endArrayError.value().addSource(sourceName, "");
			}

			return {};
		}
	};
}