#pragma once 

//own
#include "simdBase.h"

//interface
namespace natl::simd {
	template<typename DataType, typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType;

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<i8, Arch> {
		public:
		using value_type = i8;

		using simd_register = SimdRegisterI8<Arch>;
		using simd_register_info = SimdRegisterI8Info<Arch>;

		using simd_mmask = SimdMaskI8<Arch>;
		using simd_mask_info = SimdMaskI8Info<Arch>;
		
		using simd_cmask = SimdCMaskI8<Arch>;
		using simd_cmask_info = SimdCMaskI8Info<Arch>;

		using simd_register_index = SimdRegisterUI8<Arch>;
		using simd_register_index_info = SimdRegisterUI8Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_i8;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_i8;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_i8;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_i8;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_i8;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_i8;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_i8;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_i8;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_i8;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_i8;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_i8;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_i8;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_i8;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_i8;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_i8;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_i8;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_i8;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_i8;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_i8;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_i8;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_i8;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_i8;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_i8;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_i8;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_i8;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_i8;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_i8;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_i8;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_i8;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_i8;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_i8;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_i8;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_i8;
		constexpr inline static simd_register(&abs) (simd_register) noexcept = SimdArchOp<Arch>::abs_i8;
		constexpr inline static simd_register(&mmasked_abs) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_abs_i8;
		constexpr inline static simd_register(&mmasked_src_abs) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_abs_i8;
		constexpr inline static simd_register(&negate) (simd_register) noexcept = SimdArchOp<Arch>::negate_i8;
		constexpr inline static simd_register(&mmasked_negate) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_negate_i8;
		constexpr inline static simd_register(&mmasked_src_negate) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_negate_i8;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_i8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_i8;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_i8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_i8;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_i8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_i8;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_i8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_i8;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_i8;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_i8;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_i8;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_i8;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_i8;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_i8;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_i8;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_i8;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_i8;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_i8;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_i8;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_i8;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_i8;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_i8;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_i8;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_i8;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_i8;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_i8;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_i8;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_i8;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_i8;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_i8;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_i8;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_i8;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_i8;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_i8;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_i8;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_i8;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_i8;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_i8;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_i8;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_i8;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_i8;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_i8;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_i8;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_i8;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_i8;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_i8;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_i8;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_i8;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_i8;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_i8;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_i8;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_i8;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_i8;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_i8;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_i8;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_i8;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_i8;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_i8;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_i8;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_i8;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_i8;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_i8;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_i8;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_i8;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_i8;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_i8;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_i8;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_i8;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_i8;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_i8;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_i8;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_i8;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_i8;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_i8;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_i8;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_i8;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_i8;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_i8;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_i8;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_i8;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_i8;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_i8;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_i8;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_i8;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_i8;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_i8;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_i8;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_i8;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_i8;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_i8;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_i8;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_i8;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_i8;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_i8;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_i8;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_i8;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_i8;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i8;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i8;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i8;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i8;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i8;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i8;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i8;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i8;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i8;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i8;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i8;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_i8;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i8;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_i8;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_i8;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_i8;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_i8;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i8;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i8;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i8;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i8;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_i8;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_i8;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_i8;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_i8;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_i8;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_i8_to_any;
		constexpr inline static SimdRegisterUI8<Arch>(&convert_to_uint) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_uint_i8;
		constexpr inline static SimdRegisterUI8<Arch>(&mmasked_convert_to_uint) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_uint_i8;
		constexpr inline static SimdRegisterUI8<Arch>(&mmasked_src_convert_to_uint) (simd_register, SimdRegisterUI8<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_uint_i8;

		//reg i8
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<i16, Arch> {
	public:
		using value_type = i16;

		using simd_register = SimdRegisterI16<Arch>;
		using simd_register_info = SimdRegisterI16Info<Arch>;

		using simd_mmask = SimdMaskI16<Arch>;
		using simd_mask_info = SimdMaskI16Info<Arch>;

		using simd_cmask = SimdCMaskI16<Arch>;
		using simd_cmask_info = SimdCMaskI16Info<Arch>;

		using simd_register_index = SimdRegisterUI16<Arch>;
		using simd_register_index_info = SimdRegisterUI16Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_i16;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_i16;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_i16;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_i16;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_i16;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_i16;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_i16;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_i16;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_i16;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_i16;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_i16;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_i16;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_i16;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_i16;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_i16;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_i16;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_i16;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_i16;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_i16;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_i16;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_i16;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_i16;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_i16;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_i16;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_i16;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_i16;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_i16;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_i16;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_i16;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_i16;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_i16;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_i16;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_i16;
		constexpr inline static simd_register(&abs) (simd_register) noexcept = SimdArchOp<Arch>::abs_i16;
		constexpr inline static simd_register(&mmasked_abs) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_abs_i16;
		constexpr inline static simd_register(&mmasked_src_abs) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_abs_i16;
		constexpr inline static simd_register(&negate) (simd_register) noexcept = SimdArchOp<Arch>::negate_i16;
		constexpr inline static simd_register(&mmasked_negate) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_negate_i16;
		constexpr inline static simd_register(&mmasked_src_negate) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_negate_i16;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_i16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_i16;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_i16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_i16;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_i16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_i16;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_i16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_i16;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_i16;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_i16;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_i16;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_i16;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_i16;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_i16;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_i16;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_i16;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_i16;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_i16;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_i16;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_i16;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_i16;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_i16;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_i16;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_i16;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_i16;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_i16;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_i16;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_i16;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_i16;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_i16;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_i16;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_i16;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_i16;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_i16;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_i16;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_i16;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_i16;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_i16;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_i16;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_i16;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_i16;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_i16;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_i16;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_i16;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_i16;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_i16;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_i16;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_i16;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_i16;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_i16;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_i16;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_i16;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_i16;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_i16;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_i16;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_i16;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_i16;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_i16;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_i16;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_i16;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_i16;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_i16;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_i16;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_i16;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_i16;

		//store/
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_i16;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_i16;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_i16;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_i16;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_i16;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_i16;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_i16;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_i16;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_i16;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_i16;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_i16;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_i16;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_i16;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_i16;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_i16;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_i16;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_i16;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_i16;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_i16;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_i16;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_i16;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_i16;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_i16;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_i16;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_i16;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_i16;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_i16;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_i16;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_i16;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_i16;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_i16;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_i16;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i16;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i16;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i16;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i16;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i16;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i16;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i16;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i16;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i16;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i16;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i16;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_i16;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i16;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_i16;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_i16;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_i16;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_i16;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i16;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i16;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i16;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i16;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_i16;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_i16;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_i16;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_i16;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_i16;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_i16_to_any;
		constexpr inline static SimdRegisterUI16<Arch>(&convert_to_uint) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_uint_i16;
		constexpr inline static SimdRegisterUI16<Arch>(&mmasked_convert_to_uint) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_uint_i16;
		constexpr inline static SimdRegisterUI16<Arch>(&mmasked_src_convert_to_uint) (simd_register, SimdRegisterUI16<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_uint_i16;

		//reg i16
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<i32, Arch> {
	public:
		using value_type = i32;

		using simd_register = SimdRegisterI32<Arch>;
		using simd_register_info = SimdRegisterI32Info<Arch>;

		using simd_mmask = SimdMaskI32<Arch>;
		using simd_mask_info = SimdMaskI32Info<Arch>;

		using simd_cmask = SimdCMaskI32<Arch>;
		using simd_cmask_info = SimdCMaskI32Info<Arch>;

		using simd_register_index = SimdRegisterUI32<Arch>;
		using simd_register_index_info = SimdRegisterUI32Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_i32;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_i32;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_i32;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_i32;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_i32;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_i32;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_i32;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_i32;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_i32;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_i32;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_i32;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_i32;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_i32;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_i32;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_i32;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_i32;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_i32;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_i32;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_i32;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_i32;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_i32;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_i32;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_i32;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_i32;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_i32;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_i32;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_i32;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_i32;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_i32;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_i32;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_i32;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_i32;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_i32;
		constexpr inline static simd_register(&abs) (simd_register) noexcept = SimdArchOp<Arch>::abs_i32;
		constexpr inline static simd_register(&mmasked_abs) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_abs_i32;
		constexpr inline static simd_register(&mmasked_src_abs) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_abs_i32;
		constexpr inline static simd_register(&negate) (simd_register) noexcept = SimdArchOp<Arch>::negate_i32;
		constexpr inline static simd_register(&mmasked_negate) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_negate_i32;
		constexpr inline static simd_register(&mmasked_src_negate) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_negate_i32;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_i32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_i32;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_i32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_i32;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_i32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_i32;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_i32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_i32;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_i32;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_i32;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_i32;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_i32;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_i32;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_i32;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_i32;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_i32;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_i32;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_i32;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_i32;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_i32;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_i32;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_i32;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_i32;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_i32;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_i32;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_i32;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_i32;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_i32;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_i32;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_i32;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_i32;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_i32;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_i32;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_i32;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_i32;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_i32;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_i32;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_i32;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_i32;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_i32;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_i32;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_i32;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_i32;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_i32;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_i32;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_i32;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_i32;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_i32;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_i32;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_i32;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_i32;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_i32;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_i32;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_i32;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_i32;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_i32;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_i32;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_i32;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_i32;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_i32;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_i32;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_i32;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_i32;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_i32;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_i32;

		//store//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_i32;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_i32;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_i32;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_i32;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_i32;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_i32;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_i32;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_i32;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_i32;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_i32;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_i32;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_i32;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_i32;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_i32;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_i32;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_i32;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_i32;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_i32;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_i32;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_i32;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_i32;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_i32;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_i32;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_i32;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_i32;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_i32;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_i32;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_i32;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_i32;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_i32;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_i32;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_i32;
		constexpr inline static simd_register(&gather) (const value_type*, simd_register_index) noexcept = SimdArchOp<Arch>::gather_i32;
		constexpr inline static simd_register(&gather_scaled) (const value_type*, simd_register_index, const Size) noexcept = SimdArchOp<Arch>::gather_scaled_i32;
		constexpr inline static SimdRegisterI32<Arch>(&mmasked_gather) (const value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_gather_i32;
		constexpr inline static SimdRegisterI32<Arch>(&mmasked_gather_scaled) (const value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_gather_scaled_i32;
		constexpr inline static void(&scatter) (value_type*, simd_register_index, simd_register) noexcept = SimdArchOp<Arch>::scatter_i32;
		constexpr inline static void(&scatter_scaled) (value_type*, simd_register_index, simd_register, const Size) noexcept = SimdArchOp<Arch>::scatter_scaled_i32;
		constexpr inline static void(&mmasked_scatter) (value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_scatter_i32;
		constexpr inline static void(&mmasked_scatter_scaled) (value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_scatter_scaled_i32;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i32;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i32;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i32;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i32;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i32;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i32;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i32;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i32;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i32;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i32;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i32;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_i32;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i32;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_i32;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_i32;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_i32;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_i32;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i32;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i32;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i32;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i32;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_i32;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_i32;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_i32;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_i32;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_i32;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_i32_to_any;
		constexpr inline static SimdRegisterUI32<Arch>(&convert_to_uint) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_uint_i32;
		constexpr inline static SimdRegisterUI32<Arch>(&mmasked_convert_to_uint) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_uint_i32;
		constexpr inline static SimdRegisterUI32<Arch>(&mmasked_src_convert_to_uint) (simd_register, SimdRegisterUI32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_uint_i32;
		constexpr inline static SimdRegisterF32<Arch>(&convert_to_float) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_float_i32;
		constexpr inline static SimdRegisterF32<Arch>(&mmasked_convert_to_float) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_float_i32;
		constexpr inline static SimdRegisterF32<Arch>(&mmasked_src_convert_to_float) (simd_register, SimdRegisterF32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_float_i32;

		//reg i32
	};


	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<i64, Arch> {
	public:
		using value_type = i64;

		using simd_register = SimdRegisterI64<Arch>;
		using simd_register_info = SimdRegisterI64Info<Arch>;

		using simd_mmask = SimdMaskI64<Arch>;
		using simd_mask_info = SimdMaskI64Info<Arch>;

		using simd_cmask = SimdCMaskI64<Arch>;
		using simd_cmask_info = SimdCMaskI64Info<Arch>;

		using simd_register_index = SimdRegisterUI64<Arch>;
		using simd_register_index_info = SimdRegisterUI64Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_i64;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_i64;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_i64;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_i64;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_i64;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_i64;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_i64;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_i64;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_i64;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_i64;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_i64;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_i64;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_i64;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_i64;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_i64;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_i64;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_i64;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_i64;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_i64;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_i64;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_i64;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_i64;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_i64;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_i64;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_i64;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_i64;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_i64;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_i64;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_i64;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_i64;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_i64;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_i64;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_i64;
		constexpr inline static simd_register(&abs) (simd_register) noexcept = SimdArchOp<Arch>::abs_i64;
		constexpr inline static simd_register(&mmasked_abs) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_abs_i64;
		constexpr inline static simd_register(&mmasked_src_abs) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_abs_i64;
		constexpr inline static simd_register(&negate) (simd_register) noexcept = SimdArchOp<Arch>::negate_i64;
		constexpr inline static simd_register(&mmasked_negate) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_negate_i64;
		constexpr inline static simd_register(&mmasked_src_negate) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_negate_i64;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_i64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_i64;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_i64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_i64;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_i64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_i64;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_i64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_i64;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_i64;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_i64;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_i64;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_i64;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_i64;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_i64;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_i64;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_i64;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_i64;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_i64;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_i64;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_i64;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_i64;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_i64;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_i64;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_i64;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_i64;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_i64;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_i64;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_i64;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_i64;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_i64;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_i64;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_i64;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_i64;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_i64;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_i64;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_i64;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_i64;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_i64;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_i64;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_i64;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_i64;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_i64;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_i64;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_i64;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_i64;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_i64;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_i64;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_i64;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_i64;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_i64;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_i64;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_i64;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_i64;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_i64;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_i64;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_i64;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_i64;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_i64;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_i64;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_i64;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_i64;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_i64;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_i64;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_i64;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_i64;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_i64;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_i64;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_i64;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_i64;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_i64;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_i64;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_i64;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_i64;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_i64;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_i64;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_i64;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_i64;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_i64;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_i64;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_i64;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_i64;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_i64;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_i64;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_i64;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_i64;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_i64;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_i64;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_i64;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_i64;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_i64;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_i64;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_i64;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_i64;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_i64;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_i64;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_i64;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_i64;
		constexpr inline static simd_register(&gather) (const value_type*, simd_register_index) noexcept = SimdArchOp<Arch>::gather_i64;
		constexpr inline static simd_register(&gather_scaled) (const value_type*, simd_register_index, const Size) noexcept = SimdArchOp<Arch>::gather_scaled_i64;
		constexpr inline static SimdRegisterI64<Arch>(&mmasked_gather) (const value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_gather_i64;
		constexpr inline static SimdRegisterI64<Arch>(&mmasked_gather_scaled) (const value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_gather_scaled_i64;
		constexpr inline static void(&scatter) (value_type*, simd_register_index, simd_register) noexcept = SimdArchOp<Arch>::scatter_i64;
		constexpr inline static void(&scatter_scaled) (value_type*, simd_register_index, simd_register, const Size) noexcept = SimdArchOp<Arch>::scatter_scaled_i64;
		constexpr inline static void(&mmasked_scatter) (value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_scatter_i64;
		constexpr inline static void(&mmasked_scatter_scaled) (value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_scatter_scaled_i64;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_i64;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_i64;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_i64;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_i64;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_i64;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_i64;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_i64;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_i64;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_i64;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_i64;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_i64;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_i64;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_i64;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_i64;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_i64;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_i64;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_i64;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_i64;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_i64;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_i64;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_i64;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_i64;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_i64;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_i64;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_i64;

		//return//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_i64;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_i64_to_any;
		constexpr inline static SimdRegisterUI64<Arch>(&convert_to_uint) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_uint_i64;
		constexpr inline static SimdRegisterUI64<Arch>(&mmasked_convert_to_uint) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_uint_i64;
		constexpr inline static SimdRegisterUI64<Arch>(&mmasked_src_convert_to_uint) (simd_register, SimdRegisterUI64<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_uint_i64;
		constexpr inline static SimdRegisterF64<Arch>(&convert_to_float) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_float_i64;
		constexpr inline static SimdRegisterF64<Arch>(&mmasked_convert_to_float) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_float_i64;
		constexpr inline static SimdRegisterF64<Arch>(&mmasked_src_convert_to_float) (simd_register, SimdRegisterF64<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_float_i64;

		//reg i64
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<ui8, Arch> {
	public:
		using value_type = ui8;

		using simd_register = SimdRegisterUI8<Arch>;
		using simd_register_info = SimdRegisterUI8Info<Arch>;

		using simd_mmask = SimdMaskUI8<Arch>;
		using simd_mask_info = SimdMaskUI8Info<Arch>;

		using simd_cmask = SimdCMaskUI8<Arch>;
		using simd_cmask_info = SimdCMaskUI8Info<Arch>;

		using simd_register_index = SimdRegisterUI8<Arch>;
		using simd_register_index_info = SimdRegisterUI8Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_ui8;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_ui8;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_ui8;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_ui8;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_ui8;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_ui8;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_ui8;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_ui8;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_ui8;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_ui8;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_ui8;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_ui8;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_ui8;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_ui8;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_ui8;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_ui8;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_ui8;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_ui8;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_ui8;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_ui8;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_ui8;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_ui8;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_ui8;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_ui8;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_ui8;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_ui8;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_ui8;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_ui8;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_ui8;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_ui8;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_ui8;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_ui8;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_ui8;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_ui8;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_ui8;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_ui8;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_ui8;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_ui8;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_ui8;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_ui8;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_ui8;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_ui8;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_ui8;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_ui8;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_ui8;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_ui8;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_ui8;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_ui8;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_ui8;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_ui8;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_ui8;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_ui8;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_ui8;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_ui8;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_ui8;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_ui8;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_ui8;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_ui8;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_ui8;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_ui8;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_ui8;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_ui8;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_ui8;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_ui8;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_ui8;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_ui8;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_ui8;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_ui8;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_ui8;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_ui8;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_ui8;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_ui8;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_ui8;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_ui8;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_ui8;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_ui8;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_ui8;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_ui8;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_ui8;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_ui8;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_ui8;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_ui8;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_ui8;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_ui8;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_ui8;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_ui8;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_ui8;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_ui8;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_ui8;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_ui8;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_ui8;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_ui8;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_ui8;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_ui8;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_ui8;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_ui8;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_ui8;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_ui8;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_ui8;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_ui8;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_ui8;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_ui8;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_ui8;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_ui8;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_ui8;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_ui8;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_ui8;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_ui8;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_ui8;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_ui8;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_ui8;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_ui8;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_ui8;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_ui8;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_ui8;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_ui8;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui8;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui8;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui8;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui8;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui8;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui8;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui8;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui8;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui8;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui8;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui8;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui8;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui8;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_ui8;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_ui8;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_ui8;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_ui8;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui8;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui8;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui8;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui8;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_ui8;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_ui8;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_ui8;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_ui8;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_ui8;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_ui8_to_any;
		constexpr inline static SimdRegisterI8<Arch>(&convert_to_int) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_int_ui8;
		constexpr inline static SimdRegisterI8<Arch>(&mmasked_convert_to_int) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_int_ui8;
		constexpr inline static SimdRegisterI8<Arch>(&mmasked_src_convert_to_int) (simd_register, SimdRegisterI8<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_int_ui8;

		//reg ui8
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<ui16, Arch> {
	public:
		using value_type = ui16;

		using simd_register = SimdRegisterUI16<Arch>;
		using simd_register_info = SimdRegisterUI16Info<Arch>;

		using simd_mmask = SimdMaskUI16<Arch>;
		using simd_mask_info = SimdMaskUI16Info<Arch>;

		using simd_cmask = SimdCMaskUI16<Arch>;
		using simd_cmask_info = SimdCMaskUI16Info<Arch>;

		using simd_register_index = SimdRegisterUI16<Arch>;
		using simd_register_index_info = SimdRegisterUI16Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_ui16;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_ui16;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_ui16;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_ui16;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_ui16;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_ui16;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_ui16;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_ui16;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_ui16;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_ui16;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_ui16;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_ui16;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_ui16;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_ui16;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_ui16;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_ui16;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_ui16;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_ui16;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_ui16;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_ui16;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_ui16;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_ui16;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_ui16;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_ui16;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_ui16;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_ui16;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_ui16;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_ui16;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_ui16;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_ui16;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_ui16;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_ui16;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_ui16;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_ui16;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_ui16;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_ui16;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_ui16;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_ui16;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_ui16;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_ui16;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_ui16;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_ui16;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_ui16;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_ui16;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_ui16;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_ui16;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_ui16;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_ui16;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_ui16;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_ui16;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_ui16;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_ui16;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_ui16;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_ui16;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_ui16;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_ui16;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_ui16;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_ui16;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_ui16;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_ui16;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_ui16;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_ui16;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_ui16;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_ui16;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_ui16;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_ui16;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_ui16;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_ui16;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_ui16;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_ui16;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_ui16;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_ui16;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_ui16;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_ui16;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_ui16;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_ui16;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_ui16;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_ui16;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_ui16;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_ui16;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_ui16;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_ui16;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_ui16;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_ui16;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_ui16;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_ui16;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_ui16;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_ui16;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_ui16;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_ui16;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_ui16;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_ui16;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_ui16;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_ui16;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_ui16;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_ui16;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_ui16;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_ui16;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_ui16;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_ui16;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_ui16;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_ui16;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_ui16;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_ui16;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_ui16;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_ui16;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_ui16;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_ui16;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_ui16;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_ui16;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_ui16;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_ui16;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_ui16;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_ui16;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_ui16;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_ui16;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui16;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui16;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui16;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui16;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui16;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui16;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui16;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui16;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui16;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui16;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui16;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui16;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui16;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_ui16;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_ui16;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_ui16;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_ui16;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui16;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui16;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui16;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui16;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_ui16;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_ui16;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_ui16;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_ui16;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_ui16;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_ui16_to_any;
		constexpr inline static SimdRegisterI16<Arch>(&convert_to_int) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_int_ui16;
		constexpr inline static SimdRegisterI16<Arch>(&mmasked_convert_to_int) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_int_ui16;
		constexpr inline static SimdRegisterI16<Arch>(&mmasked_src_convert_to_int) (simd_register, SimdRegisterI16<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_int_ui16;

		//reg ui16
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<ui32, Arch> {
	public:
		using value_type = ui32;

		using simd_register = SimdRegisterUI32<Arch>;
		using simd_register_info = SimdRegisterUI32Info<Arch>;

		using simd_mmask = SimdMaskUI32<Arch>;
		using simd_mask_info = SimdMaskUI32Info<Arch>;

		using simd_cmask = SimdCMaskUI32<Arch>;
		using simd_cmask_info = SimdCMaskUI32Info<Arch>;

		using simd_register_index = SimdRegisterUI32<Arch>;
		using simd_register_index_info = SimdRegisterUI32Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_ui32;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_ui32;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_ui32;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_ui32;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_ui32;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_ui32;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_ui32;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_ui32;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_ui32;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_ui32;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_ui32;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_ui32;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_ui32;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_ui32;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_ui32;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_ui32;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_ui32;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_ui32;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_ui32;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_ui32;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_ui32;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_ui32;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_ui32;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_ui32;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_ui32;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_ui32;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_ui32;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_ui32;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_ui32;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_ui32;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_ui32;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_ui32;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_ui32;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_ui32;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_ui32;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_ui32;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_ui32;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_ui32;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_ui32;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_ui32;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_ui32;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_ui32;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_ui32;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_ui32;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_ui32;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_ui32;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_ui32;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_ui32;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_ui32;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_ui32;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_ui32;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_ui32;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_ui32;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_ui32;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_ui32;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_ui32;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_ui32;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_ui32;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_ui32;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_ui32;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_ui32;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_ui32;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_ui32;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_ui32;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_ui32;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_ui32;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_ui32;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_ui32;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_ui32;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_ui32;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_ui32;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_ui32;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_ui32;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_ui32;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_ui32;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_ui32;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_ui32;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_ui32;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_ui32;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_ui32;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_ui32;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_ui32;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_ui32;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_ui32;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_ui32;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_ui32;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_ui32;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_ui32;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_ui32;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_ui32;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_ui32;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_ui32;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_ui32;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_ui32;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_ui32;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_ui32;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_ui32;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_ui32;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_ui32;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_ui32;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_ui32;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_ui32;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_ui32;
		constexpr inline static SimdRegisterUI32<Arch>(&pack_ui64_in) (SimdRegisterUI64<Arch>, SimdRegisterUI64<Arch>) noexcept = SimdArchOp<Arch>::pack_ui64_in_ui32;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_ui32;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_ui32;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_ui32;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_ui32;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_ui32;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_ui32;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_ui32;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_ui32;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_ui32;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_ui32;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_ui32;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_ui32;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_ui32;
		constexpr inline static simd_register(&gather) (const value_type*, simd_register_index) noexcept = SimdArchOp<Arch>::gather_ui32;
		constexpr inline static simd_register(&gather_scaled) (const value_type*, simd_register_index, const Size) noexcept = SimdArchOp<Arch>::gather_scaled_ui32;
		constexpr inline static SimdRegisterUI32<Arch>(&mmasked_gather) (const value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_gather_ui32;
		constexpr inline static SimdRegisterUI32<Arch>(&mmasked_gather_scaled) (const value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_gather_scaled_ui32;
		constexpr inline static void(&scatter) (value_type*, simd_register_index, simd_register) noexcept = SimdArchOp<Arch>::scatter_ui32;
		constexpr inline static void(&scatter_scaled) (value_type*, simd_register_index, simd_register, const Size) noexcept = SimdArchOp<Arch>::scatter_scaled_ui32;
		constexpr inline static void(&mmasked_scatter) (value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_scatter_ui32;
		constexpr inline static void(&mmasked_scatter_scaled) (value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_scatter_scaled_ui32;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui32;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui32;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui32;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui32;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui32;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui32;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui32;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui32;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui32;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui32;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui32;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui32;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui32;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_ui32;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_ui32;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_ui32;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_ui32;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui32;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui32;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui32;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui32;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_ui32;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_ui32;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_ui32;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_ui32;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_ui32;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_ui32_to_any;
		constexpr inline static SimdRegisterI32<Arch>(&convert_to_int) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_int_ui32;
		constexpr inline static SimdRegisterI32<Arch>(&mmasked_convert_to_int) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_int_ui32;
		constexpr inline static SimdRegisterI32<Arch>(&mmasked_src_convert_to_int) (simd_register, SimdRegisterI32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_int_ui32;
		constexpr inline static SimdRegisterF32<Arch>(&convert_to_float) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_float_ui32;
		constexpr inline static SimdRegisterF32<Arch>(&mmasked_convert_to_float) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_float_ui32;
		constexpr inline static SimdRegisterF32<Arch>(&mmasked_src_convert_to_float) (simd_register, SimdRegisterF32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_float_ui32;

		//reg ui32
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<ui64, Arch> {
	public:
		using value_type = ui64;

		using simd_register = SimdRegisterUI64<Arch>;
		using simd_register_info = SimdRegisterUI64Info<Arch>;

		using simd_mmask = SimdMaskUI64<Arch>;
		using simd_mask_info = SimdMaskUI64Info<Arch>;

		using simd_cmask = SimdCMaskUI64<Arch>;
		using simd_cmask_info = SimdCMaskUI64Info<Arch>;

		using simd_register_index = SimdRegisterUI64<Arch>;
		using simd_register_index_info = SimdRegisterUI64Info<Arch>;

		using simd_register_bit = simd_register;
		using simd_register_bit_into = simd_register_info;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_ui64;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_ui64;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_ui64;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_ui64;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_ui64;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_ui64;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_ui64;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_ui64;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_ui64;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_ui64;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_ui64;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_ui64;
		constexpr inline static simd_register(&add_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_sat_ui64;
		constexpr inline static simd_register(&mmasked_add_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_sat_ui64;
		constexpr inline static simd_register(&mmasked_src_add_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_sat_ui64;
		constexpr inline static simd_register(&sub_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_sat_ui64;
		constexpr inline static simd_register(&mmasked_sub_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_sat_ui64;
		constexpr inline static simd_register(&mmasked_src_sub_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_sat_ui64;
		constexpr inline static simd_register(&mul_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_sat_ui64;
		constexpr inline static simd_register(&mmasked_mul_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_sat_ui64;
		constexpr inline static simd_register(&mmasked_src_mul_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_sat_ui64;
		constexpr inline static simd_register(&div_sat) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_sat_ui64;
		constexpr inline static simd_register(&mmasked_div_sat) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_sat_ui64;
		constexpr inline static simd_register(&mmasked_src_div_sat) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_sat_ui64;
		constexpr inline static simd_register(&remainder) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::remainder_ui64;
		constexpr inline static simd_register(&mmasked_remainder) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_remainder_ui64;
		constexpr inline static simd_register(&mmasked_src_remainder) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_remainder_ui64;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_ui64;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_ui64;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_ui64;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_ui64;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_ui64;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_ui64;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_ui64;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_ui64;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_left_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_ui64;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_shift_right_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_ui64;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_ui64;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_ui64;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_ui64;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_ui64;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_ui64;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_ui64;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_ui64;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_ui64;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_ui64;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_ui64;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_ui64;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_ui64;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_ui64;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_ui64;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_ui64;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_ui64;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_ui64;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_ui64;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_ui64;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_ui64;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_ui64;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_ui64;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_ui64;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_ui64;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_ui64;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_ui64;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_ui64;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_ui64;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_ui64;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_ui64;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_ui64;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_ui64;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_ui64;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_ui64;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_ui64;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_ui64;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_ui64;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_ui64;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_ui64;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_ui64;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_ui64;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_ui64;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_ui64;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_ui64;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_ui64;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_ui64;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_ui64;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_ui64;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_ui64;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_ui64;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_ui64;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_ui64;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_ui64;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_ui64;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_ui64;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_ui64;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_ui64;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_ui64;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_ui64;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_ui64;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_ui64;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_ui64;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_ui64;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_ui64;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_ui64;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_ui64;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_ui64;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_ui64;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_ui64;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_ui64;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_ui64;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_ui64;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_ui64;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_ui64;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_ui64;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_ui64;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_ui64;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_ui64;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_ui64;
		constexpr inline static simd_register(&gather) (const value_type*, simd_register_index) noexcept = SimdArchOp<Arch>::gather_ui64;
		constexpr inline static simd_register(&gather_scaled) (const value_type*, simd_register_index, const Size) noexcept = SimdArchOp<Arch>::gather_scaled_ui64;
		constexpr inline static SimdRegisterUI64<Arch>(&mmasked_gather) (const value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_gather_ui64;
		constexpr inline static SimdRegisterUI64<Arch>(&mmasked_gather_scaled) (const value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_gather_scaled_ui64;
		constexpr inline static void(&scatter) (value_type*, simd_register_index, simd_register) noexcept = SimdArchOp<Arch>::scatter_ui64;
		constexpr inline static void(&scatter_scaled) (value_type*, simd_register_index, simd_register, const Size) noexcept = SimdArchOp<Arch>::scatter_scaled_ui64;
		constexpr inline static void(&mmasked_scatter) (value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_scatter_ui64;
		constexpr inline static void(&mmasked_scatter_scaled) (value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_scatter_scaled_ui64;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_ui64;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_ui64;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_ui64;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_ui64;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_ui64;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_ui64;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_ui64;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_ui64;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_ui64;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_ui64;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_ui64;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_ui64;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_ui64;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_ui64;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_ui64;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_ui64;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_ui64;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_ui64;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_ui64;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_ui64;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_ui64;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_ui64;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_ui64;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_ui64;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_ui64;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_ui64;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_ui64_to_any;
		constexpr inline static SimdRegisterI64<Arch>(&convert_to_int) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_int_ui64;
		constexpr inline static SimdRegisterI64<Arch>(&mmasked_convert_to_int) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_int_ui64;
		constexpr inline static SimdRegisterF64<Arch>(&convert_to_float) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_float_ui64;
		constexpr inline static SimdRegisterF64<Arch>(&mmasked_convert_to_float) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_float_ui64;
		constexpr inline static SimdRegisterF64<Arch>(&mmasked_src_convert_to_float) (simd_register, SimdRegisterF64<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_float_ui64;


		//reg ui64
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<f32, Arch> {
	public:
		using value_type = f32;

		using simd_register = SimdRegisterF32<Arch>;
		using simd_register_info = SimdRegisterF32Info<Arch>;

		using simd_mmask = SimdMaskF32<Arch>;
		using simd_mask_info = SimdMaskF32Info<Arch>;

		using simd_cmask = SimdCMaskF32<Arch>;
		using simd_cmask_info = SimdCMaskF32Info<Arch>;

		using simd_register_index = SimdRegisterUI32<Arch>;
		using simd_register_index_info = SimdRegisterUI32Info<Arch>;

		using simd_register_bit = SimdRegisterUI32<Arch>;
		using simd_register_bit_into = SimdRegisterUI32Info<Arch>;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_f32;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_f32;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_f32;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_f32;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_f32;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_f32;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_f32;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_f32;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_f32;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_f32;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_f32;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_f32;
		constexpr inline static simd_register(&fused_mul_add) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::fused_mul_add_f32;
		constexpr inline static simd_register(&mmasked_fused_mul_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_fused_mul_add_f32;
		constexpr inline static simd_register(&mmasked_src_fused_mul_add) (simd_register, simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_fused_mul_add_f32;
		constexpr inline static simd_register(&fused_mul_sub) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::fused_mul_sub_f32;
		constexpr inline static simd_register(&mmasked_fused_mul_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_fused_mul_sub_f32;
		constexpr inline static simd_register(&mmasked_src_fused_mul_sub) (simd_register, simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_fused_mul_sub_f32;
		constexpr inline static simd_register(&square_root) (simd_register) noexcept = SimdArchOp<Arch>::square_root_f32;
		constexpr inline static simd_register(&mmasked_square_root) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_square_root_f32;
		constexpr inline static simd_register(&mmasked_src_square_root) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_square_root_f32;
		constexpr inline static simd_register(&reciprocal) (simd_register) noexcept = SimdArchOp<Arch>::reciprocal_f32;
		constexpr inline static simd_register(&mmasked_reciprocal) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reciprocal_f32;
		constexpr inline static simd_register(&mmasked_src_reciprocal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reciprocal_f32;
		constexpr inline static simd_register(&reciprocal_square_root) (simd_register) noexcept = SimdArchOp<Arch>::reciprocal_square_root_f32;
		constexpr inline static simd_register(&mmasked_reciprocal_square_root) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reciprocal_square_root_f32;
		constexpr inline static simd_register(&mmasked_src_reciprocal_square_root) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reciprocal_square_root_f32;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_f32;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_f32;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_f32;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_f32;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_f32;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_f32;
		constexpr inline static simd_register(&abs) (simd_register) noexcept = SimdArchOp<Arch>::abs_f32;
		constexpr inline static simd_register(&mmasked_abs) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_abs_f32;
		constexpr inline static simd_register(&mmasked_src_abs) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_abs_f32;
		constexpr inline static simd_register(&negate) (simd_register) noexcept = SimdArchOp<Arch>::negate_f32;
		constexpr inline static simd_register(&mmasked_negate) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_negate_f32;
		constexpr inline static simd_register(&mmasked_src_negate) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_negate_f32;

		//round//
		constexpr inline static simd_register(&ceil) (simd_register) noexcept = SimdArchOp<Arch>::ceil_f32;
		constexpr inline static simd_register(&mmasked_ceil) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_ceil_f32;
		constexpr inline static simd_register(&mmasked_src_ceil) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_ceil_f32;
		constexpr inline static simd_register(&floor) (simd_register) noexcept = SimdArchOp<Arch>::floor_f32;
		constexpr inline static simd_register(&mmasked_floor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_floor_f32;
		constexpr inline static simd_register(&mmasked_src_floor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_floor_f32;
		constexpr inline static simd_register(&trunc) (simd_register) noexcept = SimdArchOp<Arch>::trunc_f32;
		constexpr inline static simd_register(&mmasked_trunc) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_trunc_f32;
		constexpr inline static simd_register(&mmasked_src_trunc) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_trunc_f32;
		constexpr inline static simd_register(&round) (simd_register) noexcept = SimdArchOp<Arch>::round_f32;
		constexpr inline static simd_register(&mmasked_round) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_round_f32;
		constexpr inline static simd_register(&mmasked_src_round) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_round_f32;
		constexpr inline static simd_register(&basic_sin) (simd_register) noexcept = SimdArchOp<Arch>::basic_sin_f32;
		constexpr inline static simd_register(&mmasked_basic_sin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_sin_f32;
		constexpr inline static simd_register(&mmasked_src_basic_sin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_sin_f32;

		//trig//
		constexpr inline static simd_register(&sin) (simd_register) noexcept = SimdArchOp<Arch>::sin_f32;
		constexpr inline static simd_register(&mmasked_sin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sin_f32;
		constexpr inline static simd_register(&mmasked_src_sin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sin_f32;
		constexpr inline static simd_register(&basic_cos) (simd_register) noexcept = SimdArchOp<Arch>::basic_cos_f32;
		constexpr inline static simd_register(&mmasked_basic_cos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_cos_f32;
		constexpr inline static simd_register(&mmasked_src_basic_cos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_cos_f32;
		constexpr inline static simd_register(&cos) (simd_register) noexcept = SimdArchOp<Arch>::cos_f32;
		constexpr inline static simd_register(&mmasked_cos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_cos_f32;
		constexpr inline static simd_register(&mmasked_src_cos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_cos_f32;
		constexpr inline static simd_register(&basic_tan) (simd_register) noexcept = SimdArchOp<Arch>::basic_tan_f32;
		constexpr inline static simd_register(&mmasked_basic_tan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_tan_f32;
		constexpr inline static simd_register(&mmasked_src_basic_tan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_tan_f32;
		constexpr inline static simd_register(&tan) (simd_register) noexcept = SimdArchOp<Arch>::tan_f32;
		constexpr inline static simd_register(&mmasked_tan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_tan_f32;
		constexpr inline static simd_register(&mmasked_src_tan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_tan_f32;
		constexpr inline static simd_register(&basic_asin) (simd_register) noexcept = SimdArchOp<Arch>::basic_asin_f32;
		constexpr inline static simd_register(&mmasked_basic_asin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_asin_f32;
		constexpr inline static simd_register(&mmasked_src_basic_asin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_asin_f32;
		constexpr inline static simd_register(&asin) (simd_register) noexcept = SimdArchOp<Arch>::asin_f32;
		constexpr inline static simd_register(&mmasked_asin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_asin_f32;
		constexpr inline static simd_register(&mmasked_src_asin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_asin_f32;
		constexpr inline static simd_register(&basic_acos) (simd_register) noexcept = SimdArchOp<Arch>::basic_acos_f32;
		constexpr inline static simd_register(&mmasked_basic_acos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_acos_f32;
		constexpr inline static simd_register(&mmasked_src_basic_acos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_acos_f32;
		constexpr inline static simd_register(&acos) (simd_register) noexcept = SimdArchOp<Arch>::acos_f32;
		constexpr inline static simd_register(&mmasked_acos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_acos_f32;
		constexpr inline static simd_register(&mmasked_src_acos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_acos_f32;
		constexpr inline static simd_register(&basic_atan) (simd_register) noexcept = SimdArchOp<Arch>::basic_atan_f32;
		constexpr inline static simd_register(&mmasked_basic_atan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_atan_f32;
		constexpr inline static simd_register(&mmasked_src_basic_atan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_atan_f32;
		constexpr inline static simd_register(&atan) (simd_register) noexcept = SimdArchOp<Arch>::atan_f32;
		constexpr inline static simd_register(&mmasked_atan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_atan_f32;
		constexpr inline static simd_register(&mmasked_src_atan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_atan_f32;
		constexpr inline static simd_register(&basic_sinh) (simd_register) noexcept = SimdArchOp<Arch>::basic_sinh_f32;
		constexpr inline static simd_register(&mmasked_basic_sinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_sinh_f32;
		constexpr inline static simd_register(&mmasked_src_basic_sinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_sinh_f32;
		constexpr inline static simd_register(&sinh) (simd_register) noexcept = SimdArchOp<Arch>::sinh_f32;
		constexpr inline static simd_register(&mmasked_sinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sinh_f32;
		constexpr inline static simd_register(&mmasked_src_sinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sinh_f32;
		constexpr inline static simd_register(&basic_cosh) (simd_register) noexcept = SimdArchOp<Arch>::basic_cosh_f32;
		constexpr inline static simd_register(&mmasked_basic_cosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_cosh_f32;
		constexpr inline static simd_register(&mmasked_src_basic_cosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_cosh_f32;
		constexpr inline static simd_register(&cosh) (simd_register) noexcept = SimdArchOp<Arch>::cosh_f32;
		constexpr inline static simd_register(&mmasked_cosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_cosh_f32;
		constexpr inline static simd_register(&mmasked_src_cosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_cosh_f32;
		constexpr inline static simd_register(&basic_tanh) (simd_register) noexcept = SimdArchOp<Arch>::basic_tanh_f32;
		constexpr inline static simd_register(&mmasked_basic_tanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_tanh_f32;
		constexpr inline static simd_register(&mmasked_src_basic_tanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_tanh_f32;
		constexpr inline static simd_register(&tanh) (simd_register) noexcept = SimdArchOp<Arch>::tanh_f32;
		constexpr inline static simd_register(&mmasked_tanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_tanh_f32;
		constexpr inline static simd_register(&mmasked_src_tanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_tanh_f32;
		constexpr inline static simd_register(&basic_asinh) (simd_register) noexcept = SimdArchOp<Arch>::basic_asinh_f32;
		constexpr inline static simd_register(&mmasked_basic_asinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_asinh_f32;
		constexpr inline static simd_register(&mmasked_src_basic_asinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_asinh_f32;
		constexpr inline static simd_register(&asinh) (simd_register) noexcept = SimdArchOp<Arch>::asinh_f32;
		constexpr inline static simd_register(&mmasked_asinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_asinh_f32;
		constexpr inline static simd_register(&mmasked_src_asinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_asinh_f32;
		constexpr inline static simd_register(&basic_acosh) (simd_register) noexcept = SimdArchOp<Arch>::basic_acosh_f32;
		constexpr inline static simd_register(&mmasked_basic_acosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_acosh_f32;
		constexpr inline static simd_register(&mmasked_src_basic_acosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_acosh_f32;
		constexpr inline static simd_register(&acosh) (simd_register) noexcept = SimdArchOp<Arch>::acosh_f32;
		constexpr inline static simd_register(&mmasked_acosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_acosh_f32;
		constexpr inline static simd_register(&mmasked_src_acosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_acosh_f32;
		constexpr inline static simd_register(&basic_atanh) (simd_register) noexcept = SimdArchOp<Arch>::basic_atanh_f32;
		constexpr inline static simd_register(&mmasked_basic_atanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_atanh_f32;
		constexpr inline static simd_register(&mmasked_src_basic_atanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_atanh_f32;
		constexpr inline static simd_register(&atanh) (simd_register) noexcept = SimdArchOp<Arch>::atanh_f32;
		constexpr inline static simd_register(&mmasked_atanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_atanh_f32;
		constexpr inline static simd_register(&mmasked_src_atanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_atanh_f32;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_f32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_f32;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_f32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_f32;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, SimdRegisterUI32<Arch>) noexcept = SimdArchOp<Arch>::bitwise_shift_left_f32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, SimdRegisterUI32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, SimdRegisterUI32<Arch>, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_f32;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, SimdRegisterUI32<Arch>) noexcept = SimdArchOp<Arch>::bitwise_shift_right_f32;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, SimdRegisterUI32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, SimdRegisterUI32<Arch>, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_f32;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_f32;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_f32;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_f32;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_f32;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_f32;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_f32;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_f32;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_f32;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_f32;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_f32;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_f32;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_f32;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_f32;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_f32;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_f32;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_f32;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_f32;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_f32;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_f32;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_f32;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_f32;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_f32;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_f32;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_f32;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_f32;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_f32;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_f32;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_f32;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_f32;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_f32;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_f32;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_f32;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_f32;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_f32;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_f32;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_f32;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_f32;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_f32;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_f32;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_f32;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_f32;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_f32;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_f32;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_f32;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_f32;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_f32;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_f32;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_f32;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_f32;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_f32;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_f32;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_f32;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_f32;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_f32;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_f32;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_f32;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_f32;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_f32;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_f32;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_f32;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_f32;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_f32;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_f32;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_f32;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_f32;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_f32;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_f32;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_f32;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_f32;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_f32;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_f32;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_f32;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_f32;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_f32;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_f32;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_f32;
		constexpr inline static simd_register(&pack_f64_in) (SimdRegisterF64<Arch>, SimdRegisterF64<Arch>) noexcept = SimdArchOp<Arch>::pack_f64_in_f32;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_f32;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_f32;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_f32;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_f32;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_f32;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_f32;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_f32;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_f32;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_f32;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_f32;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_f32;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_f32;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_f32;
		constexpr inline static simd_register(&gather) (const value_type*, simd_register_index) noexcept = SimdArchOp<Arch>::gather_f32;
		constexpr inline static simd_register(&gather_scaled) (const value_type*, simd_register_index, const Size) noexcept = SimdArchOp<Arch>::gather_scaled_f32;
		constexpr inline static SimdRegisterF32<Arch>(&mmasked_gather) (const value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_gather_f32;
		constexpr inline static SimdRegisterF32<Arch>(&mmasked_gather_scaled) (const value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_gather_scaled_f32;
		constexpr inline static void(&scatter) (value_type*, simd_register_index, simd_register) noexcept = SimdArchOp<Arch>::scatter_f32;
		constexpr inline static void(&scatter_scaled) (value_type*, simd_register_index, simd_register, const Size) noexcept = SimdArchOp<Arch>::scatter_scaled_f32;
		constexpr inline static void(&mmasked_scatter) (value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_scatter_f32;
		constexpr inline static void(&mmasked_scatter_scaled) (value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_scatter_scaled_f32;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_f32;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_f32;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_f32;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_f32;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_f32;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_f32;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_f32;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_f32;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_f32;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_f32;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_f32;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_f32;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_f32;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_f32;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_f32;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_f32;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_f32;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_f32;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_f32;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_f32;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_f32;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_f32;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_f32;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_f32;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_f32;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_f32;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_f32_to_any;
		constexpr inline static SimdRegisterI32<Arch>(&convert_to_int) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_int_f32;
		constexpr inline static SimdRegisterI32<Arch>(&mmasked_convert_to_int) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_int_f32;
		constexpr inline static SimdRegisterI32<Arch>(&mmasked_src_convert_to_int) (simd_register, SimdRegisterI32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_int_f32;
		constexpr inline static SimdRegisterUI32<Arch>(&convert_to_uint) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_uint_f32;
		constexpr inline static SimdRegisterUI32<Arch>(&mmasked_convert_to_uint) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_uint_f32;
		constexpr inline static SimdRegisterUI32<Arch>(&mmasked_src_convert_to_uint) (simd_register, SimdRegisterUI32<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_uint_f32;

		//reg f32
	};

	template <typename Arch>
		requires(IsSimdArch<Arch>)
	struct SimdOfType<f64, Arch> {
	public:
		using value_type = f64;

		using simd_register = SimdRegisterF64<Arch>;
		using simd_register_info = SimdRegisterF64Info<Arch>;

		using simd_mmask = SimdMaskF64<Arch>;
		using simd_mask_info = SimdMaskF64Info<Arch>;

		using simd_cmask = SimdCMaskF64<Arch>;
		using simd_cmask_info = SimdCMaskF64Info<Arch>;

		using simd_register_index = SimdRegisterUI64<Arch>;
		using simd_register_index_info = SimdRegisterUI64Info<Arch>;

		using simd_register_bit = SimdRegisterUI64<Arch>;
		using simd_register_bit_into = SimdRegisterUI64Info<Arch>;

		//arithmetic//
		constexpr inline static simd_register(&add) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::add_f64;
		constexpr inline static simd_register(&mmasked_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_add_f64;
		constexpr inline static simd_register(&mmasked_src_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_add_f64;
		constexpr inline static simd_register(&sub) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::sub_f64;
		constexpr inline static simd_register(&mmasked_sub) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sub_f64;
		constexpr inline static simd_register(&mmasked_src_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sub_f64;
		constexpr inline static simd_register(&mul) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::mul_f64;
		constexpr inline static simd_register(&mmasked_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_mul_f64;
		constexpr inline static simd_register(&mmasked_src_mul) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_mul_f64;
		constexpr inline static simd_register(&div) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::div_f64;
		constexpr inline static simd_register(&mmasked_div) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_div_f64;
		constexpr inline static simd_register(&mmasked_src_div) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_div_f64;
		constexpr inline static simd_register(&fused_mul_add) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::fused_mul_add_f64;
		constexpr inline static simd_register(&mmasked_fused_mul_add) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_fused_mul_add_f64;
		constexpr inline static simd_register(&mmasked_src_fused_mul_add) (simd_register, simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_fused_mul_add_f64;
		constexpr inline static simd_register(&fused_mul_sub) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::fused_mul_sub_f64;
		constexpr inline static simd_register(&mmasked_fused_mul_sub) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_fused_mul_sub_f64;
		constexpr inline static simd_register(&mmasked_src_fused_mul_sub) (simd_register, simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_fused_mul_sub_f64;
		constexpr inline static simd_register(&square_root) (simd_register) noexcept = SimdArchOp<Arch>::square_root_f64;
		constexpr inline static simd_register(&mmasked_square_root) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_square_root_f64;
		constexpr inline static simd_register(&mmasked_src_square_root) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_square_root_f64;
		constexpr inline static simd_register(&reciprocal) (simd_register) noexcept = SimdArchOp<Arch>::reciprocal_f64;
		constexpr inline static simd_register(&mmasked_reciprocal) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reciprocal_f64;
		constexpr inline static simd_register(&mmasked_src_reciprocal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reciprocal_f64;
		constexpr inline static simd_register(&reciprocal_square_root) (simd_register) noexcept = SimdArchOp<Arch>::reciprocal_square_root_f64;
		constexpr inline static simd_register(&mmasked_reciprocal_square_root) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reciprocal_square_root_f64;
		constexpr inline static simd_register(&mmasked_src_reciprocal_square_root) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reciprocal_square_root_f64;

		//math function//
		constexpr inline static simd_register(&max) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::max_f64;
		constexpr inline static simd_register(&mmasked_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_max_f64;
		constexpr inline static simd_register(&mmasked_src_max) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_max_f64;
		constexpr inline static simd_register(&min) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::min_f64;
		constexpr inline static simd_register(&mmasked_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_min_f64;
		constexpr inline static simd_register(&mmasked_src_min) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_min_f64;
		constexpr inline static simd_register(&abs) (simd_register) noexcept = SimdArchOp<Arch>::abs_f64;
		constexpr inline static simd_register(&mmasked_abs) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_abs_f64;
		constexpr inline static simd_register(&mmasked_src_abs) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_abs_f64;
		constexpr inline static simd_register(&negate) (simd_register) noexcept = SimdArchOp<Arch>::negate_f64;
		constexpr inline static simd_register(&mmasked_negate) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_negate_f64;
		constexpr inline static simd_register(&mmasked_src_negate) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_negate_f64;

		//round//
		constexpr inline static simd_register(&ceil) (simd_register) noexcept = SimdArchOp<Arch>::ceil_f64;
		constexpr inline static simd_register(&mmasked_ceil) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_ceil_f64;
		constexpr inline static simd_register(&mmasked_src_ceil) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_ceil_f64;
		constexpr inline static simd_register(&floor) (simd_register) noexcept = SimdArchOp<Arch>::floor_f64;
		constexpr inline static simd_register(&mmasked_floor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_floor_f64;
		constexpr inline static simd_register(&mmasked_src_floor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_floor_f64;
		constexpr inline static simd_register(&trunc) (simd_register) noexcept = SimdArchOp<Arch>::trunc_f64;
		constexpr inline static simd_register(&mmasked_trunc) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_trunc_f64;
		constexpr inline static simd_register(&mmasked_src_trunc) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_trunc_f64;
		constexpr inline static simd_register(&round) (simd_register) noexcept = SimdArchOp<Arch>::round_f64;
		constexpr inline static simd_register(&mmasked_round) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_round_f64;
		constexpr inline static simd_register(&mmasked_src_round) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_round_f64;
		constexpr inline static simd_register(&basic_sin) (simd_register) noexcept = SimdArchOp<Arch>::basic_sin_f64;
		constexpr inline static simd_register(&mmasked_basic_sin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_sin_f64;
		constexpr inline static simd_register(&mmasked_src_basic_sin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_sin_f64;

		//trig//
		constexpr inline static simd_register(&sin) (simd_register) noexcept = SimdArchOp<Arch>::sin_f64;
		constexpr inline static simd_register(&mmasked_sin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sin_f64;
		constexpr inline static simd_register(&mmasked_src_sin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sin_f64;
		constexpr inline static simd_register(&basic_cos) (simd_register) noexcept = SimdArchOp<Arch>::basic_cos_f64;
		constexpr inline static simd_register(&mmasked_basic_cos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_cos_f64;
		constexpr inline static simd_register(&mmasked_src_basic_cos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_cos_f64;
		constexpr inline static simd_register(&cos) (simd_register) noexcept = SimdArchOp<Arch>::cos_f64;
		constexpr inline static simd_register(&mmasked_cos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_cos_f64;
		constexpr inline static simd_register(&mmasked_src_cos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_cos_f64;
		constexpr inline static simd_register(&basic_tan) (simd_register) noexcept = SimdArchOp<Arch>::basic_tan_f64;
		constexpr inline static simd_register(&mmasked_basic_tan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_tan_f64;
		constexpr inline static simd_register(&mmasked_src_basic_tan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_tan_f64;
		constexpr inline static simd_register(&tan) (simd_register) noexcept = SimdArchOp<Arch>::tan_f64;
		constexpr inline static simd_register(&mmasked_tan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_tan_f64;
		constexpr inline static simd_register(&mmasked_src_tan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_tan_f64;
		constexpr inline static simd_register(&basic_asin) (simd_register) noexcept = SimdArchOp<Arch>::basic_asin_f64;
		constexpr inline static simd_register(&mmasked_basic_asin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_asin_f64;
		constexpr inline static simd_register(&mmasked_src_basic_asin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_asin_f64;
		constexpr inline static simd_register(&asin) (simd_register) noexcept = SimdArchOp<Arch>::asin_f64;
		constexpr inline static simd_register(&mmasked_asin) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_asin_f64;
		constexpr inline static simd_register(&mmasked_src_asin) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_asin_f64;
		constexpr inline static simd_register(&basic_acos) (simd_register) noexcept = SimdArchOp<Arch>::basic_acos_f64;
		constexpr inline static simd_register(&mmasked_basic_acos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_acos_f64;
		constexpr inline static simd_register(&mmasked_src_basic_acos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_acos_f64;
		constexpr inline static simd_register(&acos) (simd_register) noexcept = SimdArchOp<Arch>::acos_f64;
		constexpr inline static simd_register(&mmasked_acos) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_acos_f64;
		constexpr inline static simd_register(&mmasked_src_acos) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_acos_f64;
		constexpr inline static simd_register(&basic_atan) (simd_register) noexcept = SimdArchOp<Arch>::basic_atan_f64;
		constexpr inline static simd_register(&mmasked_basic_atan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_atan_f64;
		constexpr inline static simd_register(&mmasked_src_basic_atan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_atan_f64;
		constexpr inline static simd_register(&atan) (simd_register) noexcept = SimdArchOp<Arch>::atan_f64;
		constexpr inline static simd_register(&mmasked_atan) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_atan_f64;
		constexpr inline static simd_register(&mmasked_src_atan) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_atan_f64;
		constexpr inline static simd_register(&basic_sinh) (simd_register) noexcept = SimdArchOp<Arch>::basic_sinh_f64;
		constexpr inline static simd_register(&mmasked_basic_sinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_sinh_f64;
		constexpr inline static simd_register(&mmasked_src_basic_sinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_sinh_f64;
		constexpr inline static simd_register(&sinh) (simd_register) noexcept = SimdArchOp<Arch>::sinh_f64;
		constexpr inline static simd_register(&mmasked_sinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_sinh_f64;
		constexpr inline static simd_register(&mmasked_src_sinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_sinh_f64;
		constexpr inline static simd_register(&basic_cosh) (simd_register) noexcept = SimdArchOp<Arch>::basic_cosh_f64;
		constexpr inline static simd_register(&mmasked_basic_cosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_cosh_f64;
		constexpr inline static simd_register(&mmasked_src_basic_cosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_cosh_f64;
		constexpr inline static simd_register(&cosh) (simd_register) noexcept = SimdArchOp<Arch>::cosh_f64;
		constexpr inline static simd_register(&mmasked_cosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_cosh_f64;
		constexpr inline static simd_register(&mmasked_src_cosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_cosh_f64;
		constexpr inline static simd_register(&basic_tanh) (simd_register) noexcept = SimdArchOp<Arch>::basic_tanh_f64;
		constexpr inline static simd_register(&mmasked_basic_tanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_tanh_f64;
		constexpr inline static simd_register(&mmasked_src_basic_tanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_tanh_f64;
		constexpr inline static simd_register(&tanh) (simd_register) noexcept = SimdArchOp<Arch>::tanh_f64;
		constexpr inline static simd_register(&mmasked_tanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_tanh_f64;
		constexpr inline static simd_register(&mmasked_src_tanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_tanh_f64;
		constexpr inline static simd_register(&basic_asinh) (simd_register) noexcept = SimdArchOp<Arch>::basic_asinh_f64;
		constexpr inline static simd_register(&mmasked_basic_asinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_asinh_f64;
		constexpr inline static simd_register(&mmasked_src_basic_asinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_asinh_f64;
		constexpr inline static simd_register(&asinh) (simd_register) noexcept = SimdArchOp<Arch>::asinh_f64;
		constexpr inline static simd_register(&mmasked_asinh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_asinh_f64;
		constexpr inline static simd_register(&mmasked_src_asinh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_asinh_f64;
		constexpr inline static simd_register(&basic_acosh) (simd_register) noexcept = SimdArchOp<Arch>::basic_acosh_f64;
		constexpr inline static simd_register(&mmasked_basic_acosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_acosh_f64;
		constexpr inline static simd_register(&mmasked_src_basic_acosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_acosh_f64;
		constexpr inline static simd_register(&acosh) (simd_register) noexcept = SimdArchOp<Arch>::acosh_f64;
		constexpr inline static simd_register(&mmasked_acosh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_acosh_f64;
		constexpr inline static simd_register(&mmasked_src_acosh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_acosh_f64;
		constexpr inline static simd_register(&basic_atanh) (simd_register) noexcept = SimdArchOp<Arch>::basic_atanh_f64;
		constexpr inline static simd_register(&mmasked_basic_atanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_basic_atanh_f64;
		constexpr inline static simd_register(&mmasked_src_basic_atanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_basic_atanh_f64;
		constexpr inline static simd_register(&atanh) (simd_register) noexcept = SimdArchOp<Arch>::atanh_f64;
		constexpr inline static simd_register(&mmasked_atanh) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_atanh_f64;
		constexpr inline static simd_register(&mmasked_src_atanh) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_atanh_f64;

		//bitwise//
		constexpr inline static simd_register(&bitwise_shift_left_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_left_c_f64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_c_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_c_f64;
		constexpr inline static simd_register(&bitwise_shift_right_c) (simd_register, const Size) noexcept = SimdArchOp<Arch>::bitwise_shift_right_c_f64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right_c) (simd_register, const Size, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_c_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right_c) (simd_register, const Size, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_c_f64;
		constexpr inline static simd_register(&bitwise_shift_left) (simd_register, SimdRegisterUI64<Arch>) noexcept = SimdArchOp<Arch>::bitwise_shift_left_f64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_left) (simd_register, SimdRegisterUI64<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_left_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_left) (simd_register, SimdRegisterUI64<Arch>, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_left_f64;
		constexpr inline static simd_register(&bitwise_shift_right) (simd_register, SimdRegisterUI64<Arch>) noexcept = SimdArchOp<Arch>::bitwise_shift_right_f64;
		constexpr inline static simd_register(&mmasked_bitwise_shift_right) (simd_register, SimdRegisterUI64<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_shift_right_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_shift_right) (simd_register, SimdRegisterUI64<Arch>, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_shift_right_f64;
		constexpr inline static simd_register(&bitwise_and) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_and_f64;
		constexpr inline static simd_register(&mmasked_bitwise_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_and_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_and) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_and_f64;
		constexpr inline static simd_register(&bitwise_or) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_or_f64;
		constexpr inline static simd_register(&mmasked_bitwise_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_or_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_or) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_or_f64;
		constexpr inline static simd_register(&bitwise_xor) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::bitwise_xor_f64;
		constexpr inline static simd_register(&mmasked_bitwise_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_xor_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_xor) (simd_register, simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_xor_f64;
		constexpr inline static simd_register(&bitwise_not) (simd_register) noexcept = SimdArchOp<Arch>::bitwise_not_f64;
		constexpr inline static simd_register(&mmasked_bitwise_not) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitwise_not_f64;
		constexpr inline static simd_register(&mmasked_src_bitwise_not) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitwise_not_f64;
		constexpr inline static simd_register_bit(&popcount) (simd_register) noexcept = SimdArchOp<Arch>::popcount_f64;
		constexpr inline static simd_register_bit(&mmasked_popcount) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_popcount_f64;
		constexpr inline static simd_register_bit(&mmasked_src_popcount) (simd_register, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_popcount_f64;
		constexpr inline static simd_mmask(&bitscan_forward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_forward_f64;
		constexpr inline static simd_mmask(&mmasked_bitscan_forward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_forward_f64;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_forward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_forward_f64;
		constexpr inline static simd_mmask(&bitscan_backward) (simd_register, simd_register_bit&) noexcept = SimdArchOp<Arch>::bitscan_backward_f64;
		constexpr inline static simd_mmask(&mmasked_bitscan_backward) (simd_register, simd_register_bit&, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_bitscan_backward_f64;
		constexpr inline static simd_mmask(&mmasked_src_bitscan_backward) (simd_register, simd_register_bit&, simd_register_bit, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_bitscan_backward_f64;

		//reduce//
		constexpr inline static value_type(&reduce_min) (simd_register) noexcept = SimdArchOp<Arch>::reduce_min_f64;
		constexpr inline static value_type(&mmasked_reduce_min) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_min_f64;
		constexpr inline static value_type(&mmasked_src_reduce_min) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_min_f64;
		constexpr inline static value_type(&reduce_max) (simd_register) noexcept = SimdArchOp<Arch>::reduce_max_f64;
		constexpr inline static value_type(&mmasked_reduce_max) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_max_f64;
		constexpr inline static value_type(&mmasked_src_reduce_max) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_max_f64;
		constexpr inline static value_type(&reduce_add) (simd_register) noexcept = SimdArchOp<Arch>::reduce_add_f64;
		constexpr inline static value_type(&mmasked_reduce_add) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_add_f64;
		constexpr inline static value_type(&mmasked_src_reduce_add) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_add_f64;
		constexpr inline static value_type(&reduce_mul) (simd_register) noexcept = SimdArchOp<Arch>::reduce_mul_f64;
		constexpr inline static value_type(&mmasked_reduce_mul) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_mul_f64;
		constexpr inline static value_type(&mmasked_src_reduce_mul) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_mul_f64;
		constexpr inline static value_type(&reduce_and) (simd_register) noexcept = SimdArchOp<Arch>::reduce_and_f64;
		constexpr inline static value_type(&mmasked_reduce_and) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_and_f64;
		constexpr inline static value_type(&mmasked_src_reduce_and) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_and_f64;
		constexpr inline static value_type(&reduce_or) (simd_register) noexcept = SimdArchOp<Arch>::reduce_or_f64;
		constexpr inline static value_type(&mmasked_reduce_or) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_or_f64;
		constexpr inline static value_type(&mmasked_src_reduce_or) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_or_f64;
		constexpr inline static value_type(&reduce_xor) (simd_register) noexcept = SimdArchOp<Arch>::reduce_xor_f64;
		constexpr inline static value_type(&mmasked_reduce_xor) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_reduce_xor_f64;
		constexpr inline static value_type(&mmasked_src_reduce_xor) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_reduce_xor_f64;

		//compare//
		constexpr inline static simd_mmask(&compare_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_equal_f64;
		constexpr inline static simd_mmask(&mmasked_compare_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_equal_f64;
		constexpr inline static simd_mmask(&mmasked_src_compare_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_equal_f64;
		constexpr inline static simd_mmask(&compare_not_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_not_equal_f64;
		constexpr inline static simd_mmask(&mmasked_compare_not_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_not_equal_f64;
		constexpr inline static simd_mmask(&mmasked_src_compare_not_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_not_equal_f64;
		constexpr inline static simd_mmask(&compare_less_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_f64;
		constexpr inline static simd_mmask(&mmasked_compare_less_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_f64;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_f64;
		constexpr inline static simd_mmask(&compare_greater_than) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_f64;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_f64;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_f64;
		constexpr inline static simd_mmask(&compare_less_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_less_than_or_equal_f64;
		constexpr inline static simd_mmask(&mmasked_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_less_than_or_equal_f64;
		constexpr inline static simd_mmask(&mmasked_src_compare_less_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_less_than_or_equal_f64;
		constexpr inline static simd_mmask(&compare_greater_than_or_equal) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::compare_greater_than_or_equal_f64;
		constexpr inline static simd_mmask(&mmasked_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_compare_greater_than_or_equal_f64;
		constexpr inline static simd_mmask(&mmasked_src_compare_greater_than_or_equal) (simd_register, simd_register, simd_mmask, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_compare_greater_than_or_equal_f64;

		//load//
		constexpr inline static simd_register(&load) (const value_type*) noexcept = SimdArchOp<Arch>::load_f64;
		constexpr inline static simd_register(&mmasked_load) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_f64;
		constexpr inline static simd_register(&rmasked_load) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_f64;
		constexpr inline static simd_register(&load_aligned) (const value_type*) noexcept = SimdArchOp<Arch>::load_aligned_f64;
		constexpr inline static simd_register(&mmasked_load_aligned) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_load_aligned_f64;
		constexpr inline static simd_register(&rmasked_load_aligned) (const value_type*, simd_register) noexcept = SimdArchOp<Arch>::rmasked_load_aligned_f64;

		//store/
		constexpr inline static void(&store) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_f64;
		constexpr inline static void(&mmasked_store) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_f64;
		constexpr inline static void(&rmasked_store) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_f64;
		constexpr inline static void(&store_aligned) (value_type*, simd_register) noexcept = SimdArchOp<Arch>::store_aligned_f64;
		constexpr inline static void(&mmasked_store_aligned) (value_type*, simd_mmask, simd_register) noexcept = SimdArchOp<Arch>::mmasked_store_aligned_f64;
		constexpr inline static void(&rmasked_store_aligned) (value_type*, simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_store_aligned_f64;

		//set//
		constexpr inline static simd_register(&set) (const value_type) noexcept = SimdArchOp<Arch>::set_f64;
		constexpr inline static simd_register(&mmasked_set) (const value_type, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_set_f64;
		constexpr inline static simd_register(&mmasked_src_set) (const value_type, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_f64;
		constexpr inline static simd_register(&set_zero) (void) noexcept = SimdArchOp<Arch>::set_zero_f64;
		constexpr inline static simd_register(&mmasked_src_set_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_set_zero_f64;
		constexpr inline static simd_register(&shift_left) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_left_f64;
		constexpr inline static simd_register(&shift_right) (simd_register, const Size) noexcept = SimdArchOp<Arch>::shift_right_f64;

		//test//
		constexpr inline static Bool(&test_all_zero) (simd_register) noexcept = SimdArchOp<Arch>::test_all_zero_f64;
		constexpr inline static Bool(&test_all_one) (simd_register) noexcept = SimdArchOp<Arch>::test_all_one_f64;
		constexpr inline static Bool(&mmasked_test_all_zero) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_zero_f64;
		constexpr inline static Bool(&rmasked_test_all_zero) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_zero_f64;
		constexpr inline static Bool(&mmasked_test_all_one) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_test_all_one_f64;
		constexpr inline static Bool(&rmasked_test_all_one) (simd_register, simd_register) noexcept = SimdArchOp<Arch>::rmasked_test_all_one_f64;

		//swizzle//
		constexpr inline static simd_register(&insert) (simd_register, const value_type, const Size) noexcept = SimdArchOp<Arch>::insert_f64;
		constexpr inline static value_type(&extract) (simd_register, const Size) noexcept = SimdArchOp<Arch>::extract_f64;
		constexpr inline static simd_register(&blend_mmask) (simd_register, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::blend_mmask_f64;
		constexpr inline static simd_register(&blend_rmask) (simd_register, simd_register, simd_register) noexcept = SimdArchOp<Arch>::blend_rmask_f64;

		//advance mem//
		constexpr inline static simd_register(&broadcast) (const value_type*) noexcept = SimdArchOp<Arch>::broadcast_f64;
		constexpr inline static simd_register(&mmasked_broadcast) (const value_type*, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_broadcast_f64;
		constexpr inline static simd_register(&mmasked_src_broadcast) (const value_type*, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_broadcast_f64;
		constexpr inline static simd_register(&gather) (const value_type*, simd_register_index) noexcept = SimdArchOp<Arch>::gather_f64;
		constexpr inline static simd_register(&gather_scaled) (const value_type*, simd_register_index, const Size) noexcept = SimdArchOp<Arch>::gather_scaled_f64;
		constexpr inline static simd_register(&mmasked_gather) (const value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_gather_f64;
		constexpr inline static simd_register(&mmasked_gather_scaled) (const value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_gather_scaled_f64;
		constexpr inline static void(&scatter) (value_type*, simd_register_index, simd_register) noexcept = SimdArchOp<Arch>::scatter_f64;
		constexpr inline static void(&scatter_scaled) (value_type*, simd_register_index, simd_register, const Size) noexcept = SimdArchOp<Arch>::scatter_scaled_f64;
		constexpr inline static void(&mmasked_scatter) (value_type*, simd_register_index, simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_scatter_f64;
		constexpr inline static void(&mmasked_scatter_scaled) (value_type*, simd_register_index, simd_register, simd_mmask, const Size) noexcept = SimdArchOp<Arch>::mmasked_scatter_scaled_f64;

		//mask//
		constexpr inline static Bool(&mask_compare_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_equal_f64;
		constexpr inline static Bool(&mask_compare_not_equal) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_compare_not_equal_f64;
		constexpr inline static simd_mmask(&mask_bitwise_shift_left) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_left_f64;
		constexpr inline static simd_mmask(&mask_bitwise_shift_right) (simd_mmask, const Size) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_shift_right_f64;
		constexpr inline static simd_mmask(&mask_bitwise_and) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_and_f64;
		constexpr inline static simd_mmask(&mask_bitwise_or) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_or_f64;
		constexpr inline static simd_mmask(&mask_bitwise_xor) (simd_mmask, simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_xor_f64;
		constexpr inline static simd_mmask(&mask_bitwise_not) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_bitwise_not_f64;
		constexpr inline static Size(&mask_popcount) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_popcount_f64;
		constexpr inline static Bool(&mask_bitscan_forward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_forward_f64;
		constexpr inline static Bool(&mask_bitscan_backward) (simd_mmask, Size&) noexcept = ArchSimdMaskOp<Arch>::mask_bitscan_backward_f64;
		constexpr inline static ui64(&mask_get) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_get_f64;
		constexpr inline static simd_mmask(&mask_set) (const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_f64;
		constexpr inline static Bool(&mask_test_all_inactive) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_inactive_f64;
		constexpr inline static Bool(&mask_test_all_active) (simd_mmask) noexcept = ArchSimdMaskOp<Arch>::mask_test_all_active_f64;
		constexpr inline static Bool(&mask_test_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_inactive_at_f64;
		constexpr inline static Bool(&mask_test_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_test_active_at_f64;
		constexpr inline static simd_mmask(&mask_set_all_inactive) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_inactive_f64;
		constexpr inline static simd_mmask(&mask_set_all_active) (void) noexcept = ArchSimdMaskOp<Arch>::mask_set_all_active_f64;
		constexpr inline static simd_mmask(&mask_set_inactive_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_inactive_at_f64;
		constexpr inline static simd_mmask(&mask_set_active_at) (simd_mmask, const ui64) noexcept = ArchSimdMaskOp<Arch>::mask_set_active_at_f64;

		constexpr inline static simd_mmask(&rmask_to_mmask) (simd_register) noexcept = SimdArchOp<Arch>::rmask_to_mmask_f64;
		constexpr inline static simd_register(&mmask_to_rmask) (simd_mmask) noexcept = SimdArchOp<Arch>::mmask_to_rmask_f64;
		constexpr inline static simd_mmask(&cmask_to_mmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_mmask_f64;
		constexpr inline static simd_register(&cmask_to_rmask) (simd_cmask) noexcept = SimdArchOp<Arch>::cmask_to_rmask_f64;

		//convert//
		constexpr inline static simd_register(&convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdArchOp<Arch>::convert_any_to_f64;
		constexpr inline static SimdRegisterAny<Arch>(&convert_to_any) (simd_register) noexcept = SimdArchOp<Arch>::convert_f64_to_any;
		constexpr inline static SimdRegisterI64<Arch>(&convert_to_int) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_int_f64;
		constexpr inline static SimdRegisterI64<Arch>(&mmasked_convert_to_int) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_int_f64;
		constexpr inline static SimdRegisterI64<Arch>(&mmasked_src_convert_to_int) (simd_register, SimdRegisterI64<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_int_f64;
		constexpr inline static SimdRegisterUI64<Arch>(&convert_to_uint) (simd_register) noexcept = SimdArchOp<Arch>::convert_to_uint_f64;
		constexpr inline static SimdRegisterUI64<Arch>(&mmasked_convert_to_uint) (simd_register, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_convert_to_uint_f64;
		constexpr inline static SimdRegisterUI64<Arch>(&mmasked_src_convert_to_uint) (simd_register, SimdRegisterUI64<Arch>, simd_mmask) noexcept = SimdArchOp<Arch>::mmasked_src_convert_to_uint_f64;

		//reg f64
	};

	template<typename DataType, typename Arch>
	using SimdRegister = SimdOfType<DataType, Arch>::simd_register;
	template<typename DataType, typename Arch>
	using SimdRegisterInfo = SimdOfType<DataType, Arch>::simd_register_info;
	template<typename DataType, typename Arch>
	using SimdMask = SimdOfType<DataType, Arch>::simd_mmask;
	template<typename DataType, typename Arch>
	using SimdMaskInfo = SimdOfType<DataType, Arch>::simd_mask_info;
	template<typename DataType, typename Arch>
	using SimdCMask = SimdOfType<DataType, Arch>::simd_cmask;
	template<typename DataType, typename Arch>
	using SimdCMaskInfo = SimdOfType<DataType, Arch>::simd_cmask_info;

	template<typename DataType, typename Arch>
	using SimdRegisterBit = SimdOfType<DataType, Arch>::simd_register_bit;
	template<typename DataType, typename Arch>
	using SimdRegisterBitInfo = SimdOfType<DataType, Arch>::simd_register_info_bit;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_add) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::add;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_add) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_add;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_add) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_add;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_sub) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::sub;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_sub) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_sub;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_sub) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_sub;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mul) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mul;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_mul) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_mul;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_mul) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_mul;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_div) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::div;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_div) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_div;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_div) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_div;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_add_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::add_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_add_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_add_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_add_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_add_sat;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_sub_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::sub_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_sub_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_sub_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_sub_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_sub_sat;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mul_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mul_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_mul_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_mul_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_mul_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_mul_sat;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_div_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::div_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_div_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_div_sat;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_div_sat) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_div_sat;

	template<typename DataType, typename Arch>
	constexpr inline SimdRegister<DataType, Arch>(&simd_remainder) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::remainder;
	template<typename DataType, typename Arch>
	constexpr inline SimdRegister<DataType, Arch>(&simd_mmasked_remainder) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_remainder;
	template<typename DataType, typename Arch>
	constexpr inline SimdRegister<DataType, Arch>(&simd_mmasked_src_remainder) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_remainder;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_fused_mul_add) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::fused_mul_add;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_fused_mul_add) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_fused_mul_add;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_fused_mul_add) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_fused_mul_add;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_fused_mul_sub) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::fused_mul_sub;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_fused_mul_sub) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_fused_mul_sub;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_fused_mul_sub) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_fused_mul_sub;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_square_root) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::square_root;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_square_root) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_square_root;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_square_root) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_square_root;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_reciprocal) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reciprocal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_reciprocal) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reciprocal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_reciprocal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reciprocal;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_reciprocal_square_root) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reciprocal_square_root;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_reciprocal_square_root) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reciprocal_square_root;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_reciprocal_square_root) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reciprocal_square_root;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_max) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::max;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_max) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_max;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_max) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_max;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_min) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::min;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_min) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_min;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_min) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_min;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_abs) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::abs;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_abs) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_abs;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_abs) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_abs;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_negate) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::negate;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_negate) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_negate;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_negate) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_negate;

	//round//
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_ceil) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::ceil;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_ceil) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_ceil;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_ceil) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_ceil;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_floor) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::floor;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_floor) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_floor;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_floor) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_floor;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_trunc) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::trunc;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_trunc) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_trunc;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_trunc) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_trunc;
	
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_round) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::round;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_round) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_round;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_round) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_round;

	//trig//
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_sin) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_sin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_sin) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_sin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_sin) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_sin;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_sin) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::sin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_sin) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_sin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_sin) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_sin;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_cos) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_cos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_cos) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_cos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_cos) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_cos;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_cos) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::cos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_cos) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_cos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_cos) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_cos;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_tan) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_tan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_tan) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_tan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_tan) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_tan;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_tan) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::tan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_tan) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_tan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_tan) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_tan;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_asin) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_asin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_asin) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_asin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_asin) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_asin;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_asin) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::asin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_asin) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_asin;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_asin) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_asin;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_acos) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_acos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_acos) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_acos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_acos) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_acos;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_acos) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::acos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_acos) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_acos;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_acos) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_acos;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_atan) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_atan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_atan) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_atan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_atan) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_atan;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_atan) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::atan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_atan) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_atan;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_atan) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_atan;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_sinh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_sinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_sinh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_sinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_sinh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_sinh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_sinh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::sinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_sinh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_sinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_sinh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_sinh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_cosh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_cosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_cosh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_cosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_cosh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_cosh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_cosh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::cosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_cosh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_cosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_cosh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_cosh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_tanh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_tanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_tanh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_tanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_tanh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_tanh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_tanh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::tanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_tanh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_tanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_tanh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_tanh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_asinh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_asinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_asinh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_asinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_asinh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_asinh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_asinh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::asinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_asinh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_asinh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_asinh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_asinh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_acosh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_acosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_acosh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_acosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_acosh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_acosh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_acosh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::acosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_acosh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_acosh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_acosh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_acosh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_basic_atanh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::basic_atanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_basic_atanh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_basic_atanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_basic_atanh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_basic_atanh;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_atanh) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::atanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_atanh) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_atanh;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_atanh) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_atanh;

	//bitwise//
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_shift_left_c) (SimdRegister<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::bitwise_shift_left_c;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_shift_left_c) (SimdRegister<DataType, Arch>, const Size, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_shift_left_c;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_shift_left_c) (SimdRegister<DataType, Arch>, const Size, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_shift_left_c;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_shift_right_c) (SimdRegister<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::bitwise_shift_right_c;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_shift_right_c) (SimdRegister<DataType, Arch>, const Size, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_shift_right_c;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_shift_right_c) (SimdRegister<DataType, Arch>, const Size, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_shift_right_c;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_shift_left) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::bitwise_shift_left;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_shift_left) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_shift_left;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_shift_left) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_shift_left;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_shift_right) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::bitwise_shift_right;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_shift_right) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_shift_right;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_shift_right) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_shift_right;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_and) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::bitwise_and;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_and) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_and;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_and) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_and;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_or) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::bitwise_or;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_or) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_or;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_or) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_or;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_xor) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::bitwise_xor;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_xor) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_xor;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_xor) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_xor;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_bitwise_not) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::bitwise_not;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_bitwise_not) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitwise_not;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_bitwise_not) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitwise_not;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegisterBit<DataType, Arch>(&simd_popcount) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::popcount;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegisterBit<DataType, Arch>(&simd_mmasked_popcount) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_popcount;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegisterBit<DataType, Arch>(&simd_mmasked_src_popcount) (SimdRegister<DataType, Arch>, SimdRegisterBit<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_popcount;

	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_bitscan_forward) (SimdRegister<DataType, Arch>, SimdRegisterBit<DataType, Arch>&) noexcept = SimdOfType<DataType, Arch>::bitscan_forward;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_bitscan_forward) (SimdRegister<DataType, Arch>, SimdRegisterBit<DataType, Arch>&, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitscan_forward;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_bitscan_forward) (SimdRegister<DataType, Arch>, SimdRegisterBit<DataType, Arch>&, SimdRegisterBit<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitscan_forward;

	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_bitscan_backward) (SimdRegister<DataType, Arch>, SimdRegisterBit<DataType, Arch>&) noexcept = SimdOfType<DataType, Arch>::bitscan_backward;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_bitscan_backward) (SimdRegister<DataType, Arch>, SimdRegisterBit<DataType, Arch>&, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_bitscan_backward;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_bitscan_backward) (SimdRegister<DataType, Arch>, SimdRegisterBit<DataType, Arch>&, SimdRegisterBit<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_bitscan_backward;

	//reduce//
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_reduce_min) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reduce_min;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_reduce_min) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reduce_min;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_src_reduce_min) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reduce_min;

	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_reduce_max) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reduce_max;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_reduce_max) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reduce_max;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_src_reduce_max) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reduce_max;

	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_reduce_add) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reduce_add;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_reduce_add) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reduce_add;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_src_reduce_add) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reduce_add;

	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_reduce_mul) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reduce_mul;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_reduce_mul) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reduce_mul;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_src_reduce_mul) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reduce_mul;

	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_reduce_and) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reduce_and;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_reduce_and) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reduce_and;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_src_reduce_and) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reduce_and;

	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_reduce_or) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reduce_or;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_reduce_or) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reduce_or;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_src_reduce_or) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reduce_or;

	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_reduce_xor) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::reduce_xor;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_reduce_xor) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_reduce_xor;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_mmasked_src_reduce_xor) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_reduce_xor;

	//compare//
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_compare_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::compare_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_compare_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_compare_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_compare_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_compare_equal;

	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_compare_not_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::compare_not_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_compare_not_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_compare_not_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_compare_not_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_compare_not_equal;

	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_compare_less_than) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::compare_less_than;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_compare_less_than) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_compare_less_than;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_compare_less_than) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_compare_less_than;

	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_compare_greater_than) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::compare_greater_than;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_compare_greater_than) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_compare_greater_than;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_compare_greater_than) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_compare_greater_than;

	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_compare_less_than_or_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::compare_less_than_or_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_compare_less_than_or_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_compare_less_than_or_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_compare_less_than_or_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_compare_less_than_or_equal;

	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_compare_greater_than_or_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::compare_greater_than_or_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_compare_greater_than_or_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_compare_greater_than_or_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mmasked_src_compare_greater_than_or_equal) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_compare_greater_than_or_equal;

	//load//
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_load) (const DataType*) noexcept = SimdOfType<DataType, Arch>::load;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_load) (const DataType*, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_load;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_rmasked_load) (const DataType*, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::rmasked_load;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_load_aligned) (const DataType*) noexcept = SimdOfType<DataType, Arch>::load_aligned;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_load_aligned) (const DataType*, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_load_aligned;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_rmasked_load_aligned) (const DataType*, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::rmasked_load_aligned;

	//store//
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_store) (DataType*, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::store;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_mmasked_store) (DataType*, SimdMask<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_store;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_rmasked_store) (DataType*, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::rmasked_store;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_store_aligned) (DataType*, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::store_aligned;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_mmasked_store_aligned) (DataType*, SimdMask<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_store_aligned;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_rmasked_store_aligned) (DataType*, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::rmasked_store_aligned;

	//set//
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_set) (const DataType) noexcept = SimdOfType<DataType, Arch>::set;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_set) (const DataType, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_set;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_src_mmasked_set) (const DataType, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_set;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_set_zero) (void) noexcept = SimdOfType<DataType, Arch>::set_zero;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&mmasked_src_simd_set_zero) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_set_zero;

	//test//
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_test_all_zero) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::test_all_zero;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_test_all_one) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::test_all_one;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mmasked_test_all_zero) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_test_all_zero;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_rmasked_test_all_zero) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::rmasked_test_all_zero;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mmasked_test_all_one) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_test_all_one;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_rmasked_test_all_one) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::rmasked_test_all_one;

	//convert//
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegisterAny<Arch>(&simd_convert_to_any) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::convert_to_any;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_convert_any_to) (SimdRegisterAny<Arch>) noexcept = SimdOfType<DataType, Arch>::convert_any_to;

	template<typename DataType, typename Arch> 
		requires(IsBuiltInUnsignedIntegerC<DataType> || IsBuiltInFloatingPointC<DataType>)
	constexpr inline static SimdRegister<IntOfByteSize<sizeof(DataType)>, Arch>(&simd_convert_to_int) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::convert_to_int;
	template<typename DataType, typename Arch>
		requires(IsBuiltInUnsignedIntegerC<DataType> || IsBuiltInFloatingPointC<DataType>)
	constexpr inline static SimdRegister<IntOfByteSize<sizeof(DataType)>, Arch>(&simd_mmasked_convert_to_int) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_convert_to_int;
	template<typename DataType, typename Arch>
		requires(IsBuiltInUnsignedIntegerC<DataType> || IsBuiltInFloatingPointC<DataType>)
	constexpr inline static SimdRegister<IntOfByteSize<sizeof(DataType)>, Arch>(&simd_mmasked_src_convert_to_int) (SimdRegister<DataType, Arch>, SimdRegister<IntOfByteSize<sizeof(DataType)>, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_convert_to_int;

	template<typename DataType, typename Arch>
		requires(IsBuiltInSignedIntegerC<DataType> || IsBuiltInFloatingPointC<DataType>)
	constexpr inline static SimdRegister<UIntOfByteSize<sizeof(DataType)>, Arch>(&simd_convert_to_uint) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::convert_to_uint;
	template<typename DataType, typename Arch>
		requires(IsBuiltInSignedIntegerC<DataType> || IsBuiltInFloatingPointC<DataType>)
	constexpr inline static SimdRegister<UIntOfByteSize<sizeof(DataType)>, Arch>(&simd_mmasked_convert_to_uint) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_convert_to_uint;
	template<typename DataType, typename Arch>
		requires(IsBuiltInSignedIntegerC<DataType> || IsBuiltInFloatingPointC<DataType>)
	constexpr inline static SimdRegister<UIntOfByteSize<sizeof(DataType)>, Arch>(&simd_mmasked_src_convert_to_uint) (SimdRegister<DataType, Arch>, SimdRegister<UIntOfByteSize<sizeof(DataType)>, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_convert_to_uint;

	template<typename DataType, typename Arch>
		requires(IsBuiltInIntegerC<DataType> && sizeof(DataType) >= sizeof(f32))
	constexpr inline static SimdRegister<FloatOfByteSize<sizeof(DataType)>, Arch>(&simd_convert_to_float) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::convert_to_float;
	template<typename DataType, typename Arch>
		requires(IsBuiltInIntegerC<DataType> && sizeof(DataType) >= sizeof(f32))
	constexpr inline static SimdRegister<FloatOfByteSize<sizeof(DataType)>, Arch>(&simd_mmasked_convert_to_float) (SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_convert_to_float;
	template<typename DataType, typename Arch>
		requires(IsBuiltInIntegerC<DataType> && sizeof(DataType) >= sizeof(f32))
	constexpr inline static SimdRegister<FloatOfByteSize<sizeof(DataType)>, Arch>(&simd_mmasked_src_convert_to_float) (SimdRegister<DataType, Arch>, SimdRegister<FloatOfByteSize<sizeof(DataType)>, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_convert_to_float;

	//swizzle
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_insert) (SimdRegister<DataType, Arch>, const DataType, const Size) noexcept = SimdOfType<DataType, Arch>::insert;
	template<typename DataType, typename Arch>
	constexpr inline static DataType(&simd_extract) (SimdRegister<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::extract;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_blend_mmask) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::blend_mmask;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_blend_rmask) (SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::blend_rmask;

	//advanced mem//
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_broadcast) (const DataType*) noexcept = SimdOfType<DataType, Arch>::broadcast;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_broadcast) (const DataType*, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_broadcast;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_src_broadcast) (const DataType*, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_src_broadcast;

	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_gather) (const DataType*, SimdRegisterI32<Arch>) noexcept = SimdOfType<DataType, Arch>::gather;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_gather_scaled) (const DataType*, SimdRegisterI32<Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::gather_scaled;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_gather) (const DataType*, SimdRegisterI32<Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_gather;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmasked_gather_scaled) (const DataType*, SimdRegisterI32<Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::mmasked_gather_scaled;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_scatter) (DataType*, SimdRegisterI32<Arch>, SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::scatter;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_scatter_scaled) (DataType*, SimdRegisterI32<Arch>, SimdRegister<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::scatter_scaled;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_mmasked_scatter) (DataType*, SimdRegisterI32<Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmasked_scatter;
	template<typename DataType, typename Arch>
	constexpr inline static void(&simd_mmasked_scatter_scaled) (DataType*, SimdRegisterI32<Arch>, SimdRegister<DataType, Arch>, SimdMask<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::mmasked_scatter_scaled;

	//mask//
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_rmask_to_mmask) (SimdRegister<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::rmask_to_mmask;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_mmask_to_rmask) (SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mmask_to_rmask;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_cmask_to_mmask) (SimdCMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::cmask_to_mmask;
	template<typename DataType, typename Arch>
	constexpr inline static SimdRegister<DataType, Arch>(&simd_cmask_to_rmask) (SimdCMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::cmask_to_rmask;

	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_compare_equal) (SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_compare_equal;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_compare_not_equal) (SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_compare_not_equal;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_bitwise_shift_left) (SimdMask<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::mask_bitwise_shift_left;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_bitwise_shift_right) (SimdMask<DataType, Arch>, const Size) noexcept = SimdOfType<DataType, Arch>::mask_bitwise_shift_right;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_bitwise_and) (SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_bitwise_and;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_bitwise_or) (SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_bitwise_or;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_bitwise_xor) (SimdMask<DataType, Arch>, SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_bitwise_xor;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_bitwise_not) (SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_bitwise_not;
	template<typename DataType, typename Arch>
	constexpr inline static Size(&simd_mask_popcount) (SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_popcount;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_bitscan_forward) (SimdMask<DataType, Arch>, Bool&) noexcept = SimdOfType<DataType, Arch>::mask_bitscan_forward;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_bitscan_backward) (SimdMask<DataType, Arch>, Bool&) noexcept = SimdOfType<DataType, Arch>::mask_bitscan_backward;
	template<typename DataType, typename Arch>
	constexpr inline static ui64(&simd_mask_get) (SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_get;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_set) (const ui64) noexcept = SimdOfType<DataType, Arch>::mask_set;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_test_all_inactive) (SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_test_all_inactive;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_test_all_active) (SimdMask<DataType, Arch>) noexcept = SimdOfType<DataType, Arch>::mask_test_all_active;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_test_inactive_at) (SimdMask<DataType, Arch>, const ui64) noexcept = SimdOfType<DataType, Arch>::mask_test_inactive_at;
	template<typename DataType, typename Arch>
	constexpr inline static Bool(&simd_mask_test_active_at) (SimdMask<DataType, Arch>, const ui64) noexcept = SimdOfType<DataType, Arch>::mask_test_active_at;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_set_all_inactive) (void) noexcept = SimdOfType<DataType, Arch>::mask_set_all_inactive;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_set_all_active) (void) noexcept = SimdOfType<DataType, Arch>::mask_set_all_active;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_set_inactive_at) (SimdMask<DataType, Arch>, const ui64) noexcept = SimdOfType<DataType, Arch>::mask_set_inactive_at;
	template<typename DataType, typename Arch>
	constexpr inline static SimdMask<DataType, Arch>(&simd_mask_set_active_at) (SimdMask<DataType, Arch>, const ui64) noexcept = SimdOfType<DataType, Arch>::mask_set_active_at;
}