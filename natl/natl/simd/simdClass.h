#pragma once 

//own
#include "simdBase.h"
#include "simdOf.h"

//interface 
namespace natl::simd {
	template<typename DataType, typename Arch>
		requires(IsBuiltInNumericC<DataType> && IsSimdArch<Arch>)
	struct SimdMMaskstruct {
	public:
		using simd_arch = Arch;
		using simd_impl = SimdOfType<DataType, Arch>;

		using simd_register = simd_impl::simd_register;
		using simd_register_info = simd_impl::simd_register_info;
		using simd_mmask = simd_impl::simd_mmask;
		using simd_mask_info = simd_impl::simd_mask_info;
		using simd_cmask = simd_impl::simd_cmask;
		using simd_cmask_info = simd_impl::simd_cmask_info;

		template<typename NewDataType>
		using rebind_simd_mmask = SimdMMaskstruct<NewDataType, Arch>;
	private:
		simd_mmask simdMmask;
	public:
		//constructor
		constexpr SimdMMaskstruct() noexcept = default;
		constexpr SimdMMaskstruct(const SimdMMaskstruct&) noexcept = default;
		constexpr SimdMMaskstruct(SimdMMaskstruct&&) noexcept = default;
		explicit constexpr SimdMMaskstruct(simd_mmask mmask) noexcept :
			simdMmask(mmask) {}
		explicit constexpr SimdMMaskstruct(simd_cmask cmask) noexcept : 
			simdMmask(simd_impl::cmask_to_mmask(cmask)) {}
		explicit constexpr SimdMMaskstruct(const Bool value) noexcept {
			if (value) {
				simdMmask = simd_impl::mask_set_all_active();
			} else {
				simdMmask = simd_impl::mask_set_all_inactive();
			}
		}

		//destructor 
		constexpr ~SimdMMaskstruct() noexcept = default;

		//util 
		constexpr SimdMMaskstruct& self() noexcept { return *this; }
		constexpr const SimdMMaskstruct& self() const noexcept { return *this; }

		//assignment 
		constexpr SimdMMaskstruct& operator=(const SimdMMaskstruct&) noexcept = default;
		constexpr SimdMMaskstruct& operator=(SimdMMaskstruct&&) noexcept = default;

		//bitwise operations 
		constexpr SimdMMaskstruct operator<<(const Size count) const noexcept {
			return SimdMMaskstruct(simd_impl::mask_bitwise_shift_left(simdMmask, count));
		}
		constexpr SimdMMaskstruct operator>>(const Size count) const noexcept {
			return SimdMMaskstruct(simd_impl::mask_bitwise_shift_right(simdMmask, count));
		}
		constexpr SimdMMaskstruct operator&(SimdMMaskstruct rhs) const noexcept {
			return SimdMMaskstruct(simd_impl::mask_bitwise_and(simdMmask, rhs.simdMmask));
		}
		constexpr SimdMMaskstruct operator|(SimdMMaskstruct rhs) const noexcept {
			return SimdMMaskstruct(simd_impl::mask_bitwise_or(simdMmask, rhs.simdMmask));
		}
		constexpr SimdMMaskstruct operator^(SimdMMaskstruct rhs) const noexcept {
			return SimdMMaskstruct(simd_impl::mask_bitwise_xor(simdMmask, rhs.simdMmask));
		}

		constexpr SimdMMaskstruct& operator<<=(const Size count) noexcept {
			self() = self() << count;
			return self();
		}
		constexpr SimdMMaskstruct& operator>>=(const Size count) noexcept {
			self() = self() >> count;
			return self();
		}
		constexpr SimdMMaskstruct& operator&=(SimdMMaskstruct rhs) noexcept {
			self() = self() & rhs;
			return self();
		}
		constexpr SimdMMaskstruct& operator|=(SimdMMaskstruct rhs) noexcept {
			self() = self() | rhs;
			return self();
		}
		constexpr SimdMMaskstruct& operator^=(SimdMMaskstruct rhs) noexcept {
			self() = self() ^ rhs;
			return self();
		}

		constexpr SimdMMaskstruct& operator!() noexcept {
			simdMmask = simd_impl::mask_bitwise_not(simdMmask);
			return self();
		}

		constexpr Size popcount() const noexcept{
			return simd_impl::mask_popcount(simdMmask);
		}
		constexpr Bool bitscanForward(Size& indexDst) const noexcept {
			return simd_impl::mask_bitscan_forward(simdMmask, indexDst);
		}
		constexpr Bool bitscanBackward(Size& indexDst) const noexcept {
			return simd_impl::mask_bitscan_backward(simdMmask, indexDst);
		}
		constexpr Size findFirstSet() const noexcept {
			Size index;
			bitscanForward(index);
			return index;
		}
		constexpr Size findLastSet() const noexcept {
			Size index;
			bitscanBackward(index);
			return index;
		}

		//compare
		constexpr Bool operator==(SimdMMaskstruct rhs) const noexcept {
			return simd_impl::mask_compare_equal(simdMmask, rhs.simdMmask);
		}
		constexpr Bool operator!=(SimdMMaskstruct rhs) const noexcept {
			return simd_impl::mask_compare_not_equal(simdMmask, rhs.simdMmask);
		}

