#pragma once

//own
#include "typeTraits.h"
#include "allocator.h"
#include "dataMovement.h"
#include "atomic.h"
#include "functional.h"

//interface
namespace natl {
	template<class DataType>
	class Ptr {
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
	public:
		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = true;
		constexpr static bool triviallyDestructible = true;
		constexpr static bool triviallyConstRefConstructedable = true;
		constexpr static bool triviallyMoveConstructedable = true;
	private:
		DataType* dataPtr;
	public:
		//constructor 
		constexpr Ptr() noexcept : dataPtr(nullptr) {}
		constexpr Ptr(DataType* data) noexcept requires(isNotConst<DataType>) : dataPtr(&data) {};
		constexpr Ptr(const DataType* data) noexcept requires(isConst<DataType>) : dataPtr(data) {};

		//destructor
		constexpr ~Ptr() noexcept = default;

		//access
		constexpr reference unwrap() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference unwrap() const noexcept { return *dataPtr; };
		constexpr reference get() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference get() const noexcept { return *dataPtr; };
		constexpr pointer getPtr() noexcept requires(isNotConst<DataType>) { return dataPtr; };
		constexpr const_pointer getPtr() const noexcept { return dataPtr; };
		constexpr reference operator*() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference operator*() const noexcept { return *dataPtr; };
		constexpr reference operator->() noexcept requires(isNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference operator->() const noexcept { return *dataPtr; };
	};

	template<class Deleter, class DataType>
	concept IsDeleter = HasFunctionSignature<Deleter, void, DataType*>;

	template<class DataType, class Alloc = DefaultAllocator<DataType>>
	struct DefaultDeleter {
	public:

		//constructor
		constexpr DefaultDeleter() noexcept = default;

		//delete operations
		constexpr void operator()(typename Alloc::pointer ptr) const {
			std::destroy_at<DataType>(ptr);
			Alloc::deallocate(ptr, 1); 
		}
	};

	//

	template<class DataType, class Alloc = DefaultAllocator<DataType>, class Deleter = DefaultDeleter<DataType, Alloc>>
		requires(IsAllocator<Alloc> && IsDeleter<Deleter, DataType>)
	class UniquePtr {
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;

		using element_type = DataType;
		using deleter_type = Deleter;
	public:
		//movement info 
		constexpr static bool triviallyRelocatable = true && (IsEmpty<Deleter> || IsTriviallyRelocatable<Deleter>);
		constexpr static bool triviallyDefaultConstructible = true && (IsEmpty<Deleter> || IsTriviallyDefaultConstructible<Deleter>);
		constexpr static bool triviallyCompareable = true && (IsEmpty<Deleter> || IsTriviallyCompareable<Deleter>);
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;
	private:
		pointer dataPtr;
		[[no_unique_address]] deleter_type deleter;
	public:
		//constructor 
		constexpr UniquePtr() noexcept : dataPtr(nullptr), deleter() {};
		constexpr UniquePtr(const UniquePtr&) noexcept = delete;

		
		constexpr UniquePtr(UniquePtr&& other) noexcept requires(IsEmpty<Deleter>) = default;
		constexpr UniquePtr(UniquePtr&& other) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter() {
			dataPtr = other.dataPtr;
			deleter = move(deleter);
		}

		constexpr UniquePtr(std::nullptr_t) noexcept : dataPtr(nullptr), deleter() {}

		constexpr UniquePtr(pointer ptr) noexcept requires(IsNotEmpty<Deleter>) { dataPtr = ptr; }
		constexpr UniquePtr(const value_type& value) noexcept requires(IsNotEmpty<Deleter>) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(value_type&& value) noexcept requires(IsNotEmpty<Deleter>) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, forward<DataType>(value));
		}

