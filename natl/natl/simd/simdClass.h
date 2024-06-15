#pragma once 

#include "simdBase.h"
#include "simdOf.h"

//interface 
namespace natl::simd {
	template<typename DataType, typename Arch>
		requires(IsBuiltInNumericC<DataType>&& IsSimdArch<Arch>)
	class SimdMMaskClass {
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
		using rebind_simd_mmask = SimdMMaskClass<NewDataType, Arch>;
	private:
		simd_mmask simdMmask;
	public:
		//constructor
		constexpr SimdMMaskClass() noexcept = default;
		constexpr SimdMMaskClass(const SimdMMaskClass&) noexcept = default;
		constexpr SimdMMaskClass(SimdMMaskClass&&) noexcept = default;
		explicit constexpr SimdMMaskClass(simd_mmask mmask) noexcept :
			simdMmask(mmask) {}
		explicit constexpr SimdMMaskClass(simd_cmask cmask) noexcept : 
			simdMmask(simd_impl::cmask_to_mmask(cmask)) {}
		explicit constexpr SimdMMaskClass(const Bool value) noexcept {
			if (value) {
				simdMmask = simd_impl::mask_set_all_active();
			} else {
				simdMmask = simd_impl::mask_set_all_inactive();
			}
		}

		//destructor 
		constexpr ~SimdMMaskClass() noexcept = default;

		//util 
		constexpr SimdMMaskClass& self() noexcept { return *this; }
		constexpr const SimdMMaskClass& self() const noexcept { return *this; }

		//assignment 
		constexpr SimdMMaskClass& operator=(const SimdMMaskClass&) noexcept = default;
		constexpr SimdMMaskClass& operator=(SimdMMaskClass&&) noexcept = default;

		//bitwise operations 
		constexpr SimdMMaskClass operator<<(const Size count) const noexcept {
			return SimdMMaskClass(simd_impl::mask_bitwise_shift_left(simdMmask, count));
		}
		constexpr SimdMMaskClass operator>>(const Size count) const noexcept {
			return SimdMMaskClass(simd_impl::mask_bitwise_shift_right(simdMmask, count));
		}
		constexpr SimdMMaskClass operator&(SimdMMaskClass rhs) const noexcept {
			return SimdMMaskClass(simd_impl::mask_bitwise_and(simdMmask, rhs.simdMmask));
		}
		constexpr SimdMMaskClass operator|(SimdMMaskClass rhs) const noexcept {
			return SimdMMaskClass(simd_impl::mask_bitwise_or(simdMmask, rhs.simdMmask));
		}
		constexpr SimdMMaskClass operator^(SimdMMaskClass rhs) const noexcept {
			return SimdMMaskClass(simd_impl::mask_bitwise_xor(simdMmask, rhs.simdMmask));
		}

		constexpr SimdMMaskClass& operator<<=(SimdMMaskClass rhs) noexcept {
			self() = self() << rhs;
			return self();
		}
		constexpr SimdMMaskClass& operator>>=(SimdMMaskClass rhs) noexcept {
			self() = self() >> rhs;
			return self();
		}
		constexpr SimdMMaskClass& operator&=(SimdMMaskClass rhs) noexcept {
			self() = self() & rhs;
			return self();
		}
		constexpr SimdMMaskClass& operator|=(SimdMMaskClass rhs) noexcept {
			self() = self() | rhs;
			return self();
		}
		constexpr SimdMMaskClass& operator^=(SimdMMaskClass rhs) noexcept {
			self() = self() ^ rhs;
			return self();
		}

