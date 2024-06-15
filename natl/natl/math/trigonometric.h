#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/compilerDependent.h"
#include "basicFloatOperations.h"
#include "constants.h"

//interface 
namespace natl::math {
	NATL_FORCE_INLINE f32 degToRad(const f32 degrees) noexcept {
		using namespace natl::literals;
		return degrees * f32(pi) / 180.0_f32;
	}
	NATL_FORCE_INLINE f64 degToRad(const f64 degrees) noexcept {
		using namespace natl::literals;
		return degrees * pi / 180.0_f64;
	}

	namespace bounded {
		//sin quadratic appoximation
		// value bounds [0, 2pi) 
		//only accurate for small value
		//pos appoximation -0.4053(x-0)(x-pi) or -0.4053x^2 - 1.273288x
		//neg appoximation 0.4053(x-pi)(x-2pi) or 0.4053x^2 - 3.819863x + 8.000301
		NATL_FORCE_INLINE constexpr f32 sinQuadBoundedF32(const f32 value) noexcept {
			using namespace natl::literals;
			if (value < f32(pi)) {
				return (-0.4053_f32 * value * value) + (1.273288_f32 * value);
			} else {
				return (0.4053_f32 * value * value) - (3.819863_f32 * value) + 8.000301_f32;
			}
		}

		//sin quadratic appoximation
		//has accuracy within a tolorance of 0.1 for large values 
		// value bounds [0, 2pi) 
		//pos appoximation -0.4053(x-0)(x-pi) or -0.4053x^2 - 1.273288x
		//neg appoximation 0.4053(x-pi)(x-2pi) or 0.4053x^2 - 3.819863x + 8.000301
		NATL_FORCE_INLINE constexpr f64 sinQuadBoundedF64(const f64 value) noexcept {
			if (value < pi) {
				return (-0.4053 * value * value) + (1.273288 * value);
			} else {
				return (0.4053 * value * value) - (3.819863 * value) + 8.000301;
			}
		}
	}


	//sin quadratic appoximation 
	//only accurate for small value
	//pos appoximation -0.4053(x-0)(x-pi) or -0.4053x^2 - 1.273288x
	//neg appoximation 0.4053(x-pi)(x-2pi) or 0.4053x^2 - 3.819863x + 8.000301
	NATL_FORCE_INLINE constexpr f32 sinQuadF32(const f32 value) noexcept {
		using namespace natl::literals;
		f32 constrainedValue = fmodF32(value, f32(twoPi));
		if (constrainedValue < 0.0_f32) {
			constrainedValue += f32(twoPi);
		}
		return bounded::sinQuadBoundedF32(constrainedValue);
	}

	//sin quadratic appoximation
	//has accuracy within a tolorance of 0.1 for large values 
	//pos appoximation -0.4053(x-0)(x-pi) or -0.4053x^2 - 1.273288x
	//neg appoximation 0.4053(x-pi)(x-2pi) or 0.4053x^2 - 3.819863x + 8.000301
	NATL_FORCE_INLINE constexpr f64 sinQuadF64(const f64 value) noexcept {
		f64 constrainedValue = fmodF64(value, twoPi);
		if (constrainedValue < 0.0) {
			constrainedValue += twoPi;
		}
		return bounded::sinQuadBoundedF64(constrainedValue);
	}

	//cos quadratic appoximation
	//only accurate for small value
	NATL_FORCE_INLINE constexpr f32 cosQuadF32(const f32 value) noexcept {
		using namespace natl::literals;
		return sinQuadF32(value + f32((5 * f32(pi)) / 2.0_f32));
	}
	//cos quadratic appoximation
	//has accuracy within a tolorance of 0.1 for large values 
	NATL_FORCE_INLINE constexpr f64 cosQuadF64(const f64 value) noexcept {
		return sinQuadF64(value + ((5 * pi) / 2.0));
	}

