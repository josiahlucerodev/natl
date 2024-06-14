#pragma once 

//std
#include <bit>

//own
#include "compilerDependent.h"
#include "typeTraits.h"
#include "limits.h"

//interface 
namespace natl {
    template<typename Integer>
        requires(IsBuiltInUnsignedIntegerC<Integer>)
    NATL_FORCE_INLINE constexpr Integer setNthBitToZero(const Integer num, const Integer n) noexcept {
        const Integer mask = ~(Integer(1) << n);
        const Integer result = num & mask;
        return result;
    }

    template<typename Integer>
        requires(IsBuiltInUnsignedIntegerC<Integer>)
    NATL_FORCE_INLINE constexpr Integer setNthBitToOne(const Integer num, const Integer n) noexcept {
        const Integer mask = Integer(1) << n;
        const Integer result = num | mask;
        return result;
    }
    template<typename Integer>
        requires(IsBuiltInIntegerC<Integer>)
    NATL_FORCE_INLINE constexpr Bool getBit(const Integer num, const Integer index) noexcept {
        const Integer mask = Integer(1) << index;
        return (num & mask) != 0;
    }

    template<typename Integer>
        requires(IsBuiltInIntegerC<Integer>)
    NATL_FORCE_INLINE constexpr Integer setBit(const Integer num, const Integer index, const Bool set) noexcept {
        const Integer mask = Integer(1) << index;
        if (set) {
            return num | mask;
        } else {
            return num & ~mask;
        }
    }
    template<typename Integer>
        requires(IsBuiltInIntegerC<Integer>)
    NATL_FORCE_INLINE constexpr Integer setToZeroOrAllOnes(const Bool setToZero) noexcept {
        if (setToZero) {
            return Integer(0);
        } else {
            return ~Integer(0);
        }
    }

    template<typename UnsignedInteger>
        requires(IsBuiltInUnsignedIntegerC<UnsignedInteger>)
    NATL_FORCE_INLINE constexpr UnsignedInteger boolToMask(bool value) noexcept  {
        return static_cast<UnsignedInteger>(-static_cast<IntOfByteSize<sizeof(UnsignedInteger)>>(value));
    }

    template<typename To, typename From>
        requires(IsSameByteSizeC<To, From>)
    NATL_FORCE_INLINE constexpr To bitCast(const From& from) noexcept {
#ifdef NATL_COMPILER_MSVC
        return __builtin_bit_cast(To, from);
#else
        static_assert(false, "bitCast not implemented for compiler");
#endif 
    }

    //popcount 
    namespace impl {
        NATL_FORCE_INLINE constexpr ui8 popcountImplS(const ui8 value) noexcept {
            using namespace natl::literals;
            ui8 x = value;
            x = (x & 0x55u) + ((x >> 1) & 0x55u);
            x = (x & 0x33u) + ((x >> 2) & 0x33u);
            x = (x & 0x0fu) + ((x >> 4) & 0x0fu);
            return x;
        }
        NATL_FORCE_INLINE constexpr ui16 popcountImplS(const ui16 value) noexcept {
            using namespace natl::literals;
            ui16 x = value;
            x = (x & 0x5555_ui32) + ((x >> 1) & 0x5555_ui32);
            x = (x & 0x3333_ui32) + ((x >> 2) & 0x3333_ui32);
            x = (x & 0x0f0f_ui32) + ((x >> 4) & 0x0f0f_ui32);
            x = (x & 0x00ff_ui32) + ((x >> 8) & 0x00ff_ui32);
            return x;
        }
        NATL_FORCE_INLINE constexpr ui32 popcountImplS(const ui32 value) noexcept {
            using namespace natl::literals;
            ui32 x = value;
            x = (x & 0x55555555_ui32) + ((x >> 1) & 0x55555555_ui32);
            x = (x & 0x33333333_ui32) + ((x >> 2) & 0x33333333_ui32);
            x = (x & 0x0f0f0f0f_ui32) + ((x >> 4) & 0x0f0f0f0f_ui32);
            x = (x & 0x00ff00ff_ui32) + ((x >> 8) & 0x00ff00ff_ui32);
            x = (x & 0x0000ffff_ui32) + ((x >> 16) & 0x0000ffff_ui32);
            return x;
        }
        NATL_FORCE_INLINE constexpr ui64 popcountImplS(const ui64 value) noexcept {
            using namespace natl::literals;
            ui64 x = value;
            x = (x & 0x5555555555555555_ui64) + ((x >> 1) & 0x5555555555555555_ui64);
            x = (x & 0x3333333333333333_ui64) + ((x >> 2) & 0x3333333333333333_ui64);
            x = (x & 0x0f0f0f0f0f0f0f0f_ui64) + ((x >> 4) & 0x0f0f0f0f0f0f0f0f_ui64);
            x = (x & 0x00ff00ff00ff00ff_ui64) + ((x >> 8) & 0x00ff00ff00ff00ff_ui64);
            x = (x & 0x0000ffff0000ffff_ui64) + ((x >> 16) & 0x0000ffff0000ffff_ui64);
            x = (x & 0x00000000ffffffff_ui64) + ((x >> 32) & 0x00000000ffffffff_ui64);
            return x;
        }

