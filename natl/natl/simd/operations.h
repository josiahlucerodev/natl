#pragma once

//own
#include "simdBase.h"

//interface
namespace natl::simd {
	//arithmetic//

	//add
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&add_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::add_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&add_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::add_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&add_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::add_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&add_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::add_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&add_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::add_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&add_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::add_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&add_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::add_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&add_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::add_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&add_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::add_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&add_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::add_f64;

	//mmasked add
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_add_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_add_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_add_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_add_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_add_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_add_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_add_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_add_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_add_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_add_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_f64;

	//mmasked src add
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_add_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_add_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_add_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_add_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_add_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_add_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_add_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_add_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_add_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_add_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_f64;

	//sub
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&sub_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::sub_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&sub_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::sub_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&sub_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::sub_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&sub_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::sub_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&sub_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::sub_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&sub_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::sub_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&sub_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::sub_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&sub_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::sub_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&sub_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::sub_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&sub_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::sub_f64;

	//mmasked sub
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_sub_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_sub_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_sub_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_sub_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_sub_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_sub_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_sub_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_sub_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_sub_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_sub_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_f64;

	//mmasked src sub
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_sub_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_sub_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_sub_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_sub_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_sub_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_sub_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_sub_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_sub_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_sub_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_sub_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_f64;

	//mul
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mul_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::mul_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mul_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::mul_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mul_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::mul_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mul_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::mul_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mul_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::mul_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mul_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::mul_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mul_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::mul_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mul_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::mul_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mul_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::mul_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mul_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::mul_f64;

	//mmasked mul
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_mul_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_mul_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_mul_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_mul_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_mul_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_mul_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_mul_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_mul_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_mul_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_mul_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_f64;

	//mmasked src mul
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_mul_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_mul_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_mul_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_mul_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_mul_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_mul_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_mul_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_mul_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_mul_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_mul_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_f64;

	//div
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&div_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::div_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&div_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::div_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&div_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::div_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&div_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::div_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&div_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::div_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&div_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::div_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&div_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::div_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&div_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::div_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&div_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::div_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&div_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::div_f64;

	//mmasked div
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_div_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_div_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_div_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_div_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_div_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_div_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_div_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_div_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_div_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_div_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_f64;

	//mmasked src div
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_div_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_div_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_div_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_div_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_div_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_div_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_div_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_div_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_div_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_div_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_f64;


	//add sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&add_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&add_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&add_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&add_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&add_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&add_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&add_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&add_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&add_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&add_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::add_sat_f64;

	//mmasked add sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_add_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_add_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_add_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_add_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_add_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_add_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_add_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_add_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_add_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_add_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_add_sat_f64;

	//mmasked src add sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_add_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_add_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_add_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_add_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_add_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_add_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_add_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_add_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_add_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_add_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_add_sat_f64;

	//sub sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&sub_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&sub_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&sub_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&sub_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&sub_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&sub_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&sub_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&sub_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&sub_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&sub_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::sub_sat_f64;

	//mmasked sub sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_sub_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_sub_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_sub_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_sub_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_sub_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_sub_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_sub_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_sub_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_sub_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_sub_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sub_sat_f64;

	//mmasked src sub sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_sub_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_sub_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_sub_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_sub_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_sub_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_sub_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_sub_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_sub_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_sub_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_sub_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sub_sat_f64;

	//mul sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mul_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mul_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mul_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mul_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mul_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mul_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mul_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mul_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mul_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mul_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::mul_sat_f64;

	//mmasked mul sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_mul_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_mul_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_mul_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_mul_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_mul_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_mul_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_mul_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_mul_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_mul_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_mul_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_mul_sat_f64;

	//mmasked src mul sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_mul_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_mul_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_mul_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_mul_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_mul_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_mul_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_mul_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_mul_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_mul_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_mul_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_mul_sat_f64;

	//div sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&div_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&div_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&div_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&div_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&div_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&div_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&div_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&div_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&div_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&div_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::div_sat_f64;

	//mmasked div sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_div_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_div_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_div_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_div_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_div_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_div_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_div_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_div_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_div_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_div_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_div_sat_f64;

	//mmasked src div sat
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_div_sat_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_div_sat_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_div_sat_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_div_sat_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_div_sat_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_div_sat_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_div_sat_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_div_sat_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_div_sat_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_div_sat_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_div_sat_f64;

	//remainder
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&remainder_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::remainder_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&remainder_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::remainder_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&remainder_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::remainder_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&remainder_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::remainder_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&remainder_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::remainder_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&remainder_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::remainder_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&remainder_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::remainder_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&remainder_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::remainder_ui64;

	//mmasked remainder
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_remainder_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_remainder_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_remainder_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_remainder_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_remainder_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_remainder_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_remainder_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_remainder_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_remainder_ui64;

	//mmasked src remainder
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_remainder_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_remainder_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_remainder_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_remainder_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_remainder_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_remainder_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_remainder_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_remainder_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_remainder_ui64;

	//fused mul add
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&fused_mul_add_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::fused_mul_add_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&fused_mul_add_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::fused_mul_add_f64;

	//mmasked fused mul add
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_fused_mul_add_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_fused_mul_add_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_fused_mul_add_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_fused_mul_add_f64;

	//mmasked src fused mul add
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_fused_mul_add_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF64<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_fused_mul_add_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_fused_mul_add_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_fused_mul_add_f64;

	//fused mul sub
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&fused_mul_sub_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::fused_mul_sub_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&fused_mul_sub_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::fused_mul_sub_f64;

	//mmasked fused mul sub
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_fused_mul_sub_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_fused_mul_sub_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_fused_mul_sub_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_fused_mul_sub_f64;

	//mmasked src fused mul sub
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_fused_mul_sub_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF64<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_fused_mul_sub_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_fused_mul_sub_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_fused_mul_sub_f64;

	//reciprocal
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&reciprocal_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reciprocal_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&reciprocal_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reciprocal_f64;

	//mmasked reciprocal
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_reciprocal_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reciprocal_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_reciprocal_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reciprocal_f64;