		constexpr SimdMMaskClass& operator!() noexcept {
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
		constexpr Bool operator==(SimdMMaskClass rhs) const noexcept {
			return simd_impl::mask_compare_equal(simdMmask, rhs.simdMmask);
		}
		constexpr Bool operator!=(SimdMMaskClass rhs) const noexcept {
			return simd_impl::mask_compare_not_equal(simdMmask, rhs.simdMmask);
		}

		//set 
		constexpr SimdMMaskClass& setAllInactive() noexcept {
			simdMmask = simd_impl::mask_set_all_inactive(simdMmask);
			return self();
		}
		constexpr SimdMMaskClass& setAllActive() noexcept {
			simdMmask = simd_impl::mask_set_all_active(simdMmask);
			return self();
		}
		constexpr SimdMMaskClass& setInactiveAt(const Size index) noexcept {
			simdMmask = simd_impl::mask_set_inactive_at(simdMmask, index);
			return self();
		}
		constexpr SimdMMaskClass& setActiveAt(const Size index) noexcept {
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
	
	struct SimdAlignedLoad {};
	template<typename DataType, typename Arch> 
		requires(IsBuiltInNumericC<DataType> && IsSimdArch<Arch>)
	class SimdRegisterClass {
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

		using simd_mmask_class = SimdMMaskClass<DataType, Arch>;

		template<typename NewDataType>
		using rebind_simd_register = SimdRegisterClass<NewDataType, Arch>;
	private:
		simd_register simdRegister;
	public:
		//constructor 
		constexpr SimdRegisterClass() noexcept = default;
		constexpr SimdRegisterClass(const SimdRegisterClass&) noexcept = default;
		constexpr SimdRegisterClass(SimdRegisterClass&&) noexcept = default;
		constexpr SimdRegisterClass(simd_register simdRegisterIn) noexcept : simdRegister(simdRegisterIn) {};
		constexpr SimdRegisterClass(const value_type value) noexcept {
			simdRegister = simd_impl::set(value);
		}
		constexpr SimdRegisterClass(const value_type* ptr) noexcept {
			simdRegister = simd_impl::load(ptr);
		}
		constexpr SimdRegisterClass(const value_type* ptr, SimdAlignedLoad) noexcept {
			simdRegister = simd_impl::load_aligned(ptr);
		}

		//destructor 
		constexpr ~SimdRegisterClass() noexcept = default;

		//util 
		constexpr SimdRegisterClass& self() noexcept { return *this; }
		constexpr const SimdRegisterClass& self() const noexcept { return *this; }
		constexpr static Size size() noexcept { return simd_register_info::size(); }

		//element access
		constexpr value_type operator[](const Size index) const noexcept {
			return simd_impl::extract(simdRegister, index);
		}
		constexpr value_type get(const Size index) const noexcept {
			return self()[index];
		}
		constexpr SimdRegisterClass& set(const value_type& value, const Size index) noexcept {
			simdRegister = simd_impl::insert(simdRegister, value, index);
			return self();
		}

		//inc and dec 
		constexpr SimdRegisterClass& operator++() noexcept requires(IsBuiltInIntegerC<DataType>) {
			simd_register constant1 = simd_impl::set(1);
			simdRegister = simd_impl::add(simdRegister, constant1);
			return self();
		}
		constexpr SimdRegisterClass operator++(int) noexcept requires(IsBuiltInIntegerC<DataType>) {
			SimdRegisterClass temp = self();
			--self();
			return self();
		}
		constexpr SimdRegisterClass& operator--() noexcept requires(IsBuiltInIntegerC<DataType>) {
			simd_register constant1 = simd_impl::set(1);
			simdRegister = simd_impl::sub(simdRegister, constant1);
			return self();
		}
		constexpr SimdRegisterClass operator--(int) noexcept requires(IsBuiltInIntegerC<DataType>) {
			SimdRegisterClass temp = self();
			++self();
			return temp;
		}

		//arithmetic
		constexpr SimdRegisterClass operator+() const noexcept {
			return self();
		}
		constexpr SimdRegisterClass operator-() const noexcept {
			return SimdRegisterClass(simd_impl::negate(self()));
		}

		constexpr SimdRegisterClass operator+(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::add(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator-(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::sub(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator*(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::mul(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator/(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::div(simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator%(const SimdRegisterClass& rhs) const noexcept requires(IsBuiltInIntegerC<DataType>) {
			return SimdRegisterClass(simd_impl::remainder(simdRegister, rhs.simdRegister));
		}

		constexpr SimdRegisterClass& operator+=(const SimdRegisterClass& rhs) noexcept {
			self() = self() + rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator-=(const SimdRegisterClass& rhs) noexcept {
			self() = self() - rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator*=(const SimdRegisterClass& rhs) noexcept {
			self() = self() * rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator/=(const SimdRegisterClass& rhs) noexcept {
			self() = self() / rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator%=(const SimdRegisterClass& rhs) noexcept requires(IsBuiltInIntegerC<DataType>) {
			self() = self() % rhs;
			return self();
		}

		//bitwise 
		constexpr SimdRegisterClass operator<<(const Size count) const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_shift_left_c(self().simdRegister, count));
		}
		constexpr SimdRegisterClass operator>>(const Size count) const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_shift_right_c(self().simdRegister, count));
		}
		constexpr SimdRegisterClass operator<<(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_shift_left(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator>>(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_shift_right(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator&(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_and(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator|(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_or(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator^(const SimdRegisterClass& rhs) const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_xor(self().simdRegister, rhs.simdRegister));
		}
		constexpr SimdRegisterClass operator~() const noexcept {
			return SimdRegisterClass(simd_impl::bitwise_not(self().simdRegister));
		}

		constexpr SimdRegisterClass& operator<<=(const Size count) noexcept {
			self() = self() << count;
			return self();
		}
		constexpr SimdRegisterClass& operator>>=(const Size count) noexcept {
			self() = self() >> count;
			return self();
		}
		constexpr SimdRegisterClass& operator<<=(const SimdRegisterClass& rhs) noexcept {
			self() = self() << rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator>>=(const SimdRegisterClass& rhs) noexcept {
			self() = self() >> rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator&=(const SimdRegisterClass& rhs) noexcept {
			self() = self() & rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator|=(const SimdRegisterClass& rhs) noexcept {
			self() = self() | rhs;
			return self();
		}
		constexpr SimdRegisterClass& operator^=(const SimdRegisterClass& rhs) noexcept {
			self() = self() ^ rhs;
			return self();
		}

		//compare
		constexpr simd_mmask_class operator==(const SimdRegisterClass& rhs) const noexcept {
			return simd_mmask_class(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_class operator!=(const SimdRegisterClass& rhs) const noexcept {
			return simd_mmask_class(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_class operator<(const SimdRegisterClass& rhs) const noexcept {
			return simd_mmask_class(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_class operator<=(const SimdRegisterClass& rhs) const noexcept {
			return simd_mmask_class(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_class operator>(const SimdRegisterClass& rhs) const noexcept {
			return simd_mmask_class(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
		constexpr simd_mmask_class operator>=(const SimdRegisterClass& rhs) const noexcept {
			return simd_mmask_class(simd_impl::compare_equal(self().simdRegister, rhs.simdRegister));
		}
	};

	template<typename Type>
	class IsSimdMMaskClassV : FalseType {};
	template<typename DataType, typename Arch>
	class IsSimdMMaskClassV<SimdMMaskClass<DataType, Arch>> : FalseType {};
	template<typename Type>
	constexpr inline Bool IsSimdMMaskClass = IsSimdMMaskClassV<Type>::value;

	template<typename Type>
	class IsSimdRegisterClassV : FalseType {};
	template<typename DataType, typename Arch>
	class IsSimdRegisterClassV<SimdRegisterClass<DataType, Arch>> : FalseType {};
	template<typename Type>
	constexpr inline Bool IsSimdRegisterClass = IsSimdRegisterClassV<Type>::value;
}