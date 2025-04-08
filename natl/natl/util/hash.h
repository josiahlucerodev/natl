#pragma once

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "compilerDependent.h"
#include "bits.h"

//interface 
namespace natl {
	//fnv1a
	constexpr inline Size fnv1aOffsetBasis = Size(14695981039346656037ULL);
	constexpr inline Size fnv1aPrime = Size(1099511628211ULL);
	constexpr Size fnv1aHashCstr(const Ascii* strPtr, Size hash = fnv1aOffsetBasis) noexcept {
		while (*strPtr != '\0') {
			hash ^= static_cast<Size>(*strPtr++);
			hash *= fnv1aPrime;
		}
		return hash;
	}
	constexpr Size fnv1aHashCstr(const Utf32* strPtr, Size hash = fnv1aOffsetBasis) noexcept {
		while (*strPtr != '\0') {
			hash ^= static_cast<Size>(*strPtr++);
			hash *= fnv1aPrime;
		}
		return hash;
	}

	constexpr Size fnv1aHash(const Ascii* strPtr, const Size size, Size hash = fnv1aOffsetBasis) noexcept {
		const Ascii* endPtr = strPtr + size;
		for (; strPtr < endPtr; strPtr++) {
			hash ^= static_cast<Size>(*strPtr);
			hash *= fnv1aPrime;
		}
		return hash;
	}
	constexpr Size fnv1aHash(const Utf32* strPtr, const Size size, Size hash = fnv1aOffsetBasis) noexcept {
		const Utf32* endPtr = strPtr + size;
		for (; strPtr < endPtr; strPtr++) {
			hash ^= static_cast<Size>(*strPtr);
			hash *= fnv1aPrime;
		}
		return hash;
	}

	template<typename Type>
	concept HasStaticHashFunctionC = requires(const Type& value) {
		{ Type::staticHash(value) } -> SameAs<Size>;
	};
	template<typename Type> struct HasStaticHashFunctionV : BoolConstant<HasStaticHashFunctionC<Type>> {};
	template<typename Type> constexpr inline Bool HasStaticHashFunction = HasStaticHashFunctionC<Type>;

	template<typename Type>
	concept HasHashFunctionC = requires(const Type& value) {
		{ value.hash() } -> SameAs<Size>;
	};
	template<typename Type> struct HasHashFunctionV : BoolConstant<HasHashFunctionC<Type>> {};
	template<typename Type> constexpr inline Bool HasHashFunction = HasHashFunctionC<Type>;

	template <typename Type>
	concept StdHashableC = requires(const Type& a) {
		{ std::hash<Type>{}(a) } -> ConvertibleTo<Size>;
	};
	template<typename Type> struct StdHashableV : BoolConstant<StdHashableC<Type>> {};
	template<typename Type> constexpr inline Bool StdHashable = StdHashableC<Type>;

	template<typename HashCombiner>
	concept IsHashCombinerC = requires(HashCombiner & hashCombiner) {
		{ hashCombiner.hashCombine(declval<Size>(), declval<Size>()) };
	};
	template<typename Type> struct IsHashCombinerV : BoolConstant<IsHashCombinerC<Type>> {};
	template<typename Type> constexpr inline Bool IsHashCombiner = IsHashCombinerC<Type>;

	template<typename Hasher>
	concept IsHasherC = requires(Hasher & hasher) {
		typename Hasher::hash_combiner;
		{ hasher.hashCode() } -> IsSameC<Size>;
		{ hasher.reset() } -> IsSameC<void>;
		{ hasher.hashAppend(declval<const Ascii*>(), declval<Size>()) } -> IsSameC<void>;
		{ hasher.hashAppend(declval<Size>()) } -> IsSameC<void>;
	};
	template<typename Type> struct IsHasherV : BoolConstant<IsHasherC<Type>> {};
	template<typename Type> constexpr inline Bool IsHasher = IsHasherC<Type>;

	template<typename Hasher, typename Type>
	concept HasHashAppendC = requires(const Type& value, Hasher& hasher) {
		{ value.template hashAppend<Hasher>(hasher) };
	};
	template<typename Hasher, typename Type> struct HasHashAppendV : BoolConstant<HasHashAppendC<Hasher, Type>> {};
	template<typename Hasher, typename Type> constexpr inline Bool HasHashAppend = HasHashAppendC<Hasher, Type>;

	template<typename Type> struct Hash;
	template<typename Type> concept HasHashC = requires() {
		typename Hash<Type>;
	};
	template<typename Type> struct HasHashV : BoolConstant<HasHashC<Type>> {};
	template<typename Type> constexpr inline Bool HasHash = HasHashC<Type>;

	struct StandardHashCombiner {
		constexpr Size hashCombine(const Size hash, const Size otherHash) const noexcept {
			return (hash ^ otherHash) + (Size(0x9e3779b9)) + ((hash << Size(6))) + ((hash >> Size(2)));
		}
	};

	template<typename HashCombiner>
		requires(IsHashCombinerC<HashCombiner>)
	struct Fnv1aHasher {
		using hash_combiner = HashCombiner;
		Size hash = fnv1aOffsetBasis;
		hash_combiner hashCombiner;

