#pragma once

//std
#include <cmath>
#include <array>

//own
#include "../util/bits.h"
#include "../util/assert.h"
#include "../container/array.h"
#include "../math.h"
#include "simdBase.h"

//interface
namespace natl::simd {
	template<typename DataType, Size registerSize>
	struct StandardSimdRegisterBase {
		using value_type = DataType;
		Array<DataType, registerSize / (sizeof(DataType) * 8)> values;
		constexpr StandardSimdRegisterBase() noexcept = default;
		constexpr StandardSimdRegisterBase(const StandardSimdRegisterBase& other) noexcept : values(other.values) {}
		constexpr DataType& operator[] (const Size index) noexcept { return values[index]; }
		constexpr const DataType& operator[] (const Size index) const noexcept { return values[index]; }
		constexpr static inline Size count() noexcept { return registerSize / (sizeof(DataType) * 8); };
		constexpr static inline Size size() noexcept { return count(); };
		constexpr void operator=(StandardSimdRegisterBase rhs) noexcept {
			for (Size i = 0; i < count(); i++) {
				values[i] = rhs[i];
			}
		}
	};

	template<typename DataType, Size RegisterSize>
	struct StandardSimdRegisterBaseInfo {
		using value_type = DataType;
		constexpr static inline Size count() { return RegisterSize / (sizeof(DataType) * 8); };
		constexpr static inline Size size() { return count(); };
		constexpr static inline Size bitSize() { return RegisterSize; };
	};

	template<Size RegisterSize>
	class StandardArch {
	public:
		constexpr static inline Size registerSize = RegisterSize;
	};

	template<Size RegisterSize>
	struct SimdArchInfo<StandardArch<RegisterSize>> {
		constexpr static inline Size registerSize = RegisterSize;
		constexpr static inline ConstAsciiStringView name = "StandardArch";
	};

	template<Size RegisterSize>
	struct ArchSimdRegisters<StandardArch<RegisterSize>> {
	public:
		using simd_arch = StandardArch<RegisterSize>;

		using SimdRegisterI8 = StandardSimdRegisterBase<i8, simd_arch::registerSize>;
		using SimdRegisterI16 = StandardSimdRegisterBase<i16, simd_arch::registerSize>;
		using SimdRegisterI32 = StandardSimdRegisterBase<i32, simd_arch::registerSize>;
		using SimdRegisterI64 = StandardSimdRegisterBase<i64, simd_arch::registerSize>;

		using SimdRegisterUI8 = StandardSimdRegisterBase<ui8, simd_arch::registerSize>;
		using SimdRegisterUI16 = StandardSimdRegisterBase<ui16, simd_arch::registerSize>;
		using SimdRegisterUI32 = StandardSimdRegisterBase<ui32, simd_arch::registerSize>;
		using SimdRegisterUI64 = StandardSimdRegisterBase<ui64, simd_arch::registerSize>;

		using SimdRegisterF32 = StandardSimdRegisterBase<f32, simd_arch::registerSize>;
		using SimdRegisterF64 = StandardSimdRegisterBase<f64, simd_arch::registerSize>;

		using SimdRegisterAny = StandardSimdRegisterBase<ui8, simd_arch::registerSize>;

		using SimdRegisterI8Info = StandardSimdRegisterBaseInfo<i8, simd_arch::registerSize>;
		using SimdRegisterI16Info = StandardSimdRegisterBaseInfo<i16, simd_arch::registerSize>;
		using SimdRegisterI32Info = StandardSimdRegisterBaseInfo<i32, simd_arch::registerSize>;
		using SimdRegisterI64Info = StandardSimdRegisterBaseInfo<i64, simd_arch::registerSize>;

		using SimdRegisterUI8Info = StandardSimdRegisterBaseInfo<ui8, simd_arch::registerSize>;
		using SimdRegisterUI16Info = StandardSimdRegisterBaseInfo<ui16, simd_arch::registerSize>;
		using SimdRegisterUI32Info = StandardSimdRegisterBaseInfo<ui32, simd_arch::registerSize>;
		using SimdRegisterUI64Info = StandardSimdRegisterBaseInfo<ui64, simd_arch::registerSize>;

		using SimdRegisterF32Info = StandardSimdRegisterBaseInfo<f32, simd_arch::registerSize>;
		using SimdRegisterF64Info = StandardSimdRegisterBaseInfo<f64, simd_arch::registerSize>;

		using SimdRegisterAnyInfo = StandardSimdRegisterBaseInfo<ui8, simd_arch::registerSize>;
	};

	template<class DataType, Size RegisterSize>
	struct SimdRegisterToInfoT<StandardSimdRegisterBase<DataType, RegisterSize>> {
		using type = StandardSimdRegisterBaseInfo<DataType, RegisterSize>;
	};

	//mask
	template<Size RegisterSize>
	class ArchSimdMask<StandardArch<RegisterSize>> : public DefaultArchSimdMask<RegisterSize> {};

	template<Size RegisterSize>
	class ArchSimdMaskInfo<StandardArch<RegisterSize>> : public DefaultArchSimdMaskInfo<RegisterSize> {};

	//simd control mask op 
	template<Size RegisterSize>
	struct ArchSimdMaskOp<StandardArch<RegisterSize>> : public DefaultArchSimdMaskOp<StandardArch<RegisterSize>> {};

	//simd op//

	template<Size RegisterSize>
	struct SimdArchOp<StandardArch<RegisterSize>> {
	public:
		using simd_arch = StandardArch<RegisterSize>;

		//registers
		using simd_register_i8 = SimdRegisterI8<simd_arch>;
		using simd_register_i16 = SimdRegisterI16<simd_arch>;
		using simd_register_i32 = SimdRegisterI32<simd_arch>;
		using simd_register_i64 = SimdRegisterI64<simd_arch>;

		using simd_register_ui8 = SimdRegisterUI8<simd_arch>;
		using simd_register_ui16 = SimdRegisterUI16<simd_arch>;
		using simd_register_ui32 = SimdRegisterUI32<simd_arch>;
		using simd_register_ui64 = SimdRegisterUI64<simd_arch>;

		using simd_register_f32 = SimdRegisterF32<simd_arch>;
		using simd_register_f64 = SimdRegisterF64<simd_arch>;

		//mmask 
		using simd_mmask_i8 = SimdMaskI8<simd_arch>;
		using simd_mmask_i16 = SimdMaskI16<simd_arch>;
		using simd_mmask_i32 = SimdMaskI32<simd_arch>;
		using simd_mmask_i64 = SimdMaskI64<simd_arch>;

		using simd_mmask_ui8 = SimdMaskUI8<simd_arch>;
		using simd_mmask_ui16 = SimdMaskUI16<simd_arch>;
		using simd_mmask_ui32 = SimdMaskUI32<simd_arch>;
		using simd_mmask_ui64 = SimdMaskUI64<simd_arch>;

		using simd_mmask_f32 = SimdMaskF32<simd_arch>;
		using simd_mmask_f64 = SimdMaskF64<simd_arch>;

		//mmasks 
		using simd_cmask_i8 = SimdCMaskI8<simd_arch>;
		using simd_cmask_i16 = SimdCMaskI16<simd_arch>;
		using simd_cmask_i32 = SimdCMaskI32<simd_arch>;
		using simd_cmask_i64 = SimdCMaskI64<simd_arch>;

		using simd_cmask_ui8 = SimdCMaskUI8<simd_arch>;
		using simd_cmask_ui16 = SimdCMaskUI16<simd_arch>;
		using simd_cmask_ui32 = SimdCMaskUI32<simd_arch>;
		using simd_cmask_ui64 = SimdCMaskUI64<simd_arch>;

		using simd_cmask_f32 = SimdCMaskF32<simd_arch>;
		using simd_cmask_f64 = SimdCMaskF64<simd_arch>;

		//arithmetic//

		//add
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType add(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = lhs[i] + rhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 add_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return add<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 add_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return add<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 add_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return add<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 add_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return add<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 add_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return add<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 add_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return add<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 add_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return add<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 add_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return add<simd_register_ui64>(lhs, rhs);
		}

		constexpr inline static simd_register_f32 add_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return add<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 add_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return add<simd_register_f64>(lhs, rhs);
		}

