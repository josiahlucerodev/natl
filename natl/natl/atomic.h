#pragma once

//std
#include <atomic>
#include <memory>

//own
#include "basicTypes.h"

//interface 
namespace natl {
	enum class MemoryOrder {
		relaxed, consume, acquire, release, acq_rel, seq_cst
	};
	inline constexpr MemoryOrder memory_order_relaxed = MemoryOrder::relaxed;
	inline constexpr MemoryOrder memory_order_consume = MemoryOrder::consume;
	inline constexpr MemoryOrder memory_order_acquire = MemoryOrder::acquire;
	inline constexpr MemoryOrder memory_order_release = MemoryOrder::release;
	inline constexpr MemoryOrder memory_order_acq_rel = MemoryOrder::acq_rel;
	inline constexpr MemoryOrder memory_order_seq_cst = MemoryOrder::seq_cst;

	constexpr std::memory_order convertToStdMemoryOrder(const MemoryOrder memoryOrder) {
		switch (memoryOrder) {
			case MemoryOrder::relaxed:
				return std::memory_order::relaxed;
			case MemoryOrder::consume:
				return std::memory_order::consume;
			case MemoryOrder::acquire:
				return std::memory_order::acquire;
			case MemoryOrder::release:
				return std::memory_order::release;
			case MemoryOrder::acq_rel:
				return std::memory_order::acq_rel;
			case MemoryOrder::seq_cst:
				return std::memory_order::seq_cst;
		}
		return std::memory_order::seq_cst;
	}


#ifdef _MSC_VER
#pragma warning(disable: 6031)
#endif //_MSC_VER

	namespace impl {

		template<class DataType>
			requires(
				std::is_trivially_copyable_v<DataType> &&  
				std::is_copy_constructible_v<DataType> && 
				std::is_move_constructible_v<DataType> &&
				std::is_copy_assignable_v<DataType> &&
				std::is_move_assignable_v<DataType>
			)
		class AtomicBase {
		public:
			using value_type = DataType;
			using atomic_value_type = std::atomic<DataType>;

			static constexpr bool is_always_lock_free = atomic_value_type::is_always_lock_free;
		protected:
			union {
				value_type value;
				atomic_value_type atomicValue;
			};
		public:
			//constructor 
			constexpr AtomicBase(const AtomicBase&) noexcept = delete;
			constexpr AtomicBase() noexcept {
				if (std::is_constant_evaluated()) {
					std::construct_at<value_type>(&value);
				} else {
					std::construct_at<atomic_value_type>(&atomicValue);
				}
			}
			constexpr AtomicBase(value_type desired) noexcept {
				if (std::is_constant_evaluated()) {
					std::construct_at<value_type>(&value, desired);
				} else {
					std::construct_at<atomic_value_type>(&atomicValue, desired);
				}
			}
			//destructor 
			constexpr ~AtomicBase() noexcept {
				if (std::is_constant_evaluated()) {
					std::destroy_at<value_type>(&value);
				} else {
					std::destroy_at<atomic_value_type>(&atomicValue);
				}
			}

			//util
			constexpr AtomicBase& self() noexcept { return *this; }
			constexpr const AtomicBase& self() const noexcept { return *this; }

		public:
			constexpr value_type& getValue() noexcept { return value; }
			constexpr const value_type& getValue() const noexcept { return value; }
			constexpr atomic_value_type& getAtomicValue() noexcept { return atomicValue; }
			constexpr const atomic_value_type& getAtomicValue() const noexcept { return atomicValue; }

			//assignment 
			constexpr AtomicBase& operator=(const AtomicBase&) = delete;
			constexpr AtomicBase& operator=(const AtomicBase&) volatile = delete;

			constexpr bool is_lock_free() const noexcept {
				if (std::is_constant_evaluated()) {
					return false;
				} else {
					return atomicValue.is_lock_free();
				}
			}
			constexpr bool is_lock_free() const volatile  noexcept {
				if (std::is_constant_evaluated()) {
					return false;
				} else {
					return atomicValue.is_lock_free();
				}
			}

			constexpr void store(value_type desired, MemoryOrder order = memory_order_seq_cst) noexcept {
				if (std::is_constant_evaluated()) {
					value = desired;
				} else {
					atomicValue.store(desired, convertToStdMemoryOrder(order));
				}
			}
			constexpr void store(value_type desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
				if (std::is_constant_evaluated()) {
					value = desired;
				} else {
					atomicValue.store(desired, convertToStdMemoryOrder(order));
				}
			}

