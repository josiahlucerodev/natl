#pragma once 

//own
#include "bits.h"
#include "basicTypes.h"
#include "../container/array.h"
#include "../container/arrayView.h"

//@export 
namespace natl {
	//int in bytes check 
	constexpr Bool i8InBytesCheck(const Size size) noexcept {
		return size >= 1;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool i8InBytesCheck(const BytesType& dst) noexcept {
		return i8InBytesCheck(dst.size());
	}

	constexpr Bool i16InBytesCheck(const Size size) noexcept {
		return size >= 2;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool i16InBytesCheck(const BytesType& dst) noexcept {
		return i16InBytesCheck(dst.size());
	}

	constexpr Bool i32InBytesCheck(const Size size) noexcept {
		return size >= 4;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool i32InBytesCheck(const BytesType& dst) noexcept {
		return i32InBytesCheck(dst.size());
	}

	constexpr Bool i64InBytesCheck(const Size size) noexcept {
		return size >= 8;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool i64InBytesCheck(const BytesType& dst) noexcept {
		return i64InBytesCheck(dst.size());
	}

	constexpr Bool ui8InBytesCheck(const Size size) noexcept {
		return size >= 1;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool ui8InBytesCheck(const BytesType& dst) noexcept {
		return ui8InBytesCheck(dst.size());
	}

	constexpr Bool ui16InBytesCheck(const Size size) noexcept {
		return size >= 2;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool ui16InBytesCheck(const BytesType& dst) noexcept {
		return ui16InBytesCheck(dst.size());
	}

	constexpr Bool ui32InBytesCheck(const Size size) noexcept {
		return size >= 4;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool ui32InBytesCheck(const BytesType& dst) noexcept {
		return ui32InBytesCheck(dst.size());
	}

	constexpr Bool ui64InBytesCheck(const Size size) noexcept {
		return size >= 8;
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr Bool ui64InBytesCheck(const BytesType& dst) noexcept {
		return ui64InBytesCheck(dst.size());
	}

	template<typename IntType>
		requires(IsBuiltInIntegerC<Decay<IntType>>)
	constexpr Bool intInBytesCheck(const Size size) noexcept {
		using decayed_int_type = Decay<IntType>;
		if constexpr (IsSame<i8, decayed_int_type>) {
			return i8InBytesCheck(size);
		} else if constexpr (IsSameC<i16, decayed_int_type>) {
			return i16InBytesCheck(size);
		} else if constexpr (IsSameC<i32, decayed_int_type>) {
			return i32InBytesCheck(size);
		} else if constexpr (IsSameC<ui64, decayed_int_type>) {
			return i64InBytesCheck(size);
		} else if constexpr (IsSameC<ui8, decayed_int_type>) {
			return ui8InBytesCheck(size);
		} else if constexpr (IsSameC<ui16, decayed_int_type>) {
			return ui16InBytesCheck(size);
		} else if constexpr (IsSameC<ui32, decayed_int_type>) {
			return ui32InBytesCheck(size);
		} else {
			return ui64InBytesCheck(size);
		}
	}

	template<typename IntType, typename BytesType>
		requires(IsBuiltInIntegerC<IntType>&& IsConstByteViewLike<BytesType>)
	constexpr Bool intInBytesCheck(const BytesType& dst) noexcept {
		return intInBytesCheck<IntType>(dst.size());
	}

	//int to bytes
	constexpr Array<ui8, 1> i8ToBytes(const i8 value) noexcept {
		return bitCast<Array<ui8, 1>, i8>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void i8ToBytesAt(const i8 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 1> bytes = i8ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void i8ToBytesAt(const i8 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		i8ToBytesAt(value, temp);
	}

	constexpr Array<ui8, 2> i16ToBytes(const i16 value) noexcept {
		return bitCast<Array<ui8, 2>, i16>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void i16ToBytesAt(const i16 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 2> bytes = i16ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
		dst[1] = static_cast<dst_type>(bytes[1]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void i16ToBytesAt(const i16 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		i16ToBytesAt(value, temp);
	}

	constexpr Array<ui8, 4> i32ToBytes(const i32 value) noexcept {
		return bitCast<Array<ui8, 4>, i32>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void i32ToBytesAt(const i32 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 4> bytes = i32ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
		dst[1] = static_cast<dst_type>(bytes[1]);
		dst[2] = static_cast<dst_type>(bytes[2]);
		dst[3] = static_cast<dst_type>(bytes[3]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void i32ToBytesAt(const i32 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		i32ToBytesAt(value, temp);
	}

	constexpr Array<ui8, 8> i64ToBytes(const i64 value) noexcept {
		return bitCast<Array<ui8, 8>, i64>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void i64ToBytesAt(const i64 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 8> bytes = i64ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
		dst[1] = static_cast<dst_type>(bytes[1]);
		dst[2] = static_cast<dst_type>(bytes[2]);
		dst[3] = static_cast<dst_type>(bytes[3]);
		dst[4] = static_cast<dst_type>(bytes[4]);
		dst[5] = static_cast<dst_type>(bytes[5]);
		dst[6] = static_cast<dst_type>(bytes[6]);
		dst[7] = static_cast<dst_type>(bytes[7]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void i64ToBytesAt(const i64 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		i64ToBytesAt(value, temp);
	}

	constexpr Array<ui8, 1> ui8ToBytes(const ui8 value) noexcept {
		return bitCast<Array<ui8, 1>, ui8>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void ui8ToBytesAt(const ui8 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 1> bytes = ui8ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void ui8ToBytesAt(const ui8 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		ui8ToBytesAt(value, temp);
	}

	constexpr Array<ui8, 2> ui16ToBytes(const ui16 value) noexcept {
		return bitCast<Array<ui8, 2>, ui16>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void ui16ToBytesAt(const ui16 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 2> bytes = ui16ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
		dst[1] = static_cast<dst_type>(bytes[1]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void ui16ToBytesAt(const ui16 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		ui16ToBytesAt(value, temp);
	}

	constexpr Array<ui8, 4> ui32ToBytes(const ui32 value) noexcept {
		return bitCast<Array<ui8, 4>, ui32>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void ui32ToBytesAt(const ui32 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 4> bytes = ui32ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
		dst[1] = static_cast<dst_type>(bytes[1]);
		dst[2] = static_cast<dst_type>(bytes[2]);
		dst[3] = static_cast<dst_type>(bytes[3]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void ui32ToBytesAt(const ui32 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		ui32ToBytesAt(value, temp);
	}

	constexpr Array<ui8, 8> ui64ToBytes(const ui64 value) noexcept {
		return bitCast<Array<ui8, 8>, ui64>(value);
	}
	template<typename BytesType>
		requires(IsByteViewLike<BytesType>)
	constexpr void ui64ToBytesAt(const ui64 value, BytesType& dst) noexcept {
		using dst_type = Decay<decltype(dst[0])>;
		const Array<ui8, 8> bytes = ui64ToBytes(value);
		dst[0] = static_cast<dst_type>(bytes[0]);
		dst[1] = static_cast<dst_type>(bytes[1]);
		dst[2] = static_cast<dst_type>(bytes[2]);
		dst[3] = static_cast<dst_type>(bytes[3]);
		dst[4] = static_cast<dst_type>(bytes[4]);
		dst[5] = static_cast<dst_type>(bytes[5]);
		dst[6] = static_cast<dst_type>(bytes[6]);
		dst[7] = static_cast<dst_type>(bytes[7]);
	}
	template<typename CopyableBytesType>
		requires(IsByteViewLike<CopyableBytesType>&& IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void ui64ToBytesAt(const ui64 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		ui64ToBytesAt(value, temp);
	}

	template<typename IntType>
		requires(IsBuiltInIntegerC<Decay<IntType>>)
	constexpr auto intToBytes(const IntType value) noexcept {
		using decayed_int_type = Decay<IntType>;
		if constexpr (IsSameC<i8, IntType>) {
			return i8ToBytes(value);
		} else if constexpr (IsSameC<i16, decayed_int_type>) {
			return i16ToBytes(value);
		} else if constexpr (IsSameC<i32, decayed_int_type>) {
			return i32ToBytes(value);
		} else if constexpr (IsSameC<ui64, decayed_int_type>) {
			return i64ToBytes(value);
		} else if constexpr (IsSameC<ui8, decayed_int_type>) {
			return ui8ToBytes(value);
		} else if constexpr (IsSameC<ui16, decayed_int_type>) {
			return ui16ToBytes(value);
		} else if constexpr (IsSameC<ui32, decayed_int_type>) {
			return ui32ToBytes(value);
		} else {
			return ui64ToBytes(value);
		}
	}

	template<typename IntType, typename BytesType>
		requires(IsBuiltInIntegerC<IntType>&& IsByteViewLike<BytesType>)
	constexpr void intToBytesAt(const IntType value, BytesType& dst) noexcept {
		using decayed_int_type = Decay<IntType>;
		if constexpr (IsSameC<i8, decayed_int_type>) {
			i8ToBytesAt(value, dst);
		} else if constexpr (IsSameC<i16, decayed_int_type>) {
			i16ToBytesAt(value, dst);
		} else if constexpr (IsSameC<i32, decayed_int_type>) {
			i32ToBytesAt(value, dst);
		} else if constexpr (IsSameC<ui64, decayed_int_type>) {
			i64ToBytesAt(value, dst);
		} else if constexpr (IsSameC<ui8, decayed_int_type>) {
			ui8ToBytesAt(value, dst);
		} else if constexpr (IsSameC<ui16, decayed_int_type>) {
			ui16ToBytesAt(value, dst);
		} else if constexpr (IsSameC<ui32, decayed_int_type>) {
			ui32ToBytesAt(value, dst);
		} else {
			ui64ToBytesAt(value, dst);
		}
	}
	template<typename IntType, typename CopyableBytesType>
		requires(IsBuiltInIntegerC<IntType> && IsByteViewLike<CopyableBytesType> && IsCopyableStorageDstC<CopyableBytesType>)
	constexpr void intToBytesAt(const ui64 value, const CopyableBytesType& dst) noexcept {
		CopyableBytesType temp = dst;
		intToBytesAt(value, temp);
	}

	//bytes to int
	constexpr i8 bytesToI8(const Array<ui8, 1>& bytes) noexcept {
		return bitCast<i8, Array<ui8, 1>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr i8 bytesToI8(const BytesType& bytes) noexcept {
		const Array<ui8, 1> fixedBytes = { 
			static_cast<ui8>(bytes[0]) 
		};
		return bytesToI8(fixedBytes);
	}

	constexpr i16 bytesToI16(const Array<ui8, 2>& bytes) noexcept {
		return bitCast<i16, Array<ui8, 2>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr i16 bytesToI16(const BytesType& bytes) noexcept {
		const Array<ui8, 2> fixedBytes = { 
			static_cast<ui8>(bytes[0]), static_cast<ui8>(bytes[1])
		};
		return bytesToI16(fixedBytes);
	}

	constexpr i32 bytesToI32(const Array<ui8, 4>& bytes) noexcept {
		return bitCast<i32, Array<ui8, 4>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr i32 bytesToI32(const BytesType& bytes) noexcept {
		const Array<ui8, 4> fixedBytes = { 
			static_cast<ui8>(bytes[0]), static_cast<ui8>(bytes[1]),
			static_cast<ui8>(bytes[2]), static_cast<ui8>(bytes[3]) };
		return bytesToI32(fixedBytes);
	}

	constexpr i64 bytesToI64(const Array<ui8, 8>& bytes) noexcept {
		return bitCast<i64, Array<ui8, 8>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr i64 bytesToI64(const BytesType& bytes) noexcept {
		const Array<ui8, 8> fixedBytes = {
			static_cast<ui8>(bytes[0]), static_cast<ui8>(bytes[1]),
			static_cast<ui8>(bytes[2]), static_cast<ui8>(bytes[3]),
			static_cast<ui8>(bytes[4]), static_cast<ui8>(bytes[5]),
			static_cast<ui8>(bytes[6]), static_cast<ui8>(bytes[7]) };
		return bytesToI64(fixedBytes);
	}

	constexpr ui8 bytesToUi8(const Array<ui8, 1>& bytes) noexcept {
		return bitCast<ui8, Array<ui8, 1>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr ui8 bytesToUi8(const BytesType& bytes) noexcept {
		const Array<ui8, 1> fixedBytes = { static_cast<ui8>(bytes[0]) };
		return bytesToUi8(fixedBytes);
	}

	constexpr ui16 bytesToUi16(const Array<ui8, 2>& bytes) noexcept {
		return bitCast<ui16, Array<ui8, 2>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr ui16 bytesToUi16(const BytesType& bytes) noexcept {
		const Array<ui8, 2> fixedBytes = { 
			static_cast<ui8>(bytes[0]), static_cast<ui8>(bytes[1]) };
		return bytesToUi16(fixedBytes);
	}

	constexpr ui32 bytesToUi32(const Array<ui8, 4>& bytes) noexcept {
		return bitCast<ui32, Array<ui8, 4>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr ui32 bytesToUi32(const BytesType& bytes) noexcept {
		const Array<ui8, 4> fixedBytes = { 
			static_cast<ui8>(bytes[0]), static_cast<ui8>(bytes[1]),
			static_cast<ui8>(bytes[2]), static_cast<ui8>(bytes[3]) };
		return bytesToUi32(fixedBytes);
	}

	constexpr ui64 bytesToUi64(const Array<ui8, 8>& bytes) noexcept {
		return bitCast<ui64, Array<ui8, 8>>(bytes);
	}
	template<typename BytesType>
		requires(IsConstByteViewLike<BytesType>)
	constexpr ui64 bytesToUi64(const BytesType& bytes) noexcept {
		const Array<ui8, 8> fixedBytes = {
			static_cast<ui8>(bytes[0]), static_cast<ui8>(bytes[1]),
			static_cast<ui8>(bytes[2]), static_cast<ui8>(bytes[3]),
			static_cast<ui8>(bytes[4]), static_cast<ui8>(bytes[5]),
			static_cast<ui8>(bytes[6]), static_cast<ui8>(bytes[7]),
		};
		return bytesToUi64(fixedBytes);
	}

	namespace impl {
		template<typename IntType>
		struct BytesToIntFuncArgT;
		template<> struct BytesToIntFuncArgT<i8> { using type = Array<ui8, 1>; };
		template<> struct BytesToIntFuncArgT<i16> { using type = Array<ui8, 2>; };
		template<> struct BytesToIntFuncArgT<i32> { using type = Array<ui8, 4>; };
		template<> struct BytesToIntFuncArgT<i64> { using type = Array<ui8, 8>; };
		template<> struct BytesToIntFuncArgT<ui8> { using type = Array<ui8, 1>; };
		template<> struct BytesToIntFuncArgT<ui16> { using type = Array<ui8, 2>; };
		template<> struct BytesToIntFuncArgT<ui32> { using type = Array<ui8, 4>; };
		template<> struct BytesToIntFuncArgT<ui64> { using type = Array<ui8, 8>; };
	}

	template<typename IntType>
		requires(IsBuiltInIntegerC<Decay<IntType>>)
	constexpr Decay<IntType> bytesToInt(
		const typename impl::BytesToIntFuncArgT<IntType>::type& bytes) noexcept {
		using decayed_int_type = Decay<IntType>;
		if constexpr (IsSameC<i8, decayed_int_type>) {
			return bytesToI8(bytes);
		} else if constexpr (IsSameC<i16, decayed_int_type>) {
			return bytesToI16(bytes);
		} else if constexpr (IsSameC<i32, decayed_int_type>) {
			return bytesToI32(bytes);
		} else if constexpr (IsSameC<ui64, decayed_int_type>) {
			return bytesToI64(bytes);
		} else if constexpr (IsSameC<ui8, decayed_int_type>) {
			return bytesToUi8(bytes);
		} else if constexpr (IsSameC<ui16, decayed_int_type>) {
			return bytesToUi16(bytes);
		} else if constexpr (IsSameC<ui32, decayed_int_type>) {
			return bytesToUi32(bytes);
		} else {
			return bytesToUi64(bytes);
		}
	}

	template<typename IntType, typename BytesType>
		requires(IsBuiltInIntegerC<Decay<IntType>> && IsConstByteViewLike<BytesType>
			&& IsNotSame<Decay<BytesType>, typename impl::BytesToIntFuncArgT<IntType>::type>)
	constexpr Decay<IntType> bytesToInt(
		const BytesType& bytes) noexcept {
		using decayed_int_type = Decay<IntType>;
		if constexpr (IsSameC<i8, decayed_int_type>) {
			return bytesToI8(bytes);
		} else if constexpr (IsSameC<i16, decayed_int_type>) {
			return bytesToI16(bytes);
		} else if constexpr (IsSameC<i32, decayed_int_type>) {
			return bytesToI32(bytes);
		} else if constexpr (IsSameC<ui64, decayed_int_type>) {
			return bytesToI64(bytes);
		} else if constexpr (IsSameC<ui8, decayed_int_type>) {
			return bytesToUi8(bytes);
		} else if constexpr (IsSameC<ui16, decayed_int_type>) {
			return bytesToUi16(bytes);
		} else if constexpr (IsSameC<ui32, decayed_int_type>) {
			return bytesToUi32(bytes);
		} else {
			return bytesToUi64(bytes);
		}
	}

	//int operation on bytes 
	template<typename BytesType, typename IntOp>
		requires(IsConstByteViewLike<BytesType> && HasFunctionSignature<IntOp, ui8, const ui8>)
	constexpr void i8OperationOnBytes(const BytesType& bytes, IntOp&& operation) noexcept {
		const i8 value = operation(bytesToI8(bytes));
		i8ToBytesAt(value, bytes);
	}
}