	namespace bounded {
		//sin quadratic appoximation mulFmod
		// value bounds [0, 1.0) 
		// value / 2pi 
		//only accurate for small value
		//pos appoximation -16(x-0)(x-0.5) or -16x^2 - 8x
		//neg appoximation -16(x-0.5)(x-1.0) or 16x^2 - 24x + 8
		NATL_FORCE_INLINE constexpr f32 sinQuadBoundedMulFmodF32(const f32 value) noexcept {
			using namespace natl::literals;
			if (value < 0.5_f32) {
				return (-16.0_f32 * value * value) + (8.0_f32 * value);
			} else {
				return (16.0_f32 * value * value) - (24.0_f32 * value) + 8.0_f32;
			}
		}

		//sin quadratic appoximation mulFmod
		//has accuracy within a tolorance of 0.1 for large values 
		// value bounds [0, 1.0) 
		// value / 2pi 
		//pos appoximation -16(x-0)(x-0.5) or -16x^2 - 8x
		//neg appoximation -16(x-0.5)(x-1.0) or 16x^2 - 24x + 8
		NATL_FORCE_INLINE constexpr f64 sinQuadBoundedMulFmodF64(const f64 value) noexcept {
			if (value < 0.5) {
				return (-16.0 * value * value) + (8.0 * value);
			} else {
				return (16.0 * value * value) - (24.0 * value) + 8.0;
			}
		}
	}
	

	//sin quadratic appoximation mulFmod
	//only accurate for small value
	//pos appoximation -16(x-0)(x-0.5) or -16x^2 - 8x
	//neg appoximation -16(x-0.5)(x-1.0) or 16x^2 - 24x + 8
	NATL_FORCE_INLINE constexpr f32 sinQuadMulFmodF32(const f32 value) noexcept {
		using namespace natl::literals;
		f32 constrainedValue = mulFmodNoscaleF32(value, f32(twoPi));
		if (constrainedValue < 0.0_f32) {
			constrainedValue += 1.0_f32;
		}
		return bounded::sinQuadBoundedMulFmodF32(constrainedValue);
	}

	//sin quadratic appoximation mulFmod
	//has accuracy within a tolorance of 0.1 for large values 
	//pos appoximation -16(x-0)(x-0.5) or -16x^2 - 8x
	//neg appoximation -16(x-0.5)(x-1.0) or 16x^2 - 24x + 8
	NATL_FORCE_INLINE constexpr f64 sinQuadMulFmodF64(const f64 value) noexcept {
		f64 constrainedValue = mulFmodNoscaleF64(value, twoPi);
		if (constrainedValue < 0.0) {
			constrainedValue += 1.0;
		}
		return bounded::sinQuadBoundedMulFmodF64(constrainedValue);
	}

	//cos quadratic appoximation
	//only accurate for small value
	NATL_FORCE_INLINE constexpr f32 cosQuadMulFmodF32(const f32 value) noexcept {
		using namespace natl::literals;
		return sinQuadMulFmodF32(value + f32((5 * f32(pi)) / 2.0_f32));
	}
	//cos quadratic appoximation
	//has accuracy within a tolorance of 0.1 for large values 
	NATL_FORCE_INLINE constexpr f64 cosQuadMulFmodF64(const f64 value) noexcept {
		return sinQuadMulFmodF64(value + ((5 * pi) / 2.0));
	}


	namespace bounded {
		//cos taylor series degree 4 polynomial appoximation
		// value bounds [0, 2 pi)
		//only accurate for small value
		//half appoximation 1 - ((x^2 / 2! + 0.013) + (x^4 / 4! + 3)) or 1 - ((x^2 / 2.013) + (x^4 / 27)) 
		NATL_FORCE_INLINE constexpr f32 cosTaySerd4pBoundedF32(const f32 value) noexcept {
			using namespace natl::literals;
			const f32 sign = value < f32(pi) ? 1.0_f32 : -1.0_f32;
			const f32 x = value < f32(pi) ? value - f32(piOver2) : value - f32(piOver2) - f32(pi);
			const f32 x2 = x * x;
			const f32 x4 = x2 * x2;
			constexpr f32 d2MulDivisorConstant = f32(1 / 2.013);
			constexpr f32 d4MulDivisorConstant = f32(1 / 27);
			return sign * f32(1 - (x2 * d2MulDivisorConstant) + (x4 * d4MulDivisorConstant));
		}

