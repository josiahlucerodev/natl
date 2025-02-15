#pragma once 

//own
#include "base.h"
#include "basicCompatibility.h"

//interface 
namespace natl {
	//serialize specialization
	template<> struct Serialize<i8> {
		using as_type = SerializeI8;
		using type = i8;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer> && IsSerializeComponentC<SerializeComponentType> && CanSerializeI8C<Serializer>)
		constexpr static void write(Serializer& serializer, const i8 value) noexcept {
			serializer.template writeI8<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<i16> {
		using as_type = SerializeI16;
		using type = i16;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeI16C<Serializer>)
		constexpr static void write(Serializer& serializer, const i16 value) noexcept {
			serializer.template writeI16<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<i32> {
		using as_type = SerializeI32;
		using type = i32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeI32C<Serializer>)
		constexpr static void write(Serializer& serializer, const i32 value) noexcept {
			serializer.template writeI32<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<i64> {
		using as_type = SerializeI64;
		using type = i64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeI64C<Serializer>)
		constexpr static void write(Serializer& serializer, const i64 value) noexcept {
			serializer.template writeI64<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<ui16> {
		using as_type = SerializeUI16;
		using type = ui16;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeUI16C<Serializer>)
		constexpr static void write(Serializer& serializer, const ui16 value) noexcept {
			serializer.template writeUI16<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<ui32> {
		using as_type = SerializeUI32;
		using type = ui32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeUI32C<Serializer>)
		constexpr static void write(Serializer& serializer, const ui32 value) noexcept {
			serializer.template writeUI32<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<ui64> {
		using as_type = SerializeUI64;
		using type = ui64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeUI64C<Serializer>)
		constexpr static void write(Serializer& serializer, const ui64 value) noexcept {
			serializer.template writeUI64<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<f32> {
		using as_type = SerializeF32;
		using type = f32;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeF32C<Serializer>)
		constexpr static void write(Serializer& serializer, const f32 value) noexcept {
			serializer.template writeF32<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};
	template<> struct Serialize<f64> {
		using as_type = SerializeF64;
		using type = f64;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeF64C<Serializer>)
		constexpr static void write(Serializer& serializer, const f64 value) noexcept {
			serializer.template writeF64<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<Bool> {
		using as_type = SerializeBool;
		using type = Bool;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeBoolC<Serializer>)
		constexpr static void write(Serializer& serializer, const Bool value) noexcept {
			serializer.template writeBool<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<Ascii> {
		using as_type = SerializeChar;
		using type = Ascii;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeChar<Serializer>)
		constexpr static void write(Serializer& serializer, const Ascii value) noexcept {
			serializer.template writeChar<Flags, CustomFlags, SerializeComponentType>(value);
		}
	};

	template<> struct Serialize<char*> {
		using as_type = SerializeStr;
		using type = Ascii*;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeStrC<Serializer>)
		constexpr static void write(Serializer& serializer, const char* str) noexcept {
			serializer.template writeStr<Flags, CustomFlags, SerializeComponentType>(str);
		}
	};
	template<> struct Serialize<AsciiStringView> {
		using as_type = SerializeStr;
		using type = AsciiStringView;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeStrC<Serializer>)
		constexpr static void write(Serializer& serializer, const AsciiStringView str) noexcept {
			serializer.template writeStr<Flags, CustomFlags, SerializeComponentType>(str);
		}
	};
	template<> struct Serialize<ConstAsciiStringView> {
		using as_type = SerializeStr;
		using type = ConstAsciiStringView;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeStrC<Serializer>)
		constexpr static void write(Serializer& serializer, const ConstAsciiStringView str) noexcept {
			serializer.template writeStr<Flags, CustomFlags, SerializeComponentType>(str);
		}
	};

	template<typename ElementType>
		requires(IsSerializableC<Decay<ElementType>>)
	struct Serialize<ArrayView<ElementType>> {
		using as_type = SerializeArray<Decay<ElementType>>;
		using type = ArrayView<ElementType>;
		template<typename Serializer> using error_type = void;

		template<typename Serializer, SerializeWriteFlag Flags,
			CustomSerializeWriteFlag<Serializer> CustomFlags, typename SerializeComponentType,
			typename... ElementSerializeArgs>
			requires(IsSerializerC<Serializer>&& IsSerializeComponentC<SerializeComponentType>&& CanSerializeArrayC<Serializer>)
		constexpr static void write(Serializer& serializer, const type& arrayView, ElementSerializeArgs&&... elementSerializeArgs) noexcept {
			if (arrayView.isEmpty()) {
				serializer.template writeEmptyArray<Flags, CustomFlags, SerializeComponentType>();
			} else {
				serializer.template beginWriteArray<Flags, CustomFlags, SerializeComponentType>(arrayView.size());
				for (Size i = 0; i < arrayView.size(); i++) {
					using array_member = SerializeArrayComponent<type>;
					serializer.template beginWriteArrayElement<Flags, CustomFlags, array_member>();
					serializeWrite<Serializer, Flags, CustomFlags, array_member>(
						serializer, arrayView[i], forward<ElementSerializeArgs>(elementSerializeArgs)...);
					serializer.template endWriteArrayElement<Flags, CustomFlags, array_member>();
				}
				serializer.template endWriteArray<Flags, CustomFlags, SerializeComponentType>();
			}
		}
	};

	//deserialize specialization
	template<> struct Deserialize<i8> {
		using as_type = SerializeI8;
		using type = i8;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<i8>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> && CanDeserializeI8C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i8& dst) noexcept {
			auto valueExpect = deserializer.template readI8<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};


	template<> struct Deserialize<i16> {
		using as_type = SerializeI16;
		using type = i16;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<i16>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>&& CanDeserializeI16C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i16& dst) noexcept {
			auto valueExpect = deserializer.template readI16<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<i32> {
		using as_type = SerializeI8;
		using type = i32;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<i32>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>&& CanDeserializeI32C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i32& dst) noexcept {
			auto valueExpect = deserializer.template readI32<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<i64> {
		using as_type = SerializeI64;
		using type = i64;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<i64>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>&& CanDeserializeI64C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				i64& dst) noexcept {
			auto valueExpect = deserializer.template readI64<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui8> {
		using as_type = SerializeUI8;
		using type = ui8;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<ui8>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>&& CanDeserializeUI8C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui8& dst) noexcept {
			auto valueExpect = deserializer.template readUI8<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui16> {
		using as_type = SerializeUI16;
		using type = ui16;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<ui16>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> && CanDeserializeUI16C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui16& dst) noexcept {
			auto valueExpect = deserializer.template readUI16<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui32> {
		using as_type = SerializeUI32;
		using type = ui32;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<ui32>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> && CanDeserializeUI32C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui32& dst) noexcept {
			auto valueExpect = deserializer.template readUI32<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<ui64> {
		using as_type = SerializeUI64;
		using type = ui64;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<ui64>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> && CanDeserializeUI64C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				ui64& dst) noexcept {
			auto valueExpect = deserializer.template readUI64<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<f32> {
		using as_type = SerializeF32;
		using type = f32;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<f32>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>&& CanDeserializeF32C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				f32& dst) noexcept {
			auto valueExpect = deserializer.template readF32<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<f64> {
		using as_type = SerializeF64;
		using type = f64;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<f64>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> && CanDeserializeF64C<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				f64& dst) noexcept {
			auto valueExpect = deserializer.template readF64<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<Bool> {
		using as_type = SerializeBool;
		using type = Bool;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<Bool>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer> && IsSerializeComponentC<SerializeComponentType> && CanDeserializeBoolC<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				Bool& dst) noexcept {
			auto valueExpect = deserializer.template readBool<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};

	template<> struct Deserialize<Ascii> {
		using as_type = SerializeChar;
		using type = Ascii;
		constexpr static ConstAsciiStringView sourceName = "Deserialize<Ascii>::read";
		template<typename Deserializer> using error_type = StandardDeserializeError<Deserializer>;

		template<typename Deserializer, DeserializeReadFlag Flags, CustomDeserializeReadFlag<Deserializer> CustomFlags, typename SerializeComponentType>
			requires(IsDeserializerC<Deserializer>&& IsSerializeComponentC<SerializeComponentType>&& CanDeserializeCharC<Deserializer>)
		constexpr static Option<error_type<Deserializer>>
			read(Deserializer& deserializer,
				typename Deserializer::template deserialize_info<as_type>& info,
				Ascii& dst) noexcept {
			auto valueExpect = deserializer.template readChar<Flags, CustomFlags, SerializeComponentType>(info);
			if (valueExpect.hasValue()) {
				dst = valueExpect.value();
				return {};
			} else {
				return valueExpect.error().addSource(sourceName, "");
			}
		}
	};
}