        NATL_FORCE_INLINE constexpr i8 popcountImplS(const i8 value) noexcept {
            return popcountImplS(bitCast<ui8, i8>(value));
        }
        NATL_FORCE_INLINE constexpr i16 popcountImplS(const i16 value) noexcept {
            return popcountImplS(bitCast<ui16, i16>(value));
        }
        NATL_FORCE_INLINE constexpr i32 popcountImplS(const i32 value) noexcept {
            return popcountImplS(bitCast<ui32, i32>(value));
        }
        NATL_FORCE_INLINE constexpr i64 popcountImplS(const i64 value) noexcept {
            return popcountImplS(bitCast<ui64, i64>(value));
        }

#if defined(NATL_COMPILER_MSVC) && defined(NATL_ARCHITECTURE_X86_64)
        NATL_FORCE_INLINE ui8 popcountImplD(const ui8 value) noexcept {
            return static_cast<ui8>(__popcnt(static_cast<unsigned int>(value)));
        }
        NATL_FORCE_INLINE ui16 popcountImplD(const ui16 value) noexcept {
            return __popcnt16(value);
        }
        NATL_FORCE_INLINE ui32 popcountImplD(const ui32 value) noexcept {
            return __popcnt(value);
        }
        NATL_FORCE_INLINE ui64 popcountImplD(const ui64 value) noexcept {
            return __popcnt64(value);
        }

        NATL_FORCE_INLINE i8 popcountImplD(const i8 value) noexcept {
            return popcountImplD(bitCast<ui8, i8>(value));
        }
        NATL_FORCE_INLINE i16 popcountImplD(const i16 value) noexcept {
            return popcountImplD(bitCast<ui16, i16>(value));
        }
        NATL_FORCE_INLINE i32 popcountImplD(const i32 value) noexcept {
            return popcountImplD(bitCast<ui32, i32>(value));
        }
        NATL_FORCE_INLINE i64 popcountImplD(const i64 value) noexcept {
            return popcountImplD(bitCast<ui64, i64>(value));
        }
#else 
        static_assert(false, "natl: no implemenation of dependent popcount")
#endif

    }

    NATL_FORCE_INLINE constexpr i8 popcount(const i8 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }
    NATL_FORCE_INLINE constexpr i16 popcount(const i16 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }
    NATL_FORCE_INLINE constexpr i32 popcount(const i32 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }
    NATL_FORCE_INLINE constexpr i64 popcount(const i64 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }
    NATL_FORCE_INLINE constexpr ui8 popcount(const ui8 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }
    NATL_FORCE_INLINE constexpr ui16 popcount(const ui16 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }
    NATL_FORCE_INLINE constexpr ui32 popcount(const ui32 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }
    NATL_FORCE_INLINE constexpr ui64 popcount(const ui64 value) noexcept {
        if (isConstantEvaluated()) {
            return impl::popcountImplS(value);
        } else {
            return impl::popcountImplD(value);
        }
    }

    template<typename FloatType>
        requires(IsBuiltInFloatingPointC<FloatType>)
    NATL_FORCE_INLINE constexpr UIntOfByteSize<sizeof(FloatType)> popcount(const FloatType& value) noexcept {
        return popcount(bitCast<UIntOfByteSize<sizeof(FloatType)>, FloatType>(value));
    }

