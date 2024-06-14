#pragma once

//own
#include "../util/compilerDependent.h"

//intrinsics
#ifdef NATL_COMPILER_MSVC
#include <immintrin.h>
#endif //NATL_COMPILER_MSVC

//interface 
namespace natl::math {
	//sqrt
	namespace impl {
		NATL_FORCE_INLINE constexpr f32 basicSqrtF32ImplS(const f32 value) noexcept {
			using namespace natl::literals;
			constexpr f32 sqrtMagicNumber = static_cast<f32>(0x5f3759df);
			const f32 halfX = 0.5_f32 * value;
			const f32 x = bitCast<f32, i32>(static_cast<i32>(sqrtMagicNumber - (bitCast<ui32, f32>(value) >> 1)));
			 return value * x * (1.5_f32 - halfX * x * x);
		}
		NATL_FORCE_INLINE constexpr f64 basicSqrtF64ImplS(const f64 value) noexcept {
			using namespace natl::literals;
			constexpr f64 sqrtMagicNumber = static_cast<f64>(0x5fe6eb50c7b537a9);
			const f64 halfX = 0.5_f64 * value;
			const f64 x = bitCast<f64, i64>(static_cast<i64>(sqrtMagicNumber - (bitCast<ui64, f64>(value) >> 1)));
			return value * x * (1.5_f64 - halfX * x * x);
		}

#ifdef NATL_COMPILER_MSVC
		NATL_FORCE_INLINE f32 basicSqrtF32ImplD(const f32 value) noexcept {
			return __sqrt_ss(value);
		}
		NATL_FORCE_INLINE f64 basicSqrtF64ImplD(const f64 value) noexcept {
			return __sqrt_sd(value);
		}
#endif //NATL_COMPILER_MSVC
	}