		constexpr UniquePtr(pointer ptr, const deleter_type& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(ptr), deleter(deleter) {}
		constexpr UniquePtr(pointer ptr, deleter_type&& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(ptr), deleter(move(deleter)) {}
		constexpr UniquePtr(const value_type& value, const deleter_type& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(deleter) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(const value_type& value, deleter_type&& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(move(deleter)) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(value_type&& value, const deleter_type& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(deleter) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, forward<DataType>(value));
		}
		constexpr UniquePtr(value_type&& value, deleter_type&& deleter) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(move(deleter)) {
			dataPtr = Alloc::allocate(1);
			std::construct_at<DataType, DataType>(dataPtr, forward<DataType>(value));
		}

		//destructor
	private:
		constexpr void destruct() noexcept {
			if (dataPtr) {
				if constexpr (IsEmpty<Deleter>) {
					Deleter{}(dataPtr);
				} else {
					deleter(dataPtr);
				}
			}
		}
	public:
		constexpr ~UniquePtr() noexcept {
			destruct();
		}

		//util 
		constexpr UniquePtr& self() noexcept { return *this; }
		constexpr const UniquePtr& self() const noexcept { return *this; }

		//assignment 
		constexpr UniquePtr& operator=(const UniquePtr&) = delete;
		constexpr UniquePtr& operator=(UniquePtr&& other) noexcept {
			reset(other.release());
			if constexpr (IsNotEmpty<Deleter>) {
				deleter = move(deleter);
			}
		}
		constexpr UniquePtr& operator=(std::nullptr_t) noexcept {
			reset();
		}

		//modifiers
		constexpr pointer release() noexcept {
			pointer releasePtr = dataPtr;
			dataPtr = nullptr;
			return releasePtr;
		}
		constexpr void reset(pointer ptr = pointer()) noexcept {
			destruct();
			dataPtr = ptr;
		}
		constexpr void swap(UniquePtr& other) noexcept {
			exchange<UniquePtr>(dataPtr, other.dataPtr);
			if constexpr (IsNotEmpty<Deleter>) {
				exchange<Deleter>(deleter, deleter);
			}
		}

		//observers
		constexpr pointer get() noexcept { return dataPtr; }
		constexpr const_pointer get() const noexcept { return dataPtr; }

		constexpr Deleter& get_deleter() noexcept requires(IsNotEmpty<Deleter>) {
			return deleter;
		}
		constexpr const Deleter& get_deleter() const noexcept requires(IsNotEmpty<Deleter>) {
			return deleter;
		}

		constexpr bool empty() const noexcept { return dataPtr == nullptr; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }
		explicit constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr pointer operator*() noexcept { return dataPtr; }
		constexpr const_pointer operator*() const noexcept { return dataPtr; }
		constexpr pointer operator->() noexcept { return dataPtr; }
		constexpr const_pointer operator->() const noexcept { return dataPtr; }

		//compare
		constexpr bool operator==(const UniquePtr& other) const noexcept { return get() == other.get(); }
		constexpr bool operator!=(const UniquePtr& other) const noexcept { return get() != other.get(); }
		constexpr bool operator<(const UniquePtr& other) const noexcept { return get() < other.get(); }
		constexpr bool operator<=(const UniquePtr& other) const noexcept { return get() <= other.get(); }
		constexpr bool operator>(const UniquePtr& other) const noexcept { return get() > other.get(); }
		constexpr bool operator>=(const UniquePtr& other) const noexcept { return get() >= other.get(); }
		constexpr std::compare_three_way_result_t<pointer> operator<=>(const UniquePtr& other) { return get() <=> other.get(); };

		constexpr bool operator==(std::nullptr_t) const noexcept { return !self(); }
		constexpr bool operator!=(std::nullptr_t) const noexcept { return static_cast<bool>(self()); }

		//special
		constexpr Size hash() const noexcept {
			return static_cast<Size>(std::bit_cast<UIntOfByteSize_t<sizeof(pointer)>, pointer>(dataPtr));
		}
	};

	template<class CharT, class Traits, class DataType, class Alloc, class Deleter> 
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const UniquePtr<DataType, Alloc, Deleter>& ptr) {
		os << ptr.get();
		return os;
	}

	namespace impl {
		enum class SharedPtrControlBlockState {
			seperate = 0, 
			fused, 
			seperatePolymorphic,
			fusedPolymorphic,
		};

		struct SharedPtrControlBlockSeperatePolymorphic {
			using control_block_polymorphic_destroy_function = void(*)(SharedPtrControlBlockSeperatePolymorphic*);
			using control_block_polymorphic_ref_increment_function = void(*)(SharedPtrControlBlockSeperatePolymorphic*);
			using control_block_polymorphic_weak_ref_increment_function = void(*)(SharedPtrControlBlockSeperatePolymorphic*);
			using control_block_polymorphic_get_use_count_function = Size(*)(SharedPtrControlBlockSeperatePolymorphic*);

			constexpr virtual control_block_polymorphic_destroy_function getDestoryFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_ref_increment_function getRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_weak_ref_increment_function getWeakRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_get_use_count_function getUseCountFunction() const noexcept = 0;
		};

		template<class DataType>
		struct SharedPtrControlBlockSeperate : SharedPtrControlBlockSeperatePolymorphic {
			using control_block_polymorphic_destroy_function = typename SharedPtrControlBlockSeperatePolymorphic::control_block_polymorphic_destroy_function;
			using control_block_polymorphic_ref_increment_function = typename SharedPtrControlBlockSeperatePolymorphic::control_block_polymorphic_ref_increment_function;
			using control_block_polymorphic_weak_ref_increment_function = typename SharedPtrControlBlockSeperatePolymorphic::control_block_polymorphic_weak_ref_increment_function;
			using control_block_polymorphic_get_use_count_function = typename SharedPtrControlBlockSeperatePolymorphic::control_block_polymorphic_get_use_count_function;
			using control_block_deleter_function_type = Function<void(SharedPtrControlBlockSeperate*)>;

			DataType* dataPtr;
			Atomic<Size> useCount;
			Atomic<Size> weakCount;
			Function<void(DataType*)> deleter;
			Function<void(SharedPtrControlBlockSeperate*)> controlBlockDeleter;

			template<class Deleter, class ControlBlockDeleter>
				requires(IsDeleter<Deleter, DataType> && IsDeleter<ControlBlockDeleter, SharedPtrControlBlockSeperate>)
			constexpr SharedPtrControlBlockSeperate(DataType* dataPtr, Deleter&& deleter, ControlBlockDeleter&& controlBlockDeleter) noexcept
				: dataPtr(dataPtr), useCount(1), weakCount(), deleter(deleter), controlBlockDeleter(controlBlockDeleter) {}

			constexpr static inline void destory(SharedPtrControlBlockSeperate* controlBlockSeperate) noexcept {
				if (controlBlockSeperate->useCount.load() == 1) {
					controlBlockSeperate->deleter.invoke(controlBlockSeperate->dataPtr);

					controlBlockSeperate->useCount--;
					if (controlBlockSeperate->weakCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockSeperate->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockSeperate);
					}
				}
			}
			constexpr static inline void destoryWeak(SharedPtrControlBlockSeperate* controlBlockSeperate) noexcept {
				if (controlBlockSeperate->weakCount.load() == 1) {
					if (controlBlockSeperate->useCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockSeperate->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockSeperate);
					}
				}
				controlBlockSeperate->weakCount--;
			}

			constexpr control_block_polymorphic_destroy_function getDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					destory(controlBlockSeperate);
				};
			}
			constexpr control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					destoryWeak(controlBlockSeperate);
				};
			}
			constexpr control_block_polymorphic_ref_increment_function getRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					controlBlockSeperate->useCount++;
				};
			}
			constexpr control_block_polymorphic_ref_increment_function getWeakRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					controlBlockSeperate->weakCount++;
				};
			}
			constexpr virtual control_block_polymorphic_get_use_count_function getUseCountFunction() const noexcept {
				return  [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> Size {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					return controlBlockSeperate->useCount.load();
				};
			}
		};

		struct SharedPtrControlBlockFusedPolymorphic {
			using control_block_polymorphic_destroy_function = void(*)(SharedPtrControlBlockFusedPolymorphic*);
			using control_block_polymorphic_ref_increment_function = void(*)(SharedPtrControlBlockFusedPolymorphic*);
			using control_block_polymorphic_weak_ref_increment_function = void(*)(SharedPtrControlBlockFusedPolymorphic*);
			using control_block_polymorphic_get_use_count_function = Size(*)(SharedPtrControlBlockFusedPolymorphic*);
			constexpr virtual control_block_polymorphic_destroy_function getDestoryFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_ref_increment_function getRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_weak_ref_increment_function getWeakRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_get_use_count_function getUseCountFunction() const noexcept = 0;
		};

		template<class DataType>
		struct SharedPtrControlBlockFused : SharedPtrControlBlockFusedPolymorphic {
			using control_block_polymorphic_destroy_function = typename SharedPtrControlBlockFusedPolymorphic::control_block_polymorphic_destroy_function;
			using control_block_polymorphic_ref_increment_function = typename SharedPtrControlBlockFusedPolymorphic::control_block_polymorphic_ref_increment_function;
			using control_block_polymorphic_weak_ref_increment_function = typename SharedPtrControlBlockFusedPolymorphic::control_block_polymorphic_weak_ref_increment_function;
			using control_block_polymorphic_get_use_count_function = typename SharedPtrControlBlockFusedPolymorphic::control_block_polymorphic_get_use_count_function;
			using control_block_deleter_function_type = Function<void(SharedPtrControlBlockFused*)>;

			using pre_delete_function_type = Function<void(DataType*)>;
			using post_delete_function_type = Function<void(void)>;

			Atomic<Size> useCount;
			Atomic<Size> weakCount;
			Function<void(DataType*)> preDeleteFunction;
			Function<void(void)> postDeleteFunction;
			Function<void(SharedPtrControlBlockFused*)> controlBlockDeleter;

			union {
				DataType data;
			};

			template<class PreDeleteFunctor, class PostDeleteFunctor, class ControlBlockDeleter, class... ConstructArgTypes>
				requires(HasFunctionSignature<PreDeleteFunctor, void, DataType*> &&
						HasFunctionSignature<PostDeleteFunctor, void> && 
						IsDeleter<ControlBlockDeleter, SharedPtrControlBlockFused> &&
						std::is_constructible_v<DataType, ConstructArgTypes...>)
			constexpr SharedPtrControlBlockFused(PreDeleteFunctor&& preDeleteFunctor, 
				PostDeleteFunctor&& postDeleteFunctor, 
				ControlBlockDeleter&& controlBlockDeleter, 
				ConstructArgTypes... constructArg) noexcept : 
					useCount(1), weakCount(0),
					preDeleteFunction(forward<PreDeleteFunctor>(preDeleteFunctor)),
					postDeleteFunction(forward<PostDeleteFunctor>(postDeleteFunctor)),
					controlBlockDeleter(forward<ControlBlockDeleter>(controlBlockDeleter)),  
					data(forward<ConstructArgTypes>(constructArg)...) {}

			constexpr static inline void destory(SharedPtrControlBlockFused* controlBlockFused) noexcept {
				if (controlBlockFused->useCount.load() == 1) {
					if (controlBlockFused->preDeleteFunction.isNotEmpty()) {
						controlBlockFused->preDeleteFunction.invoke(&controlBlockFused->data);
					}

					std::destroy_at<DataType>(&controlBlockFused->data);

					if (controlBlockFused->postDeleteFunction.isNotEmpty()) {
						controlBlockFused->postDeleteFunction.invoke();
					}

					controlBlockFused->useCount--;
					if (controlBlockFused->weakCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockFused->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockFused);
					}
				}
			}
			constexpr static inline void destoryWeak(SharedPtrControlBlockFused* controlBlockFused) noexcept {
				if (controlBlockFused->weakCount.load() == 1) {
					if (controlBlockFused->useCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockFused->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockFused);
					}
				}
				controlBlockFused->weakCount--;
			}

			constexpr control_block_polymorphic_destroy_function getDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					destory(controlBlockFused);
				};
			}
			constexpr control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					destoryWeak(controlBlockFused);
				};
			}
			constexpr control_block_polymorphic_ref_increment_function getRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					controlBlockFused->useCount++;
				};
			}
			constexpr control_block_polymorphic_ref_increment_function getWeakRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					controlBlockFused->weakCount++;
				};
			}
			constexpr virtual control_block_polymorphic_get_use_count_function getUseCountFunction() const noexcept {
				return (control_block_polymorphic_get_use_count_function) [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> Size {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					return controlBlockFused->useCount.load();
				};
			}
		};
	}


	struct SharedPtrFusedConstruct {};

	template<class DataType>
	class SharedPtr {
	public:
		using element_type = DataType;
		using element_pointer = DataType*;

		using control_block_seperate = impl::SharedPtrControlBlockSeperate<DataType>;
		using control_block_seperate_polymorphic = impl::SharedPtrControlBlockSeperatePolymorphic;
		using control_block_fused = impl::SharedPtrControlBlockFused<DataType>;
		using control_block_fused_polymorphic = impl::SharedPtrControlBlockFusedPolymorphic;

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;
	private:
		element_pointer dataPtr;
		impl::SharedPtrControlBlockState controlBlockState;
		union {
			control_block_seperate* controlBlockSeperate;
			control_block_fused* controlBlockFused;
			control_block_seperate_polymorphic* controlBlockSeperatePolymorphic;
			control_block_fused_polymorphic* controlBlockFusedPolymorphic;
		};
	public:

		//weak ptr 
		template<class DataType>
		class WeakPtr {
		public:
			using element_type = DataType;
			using element_pointer = DataType*;

			using control_block_seperate = impl::SharedPtrControlBlockSeperate<DataType>;
			using control_block_seperate_polymorphic = impl::SharedPtrControlBlockSeperatePolymorphic;
			using control_block_fused = impl::SharedPtrControlBlockFused<DataType>;
			using control_block_fused_polymorphic = impl::SharedPtrControlBlockFusedPolymorphic;

			//movement info 
			constexpr static bool triviallyRelocatable = true;
			constexpr static bool triviallyDefaultConstructible = true;
			constexpr static bool triviallyCompareable = false;
			constexpr static bool triviallyDestructible = false;
			constexpr static bool triviallyConstRefConstructedable = false;
			constexpr static bool triviallyMoveConstructedable = false;

		private:
			impl::SharedPtrControlBlockState controlBlockState;
			element_pointer dataPtr;
			union {
				control_block_seperate* controlBlockSeperate;
				control_block_fused* controlBlockFused;
				control_block_seperate_polymorphic* controlBlockSeperatePolymorphic;
				control_block_fused_polymorphic* controlBlockFusedPolymorphic;
			};
		public:
			//constructor 
			constexpr WeakPtr() noexcept : dataPtr(nullptr), controlBlockState(impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}
			constexpr WeakPtr(std::nullptr_t) noexcept : dataPtr(nullptr), controlBlockState(impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}

		private:
			template<class OtherType>
			constexpr void constructCopy(const OtherType& other) noexcept {
				if (other.dataPtr == nullptr) {
					dataPtr = nullptr;
					controlBlockState = impl::SharedPtrControlBlockState::seperate;
					controlBlockSeperate = nullptr;
				} else {
					dataPtr = other.dataPtr;
					controlBlockState = other.controlBlockState;

					switch (other.controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						controlBlockSeperate = other.controlBlockSeperate;
						controlBlockSeperate->weakCount++;
						break;
					case impl::SharedPtrControlBlockState::fused:
						controlBlockFused = other.controlBlockFused;
						controlBlockFused->weakCount++;
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
						controlBlockSeperatePolymorphic->getWeakRefIncrementFunction()();
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
						controlBlockFusedPolymorphic->getWeakRefIncrementFunction()();
						break;
					default:
						break;
					}
				}
			}
			template<class OtherType>
			constexpr void constructMove(OtherType&& other) noexcept {
				if (other.dataPtr == nullptr) {
					dataPtr = nullptr;
					controlBlockState = impl::SharedPtrControlBlockState::seperate;
					controlBlockSeperate = nullptr;
				} else {
					dataPtr = other.dataPtr;
					controlBlockState = other.controlBlockState;

					switch (other.controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						controlBlockSeperate = other.controlBlockSeperate;
						break;
					case impl::SharedPtrControlBlockState::fused:
						controlBlockFused = other.controlBlockFused;
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
						break;
					default:
						break;
					}
				}

				other.dataPtr = nullptr;
			}
			template<class OtherType>
			constexpr void constructPolymorphicCopy(const OtherType& other) noexcept {
				if (other.dataPtr == nullptr) {
					dataPtr = nullptr;
					controlBlockState = impl::SharedPtrControlBlockState::seperate;
					controlBlockSeperate = nullptr;
				} else {
					dataPtr = static_cast<DataType*>(other.dataPtr);
					controlBlockState = other.controlBlockState;

					switch (other.controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
						controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
						controlBlockSeperatePolymorphic->getWeakRefIncrementFunction()();
						break;
					case impl::SharedPtrControlBlockState::fused:
						controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
						controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
						controlBlockFusedPolymorphic->getWeakRefIncrementFunction()();
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
						controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
						controlBlockSeperatePolymorphic->getWeakRefIncrementFunction()();
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
						controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
						controlBlockFusedPolymorphic->getWeakRefIncrementFunction()();
						break;
					default:
						break;
					}
				}
			}
			template<class OtherType>
			constexpr void constructPolymorphicMove(const OtherType& other) noexcept {
				if (other.dataPtr == nullptr) {
					dataPtr = nullptr;
					controlBlockState = impl::SharedPtrControlBlockState::seperate;
					controlBlockSeperate = nullptr;
				}
				else {
					dataPtr = static_cast<DataType*>(other.dataPtr);
					controlBlockState = other.controlBlockState;

					switch (other.controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
						controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
						break;
					case impl::SharedPtrControlBlockState::fused:
						controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
						controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
						controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
						controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
						break;
					default:
						break;
					}
				}

				other.dataPtr = nullptr;
			}
		public:

			constexpr WeakPtr(const WeakPtr& other) noexcept {
				constructCopy<WeakPtr>(other);
			}
			constexpr WeakPtr(WeakPtr&& other) noexcept {
				constructMove<WeakPtr>(forward<WeakPtr>(other));
			}

			template<class OtherDataType>
				requires(IsPolymorphicCastable<OtherDataType, DataType>)
			constexpr WeakPtr(const WeakPtr<OtherDataType>& other) noexcept {
				constructPolymorphicCopy<WeakPtr<OtherDataType>>(other);
			}
			template<class OtherDataType>
				requires(IsPolymorphicCastable<OtherDataType, DataType>)
			constexpr WeakPtr(WeakPtr<OtherDataType>&& other) noexcept {
				constructPolymorphicMove<WeakPtr<OtherDataType>>(forward<WeakPtr<OtherDataType>>(other));
			}

			constexpr WeakPtr(const SharedPtr<DataType>& other) noexcept {
				constructCopy<SharedPtr<DataType>>(other);
			}
			template<class OtherDataType>
				requires(IsPolymorphicCastable<OtherDataType, DataType>)
			constexpr WeakPtr(const SharedPtr<DataType>& other) noexcept {
				constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
			}
		private:
			constexpr void destruct() noexcept {
				if (dataPtr) {
					switch (controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						control_block_seperate::weakDestroy(controlBlockSeperate);
						break;
					case impl::SharedPtrControlBlockState::fused:
						control_block_fused::weakDestory(controlBlockFused);
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						controlBlockSeperatePolymorphic->getWeakDestoryFunction()(controlBlockSeperatePolymorphic);
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockFusedPolymorphic->getWeakDestoryFunction()(controlBlockFusedPolymorphic);
						break;
					default:
						break;
					}
				}
			}
		public:
			//destructor
			constexpr ~WeakPtr() noexcept {
				destruct();
			}

			//util 
			constexpr WeakPtr& self() noexcept { return *this; }
			constexpr const WeakPtr& self() const noexcept { return *this; }


			//assignment
			constexpr WeakPtr& operator=(const WeakPtr& other) noexcept {
				destruct();
				constructCopy<WeakPtr>(other);
				return self();
			}
			constexpr WeakPtr& operator=(WeakPtr&& other) noexcept {
				destruct();
				constructMove<WeakPtr>(forward<WeakPtr>(other));
				return self();
			}

			template<class OtherDataType>
				requires(IsPolymorphicCastable<OtherDataType, DataType>)
			constexpr WeakPtr& operator=(const WeakPtr<OtherDataType>& other) noexcept {
				destruct();
				constructPolymorphicCopy<WeakPtr<OtherDataType>>(other);
				return self();
			}
			template<class OtherDataType>
				requires(IsPolymorphicCastable<OtherDataType, DataType>)
			constexpr WeakPtr& operator=(WeakPtr<OtherDataType>&& other) noexcept {
				destruct();
				constructPolymorphicMove<WeakPtr<OtherDataType>>(forward<WeakPtr<OtherDataType>>(other));
				return self();
			}

			constexpr WeakPtr& operator=(const SharedPtr<DataType>& other) noexcept {
				destruct();
				constructCopy<SharedPtr<DataType>>(other);
				return self();
			}
			template<class OtherDataType>
				requires(IsPolymorphicCastable<OtherDataType, DataType>)
			constexpr WeakPtr& operator=(const SharedPtr<DataType>& other) noexcept {
				destruct();
				constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
				return self();
			}

			//modifiers
			constexpr void reset() noexcept {
				destruct();
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			}

			constexpr void swap(WeakPtr& other) noexcept {
				impl::SharedPtrControlBlockState tempState = other.controlBlockState;
				other.controlBlockState = controlBlockState;
				controlBlockState = tempState;

				DataType* tempDataPtr = other.dataPtr;
				other.dataPtr = dataPtr;
				dataPtr = tempDataPtr;

				switch (controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate: {
					control_block_seperate* controlBlockSeperateTemp = other.controlBlockSeperate;
					other.controlBlockSeperate = controlBlockSeperate;
					controlBlockSeperate = controlBlockSeperateTemp;
					break;
				}
				case impl::SharedPtrControlBlockState::fused: {
					control_block_fused* controlBlockFusedTemp = other.controlBlockFused;
					other.controlBlockFused = controlBlockFused;
					controlBlockFused = controlBlockFusedTemp;
					break;
				}
				case impl::SharedPtrControlBlockState::seperatePolymorphic: {
					control_block_seperate_polymorphic* controlBlockSeperatePolymorphicTemp = other.controlBlockSeperatePolymorphic;
					other.controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphic;
					controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphicTemp;
					break;
				}
				case impl::SharedPtrControlBlockState::fusedPolymorphic: {
					control_block_fused_polymorphic* controlBlockFusedPolymorphicTemp = other.controlBlockFusedPolymorphic;
					other.controlBlockFusedPolymorphic = controlBlockFusedPolymorphic;
					controlBlockFusedPolymorphic = controlBlockFusedPolymorphicTemp;
					break;
				}
				default:
					break;
				}
			}

			//observers
			constexpr Size use_count() const noexcept {
				if (dataPtr) {
					switch (controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						return controlBlockSeperate->useCount.load();
					case impl::SharedPtrControlBlockState::fused:
						return controlBlockFused->useCount.load();
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						return controlBlockSeperatePolymorphic->getUseCountFunction()(controlBlockSeperatePolymorphic);
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						return controlBlockFusedPolymorphic->getUseCountFunction()(controlBlockFusedPolymorphic);
					default:
						break;
					}
				}
				return 0;
			}

			constexpr bool expired() const noexcept {
				return use_count() == 0;
			}
			constexpr SharedPtr lock() const noexcept {
				return expired() ? SharedPtr() : SharedPtr(self());
			}

		}; 

		using weak_type = WeakPtr<DataType>;


	private:
		template<class OtherType>
		constexpr void constructCopy(const OtherType& other) noexcept {
			if (other.dataPtr == nullptr) {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			} else {
				dataPtr = other.dataPtr;
				controlBlockState = other.controlBlockState;

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockSeperate = other.controlBlockSeperate;
					controlBlockSeperate->useCount++;
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockFused = other.controlBlockFused;
					controlBlockFused->useCount++;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					controlBlockSeperatePolymorphic->getRefIncrementFunction()();
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					controlBlockFusedPolymorphic->getRefIncrementFunction()();
					break;
				default:
					break;
				}
			}
		}
		template<class OtherType>
		constexpr void constructMove(OtherType&& other) noexcept {
			if (other.dataPtr == nullptr) {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			} else {
				dataPtr = other.dataPtr;
				controlBlockState = other.controlBlockState;

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockSeperate = other.controlBlockSeperate;
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockFused = other.controlBlockFused;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					break;
				default:
					break;
				}
			}

			other.dataPtr = nullptr;
		}
		template<class OtherType>
		constexpr void constructPolymorphicCopy(const OtherType& other) noexcept {
			if (other.dataPtr == nullptr) {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			} else {
				dataPtr = static_cast<DataType*>(other.dataPtr);
				controlBlockState = other.controlBlockState;

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
					controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
					controlBlockSeperatePolymorphic->getRefIncrementFunction()();
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
					controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
					controlBlockFusedPolymorphic->getRefIncrementFunction()();
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					controlBlockSeperatePolymorphic->getRefIncrementFunction()();
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					controlBlockFusedPolymorphic->getRefIncrementFunction()();
					break;
				default:
					break;
				}
			}
		}
		template<class OtherType>
		constexpr void constructPolymorphicMove(const OtherType& other) noexcept {
			if (other.dataPtr == nullptr) {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			} else {
				dataPtr = static_cast<DataType*>(other.dataPtr);
				controlBlockState = other.controlBlockState;

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
					controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
					controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					break;
				default:
					break;
				}
			}

			other.dataPtr = nullptr;
		}


		//constructor
		constexpr SharedPtr() noexcept : dataPtr(nullptr), controlBlockState(impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}
		constexpr SharedPtr(std::nullptr_t) noexcept : dataPtr(nullptr), controlBlockState(impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}

		constexpr SharedPtr(const SharedPtr& other) noexcept {
			constructCopy<SharedPtr>(other);
		}
		constexpr SharedPtr(SharedPtr&& other) noexcept {
			constructCopy<SharedPtr>(forward<SharedPtr>(other));
		}

		template<class OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(const SharedPtr<OtherDataType>& other) noexcept {
			constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
		}
		template<class OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(SharedPtr<OtherDataType>&& other) noexcept {
			constructPolymorphicMove<SharedPtr<OtherDataType>>(forward<SharedPtr<OtherDataType>>(other));
		}

		constexpr SharedPtr(const WeakPtr<DataType>& other) noexcept {
			constructCopy<WeakPtr<DataType>>(other);
		}
		template<class OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(const WeakPtr<OtherDataType>& other) noexcept {
			constructCopy<WeakPtr<OtherDataType>>(other);
		}

	private:
		template<class Alloc>
			requires(IsAllocator<Alloc>)
		constexpr void construct(DataType* ptr) noexcept {
			dataPtr = ptr;
			controlBlockState = impl::SharedPtrControlBlockState::seperate;
			using control_block_seperate_alloc = Alloc::template rebind_alloc<control_block_seperate>;
			controlBlockSeperate = control_block_seperate_alloc::allocate(1);
			std::construct_at<control_block_seperate>(controlBlockSeperate, dataPtr, DefaultDeleter<DataType, Alloc>(), DefaultDeleter<control_block_seperate, control_block_seperate_alloc>());
		}

		template<class Alloc, class Deleter>
			requires(IsAllocator<Alloc>&& IsDeleter<Deleter, DataType>)
		constexpr void construct(DataType* ptr, Deleter&& deleter) noexcept {
			dataPtr = ptr;
			controlBlockState = impl::SharedPtrControlBlockState::seperate;
			using control_block_seperate_alloc = Alloc::template rebind_alloc<control_block_seperate>;
			controlBlockSeperate = control_block_seperate_alloc::allocate(1);
			std::construct_at<control_block_seperate>(controlBlockSeperate, dataPtr, deleter, DefaultDeleter<control_block_seperate, control_block_seperate_alloc>());
		}
		template<class OtherDataType, class Alloc>
			requires(IsAllocator<Alloc> && IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr void construct(OtherDataType* ptr) noexcept {
			dataPtr = static_cast<OtherDataType*>(ptr);
			controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
			using other_control_block_type = impl::SharedPtrControlBlockSeperate<OtherDataType>;
			using other_control_block_type_alloc = Alloc::template rebind_alloc<other_control_block_type>;
			other_control_block_type* otherControlBlockSeperate = other_control_block_type_alloc::allocate(1);
			std::construct_at<other_control_block_type>(otherControlBlockSeperate, dataPtr, DefaultDeleter<DataType, Alloc>(), DefaultDeleter<other_control_block_type, other_control_block_type_alloc>());
			controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic>(otherControlBlockSeperate);
		}

		template<class OtherDataType, class Alloc, class Deleter>
			requires(IsAllocator<Alloc>&& IsDeleter<Deleter, DataType>)
		constexpr void construct(OtherDataType* ptr, Deleter&& deleter) noexcept {
			dataPtr = static_cast<OtherDataType*>(ptr);
			controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
			using other_control_block_type = impl::SharedPtrControlBlockSeperate<OtherDataType>;
			using other_control_block_type_alloc = Alloc::template rebind_alloc<other_control_block_type>;
			other_control_block_type* otherControlBlockSeperate = other_control_block_type_alloc::allocate(1);
			std::construct_at<other_control_block_type>(otherControlBlockSeperate, dataPtr, deleter, DefaultDeleter<other_control_block_type, other_control_block_type_alloc>());
			controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic>(otherControlBlockSeperate);
		}

		template<class Deleter>
		constexpr void construct(UniquePtr<DataType, Deleter>&& other) {
			if (other.isEmpty()) {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			} else {
				if (IsEmpty<Deleter>) {
					Deleter deleter = Deleter();
					construct<DefaultAllocatorByte, Deleter>(other, move<Deleter>(deleter));
				} else {
					construct<DefaultAllocatorByte, Deleter>(other, move<Deleter>(other.get_deleter()));
				}
			}
		}
		template<class OtherDataType, class Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr void construct(UniquePtr<DataType, Deleter>&& other) {
			if (other.isEmpty()) {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			} else {
				if (IsEmpty<Deleter>) {
					Deleter deleter = Deleter();
					construct<OtherDataType, DefaultAllocatorByte, Deleter>(other, move<Deleter>(deleter));
				} else {
					construct<OtherDataType, DefaultAllocatorByte, Deleter>(other, move<Deleter>(other.get_deleter()));
				}
			}
		}
	public:

		template<class Alloc>
			requires(IsAllocator<Alloc>)
		explicit constexpr SharedPtr(DataType* ptr, Alloc) noexcept {
			construct<Alloc>(ptr);
		}

		template<class Alloc, class Deleter>
			requires(IsAllocator<Alloc> && IsDeleter<Deleter, DataType>)
		constexpr SharedPtr(DataType* ptr, Deleter&& deleter, Alloc) noexcept {
			construct<DataType, Deleter>(ptr, forward<Deleter>(deleter));
		}

		template<class Alloc, class... ConstructArgTypes>
			requires(IsAllocator<Alloc> && std::is_constructible_v<DataType, ConstructArgTypes...>)
		constexpr SharedPtr(SharedPtrFusedConstruct, Alloc, ConstructArgTypes... constructArg) noexcept {
			controlBlockState = impl::SharedPtrControlBlockState::fused;

			using control_block_fused_alloc = Alloc::template rebind_alloc<control_block_fused>;
			controlBlockFused = control_block_fused_alloc::allocate(1);

			std::construct_at<control_block_fused>(controlBlockFused,
				typename control_block_fused::pre_delete_function_type(),
				typename control_block_fused::post_delete_function_type(),
				DefaultDeleter<control_block_fused, control_block_fused_alloc>(),
				natl::forward<ConstructArgTypes>(constructArg)...);

			dataPtr = &controlBlockFused->data;
		}	

		template<class Deleter>
		constexpr SharedPtr(UniquePtr<DataType, Deleter>&& other) {
			construct<Deleter>(forward<UniquePtr<DataType, Deleter>>(other));
		}
		template<class OtherDataType, class Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(UniquePtr<OtherDataType, Deleter>&& other) {
			construct<OtherDataType, Deleter>(forward<UniquePtr<OtherDataType, Deleter>>(other));
		}

	private:
		constexpr void destruct() noexcept {
			if (dataPtr) {
				switch (controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					control_block_seperate::destory(controlBlockSeperate);
					break;
				case impl::SharedPtrControlBlockState::fused:
					control_block_fused::destory(controlBlockFused);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic->getDestoryFunction()(controlBlockSeperatePolymorphic);
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic->getDestoryFunction()(controlBlockFusedPolymorphic);
					break;
				default:
					break;
				}
			}
		}
	public:

		//destructor
		constexpr ~SharedPtr() noexcept {
			destruct();
		}

		//util 
		constexpr SharedPtr& self() noexcept { return *this; }
		constexpr const SharedPtr& self() const noexcept { return *this; }

		//assignment 
		constexpr SharedPtr& operator=(const SharedPtr& other) noexcept {
			destruct();
			constructCopy<SharedPtr>(other);
			return self();
		}
		constexpr SharedPtr& operator=(SharedPtr&& other) noexcept {
			destruct();
			constructMove<SharedPtr>(other);
			return self();
		}

		template<class OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr& operator=(const SharedPtr<OtherDataType>& other) noexcept {
			destruct();
			constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
			return self();
		}
		template<class OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr& operator=(SharedPtr<OtherDataType>&& other) noexcept {
			destruct();
			constructPolymorphicMove<SharedPtr<OtherDataType>>(forward<SharedPtr<OtherDataType>>(other));
			return self();
		}

		template<class Deleter>
		constexpr SharedPtr& operator=(UniquePtr<DataType, Deleter>&& other) {
			destruct();
			construct<Deleter>(forward<UniquePtr<DataType, Deleter>>(other));
			return self();
		}
		template<class OtherDataType, class Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr& operator=(UniquePtr<OtherDataType, Deleter>&& other) {
			destruct();
			construct<OtherDataType, Deleter>(forward<UniquePtr<OtherDataType, Deleter>>(other));
			return self();
		}

		//modifiers
		constexpr void reset() noexcept {
			destruct();
			dataPtr = nullptr;
			controlBlockState = impl::SharedPtrControlBlockState::seperate;
			controlBlockSeperate = nullptr;
		}
		constexpr void reset(DataType* ptr) noexcept {
			destruct();
			construct<DefaultAllocatorByte>(ptr);
		}
		template<class OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr void reset(OtherDataType* ptr) noexcept {
			destruct();
			construct<OtherDataType, DefaultAllocatorByte>(ptr);
		}

		template<class OtherDataType, class Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType> && IsDeleter<Deleter>)
		constexpr void reset(OtherDataType* ptr, Deleter&& deleter) {
			destruct();
			construct<OtherDataType, Deleter, DefaultAllocatorByte>(ptr, forward<Deleter>(deleter));
		}

		constexpr void swap(SharedPtr& other) noexcept {
			impl::SharedPtrControlBlockState tempState = other.controlBlockState;
			other.controlBlockState = controlBlockState;
			controlBlockState = tempState;

			DataType* tempDataPtr = other.dataPtr;
			other.dataPtr = dataPtr;
			dataPtr = tempDataPtr;

			switch (controlBlockState) {
			case impl::SharedPtrControlBlockState::seperate: {
				control_block_seperate* controlBlockSeperateTemp = other.controlBlockSeperate;
				other.controlBlockSeperate = controlBlockSeperate;
				controlBlockSeperate = controlBlockSeperateTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::fused: {
				control_block_fused* controlBlockFusedTemp = other.controlBlockFused;
				other.controlBlockFused = controlBlockFused;
				controlBlockFused = controlBlockFusedTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::seperatePolymorphic: {
				control_block_seperate_polymorphic* controlBlockSeperatePolymorphicTemp = other.controlBlockSeperatePolymorphic;
				other.controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphic;
				controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphicTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::fusedPolymorphic: {
				control_block_fused_polymorphic* controlBlockFusedPolymorphicTemp = other.controlBlockFusedPolymorphic;
				other.controlBlockFusedPolymorphic = controlBlockFusedPolymorphic;
				controlBlockFusedPolymorphic = controlBlockFusedPolymorphicTemp;
				break;
			}
			default:
				break;
			}
		}

		//observers
		constexpr element_type* get() noexcept {
			return dataPtr;
		}
		constexpr const element_type* get() const noexcept {
			return dataPtr;
		}
		constexpr DataType& operator*() noexcept {
			return *get();
		}
		constexpr const DataType& operator*() const noexcept {
			return *get();
		}
		constexpr DataType* operator->() noexcept {
			return get();
		}
		constexpr const DataType* operator->() const noexcept {
			return get();
		}

		constexpr Size use_count() const noexcept {
			if (dataPtr) {
				switch (controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					return controlBlockSeperate->useCount.load();
				case impl::SharedPtrControlBlockState::fused:
					return controlBlockFused->useCount.load();
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					return controlBlockSeperatePolymorphic->getUseCountFunction()(controlBlockSeperatePolymorphic);
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					return controlBlockFusedPolymorphic->getUseCountFunction()(controlBlockFusedPolymorphic);
				default:
					break;
				}
			}
			return 0;
		}

		constexpr bool empty() const noexcept { return dataPtr == nullptr; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }
		explicit constexpr operator bool() const noexcept { return isNotEmpty(); }
	};
}