    //bitscan forward 
    namespace impl {
        constexpr Bool bitscanForwardImplS(const ui8 value, ui8& indexDst) noexcept {
            if (value == 0) {
                return true;
            }
            ui8 n = value;
            ui8 index = 0;
            if (n >= (Limits<ui8>::max() >> (4))) {
                index += 4;
                n >>= 4;
            }
            if (n >= (Limits<ui8>::max() >> (4 + 2))) {
                index += 2;
                n >>= 2;
            }
            if (n >= (Limits<ui8>::max() >> (4 + 2 + 1))) {
                index += 1;
            }
            indexDst = index;
            return false;
        }

        constexpr Bool bitscanForwardImplS(const ui16 value, ui16& indexDst) noexcept {
            if (value == 0) {
                return true;
            }
            ui16 n = value;
            ui16 index = 0;
            if (n >= (Limits<ui16>::max() >> (8))) {
                index += 8;
                n >>= 8;
            }
            if (n >= (Limits<ui16>::max() >> (8 + 4))) {
                index += 4;
                n >>= 4;
            }
            if (n >= (Limits<ui16>::max() >> (8 + 4 + 2))) {
                index += 2;
                n >>= 2;
            }
            if (n >= (Limits<ui16>::max() >> (8 + 4 + 2 + 1))) {
                index += 1;
            }
            indexDst = index;
            return false;
        }

        constexpr Bool bitscanForwardImplS(const ui32 value, ui32& indexDst) noexcept {
            if (value == 0) {
                return true;
            }
            ui32 n = value;
            ui32 index = 0;
            if (n >= (Limits<ui32>::max() >> (16))) {
                index += 16;
                n >>= 16;
            }
            if (n >= (Limits<ui32>::max() >> (16 + 8))) {
                index += 8;
                n >>= 8;
            }
            if (n >= (Limits<ui32>::max() >> (16 + 8 + 4))) {
                index += 4;
                n >>= 4;
            }
            if (n >= (Limits<ui32>::max() >> (16 + 8 + 4 + 2))) {
                index += 2;
                n >>= 2;
            }
            if (n >= (Limits<ui32>::max() >> (16 + 8 + 4 + 2 + 1))) {
                index += 1;
            }
            indexDst = index;
            return false;
        }

        constexpr Bool bitscanForwardImplS(const ui64 value, ui64& indexDst) noexcept {
            if (value == 0) {
                return true;
            }
            ui64 n = value;
            ui64 index = 0;
            if (n >= (Limits<ui64>::max() >> 32)) {
                index += 32;
                n >>= 32;
            }
            if (n >= (Limits<ui64>::max() >> (32 + 16))) {
                index += 16;
                n >>= 16;
            }
            if (n >= (Limits<ui64>::max() >> (32 + 16 + 8))) {
                index += 8;
                n >>= 8;
            }
            if (n >= (Limits<ui64>::max() >> (32 + 16 + 8 + 4))) {
                index += 4;
                n >>= 4;
            }
            if (n >= (Limits<ui64>::max() >> (32 + 16 + 8 + 4 + 2))) {
                index += 2;
                n >>= 2;
            }
            if (n >= (Limits<ui64>::max() >> (32 + 16 + 8 + 4 + 2 + 1))) {
                index += 1;
            }
            indexDst = index;
            return false;
        }