		//cos taylor series degree 4 polynomial appoximation
		// value bounds [0, 2 pi)
		//only accurate for small value
		//half appoximation 1 - ((x^2 / 2! + 0.013) + (x^4 / 4! + 3)) or 1 - ((x^2 / 2.013) + (x^4 / 27)) 
		NATL_FORCE_INLINE constexpr f64 cosTaySerd4pBoundedF64(const f64 value) noexcept {
			const f64 sign = value < pi ? 1.0 : -1.0;
			const f64 x = value < pi ? value - piOver2 : value - piOver2 - pi;
			const f64 x2 = x * x;
			const f64 x4 = x2 * x2;
			constexpr f64 d2MulDivisorConstant = (1 / 2.013);
			constexpr f64 d4MulDivisorConstant = (1 / 27);
			return sign * (1 - (x2 * d2MulDivisorConstant) + (x4 * d4MulDivisorConstant));
		}
	}

	//cos taylor series degree 4 polynomial appoximation
	//only accurate for small value
	//half appoximation 1 - ((x^2 / 2! + 0.013) + (x^4 / 4! + 3)) or 1 - ((x^2 / 2.013) + (x^4 / 27)) 
	NATL_FORCE_INLINE constexpr f32 cosTaySerd4pF32(const f32 value) noexcept {
		using namespace natl::literals;
		f32 constrainedValue = fmodF32(value + f32(piOver2), f32(twoPi));
		if (constrainedValue < 0.0_f32) {
			constrainedValue += f32(twoPi);
		}
		return bounded::cosTaySerd4pBoundedF32(constrainedValue);
	}

	//cos taylor series degree 4 polynomial appoximation
	//only accurate for small value
	//half appoximation 1 - ((x^2 / 2! + 0.013) + (x^4 / 4! + 3)) or 1 - ((x^2 / 2.013) + (x^4 / 27)) 
	NATL_FORCE_INLINE constexpr f64 cosTaySerd4pF64(const f64 value) noexcept {
		using namespace natl::literals;
		f64 constrainedValue = fmodF64(value + piOver2, twoPi);
		if (constrainedValue < 0.0_f64) {
			constrainedValue += twoPi;
		}
		return bounded::cosTaySerd4pBoundedF64(constrainedValue);
	}

	NATL_FORCE_INLINE constexpr f32 sinTaySerd4pF32(const f32 value) noexcept {
		using namespace natl::literals;
		return cosTaySerd4pF32(value + f32((3 * f32(pi)) / 2.0_f32));
	}
	NATL_FORCE_INLINE constexpr f64 sinTaySerd4pF64(const f64 value) noexcept {
		using namespace natl::literals;
		return cosTaySerd4pF64(value + f64((3 * f64(pi)) / 2.0_f64));
	}


	namespace bounded {
		//tan degree 4 polynomial appoximation
		//value bounds [0, pi) 
		//only accurate for small value
		//appoximation x * ( (1/945 * x^4 - 1/9 x^2 + 1) / ((1/63 * x^4 - 4/9 x^2 + 1) ) )
		NATL_FORCE_INLINE constexpr f32 tan4dpBoundedF32(const f32 value) noexcept {
			using namespace natl::literals;
			const f32 x = value;
			const f32 x2 = x * x;
			const f32 x4 = x2 * x2;
			return f32(x * (
				((f32(1.0 / 945.0) * x4) - (f32(1.0 / 9.0) * x2) + 1)
				/ ((f32(1.0 / 63.0) * x4) - (f32(4.0 / 9.0) * x2) + 1)));
		}

		//tan degree 4 polynomial appoximation
		//value bounds [0, pi) 
		//only accurate for small value
		//appoximation x * ( (1/945 * x^4 - 1/9 x^2 + 1) / ((1/63 * x^4 - 4/9 x^2 + 1) ) )
		NATL_FORCE_INLINE constexpr f64 tan4dpBoundedF64(const f64 value) noexcept {
			const f64 x = value;
			const f64 x2 = x * x;
			const f64 x4 = x2 * x2;
			return f64(x * (
				(((1.0 / 945.0) * x4) - ((1.0 / 9.0) * x2) + 1)
				/ (((1.0 / 63.0) * x4) - ((4.0 / 9.0) * x2) + 1)));
		}
	}

