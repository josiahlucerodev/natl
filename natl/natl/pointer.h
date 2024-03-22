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
			return std::bit_cast<Size, pointer>(dataPtr);
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
			seperatePolymorphicConstexpr,
		};

		struct SharedPtrControlBlockSeperatePolymorphic {
			using control_block_polymorphic_destroy_function = void(*)(SharedPtrControlBlockSeperatePolymorphic*);

			 virtual ~SharedPtrControlBlockSeperatePolymorphic() noexcept = default;
			 virtual control_block_polymorphic_destroy_function getDestoryFunction() const noexcept = 0;
			 virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			 virtual void refIncrement(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
			 virtual void weakRefIncrement(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
			 virtual Size getUseCount(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
			 virtual bool incrementIfNotZero(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
		};

		template<class DataType>
		struct SharedPtrControlBlockSeperate : SharedPtrControlBlockSeperatePolymorphic {
			using control_block_polymorphic_destroy_function = typename SharedPtrControlBlockSeperatePolymorphic::control_block_polymorphic_destroy_function;
			using control_block_deleter_function_type = Function<void(SharedPtrControlBlockSeperate*)>;

			DataType* dataPtr;
			Atomic<Size> useCount;
			Atomic<Size> weakCount;
			Function<void(DataType*)> deleter;
			Function<void(SharedPtrControlBlockSeperate*)> controlBlockDeleter;

			
			template<class Deleter, class ControlBlockDeleter>
				requires(IsDeleter<Deleter, DataType> && IsDeleter<ControlBlockDeleter, SharedPtrControlBlockSeperate>)
			 SharedPtrControlBlockSeperate(DataType* dataPtr, Deleter&& deleter, ControlBlockDeleter&& controlBlockDeleter) noexcept
				: dataPtr(dataPtr), useCount(1), weakCount(), deleter(deleter), controlBlockDeleter(controlBlockDeleter) {}

			 ~SharedPtrControlBlockSeperate() noexcept override = default;

			static inline void destory(SharedPtrControlBlockSeperate* controlBlockSeperate) noexcept {
				if (controlBlockSeperate->useCount.load() == 1) {
					controlBlockSeperate->deleter.invoke(controlBlockSeperate->dataPtr);

					controlBlockSeperate->useCount--;
					if (controlBlockSeperate->weakCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockSeperate->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockSeperate);
					}
				}
			}
			static inline void destoryWeak(SharedPtrControlBlockSeperate* controlBlockSeperate) noexcept {
				if (controlBlockSeperate->weakCount.load() == 1) {
					if (controlBlockSeperate->useCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockSeperate->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockSeperate);
						return;
					}
				}
				controlBlockSeperate->weakCount--;
			}
			static inline bool incrementIfNotZero(SharedPtrControlBlockSeperate* controlBlockSeperate) {
				Size previous = controlBlockSeperate->useCount.load();
				for (;;) {
					if (previous == 0) {
						return false;
					}
					if (controlBlockSeperate->useCount.compare_exchange_weak(previous, previous + 1)) {
						return true;
					}
				}
			}

			control_block_polymorphic_destroy_function getDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					destory(controlBlockSeperate);
				};
			}
			control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					destoryWeak(controlBlockSeperate);
				};
			}
			void refIncrement(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 controlBlockSeperate->useCount++;
			}
			void weakRefIncrement(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 controlBlockSeperate->weakCount++;
			}
			Size getUseCount(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 return controlBlockSeperate->useCount.load();
			}
			bool incrementIfNotZero(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 return incrementIfNotZero(controlBlockSeperate);
			}
		};

		struct SharedPtrControlBlockFusedPolymorphic {
			using control_block_polymorphic_destroy_function = void(*)(SharedPtrControlBlockFusedPolymorphic*);

			 virtual ~SharedPtrControlBlockFusedPolymorphic() noexcept = default;
			 virtual control_block_polymorphic_destroy_function getDestoryFunction() const noexcept = 0;
			 virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			 virtual void refIncrement(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
			 virtual void weakRefIncrement(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
			 virtual Size getUseCount(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
			 virtual bool incrementIfNotZero(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
		};

		template<class DataType>
		struct SharedPtrControlBlockFused : SharedPtrControlBlockFusedPolymorphic {
			using control_block_polymorphic_destroy_function = typename SharedPtrControlBlockFusedPolymorphic::control_block_polymorphic_destroy_function;
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
			SharedPtrControlBlockFused(PreDeleteFunctor&& preDeleteFunctor, 
				PostDeleteFunctor&& postDeleteFunctor, 
				ControlBlockDeleter&& controlBlockDeleter, 
				ConstructArgTypes... constructArg) noexcept : 
					useCount(1), weakCount(0),
					preDeleteFunction(forward<PreDeleteFunctor>(preDeleteFunctor)),
					postDeleteFunction(forward<PostDeleteFunctor>(postDeleteFunctor)),
					controlBlockDeleter(forward<ControlBlockDeleter>(controlBlockDeleter)),  
					data(forward<ConstructArgTypes>(constructArg)...) {}

			 ~SharedPtrControlBlockFused() noexcept override = default;

			static inline void destory(SharedPtrControlBlockFused* controlBlockFused) noexcept {
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
			static inline void destoryWeak(SharedPtrControlBlockFused* controlBlockFused) noexcept {
				if (controlBlockFused->weakCount.load() == 1) {
					if (controlBlockFused->useCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockFused->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockFused);
						return;
					}
				}
				controlBlockFused->weakCount--;
			}
			static inline bool incrementIfNotZero(SharedPtrControlBlockFused* controlBlockFused) noexcept {
				Size previous = controlBlockFused->useCount.load();
				for (;;) {
					if (previous == 0) {
						return false;
					}
					if (controlBlockFused->useCount.compare_exchange_weak(previous, previous + 1)) {
						return true;
					}
				}
			}

			control_block_polymorphic_destroy_function getDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					destory(controlBlockFused);
				};
			}
			control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					destoryWeak(controlBlockFused);
				};
			}
			void refIncrement(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				controlBlockFused->useCount++;
			}
			void weakRefIncrement(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				controlBlockFused->weakCount++;
			}
			Size getUseCount(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				return controlBlockFused->useCount.load();
			}
			bool incrementIfNotZero(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				return incrementIfNotZero(controlBlockFused);
			}
		};

		struct SharedPtrControlBlockSeperatePolymorphicConstexpr {
			using control_block_polymorphic_destroy_function = bool(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);
			using control_block_polymorphic_increment_function = void(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);
			using control_block_polymorphic_get_count_function = Size(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);
			using control_block_polymorphic_increment_if_not_zero_function = bool(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);

			[[nodiscard]] constexpr virtual control_block_polymorphic_destroy_function getDestroyFunction() const noexcept = 0;
			[[nodiscard]] constexpr virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_increment_function getRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_increment_function getWeakRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_get_count_function getUseCountFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_increment_if_not_zero_function getIncrementIfNotZeroFunction() const noexcept = 0;
		};
		
		template<class DataType> 
		struct SharedPtrControlBlockSeperateConstexpr : SharedPtrControlBlockSeperatePolymorphicConstexpr {
			using control_block_polymorphic_destroy_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_destroy_function;
			using control_block_polymorphic_increment_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_increment_function;
			using control_block_polymorphic_get_count_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_get_count_function;
			using control_block_polymorphic_increment_if_not_zero_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_increment_if_not_zero_function;
			
			using pre_delete_function_type = Function<void(DataType*)>;
			using post_delete_function_type = Function<void(void)>;
			using deleter_function_type = Function<void(DataType*)>;
			using control_block_deleter_function_type = Function<void(SharedPtrControlBlockSeperateConstexpr*)>;

			DataType* dataPtr;
			Size useCount;
			Size weakCount;
			pre_delete_function_type preDeleteFunction;
			post_delete_function_type postDeleteFunction;
			deleter_function_type deleter;
			control_block_deleter_function_type controlBlockDeleter;

			constexpr SharedPtrControlBlockSeperateConstexpr(
				DataType* ptr,
				pre_delete_function_type&& preDeleteFunction,
				post_delete_function_type&& postDeleteFunction,
				deleter_function_type&& deleterFunction,
				control_block_deleter_function_type&& controlBlockDeleter) noexcept :
				dataPtr(ptr),
				useCount(1),
				weakCount(0),
				preDeleteFunction(natl::move(preDeleteFunction)),
				postDeleteFunction(natl::move(postDeleteFunction)),
				deleter(natl::move(deleterFunction)),
				controlBlockDeleter(natl::move(controlBlockDeleter)) {};

			[[nodiscard]] constexpr control_block_polymorphic_destroy_function getDestroyFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> bool {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					if (controlBlock->useCount == 1) {
						if (controlBlock->preDeleteFunction.isNotEmpty()) {
							controlBlock->preDeleteFunction.invoke(controlBlock->dataPtr);
						}

						controlBlock->deleter.invoke(controlBlock->dataPtr);

						if (controlBlock->postDeleteFunction.isNotEmpty()) {
							controlBlock->postDeleteFunction.invoke();
						}

						controlBlock->useCount--;
						if (controlBlock->weakCount == 0) {
							control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlock->controlBlockDeleter);
							controlBlockDeleter.invoke(controlBlock);
							return true;
						}
					}
					return false;
					};
			}
			[[nodiscard]] constexpr control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> bool {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					if (controlBlock->weakCount == 1) {
						if (controlBlock->weakCount == 0) {
							control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlock->controlBlockDeleter);
							controlBlockDeleter.invoke(controlBlock);
							return true;
						}
					}
					controlBlock->weakCount--;
					return false;
				};
			}
			constexpr control_block_polymorphic_increment_function getRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> void {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					controlBlock->useCount++;
					};
			}
			constexpr control_block_polymorphic_increment_function getWeakRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> void {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					controlBlock->weakCount++;
					};
			}
			constexpr control_block_polymorphic_get_count_function getUseCountFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> Size {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					return controlBlock->useCount;
					};
			}
			constexpr control_block_polymorphic_increment_if_not_zero_function getIncrementIfNotZeroFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> bool {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					if (controlBlock->useCount == 0) {
						return false;
					} else {
						controlBlock->useCount++;
						return true;
					}
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
		using control_block_seperate_polymorphic_constexpr = impl::SharedPtrControlBlockSeperatePolymorphicConstexpr;


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
			control_block_seperate_polymorphic_constexpr* controlBlockSeperatePolymorphicConstexpr;
		};
	public:

		//weak ptr 
		template<class WeakPtrDataType>
		class WeakPtr {
		public:
			using element_type = WeakPtrDataType;
			using element_pointer = WeakPtrDataType*;

			using control_block_seperate = impl::SharedPtrControlBlockSeperate<WeakPtrDataType>;
			using control_block_seperate_polymorphic = impl::SharedPtrControlBlockSeperatePolymorphic;
			using control_block_fused = impl::SharedPtrControlBlockFused<WeakPtrDataType>;
			using control_block_fused_polymorphic = impl::SharedPtrControlBlockFusedPolymorphic;
			using control_block_seperate_polymorphic_constexpr = impl::SharedPtrControlBlockSeperatePolymorphicConstexpr;

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
				control_block_seperate_polymorphic_constexpr* controlBlockSeperatePolymorphicConstexpr;
			};
		public:
			//constructor 
			constexpr WeakPtr() noexcept : controlBlockState(impl::SharedPtrControlBlockState::seperate), dataPtr(nullptr), controlBlockSeperate() {}
			constexpr WeakPtr(std::nullptr_t) noexcept : controlBlockState(impl::SharedPtrControlBlockState::seperate), dataPtr(nullptr), controlBlockSeperate() {}

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
						controlBlockSeperatePolymorphic->weakRefIncrement(controlBlockSeperatePolymorphic);
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
						controlBlockFusedPolymorphic->weakRefIncrement(controlBlockFusedPolymorphic);
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
						if (isConstantEvaluated()) {
							controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
							controlBlockSeperatePolymorphicConstexpr.functions->weakRefIncrement(controlBlockSeperatePolymorphicConstexpr.data);
						} else {
							unreachable();
						}
						break;
					default:
						unreachable();
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
					case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
						if (isConstantEvaluated()) {
							controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						} else {
							unreachable();
						}
						break;
					default:
						unreachable();
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
					dataPtr = static_cast<WeakPtrDataType*>(other.dataPtr);
					controlBlockState = other.controlBlockState;

					switch (other.controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
						controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
						controlBlockSeperatePolymorphic->weakRefIncrement(controlBlockSeperatePolymorphic);
						break;
					case impl::SharedPtrControlBlockState::fused:
						controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
						controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
						controlBlockFusedPolymorphic->weakRefIncrement(controlBlockFusedPolymorphic);
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
						controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
						controlBlockSeperatePolymorphic->weakRefIncrement(controlBlockSeperatePolymorphic);
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
						controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
						controlBlockFusedPolymorphic->weakRefIncrement(controlBlockFusedPolymorphic);
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
						if (isConstantEvaluated()) {
							controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr;
							controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
							controlBlockSeperatePolymorphicConstexpr.functions->weakRefIncrement(controlBlockSeperatePolymorphicConstexpr.data);
						} else {
							unreachable();
						}
						break;
					default:
						unreachable();
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
					dataPtr = static_cast<WeakPtrDataType*>(other.dataPtr);
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
					case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
						if (isConstantEvaluated()) {
							controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr;
							controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						} else {
							unreachable();
						}
						break;
					default:
						unreachable();
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

			template<class OtherWeakPtrDataType>
				requires(IsPolymorphicCastable<OtherWeakPtrDataType, WeakPtrDataType>)
			constexpr WeakPtr(const WeakPtr<OtherWeakPtrDataType>& other) noexcept {
				constructPolymorphicCopy<WeakPtr<OtherWeakPtrDataType>>(other);
			}
			template<class OtherWeakPtrDataType>
				requires(IsPolymorphicCastable<OtherWeakPtrDataType, WeakPtrDataType>)
			constexpr WeakPtr(WeakPtr<OtherWeakPtrDataType>&& other) noexcept {
				constructPolymorphicMove<WeakPtr<OtherWeakPtrDataType>>(forward<WeakPtr<OtherWeakPtrDataType>>(other));
			}

			constexpr WeakPtr(const SharedPtr<WeakPtrDataType>& other) noexcept {
				constructCopy<SharedPtr<WeakPtrDataType>>(other);
			}
			template<class OtherWeakPtrDataType>
				requires(IsPolymorphicCastable<OtherWeakPtrDataType, WeakPtrDataType>)
			constexpr WeakPtr(const SharedPtr<WeakPtrDataType>& other) noexcept {
				constructPolymorphicCopy<SharedPtr<OtherWeakPtrDataType>>(other);
			}
		private:
			constexpr void destruct() noexcept {
				if (dataPtr) {
					switch (controlBlockState) {
					case impl::SharedPtrControlBlockState::seperate:
						control_block_seperate::destoryWeak(controlBlockSeperate);
						break;
					case impl::SharedPtrControlBlockState::fused:
						control_block_fused::destoryWeak(controlBlockFused);
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphic:
						controlBlockSeperatePolymorphic->getWeakDestoryFunction()(controlBlockSeperatePolymorphic);
						break;
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						controlBlockFusedPolymorphic->getWeakDestoryFunction()(controlBlockFusedPolymorphic);
						break;
					case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
						if (isConstantEvaluated()) {
							controlBlockSeperatePolymorphicConstexpr->getWeakDestoryFunction()(controlBlockSeperatePolymorphicConstexpr);
						} else {
							unreachable();
						}
						break;
					default:
						unreachable();
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

			template<class OtherWeakPtrDataType>
				requires(IsPolymorphicCastable<OtherWeakPtrDataType, WeakPtrDataType>)
			constexpr WeakPtr& operator=(const WeakPtr<OtherWeakPtrDataType>& other) noexcept {
				destruct();
				constructPolymorphicCopy<WeakPtr<OtherWeakPtrDataType>>(other);
				return self();
			}
			template<class OtherWeakPtrDataType>
				requires(IsPolymorphicCastable<OtherWeakPtrDataType, WeakPtrDataType>)
			constexpr WeakPtr& operator=(WeakPtr<OtherWeakPtrDataType>&& other) noexcept {
				destruct();
				constructPolymorphicMove<WeakPtr<OtherWeakPtrDataType>>(forward<WeakPtr<OtherWeakPtrDataType>>(other));
				return self();
			}

			constexpr WeakPtr& operator=(const SharedPtr<WeakPtrDataType>& other) noexcept {
				destruct();
				constructCopy<SharedPtr<WeakPtrDataType>>(other);
				return self();
			}
			template<class OtherWeakPtrDataType>
				requires(IsPolymorphicCastable<OtherWeakPtrDataType, WeakPtrDataType>)
			constexpr WeakPtr& operator=(const SharedPtr<WeakPtrDataType>& other) noexcept {
				destruct();
				constructPolymorphicCopy<SharedPtr<OtherWeakPtrDataType>>(other);
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

				WeakPtrDataType* tempDataPtr = other.dataPtr;
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
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr: {
					if (isConstantEvaluated()) {
						control_block_seperate_polymorphic_constexpr* controlBlockSeperatePolymorphicConstexprTemp = other.controlBlockSeperatePolymorphicConstexpr;
						other.controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexpr;
						controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexprTemp;
					} else {
						unreachable();
					}
					break;
				}
				default:
					unreachable();
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
						return controlBlockSeperatePolymorphic->getUseCount(controlBlockSeperatePolymorphic);
					case impl::SharedPtrControlBlockState::fusedPolymorphic:
						return controlBlockFusedPolymorphic->getUseCount(controlBlockFusedPolymorphic);
					case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
						if (isConstantEvaluated()) {
							return controlBlockSeperatePolymorphicConstexpr->getUseCountFunction()(controlBlockSeperatePolymorphicConstexpr);
						} else {
							unreachable();
						}
					default:
						unreachable();
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

			constexpr bool empty() const noexcept { return dataPtr == nullptr; }
			constexpr bool isEmpty() const noexcept { return empty(); }
			constexpr bool isNotEmpty() const noexcept { return !empty(); }
			explicit constexpr operator bool() const noexcept { return isNotEmpty(); }

			friend SharedPtr;
		}; 

		using weak_type = WeakPtr<DataType>;


	private:
		template<class OtherType, bool IncrementRefCount = true>
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
					if constexpr (IncrementRefCount) {
						controlBlockSeperate->useCount++;
					}
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockFused = other.controlBlockFused;
					if constexpr (IncrementRefCount) {
						controlBlockFused->useCount++;
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockSeperatePolymorphic->refIncrement(controlBlockSeperatePolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockFusedPolymorphic->refIncrement(controlBlockFusedPolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						if constexpr (IncrementRefCount) {
							controlBlockSeperatePolymorphicConstexpr->getRefIncrementFunction()(controlBlockSeperatePolymorphicConstexpr);
						}
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
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
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}

			other.dataPtr = nullptr;
		}
		template<class OtherType, bool IncrementRefCount = true>
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
					if constexpr (IncrementRefCount) {
						controlBlockSeperatePolymorphic->refIncrement(controlBlockSeperatePolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
					controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
					if constexpr (IncrementRefCount) {
						controlBlockFusedPolymorphic->refIncrement(controlBlockFusedPolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockSeperatePolymorphic->refIncrement(controlBlockSeperatePolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockState = impl::SharedPtrControlBlockState::fusedPolymorphic;
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockFusedPolymorphic->refIncrement(controlBlockFusedPolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr;
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						if constexpr (IncrementRefCount) {
							controlBlockSeperatePolymorphicConstexpr.functions->refIncrement(controlBlockSeperatePolymorphicConstexpr.data);
						}
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
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
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr;
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}

			other.dataPtr = nullptr;
		}

	public:
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
			if (other.isNotEmpty() && incrementIfNotZero<WeakPtr<DataType>>(other)) {
				constructCopy<WeakPtr<DataType>, false>(other);
			} else {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			}
		}
		template<class OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(const WeakPtr<OtherDataType>& other) noexcept {
			if (other.isNotEmpty() && incrementIfNotZero<WeakPtr<DataType>>(other)) {
				constructPolymorphicCopy<WeakPtr<OtherDataType>, false>(other);
			} else {
				dataPtr = nullptr;
				controlBlockState = impl::SharedPtrControlBlockState::seperate;
				controlBlockSeperate = nullptr;
			}
		}



	private:
		template<class OtherType>
		constexpr bool incrementIfNotZero(const OtherType& other) noexcept {
			switch (controlBlockState) {
			case impl::SharedPtrControlBlockState::seperate:
				return control_block_seperate::incrementIfNotZero(other.controlBlockSeperate);
			case impl::SharedPtrControlBlockState::fused:
				return control_block_fused::incrementIfNotZero(other.controlBlockFused);
			case impl::SharedPtrControlBlockState::seperatePolymorphic:
				return other.controlBlockSeperatePolymorphic->incrementIfNotZero(other.controlBlockSeperatePolymorphic);
			case impl::SharedPtrControlBlockState::fusedPolymorphic:
				return other.controlBlockFusedPolymorphic->incrementIfNotZero(other.controlBlockFusedPolymorphic);
			case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
				if (isConstantEvaluated()) {
					return other.controlBlockSeperatePolymorphicConstexpr->getIncrementIfNotZeroFunction()(other.controlBlockSeperatePolymorphicConstexpr);
				} else {
					unreachable();
				}
				break;
			default:
				unreachable();
			}
		}

		template<typename LocalDataType, typename Alloc>
			requires(IsAllocator<Alloc>)
		constexpr void constructContexpr(
			DataType* ptr, 
			typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::pre_delete_function_type&& preDeleteFunction,
			typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::post_delete_function_type&& postDeleteFunction,
			typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::deleter_function_type&& deleterFunction) noexcept {
			using SharedPtrControlBlockSeperateConstexprType = impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>;
			dataPtr = ptr;
			controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr;
			
			using control_block_seperate_data_constexpr_alloc = Alloc::template rebind_alloc<SharedPtrControlBlockSeperateConstexprType>;
			typename SharedPtrControlBlockSeperateConstexprType::control_block_deleter_function_type controlBlockDeleter =
				[](SharedPtrControlBlockSeperateConstexprType* controlBlockPtr) {
				natl::defaultDeconstruct(controlBlockPtr);
				control_block_seperate_data_constexpr_alloc::deallocate(controlBlockPtr, 1);
			};

			SharedPtrControlBlockSeperateConstexprType* controlBlock = control_block_seperate_data_constexpr_alloc::allocate(1);
			std::construct_at(controlBlock, 
				ptr,
				natl::move(preDeleteFunction), 
				natl::move(postDeleteFunction), 
				natl::move(deleterFunction), 
				natl::move(controlBlockDeleter));

			controlBlockSeperatePolymorphicConstexpr = static_cast<impl::SharedPtrControlBlockSeperatePolymorphicConstexpr*>(controlBlock);
		}

		template<typename LocalDataType>
		constexpr typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::pre_delete_function_type constructConstexprNullPreDelete() noexcept {
			return typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::pre_delete_function_type();
		}

		template<typename LocalDataType>
		constexpr typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::post_delete_function_type constructConstexprNullPostDelete() noexcept {
			return typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::post_delete_function_type();
		}
		template<typename LocalDataType, typename Alloc>
			requires(IsAllocator<Alloc>)
		constexpr typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::deleter_function_type constructConstexprStandardDeleter() noexcept {
			return 
				typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::deleter_function_type(
					[](LocalDataType* ptr) {
						using LocalDataTypeAlloc = typename Alloc::template rebind_alloc<LocalDataType>;
						LocalDataTypeAlloc::deallocate(ptr, 1);
					}
				);
		}


		template<typename Alloc>
			requires(IsAllocator<Alloc>)
		constexpr void construct(DataType* ptr) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<DataType, Alloc>(ptr,
					constructConstexprNullPreDelete<DataType>(),
					constructConstexprNullPostDelete<DataType>(),
					constructConstexprStandardDeleter<DataType, Alloc>());
				return;
			}


			dataPtr = ptr;
			controlBlockState = impl::SharedPtrControlBlockState::seperate;
			using control_block_seperate_alloc = Alloc::template rebind_alloc<control_block_seperate>;
			controlBlockSeperate = control_block_seperate_alloc::allocate(1);
			std::construct_at<control_block_seperate>(controlBlockSeperate, dataPtr, DefaultDeleter<DataType, Alloc>(), DefaultDeleter<control_block_seperate, control_block_seperate_alloc>());
		}

		template<class Alloc, class Deleter>
			requires(IsAllocator<Alloc>&& IsDeleter<Deleter, DataType>)
		constexpr void construct(DataType* ptr, Deleter&& deleter) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<DataType, Alloc>(ptr,
					constructConstexprNullPreDelete<DataType>(),
					constructConstexprNullPostDelete<DataType>(),
					natl::move(deleter));
				return;
			}

			dataPtr = ptr;
			controlBlockState = impl::SharedPtrControlBlockState::seperate;
			using control_block_seperate_alloc = Alloc::template rebind_alloc<control_block_seperate>;
			controlBlockSeperate = control_block_seperate_alloc::allocate(1);
			std::construct_at<control_block_seperate>(controlBlockSeperate, dataPtr, deleter, DefaultDeleter<control_block_seperate, control_block_seperate_alloc>());
		}
		template<class OtherDataType, class Alloc>
			requires(IsAllocator<Alloc> && IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr void construct(OtherDataType* ptr) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<OtherDataType, Alloc>(ptr,
					constructConstexprNullPreDelete<OtherDataType>(),
					constructConstexprNullPostDelete<OtherDataType>(),
					constructConstexprStandardDeleter<OtherDataType, Alloc>());
				return;
			}


			dataPtr = static_cast<OtherDataType*>(ptr);
			controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
			using other_control_block_type = impl::SharedPtrControlBlockSeperate<OtherDataType>;
			using other_control_block_type_alloc = Alloc::template rebind_alloc<other_control_block_type>;
			other_control_block_type* otherControlBlockSeperate = other_control_block_type_alloc::allocate(1);
			std::construct_at<other_control_block_type>(otherControlBlockSeperate, dataPtr, DefaultDeleter<DataType, Alloc>(), DefaultDeleter<other_control_block_type, other_control_block_type_alloc>());
			controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(otherControlBlockSeperate);
		}

		template<class OtherDataType, class Alloc, class Deleter>
			requires(IsAllocator<Alloc>&& IsDeleter<Deleter, DataType>)
		constexpr void construct(OtherDataType* ptr, Deleter&& deleter) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<OtherDataType, Alloc>(ptr,
					constructConstexprNullPreDelete<OtherDataType>(),
					constructConstexprNullPostDelete<OtherDataType>(),
					natl::move(deleter));
				return;
			}

			dataPtr = static_cast<OtherDataType*>(ptr);
			controlBlockState = impl::SharedPtrControlBlockState::seperatePolymorphic;
			using other_control_block_type = impl::SharedPtrControlBlockSeperate<OtherDataType>;
			using other_control_block_type_alloc = Alloc::template rebind_alloc<other_control_block_type>;
			other_control_block_type* otherControlBlockSeperate = other_control_block_type_alloc::allocate(1);
			std::construct_at<other_control_block_type>(otherControlBlockSeperate, dataPtr, deleter, DefaultDeleter<other_control_block_type, other_control_block_type_alloc>());
			controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(otherControlBlockSeperate);
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
			if (isConstantEvaluated()) {
				constructContexpr<DataType, Alloc>(
					Alloc::template rebind_alloc<DataType>::allocate(1),
					constructConstexprNullPreDelete<DataType>(),
					constructConstexprNullPostDelete<DataType>(),
					constructConstexprStandardDeleter<DataType, Alloc>());
				return;
			}

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
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr->getDestroyFunction()(controlBlockSeperatePolymorphicConstexpr);
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
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
			requires(IsPolymorphicCastable<OtherDataType, DataType> && IsDeleter<Deleter, OtherDataType>)
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
			case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr: {
				if (isConstantEvaluated()) {
					control_block_seperate_polymorphic_constexpr controlBlockSeperatePolymorphicConstexprTemp = other.controlBlockSeperatePolymorphicConstexpr;
					other.controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexpr;
					controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexprTemp;
				} else {
					unreachable();
				}
			}
			default:
				unreachable();
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
					return controlBlockSeperatePolymorphic->getUseCount(controlBlockSeperatePolymorphic);
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					return controlBlockFusedPolymorphic->getUseCount(controlBlockFusedPolymorphic);
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						return controlBlockSeperatePolymorphicConstexpr.functions->getUseCount(controlBlockSeperatePolymorphicConstexpr.data);
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
			return 0;
		}

		constexpr bool empty() const noexcept { return dataPtr == nullptr; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }
		explicit constexpr operator bool() const noexcept { return isNotEmpty(); }
	};

	template<class DataType>
	using WeakPtr = typename SharedPtr<DataType>::weak_type;

	template<class DataType>
	class ObserverPtr {
	public:
		using element_type = DataType;
	private:
		element_type* dataPtr;
	public:
		//construtor
		constexpr ObserverPtr() noexcept = default;
		constexpr ObserverPtr(const ObserverPtr&) noexcept = default;
		constexpr ObserverPtr(ObserverPtr&&) noexcept = default;
		constexpr explicit ObserverPtr(element_type* dataPtr) noexcept : dataPtr(dataPtr) {}

		//destructor
		constexpr ~ObserverPtr() noexcept = default;

		//util 
		constexpr ObserverPtr& self() noexcept { return *this; }
		constexpr const ObserverPtr& self() const noexcept { return *this; }

		//assignment 
		constexpr ObserverPtr& operator=(const ObserverPtr&) noexcept = default;
		constexpr ObserverPtr& operator=(ObserverPtr&&) noexcept = default;

		//modifiers
		[[nodiscard]] constexpr element_type* release() noexcept {
			element_type* tempDataPtr = dataPtr;
			dataPtr = nullptr;
			return dataPtr;
		}
		constexpr void reset(element_type* p = nullptr) noexcept {
			dataPtr = p;
		}
		constexpr void swap(ObserverPtr& other) noexcept {
			element_type* tempDataPtr = other.tempDataPtr;
			other.dataPtr = dataPtr;
			dataPtr = tempDataPtr;
		}

		//observers
		constexpr element_type* get() const noexcept { return dataPtr; }
		constexpr explicit operator bool() const noexcept { return dataPtr; }
		constexpr element_type& operator*() const { return dataPtr; }
		constexpr element_type* operator->() const noexcept { return dataPtr; }
		constexpr bool isNull() noexcept { return dataPtr == nullptr; }
		constexpr bool isNotNull() noexcept { return dataPtr != nullptr; }

		//conversions
		constexpr explicit operator element_type* () const noexcept { return dataPtr; }

		//compare 
		constexpr bool operator==(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr == other.dataPtr;
		}
		constexpr bool operator!=(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr != other.dataPtr;
		}
		constexpr bool operator<(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr < other.dataPtr;
		}
		constexpr bool operator>(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr > other.dataPtr;
		}
		constexpr bool operator<=(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr <= other.dataPtr;
		}
		constexpr bool operator>=(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr >= other.dataPtr;
		}
		constexpr auto operator<=>(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr <=> other.dataPtr;
		}
	};

	template<class DataType>
	constexpr ObserverPtr<DataType> makeObserver(DataType* ptr) noexcept {
		return ObserverPtr<DataType>(ptr);
	}
}