		//mmasked add
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedAdd(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] + rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_add_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedAdd<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_add_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedAdd<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_add_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedAdd<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_add_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedAdd<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_add_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedAdd<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_add_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedAdd<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_add_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedAdd<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_add_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedAdd<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_add_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedAdd<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_add_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedAdd<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src add
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcAdd(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] + rhs[i];
				} 
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_add_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_add_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_add_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_add_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_add_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_add_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_add_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_add_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_add_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_add_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcAdd<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//sub
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType sub(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = lhs[i] - rhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 sub_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return sub<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 sub_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return sub<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 sub_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return sub<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 sub_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return sub<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 sub_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return sub<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 sub_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return sub<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 sub_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return sub<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 sub_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return sub<simd_register_ui64>(lhs, rhs);
		}

		constexpr inline static simd_register_f32 sub_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return sub<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 sub_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return sub<simd_register_f64>(lhs, rhs);
		}

		//mmasked sub
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSub(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] - rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_sub_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedSub<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_sub_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedSub<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_sub_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedSub<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_sub_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedSub<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_sub_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSub<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_sub_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSub<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_sub_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSub<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_sub_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSub<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_sub_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedSub<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_sub_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedSub<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src sub
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcSub(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] - rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_sub_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcSub<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_sub_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcSub<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_sub_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcSub<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_sub_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcSub<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_sub_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcSub<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_sub_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcSub<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_sub_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcSub<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_sub_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcSub<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_sub_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcSub<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_sub_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcSub<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//mul
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType mul(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = lhs[i] * rhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 mul_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return mul<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 mul_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return mul<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 mul_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return mul<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 mul_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return mul<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 mul_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return mul<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 mul_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return mul<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 mul_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return mul<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 mul_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return mul<simd_register_ui64>(lhs, rhs);
		}

		constexpr inline static simd_register_f32 mul_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return mul<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 mul_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return mul<simd_register_f64>(lhs, rhs);
		}

		//mmasked mul
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedMul(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] * rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_mul_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedMul<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_mul_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedMul<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_mul_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedMul<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_mul_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedMul<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_mul_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedMul<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_mul_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedMul<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_mul_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedMul<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_mul_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedMul<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_mul_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedMul<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_mul_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedMul<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src mul
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcMul(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] * rhs[i];
				} 
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_mul_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcMul<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_mul_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcMul<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_mul_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcMul<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_mul_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcMul<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_mul_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcMul<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_mul_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcMul<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_mul_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcMul<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_mul_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcMul<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_mul_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcMul<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_mul_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcMul<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//div 
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType div(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = lhs[i] / rhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 div_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return div<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 div_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return div<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 div_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return div<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 div_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return div<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 div_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return div<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 div_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return div<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 div_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return div<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 div_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return div<simd_register_ui64>(lhs, rhs);
		}

		constexpr inline static simd_register_f32 div_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return div<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 div_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return div<simd_register_f64>(lhs, rhs);
		}

		//mmasked div 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedDiv(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] / rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_div_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedDiv<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_div_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedDiv<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_div_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedDiv<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_div_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedDiv<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_div_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedDiv<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_div_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedDiv<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_div_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedDiv<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_div_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedDiv<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_div_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedDiv<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_div_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedDiv<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src div 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcDiv(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] / rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_div_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_div_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_div_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_div_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_div_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_div_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_div_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_div_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_div_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_div_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcDiv<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//add sat
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType addsat(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = natlm::addsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_register_i8 add_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return addsat<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 add_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return addsat<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 add_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return addsat<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 add_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return addsat<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 add_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return addsat<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 add_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return addsat<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 add_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return addsat<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 add_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return addsat<simd_register_ui64>(lhs, rhs);
		}

		//mmasked add sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedAddSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::addsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_add_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedAddSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_add_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedAddSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_add_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedAddSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_add_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedAddSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_add_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedAddSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_add_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedAddSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_add_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedAddSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_add_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedAddSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		//mmasked src add sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcAddSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::addsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_add_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_add_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_add_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_add_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_add_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_add_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_add_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_add_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcAddSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}


		//sub sat
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType subsat(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = natlm::subsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_register_i8 sub_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return subsat<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 sub_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return subsat<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 sub_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return subsat<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 sub_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return subsat<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 sub_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return subsat<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 sub_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return subsat<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 sub_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return subsat<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 sub_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return subsat<simd_register_ui64>(lhs, rhs);
		}

		//mmasked sub sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSubSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::subsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_sub_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedSubSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_sub_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedSubSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_sub_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedSubSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_sub_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedSubSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_sub_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSubSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_sub_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSubSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_sub_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSubSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_sub_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSubSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		//mmasked src sub sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcSubSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::subsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_sub_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_sub_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_sub_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_sub_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_sub_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_sub_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_sub_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_sub_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcSubSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		//mul sat
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType mulsat(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = natlm::mulsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_register_i8 mul_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return mulsat<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 mul_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return mulsat<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 mul_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return mulsat<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 mul_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return mulsat<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 mul_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return mulsat<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 mul_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return mulsat<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 mul_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return mulsat<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 mul_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return mulsat<simd_register_ui64>(lhs, rhs);
		}

		//mmasked mul sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedMulSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::mulsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_mul_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedMulSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_mul_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedMulSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_mul_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedMulSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_mul_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedMulSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_mul_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedMulSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_mul_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedMulSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_mul_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedMulSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_mul_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedMulSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		//mmasked src mul sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcMulSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::mulsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_mul_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_mul_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_mul_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_mul_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_mul_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_mul_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_mul_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_mul_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcMulSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		//div sat 
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType divsat(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = natlm::divsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_register_i8 div_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return divsat<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 div_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return divsat<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 div_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return divsat<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 div_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return divsat<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 div_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return divsat<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 div_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return divsat<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 div_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return divsat<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 div_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return divsat<simd_register_ui64>(lhs, rhs);
		}

		//mmasked div sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedDivSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::divsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_div_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedDivSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_div_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedDivSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_div_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedDivSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_div_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedDivSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_div_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedDivSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_div_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedDivSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_div_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedDivSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_div_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedDivSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		//mmasked src div sat 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcDivSat(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::divsat<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(lhs[i], rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_div_sat_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_div_sat_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_div_sat_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_div_sat_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_div_sat_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_div_sat_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_div_sat_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_div_sat_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcDivSat<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		//remainder
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType remainder(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = lhs[i] % rhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 remainder_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return remainder<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 remainder_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return remainder<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 remainder_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return remainder<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 remainder_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return remainder<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 remainder_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return remainder<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 remainder_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return remainder<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 remainder_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return remainder<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 remainder_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return remainder<simd_register_ui64>(lhs, rhs);
		}

		//mmasked remainder
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedRemainder(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] % rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_remainder_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedRemainder<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_remainder_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedRemainder<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_remainder_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedRemainder<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_remainder_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedRemainder<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_remainder_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedRemainder<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_remainder_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedRemainder<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_remainder_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedRemainder<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_remainder_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedRemainder<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		//mmasked src remainder
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcRemainder(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = lhs[i] % rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_remainder_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_remainder_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_remainder_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_remainder_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_remainder_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_remainder_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_remainder_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_remainder_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcRemainder<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		//fused mul add
		constexpr inline static simd_register_f32 fused_mul_add_f32(
			simd_register_f32 mulLhs,
			simd_register_f32 mulRhs,
			simd_register_f32 addRhs) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = (mulLhs[i] * mulRhs[i]) + addRhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_f64 fused_mul_add_f64(
			simd_register_f64 mulLhs,
			simd_register_f64 mulRhs,
			simd_register_f64 addRhs) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = (mulLhs[i] * mulRhs[i]) + addRhs[i];
			}
			return output;
		}

		//mmasked fused mul add
		constexpr inline static simd_register_f32 mmasked_fused_mul_add_f32(
			simd_register_f32 mulLhs, simd_register_f32 mulRhs,
			simd_register_f32 addRhs, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output{};
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) + addRhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_fused_mul_add_f64(
			simd_register_f64 mulLhs, simd_register_f64 mulRhs,
			simd_register_f64 addRhs, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output{};
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) + addRhs[i];
				}
			}
			return output;
		}

		//mmasked src fused mul add
		constexpr inline static simd_register_f32 mmasked_src_fused_mul_add_f32(
			simd_register_f32 mulLhs, simd_register_f32 mulRhs,
			simd_register_f32 addRhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) + addRhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_src_fused_mul_add_f64(
			simd_register_f64 mulLhs, simd_register_f64 mulRhs,
			simd_register_f64 addRhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) + addRhs[i];
				}
			}
			return output;
		}

		//fused mul sub
		constexpr inline static simd_register_f32 fused_mul_sub_f32(
			simd_register_f32 mulLhs,
			simd_register_f32 mulRhs,
			simd_register_f32 subRhs) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = (mulLhs[i] * mulRhs[i]) - subRhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_f64 fused_mul_sub_f64(
			simd_register_f64 mulLhs,
			simd_register_f64 mulRhs,
			simd_register_f64 subRhs) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = (mulLhs[i] * mulRhs[i]) - subRhs[i];
			}
			return output;
		}

		//mmasked fused mul sub
		constexpr inline static simd_register_f32 mmasked_fused_mul_sub_f32(
			simd_register_f32 mulLhs, simd_register_f32 mulRhs,
			simd_register_f32 subRhs, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output{};
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) - subRhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_fused_mul_sub_f64(
			simd_register_f64 mulLhs, simd_register_f64 mulRhs,
			simd_register_f64 subRhs, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output{};
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) - subRhs[i];
				}
			}
			return output;
		}

		//mmasked src fused mul sub
		constexpr inline static simd_register_f32 mmasked_src_fused_mul_sub_f32(
			simd_register_f32 mulLhs, simd_register_f32 mulRhs,
			simd_register_f32 subRhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) - subRhs[i];
				} 
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_src_fused_mul_sub_f64(
			simd_register_f64 mulLhs, simd_register_f64 mulRhs,
			simd_register_f64 subRhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = (mulLhs[i] * mulRhs[i]) - subRhs[i];
				} 
			}
			return output;
		}

		//square root
		constexpr inline static simd_register_f32 square_root_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::sqrtF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 square_root_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::sqrtF64(value[i]);
			}
			return output;
		}

		//mmasked square root
		constexpr inline static simd_register_f32 mmasked_square_root_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sqrtF32(value[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_square_root_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sqrtF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src square root
		constexpr inline static simd_register_f32 mmasked_src_square_root_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sqrtF32(value[i]);
				} 
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_src_square_root_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sqrtF64(value[i]);
				} 
			}
			return output;
		}

		//reciprocal
		constexpr inline static simd_register_f32 reciprocal_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::reciprocalF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 reciprocal_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::reciprocalF64(value[i]);
			}
			return output;
		}

		//mmasked reciprocal
		constexpr inline static simd_register_f32 mmasked_reciprocal_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::reciprocalF32(value[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_reciprocal_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::reciprocalF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src reciprocal
		constexpr inline static simd_register_f32 mmasked_src_reciprocal_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::reciprocalF32(value[i]);
				} 
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_src_reciprocal_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::reciprocalF64(value[i]);
				} 
			}
			return output;
		}

		//reciprocal square root
		constexpr inline static simd_register_f32 reciprocal_square_root_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::rsqrtF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 reciprocal_square_root_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::rsqrtF64(value[i]);
			}
			return output;
		}

		//mmasked reciprocal square root
		constexpr inline static simd_register_f32 mmasked_reciprocal_square_root_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::rsqrtF32(value[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_reciprocal_square_root_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::rsqrtF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src reciprocal square root
		constexpr inline static simd_register_f32 mmasked_src_reciprocal_square_root_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::rsqrtF32(value[i]);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_f64 mmasked_src_reciprocal_square_root_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::rsqrtF64(value[i]);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		//math functions//

		//min
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType min(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = (lhs[i] < rhs[i]) ? lhs[i] : rhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 min_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return min<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 min_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return min<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 min_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return min<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 min_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return min<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 min_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return min<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 min_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return min<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 min_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return min<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 min_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return min<simd_register_ui64>(lhs, rhs);
		}

		constexpr inline static simd_register_f32 min_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return min<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 min_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return min<simd_register_f64>(lhs, rhs);
		}

		//mmasked min
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedMin(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = (lhs[i] < rhs[i]) ? lhs[i] : rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_min_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedMin<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_min_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedMin<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_min_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedMin<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_min_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedMin<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_min_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedMin<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_min_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedMin<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_min_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedMin<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_min_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedMin<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_min_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedMin<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_min_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedMin<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src min
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcMin(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = (lhs[i] < rhs[i]) ? lhs[i] : rhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_min_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcMin<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_min_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcMin<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_min_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcMin<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_min_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcMin<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_min_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcMin<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_min_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcMin<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_min_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcMin<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_min_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcMin<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_min_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcMin<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_min_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcMin<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//max
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType max(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = (lhs[i] < rhs[i]) ? rhs[i] : lhs[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 max_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return max<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 max_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return max<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 max_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return max<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 max_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return max<simd_register_i64>(lhs, rhs);
		}

		constexpr inline static simd_register_ui8 max_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return max<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 max_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return max<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 max_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return max<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 max_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return max<simd_register_ui64>(lhs, rhs);
		}

		constexpr inline static simd_register_f32 max_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return max<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 max_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return max<simd_register_f64>(lhs, rhs);
		}

		//mmasked max
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedMax(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = (lhs[i] < rhs[i]) ? rhs[i] : lhs[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_max_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedMax<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_max_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedMax<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_max_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedMax<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_max_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedMax<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_max_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedMax<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_max_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedMax<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_max_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedMax<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_max_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedMax<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_max_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedMax<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_max_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedMax<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src max
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcMax(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = (lhs[i] < rhs[i]) ? rhs[i] : lhs[i];
				} 
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_max_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcMax<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_max_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcMax<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_max_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcMax<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_max_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcMax<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_max_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcMax<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_max_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcMax<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_max_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcMax<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_max_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcMax<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_max_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcMax<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_max_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcMax<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//abs
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType abs(SimdRegisterType value) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = math::abs<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(value[i]);
			}
			return output;
		}

		constexpr inline static simd_register_i8 abs_i8(simd_register_i8 value) noexcept {
			return abs<simd_register_i8>(value);
		}
		constexpr inline static simd_register_i16 abs_i16(simd_register_i16 value) noexcept {
			return abs<simd_register_i16>(value);
		}
		constexpr inline static simd_register_i32 abs_i32(simd_register_i32 value) noexcept {
			return abs<simd_register_i32>(value);
		}
		constexpr inline static simd_register_i64 abs_i64(simd_register_i64 value) noexcept {
			return abs<simd_register_i64>(value);
		}

		constexpr inline static simd_register_f32 abs_f32(simd_register_f32 value) noexcept {
			return abs<simd_register_f32>(value);
		}
		constexpr inline static simd_register_f64 abs_f64(simd_register_f64 value) noexcept {
			return abs<simd_register_f64>(value);
		}

		//mmasked abs
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedAbs(SimdRegisterType value, SimdMaskType mmask) noexcept {
			SimdRegisterType output = value;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::abs<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(value[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_abs_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedAbs<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_abs_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedAbs<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_abs_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedAbs<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_abs_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedAbs<simd_register_i64, simd_mmask_i64>(value, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_abs_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedAbs<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_abs_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedAbs<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked src abs
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcAbs(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natlm::abs<typename SimdRegisterToInfo<SimdRegisterType>::value_type>(value[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_abs_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcAbs<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_abs_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcAbs<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_abs_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcAbs<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_abs_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcAbs<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_abs_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcAbs<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_abs_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcAbs<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//negate
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType negate(SimdRegisterType value) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = -value[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 negate_i8(simd_register_i8 value) noexcept {
			return negate<simd_register_i8>(value);
		}
		constexpr inline static simd_register_i16 negate_i16(simd_register_i16 value) noexcept {
			return negate<simd_register_i16>(value);
		}
		constexpr inline static simd_register_i32 negate_i32(simd_register_i32 value) noexcept {
			return negate<simd_register_i32>(value);
		}
		constexpr inline static simd_register_i64 negate_i64(simd_register_i64 value) noexcept {
			return negate<simd_register_i64>(value);
		}

		constexpr inline static simd_register_f32 negate_f32(simd_register_f32 value) noexcept {
			return negate<simd_register_f32>(value);
		}
		constexpr inline static simd_register_f64 negate_f64(simd_register_f64 value) noexcept {
			return negate<simd_register_f64>(value);
		}

		//mmasked negate
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedNegate(SimdRegisterType value, SimdMaskType mmask) noexcept {
			SimdRegisterType output = value;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = -value[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_negate_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedNegate<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_negate_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedNegate<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_negate_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedNegate<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_negate_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedNegate<simd_register_i64, simd_mmask_i64>(value, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_negate_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedNegate<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_negate_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedNegate<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked src negate
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcNegate(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = -value[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_negate_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcNegate<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_negate_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcNegate<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_negate_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcNegate<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_negate_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcNegate<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_negate_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcNegate<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_negate_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcNegate<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//ceil
		constexpr inline static simd_register_f32 ceil_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::ceilF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 ceil_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::ceilF64(value[i]);
			}
			return output;
		}

		//mmasked ceil
		constexpr inline static simd_register_f32 mmasked_ceil_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::ceilF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_ceil_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::ceilF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src ceil
		constexpr inline static simd_register_f32 mmasked_src_ceil_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::ceilF32(value[i]);
				} 
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_ceil_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::ceilF64(value[i]);
				}
			}
			return output;
		}

		//floor
		constexpr inline static simd_register_f32 floor_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::floorF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 floor_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::floorF64(value[i]);
			}
			return output;
		}

		//mmasked floor
		constexpr inline static simd_register_f32 mmasked_floor_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::floorF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_floor_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::floorF64(value[i]);
				}

			}
			return output;
		}

		//mmasked src floor
		constexpr inline static simd_register_f32 mmasked_src_floor_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::floorF32(value[i]);
				} 
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_floor_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::floorF64(value[i]);
				}
			}
			return output;
		}

		//trunc
		constexpr inline static simd_register_f32 trunc_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::truncF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 trunc_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::truncF64(value[i]);
			}
			return output;
		}

		//mmasked trunc
		constexpr inline static simd_register_f32 mmasked_trunc_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::truncF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_trunc_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::truncF64(value[i]);
				}

			}
			return output;
		}

		//mmasked src trunc
		constexpr inline static simd_register_f32 mmasked_src_trunc_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::truncF32(value[i]);
				} 
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_trunc_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::truncF64(value[i]);
				} 
			}
			return output;
		}

		//round
		constexpr inline static simd_register_f32 round_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::roundF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 round_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::roundF64(value[i]);
			}
			return output;
		}

		//mmasked round
		constexpr inline static simd_register_f32 mmasked_round_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::roundF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_round_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::roundF64(value[i]);
				}

			}
			return output;
		}

		//mmasked src round
		constexpr inline static simd_register_f32 mmasked_src_round_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::roundF32(value[i]);
				} 
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_round_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::roundF64(value[i]);
				} 
			}
			return output;
		}

		//trig//

				//basic_sin
		constexpr inline static simd_register_f32 basic_sin_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicSinF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_sin_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicSinF64(value[i]);
			}
			return output;
		}

		//mmasked basic_sin
		constexpr inline static simd_register_f32 mmasked_basic_sin_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_sin_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_sin
		constexpr inline static simd_register_f32 mmasked_src_basic_sin_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_sin_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinF64(value[i]);
				}
			}
			return output;
		}

		//sin
		constexpr inline static simd_register_f32 sin_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::sinF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 sin_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::sinF64(value[i]);
			}
			return output;
		}

		//mmasked sin
		constexpr inline static simd_register_f32 mmasked_sin_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_sin_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src sin
		constexpr inline static simd_register_f32 mmasked_src_sin_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_sin_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinF64(value[i]);
				}
			}
			return output;
		}

		//basic_cos
		constexpr inline static simd_register_f32 basic_cos_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicCosF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_cos_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicCosF64(value[i]);
			}
			return output;
		}

		//mmasked basic_cos
		constexpr inline static simd_register_f32 mmasked_basic_cos_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_cos_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCosF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_cos
		constexpr inline static simd_register_f32 mmasked_src_basic_cos_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_cos_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCosF64(value[i]);
				}
			}
			return output;
		}

		//cos
		constexpr inline static simd_register_f32 cos_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::cosF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 cos_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::cosF64(value[i]);
			}
			return output;
		}

		//mmasked cos
		constexpr inline static simd_register_f32 mmasked_cos_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::cosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_cos_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::cosF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src cos
		constexpr inline static simd_register_f32 mmasked_src_cos_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::cosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_cos_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::cosF64(value[i]);
				}
			}
			return output;
		}

		//basic_tan
		constexpr inline static simd_register_f32 basic_tan_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicTanF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_tan_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicTanF64(value[i]);
			}
			return output;
		}

		//mmasked basic_tan
		constexpr inline static simd_register_f32 mmasked_basic_tan_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_tan_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_tan
		constexpr inline static simd_register_f32 mmasked_src_basic_tan_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_tan_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanF64(value[i]);
				}
			}
			return output;
		}

		//tan
		constexpr inline static simd_register_f32 tan_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::tanF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 tan_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::tanF64(value[i]);
			}
			return output;
		}

		//mmasked tan
		constexpr inline static simd_register_f32 mmasked_tan_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_tan_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src tan
		constexpr inline static simd_register_f32 mmasked_src_tan_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_tan_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanF64(value[i]);
				}
			}
			return output;
		}

		//basic_asin
		constexpr inline static simd_register_f32 basic_asin_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicAsinF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_asin_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicAsinF64(value[i]);
			}
			return output;
		}

		//mmasked basic_asin
		constexpr inline static simd_register_f32 mmasked_basic_asin_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_asin_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_asin
		constexpr inline static simd_register_f32 mmasked_src_basic_asin_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_asin_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinF64(value[i]);
				}
			}
			return output;
		}

		//asin
		constexpr inline static simd_register_f32 asin_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::asinF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 asin_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::asinF64(value[i]);
			}
			return output;
		}

		//mmasked asin
		constexpr inline static simd_register_f32 mmasked_asin_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_asin_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src asin
		constexpr inline static simd_register_f32 mmasked_src_asin_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_asin_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinF64(value[i]);
				}
			}
			return output;
		}

		//basic_acos
		constexpr inline static simd_register_f32 basic_acos_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicAcosF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_acos_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicAcosF64(value[i]);
			}
			return output;
		}

		//mmasked basic_acos
		constexpr inline static simd_register_f32 mmasked_basic_acos_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_acos_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcosF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_acos
		constexpr inline static simd_register_f32 mmasked_src_basic_acos_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_acos_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcosF64(value[i]);
				}
			}
			return output;
		}

		//acos
		constexpr inline static simd_register_f32 acos_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::acosF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 acos_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::acosF64(value[i]);
			}
			return output;
		}

		//mmasked acos
		constexpr inline static simd_register_f32 mmasked_acos_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_acos_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acosF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src acos
		constexpr inline static simd_register_f32 mmasked_src_acos_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acosF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_acos_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acosF64(value[i]);
				}
			}
			return output;
		}

		//basic_atan
		constexpr inline static simd_register_f32 basic_atan_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicAtanF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_atan_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicAtanF64(value[i]);
			}
			return output;
		}

		//mmasked basic_atan
		constexpr inline static simd_register_f32 mmasked_basic_atan_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_atan_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_atan
		constexpr inline static simd_register_f32 mmasked_src_basic_atan_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_atan_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanF64(value[i]);
				}
			}
			return output;
		}

		//atan
		constexpr inline static simd_register_f32 atan_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::atanF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 atan_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::atanF64(value[i]);
			}
			return output;
		}

		//mmasked atan
		constexpr inline static simd_register_f32 mmasked_atan_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_atan_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src atan
		constexpr inline static simd_register_f32 mmasked_src_atan_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_atan_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanF64(value[i]);
				}
			}
			return output;
		}

		//basic_sinh
		constexpr inline static simd_register_f32 basic_sinh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicSinhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_sinh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicSinhF64(value[i]);
			}
			return output;
		}

		//mmasked basic_sinh
		constexpr inline static simd_register_f32 mmasked_basic_sinh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_sinh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_sinh
		constexpr inline static simd_register_f32 mmasked_src_basic_sinh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_sinh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicSinhF64(value[i]);
				}
			}
			return output;
		}

		//sinh
		constexpr inline static simd_register_f32 sinh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::sinhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 sinh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::sinhF64(value[i]);
			}
			return output;
		}

		//mmasked sinh
		constexpr inline static simd_register_f32 mmasked_sinh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_sinh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src sinh
		constexpr inline static simd_register_f32 mmasked_src_sinh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_sinh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::sinhF64(value[i]);
				}
			}
			return output;
		}

		//basic_cosh
		constexpr inline static simd_register_f32 basic_cosh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicCoshF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_cosh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicCoshF64(value[i]);
			}
			return output;
		}

		//mmasked basic_cosh
		constexpr inline static simd_register_f32 mmasked_basic_cosh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCoshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_cosh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCoshF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_cosh
		constexpr inline static simd_register_f32 mmasked_src_basic_cosh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCoshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_cosh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicCoshF64(value[i]);
				}
			}
			return output;
		}

		//cosh
		constexpr inline static simd_register_f32 cosh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::coshF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 cosh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::coshF64(value[i]);
			}
			return output;
		}

		//mmasked cosh
		constexpr inline static simd_register_f32 mmasked_cosh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::coshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_cosh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::coshF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src cosh
		constexpr inline static simd_register_f32 mmasked_src_cosh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::coshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_cosh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::coshF64(value[i]);
				}
			}
			return output;
		}

		//basic_tanh
		constexpr inline static simd_register_f32 basic_tanh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicTanhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_tanh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicTanhF64(value[i]);
			}
			return output;
		}

		//mmasked basic_tanh
		constexpr inline static simd_register_f32 mmasked_basic_tanh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_tanh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_tanh
		constexpr inline static simd_register_f32 mmasked_src_basic_tanh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_tanh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicTanhF64(value[i]);
				}
			}
			return output;
		}

		//tanh
		constexpr inline static simd_register_f32 tanh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::tanhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 tanh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::tanhF64(value[i]);
			}
			return output;
		}

		//mmasked tanh
		constexpr inline static simd_register_f32 mmasked_tanh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_tanh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src tanh
		constexpr inline static simd_register_f32 mmasked_src_tanh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_tanh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::tanhF64(value[i]);
				}
			}
			return output;
		}

		//basic_asinh
		constexpr inline static simd_register_f32 basic_asinh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicAsinhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_asinh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicAsinhF64(value[i]);
			}
			return output;
		}

		//mmasked basic_asinh
		constexpr inline static simd_register_f32 mmasked_basic_asinh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_asinh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_asinh
		constexpr inline static simd_register_f32 mmasked_src_basic_asinh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_asinh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAsinhF64(value[i]);
				}
			}
			return output;
		}

		//asinh
		constexpr inline static simd_register_f32 asinh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::asinhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 asinh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::asinhF64(value[i]);
			}
			return output;
		}

		//mmasked asinh
		constexpr inline static simd_register_f32 mmasked_asinh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_asinh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src asinh
		constexpr inline static simd_register_f32 mmasked_src_asinh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_asinh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::asinhF64(value[i]);
				}
			}
			return output;
		}

		//basic_acosh
		constexpr inline static simd_register_f32 basic_acosh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicAcoshF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_acosh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicAcoshF64(value[i]);
			}
			return output;
		}

		//mmasked basic_acosh
		constexpr inline static simd_register_f32 mmasked_basic_acosh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcoshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_acosh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcoshF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_acosh
		constexpr inline static simd_register_f32 mmasked_src_basic_acosh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcoshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_acosh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAcoshF64(value[i]);
				}
			}
			return output;
		}

		//acosh
		constexpr inline static simd_register_f32 acosh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::acoshF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 acosh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::acoshF64(value[i]);
			}
			return output;
		}

		//mmasked acosh
		constexpr inline static simd_register_f32 mmasked_acosh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acoshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_acosh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acoshF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src acosh
		constexpr inline static simd_register_f32 mmasked_src_acosh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acoshF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_acosh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::acoshF64(value[i]);
				}
			}
			return output;
		}

		//basic_atanh
		constexpr inline static simd_register_f32 basic_atanh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::basicAtanhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 basic_atanh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::basicAtanhF64(value[i]);
			}
			return output;
		}

		//mmasked basic_atanh
		constexpr inline static simd_register_f32 mmasked_basic_atanh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_basic_atanh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src basic_atanh
		constexpr inline static simd_register_f32 mmasked_src_basic_atanh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_basic_atanh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::basicAtanhF64(value[i]);
				}
			}
			return output;
		}

		//atanh
		constexpr inline static simd_register_f32 atanh_f32(simd_register_f32 value) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = math::atanhF32(value[i]);
			}
			return output;
		}
		constexpr inline static simd_register_f64 atanh_f64(simd_register_f64 value) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = math::atanhF64(value[i]);
			}
			return output;
		}

		//mmasked atanh
		constexpr inline static simd_register_f32 mmasked_atanh_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = value;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_atanh_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = value;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanhF64(value[i]);
				}
			}
			return output;
		}

		//mmasked src atanh
		constexpr inline static simd_register_f32 mmasked_src_atanh_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			simd_register_f32 output = src;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanhF32(value[i]);
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_src_atanh_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			simd_register_f64 output = src;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mmask[i]) {
					output[i] = math::atanhF64(value[i]);
				}
			}
			return output;
		}

		//bitwise 
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType bitwiseShiftLeftC(SimdRegisterType value, const Size count) noexcept {
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsBuiltInFloatingPointC<simd_register_value_type>) {
					output[i] = natl::bitwiseShiftLeft(value[i], static_cast<UIntOfByteSize<sizeof(simd_register_value_type)>>(count));
				} else {
					output[i] = natl::bitwiseShiftLeft(value[i], static_cast<simd_register_value_type>(count));
				}
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType bitwiseShiftRightC(SimdRegisterType value, const Size count) noexcept {
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsBuiltInFloatingPointC<simd_register_value_type>) {
					output[i] = natl::bitwiseShiftRight(value[i], static_cast<UIntOfByteSize<sizeof(simd_register_value_type)>>(count));
				} else {
					output[i] = natl::bitwiseShiftRight(value[i], static_cast<simd_register_value_type>(count));
				}
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType = LhsSimdRegisterType>
		constexpr inline static LhsSimdRegisterType bitwiseShiftLeft(LhsSimdRegisterType value, RhsSimdRegisterType count) noexcept {
			LhsSimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				output[i] = natl::bitwiseShiftLeft(value[i], count[i]);
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType = LhsSimdRegisterType>
		constexpr inline static LhsSimdRegisterType bitwiseShiftRight(LhsSimdRegisterType value, RhsSimdRegisterType count) noexcept {
			LhsSimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				output[i] = natl::bitwiseShiftRight(value[i], count[i]);
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType = LhsSimdRegisterType>
		constexpr inline static LhsSimdRegisterType bitwiseAnd(LhsSimdRegisterType lhs, RhsSimdRegisterType rhs) noexcept {
			LhsSimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				output[i] = natl::bitwiseAnd(lhs[i], rhs[i]);
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType = LhsSimdRegisterType>
		constexpr inline static LhsSimdRegisterType bitwiseOr(LhsSimdRegisterType lhs, RhsSimdRegisterType rhs) noexcept {
			LhsSimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				output[i] = natl::bitwiseOr(lhs[i], rhs[i]);
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType = LhsSimdRegisterType>
		constexpr inline static LhsSimdRegisterType bitwiseXor(LhsSimdRegisterType lhs, RhsSimdRegisterType rhs) noexcept {
			LhsSimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				output[i] = natl::bitwiseXor(lhs[i], rhs[i]);
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType bitwiseNot(SimdRegisterType value) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = natl::bitwiseNot(value[i]);
			}
			return output;
		}

		//bitwise_shift_left_c
		constexpr inline static simd_register_i8 bitwise_shift_left_c_i8(simd_register_i8 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_i8>(lhs, count);
		}
		constexpr inline static simd_register_i16 bitwise_shift_left_c_i16(simd_register_i16 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_i16>(lhs, count);
		}
		constexpr inline static simd_register_i32 bitwise_shift_left_c_i32(simd_register_i32 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_i32>(lhs, count);
		}
		constexpr inline static simd_register_i64 bitwise_shift_left_c_i64(simd_register_i64 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_i64>(lhs, count);
		}
		constexpr inline static simd_register_ui8 bitwise_shift_left_c_ui8(simd_register_ui8 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_ui8>(lhs, count);
		}
		constexpr inline static simd_register_ui16 bitwise_shift_left_c_ui16(simd_register_ui16 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_ui16>(lhs, count);
		}
		constexpr inline static simd_register_ui32 bitwise_shift_left_c_ui32(simd_register_ui32 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_ui32>(lhs, count);
		}
		constexpr inline static simd_register_ui64 bitwise_shift_left_c_ui64(simd_register_ui64 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_ui64>(lhs, count);
		}
		constexpr inline static simd_register_f32 bitwise_shift_left_c_f32(simd_register_f32 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_f32>(lhs, count);
		}
		constexpr inline static simd_register_f64 bitwise_shift_left_c_f64(simd_register_f64 lhs, const Size count) noexcept {
			return bitwiseShiftLeftC<simd_register_f64>(lhs, count);
		}

		//bitwise_shift_right_c
		constexpr inline static simd_register_i8 bitwise_shift_right_c_i8(simd_register_i8 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_i8>(lhs, count);
		}
		constexpr inline static simd_register_i16 bitwise_shift_right_c_i16(simd_register_i16 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_i16>(lhs, count);
		}
		constexpr inline static simd_register_i32 bitwise_shift_right_c_i32(simd_register_i32 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_i32>(lhs, count);
		}
		constexpr inline static simd_register_i64 bitwise_shift_right_c_i64(simd_register_i64 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_i64>(lhs, count);
		}
		constexpr inline static simd_register_ui8 bitwise_shift_right_c_ui8(simd_register_ui8 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_ui8>(lhs, count);
		}
		constexpr inline static simd_register_ui16 bitwise_shift_right_c_ui16(simd_register_ui16 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_ui16>(lhs, count);
		}
		constexpr inline static simd_register_ui32 bitwise_shift_right_c_ui32(simd_register_ui32 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_ui32>(lhs, count);
		}
		constexpr inline static simd_register_ui64 bitwise_shift_right_c_ui64(simd_register_ui64 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_ui64>(lhs, count);
		}
		constexpr inline static simd_register_f32 bitwise_shift_right_c_f32(simd_register_f32 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_f32>(lhs, count);
		}
		constexpr inline static simd_register_f64 bitwise_shift_right_c_f64(simd_register_f64 lhs, const Size count) noexcept {
			return bitwiseShiftRightC<simd_register_f64>(lhs, count);
		}

		//bitwise_shift_left
		constexpr inline static simd_register_i8 bitwise_shift_left_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 bitwise_shift_left_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 bitwise_shift_left_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 bitwise_shift_left_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_i64>(lhs, rhs);
		}
		constexpr inline static simd_register_ui8 bitwise_shift_left_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 bitwise_shift_left_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 bitwise_shift_left_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 bitwise_shift_left_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_ui64>(lhs, rhs);
		}
		constexpr inline static simd_register_f32 bitwise_shift_left_f32(simd_register_f32 lhs, simd_register_ui32 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_f32, simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 bitwise_shift_left_f64(simd_register_f64 lhs, simd_register_ui64 rhs) noexcept {
			return bitwiseShiftLeft<simd_register_f64, simd_register_ui64>(lhs, rhs);
		}

		//bitwise_shift_right
		constexpr inline static simd_register_i8 bitwise_shift_right_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return bitwiseShiftRight<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 bitwise_shift_right_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return bitwiseShiftRight<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 bitwise_shift_right_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return bitwiseShiftRight<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 bitwise_shift_right_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return bitwiseShiftRight<simd_register_i64>(lhs, rhs);
		}
		constexpr inline static simd_register_ui8 bitwise_shift_right_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return bitwiseShiftRight<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 bitwise_shift_right_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return bitwiseShiftRight<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 bitwise_shift_right_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return bitwiseShiftRight<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 bitwise_shift_right_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return bitwiseShiftRight<simd_register_ui64>(lhs, rhs);
		}
		constexpr inline static simd_register_f32 bitwise_shift_right_f32(simd_register_f32 lhs, simd_register_ui32 rhs) noexcept {
			return bitwiseShiftRight<simd_register_f32, simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 bitwise_shift_right_f64(simd_register_f64 lhs, simd_register_ui64 rhs) noexcept {
			return bitwiseShiftRight<simd_register_f64, simd_register_ui64>(lhs, rhs);
		}

		//bitwise_and
		constexpr inline static simd_register_i8 bitwise_and_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return bitwiseAnd<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 bitwise_and_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return bitwiseAnd<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 bitwise_and_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return bitwiseAnd<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 bitwise_and_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return bitwiseAnd<simd_register_i64>(lhs, rhs);
		}
		constexpr inline static simd_register_ui8 bitwise_and_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return bitwiseAnd<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 bitwise_and_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return bitwiseAnd<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 bitwise_and_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return bitwiseAnd<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 bitwise_and_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return bitwiseAnd<simd_register_ui64>(lhs, rhs);
		}
		constexpr inline static simd_register_f32 bitwise_and_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return bitwiseAnd<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 bitwise_and_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return bitwiseAnd<simd_register_f64>(lhs, rhs);
		}

		//bitwise_or
		constexpr inline static simd_register_i8 bitwise_or_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return bitwiseOr<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 bitwise_or_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return bitwiseOr<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 bitwise_or_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return bitwiseOr<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 bitwise_or_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return bitwiseOr<simd_register_i64>(lhs, rhs);
		}
		constexpr inline static simd_register_ui8 bitwise_or_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return bitwiseOr<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 bitwise_or_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return bitwiseOr<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 bitwise_or_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return bitwiseOr<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 bitwise_or_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return bitwiseOr<simd_register_ui64>(lhs, rhs);
		}
		constexpr inline static simd_register_f32 bitwise_or_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return bitwiseOr<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 bitwise_or_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return bitwiseOr<simd_register_f64>(lhs, rhs);
		}

		//bitwise_xor
		constexpr inline static simd_register_i8 bitwise_xor_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return bitwiseXor<simd_register_i8>(lhs, rhs);
		}
		constexpr inline static simd_register_i16 bitwise_xor_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return bitwiseXor<simd_register_i16>(lhs, rhs);
		}
		constexpr inline static simd_register_i32 bitwise_xor_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return bitwiseXor<simd_register_i32>(lhs, rhs);
		}
		constexpr inline static simd_register_i64 bitwise_xor_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return bitwiseXor<simd_register_i64>(lhs, rhs);
		}
		constexpr inline static simd_register_ui8 bitwise_xor_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return bitwiseXor<simd_register_ui8>(lhs, rhs);
		}
		constexpr inline static simd_register_ui16 bitwise_xor_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return bitwiseXor<simd_register_ui16>(lhs, rhs);
		}
		constexpr inline static simd_register_ui32 bitwise_xor_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return bitwiseXor<simd_register_ui32>(lhs, rhs);
		}
		constexpr inline static simd_register_ui64 bitwise_xor_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return bitwiseXor<simd_register_ui64>(lhs, rhs);
		}
		constexpr inline static simd_register_f32 bitwise_xor_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return bitwiseXor<simd_register_f32>(lhs, rhs);
		}
		constexpr inline static simd_register_f64 bitwise_xor_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return bitwiseXor<simd_register_f64>(lhs, rhs);
		}

		//bitwise_not
		constexpr inline static simd_register_i8 bitwise_not_i8(simd_register_i8 value) noexcept {
			return bitwiseNot<simd_register_i8>(value);
		}
		constexpr inline static simd_register_i16 bitwise_not_i16(simd_register_i16 value) noexcept {
			return bitwiseNot<simd_register_i16>(value);
		}
		constexpr inline static simd_register_i32 bitwise_not_i32(simd_register_i32 value) noexcept {
			return bitwiseNot<simd_register_i32>(value);
		}
		constexpr inline static simd_register_i64 bitwise_not_i64(simd_register_i64 value) noexcept {
			return bitwiseNot<simd_register_i64>(value);
		}
		constexpr inline static simd_register_ui8 bitwise_not_ui8(simd_register_ui8 value) noexcept {
			return bitwiseNot<simd_register_ui8>(value);
		}
		constexpr inline static simd_register_ui16 bitwise_not_ui16(simd_register_ui16 value) noexcept {
			return bitwiseNot<simd_register_ui16>(value);
		}
		constexpr inline static simd_register_ui32 bitwise_not_ui32(simd_register_ui32 value) noexcept {
			return bitwiseNot<simd_register_ui32>(value);
		}
		constexpr inline static simd_register_ui64 bitwise_not_ui64(simd_register_ui64 value) noexcept {
			return bitwiseNot<simd_register_ui64>(value);
		}
		constexpr inline static simd_register_f32 bitwise_not_f32(simd_register_f32 value) noexcept {
			return bitwiseNot<simd_register_f32>(value);
		}
		constexpr inline static simd_register_f64 bitwise_not_f64(simd_register_f64 value) noexcept {
			return bitwiseNot<simd_register_f64>(value);
		}

		//mmasked
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedBitwiseShiftLeftC(SimdRegisterType value, const Size count, SimdMaskType mmask) noexcept {
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			SimdRegisterType output = value;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					if constexpr (IsBuiltInFloatingPointC<simd_register_value_type>) {
						output[i] = natl::bitwiseShiftLeft(value[i], static_cast<UIntOfByteSize<sizeof(simd_register_value_type)>>(count));
					} else {
						output[i] = natl::bitwiseShiftLeft(value[i], static_cast<simd_register_value_type>(count));
					}
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedBitwiseShiftRightC(SimdRegisterType value, const Size count, SimdMaskType mmask) noexcept {
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			SimdRegisterType output = value;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					if constexpr (IsBuiltInFloatingPointC<simd_register_value_type>) {
						output[i] = natl::bitwiseShiftRight(value[i], static_cast<UIntOfByteSize<sizeof(simd_register_value_type)>>(count));
					} else {
						output[i] = natl::bitwiseShiftRight(value[i], static_cast<simd_register_value_type>(count));
					}
				}
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType, typename SimdMaskType>
		constexpr inline static LhsSimdRegisterType mmaskedBitwiseShiftLeft(LhsSimdRegisterType value, RhsSimdRegisterType count, SimdMaskType mmask) noexcept {
			LhsSimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseShiftLeft(value[i], count[i]);
				}
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType, typename SimdMaskType>
		constexpr inline static LhsSimdRegisterType mmaskedBitwiseShiftRight(LhsSimdRegisterType value, RhsSimdRegisterType count, SimdMaskType mmask) noexcept {
			LhsSimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseShiftRight(value[i], count[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedBitwiseAnd(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseAnd(lhs[i], rhs[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedBitwiseOr(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseOr(lhs[i], rhs[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedBitwiseXor(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseXor(lhs[i], rhs[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedBitwiseNot(SimdRegisterType value, SimdMaskType mmask) noexcept {
			SimdRegisterType output = value;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseNot(value[i]);
				}
			}
			return output;
		}

		//mmasked bitwise_shift_left_c
		constexpr inline static simd_register_i8 mmasked_bitwise_shift_left_c_i8(simd_register_i8 lhs, const Size count, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_i8, simd_mmask_i8>(lhs, count, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_shift_left_c_i16(simd_register_i16 lhs, const Size count, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_i16, simd_mmask_i16>(lhs, count, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_shift_left_c_i32(simd_register_i32 lhs, const Size count, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_i32, simd_mmask_i32>(lhs, count, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_shift_left_c_i64(simd_register_i64 lhs, const Size count, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_i64, simd_mmask_i64>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_shift_left_c_ui8(simd_register_ui8 lhs, const Size count, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_ui8, simd_mmask_ui8>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_shift_left_c_ui16(simd_register_ui16 lhs, const Size count, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_ui16, simd_mmask_ui16>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_shift_left_c_ui32(simd_register_ui32 lhs, const Size count, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_ui32, simd_mmask_ui32>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_shift_left_c_ui64(simd_register_ui64 lhs, const Size count, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_ui64, simd_mmask_ui64>(lhs, count, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_shift_left_c_f32(simd_register_f32 lhs, const Size count, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_f32, simd_mmask_f32>(lhs, count, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_shift_left_c_f64(simd_register_f64 lhs, const Size count, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseShiftLeftC<simd_register_f64, simd_mmask_f64>(lhs, count, mmask);
		}

		//mmasked bitwise_shift_right_c
		constexpr inline static simd_register_i8 mmasked_bitwise_shift_right_c_i8(simd_register_i8 lhs, const Size count, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_i8, simd_mmask_i8>(lhs, count, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_shift_right_c_i16(simd_register_i16 lhs, const Size count, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_i16, simd_mmask_i16>(lhs, count, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_shift_right_c_i32(simd_register_i32 lhs, const Size count, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_i32, simd_mmask_i32>(lhs, count, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_shift_right_c_i64(simd_register_i64 lhs, const Size count, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_i64, simd_mmask_i64>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_shift_right_c_ui8(simd_register_ui8 lhs, const Size count, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_ui8, simd_mmask_ui8>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_shift_right_c_ui16(simd_register_ui16 lhs, const Size count, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_ui16, simd_mmask_ui16>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_shift_right_c_ui32(simd_register_ui32 lhs, const Size count, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_ui32, simd_mmask_ui32>(lhs, count, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_shift_right_c_ui64(simd_register_ui64 lhs, const Size count, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_ui64, simd_mmask_ui64>(lhs, count, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_shift_right_c_f32(simd_register_f32 lhs, const Size count, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_f32, simd_mmask_f32>(lhs, count, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_shift_right_c_f64(simd_register_f64 lhs, const Size count, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseShiftRightC<simd_register_f64, simd_mmask_f64>(lhs, count, mmask);
		}

		//mmasked bitwise_not
		constexpr inline static simd_register_i8 mmasked_bitwise_not_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_not_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_not_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_not_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_not_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_not_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_not_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_not_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_not_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_not_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseNot<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmmasked src
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcBitwiseShiftLeftC(SimdRegisterType value, const Size count, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					if constexpr (IsBuiltInFloatingPointC<simd_register_value_type>) {
						output[i] = natl::bitwiseShiftLeft(value[i], static_cast<UIntOfByteSize<sizeof(simd_register_value_type)>>(count));
					} else {
						output[i] = natl::bitwiseShiftLeft(value[i], static_cast<simd_register_value_type>(count));
					}
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcBitwiseShiftRightC(SimdRegisterType value, const Size count, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					if constexpr (IsBuiltInFloatingPointC<simd_register_value_type>) {
						output[i] = natl::bitwiseShiftRight(value[i], static_cast<UIntOfByteSize<sizeof(simd_register_value_type)>>(count));
					} else {
						output[i] = natl::bitwiseShiftRight(value[i], static_cast<simd_register_value_type>(count));
					}
				}
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType, typename SimdMaskType>
		constexpr inline static LhsSimdRegisterType mmaskedSrcBitwiseShiftLeft(LhsSimdRegisterType value, RhsSimdRegisterType count, LhsSimdRegisterType src, SimdMaskType mmask) noexcept {
			LhsSimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseShiftLeft(value[i], count[i]);
				}
			}
			return output;
		}
		template<typename LhsSimdRegisterType, typename RhsSimdRegisterType, typename SimdMaskType>
		constexpr inline static LhsSimdRegisterType mmaskedSrcBitwiseShiftRight(LhsSimdRegisterType value, RhsSimdRegisterType count, LhsSimdRegisterType src, SimdMaskType mmask) noexcept {
			LhsSimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<LhsSimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseShiftRight(value[i], count[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcBitwiseAnd(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseAnd(lhs[i], rhs[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcBitwiseOr(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseOr(lhs[i], rhs[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcBitwiseXor(SimdRegisterType lhs, SimdRegisterType rhs, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseXor(lhs[i], rhs[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcBitwiseNot(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = natl::bitwiseNot(value[i]);
				}
			}
			return output;
		}


		//mmasked src bitwise_shift_left_c
		constexpr inline static simd_register_i8 mmasked_src_bitwise_shift_left_c_i8(simd_register_i8 lhs, const Size count, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_i8, simd_mmask_i8>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_shift_left_c_i16(simd_register_i16 lhs, const Size count, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_i16, simd_mmask_i16>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_shift_left_c_i32(simd_register_i32 lhs, const Size count, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_i32, simd_mmask_i32>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_shift_left_c_i64(simd_register_i64 lhs, const Size count, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_i64, simd_mmask_i64>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_shift_left_c_ui8(simd_register_ui8 lhs, const Size count, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_ui8, simd_mmask_ui8>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_shift_left_c_ui16(simd_register_ui16 lhs, const Size count, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_ui16, simd_mmask_ui16>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_shift_left_c_ui32(simd_register_ui32 lhs, const Size count, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_ui32, simd_mmask_ui32>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_shift_left_c_ui64(simd_register_ui64 lhs, const Size count, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_ui64, simd_mmask_ui64>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_shift_left_c_f32(simd_register_f32 lhs, const Size count, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_f32, simd_mmask_f32>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_shift_left_c_f64(simd_register_f64 lhs, const Size count, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeftC<simd_register_f64, simd_mmask_f64>(lhs, count, src, mmask);
		}

		//mmasked src bitwise_shift_right_c
		constexpr inline static simd_register_i8 mmasked_src_bitwise_shift_right_c_i8(simd_register_i8 lhs, const Size count, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_i8, simd_mmask_i8>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_shift_right_c_i16(simd_register_i16 lhs, const Size count, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_i16, simd_mmask_i16>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_shift_right_c_i32(simd_register_i32 lhs, const Size count, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_i32, simd_mmask_i32>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_shift_right_c_i64(simd_register_i64 lhs, const Size count, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_i64, simd_mmask_i64>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_shift_right_c_ui8(simd_register_ui8 lhs, const Size count, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_ui8, simd_mmask_ui8>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_shift_right_c_ui16(simd_register_ui16 lhs, const Size count, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_ui16, simd_mmask_ui16>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_shift_right_c_ui32(simd_register_ui32 lhs, const Size count, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_ui32, simd_mmask_ui32>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_shift_right_c_ui64(simd_register_ui64 lhs, const Size count, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_ui64, simd_mmask_ui64>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_shift_right_c_f32(simd_register_f32 lhs, const Size count, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_f32, simd_mmask_f32>(lhs, count, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_shift_right_c_f64(simd_register_f64 lhs, const Size count, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRightC<simd_register_f64, simd_mmask_f64>(lhs, count, src, mmask);
		}

		//mmasked bitwise_shift_left
		constexpr inline static simd_register_i8 mmasked_bitwise_shift_left_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_i8, simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_shift_left_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_i16, simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_shift_left_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_i32, simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_shift_left_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_i64, simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_shift_left_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_shift_left_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_shift_left_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_shift_left_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_shift_left_f32(simd_register_f32 lhs, simd_register_ui32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_f32, simd_register_ui32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_shift_left_f64(simd_register_f64 lhs, simd_register_ui64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseShiftLeft<simd_register_f64, simd_register_ui64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked bitwise_shift_right
		constexpr inline static simd_register_i8 mmasked_bitwise_shift_right_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_i8, simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_shift_right_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_i16, simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_shift_right_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_i32, simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_shift_right_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_i64, simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_shift_right_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_shift_right_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_shift_right_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_shift_right_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_shift_right_f32(simd_register_f32 lhs, simd_register_ui32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_f32, simd_register_ui32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_shift_right_f64(simd_register_f64 lhs, simd_register_ui64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseShiftRight<simd_register_f64, simd_register_ui64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked bitwise_and
		constexpr inline static simd_register_i8 mmasked_bitwise_and_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_and_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_and_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_and_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_and_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_and_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_and_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_and_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_and_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_and_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseAnd<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked bitwise_or
		constexpr inline static simd_register_i8 mmasked_bitwise_or_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_or_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_or_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_or_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_or_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_or_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_or_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_or_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_or_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_or_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseOr<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked bitwise_xor
		constexpr inline static simd_register_i8 mmasked_bitwise_xor_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_bitwise_xor_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_bitwise_xor_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_bitwise_xor_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_bitwise_xor_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_bitwise_xor_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_bitwise_xor_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_bitwise_xor_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_bitwise_xor_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_bitwise_xor_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitwiseXor<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src bitwise_shift_left
		constexpr inline static simd_register_i8 mmasked_src_bitwise_shift_left_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_i8, simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_shift_left_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_i16, simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_shift_left_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_i32, simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_shift_left_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_i64, simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_shift_left_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_shift_left_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_shift_left_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_shift_left_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_shift_left_f32(simd_register_f32 lhs, simd_register_ui32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_f32, simd_register_ui32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_shift_left_f64(simd_register_f64 lhs, simd_register_ui64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftLeft<simd_register_f64, simd_register_ui64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//mmasked src bitwise_shift_right
		constexpr inline static simd_register_i8 mmasked_src_bitwise_shift_right_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_i8, simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_shift_right_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_i16, simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_shift_right_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_i32, simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_shift_right_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_i64, simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_shift_right_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_shift_right_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_shift_right_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_shift_right_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_shift_right_f32(simd_register_f32 lhs, simd_register_ui32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_f32, simd_register_ui32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_shift_right_f64(simd_register_f64 lhs, simd_register_ui64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseShiftRight<simd_register_f64, simd_register_ui64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//mmasked src bitwise_and
		constexpr inline static simd_register_i8 mmasked_src_bitwise_and_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_and_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_and_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_and_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_and_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_and_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_and_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_and_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_and_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_and_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseAnd<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//mmasked src bitwise_or
		constexpr inline static simd_register_i8 mmasked_src_bitwise_or_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_or_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_or_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_or_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_or_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_or_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_or_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_or_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_or_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_or_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseOr<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//mmasked src bitwise_xor
		constexpr inline static simd_register_i8 mmasked_src_bitwise_xor_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_xor_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_xor_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_xor_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_xor_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_xor_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_xor_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_xor_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_xor_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_xor_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseXor<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//mmasked src bitwise_not
		constexpr inline static simd_register_i8 mmasked_src_bitwise_not_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_bitwise_not_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_bitwise_not_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_bitwise_not_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_bitwise_not_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_bitwise_not_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_bitwise_not_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_bitwise_not_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_bitwise_not_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_bitwise_not_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitwiseNot<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		template<typename SimdRegisterType, typename BitSimdRegisterType>
		constexpr inline static BitSimdRegisterType popcount(SimdRegisterType value) noexcept {
			BitSimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = static_cast<typename SimdRegisterToInfo<BitSimdRegisterType>::value_type>(natl::popcount(value[i]));
			}
			return output;
		}
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static BitSimdRegisterType mmaskedPopcount(SimdRegisterType value, SimdMaskType mmask) noexcept {
			BitSimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = static_cast<typename SimdRegisterToInfo<BitSimdRegisterType>::value_type>(natl::popcount(value[i]));
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static BitSimdRegisterType mmaskedSrcPopcount(SimdRegisterType value, BitSimdRegisterType src, SimdMaskType mmask) noexcept {
			BitSimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = static_cast<typename SimdRegisterToInfo<BitSimdRegisterType>::value_type>(natl::popcount(value[i]));
				}
			}
			return output;
		}

		//popcount
		constexpr inline static simd_register_i8 popcount_i8(simd_register_i8 value) noexcept {
			return popcount<simd_register_i8, simd_register_i8>(value);
		}
		constexpr inline static simd_register_i16 popcount_i16(simd_register_i16 value) noexcept {
			return popcount<simd_register_i16, simd_register_i16>(value);
		}
		constexpr inline static simd_register_i32 popcount_i32(simd_register_i32 value) noexcept {
			return popcount<simd_register_i32, simd_register_i32>(value);
		}
		constexpr inline static simd_register_i64 popcount_i64(simd_register_i64 value) noexcept {
			return popcount<simd_register_i64, simd_register_i64>(value);
		}
		constexpr inline static simd_register_ui8 popcount_ui8(simd_register_ui8 value) noexcept {
			return popcount<simd_register_ui8, simd_register_ui8>(value);
		}
		constexpr inline static simd_register_ui16 popcount_ui16(simd_register_ui16 value) noexcept {
			return popcount<simd_register_ui16, simd_register_ui16>(value);
		}
		constexpr inline static simd_register_ui32 popcount_ui32(simd_register_ui32 value) noexcept {
			return popcount<simd_register_ui32, simd_register_ui32>(value);
		}
		constexpr inline static simd_register_ui64 popcount_ui64(simd_register_ui64 value) noexcept {
			return popcount<simd_register_ui64, simd_register_ui64>(value);
		}
		constexpr inline static simd_register_ui32 popcount_f32(simd_register_f32 value) noexcept {
			return popcount<simd_register_f32, simd_register_ui32>(value);
		}
		constexpr inline static simd_register_ui64 popcount_f64(simd_register_f64 value) noexcept {
			return popcount<simd_register_f64, simd_register_ui64>(value);
		}

		//mmasked popcount
		constexpr inline static simd_register_i8 mmasked_popcount_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedPopcount<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_popcount_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedPopcount<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_popcount_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedPopcount<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_popcount_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedPopcount<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_popcount_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedPopcount<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_popcount_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedPopcount<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_popcount_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedPopcount<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_popcount_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedPopcount<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_popcount_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedPopcount<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_popcount_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedPopcount<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, mmask);
		}

		//mmasked src popcount
		constexpr inline static simd_register_i8 mmasked_src_popcount_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_popcount_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_popcount_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_popcount_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static simd_register_ui8 mmasked_src_popcount_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_popcount_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_popcount_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_popcount_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_popcount_f32(simd_register_f32 value, simd_register_ui32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_popcount_f64(simd_register_f64 value, simd_register_ui64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcPopcount<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, src, mmask);
		}

		//bitscan forward
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType bitscanForward(SimdRegisterType value, BitSimdRegisterType& indexDst) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, natl::bitscanForward(value[i], indexDst[i]));
			}
			return output;
		}
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedBitscanForward(SimdRegisterType value, BitSimdRegisterType& indexDst, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, natl::bitscanForward(value[i], indexDst[i]));
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcBitscanForward(SimdRegisterType value, BitSimdRegisterType& indexDst, BitSimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			indexDst = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, natl::bitscanForward(value[i], indexDst[i]));
				}
			}
			return output;
		}

		//bitscan_forward
		constexpr inline static simd_mmask_i8 bitscan_forward_i8(simd_register_i8 value, simd_register_i8& indexDst) noexcept {
			return bitscanForward<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, indexDst);
		}
		constexpr inline static simd_mmask_i16 bitscan_forward_i16(simd_register_i16 value, simd_register_i16& indexDst) noexcept {
			return bitscanForward<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, indexDst);
		}
		constexpr inline static simd_mmask_i32 bitscan_forward_i32(simd_register_i32 value, simd_register_i32& indexDst) noexcept {
			return bitscanForward<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, indexDst);
		}
		constexpr inline static simd_mmask_i64 bitscan_forward_i64(simd_register_i64 value, simd_register_i64& indexDst) noexcept {
			return bitscanForward<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui8 bitscan_forward_ui8(simd_register_ui8 value, simd_register_ui8& indexDst) noexcept {
			return bitscanForward<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui16 bitscan_forward_ui16(simd_register_ui16 value, simd_register_ui16& indexDst) noexcept {
			return bitscanForward<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui32 bitscan_forward_ui32(simd_register_ui32 value, simd_register_ui32& indexDst) noexcept {
			return bitscanForward<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui64 bitscan_forward_ui64(simd_register_ui64 value, simd_register_ui64& indexDst) noexcept {
			return bitscanForward<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, indexDst);
		}
		constexpr inline static simd_mmask_f32 bitscan_forward_f32(simd_register_f32 value, simd_register_ui32& indexDst) noexcept {
			return bitscanForward<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, indexDst);
		}
		constexpr inline static simd_mmask_f64 bitscan_forward_f64(simd_register_f64 value, simd_register_ui64& indexDst) noexcept {
			return bitscanForward<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, indexDst);
		}

		//mmasked bitscan_forward
		constexpr inline static simd_mmask_i8 mmasked_bitscan_forward_i8(simd_register_i8 value, simd_register_i8& indexDst, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_bitscan_forward_i16(simd_register_i16 value, simd_register_i16& indexDst, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_bitscan_forward_i32(simd_register_i32 value, simd_register_i32& indexDst, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_bitscan_forward_i64(simd_register_i64 value, simd_register_i64& indexDst, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui8 mmasked_bitscan_forward_ui8(simd_register_ui8 value, simd_register_ui8& indexDst, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_bitscan_forward_ui16(simd_register_ui16 value, simd_register_ui16& indexDst, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_bitscan_forward_ui32(simd_register_ui32 value, simd_register_ui32& indexDst, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_bitscan_forward_ui64(simd_register_ui64 value, simd_register_ui64& indexDst, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_f32 mmasked_bitscan_forward_f32(simd_register_f32 value, simd_register_ui32& indexDst, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_bitscan_forward_f64(simd_register_f64 value, simd_register_ui64& indexDst, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitscanForward<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, indexDst, mmask);
		}

		//mmasked src bitscan_forward
		constexpr inline static simd_mmask_i8 mmasked_src_bitscan_forward_i8(simd_register_i8 value, simd_register_i8& indexDst, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_bitscan_forward_i16(simd_register_i16 value, simd_register_i16& indexDst, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_bitscan_forward_i32(simd_register_i32 value, simd_register_i32& indexDst, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_bitscan_forward_i64(simd_register_i64 value, simd_register_i64& indexDst, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui8 mmasked_src_bitscan_forward_ui8(simd_register_ui8 value, simd_register_ui8& indexDst, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_bitscan_forward_ui16(simd_register_ui16 value, simd_register_ui16& indexDst, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_bitscan_forward_ui32(simd_register_ui32 value, simd_register_ui32& indexDst, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_bitscan_forward_ui64(simd_register_ui64 value, simd_register_ui64& indexDst, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_f32 mmasked_src_bitscan_forward_f32(simd_register_f32 value, simd_register_ui32& indexDst, simd_register_ui32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_bitscan_forward_f64(simd_register_f64 value, simd_register_ui64& indexDst, simd_register_ui64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitscanForward<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, indexDst, src, mmask);
		}

		//bitscan backward
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType bitscanBackward(SimdRegisterType value, BitSimdRegisterType& indexDst) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, natl::bitscanBackward(value[i], indexDst[i]));
			}
			return output;
		}
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedBitscanBackward(SimdRegisterType value, BitSimdRegisterType& indexDst, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, natl::bitscanBackward(value[i], indexDst[i]));
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename BitSimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcBitscanBackward(SimdRegisterType value, BitSimdRegisterType& indexDst, BitSimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			indexDst = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, natl::bitscanBackward(value[i], indexDst[i]));
				}
			}
			return output;
		}

		//bitscan_backward
		constexpr inline static simd_mmask_i8 bitscan_backward_i8(simd_register_i8 value, simd_register_i8& indexDst) noexcept {
			return bitscanBackward<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, indexDst);
		}
		constexpr inline static simd_mmask_i16 bitscan_backward_i16(simd_register_i16 value, simd_register_i16& indexDst) noexcept {
			return bitscanBackward<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, indexDst);
		}
		constexpr inline static simd_mmask_i32 bitscan_backward_i32(simd_register_i32 value, simd_register_i32& indexDst) noexcept {
			return bitscanBackward<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, indexDst);
		}
		constexpr inline static simd_mmask_i64 bitscan_backward_i64(simd_register_i64 value, simd_register_i64& indexDst) noexcept {
			return bitscanBackward<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui8 bitscan_backward_ui8(simd_register_ui8 value, simd_register_ui8& indexDst) noexcept {
			return bitscanBackward<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui16 bitscan_backward_ui16(simd_register_ui16 value, simd_register_ui16& indexDst) noexcept {
			return bitscanBackward<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui32 bitscan_backward_ui32(simd_register_ui32 value, simd_register_ui32& indexDst) noexcept {
			return bitscanBackward<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, indexDst);
		}
		constexpr inline static simd_mmask_ui64 bitscan_backward_ui64(simd_register_ui64 value, simd_register_ui64& indexDst) noexcept {
			return bitscanBackward<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, indexDst);
		}
		constexpr inline static simd_mmask_f32 bitscan_backward_f32(simd_register_f32 value, simd_register_ui32& indexDst) noexcept {
			return bitscanBackward<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, indexDst);
		}
		constexpr inline static simd_mmask_f64 bitscan_backward_f64(simd_register_f64 value, simd_register_ui64& indexDst) noexcept {
			return bitscanBackward<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, indexDst);
		}

		//mmasked bitscan_backward
		constexpr inline static simd_mmask_i8 mmasked_bitscan_backward_i8(simd_register_i8 value, simd_register_i8& indexDst, simd_mmask_i8 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_bitscan_backward_i16(simd_register_i16 value, simd_register_i16& indexDst, simd_mmask_i16 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_bitscan_backward_i32(simd_register_i32 value, simd_register_i32& indexDst, simd_mmask_i32 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_bitscan_backward_i64(simd_register_i64 value, simd_register_i64& indexDst, simd_mmask_i64 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui8 mmasked_bitscan_backward_ui8(simd_register_ui8 value, simd_register_ui8& indexDst, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_bitscan_backward_ui16(simd_register_ui16 value, simd_register_ui16& indexDst, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_bitscan_backward_ui32(simd_register_ui32 value, simd_register_ui32& indexDst, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_bitscan_backward_ui64(simd_register_ui64 value, simd_register_ui64& indexDst, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_f32 mmasked_bitscan_backward_f32(simd_register_f32 value, simd_register_ui32& indexDst, simd_mmask_f32 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, indexDst, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_bitscan_backward_f64(simd_register_f64 value, simd_register_ui64& indexDst, simd_mmask_f64 mmask) noexcept {
			return mmaskedBitscanBackward<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, indexDst, mmask);
		}

		//mmasked src bitscan_backward
		constexpr inline static simd_mmask_i8 mmasked_src_bitscan_backward_i8(simd_register_i8 value, simd_register_i8& indexDst, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_i8, simd_register_i8, simd_mmask_i8>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_bitscan_backward_i16(simd_register_i16 value, simd_register_i16& indexDst, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_i16, simd_register_i16, simd_mmask_i16>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_bitscan_backward_i32(simd_register_i32 value, simd_register_i32& indexDst, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_i32, simd_register_i32, simd_mmask_i32>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_bitscan_backward_i64(simd_register_i64 value, simd_register_i64& indexDst, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_i64, simd_register_i64, simd_mmask_i64>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui8 mmasked_src_bitscan_backward_ui8(simd_register_ui8 value, simd_register_ui8& indexDst, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_ui8, simd_register_ui8, simd_mmask_ui8>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_bitscan_backward_ui16(simd_register_ui16 value, simd_register_ui16& indexDst, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_ui16, simd_register_ui16, simd_mmask_ui16>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_bitscan_backward_ui32(simd_register_ui32 value, simd_register_ui32& indexDst, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_ui32, simd_register_ui32, simd_mmask_ui32>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_bitscan_backward_ui64(simd_register_ui64 value, simd_register_ui64& indexDst, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_ui64, simd_register_ui64, simd_mmask_ui64>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_f32 mmasked_src_bitscan_backward_f32(simd_register_f32 value, simd_register_ui32& indexDst, simd_register_ui32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_f32, simd_register_ui32, simd_mmask_f32>(value, indexDst, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_bitscan_backward_f64(simd_register_f64 value, simd_register_ui64& indexDst, simd_register_ui64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBitscanBackward<simd_register_f64, simd_register_ui64, simd_mmask_f64>(value, indexDst, src, mmask);
		}

		//reduce//
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type reduceMin(SimdRegisterType value) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = value[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output = natl::min(output, value[i]);
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type reduceMax(SimdRegisterType value) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = value[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output = natl::max(output, value[i]);
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type reduceAdd(SimdRegisterType value) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = value[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output = output + value[i];
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type reduceMul(SimdRegisterType value) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = value[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output = output * value[i];
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type reduceAnd(SimdRegisterType value) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = value[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output = natl::bitwiseAnd(output, value[i]);
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type reduceOr(SimdRegisterType value) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = value[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output = natl::bitwiseOr(output, value[i]);
			}
			return output;
		}
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type reduceXor(SimdRegisterType value) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = value[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output = natl::bitwiseXor(output, value[i]);
			}
			return output;
		}
		
		//reduce_min
		constexpr inline static i8 reduce_min_i8(simd_register_i8 value) noexcept {
			return reduceMin<simd_register_i8>(value);
		}
		constexpr inline static i16 reduce_min_i16(simd_register_i16 value) noexcept {
			return reduceMin<simd_register_i16>(value);
		}
		constexpr inline static i32 reduce_min_i32(simd_register_i32 value) noexcept {
			return reduceMin<simd_register_i32>(value);
		}
		constexpr inline static i64 reduce_min_i64(simd_register_i64 value) noexcept {
			return reduceMin<simd_register_i64>(value);
		}
		constexpr inline static ui8 reduce_min_ui8(simd_register_ui8 value) noexcept {
			return reduceMin<simd_register_ui8>(value);
		}
		constexpr inline static ui16 reduce_min_ui16(simd_register_ui16 value) noexcept {
			return reduceMin<simd_register_ui16>(value);
		}
		constexpr inline static ui32 reduce_min_ui32(simd_register_ui32 value) noexcept {
			return reduceMin<simd_register_ui32>(value);
		}
		constexpr inline static ui64 reduce_min_ui64(simd_register_ui64 value) noexcept {
			return reduceMin<simd_register_ui64>(value);
		}
		constexpr inline static f32 reduce_min_f32(simd_register_f32 value) noexcept {
			return reduceMin<simd_register_f32>(value);
		}
		constexpr inline static f64 reduce_min_f64(simd_register_f64 value) noexcept {
			return reduceMin<simd_register_f64>(value);
		}

		//reduce_max
		constexpr inline static i8 reduce_max_i8(simd_register_i8 value) noexcept {
			return reduceMax<simd_register_i8>(value);
		}
		constexpr inline static i16 reduce_max_i16(simd_register_i16 value) noexcept {
			return reduceMax<simd_register_i16>(value);
		}
		constexpr inline static i32 reduce_max_i32(simd_register_i32 value) noexcept {
			return reduceMax<simd_register_i32>(value);
		}
		constexpr inline static i64 reduce_max_i64(simd_register_i64 value) noexcept {
			return reduceMax<simd_register_i64>(value);
		}
		constexpr inline static ui8 reduce_max_ui8(simd_register_ui8 value) noexcept {
			return reduceMax<simd_register_ui8>(value);
		}
		constexpr inline static ui16 reduce_max_ui16(simd_register_ui16 value) noexcept {
			return reduceMax<simd_register_ui16>(value);
		}
		constexpr inline static ui32 reduce_max_ui32(simd_register_ui32 value) noexcept {
			return reduceMax<simd_register_ui32>(value);
		}
		constexpr inline static ui64 reduce_max_ui64(simd_register_ui64 value) noexcept {
			return reduceMax<simd_register_ui64>(value);
		}
		constexpr inline static f32 reduce_max_f32(simd_register_f32 value) noexcept {
			return reduceMax<simd_register_f32>(value);
		}
		constexpr inline static f64 reduce_max_f64(simd_register_f64 value) noexcept {
			return reduceMax<simd_register_f64>(value);
		}

		//reduce_add
		constexpr inline static i8 reduce_add_i8(simd_register_i8 value) noexcept {
			return reduceAdd<simd_register_i8>(value);
		}
		constexpr inline static i16 reduce_add_i16(simd_register_i16 value) noexcept {
			return reduceAdd<simd_register_i16>(value);
		}
		constexpr inline static i32 reduce_add_i32(simd_register_i32 value) noexcept {
			return reduceAdd<simd_register_i32>(value);
		}
		constexpr inline static i64 reduce_add_i64(simd_register_i64 value) noexcept {
			return reduceAdd<simd_register_i64>(value);
		}
		constexpr inline static ui8 reduce_add_ui8(simd_register_ui8 value) noexcept {
			return reduceAdd<simd_register_ui8>(value);
		}
		constexpr inline static ui16 reduce_add_ui16(simd_register_ui16 value) noexcept {
			return reduceAdd<simd_register_ui16>(value);
		}
		constexpr inline static ui32 reduce_add_ui32(simd_register_ui32 value) noexcept {
			return reduceAdd<simd_register_ui32>(value);
		}
		constexpr inline static ui64 reduce_add_ui64(simd_register_ui64 value) noexcept {
			return reduceAdd<simd_register_ui64>(value);
		}
		constexpr inline static f32 reduce_add_f32(simd_register_f32 value) noexcept {
			return reduceAdd<simd_register_f32>(value);
		}
		constexpr inline static f64 reduce_add_f64(simd_register_f64 value) noexcept {
			return reduceAdd<simd_register_f64>(value);
		}

		//reduce_mul
		constexpr inline static i8 reduce_mul_i8(simd_register_i8 value) noexcept {
			return reduceMul<simd_register_i8>(value);
		}
		constexpr inline static i16 reduce_mul_i16(simd_register_i16 value) noexcept {
			return reduceMul<simd_register_i16>(value);
		}
		constexpr inline static i32 reduce_mul_i32(simd_register_i32 value) noexcept {
			return reduceMul<simd_register_i32>(value);
		}
		constexpr inline static i64 reduce_mul_i64(simd_register_i64 value) noexcept {
			return reduceMul<simd_register_i64>(value);
		}
		constexpr inline static ui8 reduce_mul_ui8(simd_register_ui8 value) noexcept {
			return reduceMul<simd_register_ui8>(value);
		}
		constexpr inline static ui16 reduce_mul_ui16(simd_register_ui16 value) noexcept {
			return reduceMul<simd_register_ui16>(value);
		}
		constexpr inline static ui32 reduce_mul_ui32(simd_register_ui32 value) noexcept {
			return reduceMul<simd_register_ui32>(value);
		}
		constexpr inline static ui64 reduce_mul_ui64(simd_register_ui64 value) noexcept {
			return reduceMul<simd_register_ui64>(value);
		}
		constexpr inline static f32 reduce_mul_f32(simd_register_f32 value) noexcept {
			return reduceMul<simd_register_f32>(value);
		}
		constexpr inline static f64 reduce_mul_f64(simd_register_f64 value) noexcept {
			return reduceMul<simd_register_f64>(value);
		}

		//reduce_and
		constexpr inline static i8 reduce_and_i8(simd_register_i8 value) noexcept {
			return reduceAnd<simd_register_i8>(value);
		}
		constexpr inline static i16 reduce_and_i16(simd_register_i16 value) noexcept {
			return reduceAnd<simd_register_i16>(value);
		}
		constexpr inline static i32 reduce_and_i32(simd_register_i32 value) noexcept {
			return reduceAnd<simd_register_i32>(value);
		}
		constexpr inline static i64 reduce_and_i64(simd_register_i64 value) noexcept {
			return reduceAnd<simd_register_i64>(value);
		}
		constexpr inline static ui8 reduce_and_ui8(simd_register_ui8 value) noexcept {
			return reduceAnd<simd_register_ui8>(value);
		}
		constexpr inline static ui16 reduce_and_ui16(simd_register_ui16 value) noexcept {
			return reduceAnd<simd_register_ui16>(value);
		}
		constexpr inline static ui32 reduce_and_ui32(simd_register_ui32 value) noexcept {
			return reduceAnd<simd_register_ui32>(value);
		}
		constexpr inline static ui64 reduce_and_ui64(simd_register_ui64 value) noexcept {
			return reduceAnd<simd_register_ui64>(value);
		}
		constexpr inline static f32 reduce_and_f32(simd_register_f32 value) noexcept {
			return reduceAnd<simd_register_f32>(value);
		}
		constexpr inline static f64 reduce_and_f64(simd_register_f64 value) noexcept {
			return reduceAnd<simd_register_f64>(value);
		}

		//reduce_or
		constexpr inline static i8 reduce_or_i8(simd_register_i8 value) noexcept {
			return reduceOr<simd_register_i8>(value);
		}
		constexpr inline static i16 reduce_or_i16(simd_register_i16 value) noexcept {
			return reduceOr<simd_register_i16>(value);
		}
		constexpr inline static i32 reduce_or_i32(simd_register_i32 value) noexcept {
			return reduceOr<simd_register_i32>(value);
		}
		constexpr inline static i64 reduce_or_i64(simd_register_i64 value) noexcept {
			return reduceOr<simd_register_i64>(value);
		}
		constexpr inline static ui8 reduce_or_ui8(simd_register_ui8 value) noexcept {
			return reduceOr<simd_register_ui8>(value);
		}
		constexpr inline static ui16 reduce_or_ui16(simd_register_ui16 value) noexcept {
			return reduceOr<simd_register_ui16>(value);
		}
		constexpr inline static ui32 reduce_or_ui32(simd_register_ui32 value) noexcept {
			return reduceOr<simd_register_ui32>(value);
		}
		constexpr inline static ui64 reduce_or_ui64(simd_register_ui64 value) noexcept {
			return reduceOr<simd_register_ui64>(value);
		}
		constexpr inline static f32 reduce_or_f32(simd_register_f32 value) noexcept {
			return reduceOr<simd_register_f32>(value);
		}
		constexpr inline static f64 reduce_or_f64(simd_register_f64 value) noexcept {
			return reduceOr<simd_register_f64>(value);
		}

		//reduce_xor
		constexpr inline static i8 reduce_xor_i8(simd_register_i8 value) noexcept {
			return reduceXor<simd_register_i8>(value);
		}
		constexpr inline static i16 reduce_xor_i16(simd_register_i16 value) noexcept {
			return reduceXor<simd_register_i16>(value);
		}
		constexpr inline static i32 reduce_xor_i32(simd_register_i32 value) noexcept {
			return reduceXor<simd_register_i32>(value);
		}
		constexpr inline static i64 reduce_xor_i64(simd_register_i64 value) noexcept {
			return reduceXor<simd_register_i64>(value);
		}
		constexpr inline static ui8 reduce_xor_ui8(simd_register_ui8 value) noexcept {
			return reduceXor<simd_register_ui8>(value);
		}
		constexpr inline static ui16 reduce_xor_ui16(simd_register_ui16 value) noexcept {
			return reduceXor<simd_register_ui16>(value);
		}
		constexpr inline static ui32 reduce_xor_ui32(simd_register_ui32 value) noexcept {
			return reduceXor<simd_register_ui32>(value);
		}
		constexpr inline static ui64 reduce_xor_ui64(simd_register_ui64 value) noexcept {
			return reduceXor<simd_register_ui64>(value);
		}
		constexpr inline static f32 reduce_xor_f32(simd_register_f32 value) noexcept {
			return reduceXor<simd_register_f32>(value);
		}
		constexpr inline static f64 reduce_xor_f64(simd_register_f64 value) noexcept {
			return reduceXor<simd_register_f64>(value);
		}

		//mmasked reduce 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedReduceMin(SimdRegisterType value, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			Size index = 0;

			while (!mmask[index]) {
				index += 1;
				if (index == SimdRegisterToInfo<SimdRegisterType>::count()) { return 0; }
			}

			simd_register_value_type output = value[index];
			for (; index < SimdRegisterToInfo<SimdRegisterType>::count(); index++) {
				if (mmask[index]) {
					output = natl::min(output, value[index]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedReduceMax(SimdRegisterType value, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			Size index = 0;

			while (!mmask[index]) {
				index += 1;
				if (index == SimdRegisterToInfo<SimdRegisterType>::count()) { return 0; }
			}

			simd_register_value_type output = value[index];
			for (; index < SimdRegisterToInfo<SimdRegisterType>::count(); index++) {
				if (mmask[index]) {
					output = natl::max(output, value[index]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedReduceAdd(SimdRegisterType value, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			Size index = 0;

			while (!mmask[index]) {
				index += 1;
				if (index == SimdRegisterToInfo<SimdRegisterType>::count()) { return 0; }
			}

			simd_register_value_type output = value[index];
			for (; index < SimdRegisterToInfo<SimdRegisterType>::count(); index++) {
				if (mmask[index]) {
					output = output + value[index];
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedReduceMul(SimdRegisterType value, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			Size index = 0;

			while (!mmask[index]) {
				index += 1;
				if (index == SimdRegisterToInfo<SimdRegisterType>::count()) { return 0; }
			}

			simd_register_value_type output = value[index];
			for (; index < SimdRegisterToInfo<SimdRegisterType>::count(); index++) {
				if (mmask[index]) {
					output = output * value[index];
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedReduceAnd(SimdRegisterType value, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			Size index = 0;

			while (!mmask[index]) {
				index += 1;
				if (index == SimdRegisterToInfo<SimdRegisterType>::count()) { return 0; }
			}

			simd_register_value_type output = value[index];
			for (; index < SimdRegisterToInfo<SimdRegisterType>::count(); index++) {
				if (mmask[index]) {
					output = natl::bitwiseAnd(output, value[index]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedReduceOr(SimdRegisterType value, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			Size index = 0;

			while (!mmask[index]) {
				index += 1;
				if (index == SimdRegisterToInfo<SimdRegisterType>::count()) { return 0; }
			}

			simd_register_value_type output = value[index];
			for (; index < SimdRegisterToInfo<SimdRegisterType>::count(); index++) {
				if (mmask[index]) {
					output = natl::bitwiseOr(output, value[index]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedReduceXor(SimdRegisterType value, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			Size index = 0;

			while (!mmask[index]) {
				index += 1;
				if (index == SimdRegisterToInfo<SimdRegisterType>::count()) { return 0; }
			}

			simd_register_value_type output = value[index];
			for (; index < SimdRegisterToInfo<SimdRegisterType>::count(); index++) {
				if (mmask[index]) {
					output = natl::bitwiseXor(output, value[index]);
				}
			}
			return output;
		}

		//mmasked reduce_min
		constexpr inline static i8 mmasked_reduce_min_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedReduceMin<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static i16 mmasked_reduce_min_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedReduceMin<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static i32 mmasked_reduce_min_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedReduceMin<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static i64 mmasked_reduce_min_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedReduceMin<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static ui8 mmasked_reduce_min_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedReduceMin<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static ui16 mmasked_reduce_min_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedReduceMin<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static ui32 mmasked_reduce_min_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedReduceMin<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static ui64 mmasked_reduce_min_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedReduceMin<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static f32 mmasked_reduce_min_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedReduceMin<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static f64 mmasked_reduce_min_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedReduceMin<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked reduce_max
		constexpr inline static i8 mmasked_reduce_max_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedReduceMax<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static i16 mmasked_reduce_max_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedReduceMax<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static i32 mmasked_reduce_max_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedReduceMax<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static i64 mmasked_reduce_max_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedReduceMax<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static ui8 mmasked_reduce_max_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedReduceMax<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static ui16 mmasked_reduce_max_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedReduceMax<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static ui32 mmasked_reduce_max_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedReduceMax<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static ui64 mmasked_reduce_max_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedReduceMax<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static f32 mmasked_reduce_max_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedReduceMax<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static f64 mmasked_reduce_max_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedReduceMax<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked reduce_add
		constexpr inline static i8 mmasked_reduce_add_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static i16 mmasked_reduce_add_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static i32 mmasked_reduce_add_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static i64 mmasked_reduce_add_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static ui8 mmasked_reduce_add_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static ui16 mmasked_reduce_add_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static ui32 mmasked_reduce_add_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static ui64 mmasked_reduce_add_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static f32 mmasked_reduce_add_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static f64 mmasked_reduce_add_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedReduceAdd<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked reduce_mul
		constexpr inline static i8 mmasked_reduce_mul_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedReduceMul<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static i16 mmasked_reduce_mul_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedReduceMul<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static i32 mmasked_reduce_mul_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedReduceMul<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static i64 mmasked_reduce_mul_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedReduceMul<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static ui8 mmasked_reduce_mul_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedReduceMul<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static ui16 mmasked_reduce_mul_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedReduceMul<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static ui32 mmasked_reduce_mul_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedReduceMul<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static ui64 mmasked_reduce_mul_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedReduceMul<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static f32 mmasked_reduce_mul_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedReduceMul<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static f64 mmasked_reduce_mul_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedReduceMul<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked reduce_and
		constexpr inline static i8 mmasked_reduce_and_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static i16 mmasked_reduce_and_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static i32 mmasked_reduce_and_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static i64 mmasked_reduce_and_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static ui8 mmasked_reduce_and_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static ui16 mmasked_reduce_and_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static ui32 mmasked_reduce_and_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static ui64 mmasked_reduce_and_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static f32 mmasked_reduce_and_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static f64 mmasked_reduce_and_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedReduceAnd<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked reduce_or
		constexpr inline static i8 mmasked_reduce_or_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedReduceOr<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static i16 mmasked_reduce_or_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedReduceOr<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static i32 mmasked_reduce_or_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedReduceOr<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static i64 mmasked_reduce_or_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedReduceOr<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static ui8 mmasked_reduce_or_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedReduceOr<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static ui16 mmasked_reduce_or_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedReduceOr<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static ui32 mmasked_reduce_or_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedReduceOr<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static ui64 mmasked_reduce_or_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedReduceOr<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static f32 mmasked_reduce_or_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedReduceOr<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static f64 mmasked_reduce_or_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedReduceOr<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked reduce_xor
		constexpr inline static i8 mmasked_reduce_xor_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedReduceXor<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static i16 mmasked_reduce_xor_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedReduceXor<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static i32 mmasked_reduce_xor_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedReduceXor<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static i64 mmasked_reduce_xor_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedReduceXor<simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static ui8 mmasked_reduce_xor_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedReduceXor<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static ui16 mmasked_reduce_xor_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedReduceXor<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static ui32 mmasked_reduce_xor_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedReduceXor<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static ui64 mmasked_reduce_xor_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedReduceXor<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static f32 mmasked_reduce_xor_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedReduceXor<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static f64 mmasked_reduce_xor_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedReduceXor<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked src reduce 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedSrcReduceMin(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = mmask[0] ? value[0] : src[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output = natl::min(output, value[i]);
				} else {
					output = natl::min(output, src[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedSrcReduceMax(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = mmask[0] ? value[0] : src[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output = natl::max(output, value[i]);
				} else {
					output = natl::max(output, src[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedSrcReduceAdd(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = mmask[0] ? value[0] : src[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output = output + value[i];
				} else {
					output = output + src[i];
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedSrcReduceMul(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = mmask[0] ? value[0] : src[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output = output * value[i];
				} else {
					output = output * src[i];
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedSrcReduceAnd(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = mmask[0] ? value[0] : src[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output = natl::bitwiseAnd(output, value[i]);
				} else {
					output = natl::bitwiseAnd(output, src[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedSrcReduceOr(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = mmask[0] ? value[0] : src[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output = natl::bitwiseOr(output, value[i]);
				} else {
					output = natl::bitwiseOr(output, src[i]);
				}
			}
			return output;
		}
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterToInfo<SimdRegisterType>::value_type mmaskedSrcReduceXor(SimdRegisterType value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			simd_register_value_type output = mmask[0] ? value[0] : src[0];
			for (Size i = 1; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output = natl::bitwiseXor(output, value[i]);
				} else {
					output = natl::bitwiseXor(output, src[i]);
				}
			}
			return output;
		}

		//mmasked src reduce_min
		constexpr inline static i8 mmasked_src_reduce_min_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static i16 mmasked_src_reduce_min_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static i32 mmasked_src_reduce_min_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static i64 mmasked_src_reduce_min_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static ui8 mmasked_src_reduce_min_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static ui16 mmasked_src_reduce_min_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static ui32 mmasked_src_reduce_min_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static ui64 mmasked_src_reduce_min_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static f32 mmasked_src_reduce_min_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static f64 mmasked_src_reduce_min_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcReduceMin<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//mmasked src reduce_max
		constexpr inline static i8 mmasked_src_reduce_max_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static i16 mmasked_src_reduce_max_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static i32 mmasked_src_reduce_max_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static i64 mmasked_src_reduce_max_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static ui8 mmasked_src_reduce_max_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static ui16 mmasked_src_reduce_max_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static ui32 mmasked_src_reduce_max_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static ui64 mmasked_src_reduce_max_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static f32 mmasked_src_reduce_max_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static f64 mmasked_src_reduce_max_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcReduceMax<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//mmasked src reduce_add
		constexpr inline static i8 mmasked_src_reduce_add_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static i16 mmasked_src_reduce_add_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static i32 mmasked_src_reduce_add_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static i64 mmasked_src_reduce_add_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static ui8 mmasked_src_reduce_add_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static ui16 mmasked_src_reduce_add_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static ui32 mmasked_src_reduce_add_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static ui64 mmasked_src_reduce_add_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static f32 mmasked_src_reduce_add_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static f64 mmasked_src_reduce_add_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcReduceAdd<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//mmasked src reduce_mul
		constexpr inline static i8 mmasked_src_reduce_mul_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static i16 mmasked_src_reduce_mul_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static i32 mmasked_src_reduce_mul_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static i64 mmasked_src_reduce_mul_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static ui8 mmasked_src_reduce_mul_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static ui16 mmasked_src_reduce_mul_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static ui32 mmasked_src_reduce_mul_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static ui64 mmasked_src_reduce_mul_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static f32 mmasked_src_reduce_mul_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static f64 mmasked_src_reduce_mul_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcReduceMul<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//mmasked src reduce_and
		constexpr inline static i8 mmasked_src_reduce_and_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static i16 mmasked_src_reduce_and_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static i32 mmasked_src_reduce_and_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static i64 mmasked_src_reduce_and_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static ui8 mmasked_src_reduce_and_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static ui16 mmasked_src_reduce_and_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static ui32 mmasked_src_reduce_and_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static ui64 mmasked_src_reduce_and_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static f32 mmasked_src_reduce_and_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static f64 mmasked_src_reduce_and_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcReduceAnd<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//mmasked src reduce_or
		constexpr inline static i8 mmasked_src_reduce_or_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static i16 mmasked_src_reduce_or_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static i32 mmasked_src_reduce_or_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static i64 mmasked_src_reduce_or_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static ui8 mmasked_src_reduce_or_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static ui16 mmasked_src_reduce_or_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static ui32 mmasked_src_reduce_or_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static ui64 mmasked_src_reduce_or_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static f32 mmasked_src_reduce_or_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static f64 mmasked_src_reduce_or_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcReduceOr<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//mmasked src reduce_xor
		constexpr inline static i8 mmasked_src_reduce_xor_i8(simd_register_i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static i16 mmasked_src_reduce_xor_i16(simd_register_i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static i32 mmasked_src_reduce_xor_i32(simd_register_i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static i64 mmasked_src_reduce_xor_i64(simd_register_i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static ui8 mmasked_src_reduce_xor_ui8(simd_register_ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static ui16 mmasked_src_reduce_xor_ui16(simd_register_ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static ui32 mmasked_src_reduce_xor_ui32(simd_register_ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static ui64 mmasked_src_reduce_xor_ui64(simd_register_ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static f32 mmasked_src_reduce_xor_f32(simd_register_f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static f64 mmasked_src_reduce_xor_f64(simd_register_f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcReduceXor<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//compare//

		//compare equal
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType compareEqual(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, lhs[i] == rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 compare_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return compareEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i16 compare_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return compareEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i32 compare_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return compareEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i64 compare_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return compareEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_ui8 compare_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return compareEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui16 compare_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return compareEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui32 compare_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return compareEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui64 compare_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return compareEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_f32 compare_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return compareEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_f64 compare_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return compareEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs);
		}

		//mmasked compare equal
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedCompareEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] == rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_compare_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_compare_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_compare_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_compare_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_compare_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_compare_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_compare_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_compare_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_compare_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_compare_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedCompareEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src compare equal
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcCompareEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType src, SimdMaskType mmask) noexcept {
			SimdMaskType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] == rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_src_compare_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_compare_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_compare_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_compare_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_src_compare_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_compare_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_compare_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_compare_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_src_compare_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_compare_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcCompareEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//compare not equal 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType compareNotEqual(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, lhs[i] != rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 compare_not_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return compareNotEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i16 compare_not_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return compareNotEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i32 compare_not_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return compareNotEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i64 compare_not_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return compareNotEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_ui8 compare_not_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return compareNotEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui16 compare_not_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return compareNotEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui32 compare_not_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return compareNotEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui64 compare_not_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return compareNotEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_f32 compare_not_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return compareNotEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_f64 compare_not_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return compareNotEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs);
		}

		//mmasked compare not equal
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedCompareNotEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] != rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_compare_not_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_compare_not_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_compare_not_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_compare_not_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_compare_not_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_compare_not_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_compare_not_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_compare_not_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_compare_not_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_compare_not_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedCompareNotEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src compare not equal
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcCompareNotEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType src, SimdMaskType mmask) noexcept {
			SimdMaskType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] != rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_src_compare_not_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_compare_not_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_compare_not_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_compare_not_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_src_compare_not_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_compare_not_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_compare_not_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_compare_not_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_src_compare_not_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_compare_not_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcCompareNotEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//compare less than 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType compareLessThen(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, lhs[i] < rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 compare_less_than_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return compareLessThen<simd_register_i8, simd_mmask_i8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i16 compare_less_than_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return compareLessThen<simd_register_i16, simd_mmask_i16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i32 compare_less_than_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return compareLessThen<simd_register_i32, simd_mmask_i32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i64 compare_less_than_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return compareLessThen<simd_register_i64, simd_mmask_i64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_ui8 compare_less_than_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return compareLessThen<simd_register_ui8, simd_mmask_ui8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui16 compare_less_than_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return compareLessThen<simd_register_ui16, simd_mmask_ui16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui32 compare_less_than_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return compareLessThen<simd_register_ui32, simd_mmask_ui32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui64 compare_less_than_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return compareLessThen<simd_register_ui64, simd_mmask_ui64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_f32 compare_less_than_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return compareLessThen<simd_register_f32, simd_mmask_f32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_f64 compare_less_than_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return compareLessThen<simd_register_f64, simd_mmask_f64>(lhs, rhs);
		}

		//mmasked compare less than
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedCompareLessThen(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] < rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_compare_less_than_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_compare_less_than_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_compare_less_than_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_compare_less_than_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_compare_less_than_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_compare_less_than_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_compare_less_than_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_compare_less_than_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_compare_less_than_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_compare_less_than_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedCompareLessThen<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src compare less than 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcCompareLessThen(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType src, SimdMaskType mmask) noexcept {
			SimdMaskType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] < rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_src_compare_less_than_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_compare_less_than_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_compare_less_than_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_compare_less_than_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_src_compare_less_than_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_compare_less_than_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_compare_less_than_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_compare_less_than_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_src_compare_less_than_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_compare_less_than_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcCompareLessThen<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//compare greater than 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType compareGreaterThan(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, lhs[i] > rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 compare_greater_than_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return compareGreaterThan<simd_register_i8, simd_mmask_i8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i16 compare_greater_than_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return compareGreaterThan<simd_register_i16, simd_mmask_i16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i32 compare_greater_than_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return compareGreaterThan<simd_register_i32, simd_mmask_i32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i64 compare_greater_than_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return compareGreaterThan<simd_register_i64, simd_mmask_i64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_ui8 compare_greater_than_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return compareGreaterThan<simd_register_ui8, simd_mmask_ui8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui16 compare_greater_than_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return compareGreaterThan<simd_register_ui16, simd_mmask_ui16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui32 compare_greater_than_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return compareGreaterThan<simd_register_ui32, simd_mmask_ui32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui64 compare_greater_than_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return compareGreaterThan<simd_register_ui64, simd_mmask_ui64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_f32 compare_greater_than_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return compareGreaterThan<simd_register_f32, simd_mmask_f32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_f64 compare_greater_than_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return compareGreaterThan<simd_register_f64, simd_mmask_f64>(lhs, rhs);
		}

		//mmasked compare greater than
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedCompareGreaterThan(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] > rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_compare_greater_than_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_compare_greater_than_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_compare_greater_than_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_compare_greater_than_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_compare_greater_than_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_compare_greater_than_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_compare_greater_than_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_compare_greater_than_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_compare_greater_than_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_compare_greater_than_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedCompareGreaterThan<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src compare greater than 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcCompareGreaterThan(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType src, SimdMaskType mmask) noexcept {
			SimdMaskType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] > rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_src_compare_greater_than_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_compare_greater_than_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_compare_greater_than_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_compare_greater_than_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_src_compare_greater_than_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_compare_greater_than_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_compare_greater_than_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_compare_greater_than_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_src_compare_greater_than_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_compare_greater_than_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcCompareGreaterThan<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//compare less than or equal 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType compareLessThanOrEqual(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, lhs[i] <= rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 compare_less_than_or_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i16 compare_less_than_or_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i32 compare_less_than_or_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i64 compare_less_than_or_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_ui8 compare_less_than_or_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui16 compare_less_than_or_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui32 compare_less_than_or_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui64 compare_less_than_or_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_f32 compare_less_than_or_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_f64 compare_less_than_or_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return compareLessThanOrEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs);
		}

		//mmasked compare less than or equal
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedCompareLessThanOrEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] <= rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_compare_less_than_or_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_compare_less_than_or_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_compare_less_than_or_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_compare_less_than_or_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_compare_less_than_or_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_compare_less_than_or_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_compare_less_than_or_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_compare_less_than_or_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_compare_less_than_or_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_compare_less_than_or_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedCompareLessThanOrEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src compare less than or equal 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcCompareLessThanOrEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType src, SimdMaskType mmask) noexcept {
			SimdMaskType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] <= rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_src_compare_less_than_or_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_compare_less_than_or_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_compare_less_than_or_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_compare_less_than_or_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_src_compare_less_than_or_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_compare_less_than_or_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_compare_less_than_or_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_compare_less_than_or_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_src_compare_less_than_or_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_compare_less_than_or_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcCompareLessThanOrEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//compare greater than or equal 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType compareGreaterThanOrEqual(SimdRegisterType lhs, SimdRegisterType rhs) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output.setAtIndex(i, lhs[i] >= rhs[i]);
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 compare_greater_than_or_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i16 compare_greater_than_or_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i32 compare_greater_than_or_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_i64 compare_greater_than_or_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_ui8 compare_greater_than_or_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui16 compare_greater_than_or_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui32 compare_greater_than_or_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_ui64 compare_greater_than_or_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs);
		}

		constexpr inline static simd_mmask_f32 compare_greater_than_or_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs);
		}
		constexpr inline static simd_mmask_f64 compare_greater_than_or_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs) noexcept {
			return compareGreaterThanOrEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs);
		}

		//mmasked compare greater than or equal
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedCompareGreaterThanOrEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType mmask) noexcept {
			SimdMaskType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] >= rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_compare_greater_than_or_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_compare_greater_than_or_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_compare_greater_than_or_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_compare_greater_than_or_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_compare_greater_than_or_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_compare_greater_than_or_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_compare_greater_than_or_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_compare_greater_than_or_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_compare_greater_than_or_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_compare_greater_than_or_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 mmask) noexcept {
			return mmaskedCompareGreaterThanOrEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, mmask);
		}

		//mmasked src compare greater than or equal 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdMaskType mmaskedSrcCompareGreaterThanOrEqual(SimdRegisterType lhs, SimdRegisterType rhs, SimdMaskType src, SimdMaskType mmask) noexcept {
			SimdMaskType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output.setAtIndex(i, lhs[i] >= rhs[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 mmasked_src_compare_greater_than_or_equal_i8(simd_register_i8 lhs, simd_register_i8 rhs, simd_mmask_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_i8, simd_mmask_i8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i16 mmasked_src_compare_greater_than_or_equal_i16(simd_register_i16 lhs, simd_register_i16 rhs, simd_mmask_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_i16, simd_mmask_i16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i32 mmasked_src_compare_greater_than_or_equal_i32(simd_register_i32 lhs, simd_register_i32 rhs, simd_mmask_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_i32, simd_mmask_i32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_i64 mmasked_src_compare_greater_than_or_equal_i64(simd_register_i64 lhs, simd_register_i64 rhs, simd_mmask_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_i64, simd_mmask_i64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_ui8 mmasked_src_compare_greater_than_or_equal_ui8(simd_register_ui8 lhs, simd_register_ui8 rhs, simd_mmask_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_ui8, simd_mmask_ui8>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui16 mmasked_src_compare_greater_than_or_equal_ui16(simd_register_ui16 lhs, simd_register_ui16 rhs, simd_mmask_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_ui16, simd_mmask_ui16>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui32 mmasked_src_compare_greater_than_or_equal_ui32(simd_register_ui32 lhs, simd_register_ui32 rhs, simd_mmask_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_ui32, simd_mmask_ui32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_ui64 mmasked_src_compare_greater_than_or_equal_ui64(simd_register_ui64 lhs, simd_register_ui64 rhs, simd_mmask_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_ui64, simd_mmask_ui64>(lhs, rhs, src, mmask);
		}

		constexpr inline static simd_mmask_f32 mmasked_src_compare_greater_than_or_equal_f32(simd_register_f32 lhs, simd_register_f32 rhs, simd_mmask_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_f32, simd_mmask_f32>(lhs, rhs, src, mmask);
		}
		constexpr inline static simd_mmask_f64 mmasked_src_compare_greater_than_or_equal_f64(simd_register_f64 lhs, simd_register_f64 rhs, simd_mmask_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcCompareGreaterThanOrEqual<simd_register_f64, simd_mmask_f64>(lhs, rhs, src, mmask);
		}

		//load//

		//load
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType load(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = ptr[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 load_i8(const i8* ptr) noexcept {
			return load<simd_register_i8>(ptr);
		}
		constexpr inline static simd_register_i16 load_i16(const i16* ptr) noexcept {
			return load<simd_register_i16>(ptr);
		}
		constexpr inline static simd_register_i32 load_i32(const i32* ptr) noexcept {
			return load<simd_register_i32>(ptr);
		}
		constexpr inline static simd_register_i64 load_i64(const i64* ptr) noexcept {
			return load<simd_register_i64>(ptr);
		}

		constexpr inline static simd_register_ui8 load_ui8(const ui8* ptr) noexcept {
			return load<simd_register_ui8>(ptr);
		}
		constexpr inline static simd_register_ui16 load_ui16(const ui16* ptr) noexcept {
			return load<simd_register_ui16>(ptr);
		}
		constexpr inline static simd_register_ui32 load_ui32(const ui32* ptr) noexcept {
			return load<simd_register_ui32>(ptr);
		}
		constexpr inline static simd_register_ui64 load_ui64(const ui64* ptr) noexcept {
			return load<simd_register_ui64>(ptr);
		}

		constexpr inline static simd_register_f32 load_f32(const f32* ptr) noexcept {
			return load<simd_register_f32>(ptr);
		}
		constexpr inline static simd_register_f64 load_f64(const f64* ptr) noexcept {
			return load<simd_register_f64>(ptr);
		}

		//mmasked load
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedLoad(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr, SimdMaskType mmask) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = (mmask[i] == 0) ? 0 : ptr[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_load_i8(const i8* ptr, simd_mmask_i8 mmask) noexcept {
			return mmaskedLoad<simd_register_i8, simd_mmask_i8>(ptr, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_load_i16(const i16* ptr, simd_mmask_i16 mmask) noexcept {
			return mmaskedLoad<simd_register_i16, simd_mmask_i16>(ptr, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_load_i32(const i32* ptr, simd_mmask_i32 mmask) noexcept {
			return mmaskedLoad<simd_register_i32, simd_mmask_i32>(ptr, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_load_i64(const i64* ptr, simd_mmask_i64 mmask) noexcept {
			return mmaskedLoad<simd_register_i64, simd_mmask_i64>(ptr, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_load_ui8(const ui8* ptr, simd_mmask_ui8 mmask) noexcept {
			return mmaskedLoad<simd_register_ui8, simd_mmask_ui8>(ptr, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_load_ui16(const ui16* ptr, simd_mmask_ui16 mmask) noexcept {
			return mmaskedLoad<simd_register_ui16, simd_mmask_ui16>(ptr, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_load_ui32(const ui32* ptr, simd_mmask_ui32 mmask) noexcept {
			return mmaskedLoad<simd_register_ui32, simd_mmask_ui32>(ptr, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_load_ui64(const ui64* ptr, simd_mmask_ui64 mmask) noexcept {
			return mmaskedLoad<simd_register_ui64, simd_mmask_ui64>(ptr, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_load_f32(const f32* ptr, simd_mmask_f32 mmask) noexcept {
			return mmaskedLoad<simd_register_f32, simd_mmask_f32>(ptr, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_load_f64(const f64* ptr, simd_mmask_f64 mmask) noexcept {
			return mmaskedLoad<simd_register_f64, simd_mmask_f64>(ptr, mmask);
		}

		//rmasked load
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType rmaskedLoad(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr, SimdRegisterType mask) noexcept {
			SimdRegisterType output;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					output[i] = (bitCast<ui32, f32>(mask[i]) == 0) ? 0 : ptr[i];
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					output[i] = (bitCast<ui64, f64>(mask[i]) == 0) ? 0 : ptr[i];
				} else {
					output[i] = (mask[i] == 0) ? 0 : ptr[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 rmasked_load_i8(const i8* ptr, simd_register_i8 mask) noexcept {
			return rmaskedLoad<simd_register_i8>(ptr, mask);
		}
		constexpr inline static simd_register_i16 rmasked_load_i16(const i16* ptr, simd_register_i16 mask) noexcept {
			return rmaskedLoad<simd_register_i16>(ptr, mask);
		}
		constexpr inline static simd_register_i32 rmasked_load_i32(const i32* ptr, simd_register_i32 mask) noexcept {
			return rmaskedLoad<simd_register_i32>(ptr, mask);
		}
		constexpr inline static simd_register_i64 rmasked_load_i64(const i64* ptr, simd_register_i64 mask) noexcept {
			return rmaskedLoad<simd_register_i64>(ptr, mask);
		}

		constexpr inline static simd_register_ui8 rmasked_load_ui8(const ui8* ptr, simd_register_ui8 mask) noexcept {
			return rmaskedLoad<simd_register_ui8>(ptr, mask);
		}
		constexpr inline static simd_register_ui16 rmasked_load_ui16(const ui16* ptr, simd_register_ui16 mask) noexcept {
			return rmaskedLoad<simd_register_ui16>(ptr, mask);
		}
		constexpr inline static simd_register_ui32 rmasked_load_ui32(const ui32* ptr, simd_register_ui32 mask) noexcept {
			return rmaskedLoad<simd_register_ui32>(ptr, mask);
		}
		constexpr inline static simd_register_ui64 rmasked_load_ui64(const ui64* ptr, simd_register_ui64 mask) noexcept {
			return rmaskedLoad<simd_register_ui64>(ptr, mask);
		}

		constexpr inline static simd_register_f32 rmasked_load_f32(const f32* ptr, simd_register_f32 mask) noexcept {
			return rmaskedLoad<simd_register_f32>(ptr, mask);
		}
		constexpr inline static simd_register_f64 rmasked_load_f64(const f64* ptr, simd_register_f64 mask) noexcept {
			return rmaskedLoad<simd_register_f64>(ptr, mask);
		}

		//load aligned
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType loadAligned(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = ptr[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 load_aligned_i8(const i8* ptr) noexcept {
			return loadAligned<simd_register_i8>(ptr);
		}
		constexpr inline static simd_register_i16 load_aligned_i16(const i16* ptr) noexcept {
			return loadAligned<simd_register_i16>(ptr);
		}
		constexpr inline static simd_register_i32 load_aligned_i32(const i32* ptr) noexcept {
			return loadAligned<simd_register_i32>(ptr);
		}
		constexpr inline static simd_register_i64 load_aligned_i64(const i64* ptr) noexcept {
			return loadAligned<simd_register_i64>(ptr);
		}

		constexpr inline static simd_register_ui8 load_aligned_ui8(const ui8* ptr) noexcept {
			return loadAligned<simd_register_ui8>(ptr);
		}
		constexpr inline static simd_register_ui16 load_aligned_ui16(const ui16* ptr) noexcept {
			return loadAligned<simd_register_ui16>(ptr);
		}
		constexpr inline static simd_register_ui32 load_aligned_ui32(const ui32* ptr) noexcept {
			return loadAligned<simd_register_ui32>(ptr);
		}
		constexpr inline static simd_register_ui64 load_aligned_ui64(const ui64* ptr) noexcept {
			return loadAligned<simd_register_ui64>(ptr);
		}

		constexpr inline static simd_register_f32 load_aligned_f32(const f32* ptr) noexcept {
			return loadAligned<simd_register_f32>(ptr);
		}
		constexpr inline static simd_register_f64 load_aligned_f64(const f64* ptr) noexcept {
			return loadAligned<simd_register_f64>(ptr);
		}

		//mmasked load aligned
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedLoadAligned(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr, SimdMaskType mmask) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = (mmask[i] == 0) ? 0 : ptr[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_load_aligned_i8(const i8* ptr, simd_mmask_i8 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_i8, simd_mmask_i8>(ptr, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_load_aligned_i16(const i16* ptr, simd_mmask_i16 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_i16, simd_mmask_i16>(ptr, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_load_aligned_i32(const i32* ptr, simd_mmask_i32 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_i32, simd_mmask_i32>(ptr, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_load_aligned_i64(const i64* ptr, simd_mmask_i64 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_i64, simd_mmask_i64>(ptr, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_load_aligned_ui8(const ui8* ptr, simd_mmask_ui8 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_ui8, simd_mmask_ui8>(ptr, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_load_aligned_ui16(const ui16* ptr, simd_mmask_ui16 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_ui16, simd_mmask_ui16>(ptr, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_load_aligned_ui32(const ui32* ptr, simd_mmask_ui32 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_ui32, simd_mmask_ui32>(ptr, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_load_aligned_ui64(const ui64* ptr, simd_mmask_ui64 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_ui64, simd_mmask_ui64>(ptr, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_load_aligned_f32(const f32* ptr, simd_mmask_f32 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_f32, simd_mmask_f32>(ptr, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_load_aligned_f64(const f64* ptr, simd_mmask_f64 mmask) noexcept {
			return mmaskedLoadAligned<simd_register_f64, simd_mmask_f64>(ptr, mmask);
		}

		//rmasked load aligned
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType rmaskedLoadAligned(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr, SimdRegisterType mask) noexcept {
			SimdRegisterType output;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					output[i] = (bitCast<ui32, f32>(mask[i]) == 0) ? 0 : ptr[i];
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					output[i] = (bitCast<ui64, f64>(mask[i]) == 0) ? 0 : ptr[i];
				} else {
					output[i] = (mask[i] == 0) ? 0 : ptr[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 rmasked_load_aligned_i8(const i8* ptr, simd_register_i8 mask) noexcept {
			return rmaskedLoadAligned<simd_register_i8>(ptr, mask);
		}
		constexpr inline static simd_register_i16 rmasked_load_aligned_i16(const i16* ptr, simd_register_i16 mask) noexcept {
			return rmaskedLoadAligned<simd_register_i16>(ptr, mask);
		}
		constexpr inline static simd_register_i32 rmasked_load_aligned_i32(const i32* ptr, simd_register_i32 mask) noexcept {
			return rmaskedLoadAligned<simd_register_i32>(ptr, mask);
		}
		constexpr inline static simd_register_i64 rmasked_load_aligned_i64(const i64* ptr, simd_register_i64 mask) noexcept {
			return rmaskedLoadAligned<simd_register_i64>(ptr, mask);
		}

		constexpr inline static simd_register_ui8 rmasked_load_aligned_ui8(const ui8* ptr, simd_register_ui8 mask) noexcept {
			return rmaskedLoadAligned<simd_register_ui8>(ptr, mask);
		}
		constexpr inline static simd_register_ui16 rmasked_load_aligned_ui16(const ui16* ptr, simd_register_ui16 mask) noexcept {
			return rmaskedLoadAligned<simd_register_ui16>(ptr, mask);
		}
		constexpr inline static simd_register_ui32 rmasked_load_aligned_ui32(const ui32* ptr, simd_register_ui32 mask) noexcept {
			return rmaskedLoadAligned<simd_register_ui32>(ptr, mask);
		}
		constexpr inline static simd_register_ui64 rmasked_load_aligned_ui64(const ui64* ptr, simd_register_ui64 mask) noexcept {
			return rmaskedLoadAligned<simd_register_ui64>(ptr, mask);
		}

		constexpr inline static simd_register_f32 rmasked_load_aligned_f32(const f32* ptr, simd_register_f32 mask) noexcept {
			return rmaskedLoadAligned<simd_register_f32>(ptr, mask);
		}
		constexpr inline static simd_register_f64 rmasked_load_aligned_f64(const f64* ptr, simd_register_f64 mask) noexcept {
			return rmaskedLoadAligned<simd_register_f64>(ptr, mask);
		}

		//store//

		//store
		template<typename SimdRegisterType>
		constexpr inline static void store(typename SimdRegisterToInfo<SimdRegisterType>::value_type* dstPtr, SimdRegisterType src) noexcept {
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				dstPtr[i] = src[i];
			}
		}

		constexpr inline static void store_i8(i8* dstPtr, simd_register_i8 src) noexcept {
			store<simd_register_i8>(dstPtr, src);
		}
		constexpr inline static void store_i16(i16* dstPtr, simd_register_i16 src) noexcept {
			store<simd_register_i16>(dstPtr, src);
		}
		constexpr inline static void store_i32(i32* dstPtr, simd_register_i32 src) noexcept {
			store<simd_register_i32>(dstPtr, src);
		}
		constexpr inline static void store_i64(i64* dstPtr, simd_register_i64 src) noexcept {
			store<simd_register_i64>(dstPtr, src);
		}

		constexpr inline static void store_ui8(ui8* dstPtr, simd_register_ui8 src) noexcept {
			store<simd_register_ui8>(dstPtr, src);
		}
		constexpr inline static void store_ui16(ui16* dstPtr, simd_register_ui16 src) noexcept {
			store<simd_register_ui16>(dstPtr, src);
		}
		constexpr inline static void store_ui32(ui32* dstPtr, simd_register_ui32 src) noexcept {
			store<simd_register_ui32>(dstPtr, src);
		}
		constexpr inline static void store_ui64(ui64* dstPtr, simd_register_ui64 src) noexcept {
			store<simd_register_ui64>(dstPtr, src);
		}

		constexpr inline static void store_f32(f32* dstPtr, simd_register_f32 src) noexcept {
			store<simd_register_f32>(dstPtr, src);
		}
		constexpr inline static void store_f64(f64* dstPtr, simd_register_f64 src) noexcept {
			store<simd_register_f64>(dstPtr, src);
		}

		//mmasked store
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static void mmaskedStore(typename SimdRegisterToInfo<SimdRegisterType>::value_type* dstPtr, SimdMaskType mmask, SimdRegisterType src) noexcept {
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i] != 0) {
					dstPtr[i] = src[i];
				}
			}
		}

		constexpr inline static void mmasked_store_i8(i8* dstPtr, simd_mmask_i8 mmask, simd_register_i8 src) noexcept {
			mmaskedStore<simd_register_i8, simd_mmask_i8>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_i16(i16* dstPtr, simd_mmask_i16 mmask, simd_register_i16 src) noexcept {
			mmaskedStore<simd_register_i16, simd_mmask_i16>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_i32(i32* dstPtr, simd_mmask_i32 mmask, simd_register_i32 src) noexcept {
			mmaskedStore<simd_register_i32, simd_mmask_i32>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_i64(i64* dstPtr, simd_mmask_i64 mmask, simd_register_i64 src) noexcept {
			mmaskedStore<simd_register_i64, simd_mmask_i64>(dstPtr, mmask, src);
		}

		constexpr inline static void mmasked_store_ui8(ui8* dstPtr, simd_mmask_ui8 mmask, simd_register_ui8 src) noexcept {
			mmaskedStore<simd_register_ui8, simd_mmask_ui8>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_ui16(ui16* dstPtr, simd_mmask_ui16 mmask, simd_register_ui16 src) noexcept {
			mmaskedStore<simd_register_ui16, simd_mmask_ui16>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_ui32(ui32* dstPtr, simd_mmask_ui32 mmask, simd_register_ui32 src) noexcept {
			mmaskedStore<simd_register_ui32, simd_mmask_ui32>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_ui64(ui64* dstPtr, simd_mmask_ui64 mmask, simd_register_ui64 src) noexcept {
			mmaskedStore<simd_register_ui64, simd_mmask_ui64>(dstPtr, mmask, src);
		}

		constexpr inline static void mmasked_store_f32(f32* dstPtr, simd_mmask_f32 mmask, simd_register_f32 src) noexcept {
			mmaskedStore<simd_register_f32, simd_mmask_f32>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_f64(f64* dstPtr, simd_mmask_f64 mmask, simd_register_f64 src) noexcept {
			mmaskedStore<simd_register_f64, simd_mmask_f64>(dstPtr, mmask, src);
		}

		//rmasked store
		template<typename SimdRegisterType>
		constexpr inline static void rmaskedStore(typename SimdRegisterToInfo<SimdRegisterType>::value_type* dstPtr, SimdRegisterType mask, SimdRegisterType src) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					if (bitCast<ui32, f32>(mask[i]) != 0) {
						dstPtr[i] = src[i];
					}
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					if (bitCast<ui64, f64>(mask[i]) != 0) {
						dstPtr[i] = src[i];
					}
				} else {
					if (mask[i] != 0) {
						dstPtr[i] = src[i];
					}
				}
			}
		}

		constexpr inline static void rmasked_store_i8(i8* dstPtr, simd_register_i8 mask, simd_register_i8 src) noexcept {
			rmaskedStore<simd_register_i8>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_i16(i16* dstPtr, simd_register_i16 mask, simd_register_i16 src) noexcept {
			rmaskedStore<simd_register_i16>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_i32(i32* dstPtr, simd_register_i32 mask, simd_register_i32 src) noexcept {
			rmaskedStore<simd_register_i32>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_i64(i64* dstPtr, simd_register_i64 mask, simd_register_i64 src) noexcept {
			rmaskedStore<simd_register_i64>(dstPtr, mask, src);
		}

		constexpr inline static void rmasked_store_ui8(ui8* dstPtr, simd_register_ui8 mask, simd_register_ui8 src) noexcept {
			rmaskedStore<simd_register_ui8>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_ui16(ui16* dstPtr, simd_register_ui16 mask, simd_register_ui16 src) noexcept {
			rmaskedStore<simd_register_ui16>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_ui32(ui32* dstPtr, simd_register_ui32 mask, simd_register_ui32 src) noexcept {
			rmaskedStore<simd_register_ui32>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_ui64(ui64* dstPtr, simd_register_ui64 mask, simd_register_ui64 src) noexcept {
			rmaskedStore<simd_register_ui64>(dstPtr, mask, src);
		}

		constexpr inline static void rmasked_store_f32(f32* dstPtr, simd_register_f32 mask, simd_register_f32 src) noexcept {
			rmaskedStore<simd_register_f32>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_f64(f64* dstPtr, simd_register_f64 mask, simd_register_f64 src) noexcept {
			rmaskedStore<simd_register_f64>(dstPtr, mask, src);
		}

		//store aligned
		template<typename SimdRegisterType>
		constexpr inline static void storeAligned(typename SimdRegisterToInfo<SimdRegisterType>::value_type* dstPtr, SimdRegisterType src) noexcept {
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				dstPtr[i] = src[i];
			}
		}

		constexpr inline static void store_aligned_i8(i8* dstPtr, simd_register_i8 src) noexcept {
			storeAligned<simd_register_i8>(dstPtr, src);
		}
		constexpr inline static void store_aligned_i16(i16* dstPtr, simd_register_i16 src) noexcept {
			storeAligned<simd_register_i16>(dstPtr, src);
		}
		constexpr inline static void store_aligned_i32(i32* dstPtr, simd_register_i32 src) noexcept {
			storeAligned<simd_register_i32>(dstPtr, src);
		}
		constexpr inline static void store_aligned_i64(i64* dstPtr, simd_register_i64 src) noexcept {
			storeAligned<simd_register_i64>(dstPtr, src);
		}

		constexpr inline static void store_aligned_ui8(ui8* dstPtr, simd_register_ui8 src) noexcept {
			storeAligned<simd_register_ui8>(dstPtr, src);
		}
		constexpr inline static void store_aligned_ui16(ui16* dstPtr, simd_register_ui16 src) noexcept {
			storeAligned<simd_register_ui16>(dstPtr, src);
		}
		constexpr inline static void store_aligned_ui32(ui32* dstPtr, simd_register_ui32 src) noexcept {
			storeAligned<simd_register_ui32>(dstPtr, src);
		}
		constexpr inline static void store_aligned_ui64(ui64* dstPtr, simd_register_ui64 src) noexcept {
			storeAligned<simd_register_ui64>(dstPtr, src);
		}

		constexpr inline static void store_aligned_f32(f32* dstPtr, simd_register_f32 src) noexcept {
			storeAligned<simd_register_f32>(dstPtr, src);
		}
		constexpr inline static void store_aligned_f64(f64* dstPtr, simd_register_f64 src) noexcept {
			storeAligned<simd_register_f64>(dstPtr, src);
		}

		//mmasked store aligned
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static void mmaskedStoreAligned(typename SimdRegisterToInfo<SimdRegisterType>::value_type* dstPtr, SimdMaskType mmask, SimdRegisterType src) noexcept {
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i] != 0) {
					dstPtr[i] = src[i];
				}
			}
		}

		constexpr inline static void mmasked_store_aligned_i8(i8* dstPtr, simd_mmask_i8 mmask, simd_register_i8 src) noexcept {
			mmaskedStoreAligned<simd_register_i8, simd_mmask_i8>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_aligned_i16(i16* dstPtr, simd_mmask_i16 mmask, simd_register_i16 src) noexcept {
			mmaskedStoreAligned<simd_register_i16, simd_mmask_i16>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_aligned_i32(i32* dstPtr, simd_mmask_i32 mmask, simd_register_i32 src) noexcept {
			mmaskedStoreAligned<simd_register_i32, simd_mmask_i32>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_aligned_i64(i64* dstPtr, simd_mmask_i64 mmask, simd_register_i64 src) noexcept {
			mmaskedStoreAligned<simd_register_i64, simd_mmask_i64>(dstPtr, mmask, src);
		}

		constexpr inline static void mmasked_store_aligned_ui8(ui8* dstPtr, simd_mmask_ui8 mmask, simd_register_ui8 src) noexcept {
			mmaskedStoreAligned<simd_register_ui8, simd_mmask_ui8>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_aligned_ui16(ui16* dstPtr, simd_mmask_ui16 mmask, simd_register_ui16 src) noexcept {
			mmaskedStoreAligned<simd_register_ui16, simd_mmask_ui16>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_aligned_ui32(ui32* dstPtr, simd_mmask_ui32 mmask, simd_register_ui32 src) noexcept {
			mmaskedStoreAligned<simd_register_ui32, simd_mmask_ui32>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_aligned_ui64(ui64* dstPtr, simd_mmask_ui64 mmask, simd_register_ui64 src) noexcept {
			mmaskedStoreAligned<simd_register_ui64, simd_mmask_ui64>(dstPtr, mmask, src);
		}

		constexpr inline static void mmasked_store_aligned_f32(f32* dstPtr, simd_mmask_f32 mmask, simd_register_f32 src) noexcept {
			mmaskedStoreAligned<simd_register_f32, simd_mmask_f32>(dstPtr, mmask, src);
		}
		constexpr inline static void mmasked_store_aligned_f64(f64* dstPtr, simd_mmask_f64 mmask, simd_register_f64 src) noexcept {
			mmaskedStoreAligned<simd_register_f64, simd_mmask_f64>(dstPtr, mmask, src);
		}

		//rmasked store
		template<typename SimdRegisterType>
		constexpr inline static void rmaskedStoreAligned(typename SimdRegisterToInfo<SimdRegisterType>::value_type* dstPtr, SimdRegisterType mask, SimdRegisterType src) noexcept {
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					if (bitCast<ui32, f32>(mask[i]) != 0) {
						dstPtr[i] = src[i];
					}
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					if (bitCast<ui64, f64>(mask[i]) != 0) {
						dstPtr[i] = src[i];
					}
				} else {
					if (mask[i] != 0) {
						dstPtr[i] = src[i];
					}
				}
			}
		}

		constexpr inline static void rmasked_store_aligned_i8(i8* dstPtr, simd_register_i8 mask, simd_register_i8 src) noexcept {
			rmaskedStoreAligned<simd_register_i8>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_aligned_i16(i16* dstPtr, simd_register_i16 mask, simd_register_i16 src) noexcept {
			rmaskedStoreAligned<simd_register_i16>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_aligned_i32(i32* dstPtr, simd_register_i32 mask, simd_register_i32 src) noexcept {
			rmaskedStoreAligned<simd_register_i32>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_aligned_i64(i64* dstPtr, simd_register_i64 mask, simd_register_i64 src) noexcept {
			rmaskedStoreAligned<simd_register_i64>(dstPtr, mask, src);
		}

		constexpr inline static void rmasked_store_aligned_ui8(ui8* dstPtr, simd_register_ui8 mask, simd_register_ui8 src) noexcept {
			rmaskedStoreAligned<simd_register_ui8>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_aligned_ui16(ui16* dstPtr, simd_register_ui16 mask, simd_register_ui16 src) noexcept {
			rmaskedStoreAligned<simd_register_ui16>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_aligned_ui32(ui32* dstPtr, simd_register_ui32 mask, simd_register_ui32 src) noexcept {
			rmaskedStoreAligned<simd_register_ui32>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_aligned_ui64(ui64* dstPtr, simd_register_ui64 mask, simd_register_ui64 src) noexcept {
			rmaskedStoreAligned<simd_register_ui64>(dstPtr, mask, src);
		}

		constexpr inline static void rmasked_store_aligned_f32(f32* dstPtr, simd_register_f32 mask, simd_register_f32 src) noexcept {
			rmaskedStoreAligned<simd_register_f32>(dstPtr, mask, src);
		}
		constexpr inline static void rmasked_store_aligned_f64(f64* dstPtr, simd_register_f64 mask, simd_register_f64 src) noexcept {
			rmaskedStoreAligned<simd_register_f64>(dstPtr, mask, src);
		}

		//set//

		//set 
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType set(typename SimdRegisterToInfo<SimdRegisterType>::value_type value) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = value;
			}
			return output;
		}

		constexpr inline static simd_register_i8 set_i8(const i8 value) noexcept {
			return set<simd_register_i8>(value);
		}
		constexpr inline static simd_register_i16 set_i16(const i16 value) noexcept {
			return set<simd_register_i16>(value);
		}
		constexpr inline static simd_register_i32 set_i32(const i32 value) noexcept {
			return set<simd_register_i32>(value);
		}
		constexpr inline static simd_register_i64 set_i64(const i64 value) noexcept {
			return set<simd_register_i64>(value);
		}

		constexpr inline static simd_register_ui8 set_ui8(const ui8 value) noexcept {
			return set<simd_register_ui8>(value);
		}
		constexpr inline static simd_register_ui16 set_ui16(const ui16 value) noexcept {
			return set<simd_register_ui16>(value);
		}
		constexpr inline static simd_register_ui32 set_ui32(const ui32 value) noexcept {
			return set<simd_register_ui32>(value);
		}
		constexpr inline static simd_register_ui64 set_ui64(const ui64 value) noexcept {
			return set<simd_register_ui64>(value);
		}

		constexpr inline static simd_register_f32 set_f32(const f32 value) noexcept {
			return set<simd_register_f32>(value);
		}
		constexpr inline static simd_register_f64 set_f64(const f64 value) noexcept {
			return set<simd_register_f64>(value);
		}

		//mmasked set
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSet(typename SimdRegisterToInfo<SimdRegisterType>::value_type value, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = value;
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_set_i8(const i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedSet<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_set_i16(const i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedSet<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_set_i32(const i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedSet<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_set_i64(const i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedSet<simd_register_i64, simd_mmask_i64>(value, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_set_ui8(const ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSet<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_set_ui16(const ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSet<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_set_ui32(const ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSet<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_set_ui64(const ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSet<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_set_f32(const f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedSet<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_set_f64(const f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedSet<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked src set
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcSet(typename SimdRegisterToInfo<SimdRegisterType>::value_type value, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = value;
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_set_i8(const i8 value, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcSet<simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_set_i16(const i16 value, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcSet<simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_set_i32(const i32 value, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcSet<simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_set_i64(const i64 value, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcSet<simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_set_ui8(const ui8 value, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcSet<simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_set_ui16(const ui16 value, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcSet<simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_set_ui32(const ui32 value, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcSet<simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_set_ui64(const ui64 value, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcSet<simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_set_f32(const f32 value, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcSet<simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_set_f64(const f64 value, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcSet<simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//set zero 
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType setZero() noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = 0;
			}
			return output;
		}

		constexpr inline static simd_register_i8 set_zero_i8() noexcept {
			return setZero<simd_register_i8>();
		}
		constexpr inline static simd_register_i16 set_zero_i16() noexcept {
			return setZero<simd_register_i16>();
		}
		constexpr inline static simd_register_i32 set_zero_i32() noexcept {
			return setZero<simd_register_i32>();
		}
		constexpr inline static simd_register_i64 set_zero_i64() noexcept {
			return setZero<simd_register_i64>();
		}

		constexpr inline static simd_register_ui8 set_zero_ui8() noexcept {
			return setZero<simd_register_ui8>();
		}
		constexpr inline static simd_register_ui16 set_zero_ui16() noexcept {
			return setZero<simd_register_ui16>();
		}
		constexpr inline static simd_register_ui32 set_zero_ui32() noexcept {
			return setZero<simd_register_ui32>();
		}
		constexpr inline static simd_register_ui64 set_zero_ui64() noexcept {
			return setZero<simd_register_ui64>();
		}

		constexpr inline static simd_register_f32 set_zero_f32() noexcept {
			return setZero<simd_register_f32>();
		}
		constexpr inline static simd_register_f64 set_zero_f64() noexcept {
			return setZero<simd_register_f64>();
		}

		//mmasked src set zero 
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcSetZero(SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = 0;
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_set_zero_i8(simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_i8, simd_mmask_i8>(src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_set_zero_i16(simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_i16, simd_mmask_i16>(src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_set_zero_i32(simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_i32, simd_mmask_i32>(src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_set_zero_i64(simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_i64, simd_mmask_i64>(src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_set_zero_ui8(simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_ui8, simd_mmask_ui8>(src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_set_zero_ui16(simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_ui16, simd_mmask_ui16>(src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_set_zero_ui32(simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_ui32, simd_mmask_ui32>(src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_set_zero_ui64(simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_ui64, simd_mmask_ui64>(src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_set_zero_f32(simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_f32, simd_mmask_f32>(src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_set_zero_f64(simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcSetZero<simd_register_f64, simd_mmask_f64>(src, mmask);
		}

		//shift//

		//shift left 
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType shiftLeft(const SimdRegisterType value, const Size count) noexcept {
			NATL_ASSERT(count > SimdRegisterToInfo<SimdRegisterType>::count(), "cannot shift left register more than ", SimdRegisterToInfo<SimdRegisterType>::count(), " but shift was ", count);
			SimdRegisterType output = setZero<SimdRegisterType>();
			const Size amountLeft = SimdRegisterToInfo<SimdRegisterType>::count() - count;
			for (Size i = 0; i < amountLeft; i++) {
				output[i] = value[i + count];
			}
			return output;
		}

		constexpr inline static simd_register_i8 shift_left_i8(simd_register_i8 value, const Size count) noexcept {
			return shiftLeft<simd_register_i8>(value, count);
		}
		constexpr inline static simd_register_i16 shift_left_i16(simd_register_i16 value, const Size count) noexcept {
			return shiftLeft<simd_register_i16>(value, count);
		}
		constexpr inline static simd_register_i32 shift_left_i32(simd_register_i32 value, const Size count) noexcept {
			return shiftLeft<simd_register_i32>(value, count);
		}
		constexpr inline static simd_register_i64 shift_left_i64(simd_register_i64 value, const Size count) noexcept {
			return shiftLeft<simd_register_i64>(value, count);
		}

		constexpr inline static simd_register_ui8 shift_left_ui8(simd_register_ui8 value, const Size count) noexcept {
			return shiftLeft<simd_register_ui8>(value, count);
		}
		constexpr inline static simd_register_ui16 shift_left_ui16(simd_register_ui16 value, const Size count) noexcept {
			return shiftLeft<simd_register_ui16>(value, count);
		}
		constexpr inline static simd_register_ui32 shift_left_ui32(simd_register_ui32 value, const Size count) noexcept {
			return shiftLeft<simd_register_ui32>(value, count);
		}
		constexpr inline static simd_register_ui64 shift_left_ui64(simd_register_ui64 value, const Size count) noexcept {
			return shiftLeft<simd_register_ui64>(value, count);
		}

		constexpr inline static simd_register_f32 shift_left_f32(simd_register_f32 value, const Size count) noexcept {
			return shiftLeft<simd_register_f32>(value, count);
		}
		constexpr inline static simd_register_f64 shift_left_f64(simd_register_f64 value, const Size count) noexcept {
			return shiftLeft<simd_register_f64>(value, count);
		}

		//shift right
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType shiftRight(const SimdRegisterType value, const Size count) noexcept {
			NATL_ASSERT(count > SimdRegisterToInfo<SimdRegisterType>::count(), "cannot shift right register more than ", SimdRegisterToInfo<SimdRegisterType>::count(), " but shift was ", count);
			SimdRegisterType output = setZero<SimdRegisterType>();
			const Size amountLeft = SimdRegisterToInfo<SimdRegisterType>::count() - count;
			for (Size i = 0; i < amountLeft; i++) {
				output[i + count] = value[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 shift_right_i8(simd_register_i8 value, const Size count) noexcept {
			return shiftRight<simd_register_i8>(value, count);
		}
		constexpr inline static simd_register_i16 shift_right_i16(simd_register_i16 value, const Size count) noexcept {
			return shiftRight<simd_register_i16>(value, count);
		}
		constexpr inline static simd_register_i32 shift_right_i32(simd_register_i32 value, const Size count) noexcept {
			return shiftRight<simd_register_i32>(value, count);
		}
		constexpr inline static simd_register_i64 shift_right_i64(simd_register_i64 value, const Size count) noexcept {
			return shiftRight<simd_register_i64>(value, count);
		}

		constexpr inline static simd_register_ui8 shift_right_ui8(simd_register_ui8 value, const Size count) noexcept {
			return shiftRight<simd_register_ui8>(value, count);
		}
		constexpr inline static simd_register_ui16 shift_right_ui16(simd_register_ui16 value, const Size count) noexcept {
			return shiftRight<simd_register_ui16>(value, count);
		}
		constexpr inline static simd_register_ui32 shift_right_ui32(simd_register_ui32 value, const Size count) noexcept {
			return shiftRight<simd_register_ui32>(value, count);
		}
		constexpr inline static simd_register_ui64 shift_right_ui64(simd_register_ui64 value, const Size count) noexcept {
			return shiftRight<simd_register_ui64>(value, count);
		}

		constexpr inline static simd_register_f32 shift_right_f32(simd_register_f32 value, const Size count) noexcept {
			return shiftRight<simd_register_f32>(value, count);
		}
		constexpr inline static simd_register_f64 shift_right_f64(simd_register_f64 value, const Size count) noexcept {
			return shiftRight<simd_register_f64>(value, count);
		}

		//pack//
		template<class Src, class SrcInfo, class Dst>
		constexpr inline static void pack(const Src& srcReg, Dst& dstReg, Size& dstIndex) {
			for (Size i = 0; i < SrcInfo::count(); i++, dstIndex++) {
				using dst_type = decltype(dstReg[dstIndex]);
				dstReg[dstIndex] = static_cast<Decay<dst_type>>(srcReg[i]);
			}
		}

		//pack in i8
		constexpr inline static simd_register_i8 pack_i16_in_i8(
			simd_register_i16 value1,
			simd_register_i16 value2) noexcept {
			Size index = 0;
			simd_register_i8 output;
			pack<simd_register_i16, SimdRegisterI16Info<simd_arch>, simd_register_i8>(value1, output, index);
			pack<simd_register_i16, SimdRegisterI16Info<simd_arch>, simd_register_i8>(value2, output, index);
			return output;
		}

		constexpr inline static simd_register_i8 pack_i32_in_i8(
			simd_register_i32 value1,
			simd_register_i32 value2,
			simd_register_i32 value3,
			simd_register_i32 value4) noexcept {
			Size index = 0;
			simd_register_i8 output;
			pack<simd_register_i32, SimdRegisterI32Info<simd_arch>, simd_register_i8>(value1, output, index);
			pack<simd_register_i32, SimdRegisterI32Info<simd_arch>, simd_register_i8>(value2, output, index);
			pack<simd_register_i32, SimdRegisterI32Info<simd_arch>, simd_register_i8>(value3, output, index);
			pack<simd_register_i32, SimdRegisterI32Info<simd_arch>, simd_register_i8>(value4, output, index);
			return output;
		}

		constexpr inline static simd_register_i8 pack_i64_in_i8(
			simd_register_i64 value1,
			simd_register_i64 value2,
			simd_register_i64 value3,
			simd_register_i64 value4,
			simd_register_i64 value5,
			simd_register_i64 value6,
			simd_register_i64 value7,
			simd_register_i64 value8) noexcept {
			Size index = 0;
			simd_register_i8 output;
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value1, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value2, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value3, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value4, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value5, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value6, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value7, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i8>(value8, output, index);
			return output;
		}

		//pack in i16
		constexpr inline static simd_register_i16 pack_i32_in_i16(
			simd_register_i32 value1,
			simd_register_i32 value2) noexcept {
			Size index = 0;
			simd_register_i16 output;
			pack<simd_register_i32, SimdRegisterI32Info<simd_arch>, simd_register_i16>(value1, output, index);
			pack<simd_register_i32, SimdRegisterI32Info<simd_arch>, simd_register_i16>(value2, output, index);
			return output;
		}

		constexpr inline static simd_register_i16 pack_i64_in_i16(
			simd_register_i64 value1,
			simd_register_i64 value2,
			simd_register_i64 value3,
			simd_register_i64 value4) noexcept {
			Size index = 0;
			simd_register_i16 output;
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i16>(value1, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i16>(value2, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i16>(value3, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i16>(value4, output, index);
			return output;
		}

		//pack in i32
		constexpr inline static simd_register_i32 pack_i64_in_i32(
			simd_register_i64 value1,
			simd_register_i64 value2) noexcept {
			Size index = 0;
			simd_register_i32 output;
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i32>(value1, output, index);
			pack<simd_register_i64, SimdRegisterI64Info<simd_arch>, simd_register_i32>(value2, output, index);
			return output;
		}

		//pack in ui8
		constexpr inline static simd_register_ui8 pack_ui16_in_ui8(
			simd_register_ui16 value1,
			simd_register_ui16 value2) noexcept {
			Size index = 0;
			simd_register_ui8 output;
			pack<simd_register_ui16, SimdRegisterUI16Info<simd_arch>, simd_register_ui8>(value1, output, index);
			pack<simd_register_ui16, SimdRegisterUI16Info<simd_arch>, simd_register_ui8>(value2, output, index);
			return output;
		}

		constexpr inline static simd_register_ui8 pack_ui32_in_ui8(
			simd_register_ui32 value1,
			simd_register_ui32 value2,
			simd_register_ui32 value3,
			simd_register_ui32 value4) noexcept {
			Size index = 0;
			simd_register_ui8 output;
			pack<simd_register_ui32, SimdRegisterUI32Info<simd_arch>, simd_register_ui8>(value1, output, index);
			pack<simd_register_ui32, SimdRegisterUI32Info<simd_arch>, simd_register_ui8>(value2, output, index);
			pack<simd_register_ui32, SimdRegisterUI32Info<simd_arch>, simd_register_ui8>(value3, output, index);
			pack<simd_register_ui32, SimdRegisterUI32Info<simd_arch>, simd_register_ui8>(value4, output, index);
			return output;
		}

		constexpr inline static simd_register_ui8 pack_ui64_in_ui8(
			simd_register_ui64 value1,
			simd_register_ui64 value2,
			simd_register_ui64 value3,
			simd_register_ui64 value4,
			simd_register_ui64 value5,
			simd_register_ui64 value6,
			simd_register_ui64 value7,
			simd_register_ui64 value8) noexcept {
			Size index = 0;
			simd_register_ui8 output;
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value1, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value2, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value3, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value4, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value5, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value6, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value7, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui8>(value8, output, index);
			return output;
		}

		//pack in ui16
		constexpr inline static simd_register_ui16 pack_ui32_in_ui16(
			simd_register_ui32 value1,
			simd_register_ui32 value2) noexcept {
			Size index = 0;
			simd_register_ui16 output;
			pack<simd_register_ui32, SimdRegisterUI32Info<simd_arch>, simd_register_ui16>(value1, output, index);
			pack<simd_register_ui32, SimdRegisterUI32Info<simd_arch>, simd_register_ui16>(value2, output, index);
			return output;
		}

		constexpr inline static simd_register_ui16 pack_ui64_in_ui16(
			simd_register_ui64 value1,
			simd_register_ui64 value2,
			simd_register_ui64 value3,
			simd_register_ui64 value4) noexcept {
			Size index = 0;
			simd_register_ui16 output;
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui16>(value1, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui16>(value2, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui16>(value3, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui16>(value4, output, index);
			return output;
		}

		//pack in ui32
		constexpr inline static simd_register_ui32 pack_ui64_in_ui32(
			simd_register_ui64 value1,
			simd_register_ui64 value2) noexcept {
			Size index = 0;
			simd_register_ui32 output;
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui32>(value1, output, index);
			pack<simd_register_ui64, SimdRegisterUI64Info<simd_arch>, simd_register_ui32>(value2, output, index);
			return output;
		}

		//pack in f32
		constexpr inline static simd_register_f32 pack_f64_in_f32(
			simd_register_f64 value1,
			simd_register_f64 value2) noexcept {
			Size index = 0;
			simd_register_f32 output;
			pack<simd_register_f64, SimdRegisterF64Info<simd_arch>, simd_register_f32>(value1, output, index);
			pack<simd_register_f64, SimdRegisterF64Info<simd_arch>, simd_register_f32>(value2, output, index);
			return output;
		}

		//unpack//
		template<class Src, class Dst, class DstInfo>
		constexpr inline static void unpack(const Src srcReg, Dst& dstReg, Size& srcIndex) noexcept {
			for (Size i = 0; i < DstInfo::count(); i++, srcIndex++) {
				dstReg[i] = srcReg[srcIndex];
			}
		}

		//unpack i8
		constexpr inline static void unpack_i8_to_i16(
			simd_register_i8 from,
			simd_register_i16& to1,
			simd_register_i16& to2) noexcept {
			Size index = 0;
			unpack<simd_register_i8, simd_register_i16, SimdRegisterI16Info<simd_arch>>(from, to1, index);
			unpack<simd_register_i8, simd_register_i16, SimdRegisterI16Info<simd_arch>>(from, to2, index);
		}

		constexpr inline static void unpack_i8_to_i32(
			simd_register_i8 from,
			simd_register_i32& to1,
			simd_register_i32& to2,
			simd_register_i32& to3,
			simd_register_i32& to4) noexcept {
			Size index = 0;
			unpack<simd_register_i8, simd_register_i32, SimdRegisterI32Info<simd_arch>>(from, to1, index);
			unpack<simd_register_i8, simd_register_i32, SimdRegisterI32Info<simd_arch>>(from, to2, index);
			unpack<simd_register_i8, simd_register_i32, SimdRegisterI32Info<simd_arch>>(from, to3, index);
			unpack<simd_register_i8, simd_register_i32, SimdRegisterI32Info<simd_arch>>(from, to4, index);
		}

		constexpr inline static void unpack_i8_to_i64(
			simd_register_i8 from,
			simd_register_i64& to1,
			simd_register_i64& to2,
			simd_register_i64& to3,
			simd_register_i64& to4,
			simd_register_i64& to5,
			simd_register_i64& to6,
			simd_register_i64& to7,
			simd_register_i64& to8) noexcept {
			Size index = 0;
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to1, index);
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to2, index);
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to3, index);
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to4, index);
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to5, index);
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to6, index);
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to7, index);
			unpack<simd_register_i8, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to8, index);
		}

		//unpack i16
		constexpr inline static void unpack_i16_to_i32(
			simd_register_i16 from,
			simd_register_i32& to1,
			simd_register_i32& to2) noexcept {
			Size index = 0;
			unpack<simd_register_i16, simd_register_i32, SimdRegisterI32Info<simd_arch>>(from, to1, index);
			unpack<simd_register_i16, simd_register_i32, SimdRegisterI32Info<simd_arch>>(from, to2, index);
		}

		constexpr inline static void unpack_i16_to_i64(
			simd_register_i16 from,
			simd_register_i64& to1,
			simd_register_i64& to2,
			simd_register_i64& to3,
			simd_register_i64& to4) noexcept {
			Size index = 0;
			unpack<simd_register_i16, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to1, index);
			unpack<simd_register_i16, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to2, index);
			unpack<simd_register_i16, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to3, index);
			unpack<simd_register_i16, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to4, index);
		}

		//unpack i32
		constexpr inline static void unpack_i32_to_i64(
			simd_register_i32 from,
			simd_register_i64& to1,
			simd_register_i64& to2) noexcept {
			Size index = 0;
			unpack<simd_register_i32, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to1, index);
			unpack<simd_register_i32, simd_register_i64, SimdRegisterI64Info<simd_arch>>(from, to2, index);
		}

		//unpack ui8
		constexpr inline static void unpack_ui8_to_ui16(
			simd_register_ui8 from,
			simd_register_ui16& to1,
			simd_register_ui16& to2) noexcept {
			Size index = 0;
			unpack<simd_register_ui8, simd_register_ui16, SimdRegisterUI16Info<simd_arch>>(from, to1, index);
			unpack<simd_register_ui8, simd_register_ui16, SimdRegisterUI16Info<simd_arch>>(from, to2, index);
		}

		constexpr inline static void unpack_ui8_to_ui32(
			simd_register_ui8 from,
			simd_register_ui32& to1,
			simd_register_ui32& to2,
			simd_register_ui32& to3,
			simd_register_ui32& to4) noexcept {
			Size index = 0;
			unpack<simd_register_ui8, simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(from, to1, index);
			unpack<simd_register_ui8, simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(from, to2, index);
			unpack<simd_register_ui8, simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(from, to3, index);
			unpack<simd_register_ui8, simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(from, to4, index);
		}

		constexpr inline static void unpack_ui8_to_ui64(
			simd_register_ui8 from,
			simd_register_ui64& to1,
			simd_register_ui64& to2,
			simd_register_ui64& to3,
			simd_register_ui64& to4,
			simd_register_ui64& to5,
			simd_register_ui64& to6,
			simd_register_ui64& to7,
			simd_register_ui64& to8) noexcept {
			Size index = 0;
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to1, index);
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to2, index);
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to3, index);
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to4, index);
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to5, index);
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to6, index);
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to7, index);
			unpack<simd_register_ui8, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to8, index);
		}

		//unpack ui16
		constexpr inline static void unpack_ui16_to_ui32(
			simd_register_ui16 from,
			simd_register_ui32& to1,
			simd_register_ui32& to2) noexcept {
			Size index = 0;
			unpack<simd_register_ui16, simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(from, to1, index);
			unpack<simd_register_ui16, simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(from, to2, index);
		}

		constexpr inline static void unpack_ui16_to_ui64(
			simd_register_ui16 from,
			simd_register_ui64& to1,
			simd_register_ui64& to2,
			simd_register_ui64& to3,
			simd_register_ui64& to4) noexcept {
			Size index = 0;
			unpack<simd_register_ui16, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to1, index);
			unpack<simd_register_ui16, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to2, index);
			unpack<simd_register_ui16, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to3, index);
			unpack<simd_register_ui16, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to4, index);
		}

		//unpack ui32
		constexpr inline static void unpack_ui32_to_ui64(
			simd_register_ui32 from,
			simd_register_ui64& to1,
			simd_register_ui64& to2) noexcept {
			Size index = 0;
			unpack<simd_register_ui32, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to1, index);
			unpack<simd_register_ui32, simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(from, to2, index);
		}

		//unpack f32
		constexpr inline static void unpack_f32_to_f64(
			simd_register_f32 from,
			simd_register_f64& to1,
			simd_register_f64& to2) noexcept {
			Size index = 0;
			unpack<simd_register_f32, simd_register_f64, SimdRegisterF64Info<simd_arch>>(from, to1, index);
			unpack<simd_register_f32, simd_register_f64, SimdRegisterF64Info<simd_arch>>(from, to2, index);
		}

		//test//

		//test all zeros
		template<typename SimdRegisterType>
		constexpr inline static Bool testAllZero(SimdRegisterType value) noexcept {
			Bool output = true;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					output = (bitCast<i32, f32>(value[i]) == 0) && output;
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					output = (bitCast<i64, f64>(value[i]) == 0) && output;
				} else {
					output = (value[i] == 0) && output;
				}
			}
			return output;
		}

		constexpr inline static Bool test_all_zero_i8(simd_register_i8 value) noexcept {
			return testAllZero<simd_register_i8>(value);
		}
		constexpr inline static Bool test_all_zero_i16(simd_register_i16 value) noexcept {
			return testAllZero<simd_register_i16>(value);
		}
		constexpr inline static Bool test_all_zero_i32(simd_register_i32 value) noexcept {
			return testAllZero<simd_register_i32>(value);
		}
		constexpr inline static Bool test_all_zero_i64(simd_register_i64 value) noexcept {
			return testAllZero<simd_register_i64>(value);
		}

		constexpr inline static Bool test_all_zero_ui8(simd_register_ui8 value) noexcept {
			return testAllZero<simd_register_ui8>(value);
		}
		constexpr inline static Bool test_all_zero_ui16(simd_register_ui16 value) noexcept {
			return testAllZero<simd_register_ui16>(value);
		}
		constexpr inline static Bool test_all_zero_ui32(simd_register_ui32 value) noexcept {
			return testAllZero<simd_register_ui32>(value);
		}
		constexpr inline static Bool test_all_zero_ui64(simd_register_ui64 value) noexcept {
			return testAllZero<simd_register_ui64>(value);
		}

		constexpr inline static Bool test_all_zero_f32(simd_register_f32 value) noexcept {
			return testAllZero<simd_register_f32>(value);
		}
		constexpr inline static Bool test_all_zero_f64(simd_register_f64 value) noexcept {
			return testAllZero<simd_register_f64>(value);
		}

		//test all ones
		template<typename SimdRegisterType>
		constexpr inline static Bool testAllOne(SimdRegisterType value) noexcept {
			Bool output = true;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					output = (bitCast<i32, f32>(value[i]) == 1) && output;
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					output = (bitCast<i64, f64>(value[i]) == 1) && output;
				} else {
					output = (value[i] == 1) && output;
				}
			}
			return output;
		}

		constexpr inline static Bool test_all_one_i8(simd_register_i8 value) noexcept {
			return testAllOne<simd_register_i8>(value);
		}
		constexpr inline static Bool test_all_one_i16(simd_register_i16 value) noexcept {
			return testAllOne<simd_register_i16>(value);
		}
		constexpr inline static Bool test_all_one_i32(simd_register_i32 value) noexcept {
			return testAllOne<simd_register_i32>(value);
		}
		constexpr inline static Bool test_all_one_i64(simd_register_i64 value) noexcept {
			return testAllOne<simd_register_i64>(value);
		}

		constexpr inline static Bool test_all_one_ui8(simd_register_ui8 value) noexcept {
			return testAllOne<simd_register_ui8>(value);
		}
		constexpr inline static Bool test_all_one_ui16(simd_register_ui16 value) noexcept {
			return testAllOne<simd_register_ui16>(value);
		}
		constexpr inline static Bool test_all_one_ui32(simd_register_ui32 value) noexcept {
			return testAllOne<simd_register_ui32>(value);
		}
		constexpr inline static Bool test_all_one_ui64(simd_register_ui64 value) noexcept {
			return testAllOne<simd_register_ui64>(value);
		}

		constexpr inline static Bool test_all_one_f32(simd_register_f32 value) noexcept {
			return testAllOne<simd_register_f32>(value);
		}
		constexpr inline static Bool test_all_one_f64(simd_register_f64 value) noexcept {
			return testAllOne<simd_register_f64>(value);
		}

		//rmasked test all zeros
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static Bool mmaskedTestAllZero(SimdRegisterType value, SimdMaskType mmask) noexcept {
			Bool output = true;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					if constexpr (IsSame<simd_register_value_type, f32>) {
						output = (bitCast<i32, f32>(value[i]) == 0) && output;
					} else if constexpr (IsSame<simd_register_value_type, f64>) {
						output = (bitCast<i64, f64>(value[i]) == 0) && output;
					} else {
						output = (value[i] == 0) && output;
					}
				}
			}
			return output;
		}

		constexpr inline static Bool mmasked_test_all_zero_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_zero_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_zero_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_zero_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_i64, simd_mmask_i64>(value, mmask);
		}

		constexpr inline static Bool mmasked_test_all_zero_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_zero_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_zero_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_zero_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}

		constexpr inline static Bool mmasked_test_all_zero_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_zero_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedTestAllZero<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//rmasked test all zeros
		template<typename SimdRegisterType>
		constexpr inline static Bool rmaskedTestAllZero(SimdRegisterType value, SimdRegisterType mask) noexcept {
			Bool output = true;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					if (bitCast<i32, f32>(mask[i])) {
						output = (bitCast<i32, f32>(value[i]) == 0) && output;
					}
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					if (bitCast<i64, f64>(mask[i])) {
						output = (bitCast<i64, f64>(value[i]) == 0) && output;
					}
				} else {
					if (mask[i]) {
						output = (value[i] == 0) && output;
					}
				}
			}
			return output;
		}

		constexpr inline static Bool rmasked_test_all_zero_i8(simd_register_i8 value, simd_register_i8 mask) noexcept {
			return rmaskedTestAllZero<simd_register_i8>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_zero_i16(simd_register_i16 value, simd_register_i16 mask) noexcept {
			return rmaskedTestAllZero<simd_register_i16>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_zero_i32(simd_register_i32 value, simd_register_i32 mask) noexcept {
			return rmaskedTestAllZero<simd_register_i32>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_zero_i64(simd_register_i64 value, simd_register_i64 mask) noexcept {
			return rmaskedTestAllZero<simd_register_i64>(value, mask);
		}

		constexpr inline static Bool rmasked_test_all_zero_ui8(simd_register_ui8 value, simd_register_ui8 mask) noexcept {
			return rmaskedTestAllZero<simd_register_ui8>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_zero_ui16(simd_register_ui16 value, simd_register_ui16 mask) noexcept {
			return rmaskedTestAllZero<simd_register_ui16>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_zero_ui32(simd_register_ui32 value, simd_register_ui32 mask) noexcept {
			return rmaskedTestAllZero<simd_register_ui32>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_zero_ui64(simd_register_ui64 value, simd_register_ui64 mask) noexcept {
			return rmaskedTestAllZero<simd_register_ui64>(value, mask);
		}

		constexpr inline static Bool rmasked_test_all_zero_f32(simd_register_f32 value, simd_register_f32 mask) noexcept {
			return rmaskedTestAllZero<simd_register_f32>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_zero_f64(simd_register_f64 value, simd_register_f64 mask) noexcept {
			return rmaskedTestAllZero<simd_register_f64>(value, mask);
		}

		//mmasked test all ones
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static Bool mmaskedTestAllOne(SimdRegisterType value, SimdMaskType mmask) noexcept {
			Bool output = true;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					if constexpr (IsSame<simd_register_value_type, f32>) {
						output = (bitCast<i32, f32>(value[i]) == 1) && output;
					} else if constexpr (IsSame<simd_register_value_type, f64>) {
						output = (bitCast<i64, f64>(value[i]) == 1) && output;
					} else {
						output = (value[i] == 1) && output;
					}
				}
			}
			return output;
		}

		constexpr inline static Bool mmasked_test_all_one_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_one_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_one_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_one_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_i64, simd_mmask_i64>(value, mmask);
		}

		constexpr inline static Bool mmasked_test_all_one_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_one_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_one_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_one_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}

		constexpr inline static Bool mmasked_test_all_one_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static Bool mmasked_test_all_one_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedTestAllOne<simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//rmasked test all ones
		template<typename SimdRegisterType>
		constexpr inline static Bool rmaskedTestAllOne(SimdRegisterType value, SimdRegisterType mask) noexcept {
			Bool output = true;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					if (bitCast<i32, f32>(mask[i])) {
						output = (bitCast<i32, f32>(value[i]) == 1) && output;
					}
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					if (bitCast<i64, f64>(mask[i])) {
						output = (bitCast<i64, f64>(value[i]) == 1) && output;
					}
				} else {
					if (mask[i]) {
						output = (value[i] == 1) && output;
					}
				}
			}
			return output;
		}

		constexpr inline static Bool rmasked_test_all_one_i8(simd_register_i8 value, simd_register_i8 mask) noexcept {
			return rmaskedTestAllOne<simd_register_i8>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_one_i16(simd_register_i16 value, simd_register_i16 mask) noexcept {
			return rmaskedTestAllOne<simd_register_i16>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_one_i32(simd_register_i32 value, simd_register_i32 mask) noexcept {
			return rmaskedTestAllOne<simd_register_i32>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_one_i64(simd_register_i64 value, simd_register_i64 mask) noexcept {
			return rmaskedTestAllOne<simd_register_i64>(value, mask);
		}

		constexpr inline static Bool rmasked_test_all_one_ui8(simd_register_ui8 value, simd_register_ui8 mask) noexcept {
			return rmaskedTestAllOne<simd_register_ui8>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_one_ui16(simd_register_ui16 value, simd_register_ui16 mask) noexcept {
			return rmaskedTestAllOne<simd_register_ui16>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_one_ui32(simd_register_ui32 value, simd_register_ui32 mask) noexcept {
			return rmaskedTestAllOne<simd_register_ui32>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_one_ui64(simd_register_ui64 value, simd_register_ui64 mask) noexcept {
			return rmaskedTestAllOne<simd_register_ui64>(value, mask);
		}

		constexpr inline static Bool rmasked_test_all_one_f32(simd_register_f32 value, simd_register_f32 mask) noexcept {
			return rmaskedTestAllOne<simd_register_f32>(value, mask);
		}
		constexpr inline static Bool rmasked_test_all_one_f64(simd_register_f64 value, simd_register_f64 mask) noexcept {
			return rmaskedTestAllOne<simd_register_f64>(value, mask);
		}

		template<class Src, class SrcInfo>
		constexpr inline static SimdRegisterAny<simd_arch> convert_to_any(Src value) noexcept {
			using SrcDataType = SrcInfo::value_type;
			using UIntByteType = UIntOfByteSize<sizeof(SrcDataType)>;
			SimdRegisterAny<simd_arch> output;
			if (isConstantEvaluated()) {
				Size dstIndex = 0;
				for (Size srcIndex = 0; srcIndex < SrcInfo::count(); srcIndex++) {
					for (Size wordIndex = 0; wordIndex < sizeof(SrcDataType) / sizeof(ui8); wordIndex++, dstIndex++) {
						output[dstIndex] = static_cast<ui8>(static_cast<UIntByteType>(bitCast<UIntByteType, SrcDataType>(value[srcIndex])) >> (wordIndex * 8));
					}
				}
			} else {
				std::memcpy(output.values.data(), value.values.data(), sizeof(Src));
			}
			return output;
		}

		//convert to any 
		constexpr inline static SimdRegisterAny<simd_arch> convert_i8_to_any(simd_register_i8 value) noexcept {
			return convert_to_any<simd_register_i8, SimdRegisterI8Info<simd_arch>>(value);
		}
		constexpr inline static SimdRegisterAny<simd_arch> convert_i16_to_any(simd_register_i16 value) noexcept {
			return convert_to_any<simd_register_i16, SimdRegisterI16Info<simd_arch>>(value);
		}
		constexpr inline static SimdRegisterAny<simd_arch> convert_i32_to_any(simd_register_i32 value) noexcept {
			return convert_to_any<simd_register_i32, SimdRegisterI32Info<simd_arch>>(value);
		}
		constexpr inline static SimdRegisterAny<simd_arch> convert_i64_to_any(simd_register_i64 value) noexcept {
			return convert_to_any<simd_register_i64, SimdRegisterI64Info<simd_arch>>(value);
		}

		constexpr inline static SimdRegisterAny<simd_arch> convert_ui8_to_any(simd_register_ui8 value) noexcept {
			return convert_to_any<simd_register_ui8, SimdRegisterUI8Info<simd_arch>>(value);
		}
		constexpr inline static SimdRegisterAny<simd_arch> convert_ui16_to_any(simd_register_ui16 value) noexcept {
			return convert_to_any<simd_register_ui16, SimdRegisterUI16Info<simd_arch>>(value);
		}
		constexpr inline static SimdRegisterAny<simd_arch> convert_ui32_to_any(simd_register_ui32 value) noexcept {
			return convert_to_any<simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(value);
		}
		constexpr inline static SimdRegisterAny<simd_arch> convert_ui64_to_any(simd_register_ui64 value) noexcept {
			return convert_to_any<simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(value);
		}

		constexpr inline static SimdRegisterAny<simd_arch> convert_f32_to_any(simd_register_f32 value) noexcept {
			return convert_to_any<simd_register_f32, SimdRegisterF32Info<simd_arch>>(value);
		}
		constexpr inline static SimdRegisterAny<simd_arch> convert_f64_to_any(simd_register_f64 value) noexcept {
			return convert_to_any<simd_register_f64, SimdRegisterF64Info<simd_arch>>(value);
		}
		
		template<class Src, class SrcInfo>
		constexpr inline static Src convert_any_to(SimdRegisterAny<simd_arch> src) noexcept {
			using DstDataType = SrcInfo::value_type;
			using UIntByteType = UIntOfByteSize<sizeof(DstDataType)>;

			if (isConstantEvaluated()) {
				Src output{};
				Size srcIndex = 0;
				for (Size dstIndex = 0; dstIndex < SrcInfo::count(); dstIndex++) {
					const Size byteCount = sizeof(DstDataType) / sizeof(ui8);
					for (Size valueIndex = 0; valueIndex < byteCount; valueIndex++, srcIndex++) {
						UIntByteType value = bitCast<UIntByteType, DstDataType>(output[dstIndex]);
						output[dstIndex] = bitCast<DstDataType, UIntByteType>(
							value | (static_cast<UIntByteType>(src[srcIndex]) << ((valueIndex) * 8))
						);
					}
				}
				return output;
			} else {
				Src output;
				std::memcpy(output.values.data(), src.values.data(), sizeof(Src));
				return output;
			}
		}

		//convert any to  
		constexpr inline static simd_register_i8 convert_any_to_i8(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_i8, SimdRegisterI8Info<simd_arch>>(value);
		}
		constexpr inline static simd_register_i16 convert_any_to_i16(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_i16, SimdRegisterI16Info<simd_arch>>(value);
		}
		constexpr inline static simd_register_i32 convert_any_to_i32(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_i32, SimdRegisterI32Info<simd_arch>>(value);
		}
		constexpr inline static simd_register_i64 convert_any_to_i64(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_i64, SimdRegisterI64Info<simd_arch>>(value);
		}

		constexpr inline static simd_register_ui8 convert_any_to_ui8(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_ui8, SimdRegisterUI8Info<simd_arch>>(value);
		}
		constexpr inline static simd_register_ui16 convert_any_to_ui16(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_ui16, SimdRegisterUI16Info<simd_arch>>(value);
		}
		constexpr inline static simd_register_ui32 convert_any_to_ui32(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_ui32, SimdRegisterUI32Info<simd_arch>>(value);
		}
		constexpr inline static simd_register_ui64 convert_any_to_ui64(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_ui64, SimdRegisterUI64Info<simd_arch>>(value);
		}

		constexpr inline static simd_register_f32 convert_any_to_f32(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_f32, SimdRegisterF32Info<simd_arch>>(value);
		}
		constexpr inline static simd_register_f64 convert_any_to_f64(SimdRegisterAny<simd_arch> value) noexcept {
			return convert_any_to<simd_register_f64, SimdRegisterF64Info<simd_arch>>(value);
		}

		template<typename ToSimdRegisterType, typename FromSimdRegisterType>
		constexpr inline static ToSimdRegisterType convertTo(FromSimdRegisterType value) noexcept {
			ToSimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<FromSimdRegisterType>::count(); i++) {
				output[i] = static_cast<typename SimdRegisterToInfo<ToSimdRegisterType>::value_type>(value[i]);
			}
			return output;
		}

		template<typename ToSimdRegisterType, typename FromSimdRegisterType, typename FromSimdMaskType>
		constexpr inline static ToSimdRegisterType mmaskedConvertTo(FromSimdRegisterType value, FromSimdMaskType mmask) noexcept {
			ToSimdRegisterType output{};
			for (Size i = 0; i < SimdRegisterToInfo<FromSimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = static_cast<typename SimdRegisterToInfo<ToSimdRegisterType>::value_type>(value[i]);
				}
			}
			return output;
		}
		template<typename ToSimdRegisterType, typename FromSimdRegisterType, typename FromSimdMaskType>
		constexpr inline static ToSimdRegisterType mmaskedSrcConvertTo(FromSimdRegisterType value, ToSimdRegisterType src, FromSimdMaskType mmask) noexcept {
			ToSimdRegisterType output = src;
			for (Size i = 0; i < SimdRegisterToInfo<FromSimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = static_cast<typename SimdRegisterToInfo<ToSimdRegisterType>::value_type>(value[i]);
				}
			}
			return output;
		}

		//convert to int 
		constexpr inline static simd_register_i8 convert_to_int_ui8(simd_register_ui8 value) noexcept {
			return convertTo<simd_register_i8, simd_register_ui8>(value);
		}
		constexpr inline static simd_register_i16 convert_to_int_ui16(simd_register_ui16 value) noexcept {
			return convertTo<simd_register_i16, simd_register_ui16>(value);
		}
		constexpr inline static simd_register_i32 convert_to_int_ui32(simd_register_ui32 value) noexcept {
			return convertTo<simd_register_i32, simd_register_ui32>(value);
		}
		constexpr inline static simd_register_i64 convert_to_int_ui64(simd_register_ui64 value) noexcept {
			return convertTo<simd_register_i64, simd_register_ui64>(value);
		}
		constexpr inline static simd_register_i32 convert_to_int_f32(simd_register_f32 value) noexcept {
			return convertTo<simd_register_i32, simd_register_f32>(value);
		}
		constexpr inline static simd_register_i64 convert_to_int_f64(simd_register_f64 value) noexcept {
			return convertTo<simd_register_i64, simd_register_f64>(value);
		}

		//mmasked convert to int 
		constexpr inline static simd_register_i8 mmasked_convert_to_int_ui8(simd_register_ui8 value, simd_mmask_ui8 mmask) noexcept {
			return mmaskedConvertTo<simd_register_i8, simd_register_ui8, simd_mmask_ui8>(value, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_convert_to_int_ui16(simd_register_ui16 value, simd_mmask_ui16 mmask) noexcept {
			return mmaskedConvertTo<simd_register_i16, simd_register_ui16, simd_mmask_ui16>(value, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_convert_to_int_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedConvertTo<simd_register_i32, simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_convert_to_int_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedConvertTo<simd_register_i64, simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_convert_to_int_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedConvertTo<simd_register_i32, simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_convert_to_int_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedConvertTo<simd_register_i64, simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked src convert to int 
		constexpr inline static simd_register_i8 mmasked_src_convert_to_int_ui8(simd_register_ui8 value, simd_register_i8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_i8, simd_register_ui8, simd_mmask_ui8>(value, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_convert_to_int_ui16(simd_register_ui16 value, simd_register_i16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_i16, simd_register_ui16, simd_mmask_ui16>(value, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_convert_to_int_ui32(simd_register_ui32 value, simd_register_i32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_i32, simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_convert_to_int_ui64(simd_register_ui64 value, simd_register_i64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_i64, simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_convert_to_int_f32(simd_register_f32 value, simd_register_i32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_i32, simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_convert_to_int_f64(simd_register_f64 value, simd_register_i64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_i64, simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//convert to uint 
		constexpr inline static simd_register_ui8 convert_to_uint_i8(simd_register_i8 value) noexcept {
			return convertTo<simd_register_ui8, simd_register_i8>(value);
		}
		constexpr inline static simd_register_ui16 convert_to_uint_i16(simd_register_i16 value) noexcept {
			return convertTo<simd_register_ui16, simd_register_i16>(value);
		}
		constexpr inline static simd_register_ui32 convert_to_uint_i32(simd_register_i32 value) noexcept {
			return convertTo<simd_register_ui32, simd_register_i32>(value);
		}
		constexpr inline static simd_register_ui64 convert_to_uint_i64(simd_register_i64 value) noexcept {
			return convertTo<simd_register_ui64, simd_register_i64>(value);
		}
		constexpr inline static simd_register_ui32 convert_to_uint_f32(simd_register_f32 value) noexcept {
			return convertTo<simd_register_ui32, simd_register_f32>(value);
		}
		constexpr inline static simd_register_ui64 convert_to_uint_f64(simd_register_f64 value) noexcept {
			return convertTo<simd_register_ui64, simd_register_f64>(value);
		}

		//mmasked convert to uint 
		constexpr inline static simd_register_ui8 mmasked_convert_to_uint_i8(simd_register_i8 value, simd_mmask_i8 mmask) noexcept {
			return mmaskedConvertTo<simd_register_ui8, simd_register_i8, simd_mmask_i8>(value, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_convert_to_uint_i16(simd_register_i16 value, simd_mmask_i16 mmask) noexcept {
			return mmaskedConvertTo<simd_register_ui16, simd_register_i16, simd_mmask_i16>(value, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_convert_to_uint_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedConvertTo<simd_register_ui32, simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_convert_to_uint_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedConvertTo<simd_register_ui64, simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_convert_to_uint_f32(simd_register_f32 value, simd_mmask_f32 mmask) noexcept {
			return mmaskedConvertTo<simd_register_ui32, simd_register_f32, simd_mmask_f32>(value, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_convert_to_uint_f64(simd_register_f64 value, simd_mmask_f64 mmask) noexcept {
			return mmaskedConvertTo<simd_register_ui64, simd_register_f64, simd_mmask_f64>(value, mmask);
		}

		//mmasked src convert to uint 
		constexpr inline static simd_register_ui8 mmasked_src_convert_to_uint_i8(simd_register_i8 value, simd_register_ui8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_ui8, simd_register_i8, simd_mmask_i8>(value, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_convert_to_uint_i16(simd_register_i16 value, simd_register_ui16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_ui16, simd_register_i16, simd_mmask_i16>(value, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_convert_to_uint_i32(simd_register_i32 value, simd_register_ui32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_ui32, simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_convert_to_uint_i64(simd_register_i64 value, simd_register_ui64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_ui64, simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_convert_to_uint_f32(simd_register_f32 value, simd_register_ui32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_ui32, simd_register_f32, simd_mmask_f32>(value, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_convert_to_uint_f64(simd_register_f64 value, simd_register_ui64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_ui64, simd_register_f64, simd_mmask_f64>(value, src, mmask);
		}

		//convert to float 
		constexpr inline static simd_register_f32 convert_to_float_i32(simd_register_i32 value) noexcept {
			return convertTo<simd_register_f32, simd_register_i32>(value);
		}
		constexpr inline static simd_register_f64 convert_to_float_i64(simd_register_i64 value) noexcept {
			return convertTo<simd_register_f64, simd_register_i64>(value);
		}
		constexpr inline static simd_register_f32 convert_to_float_ui32(simd_register_ui32 value) noexcept {
			return convertTo<simd_register_f32, simd_register_ui32>(value);
		}
		constexpr inline static simd_register_f64 convert_to_float_ui64(simd_register_ui64 value) noexcept {
			return convertTo<simd_register_f64, simd_register_ui64>(value);
		}

		//mmasked convert to float 
		constexpr inline static simd_register_f32 mmasked_convert_to_float_i32(simd_register_i32 value, simd_mmask_i32 mmask) noexcept {
			return mmaskedConvertTo<simd_register_f32, simd_register_i32, simd_mmask_i32>(value, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_convert_to_float_i64(simd_register_i64 value, simd_mmask_i64 mmask) noexcept {
			return mmaskedConvertTo<simd_register_f64, simd_register_i64, simd_mmask_i64>(value, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_convert_to_float_ui32(simd_register_ui32 value, simd_mmask_ui32 mmask) noexcept {
			return mmaskedConvertTo<simd_register_f32, simd_register_ui32, simd_mmask_ui32>(value, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_convert_to_float_ui64(simd_register_ui64 value, simd_mmask_ui64 mmask) noexcept {
			return mmaskedConvertTo<simd_register_f64, simd_register_ui64, simd_mmask_ui64>(value, mmask);
		}

		//mmasked src convert to float 
		constexpr inline static simd_register_f32 mmasked_src_convert_to_float_i32(simd_register_i32 value, simd_register_f32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_f32, simd_register_i32, simd_mmask_i32>(value, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_convert_to_float_i64(simd_register_i64 value, simd_register_f64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_f64, simd_register_i64, simd_mmask_i64>(value, src, mmask);
		}
		constexpr inline static simd_register_f32 mmasked_src_convert_to_float_ui32(simd_register_ui32 value, simd_register_f32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_f32, simd_register_ui32, simd_mmask_ui32>(value, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_convert_to_float_ui64(simd_register_ui64 value, simd_register_f64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcConvertTo<simd_register_f64, simd_register_ui64, simd_mmask_ui64>(value, src, mmask);
		}

		//swizzle//

		//insert 
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType insert(SimdRegisterType src, typename SimdRegisterToInfo<SimdRegisterType>::value_type value, const Size index) noexcept {
			SimdRegisterType output = src;
			output[index] = value;
			return output;
		}

		constexpr inline static simd_register_i8 insert_i8(simd_register_i8 src, const i8 value, const Size index) noexcept {
			return insert<simd_register_i8>(src, value, index);
		}
		constexpr inline static simd_register_i16 insert_i16(simd_register_i16 src, const i16 value, const Size index) noexcept {
			return insert<simd_register_i16>(src, value, index);
		}
		constexpr inline static simd_register_i32 insert_i32(simd_register_i32 src, const i32 value, const Size index) noexcept {
			return insert<simd_register_i32>(src, value, index);
		}
		constexpr inline static simd_register_i64 insert_i64(simd_register_i64 src, const i64 value, const Size index) noexcept {
			return insert<simd_register_i64>(src, value, index);
		}

		constexpr inline static simd_register_ui8 insert_ui8(simd_register_ui8 src, const ui8 value, const Size index) noexcept {
			return insert<simd_register_ui8>(src, value, index);
		}
		constexpr inline static simd_register_ui16 insert_ui16(simd_register_ui16 src, const ui16 value, const Size index) noexcept {
			return insert<simd_register_ui16>(src, value, index);
		}
		constexpr inline static simd_register_ui32 insert_ui32(simd_register_ui32 src, const ui32 value, const Size index) noexcept {
			return insert<simd_register_ui32>(src, value, index);
		}
		constexpr inline static simd_register_ui64 insert_ui64(simd_register_ui64 src, const ui64 value, const Size index) noexcept {
			return insert<simd_register_ui64>(src, value, index);
		}

		constexpr inline static simd_register_f32 insert_f32(simd_register_f32 src, const f32 value, const Size index) noexcept {
			return insert<simd_register_f32>(src, value, index);
		}
		constexpr inline static simd_register_f64 insert_f64(simd_register_f64 src, const f64 value, const Size index) noexcept {
			return insert<simd_register_f64>(src, value, index);
		}

		//extract
		template<typename SimdRegisterType>
		constexpr inline static typename SimdRegisterToInfo<SimdRegisterType>::value_type extract(SimdRegisterType src, const Size index) noexcept {
			return src[index];
		}

		constexpr inline static i8 extract_i8(simd_register_i8 src, const Size index) noexcept {
			return extract<simd_register_i8>(src, index);
		}
		constexpr inline static i16 extract_i16(simd_register_i16 src, const Size index) noexcept {
			return extract<simd_register_i16>(src, index);
		}
		constexpr inline static i32 extract_i32(simd_register_i32 src, const Size index) noexcept {
			return extract<simd_register_i32>(src, index);
		}
		constexpr inline static i64 extract_i64(simd_register_i64 src, const Size index) noexcept {
			return extract<simd_register_i64>(src, index);
		}

		constexpr inline static ui8 extract_ui8(simd_register_ui8 src, const Size index) noexcept {
			return extract<simd_register_ui8>(src, index);
		}
		constexpr inline static ui16 extract_ui16(simd_register_ui16 src, const Size index) noexcept {
			return extract<simd_register_ui16>(src, index);
		}
		constexpr inline static ui32 extract_ui32(simd_register_ui32 src, const Size index) noexcept {
			return extract<simd_register_ui32>(src, index);
		}
		constexpr inline static ui64 extract_ui64(simd_register_ui64 src, const Size index) noexcept {
			return extract<simd_register_ui64>(src, index);
		}

		constexpr inline static f32 extract_f32(simd_register_f32 src, const Size index) noexcept {
			return extract<simd_register_f32>(src, index);
		}
		constexpr inline static f64 extract_f64(simd_register_f64 src, const Size index) noexcept {
			return extract<simd_register_f64>(src, index);
		}

		//blend mmask
		template<typename SimdRegisterType, typename MaskType>
		constexpr inline static SimdRegisterType blendMmask(SimdRegisterType a, SimdRegisterType b, MaskType mask) noexcept {
			SimdRegisterType output;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = mask[i] ? a[i] : b[i];
			}
			return output;
		}

		constexpr inline static simd_register_i8 blend_mmask_i8(simd_register_i8 a, simd_register_i8 b, simd_mmask_i8 mask) noexcept {
			return blendMmask<simd_register_i8, simd_mmask_i8>(a, b, mask);
		}
		constexpr inline static simd_register_i16 blend_mmask_i16(simd_register_i16 a, simd_register_i16 b, simd_mmask_i16 mask) noexcept {
			return blendMmask<simd_register_i16, simd_mmask_i16>(a, b, mask);
		}
		constexpr inline static simd_register_i32 blend_mmask_i32(simd_register_i32 a, simd_register_i32 b, simd_mmask_i32 mask) noexcept {
			return blendMmask<simd_register_i32, simd_mmask_i32>(a, b, mask);
		}
		constexpr inline static simd_register_i64 blend_mmask_i64(simd_register_i64 a, simd_register_i64 b, simd_mmask_i64 mask) noexcept {
			return blendMmask<simd_register_i64, simd_mmask_i64>(a, b, mask);
		}

		constexpr inline static simd_register_ui8 blend_mmask_ui8(simd_register_ui8 a, simd_register_ui8 b, simd_mmask_ui8 mask) noexcept {
			return blendMmask<simd_register_ui8, simd_mmask_ui8>(a, b, mask);
		}
		constexpr inline static simd_register_ui16 blend_mmask_ui16(simd_register_ui16 a, simd_register_ui16 b, simd_mmask_ui16 mask) noexcept {
			return blendMmask<simd_register_ui16, simd_mmask_ui16>(a, b, mask);
		}
		constexpr inline static simd_register_ui32 blend_mmask_ui32(simd_register_ui32 a, simd_register_ui32 b, simd_mmask_ui32 mask) noexcept {
			return blendMmask<simd_register_ui32, simd_mmask_ui32>(a, b, mask);
		}
		constexpr inline static simd_register_ui64 blend_mmask_ui64(simd_register_ui64 a, simd_register_ui64 b, simd_mmask_ui64 mask) noexcept {
			return blendMmask<simd_register_ui64, simd_mmask_ui64>(a, b, mask);
		}

		constexpr inline static simd_register_f32 blend_mmask_f32(simd_register_f32 a, simd_register_f32 b, simd_mmask_f32 mask) noexcept {
			return blendMmask<simd_register_f32, simd_mmask_f32>(a, b, mask);
		}
		constexpr inline static simd_register_f64 blend_mmask_f64(simd_register_f64 a, simd_register_f64 b, simd_mmask_f64 mask) noexcept {
			return blendMmask<simd_register_f64, simd_mmask_f64>(a, b, mask);
		}

		//blend rmask
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType blendRmask(SimdRegisterType a, SimdRegisterType b, SimdRegisterType rmask) noexcept {
			SimdRegisterType output;
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					output[i] = (bitCast<ui32, f32>(rmask[i]) == 0) ? a[i] : b[i];
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					output[i] = (bitCast<ui64, f64>(rmask[i]) == 0) ? a[i] : b[i];
				} else {
					output[i] = (rmask[i] == 0) ? a[i] : b[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 blend_rmask_i8(simd_register_i8 a, simd_register_i8 b, simd_register_i8 rmask) noexcept {
			return blendRmask<simd_register_i8>(a, b, rmask);
		}
		constexpr inline static simd_register_i16 blend_rmask_i16(simd_register_i16 a, simd_register_i16 b, simd_register_i16 rmask) noexcept {
			return blendRmask<simd_register_i16>(a, b, rmask);
		}
		constexpr inline static simd_register_i32 blend_rmask_i32(simd_register_i32 a, simd_register_i32 b, simd_register_i32 rmask) noexcept {
			return blendRmask<simd_register_i32>(a, b, rmask);
		}
		constexpr inline static simd_register_i64 blend_rmask_i64(simd_register_i64 a, simd_register_i64 b, simd_register_i64 rmask) noexcept {
			return blendRmask<simd_register_i64>(a, b, rmask);
		}

		constexpr inline static simd_register_ui8 blend_rmask_ui8(simd_register_ui8 a, simd_register_ui8 b, simd_register_ui8 rmask) noexcept {
			return blendRmask<simd_register_ui8>(a, b, rmask);
		}
		constexpr inline static simd_register_ui16 blend_rmask_ui16(simd_register_ui16 a, simd_register_ui16 b, simd_register_ui16 rmask) noexcept {
			return blendRmask<simd_register_ui16>(a, b, rmask);
		}
		constexpr inline static simd_register_ui32 blend_rmask_ui32(simd_register_ui32 a, simd_register_ui32 b, simd_register_ui32 rmask) noexcept {
			return blendRmask<simd_register_ui32>(a, b, rmask);
		}
		constexpr inline static simd_register_ui64 blend_rmask_ui64(simd_register_ui64 a, simd_register_ui64 b, simd_register_ui64 rmask) noexcept {
			return blendRmask<simd_register_ui64>(a, b, rmask);
		}

		constexpr inline static simd_register_f32 blend_rmask_f32(simd_register_f32 a, simd_register_f32 b, simd_register_f32 rmask) noexcept {
			return blendRmask<simd_register_f32>(a, b, rmask);
		}
		constexpr inline static simd_register_f64 blend_rmask_f64(simd_register_f64 a, simd_register_f64 b, simd_register_f64 rmask) noexcept {
			return blendRmask<simd_register_f64>(a, b, rmask);
		}

		//advance mem//

		//broadcast
		template<typename SimdRegisterType>
		constexpr inline static SimdRegisterType broadcast(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr) noexcept {
			SimdRegisterType output;
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			const simd_register_value_type value = *ptr;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				output[i] = value;
			}
			return output;
		}

		constexpr inline static simd_register_i8 broadcast_i8(const i8* ptr) noexcept {
			return broadcast<simd_register_i8>(ptr);
		}
		constexpr inline static simd_register_i16 broadcast_i16(const i16* ptr) noexcept {
			return broadcast<simd_register_i16>(ptr);
		}
		constexpr inline static simd_register_i32 broadcast_i32(const i32* ptr) noexcept {
			return broadcast<simd_register_i32>(ptr);
		}
		constexpr inline static simd_register_i64 broadcast_i64(const i64* ptr) noexcept {
			return broadcast<simd_register_i64>(ptr);
		}

		constexpr inline static simd_register_ui8 broadcast_ui8(const ui8* ptr) noexcept {
			return broadcast<simd_register_ui8>(ptr);
		}
		constexpr inline static simd_register_ui16 broadcast_ui16(const ui16* ptr) noexcept {
			return broadcast<simd_register_ui16>(ptr);
		}
		constexpr inline static simd_register_ui32 broadcast_ui32(const ui32* ptr) noexcept {
			return broadcast<simd_register_ui32>(ptr);
		}
		constexpr inline static simd_register_ui64 broadcast_ui64(const ui64* ptr) noexcept {
			return broadcast<simd_register_ui64>(ptr);
		}

		constexpr inline static simd_register_f32 broadcast_f32(const f32* ptr) noexcept {
			return broadcast<simd_register_f32>(ptr);
		}
		constexpr inline static simd_register_f64 broadcast_f64(const f64* ptr) noexcept {
			return broadcast<simd_register_f64>(ptr);
		}

		//mmasked broadcast
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedBroadcast(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr, SimdMaskType mmask) noexcept {
			SimdRegisterType output{};
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			const simd_register_value_type value = *ptr;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = value;
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_broadcast_i8(const i8* ptr, simd_mmask_i8 mmask) noexcept {
			return mmaskedBroadcast<simd_register_i8, simd_mmask_i8>(ptr, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_broadcast_i16(const i16* ptr, simd_mmask_i16 mmask) noexcept {
			return mmaskedBroadcast<simd_register_i16, simd_mmask_i16>(ptr, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_broadcast_i32(const i32* ptr, simd_mmask_i32 mmask) noexcept {
			return mmaskedBroadcast<simd_register_i32, simd_mmask_i32>(ptr, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_broadcast_i64(const i64* ptr, simd_mmask_i64 mmask) noexcept {
			return mmaskedBroadcast<simd_register_i64, simd_mmask_i64>(ptr, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_broadcast_ui8(const ui8* ptr, simd_mmask_ui8 mmask) noexcept {
			return mmaskedBroadcast<simd_register_ui8, simd_mmask_ui8>(ptr, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_broadcast_ui16(const ui16* ptr, simd_mmask_ui16 mmask) noexcept {
			return mmaskedBroadcast<simd_register_ui16, simd_mmask_ui16>(ptr, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_broadcast_ui32(const ui32* ptr, simd_mmask_ui32 mmask) noexcept {
			return mmaskedBroadcast<simd_register_ui32, simd_mmask_ui32>(ptr, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_broadcast_ui64(const ui64* ptr, simd_mmask_ui64 mmask) noexcept {
			return mmaskedBroadcast<simd_register_ui64, simd_mmask_ui64>(ptr, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_broadcast_f32(const f32* ptr, simd_mmask_f32 mmask) noexcept {
			return mmaskedBroadcast<simd_register_f32, simd_mmask_f32>(ptr, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_broadcast_f64(const f64* ptr, simd_mmask_f64 mmask) noexcept {
			return mmaskedBroadcast<simd_register_f64, simd_mmask_f64>(ptr, mmask);
		}

		//mmasked src broadcast
		template<typename SimdRegisterType, typename SimdMaskType>
		constexpr inline static SimdRegisterType mmaskedSrcBroadcast(const typename SimdRegisterToInfo<SimdRegisterType>::value_type* ptr, SimdRegisterType src, SimdMaskType mmask) noexcept {
			SimdRegisterType output = src;
			using simd_register_value_type = typename SimdRegisterToInfo<SimdRegisterType>::value_type;
			const simd_register_value_type value = *ptr;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if (mmask[i]) {
					output[i] = value;
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmasked_src_broadcast_i8(const i8* ptr, simd_register_i8 src, simd_mmask_i8 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_i8, simd_mmask_i8>(ptr, src, mmask);
		}
		constexpr inline static simd_register_i16 mmasked_src_broadcast_i16(const i16* ptr, simd_register_i16 src, simd_mmask_i16 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_i16, simd_mmask_i16>(ptr, src, mmask);
		}
		constexpr inline static simd_register_i32 mmasked_src_broadcast_i32(const i32* ptr, simd_register_i32 src, simd_mmask_i32 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_i32, simd_mmask_i32>(ptr, src, mmask);
		}
		constexpr inline static simd_register_i64 mmasked_src_broadcast_i64(const i64* ptr, simd_register_i64 src, simd_mmask_i64 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_i64, simd_mmask_i64>(ptr, src, mmask);
		}

		constexpr inline static simd_register_ui8 mmasked_src_broadcast_ui8(const ui8* ptr, simd_register_ui8 src, simd_mmask_ui8 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_ui8, simd_mmask_ui8>(ptr, src, mmask);
		}
		constexpr inline static simd_register_ui16 mmasked_src_broadcast_ui16(const ui16* ptr, simd_register_ui16 src, simd_mmask_ui16 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_ui16, simd_mmask_ui16>(ptr, src, mmask);
		}
		constexpr inline static simd_register_ui32 mmasked_src_broadcast_ui32(const ui32* ptr, simd_register_ui32 src, simd_mmask_ui32 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_ui32, simd_mmask_ui32>(ptr, src, mmask);
		}
		constexpr inline static simd_register_ui64 mmasked_src_broadcast_ui64(const ui64* ptr, simd_register_ui64 src, simd_mmask_ui64 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_ui64, simd_mmask_ui64>(ptr, src, mmask);
		}

		constexpr inline static simd_register_f32 mmasked_src_broadcast_f32(const f32* ptr, simd_register_f32 src, simd_mmask_f32 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_f32, simd_mmask_f32>(ptr, src, mmask);
		}
		constexpr inline static simd_register_f64 mmasked_src_broadcast_f64(const f64* ptr, simd_register_f64 src, simd_mmask_f64 mmask) noexcept {
			return mmaskedSrcBroadcast<simd_register_f64, simd_mmask_f64>(ptr, src, mmask);
		}

		//gather
		constexpr inline static simd_register_i32 gather_i32(const i32* basePtr, simd_register_ui32 index) noexcept {
			simd_register_i32 output;
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * 8);
			}
			return output;
		}
		constexpr inline static simd_register_i64 gather_i64(const i64* basePtr, simd_register_ui64 index) noexcept {
			simd_register_i64 output;
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * 8);
			}
			return output;
		}

		constexpr inline static simd_register_ui32 gather_ui32(const ui32* basePtr, simd_register_ui32 index) noexcept {
			simd_register_ui32 output;
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * 8);
			}
			return output;
		}
		constexpr inline static simd_register_ui64 gather_ui64(const ui64* basePtr, simd_register_ui64 index) noexcept {
			simd_register_ui64 output;
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * 8);
			}
			return output;
		}

		constexpr inline static simd_register_f32 gather_f32(const f32* basePtr, simd_register_ui32 index) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * 8);
			}
			return output;
		}
		constexpr inline static simd_register_f64 gather_f64(const f64* basePtr, simd_register_ui64 index) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * 8);
			}
			return output;
		}

		//gather scaled
		constexpr inline static simd_register_i32 gather_scaled_i32(const i32* basePtr, simd_register_ui32 index, const Size scale) noexcept {
			simd_register_i32 output;
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * scale * 8);
			}
			return output;
		}
		constexpr inline static simd_register_i64 gather_scaled_i64(const i64* basePtr, simd_register_ui64 index, const Size scale) noexcept {
			simd_register_i64 output;
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * scale * 8);
			}
			return output;
		}

		constexpr inline static simd_register_ui32 gather_scaled_ui32(const ui32* basePtr, simd_register_ui32 index, const Size scale) noexcept {
			simd_register_ui32 output;
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * scale * 8);
			}
			return output;
		}
		constexpr inline static simd_register_ui64 gather_scaled_ui64(const ui64* basePtr, simd_register_ui64 index, const Size scale) noexcept {
			simd_register_ui64 output;
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * scale * 8);
			}
			return output;
		}

		constexpr inline static simd_register_f32 gather_scaled_f32(const f32* basePtr, simd_register_ui32 index, const Size scale) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * scale * 8);
			}
			return output;
		}
		constexpr inline static simd_register_f64 gather_scaled_f64(const f64* basePtr, simd_register_ui64 index, const Size scale) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				output[i] = *(basePtr + index[i] * scale * 8);
			}
			return output;
		}

		//gather masked
		constexpr inline static simd_register_i32 mmasked_gather_i32(
			const i32* basePtr, simd_register_ui32 index,
			simd_register_i32 src, simd_mmask_i32 mask) noexcept {
			simd_register_i32 output;
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}
		constexpr inline static simd_register_i64 mmasked_gather_i64(
			const i64* basePtr, simd_register_ui64 index,
			simd_register_i64 src, simd_mmask_i64 mask) noexcept {
			simd_register_i64 output;
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_ui32 mmasked_gather_ui32(
			const ui32* basePtr, simd_register_ui32 index,
			simd_register_ui32 src, simd_mmask_ui32 mask) noexcept {
			simd_register_ui32 output;
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}
		constexpr inline static simd_register_ui64 mmasked_gather_ui64(
			const ui64* basePtr, simd_register_ui64 index,
			simd_register_ui64 src, simd_mmask_ui64 mask) noexcept {
			simd_register_ui64 output;
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_f32 mmasked_gather_f32(
			const f32* basePtr, simd_register_ui32 index,
			simd_register_f32 src, simd_mmask_f32 mask) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_gather_f64(
			const f64* basePtr, simd_register_ui64 index,
			simd_register_f64 src, simd_mmask_f64 mask) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		//masked gather scaled
		constexpr inline static simd_register_i32 mmasked_gather_scaled_i32(
			const i32* basePtr, simd_register_ui32 index,
			simd_register_i32 src, simd_mmask_i32 mask, const Size scale) noexcept {
			simd_register_i32 output;
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * scale * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}
		constexpr inline static simd_register_i64 mmasked_gather_scaled_i64(
			const i64* basePtr, simd_register_ui64 index,
			simd_register_i64 src, simd_mmask_i64 mask, const Size scale) noexcept {
			simd_register_i64 output;
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * scale * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_ui32 mmasked_gather_scaled_ui32(
			const ui32* basePtr, simd_register_ui32 index,
			simd_register_ui32 src, simd_mmask_ui32 mask, const Size scale) noexcept {
			simd_register_ui32 output;
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * scale * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}
		constexpr inline static simd_register_ui64 mmasked_gather_scaled_ui64(
			const ui64* basePtr, simd_register_ui64 index,
			simd_register_ui64 src, simd_mmask_ui64 mask, const Size scale) noexcept {
			simd_register_ui64 output;
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * scale * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		constexpr inline static simd_register_f32 mmasked_gather_scaled_f32(
			const f32* basePtr, simd_register_ui32 index,
			simd_register_f32 src, simd_mmask_f32 mask, const Size scale) noexcept {
			simd_register_f32 output;
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * scale * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}
		constexpr inline static simd_register_f64 mmasked_gather_scaled_f64(
			const f64* basePtr, simd_register_ui64 index,
			simd_register_f64 src, simd_mmask_f64 mask, const Size scale) noexcept {
			simd_register_f64 output;
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					output[i] = *(basePtr + index[i] * scale * 8);
				} else {
					output[i] = src[i];
				}
			}
			return output;
		}

		//scatter
		constexpr inline static void scatter_i32(i32* basePtr, simd_register_ui32 index, simd_register_i32 src) noexcept {
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8) = src[i];
			}
		}
		constexpr inline static void scatter_i64(i64* basePtr, simd_register_ui64 index, simd_register_i64 src) noexcept {
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8) = src[i];
			}
		}

		constexpr inline static void scatter_ui32(ui32* basePtr, simd_register_ui32 index, simd_register_ui32 src) noexcept {
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8) = src[i];
			}
		}
		constexpr inline static void scatter_ui64(ui64* basePtr, simd_register_ui64 index, simd_register_ui64 src) noexcept {
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8) = src[i];
			}
		}

		constexpr inline static void scatter_f32(f32* basePtr, simd_register_ui32 index, simd_register_f32 src) noexcept {
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8) = src[i];
			}
		}
		constexpr inline static void scatter_f64(f64* basePtr, simd_register_ui64 index, simd_register_f64 src) noexcept {
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8) = src[i];
			}
		}

		//scatter scaled 
		constexpr inline static void scatter_scaled_i32(i32* basePtr, simd_register_ui32 index, simd_register_i32 src, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8 * scale) = src[i];
			}
		}
		constexpr inline static void scatter_scaled_i64(i64* basePtr, simd_register_ui64 index, simd_register_i64 src, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8 * scale) = src[i];
			}
		}

		constexpr inline static void scatter_scaled_ui32(ui32* basePtr, simd_register_ui32 index, simd_register_ui32 src, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8 * scale) = src[i];
			}
		}
		constexpr inline static void scatter_scaled_ui64(ui64* basePtr, simd_register_ui64 index, simd_register_ui64 src, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8 * scale) = src[i];
			}
		}

		constexpr inline static void scatter_scaled_f32(f32* basePtr, simd_register_ui32 index, simd_register_f32 src, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8 * scale) = src[i];
			}
		}
		constexpr inline static void scatter_scaled_f64(f64* basePtr, simd_register_ui64 index, simd_register_f64 src, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				*(basePtr + index[i] * 8 * scale) = src[i];
			}
		}

		//masked scatter 
		constexpr inline static void mmasked_scatter_i32(
			i32* basePtr, simd_register_ui32 index,
			simd_register_i32 src, simd_mmask_i32 mask) noexcept {
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * 8) = src[i];
				}
			}
		}
		constexpr inline static void mmasked_scatter_i64(
			i64* basePtr, simd_register_ui64 index,
			simd_register_i64 src, simd_mmask_i64 mask) noexcept {
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * 8) = src[i];
				}
			}
		}

		constexpr inline static void mmasked_scatter_ui32(
			ui32* basePtr, simd_register_ui32 index,
			simd_register_ui32 src, simd_mmask_ui32 mask) noexcept {
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * 8) = src[i];
				}
			}
		}
		constexpr inline static void mmasked_scatter_ui64(
			ui64* basePtr, simd_register_ui64 index,
			simd_register_ui64 src, simd_mmask_ui64 mask) noexcept {
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * 8) = src[i];
				}
			}
		}

		constexpr inline static void mmasked_scatter_f32(
			f32* basePtr, simd_register_ui32 index,
			simd_register_f32 src, simd_mmask_f32 mask) noexcept {
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * 8) = src[i];
				}
			}
		}
		constexpr inline static void mmasked_scatter_f64(
			f64* basePtr, simd_register_ui64 index,
			simd_register_f64 src, simd_mmask_f64 mask) noexcept {
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * 8) = src[i];
				}
			}
		}

		//masked scatter scaled 
		constexpr inline static void mmasked_scatter_scaled_i32(
			i32* basePtr, simd_register_ui32 index,
			simd_register_i32 src, simd_mmask_i32 mask, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * scale * 8) = src[i];
				}
			}
		}
		constexpr inline static void mmasked_scatter_scaled_i64(
			i64* basePtr, simd_register_ui64 index,
			simd_register_i64 src, simd_mmask_i64 mask, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * scale * 8) = src[i];
				}
			}
		}

		constexpr inline static void mmasked_scatter_scaled_ui32(
			ui32* basePtr, simd_register_ui32 index,
			simd_register_ui32 src, simd_mmask_ui32 mask, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterUI32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * scale * 8) = src[i];
				}
			}
		}
		constexpr inline static void mmasked_scatter_scaled_ui64(
			ui64* basePtr, simd_register_ui64 index,
			simd_register_ui64 src, simd_mmask_ui64 mask, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterUI64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * scale * 8) = src[i];
				}
			}
		}

		constexpr inline static void mmasked_scatter_scaled_f32(
			f32* basePtr, simd_register_ui32 index,
			simd_register_f32 src, simd_mmask_f32 mask, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterF32Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * scale * 8) = src[i];
				}
			}
		}
		constexpr inline static void mmasked_scatter_scaled_f64(
			f64* basePtr, simd_register_ui64 index,
			simd_register_f64 src, simd_mmask_f64 mask, const Size scale) noexcept {
			for (Size i = 0; i < SimdRegisterF64Info<simd_arch>::count(); i++) {
				if (mask[i]) {
					*(basePtr + index[i] * scale * 8) = src[i];
				}
			}
		}

		//masks

		//rmask to mmask 
		template<typename SimdRegisterType, typename MaskType>
		constexpr inline static MaskType rmaskToMmask(SimdRegisterType mask) noexcept {
			MaskType output;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				Bool setValue;
				if constexpr (IsSame<simd_register_value_type, f32>) {
					setValue = bitCast<ui32, f32>(mask[i]) != 0;
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					setValue = bitCast<ui64, f64>(mask[i]) != 0;
				} else {
					setValue = mask[i];
				}
				output.setAtIndex(i, setValue);
			}
			return output;
		}

		constexpr inline static simd_mmask_i8 rmask_to_mmask_i8(simd_register_i8 mask) noexcept {
			return rmaskToMmask<simd_register_i8, simd_mmask_i8>(mask);
		}
		constexpr inline static simd_mmask_i16 rmask_to_mmask_i16(simd_register_i16 mask) noexcept {
			return rmaskToMmask<simd_register_i16, simd_mmask_i16>(mask);
		}
		constexpr inline static simd_mmask_i32 rmask_to_mmask_i32(simd_register_i32 mask) noexcept {
			return rmaskToMmask<simd_register_i32, simd_mmask_i32>(mask);
		}
		constexpr inline static simd_mmask_i64 rmask_to_mmask_i64(simd_register_i64 mask) noexcept {
			return rmaskToMmask<simd_register_i64, simd_mmask_i64>(mask);
		}

		constexpr inline static simd_mmask_ui8 rmask_to_mmask_ui8(simd_register_ui8 mask) noexcept {
			return rmaskToMmask<simd_register_ui8, simd_mmask_ui8>(mask);
		}
		constexpr inline static simd_mmask_ui16 rmask_to_mmask_ui16(simd_register_ui16 mask) noexcept {
			return rmaskToMmask<simd_register_ui16, simd_mmask_ui16>(mask);
		}
		constexpr inline static simd_mmask_ui32 rmask_to_mmask_ui32(simd_register_ui32 mask) noexcept {
			return rmaskToMmask<simd_register_ui32, simd_mmask_ui32>(mask);
		}
		constexpr inline static simd_mmask_ui64 rmask_to_mmask_ui64(simd_register_ui64 mask) noexcept {
			return rmaskToMmask<simd_register_ui64, simd_mmask_ui64>(mask);
		}

		constexpr inline static simd_mmask_f32 rmask_to_mmask_f32(simd_register_f32 mask) noexcept {
			return rmaskToMmask<simd_register_f32, simd_mmask_f32>(mask);
		}
		constexpr inline static simd_mmask_f64 rmask_to_mmask_f64(simd_register_f64 mask) noexcept {
			return rmaskToMmask<simd_register_f64, simd_mmask_f64>(mask);
		}

		//rmask to mmask 
		template<typename SimdRegisterType, typename MaskType>
		constexpr inline static SimdRegisterType mmaskToRmask(MaskType mask) noexcept {
			SimdRegisterType output;
			using simd_register_value_type = SimdRegisterToInfo<SimdRegisterType>::value_type;
			for (Size i = 0; i < SimdRegisterToInfo<SimdRegisterType>::count(); i++) {
				if constexpr (IsSame<simd_register_value_type, f32>) {
					output[i] = bitCast<f32, ui32>(setToZeroOrAllOnes<ui32>(mask[i]));
				} else if constexpr (IsSame<simd_register_value_type, f64>) {
					output[i] = bitCast<f64, ui64>(setToZeroOrAllOnes<ui64>(mask[i]));
				} else {
					output[i] = setToZeroOrAllOnes<simd_register_value_type>(mask[i]);
				}
			}
			return output;
		}

		constexpr inline static simd_register_i8 mmask_to_rmask_i8(simd_mmask_i8 mask) noexcept {
			return mmaskToRmask<simd_register_i8, simd_mmask_i8>(mask);
		}
		constexpr inline static simd_register_i16 mmask_to_rmask_i16(simd_mmask_i16 mask) noexcept {
			return mmaskToRmask<simd_register_i16, simd_mmask_i16>(mask);
		}
		constexpr inline static simd_register_i32 mmask_to_rmask_i32(simd_mmask_i32 mask) noexcept {
			return mmaskToRmask<simd_register_i32, simd_mmask_i32>(mask);
		}
		constexpr inline static simd_register_i64 mmask_to_rmask_i64(simd_mmask_i64 mask) noexcept {
			return mmaskToRmask<simd_register_i64, simd_mmask_i64>(mask);
		}

		constexpr inline static simd_register_ui8 mmask_to_rmask_ui8(simd_mmask_ui8 mask) noexcept {
			return mmaskToRmask<simd_register_ui8, simd_mmask_ui8>(mask);
		}
		constexpr inline static simd_register_ui16 mmask_to_rmask_ui16(simd_mmask_ui16 mask) noexcept {
			return mmaskToRmask<simd_register_ui16, simd_mmask_ui16>(mask);
		}
		constexpr inline static simd_register_ui32 mmask_to_rmask_ui32(simd_mmask_ui32 mask) noexcept {
			return mmaskToRmask<simd_register_ui32, simd_mmask_ui32>(mask);
		}
		constexpr inline static simd_register_ui64 mmask_to_rmask_ui64(simd_mmask_ui64 mask) noexcept {
			return mmaskToRmask<simd_register_ui64, simd_mmask_ui64>(mask);
		}

		constexpr inline static simd_register_f32 mmask_to_rmask_f32(simd_mmask_f32 mask) noexcept {
			return mmaskToRmask<simd_register_f32, simd_mmask_f32>(mask);
		}
		constexpr inline static simd_register_f64 mmask_to_rmask_f64(simd_mmask_f64 mask) noexcept {
			return mmaskToRmask<simd_register_f64, simd_mmask_f64>(mask);
		}

		//cmask to mmask
		template<typename CMaskType, typename MaskType>
		constexpr inline static MaskType cmaskToMmask(const CMaskType cmask) noexcept {
			return MaskType(cmask.value());
		}

		constexpr inline static simd_mmask_i8 cmask_to_mmask_i8(simd_cmask_i8 mask) noexcept {
			return cmaskToMmask<simd_cmask_i8, simd_mmask_i8>(mask);
		}
		constexpr inline static simd_mmask_i16 cmask_to_mmask_i16(simd_cmask_i16 mask) noexcept {
			return cmaskToMmask<simd_cmask_i16, simd_mmask_i16>(mask);
		}
		constexpr inline static simd_mmask_i32 cmask_to_mmask_i32(simd_cmask_i32 mask) noexcept {
			return cmaskToMmask<simd_cmask_i32, simd_mmask_i32>(mask);
		}
		constexpr inline static simd_mmask_i64 cmask_to_mmask_i64(simd_cmask_i64 mask) noexcept {
			return cmaskToMmask<simd_cmask_i64, simd_mmask_i64>(mask);
		}

		constexpr inline static simd_mmask_ui8 cmask_to_mmask_ui8(simd_cmask_ui8 mask) noexcept {
			return cmaskToMmask<simd_cmask_ui8, simd_mmask_ui8>(mask);
		}
		constexpr inline static simd_mmask_ui16 cmask_to_mmask_ui16(simd_cmask_ui16 mask) noexcept {
			return cmaskToMmask<simd_cmask_ui16, simd_mmask_ui16>(mask);
		}
		constexpr inline static simd_mmask_ui32 cmask_to_mmask_ui32(simd_cmask_ui32 mask) noexcept {
			return cmaskToMmask<simd_cmask_ui32, simd_mmask_ui32>(mask);
		}
		constexpr inline static simd_mmask_ui64 cmask_to_mmask_ui64(simd_cmask_ui64 mask) noexcept {
			return cmaskToMmask<simd_cmask_ui64, simd_mmask_ui64>(mask);
		}

		constexpr inline static simd_mmask_f32 cmask_to_mmask_f32(simd_cmask_f32 mask) noexcept {
			return cmaskToMmask<simd_cmask_f32, simd_mmask_f32>(mask);
		}
		constexpr inline static simd_mmask_f64 cmask_to_mmask_f64(simd_cmask_f64 mask) noexcept {
			return cmaskToMmask<simd_cmask_f64, simd_mmask_f64>(mask);
		}

		//cmask to rmask
		template<typename CMaskType, typename MMaskType, typename SimdRegisterType>
		constexpr inline static SimdRegisterType cmaskToRmask(const CMaskType cmask) noexcept {
			return mmaskToRmask<SimdRegisterType, MMaskType>(cmaskToMmask<CMaskType, MMaskType>(cmask));
		}

		constexpr inline static simd_register_i8 cmask_to_rmask_i8(simd_cmask_i8 mask) noexcept {
			return cmaskToRmask<simd_cmask_i8, simd_mmask_i8, simd_register_i8>(mask);
		}
		constexpr inline static simd_register_i16 cmask_to_rmask_i16(simd_cmask_i16 mask) noexcept {
			return cmaskToRmask<simd_cmask_i16, simd_mmask_i16, simd_register_i16>(mask);
		}
		constexpr inline static simd_register_i32 cmask_to_rmask_i32(simd_cmask_i32 mask) noexcept {
			return cmaskToRmask<simd_cmask_i32, simd_mmask_i32, simd_register_i32>(mask);
		}
		constexpr inline static simd_register_i64 cmask_to_rmask_i64(simd_cmask_i64 mask) noexcept {
			return cmaskToRmask<simd_cmask_i64, simd_mmask_i64, simd_register_i64>(mask);
		}

		constexpr inline static simd_register_ui8 cmask_to_rmask_ui8(simd_cmask_ui8 mask) noexcept {
			return cmaskToRmask<simd_cmask_ui8, simd_mmask_ui8, simd_register_ui8>(mask);
		}
		constexpr inline static simd_register_ui16 cmask_to_rmask_ui16(simd_cmask_ui16 mask) noexcept {
			return cmaskToRmask<simd_cmask_ui16, simd_mmask_ui16, simd_register_ui16>(mask);
		}
		constexpr inline static simd_register_ui32 cmask_to_rmask_ui32(simd_cmask_ui32 mask) noexcept {
			return cmaskToRmask<simd_cmask_ui32, simd_mmask_ui32, simd_register_ui32>(mask);
		}
		constexpr inline static simd_register_ui64 cmask_to_rmask_ui64(simd_cmask_ui64 mask) noexcept {
			return cmaskToRmask<simd_cmask_ui64, simd_mmask_ui64, simd_register_ui64>(mask);
		}

		constexpr inline static simd_register_f32 cmask_to_rmask_f32(simd_cmask_f32 mask) noexcept {
			return cmaskToRmask<simd_cmask_f32, simd_mmask_f32, simd_register_f32>(mask);
		}
		constexpr inline static simd_register_f64 cmask_to_rmask_f64(simd_cmask_f64 mask) noexcept {
			return cmaskToRmask<simd_cmask_f64, simd_mmask_f64, simd_register_f64>(mask);
		}
	};
}