	NATL_FORCE_INLINE constexpr f32 basicSqrtF32(const f32 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::basicSqrtF32ImplS(value);
		} else {
			return impl::basicSqrtF32ImplD(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 basicSqrtF64(const f64 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::basicSqrtF64ImplS(value);
		} else {
			return impl::basicSqrtF64ImplD(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicSqrt(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicSqrtF64(value);
		} else {
			return basicSqrtF64(value);
		}
	}

	namespace impl {
		NATL_FORCE_INLINE constexpr f32 sqrtF32ImplS(const f32 value) noexcept {
			using namespace natl::literals;
			if (value < 0.0_f32) {
				return Limits<f32>::quietNaN();
			} 
			if (isInfinityF32(value) || isnanF32(value)) {
				return value;
			}
			return basicSqrtF32ImplS(value);
		}
		NATL_FORCE_INLINE constexpr f64 sqrtF64ImplS(const f64 value) noexcept {
			using namespace natl::literals;
			if (value < 0.0_f64) {
				return Limits<f64>::quietNaN();
			}
			if (isInfinityF64(value) || isnanF64(value)) {
				return value;
			}
			return basicSqrtF64ImplS(value);
		}

#ifdef NATL_COMPILER_MSVC
		NATL_FORCE_INLINE f32 sqrtF32ImplD(const f32 value) noexcept {
			return __sqrt_ss(value);
		}
		NATL_FORCE_INLINE f64 sqrtF64ImplD(const f64 value) noexcept {
			return __sqrt_sd(value);
		}
#endif //NATL_COMPILER_MSVC
	}


	NATL_FORCE_INLINE constexpr f32 sqrtF32(const f32 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::sqrtF32ImplS(value);
		} else {
			return impl::sqrtF32ImplD(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 sqrtF64(const f64 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::sqrtF64ImplS(value);
		} else {
			return impl::sqrtF64ImplD(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType sqrt(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return sqrtF64(value);
		} else {
			return sqrtF64(value);
		}
	}

	//rsqrt 
	namespace impl {
		NATL_FORCE_INLINE constexpr f32 basicRsqrtF32ImplS(const f32 value) noexcept {
			using namespace natl::literals;
			constexpr i32 rsqrtMagicNumber = static_cast<i32>(0x5f3759df);
			const f32 halfX = 0.5_f32 * value;
			f32 x = bitCast<f32, i32>(rsqrtMagicNumber - (bitCast<i32, f32>(value) >> 1));
			x = x * (1.5_f32 - (halfX * x * x));
			x = x * (1.5_f32 - (halfX * x * x));
			return x;
		}
		NATL_FORCE_INLINE constexpr f64 basicRsqrtF64ImplS(const f64 value) noexcept {
			using namespace natl::literals;
			constexpr i64 rsqrtMagicNumber = static_cast<i64>(0x5fe6eb50c7b537a9);
			const f64 halfX = 0.5_f64 * value;
			f64 x = bitCast<f64, i64>(rsqrtMagicNumber - (bitCast<i64, f64>(value) >> 1));
			x = x * (1.5_f64 - (halfX * x * x));
			x = x * (1.5_f64 - (halfX * x * x));
			return x;
		}

#ifdef NATL_COMPILER_MSVC
		NATL_FORCE_INLINE f32 basicRsqrtF32ImplD(const f32 value) noexcept {
			return basicRsqrtF32ImplS(value);
		}
		NATL_FORCE_INLINE f64 basicRsqrtF64ImplD(const f64 value) noexcept {
			return basicRsqrtF64ImplS(value);
		}
#endif //NATL_COMPILER_MSVC
	}

	NATL_FORCE_INLINE constexpr f32 basicRsqrtF32(const f32 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::basicRsqrtF32ImplS(value);
		} else {
			return impl::basicRsqrtF32ImplD(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 basicRsqrtF64(const f64 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::basicRsqrtF64ImplS(value);
		} else {
			return impl::basicRsqrtF64ImplD(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicRsqrt(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicRsqrtF64(value);
		} else {
			return basicRsqrtF64(value);
		}
	}

	namespace impl {
		NATL_FORCE_INLINE constexpr f32 rsqrtF32ImplS(const f32 value) noexcept {
			using namespace natl::literals;
			if (value <= 0.0_f32) {
				return Limits<f32>::quietNaN();
			}
			if (isInfinityF32(value) || isnanF32(value)) {
				return value;
			}
			return basicRsqrtF32ImplS(value);
		}
		NATL_FORCE_INLINE constexpr f64 rsqrtF64ImplS(const f64 value) noexcept {
			using namespace natl::literals;
			if (value <= 0.0_f64) {
				return Limits<f64>::quietNaN();
			}
			if (isInfinityF64(value) || isnanF64(value)) {
				return value;
			}
			return basicRsqrtF64ImplS(value);
		}

#ifdef NATL_COMPILER_MSVC
		NATL_FORCE_INLINE f32 rsqrtF32ImplD(const f32 value) noexcept {
			return rsqrtF32ImplS(value);
		}
		NATL_FORCE_INLINE f64 rsqrtF64ImplD(const f64 value) noexcept {
			return rsqrtF64ImplS(value);
		}
#endif //NATL_COMPILER_MSVC
	}

	NATL_FORCE_INLINE constexpr f32 rsqrtF32(const f32 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::rsqrtF32ImplS(value);
		} else {
			return impl::rsqrtF32ImplD(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 rsqrtF64(const f64 value) noexcept {
		if (isConstantEvaluated()) {
			return impl::rsqrtF64ImplS(value);
		} else {
			return impl::rsqrtF64ImplD(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType rsqrt(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return rsqrtF64(value);
		} else {
			return rsqrtF64(value);
		}
	}

	//hypot 
	NATL_FORCE_INLINE constexpr f32 basicHypotF32(const f32 x, const f32 y) noexcept {
		return basicSqrtF32((x * x) + (y * y));
	}
	NATL_FORCE_INLINE constexpr f64 basicHypotF64(const f64 x, const f64 y) noexcept {
		return basicSqrtF64((x * x) + (y * y));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicHypot(const FloatType x, const FloatType y) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicHypotF32(x, y);
		} else {
			return basicHypotF64(x, y);
		}
	}

	NATL_FORCE_INLINE constexpr f32 hypotF32(const f32 x, const f32 y) noexcept {
		return sqrtF32((x * x) + (y * y));
	}
	NATL_FORCE_INLINE constexpr f64 hypotF64(const f64 x, const f64 y) noexcept {
		return sqrtF64((x * x) + (y * y));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType hypot(const FloatType x, const FloatType y) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return hypotF32(x, y);
		} else {
			return hypotF64(x, y);
		}
	}

	NATL_FORCE_INLINE constexpr f32 basicHypotF32(const f32 x, const f32 y, const f32 z) noexcept {
		return basicSqrtF32((x * x) + (y * y) + z * z);
	}
	NATL_FORCE_INLINE constexpr f64 basicHypotF64(const f64 x, const f64 y, const f32 z) noexcept {
		return basicSqrtF64((x * x) + (y * y) + (z * z));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicHypot(const FloatType x, const FloatType y, const FloatType z) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicHypotF32(x, y, z);
		} else {
			return basicHypotF64(x, y, z);
		}
	}

	NATL_FORCE_INLINE constexpr f32 hypotF32(const f32 x, const f32 y, const f32 z) noexcept {
		return sqrtF32((x * x) + (y * y) + z * z);
	}
	NATL_FORCE_INLINE constexpr f64 hypotF64(const f64 x, const f64 y, const f32 z) noexcept {
		return sqrtF64((x * x) + (y * y) + (z * z));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType hypot(const FloatType x, const FloatType y, const FloatType z) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return hypotF32(x, y, z);
		} else {
			return hypotF64(x, y, z);
		}
	}
}