		constexpr Size hashCode() const noexcept {
			return hash;
		}
		constexpr void reset() noexcept {
			hash = fnv1aOffsetBasis;
		}
		constexpr void hashAppend(const Ascii* str, const Size len) noexcept {
			const Ascii* strPtr = str;
			const Ascii* endPtr = str + len;
			for (; strPtr < endPtr; strPtr++) {
				hash ^= static_cast<Size>(*strPtr);
				hash *= fnv1aPrime;
			}
		}
		constexpr void hashAppend(const Size otherHash) noexcept {
			hash = hashCombiner.hashCombine(hash, otherHash);
		}
	};

	using StandardHasher = Fnv1aHasher<StandardHashCombiner>;

	template<typename Type>
	concept IsHashableC = HasStaticHashFunctionC<Type> || HasHashFunctionC<Type>
		|| IsPointerC<Type> || StdHashableC<Type> || HasHashAppendC<StandardHasher, Type>
		|| HasHashC<Type>;
	template<typename Type> struct IsHashableV : BoolConstant<IsHashableC<Type>> {};
	template<typename Type> constexpr inline Bool IsHashable = IsHashableC<Type>;

	namespace impl {
		template<typename Hasher, typename Type>
		constexpr void hashAppendSingle(Hasher& hasher, const Type& value) noexcept {
			if constexpr (HasHashAppendC<Hasher, Type>) {
				value.template hashAppend<Hasher>(hasher);
			} else {
				hasher.hashAppend(hashValue<Type>(value));
			}
		}
	}

	template<typename Hasher, typename... Types>
		requires(IsHasherC<Hasher> && (IsHashableC<Types> && ...))
	constexpr void hashAppend(Hasher& hasher, const Types&... value) noexcept {
		(impl::hashAppendSingle<Hasher, Types>(hasher, value), ...);
	}

	template<typename Hasher, typename... Types>
		requires(IsHasherC<Hasher> && (IsHashableC<Types> && ...))
	constexpr Size hashValues(const Types&... values) {
		Hasher hasher{};
		(hashAppend<Hasher, Types>(hasher, values), ...);
		return hasher.hashCode();
	}

	template<typename Type>
	struct Hash {
	public:
		constexpr static Size hash(const Type& value) requires(IsHashableC<Type>) {
			if constexpr (HasStaticHashFunctionC<Type>) {
				return Type::staticHash(value);
			} else if constexpr (HasHashAppendC<StandardHasher, Type>) {
				return hashValues<StandardHasher>(value);
			} else if constexpr (HasHashFunctionC<Type>) {
				return value.hash();
			} else if constexpr (IsPointerC<Type>) {
				return static_cast<Size>(natl::bitCast<UIntPtrSized, Type>(value));
			} else if constexpr (StdHashableC<Type>) {
				return static_cast<Size>(std::hash<Type>{}(value));
			} else {
				static_assert(AlwaysFalse<Type>, "natl: Hash<Type>::hash - Type has no hash");
			}
		}
	};

	template<> struct Hash<i8> {
		constexpr static Size hash(const i8 value) { return static_cast<Size>(natl::bitCast<ui8, i8>(value)); }
	};
	template<> struct Hash<i16> {
		constexpr static Size hash(const i16 value) { return static_cast<Size>(natl::bitCast<ui16, i16>(value)); }
	};
	template<> struct Hash<i32> {
		constexpr static Size hash(const i32 value) { return static_cast<Size>(natl::bitCast<ui32, i32>(value)); }
	};
	template<> struct Hash<i64> {
		constexpr static Size hash(const i64 value) { return static_cast<Size>(natl::bitCast<ui64, i64>(value)); }
	};

	template<> struct Hash<ui8> {
		constexpr static Size hash(const ui8 value) { return static_cast<Size>(value); }
	};
	template<> struct Hash<ui16> {
		constexpr static Size hash(const ui16 value) { return static_cast<Size>(value); }
	};
	template<> struct Hash<ui32> {
		constexpr static Size hash(const ui32 value) { return static_cast<Size>(value); }
	};
	template<> struct Hash<ui64> {
		constexpr static Size hash(const ui64 value) { return static_cast<Size>(value); }
	};

	template<> struct Hash<float> {
		constexpr static Size hash(const float value) { return static_cast<Size>(natl::bitCast<ui32, float>(value)); }
	};
	template<> struct Hash<double> {
		constexpr static Size hash(const double value) { return static_cast<Size>(natl::bitCast<ui64, double>(value)); }
	};

	template<typename Type>
		requires(IsHashableC<Type>)
	constexpr Size hashValue(const Type& value) noexcept {
		return Hash<Decay<Type>>().hash(value);
	}
 }
 static_assert(natl::IsHashCombinerC<natl::StandardHashCombiner>, "natl: StandardHashCombiner is not a natl::HasherCombiner");
 static_assert(natl::IsHasherC<natl::Fnv1aHasher<natl::StandardHashCombiner>>, "natl: Fnv1aHasher is not a natl::Hasher");