		//set 
		constexpr SimdMMaskstruct& setAllInactive() noexcept {
			simdMmask = simd_impl::mask_set_all_inactive(simdMmask);
			return self();
		}
		constexpr SimdMMaskstruct& setAllActive() noexcept {
			simdMmask = simd_impl::mask_set_all_active(simdMmask);
			return self();
		}
		constexpr SimdMMaskstruct& setInactiveAt(const Size index) noexcept {
			simdMmask = simd_impl::mask_set_inactive_at(simdMmask, index);
			return self();
		}
		constexpr SimdMMaskstruct& setActiveAt(const Size index) noexcept {
			simdMmask = simd_impl::mask_set_active_at(simdMmask, index);
			return self();
		}

		//test 
		constexpr Bool testAllInactive() const noexcept {
			return simdMmask = simd_impl::mask_test_all_inactive(simdMmask);
		}
		constexpr Bool testAllActive() const noexcept {
			return simdMmask = simd_impl::mask_test_all_active(simdMmask);
		}
		constexpr Bool testInactiveAt(const Size index) const noexcept {
			return simdMmask = simd_impl::mask_test_inactive_at(simdMmask, index);
		}
		constexpr Bool testActiveAt(const Size index) const noexcept {
			return simdMmask = simd_impl::mask_test_active_at(simdMmask, index);
		}
		explicit constexpr operator Bool() const noexcept {
			return testAllActive();
		}

		//convert 
		constexpr simd_register toRmask() const noexcept { 
			return simd_impl::mmask_to_rmask(simdMmask); 
		}
	};

	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructI8 = SimdMMaskstruct<i8, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructI16 = SimdMMaskstruct<i16, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructI32 = SimdMMaskstruct<i32, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructI64 = SimdMMaskstruct<i64, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructUI8 = SimdMMaskstruct<ui8, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructUI16 = SimdMMaskstruct<ui16, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructUI32 = SimdMMaskstruct<ui32, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructUI64 = SimdMMaskstruct<ui64, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructF32 = SimdMMaskstruct<f32, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdMMaskstructF64 = SimdMMaskstruct<f64, SimdArchType>;

	struct SimdAlignedLoad {};
	template<typename DataType, typename Arch> 
		requires(IsBuiltInNumericC<DataType> && IsSimdArch<Arch>)
	struct SimdRegisterstruct {
	public:
		using value_type = DataType;
		using simd_arch = Arch;

		using simd_impl = SimdOfType<DataType, Arch>;
		using simd_register = simd_impl::simd_register;
		using simd_register_info = simd_impl::simd_register_info;
		using simd_mmask = simd_impl::simd_mmask;
		using simd_mask_info = simd_impl::simd_mask_info;
		using simd_cmask = simd_impl::simd_cmask;
		using simd_cmask_info = simd_impl::simd_cmask_info;

		using simd_mmask_struct = SimdMMaskstruct<DataType, Arch>;

		template<typename NewDataType>
		using rebind_simd_register = SimdRegisterstruct<NewDataType, Arch>;
	private:
		simd_register simdRegister;
	public:
		//constructor 
		constexpr SimdRegisterstruct() noexcept = default;
		constexpr SimdRegisterstruct(const SimdRegisterstruct&) noexcept = default;
		constexpr SimdRegisterstruct(SimdRegisterstruct&&) noexcept = default;
		constexpr SimdRegisterstruct(simd_register simdRegisterIn) noexcept : simdRegister(simdRegisterIn) {};
		constexpr SimdRegisterstruct(const value_type value) noexcept {
			simdRegister = simd_impl::set(value);
		}
		constexpr SimdRegisterstruct(const value_type* ptr) noexcept {
			simdRegister = simd_impl::load(ptr);
		}
		constexpr SimdRegisterstruct(const value_type* ptr, SimdAlignedLoad) noexcept {
			simdRegister = simd_impl::load_aligned(ptr);
		}

		//destructor 
		constexpr ~SimdRegisterstruct() noexcept = default;

		//util 
		constexpr SimdRegisterstruct& self() noexcept { return *this; }
		constexpr const SimdRegisterstruct& self() const noexcept { return *this; }
		constexpr static Size size() noexcept { return simd_register_info::size(); }

		//element access
		constexpr value_type operator[](const Size index) const noexcept {
			return simd_impl::extract(simdRegister, index);
		}
		constexpr value_type get(const Size index) const noexcept {
			return self()[index];
		}
		constexpr SimdRegisterstruct& set(const value_type& value, const Size index) noexcept {
			simdRegister = simd_impl::insert(simdRegister, value, index);
			return self();
		}