	//mmasked src reciprocal
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_reciprocal_f32) (SimdRegisterF32<Arch>, SimdRegisterF64<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reciprocal_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_reciprocal_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reciprocal_f64;

	//square root
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&square_root_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::square_root_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&square_root_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::square_root_f64;

	//mmasked square root
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_square_root_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_square_root_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_square_root_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_square_root_f64;

	//mmasked src square root
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_square_root_f32) (SimdRegisterF32<Arch>, SimdRegisterF64<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_square_root_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_square_root_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_square_root_f64;

	//reciprocal square root
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&reciprocal_square_root_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reciprocal_square_root_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&reciprocal_square_root_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reciprocal_square_root_f64;
	
	//mmasked reciprocal square root
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_reciprocal_square_root_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reciprocal_square_root_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_reciprocal_square_root_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reciprocal_square_root_f64;
	
	//mmasked src reciprocal square root
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_reciprocal_square_root_f32) (SimdRegisterF32<Arch>, SimdRegisterF64<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reciprocal_square_root_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_reciprocal_square_root_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reciprocal_square_root_f64;

	//math function//

	//max
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&max_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::max_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&max_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::max_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&max_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::max_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&max_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::max_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&max_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::max_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&max_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::max_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&max_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::max_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&max_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::max_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&max_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::max_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&max_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::max_f64;

	//mmasked max
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_max_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_max_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_max_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_max_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_max_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_max_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_max_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_max_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_max_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_max_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_max_f64;
		
	//mmasked src max
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_max_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_max_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_max_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_max_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_max_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_max_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_max_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_max_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_max_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_max_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_max_f64;

	//min 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&min_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::min_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&min_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::min_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&min_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::min_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&min_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::min_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&min_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::min_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&min_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::min_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&min_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::min_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&min_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::min_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&min_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::min_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&min_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::min_f64;

	//mmasked max
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_min_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_min_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_min_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_min_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_min_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_min_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_min_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_min_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_min_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_min_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_min_f64;

	//mmasked src max
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_min_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_min_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_min_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_min_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_min_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_min_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_min_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_min_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_min_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_min_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_min_f64;

	//abs 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&abs_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::abs_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&abs_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::abs_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&abs_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::abs_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&abs_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::abs_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&abs_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::abs_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&abs_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::abs_f64;

	//mmasked abs 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_abs_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_abs_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_abs_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_abs_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_abs_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_abs_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_abs_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_abs_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_abs_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_abs_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_abs_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_abs_f64;

	//mmasked src abs 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_abs_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_abs_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_abs_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_abs_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_abs_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_abs_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_abs_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_abs_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_abs_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_abs_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_abs_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_abs_f64;

	//negate 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&negate_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::negate_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&negate_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::negate_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&negate_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::negate_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&negate_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::negate_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&negate_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::negate_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&negate_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::negate_f64;

	//mmasked negate 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_negate_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_negate_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_negate_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_negate_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_negate_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_negate_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_negate_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_negate_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_negate_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_negate_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_negate_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_negate_f64;

	//mmasked src negate 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_negate_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_negate_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_negate_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_negate_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_negate_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_negate_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_negate_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_negate_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_negate_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_negate_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_negate_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_negate_f64;

	//round//

	//ceil
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&ceil_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::ceil_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&ceil_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::ceil_f64;

	//mmasked ceil
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_ceil_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_ceil_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_ceil_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_ceil_f64;

	//mmasked src ceil
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_ceil_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_ceil_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_ceil_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_ceil_f64;

	//floor
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&floor_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::floor_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&floor_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::floor_f64;

	//mmasked floor
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_floor_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_floor_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_floor_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_floor_f64;

	//mmasked floor
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_floor_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_floor_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_floor_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_floor_f64;

	//trunc
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&trunc_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::trunc_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&trunc_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::trunc_f64;

	//mmasked trunc
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_trunc_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_trunc_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_trunc_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_trunc_f64;

	//mmasked trunc
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_trunc_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_trunc_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_trunc_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_trunc_f64;

	//round
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&round_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::round_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&round_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::round_f64;

	//mmasked round
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_round_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_round_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_round_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_round_f64;

	//mmasked round
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_round_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_round_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_round_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_round_f64;

	//trig//
	
	//basic_sin
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_sin_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_sin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_sin_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_sin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_sin_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_sin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_sin_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_sin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_sin_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_sin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_sin_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_sin_f64;

	//sin
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&sin_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::sin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&sin_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::sin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_sin_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_sin_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_sin_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_sin_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sin_f64;

	//basic_cos
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_cos_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_cos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_cos_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_cos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_cos_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_cos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_cos_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_cos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_cos_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_cos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_cos_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_cos_f64;

	//cos
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&cos_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::cos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&cos_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::cos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_cos_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_cos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_cos_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_cos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_cos_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_cos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_cos_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_cos_f64;

	//basic_tan
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_tan_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_tan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_tan_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_tan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_tan_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_tan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_tan_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_tan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_tan_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_tan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_tan_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_tan_f64;

	//tan
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&tan_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::tan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&tan_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::tan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_tan_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_tan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_tan_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_tan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_tan_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_tan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_tan_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_tan_f64;

	//basic_asin
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_asin_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_asin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_asin_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_asin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_asin_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_asin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_asin_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_asin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_asin_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_asin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_asin_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_asin_f64;

	//asin
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&asin_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::asin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&asin_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::asin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_asin_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_asin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_asin_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_asin_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_asin_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_asin_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_asin_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_asin_f64;

	//basic_acos
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_acos_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_acos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_acos_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_acos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_acos_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_acos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_acos_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_acos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_acos_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_acos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_acos_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_acos_f64;

	//acos
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&acos_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::acos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&acos_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::acos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_acos_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_acos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_acos_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_acos_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_acos_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_acos_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_acos_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_acos_f64;

	//basic_atan
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_atan_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_atan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_atan_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_atan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_atan_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_atan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_atan_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_atan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_atan_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_atan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_atan_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_atan_f64;

	//atan
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&atan_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::atan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&atan_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::atan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_atan_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_atan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_atan_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_atan_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_atan_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_atan_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_atan_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_atan_f64;

	//basic_sinh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_sinh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_sinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_sinh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_sinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_sinh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_sinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_sinh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_sinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_sinh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_sinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_sinh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_sinh_f64;

	//sinh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&sinh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::sinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&sinh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::sinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_sinh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_sinh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_sinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_sinh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_sinh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_sinh_f64;

	//basic_cosh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_cosh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_cosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_cosh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_cosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_cosh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_cosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_cosh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_cosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_cosh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_cosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_cosh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_cosh_f64;

	//cosh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&cosh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::cosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&cosh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::cosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_cosh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_cosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_cosh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_cosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_cosh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_cosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_cosh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_cosh_f64;

	//basic_tanh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_tanh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_tanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_tanh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_tanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_tanh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_tanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_tanh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_tanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_tanh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_tanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_tanh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_tanh_f64;

	//tanh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&tanh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::tanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&tanh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::tanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_tanh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_tanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_tanh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_tanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_tanh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_tanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_tanh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_tanh_f64;

	//basic_asinh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_asinh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_asinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_asinh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_asinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_asinh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_asinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_asinh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_asinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_asinh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_asinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_asinh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_asinh_f64;

	//asinh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&asinh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::asinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&asinh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::asinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_asinh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_asinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_asinh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_asinh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_asinh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_asinh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_asinh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_asinh_f64;

	//basic_acosh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_acosh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_acosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_acosh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_acosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_acosh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_acosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_acosh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_acosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_acosh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_acosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_acosh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_acosh_f64;

	//acosh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&acosh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::acosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&acosh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::acosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_acosh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_acosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_acosh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_acosh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_acosh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_acosh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_acosh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_acosh_f64;

	//basic_atanh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&basic_atanh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::basic_atanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&basic_atanh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::basic_atanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_basic_atanh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_atanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_basic_atanh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_basic_atanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_basic_atanh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_atanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_basic_atanh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_basic_atanh_f64;

	//atanh
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&atanh_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::atanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&atanh_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::atanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_atanh_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_atanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_atanh_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_atanh_f64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_atanh_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_atanh_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_atanh_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_atanh_f64;

	//bitwise// 
	
	///bitwise_shift_left_c
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&bitwise_shift_left_c_i8) (SimdRegisterI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&bitwise_shift_left_c_i16) (SimdRegisterI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&bitwise_shift_left_c_i32) (SimdRegisterI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&bitwise_shift_left_c_i64) (SimdRegisterI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&bitwise_shift_left_c_ui8) (SimdRegisterUI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&bitwise_shift_left_c_ui16) (SimdRegisterUI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&bitwise_shift_left_c_ui32) (SimdRegisterUI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&bitwise_shift_left_c_ui64) (SimdRegisterUI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&bitwise_shift_left_c_f32) (SimdRegisterF32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&bitwise_shift_left_c_f64) (SimdRegisterF64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_left_c_f64;

	//mmasked bitwise_shift_left_c
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_shift_left_c_i8) (SimdRegisterI8<Arch>, const Size, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_shift_left_c_i16) (SimdRegisterI16<Arch>, const Size, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_shift_left_c_i32) (SimdRegisterI32<Arch>, const Size, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_shift_left_c_i64) (SimdRegisterI64<Arch>, const Size, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_shift_left_c_ui8) (SimdRegisterUI8<Arch>, const Size, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_shift_left_c_ui16) (SimdRegisterUI16<Arch>, const Size, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_shift_left_c_ui32) (SimdRegisterUI32<Arch>, const Size, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_shift_left_c_ui64) (SimdRegisterUI64<Arch>, const Size, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_shift_left_c_f32) (SimdRegisterF32<Arch>, const Size, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_shift_left_c_f64) (SimdRegisterF64<Arch>, const Size, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_c_f64;

	//mmasked src bitwise_shift_left_c
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_shift_left_c_i8) (SimdRegisterI8<Arch>, const Size, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_shift_left_c_i16) (SimdRegisterI16<Arch>, const Size, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_shift_left_c_i32) (SimdRegisterI32<Arch>, const Size, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_shift_left_c_i64) (SimdRegisterI64<Arch>, const Size, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_shift_left_c_ui8) (SimdRegisterUI8<Arch>, const Size, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_shift_left_c_ui16) (SimdRegisterUI16<Arch>, const Size, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_shift_left_c_ui32) (SimdRegisterUI32<Arch>, const Size, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_shift_left_c_ui64) (SimdRegisterUI64<Arch>, const Size, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_shift_left_c_f32) (SimdRegisterF32<Arch>, const Size, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_shift_left_c_f64) (SimdRegisterF64<Arch>, const Size, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_c_f64;

	//bitwise_shift_right_c
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&bitwise_shift_right_c_i8) (SimdRegisterI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&bitwise_shift_right_c_i16) (SimdRegisterI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&bitwise_shift_right_c_i32) (SimdRegisterI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&bitwise_shift_right_c_i64) (SimdRegisterI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&bitwise_shift_right_c_ui8) (SimdRegisterUI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&bitwise_shift_right_c_ui16) (SimdRegisterUI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&bitwise_shift_right_c_ui32) (SimdRegisterUI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&bitwise_shift_right_c_ui64) (SimdRegisterUI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&bitwise_shift_right_c_f32) (SimdRegisterF32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&bitwise_shift_right_c_f64) (SimdRegisterF64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::bitwise_shift_right_c_f64;

	//mmasked bitwise_shift_right_c
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_shift_right_c_i8) (SimdRegisterI8<Arch>, const Size, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_shift_right_c_i16) (SimdRegisterI16<Arch>, const Size, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_shift_right_c_i32) (SimdRegisterI32<Arch>, const Size, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_shift_right_c_i64) (SimdRegisterI64<Arch>, const Size, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_shift_right_c_ui8) (SimdRegisterUI8<Arch>, const Size, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_shift_right_c_ui16) (SimdRegisterUI16<Arch>, const Size, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_shift_right_c_ui32) (SimdRegisterUI32<Arch>, const Size, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_shift_right_c_ui64) (SimdRegisterUI64<Arch>, const Size, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_shift_right_c_f32) (SimdRegisterF32<Arch>, const Size, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_shift_right_c_f64) (SimdRegisterF64<Arch>, const Size, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_c_f64;

	//mmasked src bitwise_shift_right_c
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_shift_right_c_i8) (SimdRegisterI8<Arch>, const Size, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_shift_right_c_i16) (SimdRegisterI16<Arch>, const Size, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_shift_right_c_i32) (SimdRegisterI32<Arch>, const Size, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_shift_right_c_i64) (SimdRegisterI64<Arch>, const Size, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_shift_right_c_ui8) (SimdRegisterUI8<Arch>, const Size, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_shift_right_c_ui16) (SimdRegisterUI16<Arch>, const Size, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_shift_right_c_ui32) (SimdRegisterUI32<Arch>, const Size, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_shift_right_c_ui64) (SimdRegisterUI64<Arch>, const Size, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_shift_right_c_f32) (SimdRegisterF32<Arch>, const Size, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_shift_right_c_f64) (SimdRegisterF64<Arch>, const Size, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_c_f64;

	//mmasked bitwise_shift_left
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_shift_left_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_shift_left_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_shift_left_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_shift_left_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_shift_left_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_shift_left_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_shift_left_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_shift_left_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_shift_left_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_shift_left_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_left_f64;

	//mmasked src bitwise_shift_left
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_shift_left_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_shift_left_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_shift_left_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_shift_left_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_shift_left_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_shift_left_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_shift_left_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_shift_left_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_shift_left_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_shift_left_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_left_f64;

	//mmasked bitwise_shift_right
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_shift_right_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_shift_right_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_shift_right_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_shift_right_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_shift_right_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_shift_right_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_shift_right_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_shift_right_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_shift_right_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_shift_right_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_shift_right_f64;

	//mmasked src bitwise_shift_right
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_shift_right_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_shift_right_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_shift_right_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_shift_right_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_shift_right_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_shift_right_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_shift_right_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_shift_right_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_shift_right_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_shift_right_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_shift_right_f64;

	//mmasked bitwise_and
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_and_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_and_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_and_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_and_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_and_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_and_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_and_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_and_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_and_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_and_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_and_f64;

	//mmasked src bitwise_and
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_and_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_and_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_and_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_and_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_and_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_and_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_and_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_and_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_and_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_and_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_and_f64;

	//mmasked bitwise_or
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_or_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_or_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_or_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_or_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_or_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_or_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_or_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_or_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_or_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_or_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_or_f64;

	//mmasked src bitwise_or
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_or_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_or_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_or_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_or_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_or_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_or_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_or_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_or_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_or_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_or_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_or_f64;

	//mmasked bitwise_xor
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_xor_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_xor_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_xor_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_xor_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_xor_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_xor_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_xor_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_xor_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_xor_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_xor_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_xor_f64;

	//mmasked src bitwise_xor
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_xor_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_xor_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_xor_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_xor_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_xor_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_xor_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_xor_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_xor_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_xor_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_xor_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_xor_f64;

	//bitwise_not
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&bitwise_not_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&bitwise_not_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&bitwise_not_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&bitwise_not_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&bitwise_not_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&bitwise_not_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&bitwise_not_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&bitwise_not_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&bitwise_not_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&bitwise_not_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::bitwise_not_f64;

	//mmasked bitwise_not
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_bitwise_not_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_bitwise_not_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_bitwise_not_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_bitwise_not_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_bitwise_not_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_bitwise_not_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_bitwise_not_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_bitwise_not_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_bitwise_not_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_bitwise_not_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitwise_not_f64;

	//mmasked src bitwise_not
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_bitwise_not_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_bitwise_not_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_bitwise_not_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_bitwise_not_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_bitwise_not_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_bitwise_not_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_bitwise_not_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_bitwise_not_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_bitwise_not_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_bitwise_not_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitwise_not_f64;

	//popcount
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&popcount_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::popcount_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&popcount_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::popcount_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&popcount_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::popcount_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&popcount_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::popcount_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&popcount_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::popcount_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&popcount_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::popcount_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&popcount_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::popcount_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&popcount_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::popcount_ui64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&popcount_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::popcount_f32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&popcount_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::popcount_f64;

	//mmasked popcount
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_popcount_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_popcount_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_popcount_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_popcount_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_popcount_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_popcount_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_popcount_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_popcount_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_ui64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_popcount_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_f32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_popcount_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_popcount_f64;

	//mmasked src popcount
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_popcount_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_popcount_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_popcount_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_popcount_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_popcount_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_popcount_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_popcount_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_popcount_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_ui64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_popcount_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_f32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_popcount_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_popcount_f64;

	//bitscan forward
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&bitscan_forward_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&bitscan_forward_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&bitscan_forward_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&bitscan_forward_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&bitscan_forward_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&bitscan_forward_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&bitscan_forward_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&bitscan_forward_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&bitscan_forward_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&bitscan_forward_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_forward_f64;

	//mmasked bitscan forward
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mmasked_bitscan_forward_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>&, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mmasked_bitscan_forward_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>&, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mmasked_bitscan_forward_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>&, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mmasked_bitscan_forward_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>&, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mmasked_bitscan_forward_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>&, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mmasked_bitscan_forward_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>&, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mmasked_bitscan_forward_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>&, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mmasked_bitscan_forward_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>&, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mmasked_bitscan_forward_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>&, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mmasked_bitscan_forward_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>&, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_forward_f64;

	//mmasked src bitscan forward
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mmasked_src_bitscan_forward_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>&, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mmasked_src_bitscan_forward_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>&, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mmasked_src_bitscan_forward_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>&, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mmasked_src_bitscan_forward_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>&, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mmasked_src_bitscan_forward_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>&, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mmasked_src_bitscan_forward_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>&, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mmasked_src_bitscan_forward_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>&, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mmasked_src_bitscan_forward_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>&, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mmasked_src_bitscan_forward_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>&, SimdRegisterUI32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mmasked_src_bitscan_forward_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>&, SimdRegisterUI64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_forward_f64;

	//bitscan backward
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&bitscan_backward_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&bitscan_backward_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&bitscan_backward_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&bitscan_backward_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&bitscan_backward_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&bitscan_backward_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&bitscan_backward_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&bitscan_backward_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&bitscan_backward_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&bitscan_backward_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>&) noexcept = ArchSimdOp<Arch>::bitscan_backward_f64;

	//mmasked bitscan backward
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mmasked_bitscan_backward_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>&, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mmasked_bitscan_backward_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>&, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mmasked_bitscan_backward_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>&, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mmasked_bitscan_backward_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>&, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mmasked_bitscan_backward_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>&, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mmasked_bitscan_backward_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>&, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mmasked_bitscan_backward_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>&, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mmasked_bitscan_backward_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>&, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mmasked_bitscan_backward_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>&, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mmasked_bitscan_backward_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>&, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_bitscan_backward_f64;

	//mmasked src bitscan backward
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mmasked_src_bitscan_backward_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>&, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mmasked_src_bitscan_backward_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>&, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mmasked_src_bitscan_backward_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>&, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mmasked_src_bitscan_backward_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>&, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mmasked_src_bitscan_backward_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>&, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mmasked_src_bitscan_backward_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>&, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mmasked_src_bitscan_backward_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>&, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mmasked_src_bitscan_backward_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>&, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mmasked_src_bitscan_backward_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>&, SimdRegisterUI32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mmasked_src_bitscan_backward_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>&, SimdRegisterUI64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_bitscan_backward_f64;

	//reduce//

	//reduce_min
	template<class Arch> constexpr inline i8(&reduce_min_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_i8;
	template<class Arch> constexpr inline i16(&reduce_min_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_i16;
	template<class Arch> constexpr inline i32(&reduce_min_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_i32;
	template<class Arch> constexpr inline i64(&reduce_min_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_i64;
	template<class Arch> constexpr inline ui8(&reduce_min_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_ui8;
	template<class Arch> constexpr inline ui16(&reduce_min_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_ui16;
	template<class Arch> constexpr inline ui32(&reduce_min_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_ui32;
	template<class Arch> constexpr inline ui64(&reduce_min_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_ui64;
	template<class Arch> constexpr inline f32(&reduce_min_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_f32;
	template<class Arch> constexpr inline f64(&reduce_min_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_min_f64;

	//mmasked reduce_min
	template<class Arch> constexpr inline i8(&mmasked_reduce_min_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_i8;
	template<class Arch> constexpr inline i16(&mmasked_reduce_min_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_i16;
	template<class Arch> constexpr inline i32(&mmasked_reduce_min_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_i32;
	template<class Arch> constexpr inline i64(&mmasked_reduce_min_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_i64;
	template<class Arch> constexpr inline ui8(&mmasked_reduce_min_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_reduce_min_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_reduce_min_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_reduce_min_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_ui64;
	template<class Arch> constexpr inline f32(&mmasked_reduce_min_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_f32;
	template<class Arch> constexpr inline f64(&mmasked_reduce_min_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_min_f64;

	//mmasked src reduce_min
	template<class Arch> constexpr inline i8(&mmasked_src_reduce_min_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_i8;
	template<class Arch> constexpr inline i16(&mmasked_src_reduce_min_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_i16;
	template<class Arch> constexpr inline i32(&mmasked_src_reduce_min_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_i32;
	template<class Arch> constexpr inline i64(&mmasked_src_reduce_min_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_i64;
	template<class Arch> constexpr inline ui8(&mmasked_src_reduce_min_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_src_reduce_min_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_src_reduce_min_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_src_reduce_min_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_ui64;
	template<class Arch> constexpr inline f32(&mmasked_src_reduce_min_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_f32;
	template<class Arch> constexpr inline f64(&mmasked_src_reduce_min_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_min_f64;

	//reduce_max
	template<class Arch> constexpr inline i8(&reduce_max_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_i8;
	template<class Arch> constexpr inline i16(&reduce_max_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_i16;
	template<class Arch> constexpr inline i32(&reduce_max_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_i32;
	template<class Arch> constexpr inline i64(&reduce_max_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_i64;
	template<class Arch> constexpr inline ui8(&reduce_max_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_ui8;
	template<class Arch> constexpr inline ui16(&reduce_max_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_ui16;
	template<class Arch> constexpr inline ui32(&reduce_max_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_ui32;
	template<class Arch> constexpr inline ui64(&reduce_max_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_ui64;
	template<class Arch> constexpr inline f32(&reduce_max_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_f32;
	template<class Arch> constexpr inline f64(&reduce_max_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_max_f64;

	//mmasked reduce_max
	template<class Arch> constexpr inline i8(&mmasked_reduce_max_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_i8;
	template<class Arch> constexpr inline i16(&mmasked_reduce_max_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_i16;
	template<class Arch> constexpr inline i32(&mmasked_reduce_max_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_i32;
	template<class Arch> constexpr inline i64(&mmasked_reduce_max_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_i64;
	template<class Arch> constexpr inline ui8(&mmasked_reduce_max_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_reduce_max_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_reduce_max_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_reduce_max_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_ui64;
	template<class Arch> constexpr inline f32(&mmasked_reduce_max_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_f32;
	template<class Arch> constexpr inline f64(&mmasked_reduce_max_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_max_f64;

	//mmasked src reduce_max
	template<class Arch> constexpr inline i8(&mmasked_src_reduce_max_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_i8;
	template<class Arch> constexpr inline i16(&mmasked_src_reduce_max_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_i16;
	template<class Arch> constexpr inline i32(&mmasked_src_reduce_max_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_i32;
	template<class Arch> constexpr inline i64(&mmasked_src_reduce_max_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_i64;
	template<class Arch> constexpr inline ui8(&mmasked_src_reduce_max_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_src_reduce_max_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_src_reduce_max_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_src_reduce_max_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_ui64;
	template<class Arch> constexpr inline f32(&mmasked_src_reduce_max_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_f32;
	template<class Arch> constexpr inline f64(&mmasked_src_reduce_max_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_max_f64;

	//reduce_add
	template<class Arch> constexpr inline i8(&reduce_add_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_i8;
	template<class Arch> constexpr inline i16(&reduce_add_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_i16;
	template<class Arch> constexpr inline i32(&reduce_add_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_i32;
	template<class Arch> constexpr inline i64(&reduce_add_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_i64;
	template<class Arch> constexpr inline ui8(&reduce_add_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_ui8;
	template<class Arch> constexpr inline ui16(&reduce_add_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_ui16;
	template<class Arch> constexpr inline ui32(&reduce_add_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_ui32;
	template<class Arch> constexpr inline ui64(&reduce_add_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_ui64;
	template<class Arch> constexpr inline f32(&reduce_add_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_f32;
	template<class Arch> constexpr inline f64(&reduce_add_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_add_f64;

	//mmasked reduce_add
	template<class Arch> constexpr inline i8(&mmasked_reduce_add_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_i8;
	template<class Arch> constexpr inline i16(&mmasked_reduce_add_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_i16;
	template<class Arch> constexpr inline i32(&mmasked_reduce_add_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_i32;
	template<class Arch> constexpr inline i64(&mmasked_reduce_add_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_i64;
	template<class Arch> constexpr inline ui8(&mmasked_reduce_add_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_reduce_add_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_reduce_add_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_reduce_add_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_ui64;
	template<class Arch> constexpr inline f32(&mmasked_reduce_add_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_f32;
	template<class Arch> constexpr inline f64(&mmasked_reduce_add_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_add_f64;

	//mmasked src reduce_add
	template<class Arch> constexpr inline i8(&mmasked_src_reduce_add_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_i8;
	template<class Arch> constexpr inline i16(&mmasked_src_reduce_add_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_i16;
	template<class Arch> constexpr inline i32(&mmasked_src_reduce_add_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_i32;
	template<class Arch> constexpr inline i64(&mmasked_src_reduce_add_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_i64;
	template<class Arch> constexpr inline ui8(&mmasked_src_reduce_add_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_src_reduce_add_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_src_reduce_add_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_src_reduce_add_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_ui64;
	template<class Arch> constexpr inline f32(&mmasked_src_reduce_add_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_f32;
	template<class Arch> constexpr inline f64(&mmasked_src_reduce_add_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_add_f64;

	//reduce_mul
	template<class Arch> constexpr inline i8(&reduce_mul_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_i8;
	template<class Arch> constexpr inline i16(&reduce_mul_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_i16;
	template<class Arch> constexpr inline i32(&reduce_mul_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_i32;
	template<class Arch> constexpr inline i64(&reduce_mul_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_i64;
	template<class Arch> constexpr inline ui8(&reduce_mul_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_ui8;
	template<class Arch> constexpr inline ui16(&reduce_mul_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_ui16;
	template<class Arch> constexpr inline ui32(&reduce_mul_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_ui32;
	template<class Arch> constexpr inline ui64(&reduce_mul_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_ui64;
	template<class Arch> constexpr inline f32(&reduce_mul_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_f32;
	template<class Arch> constexpr inline f64(&reduce_mul_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_mul_f64;

	//mmasked reduce_mul
	template<class Arch> constexpr inline i8(&mmasked_reduce_mul_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_i8;
	template<class Arch> constexpr inline i16(&mmasked_reduce_mul_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_i16;
	template<class Arch> constexpr inline i32(&mmasked_reduce_mul_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_i32;
	template<class Arch> constexpr inline i64(&mmasked_reduce_mul_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_i64;
	template<class Arch> constexpr inline ui8(&mmasked_reduce_mul_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_reduce_mul_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_reduce_mul_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_reduce_mul_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_ui64;
	template<class Arch> constexpr inline f32(&mmasked_reduce_mul_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_f32;
	template<class Arch> constexpr inline f64(&mmasked_reduce_mul_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_mul_f64;

	//mmasked src reduce_mul
	template<class Arch> constexpr inline i8(&mmasked_src_reduce_mul_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_i8;
	template<class Arch> constexpr inline i16(&mmasked_src_reduce_mul_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_i16;
	template<class Arch> constexpr inline i32(&mmasked_src_reduce_mul_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_i32;
	template<class Arch> constexpr inline i64(&mmasked_src_reduce_mul_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_i64;
	template<class Arch> constexpr inline ui8(&mmasked_src_reduce_mul_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_src_reduce_mul_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_src_reduce_mul_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_src_reduce_mul_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_ui64;
	template<class Arch> constexpr inline f32(&mmasked_src_reduce_mul_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_f32;
	template<class Arch> constexpr inline f64(&mmasked_src_reduce_mul_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_mul_f64;

	//reduce_and
	template<class Arch> constexpr inline i8(&reduce_and_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_i8;
	template<class Arch> constexpr inline i16(&reduce_and_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_i16;
	template<class Arch> constexpr inline i32(&reduce_and_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_i32;
	template<class Arch> constexpr inline i64(&reduce_and_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_i64;
	template<class Arch> constexpr inline ui8(&reduce_and_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_ui8;
	template<class Arch> constexpr inline ui16(&reduce_and_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_ui16;
	template<class Arch> constexpr inline ui32(&reduce_and_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_ui32;
	template<class Arch> constexpr inline ui64(&reduce_and_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_ui64;
	template<class Arch> constexpr inline f32(&reduce_and_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_f32;
	template<class Arch> constexpr inline f64(&reduce_and_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_and_f64;

	//mmasked reduce_and
	template<class Arch> constexpr inline i8(&mmasked_reduce_and_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_i8;
	template<class Arch> constexpr inline i16(&mmasked_reduce_and_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_i16;
	template<class Arch> constexpr inline i32(&mmasked_reduce_and_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_i32;
	template<class Arch> constexpr inline i64(&mmasked_reduce_and_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_i64;
	template<class Arch> constexpr inline ui8(&mmasked_reduce_and_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_reduce_and_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_reduce_and_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_reduce_and_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_ui64;
	template<class Arch> constexpr inline f32(&mmasked_reduce_and_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_f32;
	template<class Arch> constexpr inline f64(&mmasked_reduce_and_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_and_f64;

	//mmasked src reduce_and
	template<class Arch> constexpr inline i8(&mmasked_src_reduce_and_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_i8;
	template<class Arch> constexpr inline i16(&mmasked_src_reduce_and_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_i16;
	template<class Arch> constexpr inline i32(&mmasked_src_reduce_and_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_i32;
	template<class Arch> constexpr inline i64(&mmasked_src_reduce_and_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_i64;
	template<class Arch> constexpr inline ui8(&mmasked_src_reduce_and_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_src_reduce_and_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_src_reduce_and_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_src_reduce_and_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_ui64;
	template<class Arch> constexpr inline f32(&mmasked_src_reduce_and_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_f32;
	template<class Arch> constexpr inline f64(&mmasked_src_reduce_and_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_and_f64;

	//reduce_or
	template<class Arch> constexpr inline i8(&reduce_or_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_i8;
	template<class Arch> constexpr inline i16(&reduce_or_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_i16;
	template<class Arch> constexpr inline i32(&reduce_or_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_i32;
	template<class Arch> constexpr inline i64(&reduce_or_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_i64;
	template<class Arch> constexpr inline ui8(&reduce_or_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_ui8;
	template<class Arch> constexpr inline ui16(&reduce_or_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_ui16;
	template<class Arch> constexpr inline ui32(&reduce_or_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_ui32;
	template<class Arch> constexpr inline ui64(&reduce_or_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_ui64;
	template<class Arch> constexpr inline f32(&reduce_or_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_f32;
	template<class Arch> constexpr inline f64(&reduce_or_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_or_f64;

	//mmasked reduce_or
	template<class Arch> constexpr inline i8(&mmasked_reduce_or_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_i8;
	template<class Arch> constexpr inline i16(&mmasked_reduce_or_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_i16;
	template<class Arch> constexpr inline i32(&mmasked_reduce_or_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_i32;
	template<class Arch> constexpr inline i64(&mmasked_reduce_or_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_i64;
	template<class Arch> constexpr inline ui8(&mmasked_reduce_or_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_reduce_or_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_reduce_or_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_reduce_or_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_ui64;
	template<class Arch> constexpr inline f32(&mmasked_reduce_or_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_f32;
	template<class Arch> constexpr inline f64(&mmasked_reduce_or_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_or_f64;

	//mmasked src reduce_or
	template<class Arch> constexpr inline i8(&mmasked_src_reduce_or_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_i8;
	template<class Arch> constexpr inline i16(&mmasked_src_reduce_or_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_i16;
	template<class Arch> constexpr inline i32(&mmasked_src_reduce_or_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_i32;
	template<class Arch> constexpr inline i64(&mmasked_src_reduce_or_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_i64;
	template<class Arch> constexpr inline ui8(&mmasked_src_reduce_or_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_src_reduce_or_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_src_reduce_or_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_src_reduce_or_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_ui64;
	template<class Arch> constexpr inline f32(&mmasked_src_reduce_or_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_f32;
	template<class Arch> constexpr inline f64(&mmasked_src_reduce_or_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_or_f64;

	//reduce_xor
	template<class Arch> constexpr inline i8(&reduce_xor_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_i8;
	template<class Arch> constexpr inline i16(&reduce_xor_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_i16;
	template<class Arch> constexpr inline i32(&reduce_xor_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_i32;
	template<class Arch> constexpr inline i64(&reduce_xor_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_i64;
	template<class Arch> constexpr inline ui8(&reduce_xor_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_ui8;
	template<class Arch> constexpr inline ui16(&reduce_xor_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_ui16;
	template<class Arch> constexpr inline ui32(&reduce_xor_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_ui32;
	template<class Arch> constexpr inline ui64(&reduce_xor_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_ui64;
	template<class Arch> constexpr inline f32(&reduce_xor_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_f32;
	template<class Arch> constexpr inline f64(&reduce_xor_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::reduce_xor_f64;

	//mmasked reduce_xor
	template<class Arch> constexpr inline i8(&mmasked_reduce_xor_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_i8;
	template<class Arch> constexpr inline i16(&mmasked_reduce_xor_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_i16;
	template<class Arch> constexpr inline i32(&mmasked_reduce_xor_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_i32;
	template<class Arch> constexpr inline i64(&mmasked_reduce_xor_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_i64;
	template<class Arch> constexpr inline ui8(&mmasked_reduce_xor_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_reduce_xor_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_reduce_xor_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_reduce_xor_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_ui64;
	template<class Arch> constexpr inline f32(&mmasked_reduce_xor_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_f32;
	template<class Arch> constexpr inline f64(&mmasked_reduce_xor_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_reduce_xor_f64;

	//mmasked src reduce_xor
	template<class Arch> constexpr inline i8(&mmasked_src_reduce_xor_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_i8;
	template<class Arch> constexpr inline i16(&mmasked_src_reduce_xor_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_i16;
	template<class Arch> constexpr inline i32(&mmasked_src_reduce_xor_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_i32;
	template<class Arch> constexpr inline i64(&mmasked_src_reduce_xor_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_i64;
	template<class Arch> constexpr inline ui8(&mmasked_src_reduce_xor_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_ui8;
	template<class Arch> constexpr inline ui16(&mmasked_src_reduce_xor_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_ui16;
	template<class Arch> constexpr inline ui32(&mmasked_src_reduce_xor_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_ui32;
	template<class Arch> constexpr inline ui64(&mmasked_src_reduce_xor_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_ui64;
	template<class Arch> constexpr inline f32(&mmasked_src_reduce_xor_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_f32;
	template<class Arch> constexpr inline f64(&mmasked_src_reduce_xor_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_reduce_xor_f64;

	//compare//

	//compare equal mmask
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&compare_equal_mmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&compare_equal_mmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&compare_equal_mmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&compare_equal_mmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&compare_equal_mmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&compare_equal_mmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&compare_equal_mmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&compare_equal_mmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&compare_equal_mmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&compare_equal_mmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_mmask_f64;

	//compare not equal mmask
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&compare_not_equal_mmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&compare_not_equal_mmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&compare_not_equal_mmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&compare_not_equal_mmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&compare_not_equal_mmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&compare_not_equal_mmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&compare_not_equal_mmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&compare_not_equal_mmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&compare_not_equal_mmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&compare_not_equal_mmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_mmask_f64;

	//compare less than mmask
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&compare_less_than_mmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&compare_less_than_mmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&compare_less_than_mmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&compare_less_than_mmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&compare_less_than_mmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&compare_less_than_mmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&compare_less_than_mmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&compare_less_than_mmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&compare_less_than_mmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&compare_less_than_mmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_mmask_f64;

	//compare greater than mmask
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&compare_greater_than_mmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&compare_greater_than_mmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&compare_greater_than_mmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&compare_greater_than_mmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&compare_greater_than_mmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&compare_greater_than_mmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&compare_greater_than_mmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&compare_greater_than_mmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&compare_greater_than_mmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&compare_greater_than_mmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_mmask_f64;

	//compare less than or equal mmask
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&compare_less_than_or_equal_mmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&compare_less_than_or_equal_mmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&compare_less_than_or_equal_mmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&compare_less_than_or_equal_mmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&compare_less_than_or_equal_mmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&compare_less_than_or_equal_mmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&compare_less_than_or_equal_mmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&compare_less_than_or_equal_mmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&compare_less_than_or_equal_mmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&compare_less_than_or_equal_mmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_mmask_f64;

	//compare greater than or equal mmask
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&compare_greater_than_or_equal_mmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&compare_greater_than_or_equal_mmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&compare_greater_than_or_equal_mmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&compare_greater_than_or_equal_mmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&compare_greater_than_or_equal_mmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&compare_greater_than_or_equal_mmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&compare_greater_than_or_equal_mmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&compare_greater_than_or_equal_mmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&compare_greater_than_or_equal_mmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&compare_greater_than_or_equal_mmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_mmask_f64;

	//compare equal rmask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&compare_equal_rmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&compare_equal_rmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&compare_equal_rmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&compare_equal_rmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&compare_equal_rmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&compare_equal_rmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&compare_equal_rmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&compare_equal_rmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&compare_equal_rmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&compare_equal_rmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_equal_rmask_f64;

	//compare not equal rmask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&compare_not_equal_rmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&compare_not_equal_rmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&compare_not_equal_rmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&compare_not_equal_rmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&compare_not_equal_rmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&compare_not_equal_rmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&compare_not_equal_rmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&compare_not_equal_rmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&compare_not_equal_rmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&compare_not_equal_rmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_not_equal_rmask_f64;

	//compare less than rmask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&compare_less_than_rmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&compare_less_than_rmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&compare_less_than_rmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&compare_less_than_rmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&compare_less_than_rmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&compare_less_than_rmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&compare_less_than_rmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&compare_less_than_rmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&compare_less_than_rmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&compare_less_than_rmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_rmask_f64;

	//compare greater than rmask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&compare_greater_than_rmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&compare_greater_than_rmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&compare_greater_than_rmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&compare_greater_than_rmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&compare_greater_than_rmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&compare_greater_than_rmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&compare_greater_than_rmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&compare_greater_than_rmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&compare_greater_than_rmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&compare_greater_than_rmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_rmask_f64;

	//compare less than or equal rmask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&compare_less_than_or_equal_rmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&compare_less_than_or_equal_rmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&compare_less_than_or_equal_rmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&compare_less_than_or_equal_rmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&compare_less_than_or_equal_rmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&compare_less_than_or_equal_rmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&compare_less_than_or_equal_rmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&compare_less_than_or_equal_rmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&compare_less_than_or_equal_rmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&compare_less_than_or_equal_rmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_less_than_or_equal_rmask_f64;

	//compare greater than or equal rmask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&compare_greater_than_or_equal_rmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&compare_greater_than_or_equal_rmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&compare_greater_than_or_equal_rmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&compare_greater_than_or_equal_rmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&compare_greater_than_or_equal_rmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&compare_greater_than_or_equal_rmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&compare_greater_than_or_equal_rmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&compare_greater_than_or_equal_rmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&compare_greater_than_or_equal_rmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&compare_greater_than_or_equal_rmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::compare_greater_than_or_equal_rmask_f64;

	//load//

	//load
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&load_i8) (const i8*) noexcept = ArchSimdOp<Arch>::load_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&load_i16) (const i16*) noexcept = ArchSimdOp<Arch>::load_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&load_i32) (const i32*) noexcept = ArchSimdOp<Arch>::load_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&load_i64) (const i64*) noexcept = ArchSimdOp<Arch>::load_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&load_ui8) (const ui8*) noexcept = ArchSimdOp<Arch>::load_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&load_ui16) (const ui16*) noexcept = ArchSimdOp<Arch>::load_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&load_ui32) (const ui32*) noexcept = ArchSimdOp<Arch>::load_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&load_ui64) (const ui64*) noexcept = ArchSimdOp<Arch>::load_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&load_f32) (const f32*) noexcept = ArchSimdOp<Arch>::load_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&load_f64) (const f64*) noexcept = ArchSimdOp<Arch>::load_f64;

	//mmasked load
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_load_i8) (const i8*, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_load_i16) (const i16*, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_load_i32) (const i32*, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_load_i64) (const i64*, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_load_ui8) (const ui8*, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_load_ui16) (const ui16*, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_load_ui32) (const ui32*, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_load_ui64) (const ui64*, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_load_f32) (const f32*, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_load_f64) (const f64*, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_f64;

	//rmasked load
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&rmasked_load_i8) (const i8*, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&rmasked_load_i16) (const i16*, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&rmasked_load_i32) (const i32*, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&rmasked_load_i64) (const i64*, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&rmasked_load_ui8) (const ui8*, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&rmasked_load_ui16) (const ui16*, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&rmasked_load_ui32) (const ui32*, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&rmasked_load_ui64) (const ui64*, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&rmasked_load_f32) (const f32*, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&rmasked_load_f64) (const f64*, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_f64;

	//load aligned
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&load_aligned_i8) (const i8*) noexcept = ArchSimdOp<Arch>::load_aligned_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&load_aligned_i16) (const i16*) noexcept = ArchSimdOp<Arch>::load_aligned_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&load_aligned_i32) (const i32*) noexcept = ArchSimdOp<Arch>::load_aligned_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&load_aligned_i64) (const i64*) noexcept = ArchSimdOp<Arch>::load_aligned_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&load_aligned_ui8) (const ui8*) noexcept = ArchSimdOp<Arch>::load_aligned_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&load_aligned_ui16) (const ui16*) noexcept = ArchSimdOp<Arch>::load_aligned_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&load_aligned_ui32) (const ui32*) noexcept = ArchSimdOp<Arch>::load_aligned_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&load_aligned_ui64) (const ui64*) noexcept = ArchSimdOp<Arch>::load_aligned_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&load_aligned_f32) (const f32*) noexcept = ArchSimdOp<Arch>::load_aligned_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&load_aligned_f64) (const f64*) noexcept = ArchSimdOp<Arch>::load_aligned_f64;

	//mmasked load aligned
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_load_aligned_i8) (const i8*, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_load_aligned_i16) (const i16*, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_load_aligned_i32) (const i32*, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_load_aligned_i64) (const i64*, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_load_aligned_ui8) (const ui8*, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_load_aligned_ui16) (const ui16*, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_load_aligned_ui32) (const ui32*, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_load_aligned_ui64) (const ui64*, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_load_aligned_f32) (const f32*, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_load_aligned_f64) (const f64*, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_load_aligned_f64;

	//rmasked load aligned
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&rmasked_load_aligned_i8) (const i8*, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&rmasked_load_aligned_i16) (const i16*, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&rmasked_load_aligned_i32) (const i32*, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&rmasked_load_aligned_i64) (const i64*, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&rmasked_load_aligned_ui8) (const ui8*, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&rmasked_load_aligned_ui16) (const ui16*, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&rmasked_load_aligned_ui32) (const ui32*, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&rmasked_load_aligned_ui64) (const ui64*, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&rmasked_load_aligned_f32) (const f32*, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&rmasked_load_aligned_f64) (const f64*, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_load_aligned_f64;

	//store//

	//store
	template<class Arch> constexpr inline void(&store_i8) (const i8*, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::store_i8;
	template<class Arch> constexpr inline void(&store_i16) (const i16*, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::store_i16;
	template<class Arch> constexpr inline void(&store_i32) (const i32*, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::store_i32;
	template<class Arch> constexpr inline void(&store_i64) (const i64*, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::store_i64;
	template<class Arch> constexpr inline void(&store_ui8) (const ui8*, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::store_ui8;
	template<class Arch> constexpr inline void(&store_ui16) (const ui16*, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::store_ui16;
	template<class Arch> constexpr inline void(&store_ui32) (const ui32*, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::store_ui32;
	template<class Arch> constexpr inline void(&store_ui64) (const ui64*, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::store_ui64;
	template<class Arch> constexpr inline void(&store_f32) (const f32*, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::store_f32;
	template<class Arch> constexpr inline void(&store_f64) (const f64*, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::store_f64;

	//mmasked store
	template<class Arch> constexpr inline void(&mmasked_store_i8) (const i8*, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_i8;
	template<class Arch> constexpr inline void(&mmasked_store_i16) (const i16*, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_i16;
	template<class Arch> constexpr inline void(&mmasked_store_i32) (const i32*, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_i32;
	template<class Arch> constexpr inline void(&mmasked_store_i64) (const i64*, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_i64;
	template<class Arch> constexpr inline void(&mmasked_store_ui8) (const ui8*, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_ui8;
	template<class Arch> constexpr inline void(&mmasked_store_ui16) (const ui16*, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_ui16;
	template<class Arch> constexpr inline void(&mmasked_store_ui32) (const ui32*, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_ui32;
	template<class Arch> constexpr inline void(&mmasked_store_ui64) (const ui64*, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_ui64;
	template<class Arch> constexpr inline void(&mmasked_store_f32) (const f32*, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_f32;
	template<class Arch> constexpr inline void(&mmasked_store_f64) (const f64*, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_f64;

	//rmasked store
	template<class Arch> constexpr inline void(&rmasked_store_i8) (const i8*, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_i8;
	template<class Arch> constexpr inline void(&rmasked_store_i16) (const i16*, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_i16;
	template<class Arch> constexpr inline void(&rmasked_store_i32) (const i32*, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_i32;
	template<class Arch> constexpr inline void(&rmasked_store_i64) (const i64*, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_i64;
	template<class Arch> constexpr inline void(&rmasked_store_ui8) (const ui8*, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_ui8;
	template<class Arch> constexpr inline void(&rmasked_store_ui16) (const ui16*, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_ui16;
	template<class Arch> constexpr inline void(&rmasked_store_ui32) (const ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_ui32;
	template<class Arch> constexpr inline void(&rmasked_store_ui64) (const ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_ui64;
	template<class Arch> constexpr inline void(&rmasked_store_f32) (const f32*, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_f32;
	template<class Arch> constexpr inline void(&rmasked_store_f64) (const f64*, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_f64;

	//store aligned
	template<class Arch> constexpr inline void(&store_aligned_i8) (const i8*, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_i8;
	template<class Arch> constexpr inline void(&store_aligned_i16) (const i16*, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_i16;
	template<class Arch> constexpr inline void(&store_aligned_i32) (const i32*, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_i32;
	template<class Arch> constexpr inline void(&store_aligned_i64) (const i64*, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_i64;
	template<class Arch> constexpr inline void(&store_aligned_ui8) (const ui8*, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_ui8;
	template<class Arch> constexpr inline void(&store_aligned_ui16) (const ui16*, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_ui16;
	template<class Arch> constexpr inline void(&store_aligned_ui32) (const ui32*, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_ui32;
	template<class Arch> constexpr inline void(&store_aligned_ui64) (const ui64*, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_ui64;
	template<class Arch> constexpr inline void(&store_aligned_f32) (const f32*, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_f32;
	template<class Arch> constexpr inline void(&store_aligned_f64) (const f64*, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::store_aligned_f64;

	//mmasked store aligned
	template<class Arch> constexpr inline void(&mmasked_store_aligned_i8) (const i8*, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_i8;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_i16) (const i16*, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_i16;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_i32) (const i32*, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_i32;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_i64) (const i64*, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_i64;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_ui8) (const ui8*, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_ui8;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_ui16) (const ui16*, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_ui16;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_ui32) (const ui32*, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_ui32;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_ui64) (const ui64*, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_ui64;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_f32) (const f32*, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_f32;
	template<class Arch> constexpr inline void(&mmasked_store_aligned_f64) (const f64*, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_store_aligned_f64;

	//rmasked store aligned
	template<class Arch> constexpr inline void(&rmasked_store_aligned_i8) (const i8*, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_i8;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_i16) (const i16*, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_i16;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_i32) (const i32*, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_i32;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_i64) (const i64*, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_i64;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_ui8) (const ui8*, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_ui8;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_ui16) (const ui16*, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_ui16;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_ui32) (const ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_ui32;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_ui64) (const ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_ui64;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_f32) (const f32*, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_f32;
	template<class Arch> constexpr inline void(&rmasked_store_aligned_f64) (const f64*, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_store_aligned_f64;

	//set//

	//set 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&set_i8) (const i8) noexcept = ArchSimdOp<Arch>::set_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&set_i16) (const i16) noexcept = ArchSimdOp<Arch>::set_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&set_i32) (const i32) noexcept = ArchSimdOp<Arch>::set_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&set_i64) (const i64) noexcept = ArchSimdOp<Arch>::set_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&set_ui8) (const ui8) noexcept = ArchSimdOp<Arch>::set_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&set_ui16) (const ui16) noexcept = ArchSimdOp<Arch>::set_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&set_ui32) (const ui32) noexcept = ArchSimdOp<Arch>::set_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&set_ui64) (const ui64) noexcept = ArchSimdOp<Arch>::set_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&set_f32) (const f32) noexcept = ArchSimdOp<Arch>::set_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&set_f64) (const f64) noexcept = ArchSimdOp<Arch>::set_f64;

	//mmasked set 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_set_i8) (const i8, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_set_i16) (const i16, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_set_i32) (const i32, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_set_i64) (const i64, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_set_ui8) (const ui8, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_set_ui16) (const ui16, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_set_ui32) (const ui32, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_set_ui64) (const ui64, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_set_f32) (const f32, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_set_f64) (const f64, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_set_f64;
	
	//mmasked src set 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_set_i8) (const i8, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_set_i16) (const i16, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_set_i32) (const i32, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_set_i64) (const i64, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_set_ui8) (const ui8, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_set_ui16) (const ui16, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_set_ui32) (const ui32, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_set_ui64) (const ui64, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_set_f32) (const f32, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_set_f64) (const f64, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_f64;

	//set zero 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&set_zero_i8) (void) noexcept = ArchSimdOp<Arch>::set_zero_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&set_zero_i16) (void) noexcept = ArchSimdOp<Arch>::set_zero_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&set_zero_i32) (void) noexcept = ArchSimdOp<Arch>::set_zero_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&set_zero_i64) (void) noexcept = ArchSimdOp<Arch>::set_zero_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&set_zero_ui8) (void) noexcept = ArchSimdOp<Arch>::set_zero_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&set_zero_ui16) (void) noexcept = ArchSimdOp<Arch>::set_zero_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&set_zero_ui32) (void) noexcept = ArchSimdOp<Arch>::set_zero_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&set_zero_ui64) (void) noexcept = ArchSimdOp<Arch>::set_zero_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&set_zero_f32) (void) noexcept = ArchSimdOp<Arch>::set_zero_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&set_zero_f64) (void) noexcept = ArchSimdOp<Arch>::set_zero_f64;

	//mmasked set zero
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_set_zero_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_set_zero_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_set_zero_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_set_zero_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_set_zero_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_set_zero_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_set_zero_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_set_zero_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_set_zero_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_set_zero_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_set_zero_f64;


	//shift//
	 
	//shift left 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&shift_left_i8) (SimdRegisterI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&shift_left_i16) (SimdRegisterI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&shift_left_i32) (SimdRegisterI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&shift_left_i64) (SimdRegisterI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&shift_left_ui8) (SimdRegisterUI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&shift_left_ui16) (SimdRegisterUI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&shift_left_ui32) (SimdRegisterUI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&shift_left_ui64) (SimdRegisterUI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&shift_left_f32) (SimdRegisterF32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&shift_left_f64) (SimdRegisterF64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_left_f64;

	//shift right 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&shift_right_i8) (SimdRegisterI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&shift_right_i16) (SimdRegisterI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&shift_right_i32) (SimdRegisterI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&shift_right_i64) (SimdRegisterI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&shift_right_ui8) (SimdRegisterUI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&shift_right_ui16) (SimdRegisterUI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&shift_right_ui32) (SimdRegisterUI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&shift_right_ui64) (SimdRegisterUI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&shift_right_f32) (SimdRegisterF32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&shift_right_f64) (SimdRegisterF64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::shift_right_f64;

	//pack//

	//pack in i8
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&pack_i16_in_i8) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::pack_i16_in_i8;

	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&pack_i32_in_i8)
		(SimdRegisterI32<Arch>,
			SimdRegisterI32<Arch>,
			SimdRegisterI32<Arch>,
			SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::pack_i32_in_i8;

	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&pack_i64_in_i8)
		(SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::pack_i64_in_i8;

	//pack in i16
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&pack_i32_in_i16) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::pack_i32_in_i16;

	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&pack_i64_in_i16)
		(SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>,
			SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::pack_i64_in_i16;

	//pack in i32
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&pack_i64_in_i32) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::pack_i64_in_i32;

	//pack in ui8
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&pack_ui16_in_ui8) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::pack_ui16_in_ui8;

	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&pack_ui32_in_ui8)
		(SimdRegisterUI32<Arch>,
			SimdRegisterUI32<Arch>,
			SimdRegisterUI32<Arch>,
			SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::pack_ui32_in_ui8;

	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&pack_ui64_in_ui8)
		(SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::pack_ui64_in_ui8;

	//pack in ui16
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&pack_ui32_in_ui16) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::pack_ui32_in_ui16;

	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&pack_ui64_in_ui16)
		(SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>,
			SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::pack_ui64_in_ui16;

	//pack in ui32
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&pack_ui64_in_ui32) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::pack_ui64_in_ui32;

	//pack in f32
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&pack_f64_in_f32) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::pack_f64_in_f32;

	//unpack//

	//unpack i8
	template<class Arch> constexpr inline void(&unpack_i8_to_i16)
		(SimdRegisterI8<Arch>,
			SimdRegisterI16<Arch>&,
			SimdRegisterI16<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_i8_to_i16;

	template<class Arch> constexpr inline void(&unpack_i8_to_i32)
		(SimdRegisterI8<Arch>,
			SimdRegisterI32<Arch>&,
			SimdRegisterI32<Arch>&,
			SimdRegisterI32<Arch>&,
			SimdRegisterI32<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_i8_to_i32;

	template<class Arch> constexpr inline void(&unpack_i8_to_i64)
		(SimdRegisterI8<Arch>,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_i8_to_i64;

	//unpack i16
	template<class Arch> constexpr inline void(&unpack_i16_to_i32)
		(SimdRegisterUI16<Arch>,
			SimdRegisterI32<Arch>&,
			SimdRegisterI32<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_i16_to_i32;

	template<class Arch> constexpr inline void(&unpack_i16_to_i64)
		(SimdRegisterUI16<Arch>,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_i16_to_i64;

	//unpack i32
	template<class Arch> constexpr inline void(&unpack_i32_to_i64)
		(SimdRegisterUI32<Arch>,
			SimdRegisterI64<Arch>&,
			SimdRegisterI64<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_i32_to_i64;

	//unpack ui8
	template<class Arch> constexpr inline void(&unpack_ui8_to_ui16)
		(SimdRegisterUI8<Arch>,
			SimdRegisterUI16<Arch>&,
			SimdRegisterUI16<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_ui8_to_ui16;

	template<class Arch> constexpr inline void(&unpack_ui8_to_ui32)
		(SimdRegisterUI8<Arch>,
			SimdRegisterUI32<Arch>&,
			SimdRegisterUI32<Arch>&,
			SimdRegisterUI32<Arch>&,
			SimdRegisterUI32<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_ui8_to_ui32;

	template<class Arch> constexpr inline void(&unpack_ui8_to_ui64)
		(SimdRegisterUI8<Arch>,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_ui8_to_ui64;

	//unpack ui16
	template<class Arch> constexpr inline void(&unpack_ui16_to_ui32)
		(SimdRegisterUI16<Arch>,
			SimdRegisterUI32<Arch>&,
			SimdRegisterUI32<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_ui16_to_ui32;

	template<class Arch> constexpr inline void(&unpack_ui16_to_ui64)
		(SimdRegisterUI16<Arch>,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_ui16_to_ui64;

	//unpack ui32
	template<class Arch> constexpr inline void(&unpack_ui32_to_ui64)
		(SimdRegisterUI32<Arch>,
			SimdRegisterUI64<Arch>&,
			SimdRegisterUI64<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_ui32_to_ui64;

	//unpack f32
	template<class Arch> constexpr inline void(&unpack_f32_to_f64)
		(SimdRegisterF32<Arch>,
			SimdRegisterF64<Arch>&,
			SimdRegisterF64<Arch>&) noexcept = ArchSimdOp<Arch>::unpack_f32_to_f64;

	//test//

	//test all zeros
	template<class Arch> constexpr inline Bool(&test_all_zero_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_i8;
	template<class Arch> constexpr inline Bool(&test_all_zero_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_i16;
	template<class Arch> constexpr inline Bool(&test_all_zero_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_i32;
	template<class Arch> constexpr inline Bool(&test_all_zero_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_i64;
	template<class Arch> constexpr inline Bool(&test_all_zero_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_ui8;
	template<class Arch> constexpr inline Bool(&test_all_zero_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_ui16;
	template<class Arch> constexpr inline Bool(&test_all_zero_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_ui32;
	template<class Arch> constexpr inline Bool(&test_all_zero_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_ui64;
	template<class Arch> constexpr inline Bool(&test_all_zero_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_f32;
	template<class Arch> constexpr inline Bool(&test_all_zero_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::test_all_zero_f64;

	//test all ones
	template<class Arch> constexpr inline Bool(&test_all_one_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_i8;
	template<class Arch> constexpr inline Bool(&test_all_one_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_i16;
	template<class Arch> constexpr inline Bool(&test_all_one_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_i32;
	template<class Arch> constexpr inline Bool(&test_all_one_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_i64;
	template<class Arch> constexpr inline Bool(&test_all_one_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_ui8;
	template<class Arch> constexpr inline Bool(&test_all_one_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_ui16;
	template<class Arch> constexpr inline Bool(&test_all_one_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_ui32;
	template<class Arch> constexpr inline Bool(&test_all_one_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_ui64;
	template<class Arch> constexpr inline Bool(&test_all_one_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_f32;
	template<class Arch> constexpr inline Bool(&test_all_one_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::test_all_one_f64;

	//mmasked test all zeros
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_i8;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_i16;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_i32;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_i64;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_ui8;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_ui16;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_ui32;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_ui64;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_f32;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_zero_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_zero_f64;

	//rmasked test all zeros
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_i8;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_i16;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_i32;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_i64;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_ui8;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_ui16;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_ui32;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_ui64;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_f32;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_zero_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_zero_f64;

	//mmasked test all ones 
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_i8;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_i16;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_i32;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_i64;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_ui8;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_ui16;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_ui32;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_ui64;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_f32;
	template<class Arch> constexpr inline Bool(&mmasked_test_all_one_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_test_all_one_f64;

	//rmasked test all ones 
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_i8;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_i16;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_i32;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_i64;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_ui8;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_ui16;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_ui32;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_ui64;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_f32;
	template<class Arch> constexpr inline Bool(&rmasked_test_all_one_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::rmasked_test_all_one_f64;

	//convert//

	//convert to any 
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_i8_to_any) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::convert_i8_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_i16_to_any) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::convert_i16_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_i32_to_any) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::convert_i32_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_i64_to_any) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::convert_i64_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_ui8_to_any) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::convert_ui8_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_ui16_to_any) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::convert_ui16_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_ui32_to_any) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::convert_ui32_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_ui64_to_any) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::convert_ui64_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_f32_to_any) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::convert_f32_to_any;
	template<class Arch> constexpr inline SimdRegisterAny<Arch>(&convert_f64_to_any) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::convert_f64_to_any;

	//convert to any 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&convert_any_to_i8) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&convert_any_to_i16) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&convert_any_to_i32) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&convert_any_to_i64) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&convert_any_to_ui8) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&convert_any_to_ui16) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&convert_any_to_ui32) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&convert_any_to_ui64) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&convert_any_to_f32) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&convert_any_to_f64) (SimdRegisterAny<Arch>) noexcept = ArchSimdOp<Arch>::convert_any_to_f64;

	//convert to int 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&convert_to_int_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_int_ui8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&convert_to_int_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_int_ui16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&convert_to_int_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_int_ui32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&convert_to_int_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_int_ui64;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&convert_to_int_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_int_f32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&convert_to_int_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_int_f64;

	//mmasked convert to int 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_convert_to_int_ui8) (SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_int_ui8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_convert_to_int_ui16) (SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_int_ui16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_convert_to_int_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_int_ui32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_convert_to_int_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_int_ui64;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_convert_to_int_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_int_f32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_convert_to_int_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_int_f64;

	//mmasked src convert to int 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_convert_to_int_ui8) (SimdRegisterUI8<Arch>, SimdRegisterI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_int_ui8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_convert_to_int_ui16) (SimdRegisterUI16<Arch>, SimdRegisterI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_int_ui16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_convert_to_int_ui32) (SimdRegisterUI32<Arch>, SimdRegisterI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_int_ui32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_convert_to_int_ui64) (SimdRegisterUI64<Arch>, SimdRegisterI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_int_ui64;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_convert_to_int_f32) (SimdRegisterF32<Arch>, SimdRegisterI32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_int_f32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_convert_to_int_f64) (SimdRegisterF64<Arch>, SimdRegisterI64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_int_f64;

	//convert to uint 
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&convert_to_uint_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_uint_i8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&convert_to_uint_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_uint_i16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&convert_to_uint_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_uint_i32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&convert_to_uint_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_uint_i64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&convert_to_uint_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_uint_f32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&convert_to_uint_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_uint_f64;

	//mmasked convert to uint 
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_convert_to_uint_i8) (SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_uint_i8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_convert_to_uint_i16) (SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_uint_i16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_convert_to_uint_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_uint_i32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_convert_to_uint_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_uint_i64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_convert_to_uint_f32) (SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_uint_f32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_convert_to_uint_f64) (SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_uint_f64;

	//mmasked src convert to uint 
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_convert_to_uint_i8) (SimdRegisterI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_uint_i8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_convert_to_uint_i16) (SimdRegisterI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_uint_i16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_convert_to_uint_i32) (SimdRegisterI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_uint_i32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_convert_to_uint_i64) (SimdRegisterI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_uint_i64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_convert_to_uint_f32) (SimdRegisterF32<Arch>, SimdRegisterUI32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_uint_f32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_convert_to_uint_f64) (SimdRegisterF64<Arch>, SimdRegisterUI64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_uint_f64;
	
	//convert to float 
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&convert_to_float_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_float_i32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&convert_to_float_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_float_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&convert_to_float_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_float_ui32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&convert_to_float_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::convert_to_float_ui64;

	//mmasked convert to float 
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_convert_to_float_i32) (SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_float_i32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_convert_to_float_i64) (SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_float_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_convert_to_float_ui32) (SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_float_ui32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_convert_to_float_ui64) (SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_convert_to_float_ui64;

	//mmasked src convert to float 
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_convert_to_float_i32) (SimdRegisterI32<Arch>, SimdRegisterF32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_float_i32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_convert_to_float_i64) (SimdRegisterI64<Arch>, SimdRegisterF64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_float_i64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_convert_to_float_ui32) (SimdRegisterUI32<Arch>, SimdRegisterF32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_float_ui32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_convert_to_float_ui64) (SimdRegisterUI64<Arch>, SimdRegisterF64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_convert_to_float_ui64;

	//swizzle//

	//insert
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&insert_i8) (SimdRegisterI8<Arch>, const i8, const Size) noexcept = ArchSimdOp<Arch>::insert_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&insert_i16) (SimdRegisterI16<Arch>, const i16, const Size) noexcept = ArchSimdOp<Arch>::insert_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&insert_i32) (SimdRegisterI32<Arch>, const i32, const Size) noexcept = ArchSimdOp<Arch>::insert_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&insert_i64) (SimdRegisterI64<Arch>, const i64, const Size) noexcept = ArchSimdOp<Arch>::insert_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&insert_ui8) (SimdRegisterUI8<Arch>, const ui8, const Size) noexcept = ArchSimdOp<Arch>::insert_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&insert_ui16) (SimdRegisterUI16<Arch>, const ui16, const Size) noexcept = ArchSimdOp<Arch>::insert_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&insert_ui32) (SimdRegisterUI32<Arch>, const ui32, const Size) noexcept = ArchSimdOp<Arch>::insert_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&insert_ui64) (SimdRegisterUI64<Arch>, const ui64, const Size) noexcept = ArchSimdOp<Arch>::insert_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&insert_f32) (SimdRegisterF32<Arch>, const f32, const Size) noexcept = ArchSimdOp<Arch>::insert_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&insert_f64) (SimdRegisterF64<Arch>, const f64, const Size) noexcept = ArchSimdOp<Arch>::insert_f64;

	//extract
	template<class Arch> constexpr inline i8(&extract_i8) (SimdRegisterI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_i8;
	template<class Arch> constexpr inline i16(&extract_i16) (SimdRegisterI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_i16;
	template<class Arch> constexpr inline i32(&extract_i32) (SimdRegisterI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_i32;
	template<class Arch> constexpr inline i64(&extract_i64) (SimdRegisterI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_i64;
	template<class Arch> constexpr inline ui8(&extract_ui8) (SimdRegisterUI8<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_ui8;
	template<class Arch> constexpr inline ui16(&extract_ui16) (SimdRegisterUI16<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_ui16;
	template<class Arch> constexpr inline ui32(&extract_ui32) (SimdRegisterUI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_ui32;
	template<class Arch> constexpr inline ui64(&extract_ui64) (SimdRegisterUI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_ui64;
	template<class Arch> constexpr inline f32(&extract_f32) (SimdRegisterF32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_f32;
	template<class Arch> constexpr inline f64(&extract_f64) (SimdRegisterF64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::extract_f64;

	//blend
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&blend_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::blend_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&blend_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::blend_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&blend_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::blend_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&blend_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::blend_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&blend_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::blend_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&blend_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::blend_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&blend_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::blend_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&blend_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::blend_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&blend_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::blend_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&blend_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::blend_f64;

	//blend with blend mask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&blend_mmask_i8) (SimdRegisterI8<Arch>, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&blend_mmask_i16) (SimdRegisterI16<Arch>, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&blend_mmask_i32) (SimdRegisterI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&blend_mmask_i64) (SimdRegisterI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&blend_mmask_ui8) (SimdRegisterUI8<Arch>, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&blend_mmask_ui16) (SimdRegisterUI16<Arch>, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&blend_mmask_ui32) (SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&blend_mmask_ui64) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&blend_mmask_f32) (SimdRegisterF32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&blend_mmask_f64) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::blend_mmask_f64;

	//advance mem//

	//broadcast 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&broadcast_i8) (const i8*) noexcept = ArchSimdOp<Arch>::broadcast_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&broadcast_i16) (const i16*) noexcept = ArchSimdOp<Arch>::broadcast_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&broadcast_i32) (const i32*) noexcept = ArchSimdOp<Arch>::broadcast_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&broadcast_i64) (const i64*) noexcept = ArchSimdOp<Arch>::broadcast_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&broadcast_ui8) (const ui8*) noexcept = ArchSimdOp<Arch>::broadcast_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&broadcast_ui16) (const ui16*) noexcept = ArchSimdOp<Arch>::broadcast_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&broadcast_ui32) (const ui32*) noexcept = ArchSimdOp<Arch>::broadcast_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&broadcast_ui64) (const ui64*) noexcept = ArchSimdOp<Arch>::broadcast_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&broadcast_f32) (const f32*) noexcept = ArchSimdOp<Arch>::broadcast_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&broadcast_f64) (const f64*) noexcept = ArchSimdOp<Arch>::broadcast_f64;

	//mmasked broadcast 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_broadcast_i8) (const i8*, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_broadcast_i16) (const i16*, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_broadcast_i32) (const i32*, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_broadcast_i64) (const i64*, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_broadcast_ui8) (const ui8*, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_broadcast_ui16) (const ui16*, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_broadcast_ui32) (const ui32*, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_broadcast_ui64) (const ui64*, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_broadcast_f32) (const f32*, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_broadcast_f64) (const f64*, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_broadcast_f64;

	//mmasked src broadcast 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmasked_src_broadcast_i8) (const i8*, SimdRegisterI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmasked_src_broadcast_i16) (const i16*, SimdRegisterI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_src_broadcast_i32) (const i32*, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_src_broadcast_i64) (const i64*, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmasked_src_broadcast_ui8) (const ui8*, SimdRegisterUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmasked_src_broadcast_ui16) (const ui16*, SimdRegisterUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_src_broadcast_ui32) (const ui32*, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_src_broadcast_ui64) (const ui64*, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_src_broadcast_f32) (const f32*, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_src_broadcast_f64) (const f64*, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_src_broadcast_f64;

	//gather
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&gather_i32) (const i32*, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::gather_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&gather_i64) (const i64*, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::gather_i64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&gather_ui32) (const ui32*, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::gather_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&gather_ui64) (const ui64*, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::gather_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&gather_f32) (const f32*, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::gather_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&gather_f64) (const f64*, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::gather_f64;

	//gather scaled
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&gather_scaled_i32) (const i32*, SimdRegisterUI32<Arch>, const i32) noexcept = ArchSimdOp<Arch>::gather_scaled_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&gather_scaled_i64) (const i64*, SimdRegisterUI64<Arch>, const i32) noexcept = ArchSimdOp<Arch>::gather_scaled_i64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&gather_scaled_ui32) (const ui32*, SimdRegisterUI32<Arch>, const i32) noexcept = ArchSimdOp<Arch>::gather_scaled_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&gather_scaled_ui64) (const ui64*, SimdRegisterUI64<Arch>, const i32) noexcept = ArchSimdOp<Arch>::gather_scaled_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&gather_scaled_f32) (const f32*, SimdRegisterUI32<Arch>, const i32) noexcept = ArchSimdOp<Arch>::gather_scaled_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&gather_scaled_f64) (const f64*, SimdRegisterUI64<Arch>, const i32) noexcept = ArchSimdOp<Arch>::gather_scaled_f64;

	//gather masked
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_gather_i32) (const i32*, SimdRegisterUI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_gather_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_gather_i64) (const i64*, SimdRegisterUI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_gather_i64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_gather_ui32) (const ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_gather_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_gather_ui64) (const ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_gather_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_gather_f32) (const f32*, SimdRegisterUI32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_gather_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_gather_f64) (const f64*, SimdRegisterUI64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_gather_f64;

	//masked gather scaled
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmasked_gather_scaled_i32) (const i32*, SimdRegisterUI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>, const i32) noexcept = ArchSimdOp<Arch>::mmasked_gather_scaled_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmasked_gather_scaled_i64) (const i64*, SimdRegisterUI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>, const i32) noexcept = ArchSimdOp<Arch>::mmasked_gather_scaled_i64;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmasked_gather_scaled_ui32) (const ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>, const i32) noexcept = ArchSimdOp<Arch>::mmasked_gather_scaled_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmasked_gather_scaled_ui64) (const ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>, const i32) noexcept = ArchSimdOp<Arch>::mmasked_gather_scaled_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmasked_gather_scaled_f32) (const f32*, SimdRegisterUI32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>, const i32) noexcept = ArchSimdOp<Arch>::mmasked_gather_scaled_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmasked_gather_scaled_f64) (const f64*, SimdRegisterUI64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>, const i32) noexcept = ArchSimdOp<Arch>::mmasked_gather_scaled_f64;

	//scatter
	template<class Arch> constexpr inline void(&scatter_i32) (i32*, SimdRegisterUI32<Arch>, SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::scatter_i32;
	template<class Arch> constexpr inline void(&scatter_i64) (i64*, SimdRegisterUI64<Arch>, SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::scatter_i64;
	template<class Arch> constexpr inline void(&scatter_ui32) (ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::scatter_ui32;
	template<class Arch> constexpr inline void(&scatter_ui64) (ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::scatter_ui64;
	template<class Arch> constexpr inline void(&scatter_f32) (f32*, SimdRegisterUI32<Arch>, SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::scatter_f32;
	template<class Arch> constexpr inline void(&scatter_f64) (f64*, SimdRegisterUI64<Arch>, SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::scatter_f64;

	//scatter scaled
	template<class Arch> constexpr inline void(&scatter_scaled_i32) (i32*, SimdRegisterUI32<Arch>, SimdRegisterI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::scatter_scaled_i32;
	template<class Arch> constexpr inline void(&scatter_scaled_i64) (i64*, SimdRegisterUI64<Arch>, SimdRegisterI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::scatter_scaled_i64;
	template<class Arch> constexpr inline void(&scatter_scaled_ui32) (ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::scatter_scaled_ui32;
	template<class Arch> constexpr inline void(&scatter_scaled_ui64) (ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::scatter_scaled_ui64;
	template<class Arch> constexpr inline void(&scatter_scaled_f32) (f32*, SimdRegisterUI32<Arch>, SimdRegisterF32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::scatter_scaled_f32;
	template<class Arch> constexpr inline void(&scatter_scaled_f64) (f64*, SimdRegisterUI64<Arch>, SimdRegisterF64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::scatter_scaled_f64;

	//masked scatter
	template<class Arch> constexpr inline void(&mmasked_scatter_i32) (i32*, SimdRegisterUI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_scatter_i32;
	template<class Arch> constexpr inline void(&mmasked_scatter_i64) (i64*, SimdRegisterUI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_scatter_i64;
	template<class Arch> constexpr inline void(&mmasked_scatter_ui32) (ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_scatter_ui32;
	template<class Arch> constexpr inline void(&mmasked_scatter_ui64) (ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_scatter_ui64;
	template<class Arch> constexpr inline void(&mmasked_scatter_f32) (f32*, SimdRegisterUI32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_scatter_f32;
	template<class Arch> constexpr inline void(&mmasked_scatter_f64) (f64*, SimdRegisterUI64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmasked_scatter_f64;

	//masked scatter scaled
	template<class Arch> constexpr inline void(&mmasked_scatter_scaled_i32) (i32*, SimdRegisterUI32<Arch>, SimdRegisterI32<Arch>, SimdMaskI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::mmasked_scatter_scaled_i32;
	template<class Arch> constexpr inline void(&mmasked_scatter_scaled_i64) (i64*, SimdRegisterUI64<Arch>, SimdRegisterI64<Arch>, SimdMaskI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::mmasked_scatter_scaled_i64;
	template<class Arch> constexpr inline void(&mmasked_scatter_scaled_ui32) (ui32*, SimdRegisterUI32<Arch>, SimdRegisterUI32<Arch>, SimdMaskUI32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::mmasked_scatter_scaled_ui32;
	template<class Arch> constexpr inline void(&mmasked_scatter_scaled_ui64) (ui64*, SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>, SimdMaskUI64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::mmasked_scatter_scaled_ui64;
	template<class Arch> constexpr inline void(&mmasked_scatter_scaled_f32) (f32*, SimdRegisterUI32<Arch>, SimdRegisterF32<Arch>, SimdMaskF32<Arch>, const Size) noexcept = ArchSimdOp<Arch>::mmasked_scatter_scaled_f32;
	template<class Arch> constexpr inline void(&mmasked_scatter_scaled_f64) (f64*, SimdRegisterUI64<Arch>, SimdRegisterF64<Arch>, SimdMaskF64<Arch>, const Size) noexcept = ArchSimdOp<Arch>::mmasked_scatter_scaled_f64;

	//mask convert//

	//rmask to mmask 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&rmask_to_mmask_i8) (SimdRegisterI8<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&rmask_to_mmask_i16) (SimdRegisterI16<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&rmask_to_mmask_i32) (SimdRegisterI32<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&rmask_to_mmask_i64) (SimdRegisterI64<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&rmask_to_mmask_ui8) (SimdRegisterUI8<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&rmask_to_mmask_ui16) (SimdRegisterUI16<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&rmask_to_mmask_ui32) (SimdRegisterUI32<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&rmask_to_mmask_ui64) (SimdRegisterUI64<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&rmask_to_mmask_f32) (SimdRegisterF32<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&rmask_to_mmask_f64) (SimdRegisterF64<Arch>) noexcept = ArchSimdOp<Arch>::rmask_to_mmask_f64;

	//mmask to rmask
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&mmask_to_rmask_i8) (SimdMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&mmask_to_rmask_i16) (SimdMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&mmask_to_rmask_i32) (SimdMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&mmask_to_rmask_i64) (SimdMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&mmask_to_rmask_ui8) (SimdMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&mmask_to_rmask_ui16) (SimdMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&mmask_to_rmask_ui32) (SimdMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&mmask_to_rmask_ui64) (SimdMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&mmask_to_rmask_f32) (SimdMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&mmask_to_rmask_f64) (SimdMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::mmask_to_rmask_f64;

	//cmask to mmask 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&cmask_to_mmask_i8) (SimdCMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&cmask_to_mmask_i16) (SimdCMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&cmask_to_mmask_i32) (SimdCMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&cmask_to_mmask_i64) (SimdCMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&cmask_to_mmask_ui8) (SimdCMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&cmask_to_mmask_ui16) (SimdCMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&cmask_to_mmask_ui32) (SimdCMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&cmask_to_mmask_ui64) (SimdCMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&cmask_to_mmask_f32) (SimdCMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&cmask_to_mmask_f64) (SimdCMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_mmask_f64;

	//cmask to rmask 
	template<class Arch> constexpr inline SimdRegisterI8<Arch>(&cmask_to_rmask_i8) (SimdCMaskI8<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_i8;
	template<class Arch> constexpr inline SimdRegisterI16<Arch>(&cmask_to_rmask_i16) (SimdCMaskI16<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_i16;
	template<class Arch> constexpr inline SimdRegisterI32<Arch>(&cmask_to_rmask_i32) (SimdCMaskI32<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_i32;
	template<class Arch> constexpr inline SimdRegisterI64<Arch>(&cmask_to_rmask_i64) (SimdCMaskI64<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_i64;
	template<class Arch> constexpr inline SimdRegisterUI8<Arch>(&cmask_to_rmask_ui8) (SimdCMaskUI8<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_ui8;
	template<class Arch> constexpr inline SimdRegisterUI16<Arch>(&cmask_to_rmask_ui16) (SimdCMaskUI16<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_ui16;
	template<class Arch> constexpr inline SimdRegisterUI32<Arch>(&cmask_to_rmask_ui32) (SimdCMaskUI32<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_ui32;
	template<class Arch> constexpr inline SimdRegisterUI64<Arch>(&cmask_to_rmask_ui64) (SimdCMaskUI64<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_ui64;
	template<class Arch> constexpr inline SimdRegisterF32<Arch>(&cmask_to_rmask_f32) (SimdCMaskF32<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_f32;
	template<class Arch> constexpr inline SimdRegisterF64<Arch>(&cmask_to_rmask_f64) (SimdCMaskF64<Arch>) noexcept = ArchSimdOp<Arch>::cmask_to_rmask_f64;

	//mask operations
	
	//mask bitwise compare equal 
	template<class Arch> constexpr inline Bool(&mask_compare_equal_i8) (SimdMaskI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i8;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_i16) (SimdMaskI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i16;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_i32) (SimdMaskI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i32;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_i64) (SimdMaskI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i64;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_ui8) (SimdMaskUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui8;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_ui16) (SimdMaskUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui16;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_ui32) (SimdMaskUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui32;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_ui64) (SimdMaskUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui64;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_f32) (SimdMaskF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_f32;
	template<class Arch> constexpr inline Bool(&mask_compare_equal_f64) (SimdMaskF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_f64;

	//mask bitwise compare not equal 
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_i8) (SimdMaskI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i8;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_i16) (SimdMaskI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i16;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_i32) (SimdMaskI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i32;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_i64) (SimdMaskI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i64;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_ui8) (SimdMaskUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui8;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_ui16) (SimdMaskUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui16;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_ui32) (SimdMaskUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui32;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_ui64) (SimdMaskUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui64;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_f32) (SimdMaskF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_f32;
	template<class Arch> constexpr inline Bool(&mask_compare_not_equal_f64) (SimdMaskF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_f64;


	//mask bitwise shift left 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_bitwise_shift_left_i8) (SimdMaskI8<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_bitwise_shift_left_i16) (SimdMaskI16<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_bitwise_shift_left_i32) (SimdMaskI32<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_bitwise_shift_left_i64) (SimdMaskI64<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_bitwise_shift_left_ui8) (SimdMaskUI8<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_bitwise_shift_left_ui16) (SimdMaskUI16<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_bitwise_shift_left_ui32) (SimdMaskUI32<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_bitwise_shift_left_ui64) (SimdMaskUI64<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_bitwise_shift_left_f32) (SimdMaskF32<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_bitwise_shift_left_f64) (SimdMaskF64<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_f64;

	//mask bitwise shift right 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_bitwise_shift_right_i8) (SimdMaskI8<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_bitwise_shift_right_i16) (SimdMaskI16<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_bitwise_shift_right_i32) (SimdMaskI32<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_bitwise_shift_right_i64) (SimdMaskI64<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_bitwise_shift_right_ui8) (SimdMaskUI8<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_bitwise_shift_right_ui16) (SimdMaskUI16<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_bitwise_shift_right_ui32) (SimdMaskUI32<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_bitwise_shift_right_ui64) (SimdMaskUI64<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_bitwise_shift_right_f32) (SimdMaskF32<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_bitwise_shift_right_f64) (SimdMaskF64<Arch>, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_f64;

	//mask bitwise and 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_bitwise_and_i8) (SimdMaskI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_bitwise_and_i16) (SimdMaskI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_bitwise_and_i32) (SimdMaskI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_bitwise_and_i64) (SimdMaskI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_bitwise_and_ui8) (SimdMaskUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_bitwise_and_ui16) (SimdMaskUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_bitwise_and_ui32) (SimdMaskUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_bitwise_and_ui64) (SimdMaskUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_bitwise_and_f32) (SimdMaskF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_bitwise_and_f64) (SimdMaskF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_f64;

	//mask bitwise or 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_bitwise_or_i8) (SimdMaskI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_bitwise_or_i16) (SimdMaskI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_bitwise_or_i32) (SimdMaskI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_bitwise_or_i64) (SimdMaskI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_bitwise_or_ui8) (SimdMaskUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_bitwise_or_ui16) (SimdMaskUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_bitwise_or_ui32) (SimdMaskUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_bitwise_or_ui64) (SimdMaskUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_bitwise_or_f32) (SimdMaskF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_bitwise_or_f64) (SimdMaskF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_f64;

	//mask bitwise xor 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_bitwise_xor_i8) (SimdMaskI8<Arch>, SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_bitwise_xor_i16) (SimdMaskI16<Arch>, SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_bitwise_xor_i32) (SimdMaskI32<Arch>, SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_bitwise_xor_i64) (SimdMaskI64<Arch>, SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_bitwise_xor_ui8) (SimdMaskUI8<Arch>, SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_bitwise_xor_ui16) (SimdMaskUI16<Arch>, SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_bitwise_xor_ui32) (SimdMaskUI32<Arch>, SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_bitwise_xor_ui64) (SimdMaskUI64<Arch>, SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_bitwise_xor_f32) (SimdMaskF32<Arch>, SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_bitwise_xor_f64) (SimdMaskF64<Arch>, SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_f64;

	//mask bitwise not 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_bitwise_not_i8) (SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_bitwise_not_i16) (SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_bitwise_not_i32) (SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_bitwise_not_i64) (SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_bitwise_not_ui8) (SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_bitwise_not_ui16) (SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_bitwise_not_ui32) (SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_bitwise_not_ui64) (SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_bitwise_not_f32) (SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_bitwise_not_f64) (SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_f64;

	//mask popcount
	template<class Arch> constexpr inline Size(&mask_popcount_i8) (SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i8;
	template<class Arch> constexpr inline Size(&mask_popcount_i16) (SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i16;
	template<class Arch> constexpr inline Size(&mask_popcount_i32) (SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i32;
	template<class Arch> constexpr inline Size(&mask_popcount_i64) (SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i64;
	template<class Arch> constexpr inline Size(&mask_popcount_ui8) (SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui8;
	template<class Arch> constexpr inline Size(&mask_popcount_ui16) (SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui16;
	template<class Arch> constexpr inline Size(&mask_popcount_ui32) (SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui32;
	template<class Arch> constexpr inline Size(&mask_popcount_ui64) (SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui64;
	template<class Arch> constexpr inline Size(&mask_popcount_f32) (SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_f32;
	template<class Arch> constexpr inline Size(&mask_popcount_f64) (SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_f64;

	//mask bitscan forward
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_i8) (SimdMaskI8<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i8;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_i16) (SimdMaskI16<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i16;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_i32) (SimdMaskI32<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i32;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_i64) (SimdMaskI64<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i64;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_ui8) (SimdMaskUI8<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui8;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_ui16) (SimdMaskUI16<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui16;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_ui32) (SimdMaskUI32<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui32;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_ui64) (SimdMaskUI64<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui64;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_f32) (SimdMaskF32<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_f32;
	template<class Arch> constexpr inline Bool(&mask_bitscan_forward_f64) (SimdMaskF64<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_f64;

	//mask bitscan backward
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_i8) (SimdMaskI8<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i8;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_i16) (SimdMaskI16<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i16;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_i32) (SimdMaskI32<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i32;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_i64) (SimdMaskI64<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i64;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_ui8) (SimdMaskUI8<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui8;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_ui16) (SimdMaskUI16<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui16;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_ui32) (SimdMaskUI32<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui32;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_ui64) (SimdMaskUI64<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui64;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_f32) (SimdMaskF32<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_f32;
	template<class Arch> constexpr inline Bool(&mask_bitscan_backward_f64) (SimdMaskF64<Arch>, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_f64;

	//mask get 
	template<class Arch> constexpr inline ui64(&mask_get_i8) (SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_i8;
	template<class Arch> constexpr inline ui64(&mask_get_i16) (SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_i16;
	template<class Arch> constexpr inline ui64(&mask_get_i32) (SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_i32;
	template<class Arch> constexpr inline ui64(&mask_get_i64) (SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_i64;
	template<class Arch> constexpr inline ui64(&mask_get_ui8) (SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui8;
	template<class Arch> constexpr inline ui64(&mask_get_ui16) (SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui16;
	template<class Arch> constexpr inline ui64(&mask_get_ui32) (SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui32;
	template<class Arch> constexpr inline ui64(&mask_get_ui64) (SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui64;
	template<class Arch> constexpr inline ui64(&mask_get_f32) (SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_f32;
	template<class Arch> constexpr inline ui64(&mask_get_f64) (SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_get_f64;

	//mask set 
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_set_i8) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_set_i16) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_set_i32) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_set_i64) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_set_ui8) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_set_ui16) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_set_ui32) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_set_ui64) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_set_f32) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_set_f64) (ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_f64;

	//mask test all inactive
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_i8) (SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i8;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_i16) (SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i16;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_i32) (SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i32;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_i64) (SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i64;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_ui8) (SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui8;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_ui16) (SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui16;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_ui32) (SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui32;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_ui64) (SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui64;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_f32) (SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_f32;
	template<class Arch> constexpr inline Bool(&mask_test_all_inactive_f64) (SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_f64;

	//mask test all active
	template<class Arch> constexpr inline Bool(&mask_test_all_active_i8) (SimdMaskI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i8;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_i16) (SimdMaskI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i16;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_i32) (SimdMaskI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i32;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_i64) (SimdMaskI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i64;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_ui8) (SimdMaskUI8<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui8;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_ui16) (SimdMaskUI16<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui16;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_ui32) (SimdMaskUI32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui32;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_ui64) (SimdMaskUI64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui64;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_f32) (SimdMaskF32<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_f32;
	template<class Arch> constexpr inline Bool(&mask_test_all_active_f64) (SimdMaskF64<Arch>) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_f64;

	//mask test inactive at
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_i8) (SimdMaskI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i8;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_i16) (SimdMaskI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i16;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_i32) (SimdMaskI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i32;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_i64) (SimdMaskI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i64;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_ui8) (SimdMaskUI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui8;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_ui16) (SimdMaskUI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui16;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_ui32) (SimdMaskUI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui32;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_ui64) (SimdMaskUI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui64;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_f32) (SimdMaskF32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_f32;
	template<class Arch> constexpr inline Bool(&mask_test_inactive_at_f64) (SimdMaskF64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_f64;

	//mask test active at
	template<class Arch> constexpr inline Bool(&mask_test_active_at_i8) (SimdMaskI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i8;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_i16) (SimdMaskI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i16;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_i32) (SimdMaskI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i32;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_i64) (SimdMaskI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i64;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_ui8) (SimdMaskUI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui8;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_ui16) (SimdMaskUI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui16;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_ui32) (SimdMaskUI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui32;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_ui64) (SimdMaskUI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui64;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_f32) (SimdMaskF32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_f32;
	template<class Arch> constexpr inline Bool(&mask_test_active_at_f64) (SimdMaskF64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_f64;

	//mask set all inactive
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_set_all_inactive_i8) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_set_all_inactive_i16) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_set_all_inactive_i32) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_set_all_inactive_i64) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_set_all_inactive_ui8) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_set_all_inactive_ui16) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_set_all_inactive_ui32) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_set_all_inactive_ui64) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_set_all_inactive_f32) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_set_all_inactive_f64) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_f64;

	//mask set all active
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_set_all_active_i8) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_set_all_active_i16) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_set_all_active_i32) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_set_all_active_i64) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_set_all_active_ui8) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_set_all_active_ui16) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_set_all_active_ui32) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_set_all_active_ui64) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_set_all_active_f32) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_set_all_active_f64) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_f64;

	//mask set inactive at
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_set_inactive_at_i8) (SimdMaskI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_set_inactive_at_i16) (SimdMaskI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_set_inactive_at_i32) (SimdMaskI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_set_inactive_at_i64) (SimdMaskI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_set_inactive_at_ui8) (SimdMaskUI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_set_inactive_at_ui16) (SimdMaskUI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_set_inactive_at_ui32) (SimdMaskUI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_set_inactive_at_ui64) (SimdMaskUI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_set_inactive_at_f32) (SimdMaskF32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_set_inactive_at_f64) (SimdMaskF64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_f64;

	//mask set active at
	template<class Arch> constexpr inline SimdMaskI8<Arch>(&mask_set_active_at_i8) (SimdMaskI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i8;
	template<class Arch> constexpr inline SimdMaskI16<Arch>(&mask_set_active_at_i16) (SimdMaskI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i16;
	template<class Arch> constexpr inline SimdMaskI32<Arch>(&mask_set_active_at_i32) (SimdMaskI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i32;
	template<class Arch> constexpr inline SimdMaskI64<Arch>(&mask_set_active_at_i64) (SimdMaskI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i64;
	template<class Arch> constexpr inline SimdMaskUI8<Arch>(&mask_set_active_at_ui8) (SimdMaskUI8<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui8;
	template<class Arch> constexpr inline SimdMaskUI16<Arch>(&mask_set_active_at_ui16) (SimdMaskUI16<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui16;
	template<class Arch> constexpr inline SimdMaskUI32<Arch>(&mask_set_active_at_ui32) (SimdMaskUI32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui32;
	template<class Arch> constexpr inline SimdMaskUI64<Arch>(&mask_set_active_at_ui64) (SimdMaskUI64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui64;
	template<class Arch> constexpr inline SimdMaskF32<Arch>(&mask_set_active_at_f32) (SimdMaskF32<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_f32;
	template<class Arch> constexpr inline SimdMaskF64<Arch>(&mask_set_active_at_f64) (SimdMaskF64<Arch>, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_f64;
}