			constexpr value_type load(MemoryOrder order = memory_order_seq_cst) const noexcept {
				if (std::is_constant_evaluated()) {
					return value;
				} else {
					return atomicValue.load(convertToStdMemoryOrder(order));
				}
			}
			constexpr value_type load(MemoryOrder order = memory_order_seq_cst) const volatile noexcept {
				if (std::is_constant_evaluated()) {
					return value;
				} else {
					return atomicValue.load(convertToStdMemoryOrder(order));
				}
			}

			constexpr value_type exchange(value_type desired, MemoryOrder order = memory_order_seq_cst) noexcept {
				if (std::is_constant_evaluated()) {
					value_type original = value;
					value = desired;
					return original;
				} else {
					return atomicValue.exchange(desired, convertToStdMemoryOrder(order));
				}
			}
			constexpr value_type exchange(value_type desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
				if (std::is_constant_evaluated()) {
					value_type original = value;
					value = desired;
					return original;
				} else {
					return atomicValue.exchange(desired, convertToStdMemoryOrder(order));
				}
			}
			constexpr bool compare_exchange_weak(value_type& expected, value_type desired, MemoryOrder success, MemoryOrder failure) noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_weak(expected, desired, convertToStdMemoryOrder(success), convertToStdMemoryOrder(failure));
				}
			}
			constexpr bool compare_exchange_weak(value_type& expected, value_type desired, MemoryOrder success, MemoryOrder failure) volatile noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_weak(expected, desired, convertToStdMemoryOrder(success), convertToStdMemoryOrder(failure));
				}
			}
			constexpr bool compare_exchange_weak(value_type& expected, value_type desired, MemoryOrder order = memory_order_seq_cst) noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_weak(expected, desired, convertToStdMemoryOrder(order));
				}
			}
			constexpr bool compare_exchange_weak(value_type& expected, value_type desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_weak(expected, desired, convertToStdMemoryOrder(order));
				}
			}

			constexpr bool compare_exchange_strong(value_type& expected, value_type desired, MemoryOrder success, MemoryOrder failure) noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_strong(expected, desired, success, failure);
				}
			}
			constexpr bool compare_exchange_strong(value_type& expected, value_type desired, MemoryOrder success, MemoryOrder failure) volatile noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_strong(expected, desired, convertToStdMemoryOrder(success), convertToStdMemoryOrder(failure));
				}
			}
			constexpr bool compare_exchange_strong(value_type& expected, value_type desired, MemoryOrder order = memory_order_seq_cst) noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_strong(expected, desired, convertToStdMemoryOrder(order));
				}
			}
			constexpr bool compare_exchange_strong(value_type& expected, value_type desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
				if (std::is_constant_evaluated()) {
					if (value == expected) {
						value = desired;
						return true;
					} else {
						expected = value;
						return false;
					}
				} else {
					return atomicValue.compare_exchange_strong(expected, desired, convertToStdMemoryOrder(order));
				}
			}

			//operator 
			constexpr operator value_type() const noexcept {
				return load();
			}
			constexpr operator value_type() const volatile noexcept {
				return load();
			}
		};
	}