		//inc and dec 
		constexpr SimdRegisterstruct& operator++() noexcept requires(IsBuiltInIntegerC<DataType>) {
			simd_register constant1 = simd_impl::set(1);
			simdRegister = simd_impl::add(simdRegister, constant1);
			return self();
		}
		constexpr SimdRegisterstruct operator++(int) noexcept requires(IsBuiltInIntegerC<DataType>) {
			SimdRegisterstruct temp = self();
			--self();
			return self();
		}
		constexpr SimdRegisterstruct& operator--() noexcept requires(IsBuiltInIntegerC<DataType>) {
			simd_register constant1 = simd_impl::set(1);
			simdRegister = simd_impl::sub(simdRegister, constant1);
			return self();
		}
		constexpr SimdRegisterstruct operator--(int) noexcept requires(IsBuiltInIntegerC<DataType>) {
			SimdRegisterstruct temp = self();
			++self();
			return temp;
		}

		//arithmetic
		constexpr SimdRegisterstruct operator+() const noexcept {
			return self();
		}
		constexpr SimdRegisterstruct operator-() const noexcept {
			return SimdRegisterstruct(simd_impl::negate(self()));
		}

		constexpr SimdRegisterstruct operator+(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::add(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator-(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::sub(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator*(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::mul(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator/(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::div(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator%(const SimdRegisterstruct& rhs) const noexcept requires(IsBuiltInIntegerC<DataType>) {
			return SimdRegisterstruct(simd_impl::remainder(simdRegister, rhs.simdRegister));
		}

		constexpr SimdRegisterstruct& operator+=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() + rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator-=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() - rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator*=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() * rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator/=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() / rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator%=(const SimdRegisterstruct& rhs) noexcept requires(IsBuiltInIntegerC<DataType>) {
			self() = self() % rhs;
			return self();
		}

		//bitwise 
		constexpr SimdRegisterstruct operator<<(const Size count) const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_shift_left_c(self().simdRegister, count));
		}
		constexpr SimdRegisterstruct operator>>(const Size count) const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_shift_right_c(self().simdRegister, count));
		}
		constexpr SimdRegisterstruct operator<<(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_shift_left(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator>>(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_shift_right(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator&(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_and(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator|(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_or(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator^(const SimdRegisterstruct& rhs) const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_xor(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterstruct operator~() const noexcept {
			return SimdRegisterstruct(simd_impl::bitwise_not(self().simdRegister));
		}

		constexpr SimdRegisterstruct& operator<<=(const Size count) noexcept {
			self() = self() << count;
			return self();
		}
		constexpr SimdRegisterstruct& operator>>=(const Size count) noexcept {
			self() = self() >> count;
			return self();
		}
		constexpr SimdRegisterstruct& operator<<=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() << rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator>>=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() >> rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator&=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() & rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator|=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() | rhs;
			return self();
		}
		constexpr SimdRegisterstruct& operator^=(const SimdRegisterstruct& rhs) noexcept {
			self() = self() ^ rhs;
			return self();
		}

		//compare
		constexpr simd_mmask_struct operator==(const SimdRegisterstruct& rhs) const noexcept {
			return simd_mmask_struct(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_struct operator!=(const SimdRegisterstruct& rhs) const noexcept {
			return simd_mmask_struct(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_struct operator<(const SimdRegisterstruct& rhs) const noexcept {
			return simd_mmask_struct(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_struct operator<=(const SimdRegisterstruct& rhs) const noexcept {
			return simd_mmask_struct(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_struct operator>(const SimdRegisterstruct& rhs) const noexcept {
			return simd_mmask_struct(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_struct operator>=(const SimdRegisterstruct& rhs) const noexcept {
			return simd_mmask_struct(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}

		//convert 
		constexpr simd_register& getRegister() noexcept { return simdRegister; }
		constexpr const simd_register& getRegister() const noexcept { return simdRegister; }
	};

	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructI8 = SimdRegisterstruct<i8, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructI16 = SimdRegisterstruct<i16, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructI32 = SimdRegisterstruct<i32, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructI64 = SimdRegisterstruct<i64, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructUI8 = SimdRegisterstruct<ui8, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructUI16 = SimdRegisterstruct<ui16, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructUI32 = SimdRegisterstruct<ui32, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructUI64 = SimdRegisterstruct<ui64, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructF32 = SimdRegisterstruct<f32, SimdArchType>;
	template<typename SimdArchType>
		requires(IsSimdArch<SimdArchType>)
	using SimdRegisterstructF64 = SimdRegisterstruct<f64, SimdArchType>;

	template<typename Type>
	struct IsSimdMMaskstructV : FalseType {};
	template<typename DataType, typename Arch>
	struct IsSimdMMaskstructV<SimdMMaskstruct<DataType, Arch>> : FalseType {};
	template<typename Type>
	constexpr inline Bool IsSimdMMaskstruct = IsSimdMMaskstructV<Type>::value;

	template<typename Type>
	struct IsSimdRegisterstructV : FalseType {};
	template<typename DataType, typename Arch>
	struct IsSimdRegisterstructV<SimdRegisterstruct<DataType, Arch>> : FalseType {};
	template<typename Type>
	constexpr inline Bool IsSimdRegisterstruct = IsSimdRegisterstructV<Type>::value;
}