	//tan degree 4 polynomial appoximation
	//only accurate for small value
	//appoximation x * ( (1/945 * x^4 - 1/9 x^2 + 1) / ((1/63 * x^4 - 4/9 x^2 + 1) ) )
	NATL_FORCE_INLINE constexpr f32 tan4dpF32(const f32 value) noexcept {
		const f32 constrainedValue = fmodF32(value, f32(pi));
		return bounded::tan4dpBoundedF32(constrainedValue);
	}

	//tan degree 4 polynomial appoximation
	//x * ( (1/945 * x^4 - 1/9 x^2 + 1) / ((1/63 * x^4 - 4/9 x^2 + 1) ) )
	NATL_FORCE_INLINE constexpr f64 tan4dpF64(const f64 value) noexcept {
		const f64 constrainedValue = fmodF64(value, pi);
		return bounded::tan4dpBoundedF64(constrainedValue);
	}


	//inverse functions//

	//asin quadratic appoximation
	//value bounds (-1, 1)
	//only accurate for small value
	//appoximation x * (( -0.704 * x^2 + 1) / (-0.81 * x^2 + 1))
	NATL_FORCE_INLINE constexpr f32 asinQuadF32(const f32 value) noexcept {
		const f32 x = value;
		const f32 x2 = x * x;
		return x * ((f32(-0.704) * x2) + 1) / ((f32(-0.81) * x2) + 1);
	}

	//asin quadratic appoximation
	//value bounds (-1, 1)
	//only accurate for small value
	//appoximation x * (( -0.704 * x^2 + 1) / (-0.81 * x^2 + 1))
	NATL_FORCE_INLINE constexpr f64 asinQuadF64(const f64 value) noexcept {
		const f64 x = value;
		const f64 x2 = x * x;
		return x * ((f64(-0.704) * x2) + 1) / ((f64(-0.81) * x2) + 1);
	}

	//acos quadratic appoximation
	//value bounds (-1, 1)
	//only accurate for small value
	NATL_FORCE_INLINE constexpr f32 acosQuadF32(const f32 value) noexcept {
		return -asinQuadF32(value) + f32(piOver2);
	}

	//acos quadratic appoximation
	//value bounds (-1, 1)
	//only accurate for small value
	NATL_FORCE_INLINE constexpr f64 acosQuadF64(const f64 value) noexcept {
		return -asinQuadF64(value) + piOver2;
	}

	//atan degree 4 polynomial appoximation
	//value bounds (-1, 1)
	//appoximation x / (1 + (0.3 * x^2) + (-0.0285 * x^4))
	NATL_FORCE_INLINE constexpr f32 atan4dpF32(const f32 value) noexcept {
		const f32 x = value;
		const f32 x2 = x * x;
		const f32 x4 = x2 * x2;
		return x / (f32(1) + (f32(0.3) * x2) + (f32(-0.0285) * x4));
	}

	//atan degree 4 polynomial appoximation
	//value bounds (-1, 1)
	//appoximation x / (1 + (0.3 * x^2) + (-0.0285 * x^4))
	NATL_FORCE_INLINE constexpr f64 atan4dpF64(const f64 value) noexcept {
		const f64 x = value;
		const f64 x2 = x * x;
		const f64 x4 = x2 * x2;
		return x / (1 + (0.3 * x2) + (-0.0285 * x4));
	}


