#pragma once

//@begin_non_modules
//own
#include "../util/compilerDependent.h"
//@end_non_modules

//own
#include "basicFloatOperations.h"
#include "exponential.h"
#include "power.h"

//@export
namespace natl::math {
	//sinh
	NATL_FORCE_INLINE constexpr f32 basicSinhF32(const f32 value) noexcept {
		using namespace natl::literals;
		return 0.5_f32 * (basicExpF32(value) - basicExpF32(-value));
	}
	NATL_FORCE_INLINE constexpr f64 basicSinhF64(const f64 value) noexcept {
		using namespace natl::literals;
		return 0.5_f64 * (basicExpF64(value) - basicExpF64(-value));;
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicSinh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicSinhF32(value);
		} else {
			return basicSinhF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 sinhF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return value;
		} else {
			return basicSinhF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 sinhF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return value;
		} else {
			return basicSinhF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType sinh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return sinhF32(value);
		} else {
			return sinhF64(value);
		}
	}

	//cosh 
	NATL_FORCE_INLINE constexpr f32 basicCoshF32(const f32 value) noexcept {
		using namespace natl::literals;
		return 0.5_f32 * (basicExpF32(value) + basicExpF32(-value));
	}
	NATL_FORCE_INLINE constexpr f64 basicCoshF64(const f64 value) noexcept {
		using namespace natl::literals;
		return 0.5_f64 * (basicExpF64(value) + basicExpF64(-value));;
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicCosh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicCoshF32(value);
		} else {
			return basicCoshF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 coshF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return value;
		} else {
			return basicCoshF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 coshF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return value;
		} else {
			return basicCoshF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType cosh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return coshF32(value);
		} else {
			return coshF64(value);
		}
	}

	//tanh 
	NATL_FORCE_INLINE constexpr f32 basicTanhF32(const f32 value) noexcept {
		const f32 eToTheX = basicExpF32(value);
		const f32 eToTheNegativeX = basicExpF32(-value);
		return (eToTheX - eToTheNegativeX) / (eToTheX + eToTheNegativeX);
	}
	NATL_FORCE_INLINE constexpr f64 basicTanhF64(const f64 value) noexcept {
		const f64 eToTheX = basicExpF64(value);
		const f64 eToTheNegativeX = basicExpF64(-value);
		return (eToTheX - eToTheNegativeX) / (eToTheX + eToTheNegativeX);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicTanh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicTanhF32(value);
		} else {
			return basicTanhF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 tanhF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return value;
		} else {
			return basicTanhF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 tanhF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return value;
		} else {
			return basicTanhF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType tanh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return tanhF32(value);
		} else {
			return tanhF64(value);
		}
	}

	//asinh 
	NATL_FORCE_INLINE constexpr f32 basicAsinhF32(const f32 value) noexcept {
		using namespace natl::literals;
		return basicLnF32(value + basicSqrtF32((value * value) + 1.0_f32));
	}
	NATL_FORCE_INLINE constexpr f64 basicAsinhF64(const f64 value) noexcept {
		using namespace natl::literals;
		return basicLnF64(value + basicSqrtF64((value * value) + 1.0_f64));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicAsinh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicAsinhF32(value);
		} else {
			return basicAsinhF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 asinhF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return value;
		} else {
			return basicAsinhF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 asinhF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return value;
		} else {
			return basicAsinhF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType asinh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return asinhF32(value);
		} else {
			return asinhF64(value);
		}
	}

	//acosh 
	NATL_FORCE_INLINE constexpr f32 basicAcoshF32(const f32 value) noexcept {
		using namespace natl::literals;
		return basicLnF32(value + basicSqrtF32((value * value) - 1.0_f32));
	}
	NATL_FORCE_INLINE constexpr f64 basicAcoshF64(const f64 value) noexcept {
		using namespace natl::literals;
		return basicLnF64(value + basicSqrtF64((value * value) - 1.0_f64));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicAcosh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicAcoshF32(value);
		} else {
			return basicAcoshF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 acoshF32(const f32 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF32(value) || isnanF32(value)) {
			return value;
		} else if (value < 1.0_f32) {
			return Limits<f32>::quietNaN();
		} else {
			return basicAcoshF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 acoshF64(const f64 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF64(value) || isnanF64(value)) {
			return value;
		} else if (value < 1.0_f64) {
			return Limits<f64>::quietNaN();
		} else {
			return basicAcoshF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType acosh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return acoshF32(value);
		} else {
			return acoshF64(value);
		}
	}

	//atanh 
	NATL_FORCE_INLINE constexpr f32 basicAtanhF32(const f32 value) noexcept {
		using namespace natl::literals;
		return 0.5_f32 * basicLnF32((1.0_f32 + value) / (1.0_f32 - value));
	}
	NATL_FORCE_INLINE constexpr f64 basicAtanhF64(const f64 value) noexcept {
		using namespace natl::literals;
		return 0.5_f64 * basicLnF64((1.0_f64 + value) / (1.0_f64 - value));
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicAtanh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicAtanhF32(value);
		} else {
			return basicAtanhF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 atanhF32(const f32 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF32(value) || isnanF32(value) || value < -1.0_f32 || value > 1.0_f32) {
			return Limits<f32>::quietNaN();
		} else if (value == -1.0_f32) {
			return -Limits<f32>::infinity();
		} else if (value == 1.0_f32) {
			return Limits<f32>::infinity();
		} else {
			return basicAtanhF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 atanhF64(const f64 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF64(value) || isnanF64(value) || value < -1.0_f64 || value > 1.0_f64) {
			return Limits<f64>::quietNaN();
		} else if (value == -1.0_f64) {
			return -Limits<f64>::infinity();
		} else if (value == 1.0_f64) {
			return Limits<f64>::infinity();
		} else {
			return basicAtanhF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType atanh(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return atanhF32(value);
		} else {
			return atanhF64(value);
		}
	}
}