        NATL_FORCE_INLINE constexpr Bool bitscanForwardImplS(const i8 value, i8& indexDst) noexcept {
            const ui8 unsignedValue = bitCast<ui8, i8>(value);
            if (unsignedValue == 0) { return false; }
            ui8 altIndexDst;
            bitscanForwardImplS(unsignedValue, altIndexDst);
            indexDst = static_cast<i8>(altIndexDst);
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanForwardImplS(const i16 value, i16& indexDst) noexcept {
            const ui16 unsignedValue = bitCast<ui16, i16>(value);
            if (unsignedValue == 0) { return false; }
            ui16 altIndexDst;
            bitscanForwardImplS(unsignedValue, altIndexDst);
            indexDst = static_cast<i16>(altIndexDst);
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanForwardImplS(const i32 value, i32& indexDst) noexcept {
            const ui32 unsignedValue = bitCast<ui32, i32>(value);
            if (unsignedValue == 0) { return false; }
            ui32 altIndexDst;
            bitscanForwardImplS(unsignedValue, altIndexDst);
            indexDst = static_cast<i32>(altIndexDst);
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanForwardImplS(const i64 value, i64& indexDst) noexcept {
            const ui64 unsignedValue = bitCast<ui64, i64>(value);
            if (unsignedValue == 0) { return false; }
            ui64 altIndexDst;
            bitscanForwardImplS(unsignedValue, altIndexDst);
            indexDst = static_cast<i64>(altIndexDst);
            return true;
        }

#if defined(NATL_COMPILER_MSVC) && defined(NATL_ARCHITECTURE_X86_64)
        NATL_FORCE_INLINE Bool bitscanForwardImplD(const i8 value, i8& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse(&altIndexDst, static_cast<ui32>(bitCast<ui8, i8>(value)));
            indexDst = static_cast<i8>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanForwardImplD(const i16 value, i16& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse(&altIndexDst, static_cast<ui32>(bitCast<ui16, i16>(value)));
            indexDst = static_cast<i16>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanForwardImplD(const i32 value, i32& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse(&altIndexDst, bitCast<ui32, i32>(value));
            indexDst = static_cast<i32>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanForwardImplD(const i64 value, i64& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse64(&altIndexDst, bitCast<ui64, i64>(value));
            indexDst = static_cast<i64>(altIndexDst);
            return result;
        }

        NATL_FORCE_INLINE Bool bitscanForwardImplD(const ui8 value, ui8& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse(&altIndexDst, static_cast<ui32>(value));
            indexDst = static_cast<ui8>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanForwardImplD(const ui16 value, ui16& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse(&altIndexDst, static_cast<ui32>(value));
            indexDst = static_cast<ui16>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanForwardImplD(const ui32 value, ui32& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse(&altIndexDst, value);
            indexDst = static_cast<ui32>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanForwardImplD(const ui64 value, ui64& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanReverse64(&altIndexDst, value);
            indexDst = static_cast<ui64>(altIndexDst);
            return result;
        }
#else 
        static_assert(false, "natl: no implemenation of dependent bitscan forward")
#endif
    }

    NATL_FORCE_INLINE constexpr Bool bitscanForward(const i8 value, i8& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const i16 value, i16& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const i32 value, i32& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const i64 value, i64& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const ui8 value, ui8& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const ui16 value, ui16& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const ui32 value, ui32& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const ui64 value, ui64& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanForwardImplS(value, indexDst);
        } else {
            return impl::bitscanForwardImplD(value, indexDst);
        }
    }
    template<typename FloatType>
        requires(IsBuiltInFloatingPointC<FloatType>)
    NATL_FORCE_INLINE constexpr Bool bitscanForward(const FloatType& value, UIntOfByteSize<sizeof(FloatType)> indexDst) noexcept {
        return bitscanForward(bitCast<UIntOfByteSize<sizeof(FloatType)>, FloatType>(value), indexDst);
    }

    //bitscan backword
    namespace impl {
        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const ui8 value, ui8& indexDst) noexcept {
            if (value == 0) { return false; }
            indexDst = popcount((value & ~value + 1) - 1);
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const ui16 value, ui16& indexDst) noexcept {
            if (value == 0) { return false; }
            indexDst = popcount((value & ~value + 1) - 1);
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const ui32 value, ui32& indexDst) noexcept {
            if (value == 0) { return false; }
            indexDst = popcount((value & ~value + 1) - 1);
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const ui64 value, ui64& indexDst) noexcept {
            if (value == 0) { return false; }
            indexDst = popcount((value & ~value + 1) - 1);
            return true;
        }

        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const i8 value, i8& indexDst) noexcept {
            const ui8 unsignedValue = bitCast<ui8, i8>(value);
            if (unsignedValue == 0) { return false; }
            indexDst = static_cast<i8>(popcount((unsignedValue & ~unsignedValue + 1) - 1));
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const i16 value, i16& indexDst) noexcept {
            const ui16 unsignedValue = bitCast<ui16, i16>(value);
            if (unsignedValue == 0) { return false; }
            indexDst = static_cast<i16>(popcount((unsignedValue & ~unsignedValue + 1) - 1));
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const i32 value, i32& indexDst) noexcept {
            const ui32 unsignedValue = bitCast<ui32, i32>(value);
            if (unsignedValue == 0) { return false; }
            indexDst = static_cast<i32>(popcount((unsignedValue & ~unsignedValue + 1) - 1));
            return true;
        }
        NATL_FORCE_INLINE constexpr Bool bitscanBackwardImplS(const i64 value, i64& indexDst) noexcept {
            const ui64 unsignedValue = bitCast<ui64, i64>(value);
            if (unsignedValue == 0) { return false; }
            indexDst = static_cast<i64>(popcount((unsignedValue & ~unsignedValue + 1) - 1));
            return true;
        }

#if defined(NATL_COMPILER_MSVC) && defined(NATL_ARCHITECTURE_X86_64)
        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const i8 value, i8& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward(&altIndexDst, static_cast<ui32>(bitCast<ui8, i8>(value)));
            indexDst = static_cast<i8>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const i16 value, i16& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward(&altIndexDst, static_cast<ui32>(bitCast<ui16, i16>(value)));
            indexDst = static_cast<i16>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const i32 value, i32& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward(&altIndexDst, bitCast<ui32, i32>(value));
            indexDst = static_cast<i32>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const i64 value, i64& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward64(&altIndexDst, bitCast<ui64, i64>(value));
            indexDst = static_cast<i64>(altIndexDst);
            return result;
        }

        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const ui8 value, ui8& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward(&altIndexDst, static_cast<ui32>(value));
            indexDst = static_cast<ui8>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const ui16 value, ui16& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward(&altIndexDst, static_cast<ui32>(value));
            indexDst = static_cast<ui16>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const ui32 value, ui32& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward(&altIndexDst, value);
            indexDst = static_cast<ui32>(altIndexDst);
            return result;
        }
        NATL_FORCE_INLINE Bool bitscanBackwardImplD(const ui64 value, ui64& indexDst) noexcept {
            unsigned long altIndexDst;
            const Bool result = _BitScanForward64(&altIndexDst, value);
            indexDst = static_cast<ui64>(altIndexDst);
            return result;
        }
#else 
        static_assert(false, "natl: no implemenation of dependent bitscan forward")
#endif
    }

    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const i8 value, i8& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const i16 value, i16& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const i32 value, i32& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const i64 value, i64& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const ui8 value, ui8& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const ui16 value, ui16& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const ui32 value, ui32& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const ui64 value, ui64& indexDst) noexcept {
        if (isConstantEvaluated()) {
            return impl::bitscanBackwardImplS(value, indexDst);
        } else {
            return impl::bitscanBackwardImplD(value, indexDst);
        }
    }
    template<typename FloatType>
        requires(IsBuiltInFloatingPointC<FloatType>)
    NATL_FORCE_INLINE constexpr Bool bitscanBackward(const FloatType& value, UIntOfByteSize<sizeof(FloatType)> indexDst) noexcept {
        return bitscanBackward(bitCast<UIntOfByteSize<sizeof(FloatType)>, FloatType>(value), indexDst);
    }

    template<typename NumericType>
    struct BitscanResult {
        Bool result;
        NumericType index;
    };
    template<typename FloatType>
        requires(IsBuiltInFloatingPointC<FloatType>)
    struct BitscanResult<FloatType> {
        Bool result;
        UIntOfByteSize<sizeof(FloatType)> index;
    };

    template<typename NumericType>
        requires(IsBuiltInNumericC<NumericType>)
    NATL_FORCE_INLINE constexpr BitscanResult<NumericType> bitscanForwardResult(const NumericType& value) noexcept {
        BitscanResult<NumericType> result{};
        result.result = bitscanForward(value, result.index);
        return result;
    }
    template<typename NumericType>
        requires(IsBuiltInNumericC<NumericType>)
    NATL_FORCE_INLINE constexpr BitscanResult<NumericType> bitscanBackwardResult(const NumericType& value) noexcept {
        BitscanResult<NumericType> result{};
        result.result = bitscanBackward(value, result.index);
        return result;
    }



    //bitwise
    template<typename IntegerType>
        requires(IsBuiltInIntegerC<IntegerType>)
    NATL_FORCE_INLINE constexpr IntegerType bitwiseShiftLeft(const IntegerType value, const IntegerType count) noexcept {
        return count > (sizeof(IntegerType) * 8) ? IntegerType{ 0 } : value << count;
    }
    template<typename IntegerType>
        requires(IsBuiltInIntegerC<IntegerType>)
    NATL_FORCE_INLINE constexpr IntegerType bitwiseShiftRight(const IntegerType value, const IntegerType count) noexcept {
        return count > (sizeof(IntegerType) * 8) ? IntegerType{ 0 } : value >> count;
    }

    template<typename FloatType>
        requires(IsBuiltInFloatingPointC<FloatType>)
    NATL_FORCE_INLINE constexpr FloatType bitwiseShiftLeft(const FloatType value, const UIntOfByteSize<sizeof(FloatType)> count) noexcept {
        using unsigned_integer_type = UIntOfByteSize<sizeof(FloatType)>;
        return
            count > (sizeof(unsigned_integer_type) * 8)
            ? bitCast<FloatType, unsigned_integer_type>(unsigned_integer_type{ 0 })
            : bitCast<FloatType, unsigned_integer_type>(bitCast<unsigned_integer_type, FloatType>(value) << count);
    }
    template<typename FloatType>
        requires(IsBuiltInFloatingPointC<FloatType>)
    NATL_FORCE_INLINE constexpr FloatType bitwiseShiftRight(const FloatType value, const UIntOfByteSize<sizeof(FloatType)> count) noexcept {
        using unsigned_integer_type = UIntOfByteSize<sizeof(FloatType)>;
        return
            count > (sizeof(unsigned_integer_type) * 8)
            ? bitCast<FloatType, unsigned_integer_type>(unsigned_integer_type{ 0 })
            : bitCast<FloatType, unsigned_integer_type>(bitCast<unsigned_integer_type, FloatType>(value) >> count);
    }

    template<typename NumericType>
        requires(IsBuiltInNumericC<NumericType>)
    NATL_FORCE_INLINE constexpr NumericType bitwiseAnd(const NumericType lhs, const NumericType rhs) noexcept {
        if constexpr (IsBuiltInFloatingPointC<NumericType>) {
            using unsigned_integer_type = UIntOfByteSize<sizeof(NumericType)>;
            return bitCast<NumericType, unsigned_integer_type>(
                bitCast<unsigned_integer_type, NumericType>(lhs) 
                & bitCast<unsigned_integer_type, NumericType>(rhs)
            );
        } else {
            return lhs & rhs;
        }
    }
    template<typename NumericType>
        requires(IsBuiltInNumericC<NumericType>)
    NATL_FORCE_INLINE constexpr NumericType bitwiseOr(const NumericType lhs, const NumericType rhs) noexcept {
        if constexpr (IsBuiltInFloatingPointC<NumericType>) {
            using unsigned_integer_type = UIntOfByteSize<sizeof(NumericType)>;
            return bitCast<NumericType, unsigned_integer_type>(
                bitCast<unsigned_integer_type, NumericType>(lhs)
                | bitCast<unsigned_integer_type, NumericType>(rhs)
            );
        } else {
            return lhs | rhs;
        }
    }
    template<typename NumericType>
        requires(IsBuiltInNumericC<NumericType>)
    NATL_FORCE_INLINE constexpr NumericType bitwiseXor(const NumericType lhs, const NumericType rhs) noexcept {
        if constexpr (IsBuiltInFloatingPointC<NumericType>) {
            using unsigned_integer_type = UIntOfByteSize<sizeof(NumericType)>;
            return bitCast<NumericType, unsigned_integer_type>(
                bitCast<unsigned_integer_type, NumericType>(lhs)
                ^ bitCast<unsigned_integer_type, NumericType>(rhs)
            );
        } else {
            return lhs ^ rhs;
        }
    }
    template<typename NumericType>
        requires(IsBuiltInNumericC<NumericType>)
    NATL_FORCE_INLINE constexpr NumericType bitwiseNot(const NumericType value) noexcept {
        if constexpr (IsBuiltInFloatingPointC<NumericType>) {
            using unsigned_integer_type = UIntOfByteSize<sizeof(NumericType)>;
            return bitCast<NumericType, unsigned_integer_type>(
                ~bitCast<unsigned_integer_type, NumericType>(value)
            );
        } else {
            return ~value;
        }
    }
}