	//sin
	NATL_FORCE_INLINE constexpr f32 basicSinF32(const f32 value) noexcept {
		return sinTaySerd4pF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicSinF64(const f64 value) noexcept {
		return sinTaySerd4pF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicSin(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicSinF32(value);
		} else {
			return basicSinF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 sinF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return Limits<f32>::quietNaN();
		} else {
			return basicSinF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 sinF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return Limits<f64>::quietNaN();
		} else {
			return basicSinF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType sin(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return sinF32(value);
		} else {
			return sinF64(value);
		}
	}

	//cos 
	NATL_FORCE_INLINE constexpr f32 basicCosF32(const f32 value) noexcept {
		return cosTaySerd4pF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicCosF64(const f64 value) noexcept {
		return cosTaySerd4pF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicCos(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicCosF32(value);
		} else {
			return basicCosF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 cosF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return Limits<f32>::quietNaN();
		} else {
			return basicCosF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 cosF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return Limits<f64>::quietNaN();
		} else {
			return basicCosF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType cos(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return cosF32(value);
		} else {
			return cosF64(value);
		}
	}

	//tan 
	NATL_FORCE_INLINE constexpr f32 basicTanF32(const f32 value) noexcept {
		return tan4dpF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicTanF64(const f64 value) noexcept {
		return tan4dpF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicTan(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicTanF32(value);
		} else {
			return basicTanF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 tanF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return Limits<f32>::quietNaN();
		} else {
			return basicTanF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 tanF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return Limits<f64>::quietNaN();
		} else {
			return basicTanF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType tan(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return tanF32(value);
		} else {
			return tanF64(value);
		}
	}

	//asin 
	NATL_FORCE_INLINE constexpr f32 basicAsinF32(const f32 value) noexcept {
		return asinQuadF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicAsinF64(const f64 value) noexcept {
		return asinQuadF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicAsin(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicAsinF32(value);
		} else {
			return basicAsinF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 asinF32(const f32 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF32(value) || isnanF32(value) || value < -1.0_f32 || value > 1.0_f32) {
			return Limits<f32>::quietNaN();
		} else {
			return basicAsinF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 asinF64(const f64 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF64(value) || isnanF64(value) || value < -1.0_f64 || value > 1.0_f64) {
			return Limits<f64>::quietNaN();
		} else {
			return basicAsinF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType asin(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return asinF32(value);
		} else {
			return asinF64(value);
		}
	}

	//acos 
	NATL_FORCE_INLINE constexpr f32 basicAcosF32(const f32 value) noexcept {
		return acosQuadF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicAcosF64(const f64 value) noexcept {
		return acosQuadF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicAcos(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicAcosF32(value);
		} else {
			return basicAcosF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 acosF32(const f32 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF32(value) || isnanF32(value) || value < -1.0_f32 || value > 1.0_f32) {
			return Limits<f32>::quietNaN();
		} else {
			return basicAcosF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 acosF64(const f64 value) noexcept {
		using namespace natl::literals;
		if (isInfinityF64(value) || isnanF64(value) || value < -1.0_f64 || value > 1.0_f64) {
			return Limits<f64>::quietNaN();
		} else {
			return basicAcosF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType acos(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return acosF32(value);
		} else {
			return acosF64(value);
		}
	}

	//atan 
	NATL_FORCE_INLINE constexpr f32 basicAtanF32(const f32 value) noexcept {
		return atan4dpF32(value);
	}
	NATL_FORCE_INLINE constexpr f64 basicAtanF64(const f64 value) noexcept {
		return atan4dpF64(value);
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType basicAtan(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return basicAtanF32(value);
		} else {
			return basicAtanF64(value);
		}
	}

	NATL_FORCE_INLINE constexpr f32 atanF32(const f32 value) noexcept {
		if (isInfinityF32(value) || isnanF32(value)) {
			return value;
		} else {
			return basicAtanF32(value);
		}
	}
	NATL_FORCE_INLINE constexpr f64 atanF64(const f64 value) noexcept {
		if (isInfinityF64(value) || isnanF64(value)) {
			return value;
		} else {
			return basicAtanF64(value);
		}
	}

	template<typename FloatType>
		requires(IsBuiltInFloatingPointC<Decay<FloatType>>)
	NATL_FORCE_INLINE constexpr FloatType atan(const FloatType value) noexcept {
		if constexpr (sizeof(FloatType) == sizeof(f32)) {
			return atanF32(value);
		} else {
			return atanF64(value);
		}
	}

}