#ifdef _MSC_VER
#pragma warning(default: 6031)
#endif

	template<class DataType>
	struct Atomic : public impl::AtomicBase<DataType> {
	public:
		using impl::AtomicBase<DataType>::AtomicBase;
	};

	template<class DataType>
		requires(std::is_integral_v<DataType>)
	struct Atomic<DataType> : public impl::AtomicBase<DataType> {
	public:
		using value_type = DataType;
		using atomic_value_type = std::atomic<DataType>;

		static constexpr bool is_always_lock_free = atomic_value_type::is_always_lock_free;
	public:
		using impl::AtomicBase<DataType>::AtomicBase;

		//util
		constexpr Atomic& self() noexcept { return *this; }
		constexpr const Atomic& self() const noexcept { return *this; }

		constexpr value_type fetch_add(value_type arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value += arg;
				return original;
			} else {
				return this->atomicValue.fetch_add(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_add(value_type arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value += arg;
				return original;
			} else {
				return this->atomicValue.fetch_add(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_sub(value_type arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value -= arg;
				return original;
			} else {
				return this->atomicValue.fetch_sub(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_sub(value_type arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value -= arg;
				return original;
			} else {
				return this->atomicValue.fetch_sub(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_and(value_type arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value &= arg;
				return original;
			} else {
				return this->atomicValue.fetch_and(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_and(value_type arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value &= arg;
				return original;
			} else {
				return this->atomicValue.fetch_and(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_or(value_type arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value |= arg;
				return original;
			} else {
				return this->atomicValue.fetch_or(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_or(value_type arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value |= arg;
				return original;
			} else {
				return this->atomicValue.fetch_or(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_xor(value_type arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value ^= arg;
				return original;
			} else {
				return this->atomicValue.fetch_xor(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_xor(value_type arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value ^= arg;
				return original;
			} else {
				return this->atomicValue.fetch_xor(arg, convertToStdMemoryOrder(order));
			}
		}

		//operators 
		constexpr operator value_type() const noexcept {
			return this->load();
		}
		constexpr operator value_type() const volatile noexcept {
			return this->load();
		}

		constexpr value_type operator++() noexcept {
			return fetch_add(value_type(1)) + value_type(1);
		}
		constexpr value_type operator++() volatile noexcept {
			return fetch_add(value_type(1)) + value_type(1);
		}
		constexpr value_type operator++(int) noexcept {
			return fetch_add(value_type(1));
		}
		constexpr value_type operator++(int) volatile noexcept {
			return fetch_add(value_type(1));
		}

		constexpr value_type operator--() noexcept {
			return fetch_sub(value_type(1)) - value_type(1);
		}
		constexpr value_type operator--() volatile noexcept {
			return fetch_sub(value_type(1)) - value_type(1);
		}
		constexpr value_type operator--(int) noexcept {
			return fetch_sub(value_type(1));
		}
		constexpr value_type operator--(int) volatile noexcept {
			return fetch_sub(value_type(1));
		}

		constexpr value_type operator+=(value_type arg) noexcept {
			return fetch_add(arg) + arg;
		}
		constexpr value_type operator+=(value_type arg) volatile noexcept {
			return fetch_add(arg) + arg;
		}

		constexpr value_type operator-=(value_type arg) noexcept {
			return fetch_sub(arg) - arg;
		}
		constexpr value_type operator-=(value_type arg) volatile noexcept {
			return fetch_sub(arg) - arg;
		}

		constexpr value_type operator&=(value_type arg) noexcept {
			return fetch_and(arg) & arg;
		}
		constexpr value_type operator&=(value_type arg) volatile noexcept {
			return fetch_and(arg) & arg;
		}

		constexpr value_type operator|=(value_type arg) noexcept {
			return fetch_or(arg) | arg;
		}
		constexpr value_type operator|=(value_type arg) volatile noexcept {
			return fetch_or(arg) | arg;
		}

		constexpr value_type operator^=(value_type arg) noexcept {
			return fetch_xor(arg) ^ arg;
		}
		constexpr value_type operator^=(value_type arg) volatile noexcept {
			return fetch_xor(arg) ^ arg;
		}
	};

	template<class DataType>
		requires(std::is_floating_point_v<DataType>)
	struct Atomic<DataType> : public impl::AtomicBase<DataType> {
	public:
		using value_type = DataType;
		using atomic_value_type = std::atomic<DataType>;

		static constexpr bool is_always_lock_free = atomic_value_type::is_always_lock_free;
	public:
		using impl::AtomicBase<DataType>::AtomicBase;

		//util
		constexpr Atomic& self() noexcept { return *this; }
		constexpr const Atomic& self() const noexcept { return *this; }

		constexpr value_type fetch_add(value_type arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value += arg;
				return original;
			} else {
				return this->atomicValue.fetch_add(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_add(value_type arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value += arg;
				return original;
			} else {
				return this->atomicValue.fetch_add(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_sub(value_type arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value -= arg;
				return original;
			} else {
				return this->atomicValue.fetch_sub(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type fetch_sub(value_type arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type original = this->value;
				this->value -= arg;
				return original;
			} else {
				return this->atomicValue.fetch_sub(arg, convertToStdMemoryOrder(order));
			}
		}

		//operators
		constexpr operator value_type() const noexcept {
			return this->load();
		}
		constexpr operator value_type() const volatile noexcept {
			return this->load();
		}

		constexpr value_type operator+=(value_type arg) noexcept {
			return fetch_add(arg) + arg;
		}
		constexpr value_type operator+=(value_type arg) volatile noexcept {
			return fetch_add(arg) + arg;
		}

		constexpr value_type operator-=(value_type arg) noexcept {
			return fetch_sub(arg) - arg;
		}
		constexpr value_type operator-=(value_type arg) volatile noexcept {
			return fetch_sub(arg) - arg;
		}
	};



	template<class DataType>
	struct Atomic<DataType*> : public impl::AtomicBase<DataType> {
	public:
		using value_type = DataType;
		using atomic_value_type = std::atomic<DataType>;

		static constexpr bool is_always_lock_free = atomic_value_type::is_always_lock_free;
	public:
		using impl::AtomicBase<DataType>::AtomicBase;

		//util
		constexpr Atomic& self() noexcept { return *this; }
		constexpr const Atomic& self() const noexcept { return *this; }

		constexpr value_type* fetch_add(PtrDiff arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type* original = this->value;
				this->value += arg;
				return original;
			} else {
				return this->atomicValue.fetch_add(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type* fetch_add(PtrDiff arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type* original = this->value;
				this->value += arg;
				return original;
			}
			else {
				return this->atomicValue.fetch_add(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type* fetch_sub(PtrDiff arg, MemoryOrder order = memory_order_seq_cst) noexcept {
			if (std::is_constant_evaluated()) {
				value_type* original = this->value;
				this->value -= arg;
				return original;
			} else {
				return this->atomicValue.fetch_sub(arg, convertToStdMemoryOrder(order));
			}
		}
		constexpr value_type* fetch_sub(PtrDiff arg, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
			if (std::is_constant_evaluated()) {
				value_type* original = this->value;
				this->value -= arg;
				return original;
			} else {
				return this->atomicValue.fetch_sub(arg, convertToStdMemoryOrder(order));
			}
		}

		//operators 
		constexpr operator value_type*() const noexcept {
			return this->load();
		}
		constexpr operator value_type*() const volatile noexcept {
			return this->load();
		}

		constexpr value_type* operator++() noexcept {
			return fetch_add(PtrDiff(1)) + PtrDiff(1);
		}
		constexpr value_type* operator++() volatile noexcept {
			return fetch_add(PtrDiff(1)) + PtrDiff(1);
		}
		constexpr value_type* operator++(int) noexcept {
			return fetch_add(PtrDiff(1));
		}
		constexpr value_type* operator++(int) volatile noexcept {
			return fetch_add(PtrDiff(1));
		}

		constexpr value_type* operator--() noexcept {
			return fetch_sub(PtrDiff(1)) - PtrDiff(1);
		}
		constexpr value_type* operator--() volatile noexcept {
			return fetch_sub(PtrDiff(1)) - PtrDiff(1);
		}
		constexpr value_type* operator--(int) noexcept {
			return fetch_sub(PtrDiff(1));
		}
		constexpr value_type* operator--(int) volatile noexcept {
			return fetch_sub(PtrDiff(1));
		}

		constexpr value_type* operator+=(PtrDiff arg) noexcept {
			return fetch_add(arg) + arg;
		}
		constexpr value_type* operator+=(PtrDiff arg) volatile noexcept {
			return fetch_add(arg) + arg;
		}

		constexpr value_type* operator-=(PtrDiff arg) noexcept {
			return fetch_sub(arg) - arg;
		}
		constexpr value_type* operator-=(PtrDiff arg) volatile noexcept {
			return fetch_sub(arg) - arg;
		}
	};
	using AtomicBool = Atomic<bool>;

	using AtomicI8 = Atomic<i8>;
	using AtomicI16 = Atomic<i16>;
	using AtomicI32 = Atomic<i32>;
	using AtomicI64 = Atomic<i64>;

	using AtomicUi8 = Atomic<ui8>;
	using AtomicUi16 = Atomic<ui16>;
	using AtomicUi32 = Atomic<ui32>;
	using AtomicUi64 = Atomic<ui64>;

	using AtomicF32 = Atomic<f32>;
	using AtomicF64 = Atomic<f64>;

	using AtomicSize = Atomic<Size>;
	using AtomicPtrDiff = Atomic<PtrDiff>;

	using AtomicChar = Atomic<Char>;
	using AtomicAssciCode = Atomic<AssciCode>;
	using AtomicUtf32 = Atomic<Utf32>;
}