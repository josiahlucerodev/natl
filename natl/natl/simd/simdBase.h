#pragma once

//own
#include "../util/basicTypes.h"
#include "../util/typeTraits.h"
#include "../util/bits.h"
#include "math.h"

//interface
namespace natl::simd {
	template<class Type>
	concept IsSimdDataType = IsBuiltInNumericC<Type>;

	template<class Arch>
	struct ArchSimdRegisters;

	template<class Arch>
	concept IsSimdArch = requires(Arch arch) {
		{ ArchSimdRegisters<Arch>{} };
	};

	template<typename SimdRegisterType>
	struct SimdRegisterToInfoT;


	template<typename SimdRegisterType>
	using SimdRegisterToInfo = SimdRegisterToInfoT<SimdRegisterType>::type;

	//simd op
	template<class Arch>
		requires(IsSimdArch<Arch>)
	struct ArchSimdOp {};

	//SimdRegisterI8;
	//SimdRegisterI16;
	//SimdRegisterI32;
	//SimdRegisterI64;
	//SimdRegisterUI8;
	//SimdRegisterUI16;
	//SimdRegisterUI32;
	//SimdRegisterUI64;
	//SimdRegisterF32;
	//SimdRegisterF64;
	//AnyRegister;

	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI8; })
	using SimdRegisterI8 = typename ArchSimdRegisters<Arch>::SimdRegisterI8;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI16; })
	using SimdRegisterI16 = typename ArchSimdRegisters<Arch>::SimdRegisterI16;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI32; })
	using SimdRegisterI32 = typename ArchSimdRegisters<Arch>::SimdRegisterI32;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI64; })
	using SimdRegisterI64 = typename ArchSimdRegisters<Arch>::SimdRegisterI64;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI8; })
	using SimdRegisterUI8 = typename ArchSimdRegisters<Arch>::SimdRegisterUI8;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI16; })
	using SimdRegisterUI16 = typename ArchSimdRegisters<Arch>::SimdRegisterUI16;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI32; })
	using SimdRegisterUI32 = typename ArchSimdRegisters<Arch>::SimdRegisterUI32;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI64; })
	using SimdRegisterUI64 = typename ArchSimdRegisters<Arch>::SimdRegisterUI64;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterF32; })
	using SimdRegisterF32 = typename ArchSimdRegisters<Arch>::SimdRegisterF32;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterF64; })
	using SimdRegisterF64 = typename ArchSimdRegisters<Arch>::SimdRegisterF64;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterAny; })
	using SimdRegisterAny = typename ArchSimdRegisters<Arch>::SimdRegisterAny;

	//ArchSimdRegistersInfo//
	class ArchSimdRegistersInfo;

	//SimdRegisterI8Info
	//SimdRegisterI16Info
	//SimdRegisterI32Info
	//SimdRegisterI64Info
	//SimdRegisterUI8Info
	//SimdRegisterUI16Info
	//SimdRegisterUI32Info
	//SimdRegisterUI64Info
	//SimdRegisterF32Info
	//SimdRegisterF64Info
	//AnyRegisterInfo

	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI8Info; })
	using SimdRegisterI8Info = typename ArchSimdRegisters<Arch>::SimdRegisterI8Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI16Info; })
	using SimdRegisterI16Info = typename ArchSimdRegisters<Arch>::SimdRegisterI16Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI32Info; })
	using SimdRegisterI32Info = typename ArchSimdRegisters<Arch>::SimdRegisterI32Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterI64Info; })
	using SimdRegisterI64Info = typename ArchSimdRegisters<Arch>::SimdRegisterI64Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI8Info; })
	using SimdRegisterUI8Info = typename ArchSimdRegisters<Arch>::SimdRegisterUI8Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI16Info; })
	using SimdRegisterUI16Info = typename ArchSimdRegisters<Arch>::SimdRegisterUI16Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI32Info; })
	using SimdRegisterUI32Info = typename ArchSimdRegisters<Arch>::SimdRegisterUI32Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterUI64Info; })
	using SimdRegisterUI64Info = typename ArchSimdRegisters<Arch>::SimdRegisterUI64Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterF32Info; })
	using SimdRegisterF32Info = typename ArchSimdRegisters<Arch>::SimdRegisterF32Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterF64Info; })
	using SimdRegisterF64Info = typename ArchSimdRegisters<Arch>::SimdRegisterF64Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdRegisters<Arch>::SimdRegisterAnyInfo; })
	using SimdRegisterAnyInfo = typename ArchSimdRegisters<Arch>::SimdRegisterAnyInfo;

	//simd mask op 
	template<class Arch>
	struct ArchSimdMaskOp;

	//mask
	template<class Arch>
	class ArchSimdMask;

	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskI8; })
	using SimdMaskI8 = typename ArchSimdMask<Arch>::SimdMaskI8;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskI16; })
	using SimdMaskI16 = typename ArchSimdMask<Arch>::SimdMaskI16;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskI32; })
	using SimdMaskI32 = typename ArchSimdMask<Arch>::SimdMaskI32;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskI64; })
	using SimdMaskI64 = typename ArchSimdMask<Arch>::SimdMaskI64;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskUI8; })
	using SimdMaskUI8 = typename ArchSimdMask<Arch>::SimdMaskUI8;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskUI16; })
	using SimdMaskUI16 = typename ArchSimdMask<Arch>::SimdMaskUI16;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskUI32; })
	using SimdMaskUI32 = typename ArchSimdMask<Arch>::SimdMaskUI32;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskUI64; })
	using SimdMaskUI64 = typename ArchSimdMask<Arch>::SimdMaskUI64;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskF32; })
	using SimdMaskF32 = typename ArchSimdMask<Arch>::SimdMaskF32;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMask<Arch>::SimdMaskF64; })
	using SimdMaskF64 = typename ArchSimdMask<Arch>::SimdMaskF64;

	template<class Arch>
	class ArchSimdMaskInfo;

	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI8Info; })
	using SimdMaskI8Info = typename ArchSimdMaskInfo<Arch>::SimdMaskI8Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI16Info; })
	using SimdMaskI16Info = typename ArchSimdMaskInfo<Arch>::SimdMaskI16Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI32Info; })
	using SimdMaskI32Info = typename ArchSimdMaskInfo<Arch>::SimdMaskI32Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI64Info; })
	using SimdMaskI64Info = typename ArchSimdMaskInfo<Arch>::SimdMaskI64Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI8Info; })
	using SimdMaskUI8Info = typename ArchSimdMaskInfo<Arch>::SimdMaskUI8Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI16Info; })
	using SimdMaskUI16Info = typename ArchSimdMaskInfo<Arch>::SimdMaskUI16Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI32Info; })
	using SimdMaskUI32Info = typename ArchSimdMaskInfo<Arch>::SimdMaskUI32Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI64Info; })
	using SimdMaskUI64Info = typename ArchSimdMaskInfo<Arch>::SimdMaskUI64Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskF32Info; })
	using SimdMaskF32Info = typename ArchSimdMaskInfo<Arch>::SimdMaskF32Info;
	template<class Arch>
		requires(IsSimdArch<Arch> && requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskF64Info; })
	using SimdMaskF64Info = typename ArchSimdMaskInfo<Arch>::SimdMaskF64Info;

	namespace impl {
		template<class DataType, Size RegisterSize>
		class DefaultSimdMaskBase {
		private:
			static constexpr inline Size count() { return RegisterSize / (sizeof(DataType) * 8); };
			Size maskData;
		public:
			constexpr DefaultSimdMaskBase() noexcept = default;
			explicit constexpr DefaultSimdMaskBase(const Size value) noexcept : maskData(value) {}
			constexpr void set(const Size value) noexcept { maskData = value; }
			constexpr void setAllInactive() noexcept { maskData = Size(1); }
			constexpr void setAllActive() noexcept { maskData = ~Size(1); }
			constexpr void setAtIndex(const Size index, const Bool value) noexcept {
				maskData = setBit<Size>(maskData, index, value);
			}
			constexpr Bool operator[](const Size index) const noexcept { return getBit<Size>(maskData, index); }
			constexpr Size value() noexcept { return maskData;  }
		};
	}

	template<Size RegisterSize>
	class DefaultArchSimdMask {
	public:
		using SimdMaskI8 = impl::DefaultSimdMaskBase<i8, RegisterSize>;
		using SimdMaskI16 = impl::DefaultSimdMaskBase<i16, RegisterSize>;
		using SimdMaskI32 = impl::DefaultSimdMaskBase<i32, RegisterSize>;
		using SimdMaskI64 = impl::DefaultSimdMaskBase<i64, RegisterSize>;
		using SimdMaskUI8 = impl::DefaultSimdMaskBase<ui8, RegisterSize>;
		using SimdMaskUI16 = impl::DefaultSimdMaskBase<ui16, RegisterSize>;
		using SimdMaskUI32 = impl::DefaultSimdMaskBase<ui32, RegisterSize>;
		using SimdMaskUI64 = impl::DefaultSimdMaskBase<ui64, RegisterSize>;
		using SimdMaskF32 = impl::DefaultSimdMaskBase<f32, RegisterSize>;
		using SimdMaskF64 = impl::DefaultSimdMaskBase<f64, RegisterSize>;
	};

	namespace impl {
		template<class DataType, Size RegisterSize>
		class DefaultSimdMaskInfoBase {
		public:
			static constexpr inline Size count() { return RegisterSize / (sizeof(DataType) * 8); };
			static constexpr inline Size bitSize() { return RegisterSize; };
			using value_type = DataType;
		};
	}

	template<ui32 RegisterSize>
	class DefaultArchSimdMaskInfo {
	public:
		using SimdMaskI8Info = impl::DefaultSimdMaskInfoBase<i8, RegisterSize>;
		using SimdMaskI16Info = impl::DefaultSimdMaskInfoBase<i16, RegisterSize>;
		using SimdMaskI32Info = impl::DefaultSimdMaskInfoBase<i32, RegisterSize>;
		using SimdMaskI64Info = impl::DefaultSimdMaskInfoBase<i64, RegisterSize>;
		using SimdMaskUI8Info = impl::DefaultSimdMaskInfoBase<ui8, RegisterSize>;
		using SimdMaskUI16Info = impl::DefaultSimdMaskInfoBase<ui16, RegisterSize>;
		using SimdMaskUI32Info = impl::DefaultSimdMaskInfoBase<ui32, RegisterSize>;
		using SimdMaskUI64Info = impl::DefaultSimdMaskInfoBase<ui64, RegisterSize>;
		using SimdMaskF32Info = impl::DefaultSimdMaskInfoBase<f32, RegisterSize>;
		using SimdMaskF64Info = impl::DefaultSimdMaskInfoBase<f64, RegisterSize>;
	};

	//mask op 
	template<class Arch>
	struct DefaultArchSimdMaskOp {
	public:
		//mask compare equal
		template<typename SimdMaskType>
		constexpr inline static Bool maskCompareEqual(SimdMaskType lhs, SimdMaskType rhs) noexcept {
			return lhs.value() == rhs.value();
		}

		constexpr inline static Bool mask_compare_equal_i8(SimdMaskI8<Arch> lhs, SimdMaskI8<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskI8<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_i16(SimdMaskI16<Arch> lhs, SimdMaskI16<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskI16<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_i32(SimdMaskI32<Arch> lhs, SimdMaskI32<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskI32<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_i64(SimdMaskI64<Arch> lhs, SimdMaskI64<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskI64<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_ui8(SimdMaskUI8<Arch> lhs, SimdMaskUI8<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskUI8<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_ui16(SimdMaskUI16<Arch> lhs, SimdMaskUI16<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskUI16<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_ui32(SimdMaskUI32<Arch> lhs, SimdMaskUI32<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskUI32<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_ui64(SimdMaskUI64<Arch> lhs, SimdMaskUI64<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskUI64<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_f32(SimdMaskF32<Arch> lhs, SimdMaskF32<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskF32<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_equal_f64(SimdMaskF64<Arch> lhs, SimdMaskF64<Arch> rhs) noexcept { return maskCompareEqual<SimdMaskF64<Arch>>(lhs, rhs); }

		//mask compare not equal
		template<typename SimdMaskType>
		constexpr inline static Bool maskCompareNotEqaul(SimdMaskType lhs, SimdMaskType rhs) noexcept {
			return lhs.value() != rhs.value();
		}

		constexpr inline static Bool mask_compare_not_equal_i8(SimdMaskI8<Arch> lhs, SimdMaskI8<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskI8<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_i16(SimdMaskI16<Arch> lhs, SimdMaskI16<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskI16<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_i32(SimdMaskI32<Arch> lhs, SimdMaskI32<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskI32<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_i64(SimdMaskI64<Arch> lhs, SimdMaskI64<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskI64<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_ui8(SimdMaskUI8<Arch> lhs, SimdMaskUI8<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskUI8<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_ui16(SimdMaskUI16<Arch> lhs, SimdMaskUI16<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskUI16<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_ui32(SimdMaskUI32<Arch> lhs, SimdMaskUI32<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskUI32<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_ui64(SimdMaskUI64<Arch> lhs, SimdMaskUI64<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskUI64<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_f32(SimdMaskF32<Arch> lhs, SimdMaskF32<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskF32<Arch>>(lhs, rhs); }
		constexpr inline static Bool mask_compare_not_equal_f64(SimdMaskF64<Arch> lhs, SimdMaskF64<Arch> rhs) noexcept { return maskCompareNotEqaul<SimdMaskF64<Arch>>(lhs, rhs); }

		//mask shift left
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskShiftLeft(SimdMaskType value, const Size count) noexcept {
			return SimdMaskType(value.value() >> count);
		}

		constexpr inline static SimdMaskI8<Arch> mask_bitwise_shift_left_i8(SimdMaskI8<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskI8<Arch>>(value, count); }
		constexpr inline static SimdMaskI16<Arch> mask_bitwise_shift_left_i16(SimdMaskI16<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskI16<Arch>>(value, count); }
		constexpr inline static SimdMaskI32<Arch> mask_bitwise_shift_left_i32(SimdMaskI32<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskI32<Arch>>(value, count); }
		constexpr inline static SimdMaskI64<Arch> mask_bitwise_shift_left_i64(SimdMaskI64<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskI64<Arch>>(value, count); }
		constexpr inline static SimdMaskUI8<Arch> mask_bitwise_shift_left_ui8(SimdMaskUI8<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskUI8<Arch>>(value, count); }
		constexpr inline static SimdMaskUI16<Arch> mask_bitwise_shift_left_ui16(SimdMaskUI16<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskUI16<Arch>>(value, count); }
		constexpr inline static SimdMaskUI32<Arch> mask_bitwise_shift_left_ui32(SimdMaskUI32<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskUI32<Arch>>(value, count); }
		constexpr inline static SimdMaskUI64<Arch> mask_bitwise_shift_left_ui64(SimdMaskUI64<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskUI64<Arch>>(value, count); }
		constexpr inline static SimdMaskF32<Arch> mask_bitwise_shift_left_f32(SimdMaskF32<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskF32<Arch>>(value, count); }
		constexpr inline static SimdMaskF64<Arch> mask_bitwise_shift_left_f64(SimdMaskF64<Arch> value, const Size count) noexcept { return maskShiftLeft<SimdMaskF64<Arch>>(value, count); }

		//mask shift right
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskShiftRight(SimdMaskType value, const Size count) noexcept {
			return SimdMaskType(value.value() << count);
		}

		constexpr inline static SimdMaskI8<Arch> mask_bitwise_shift_right_i8(SimdMaskI8<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskI8<Arch>>(value, count); }
		constexpr inline static SimdMaskI16<Arch> mask_bitwise_shift_right_i16(SimdMaskI16<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskI16<Arch>>(value, count); }
		constexpr inline static SimdMaskI32<Arch> mask_bitwise_shift_right_i32(SimdMaskI32<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskI32<Arch>>(value, count); }
		constexpr inline static SimdMaskI64<Arch> mask_bitwise_shift_right_i64(SimdMaskI64<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskI64<Arch>>(value, count); }
		constexpr inline static SimdMaskUI8<Arch> mask_bitwise_shift_right_ui8(SimdMaskUI8<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskUI8<Arch>>(value, count); }
		constexpr inline static SimdMaskUI16<Arch> mask_bitwise_shift_right_ui16(SimdMaskUI16<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskUI16<Arch>>(value, count); }
		constexpr inline static SimdMaskUI32<Arch> mask_bitwise_shift_right_ui32(SimdMaskUI32<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskUI32<Arch>>(value, count); }
		constexpr inline static SimdMaskUI64<Arch> mask_bitwise_shift_right_ui64(SimdMaskUI64<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskUI64<Arch>>(value, count); }
		constexpr inline static SimdMaskF32<Arch> mask_bitwise_shift_right_f32(SimdMaskF32<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskF32<Arch>>(value, count); }
		constexpr inline static SimdMaskF64<Arch> mask_bitwise_shift_right_f64(SimdMaskF64<Arch> value, const Size count) noexcept { return maskShiftRight<SimdMaskF64<Arch>>(value, count); }

		//mask and
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskAnd(SimdMaskType lhs, SimdMaskType rhs) noexcept {
			return SimdMaskType(lhs.value() & rhs.value());
		}

		constexpr inline static SimdMaskI8<Arch> mask_bitwise_and_i8(SimdMaskI8<Arch> lhs, SimdMaskI8<Arch> rhs) noexcept { return maskAnd<SimdMaskI8<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI16<Arch> mask_bitwise_and_i16(SimdMaskI16<Arch> lhs, SimdMaskI16<Arch> rhs) noexcept { return maskAnd<SimdMaskI16<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI32<Arch> mask_bitwise_and_i32(SimdMaskI32<Arch> lhs, SimdMaskI32<Arch> rhs) noexcept { return maskAnd<SimdMaskI32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI64<Arch> mask_bitwise_and_i64(SimdMaskI64<Arch> lhs, SimdMaskI64<Arch> rhs) noexcept { return maskAnd<SimdMaskI64<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI8<Arch> mask_bitwise_and_ui8(SimdMaskUI8<Arch> lhs, SimdMaskUI8<Arch> rhs) noexcept { return maskAnd<SimdMaskUI8<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI16<Arch> mask_bitwise_and_ui16(SimdMaskUI16<Arch> lhs, SimdMaskUI16<Arch> rhs) noexcept { return maskAnd<SimdMaskUI16<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI32<Arch> mask_bitwise_and_ui32(SimdMaskUI32<Arch> lhs, SimdMaskUI32<Arch> rhs) noexcept { return maskAnd<SimdMaskUI32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI64<Arch> mask_bitwise_and_ui64(SimdMaskUI64<Arch> lhs, SimdMaskUI64<Arch> rhs) noexcept { return maskAnd<SimdMaskUI64<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskF32<Arch> mask_bitwise_and_f32(SimdMaskF32<Arch> lhs, SimdMaskF32<Arch> rhs) noexcept { return maskAnd<SimdMaskF32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskF64<Arch> mask_bitwise_and_f64(SimdMaskF64<Arch> lhs, SimdMaskF64<Arch> rhs) noexcept { return maskAnd<SimdMaskF64<Arch>>(lhs, rhs); }

		//mask or
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskOr(SimdMaskType lhs, SimdMaskType rhs) noexcept {
			return SimdMaskType(lhs.value() | rhs.value());
		}

		constexpr inline static SimdMaskI8<Arch> mask_bitwise_or_i8(SimdMaskI8<Arch> lhs, SimdMaskI8<Arch> rhs) noexcept { return maskOr<SimdMaskI8<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI16<Arch> mask_bitwise_or_i16(SimdMaskI16<Arch> lhs, SimdMaskI16<Arch> rhs) noexcept { return maskOr<SimdMaskI16<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI32<Arch> mask_bitwise_or_i32(SimdMaskI32<Arch> lhs, SimdMaskI32<Arch> rhs) noexcept { return maskOr<SimdMaskI32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI64<Arch> mask_bitwise_or_i64(SimdMaskI64<Arch> lhs, SimdMaskI64<Arch> rhs) noexcept { return maskOr<SimdMaskI64<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI8<Arch> mask_bitwise_or_ui8(SimdMaskUI8<Arch> lhs, SimdMaskUI8<Arch> rhs) noexcept { return maskOr<SimdMaskUI8<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI16<Arch> mask_bitwise_or_ui16(SimdMaskUI16<Arch> lhs, SimdMaskUI16<Arch> rhs) noexcept { return maskOr<SimdMaskUI16<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI32<Arch> mask_bitwise_or_ui32(SimdMaskUI32<Arch> lhs, SimdMaskUI32<Arch> rhs) noexcept { return maskOr<SimdMaskUI32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI64<Arch> mask_bitwise_or_ui64(SimdMaskUI64<Arch> lhs, SimdMaskUI64<Arch> rhs) noexcept { return maskOr<SimdMaskUI64<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskF32<Arch> mask_bitwise_or_f32(SimdMaskF32<Arch> lhs, SimdMaskF32<Arch> rhs) noexcept { return maskOr<SimdMaskF32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskF64<Arch> mask_bitwise_or_f64(SimdMaskF64<Arch> lhs, SimdMaskF64<Arch> rhs) noexcept { return maskOr<SimdMaskF64<Arch>>(lhs, rhs); }

		//mask xor
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskXor(SimdMaskType lhs, SimdMaskType rhs) noexcept {
			return SimdMaskType(lhs.value() ^ rhs.value());
		}

		constexpr inline static SimdMaskI8<Arch> mask_bitwise_xor_i8(SimdMaskI8<Arch> lhs, SimdMaskI8<Arch> rhs) noexcept { return maskXor<SimdMaskI8<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI16<Arch> mask_bitwise_xor_i16(SimdMaskI16<Arch> lhs, SimdMaskI16<Arch> rhs) noexcept { return maskXor<SimdMaskI16<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI32<Arch> mask_bitwise_xor_i32(SimdMaskI32<Arch> lhs, SimdMaskI32<Arch> rhs) noexcept { return maskXor<SimdMaskI32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskI64<Arch> mask_bitwise_xor_i64(SimdMaskI64<Arch> lhs, SimdMaskI64<Arch> rhs) noexcept { return maskXor<SimdMaskI64<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI8<Arch> mask_bitwise_xor_ui8(SimdMaskUI8<Arch> lhs, SimdMaskUI8<Arch> rhs) noexcept { return maskXor<SimdMaskUI8<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI16<Arch> mask_bitwise_xor_ui16(SimdMaskUI16<Arch> lhs, SimdMaskUI16<Arch> rhs) noexcept { return maskXor<SimdMaskUI16<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI32<Arch> mask_bitwise_xor_ui32(SimdMaskUI32<Arch> lhs, SimdMaskUI32<Arch> rhs) noexcept { return maskXor<SimdMaskUI32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskUI64<Arch> mask_bitwise_xor_ui64(SimdMaskUI64<Arch> lhs, SimdMaskUI64<Arch> rhs) noexcept { return maskXor<SimdMaskUI64<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskF32<Arch> mask_bitwise_xor_f32(SimdMaskF32<Arch> lhs, SimdMaskF32<Arch> rhs) noexcept { return maskXor<SimdMaskF32<Arch>>(lhs, rhs); }
		constexpr inline static SimdMaskF64<Arch> mask_bitwise_xor_f64(SimdMaskF64<Arch> lhs, SimdMaskF64<Arch> rhs) noexcept { return maskXor<SimdMaskF64<Arch>>(lhs, rhs); }

		//not
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskNot(SimdMaskType value) noexcept {
			return SimdMaskType(~value.value());
		}

		constexpr inline static SimdMaskI8<Arch> mask_bitwise_not_i8(SimdMaskI8<Arch> value) noexcept { return maskNot<SimdMaskI8<Arch>>(value); }
		constexpr inline static SimdMaskI16<Arch> mask_bitwise_not_i16(SimdMaskI16<Arch> value) noexcept { return maskNot<SimdMaskI16<Arch>>(value); }
		constexpr inline static SimdMaskI32<Arch> mask_bitwise_not_i32(SimdMaskI32<Arch> value) noexcept { return maskNot<SimdMaskI32<Arch>>(value); }
		constexpr inline static SimdMaskI64<Arch> mask_bitwise_not_i64(SimdMaskI64<Arch> value) noexcept { return maskNot<SimdMaskI64<Arch>>(value); }
		constexpr inline static SimdMaskUI8<Arch> mask_bitwise_not_ui8(SimdMaskUI8<Arch> value) noexcept { return maskNot<SimdMaskUI8<Arch>>(value); }
		constexpr inline static SimdMaskUI16<Arch> mask_bitwise_not_ui16(SimdMaskUI16<Arch> value) noexcept { return maskNot<SimdMaskUI16<Arch>>(value); }
		constexpr inline static SimdMaskUI32<Arch> mask_bitwise_not_ui32(SimdMaskUI32<Arch> value) noexcept { return maskNot<SimdMaskUI32<Arch>>(value); }
		constexpr inline static SimdMaskUI64<Arch> mask_bitwise_not_ui64(SimdMaskUI64<Arch> value) noexcept { return maskNot<SimdMaskUI64<Arch>>(value); }
		constexpr inline static SimdMaskF32<Arch> mask_bitwise_not_f32(SimdMaskF32<Arch> value) noexcept { return maskNot<SimdMaskF32<Arch>>(value); }
		constexpr inline static SimdMaskF64<Arch> mask_bitwise_not_f64(SimdMaskF64<Arch> value) noexcept { return maskNot<SimdMaskF64<Arch>>(value); }

		//popcount 
		template<typename SimdMaskType>
		constexpr inline static Size maskPopcount(SimdMaskType value) noexcept {
			return natl::popcount(value.value());
		}

		constexpr inline static Size mask_popcount_i8(SimdMaskI8<Arch> value) noexcept { return maskPopcount<SimdMaskI8<Arch>>(value); }
		constexpr inline static Size mask_popcount_i16(SimdMaskI16<Arch> value) noexcept { return maskPopcount<SimdMaskI16<Arch>>(value); }
		constexpr inline static Size mask_popcount_i32(SimdMaskI32<Arch> value) noexcept { return maskPopcount<SimdMaskI32<Arch>>(value); }
		constexpr inline static Size mask_popcount_i64(SimdMaskI64<Arch> value) noexcept { return maskPopcount<SimdMaskI64<Arch>>(value); }
		constexpr inline static Size mask_popcount_ui8(SimdMaskUI8<Arch> value) noexcept { return maskPopcount<SimdMaskUI8<Arch>>(value); }
		constexpr inline static Size mask_popcount_ui16(SimdMaskUI16<Arch> value) noexcept { return maskPopcount<SimdMaskUI16<Arch>>(value); }
		constexpr inline static Size mask_popcount_ui32(SimdMaskUI32<Arch> value) noexcept { return maskPopcount<SimdMaskUI32<Arch>>(value); }
		constexpr inline static Size mask_popcount_ui64(SimdMaskUI64<Arch> value) noexcept { return maskPopcount<SimdMaskUI64<Arch>>(value); }
		constexpr inline static Size mask_popcount_f32(SimdMaskF32<Arch> value) noexcept { return maskPopcount<SimdMaskF32<Arch>>(value); }
		constexpr inline static Size mask_popcount_f64(SimdMaskF64<Arch> value) noexcept { return maskPopcount<SimdMaskF64<Arch>>(value); }


		//bitscan forward 
		template<typename SimdMaskType>
		constexpr inline static Size maskBitscanForward(SimdMaskType value, Size& indexDst) noexcept {
			return natl::bitscanForward(value.value(), indexDst);
		}

		constexpr inline static Bool mask_bitscan_forward_i8(SimdMaskI8<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskI8<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_i16(SimdMaskI16<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskI16<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_i32(SimdMaskI32<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskI32<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_i64(SimdMaskI64<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskI64<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_ui8(SimdMaskUI8<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskUI8<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_ui16(SimdMaskUI16<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskUI16<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_ui32(SimdMaskUI32<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskUI32<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_ui64(SimdMaskUI64<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskUI64<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_f32(SimdMaskF32<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskF32<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_forward_f64(SimdMaskF64<Arch> value, Size& indexDst) noexcept { return maskBitscanForward<SimdMaskF64<Arch>>(value, indexDst); }

		//bitscan backward 
		template<typename SimdMaskType>
		constexpr inline static Size maskBitscanBackward(SimdMaskType value, Size& indexDst) noexcept {
			return natl::bitscanBackward(value.value(), indexDst);
		}

		constexpr inline static Bool mask_bitscan_backward_i8(SimdMaskI8<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskI8<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_i16(SimdMaskI16<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskI16<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_i32(SimdMaskI32<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskI32<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_i64(SimdMaskI64<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskI64<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_ui8(SimdMaskUI8<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskUI8<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_ui16(SimdMaskUI16<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskUI16<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_ui32(SimdMaskUI32<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskUI32<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_ui64(SimdMaskUI64<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskUI64<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_f32(SimdMaskF32<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskF32<Arch>>(value, indexDst); }
		constexpr inline static Bool mask_bitscan_backward_f64(SimdMaskF64<Arch> value, Size& indexDst) noexcept { return maskBitscanBackward<SimdMaskF64<Arch>>(value, indexDst); }

		//mask get
		template<typename SimdMaskType>
		constexpr inline static ui64 maskGet(SimdMaskType mask) noexcept {
			return mask.value();
		}

		constexpr inline static ui64 mask_get_i8(SimdMaskI8<Arch> mask) noexcept { return maskGet<SimdMaskI8<Arch>>(mask); }
		constexpr inline static ui64 mask_get_i16(SimdMaskI16<Arch> mask) noexcept { return maskGet<SimdMaskI16<Arch>>(mask); }
		constexpr inline static ui64 mask_get_i32(SimdMaskI32<Arch> mask) noexcept { return maskGet<SimdMaskI32<Arch>>(mask); }
		constexpr inline static ui64 mask_get_i64(SimdMaskI64<Arch> mask) noexcept { return maskGet<SimdMaskI64<Arch>>(mask); }
		constexpr inline static ui64 mask_get_ui8(SimdMaskUI8<Arch> mask) noexcept { return maskGet<SimdMaskUI8<Arch>>(mask); }
		constexpr inline static ui64 mask_get_ui16(SimdMaskUI16<Arch> mask) noexcept { return maskGet<SimdMaskUI16<Arch>>(mask); }
		constexpr inline static ui64 mask_get_ui32(SimdMaskUI32<Arch> mask) noexcept { return maskGet<SimdMaskUI32<Arch>>(mask); }
		constexpr inline static ui64 mask_get_ui64(SimdMaskUI64<Arch> mask) noexcept { return maskGet<SimdMaskUI64<Arch>>(mask); }
		constexpr inline static ui64 mask_get_f32(SimdMaskF32<Arch> mask) noexcept { return maskGet<SimdMaskF32<Arch>>(mask); }
		constexpr inline static ui64 mask_get_f64(SimdMaskF64<Arch> mask) noexcept { return maskGet<SimdMaskF64<Arch>>(mask); }

		//mask set
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskSet(const ui64 value) noexcept {
			return SimdMaskType(value);
		}

		constexpr inline static SimdMaskI8<Arch> mask_set_i8(const ui64 value) noexcept { return maskSet<SimdMaskI8<Arch>>(value); }
		constexpr inline static SimdMaskI16<Arch> mask_set_i16(const ui64 value) noexcept { return maskSet<SimdMaskI16<Arch>>(value); }
		constexpr inline static SimdMaskI32<Arch> mask_set_i32(const ui64 value) noexcept { return maskSet<SimdMaskI32<Arch>>(value); }
		constexpr inline static SimdMaskI64<Arch> mask_set_i64(const ui64 value) noexcept { return maskSet<SimdMaskI64<Arch>>(value); }
		constexpr inline static SimdMaskUI8<Arch> mask_set_ui8(const ui64 value) noexcept { return maskSet<SimdMaskUI8<Arch>>(value); }
		constexpr inline static SimdMaskUI16<Arch> mask_set_ui16(const ui64 value) noexcept { return maskSet<SimdMaskUI16<Arch>>(value); }
		constexpr inline static SimdMaskUI32<Arch> mask_set_ui32(const ui64 value) noexcept { return maskSet<SimdMaskUI32<Arch>>(value); }
		constexpr inline static SimdMaskUI64<Arch> mask_set_ui64(const ui64 value) noexcept { return maskSet<SimdMaskUI64<Arch>>(value); }
		constexpr inline static SimdMaskF32<Arch> mask_set_f32(const ui64 value) noexcept { return maskSet<SimdMaskF32<Arch>>(value); }
		constexpr inline static SimdMaskF64<Arch> mask_set_f64(const ui64 value) noexcept { return maskSet<SimdMaskF64<Arch>>(value); }

		//mask test all inactive
		template<typename SimdMaskType>
		constexpr inline static Bool maskTestAllInactive(SimdMaskType mask) noexcept {
			return mask.value() == 0;
		}

		constexpr inline static Bool mask_test_all_inactive_i8(SimdMaskI8<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskI8<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_i16(SimdMaskI16<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskI16<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_i32(SimdMaskI32<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskI32<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_i64(SimdMaskI64<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskI64<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_ui8(SimdMaskUI8<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskUI8<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_ui16(SimdMaskUI16<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskUI16<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_ui32(SimdMaskUI32<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskUI32<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_ui64(SimdMaskUI64<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskUI64<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_f32(SimdMaskF32<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskF32<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_inactive_f64(SimdMaskF64<Arch> mask) noexcept { return maskTestAllInactive<SimdMaskF64<Arch>>(mask); }

		//mask test all active
		template<typename SimdMaskType>
		constexpr inline static Bool maskTestAllActive(SimdMaskType mask) noexcept {
			return (~mask.value()) == 0;
		}

		constexpr inline static Bool mask_test_all_active_i8(SimdMaskI8<Arch> mask) noexcept { return maskTestAllActive<SimdMaskI8<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_i16(SimdMaskI16<Arch> mask) noexcept { return maskTestAllActive<SimdMaskI16<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_i32(SimdMaskI32<Arch> mask) noexcept { return maskTestAllActive<SimdMaskI32<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_i64(SimdMaskI64<Arch> mask) noexcept { return maskTestAllActive<SimdMaskI64<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_ui8(SimdMaskUI8<Arch> mask) noexcept { return maskTestAllActive<SimdMaskUI8<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_ui16(SimdMaskUI16<Arch> mask) noexcept { return maskTestAllActive<SimdMaskUI16<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_ui32(SimdMaskUI32<Arch> mask) noexcept { return maskTestAllActive<SimdMaskUI32<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_ui64(SimdMaskUI64<Arch> mask) noexcept { return maskTestAllActive<SimdMaskUI64<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_f32(SimdMaskF32<Arch> mask) noexcept { return maskTestAllActive<SimdMaskF32<Arch>>(mask); }
		constexpr inline static Bool mask_test_all_active_f64(SimdMaskF64<Arch> mask) noexcept { return maskTestAllActive<SimdMaskF64<Arch>>(mask); }

		//mask test inactive at
		template<typename SimdMaskType>
		constexpr inline static Bool maskTestInactiveAt(SimdMaskType mask, const ui64 index) noexcept {
			return mask[index] == false;
		}

		constexpr inline static Bool mask_test_inactive_at_i8(SimdMaskI8<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskI8<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_i16(SimdMaskI16<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskI16<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_i32(SimdMaskI32<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskI32<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_i64(SimdMaskI64<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskI64<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_ui8(SimdMaskUI8<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskUI8<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_ui16(SimdMaskUI16<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskUI16<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_ui32(SimdMaskUI32<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskUI32<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_ui64(SimdMaskUI64<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskUI64<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_f32(SimdMaskF32<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskF32<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_inactive_at_f64(SimdMaskF64<Arch> mask, const ui64 index) noexcept { return maskTestInactiveAt<SimdMaskF64<Arch>>(mask, index); }
		
		//mask test active at
		template<typename SimdMaskType>
		constexpr inline static Bool maskTestActiveAt(SimdMaskType mask, const ui64 index) noexcept {
			return mask[index] == true;
		}

		constexpr inline static Bool mask_test_active_at_i8(SimdMaskI8<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskI8<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_i16(SimdMaskI16<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskI16<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_i32(SimdMaskI32<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskI32<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_i64(SimdMaskI64<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskI64<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_ui8(SimdMaskUI8<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskUI8<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_ui16(SimdMaskUI16<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskUI16<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_ui32(SimdMaskUI32<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskUI32<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_ui64(SimdMaskUI64<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskUI64<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_f32(SimdMaskF32<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskF32<Arch>>(mask, index); }
		constexpr inline static Bool mask_test_active_at_f64(SimdMaskF64<Arch> mask, const ui64 index) noexcept { return maskTestActiveAt<SimdMaskF64<Arch>>(mask, index); }

		//mask set all inactive
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskSetAllInactive() noexcept { 
			SimdMaskType output{};
			output.setAllInactive();
			return output;
		}

		constexpr inline static SimdMaskI8<Arch> mask_set_all_inactive_i8() noexcept { return maskSetAllInactive<SimdMaskI8<Arch>>(); }
		constexpr inline static SimdMaskI16<Arch> mask_set_all_inactive_i16() noexcept { return maskSetAllInactive<SimdMaskI16<Arch>>(); }
		constexpr inline static SimdMaskI32<Arch> mask_set_all_inactive_i32() noexcept { return maskSetAllInactive<SimdMaskI32<Arch>>(); }
		constexpr inline static SimdMaskI64<Arch> mask_set_all_inactive_i64() noexcept { return maskSetAllInactive<SimdMaskI64<Arch>>(); }
		constexpr inline static SimdMaskUI8<Arch> mask_set_all_inactive_ui8() noexcept { return maskSetAllInactive<SimdMaskUI8<Arch>>(); }
		constexpr inline static SimdMaskUI16<Arch> mask_set_all_inactive_ui16() noexcept { return maskSetAllInactive<SimdMaskUI16<Arch>>(); }
		constexpr inline static SimdMaskUI32<Arch> mask_set_all_inactive_ui32() noexcept { return maskSetAllInactive<SimdMaskUI32<Arch>>(); }
		constexpr inline static SimdMaskUI64<Arch> mask_set_all_inactive_ui64() noexcept { return maskSetAllInactive<SimdMaskUI64<Arch>>(); }
		constexpr inline static SimdMaskF32<Arch> mask_set_all_inactive_f32() noexcept { return maskSetAllInactive<SimdMaskF32<Arch>>(); }
		constexpr inline static SimdMaskF64<Arch> mask_set_all_inactive_f64() noexcept { return maskSetAllInactive<SimdMaskF64<Arch>>(); }

		//mask set all active
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskSetAllActive() noexcept { 
			SimdMaskType output{};
			output.setAllActive();
			return output;;
		}

		constexpr inline static SimdMaskI8<Arch> mask_set_all_active_i8() noexcept { return maskSetAllActive<SimdMaskI8<Arch>>(); }
		constexpr inline static SimdMaskI16<Arch> mask_set_all_active_i16() noexcept { return maskSetAllActive<SimdMaskI16<Arch>>(); }
		constexpr inline static SimdMaskI32<Arch> mask_set_all_active_i32() noexcept { return maskSetAllActive<SimdMaskI32<Arch>>(); }
		constexpr inline static SimdMaskI64<Arch> mask_set_all_active_i64() noexcept { return maskSetAllActive<SimdMaskI64<Arch>>(); }
		constexpr inline static SimdMaskUI8<Arch> mask_set_all_active_ui8() noexcept { return maskSetAllActive<SimdMaskUI8<Arch>>(); }
		constexpr inline static SimdMaskUI16<Arch> mask_set_all_active_ui16() noexcept { return maskSetAllActive<SimdMaskUI16<Arch>>(); }
		constexpr inline static SimdMaskUI32<Arch> mask_set_all_active_ui32() noexcept { return maskSetAllActive<SimdMaskUI32<Arch>>(); }
		constexpr inline static SimdMaskUI64<Arch> mask_set_all_active_ui64() noexcept { return maskSetAllActive<SimdMaskUI64<Arch>>(); }
		constexpr inline static SimdMaskF32<Arch> mask_set_all_active_f32() noexcept { return maskSetAllActive<SimdMaskF32<Arch>>(); }
		constexpr inline static SimdMaskF64<Arch> mask_set_all_active_f64() noexcept { return maskSetAllActive<SimdMaskF64<Arch>>(); }

		//mask set inactive at index
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskSetInactiveAt(SimdMaskType src, const ui64 index) noexcept {
			SimdMaskType output = src;
			output.setAtIndex(index, false);
			return output;
		}

		constexpr inline static SimdMaskI8<Arch> mask_set_inactive_at_i8(SimdMaskI8<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskI8<Arch>>(src, index); }
		constexpr inline static SimdMaskI16<Arch> mask_set_inactive_at_i16(SimdMaskI16<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskI16<Arch>>(src, index); }
		constexpr inline static SimdMaskI32<Arch> mask_set_inactive_at_i32(SimdMaskI32<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskI32<Arch>>(src, index); }
		constexpr inline static SimdMaskI64<Arch> mask_set_inactive_at_i64(SimdMaskI64<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskI64<Arch>>(src, index); }
		constexpr inline static SimdMaskUI8<Arch> mask_set_inactive_at_ui8(SimdMaskUI8<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskUI8<Arch>>(src, index); }
		constexpr inline static SimdMaskUI16<Arch> mask_set_inactive_at_ui16(SimdMaskUI16<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskUI16<Arch>>(src, index); }
		constexpr inline static SimdMaskUI32<Arch> mask_set_inactive_at_ui32(SimdMaskUI32<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskUI32<Arch>>(src, index); }
		constexpr inline static SimdMaskUI64<Arch> mask_set_inactive_at_ui64(SimdMaskUI64<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskUI64<Arch>>(src, index); }
		constexpr inline static SimdMaskF32<Arch> mask_set_inactive_at_f32(SimdMaskF32<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskF32<Arch>>(src, index); }
		constexpr inline static SimdMaskF64<Arch> mask_set_inactive_at_f64(SimdMaskF64<Arch> src, const ui64 index) noexcept { return maskSetInactiveAt<SimdMaskF64<Arch>>(src, index); }

		//mask set active at index
		template<typename SimdMaskType>
		constexpr inline static SimdMaskType maskSetActiveAt(SimdMaskType src, const ui64 index) noexcept {
			SimdMaskType output = src;
			output.setAtIndex(index, true);
			return output;
		}

		constexpr inline static SimdMaskI8<Arch> mask_set_active_at_i8(SimdMaskI8<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskI8<Arch>>(src, index); }
		constexpr inline static SimdMaskI16<Arch> mask_set_active_at_i16(SimdMaskI16<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskI16<Arch>>(src, index); }
		constexpr inline static SimdMaskI32<Arch> mask_set_active_at_i32(SimdMaskI32<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskI32<Arch>>(src, index); }
		constexpr inline static SimdMaskI64<Arch> mask_set_active_at_i64(SimdMaskI64<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskI64<Arch>>(src, index); }
		constexpr inline static SimdMaskUI8<Arch> mask_set_active_at_ui8(SimdMaskUI8<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskUI8<Arch>>(src, index); }
		constexpr inline static SimdMaskUI16<Arch> mask_set_active_at_ui16(SimdMaskUI16<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskUI16<Arch>>(src, index); }
		constexpr inline static SimdMaskUI32<Arch> mask_set_active_at_ui32(SimdMaskUI32<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskUI32<Arch>>(src, index); }
		constexpr inline static SimdMaskUI64<Arch> mask_set_active_at_ui64(SimdMaskUI64<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskUI64<Arch>>(src, index); }
		constexpr inline static SimdMaskF32<Arch> mask_set_active_at_f32(SimdMaskF32<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskF32<Arch>>(src, index); }
		constexpr inline static SimdMaskF64<Arch> mask_set_active_at_f64(SimdMaskF64<Arch> src, const ui64 index) noexcept { return maskSetActiveAt<SimdMaskF64<Arch>>(src, index); }
	};

	namespace impl {
		template<typename DataType, typename SimdArchType>
		class BaseSimdCMask {
		private:
			static constexpr inline Size count() { return SimdArchType::registerSize / (sizeof(DataType) * 8); };
			Size maskData;
		public:
			constexpr BaseSimdCMask() noexcept = default;
			explicit constexpr BaseSimdCMask(const Size value) noexcept : maskData(value) {}
			constexpr void set(const Size value) noexcept { maskData = value; }
			constexpr void setAllInactive() noexcept { maskData = Size(1); }
			constexpr void setAllActive() noexcept { maskData = ~Size(1); }
			constexpr void setAtIndex(const Size index, const Bool value) noexcept {
				maskData = setBit<Size>(maskData, index, value);
			}
			constexpr Bool operator[](const Size index) const noexcept { return getBit<Size>(maskData, index); }
			constexpr Size value() const noexcept { return maskData; }
		};
	}

	template<typename SimdArchType>
	class ArchSimdCMask {
	public:
		using SimdCMaskI8 = impl::BaseSimdCMask<i8, SimdArchType>;
		using SimdCMaskI16 = impl::BaseSimdCMask<i16, SimdArchType>;
		using SimdCMaskI32 = impl::BaseSimdCMask<i32, SimdArchType>;
		using SimdCMaskI64 = impl::BaseSimdCMask<i64, SimdArchType>;
		using SimdCMaskUI8 = impl::BaseSimdCMask<ui8, SimdArchType>;
		using SimdCMaskUI16 = impl::BaseSimdCMask<ui16, SimdArchType>;
		using SimdCMaskUI32 = impl::BaseSimdCMask<ui32, SimdArchType>;
		using SimdCMaskUI64 = impl::BaseSimdCMask<ui64, SimdArchType>;
		using SimdCMaskF32 = impl::BaseSimdCMask<f32, SimdArchType>;
		using SimdCMaskF64 = impl::BaseSimdCMask<f64, SimdArchType>;
	};

	namespace impl {
		template<typename DataType, typename SimdArchType>
		class BaseSimdCMaskInfo {
		public:
			static constexpr inline Size count() { return SimdArchType::registerSize / (sizeof(DataType) * 8); };
			static constexpr inline Size bitSize() { return SimdArchType::registerSize; };
			using value_type = DataType;
		};
	}

	template<typename SimdArchType>
	class ArchSimdCMaskInfo {
	public:
		using SimdCMaskI8Info = impl::BaseSimdCMaskInfo<i8, SimdArchType>;
		using SimdCMaskI16Info = impl::BaseSimdCMaskInfo<i16, SimdArchType>;
		using SimdCMaskI32Info = impl::BaseSimdCMaskInfo<i32, SimdArchType>;
		using SimdCMaskI64Info = impl::BaseSimdCMaskInfo<i64, SimdArchType>;
		using SimdCMaskUI8Info = impl::BaseSimdCMaskInfo<ui8, SimdArchType>;
		using SimdCMaskUI16Info = impl::BaseSimdCMaskInfo<ui16, SimdArchType>;
		using SimdCMaskUI32Info = impl::BaseSimdCMaskInfo<ui32, SimdArchType>;
		using SimdCMaskUI64Info = impl::BaseSimdCMaskInfo<ui64, SimdArchType>;
		using SimdCMaskF32Info = impl::BaseSimdCMaskInfo<f32, SimdArchType>;
		using SimdCMaskF64Info = impl::BaseSimdCMaskInfo<f64, SimdArchType>;
	};

	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskI8; })
	using SimdCMaskI8 = typename ArchSimdCMask<Arch>::SimdCMaskI8;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskI16; })
	using SimdCMaskI16 = typename ArchSimdCMask<Arch>::SimdCMaskI16;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskI32; })
	using SimdCMaskI32 = typename ArchSimdCMask<Arch>::SimdCMaskI32;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskI64; })
	using SimdCMaskI64 = typename ArchSimdCMask<Arch>::SimdCMaskI64;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskUI8; })
	using SimdCMaskUI8 = typename ArchSimdCMask<Arch>::SimdCMaskUI8;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskUI16; })
	using SimdCMaskUI16 = typename ArchSimdCMask<Arch>::SimdCMaskUI16;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskUI32; })
	using SimdCMaskUI32 = typename ArchSimdCMask<Arch>::SimdCMaskUI32;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskUI64; })
	using SimdCMaskUI64 = typename ArchSimdCMask<Arch>::SimdCMaskUI64;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskF32; })
	using SimdCMaskF32 = typename ArchSimdCMask<Arch>::SimdCMaskF32;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMask<Arch>::SimdMaskF64; })
	using SimdCMaskF64 = typename ArchSimdCMask<Arch>::SimdCMaskF64;

	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI8Info; })
	using SimdCMaskI8Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskI8Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI16Info; })
	using SimdCMaskI16Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskI16Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI32Info; })
	using SimdCMaskI32Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskI32Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskI64Info; })
	using SimdCMaskI64Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskI64Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI8Info; })
	using SimdCMaskUI8Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskUI8Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI16Info; })
	using SimdCMaskUI16Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskUI16Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI32Info; })
	using SimdCMaskUI32Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskUI32Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskUI64Info; })
	using SimdCMaskUI64Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskUI64Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskF32Info; })
	using SimdCMaskF32Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskF32Info;
	template<class Arch>
		requires(IsSimdArch<Arch>&& requires() { typename ArchSimdMaskInfo<Arch>::SimdMaskF64Info; })
	using SimdCMaskF64Info = typename ArchSimdCMaskInfo<Arch>::SimdCMaskF64Info;
}