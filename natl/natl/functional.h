#pragma once

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "dataMovement.h"
#include "allocator.h"

//interface
namespace natl {
	template <typename Functor, typename ReturnType, typename... ArgTypes>
	concept HasFunctionSignature = requires(Functor functor, ArgTypes... args) {
		{ functor(args...) } -> std::same_as<ReturnType>;
	};

	namespace impl{
		template<typename ReturnType, typename... ArgTypes>
		struct CallableBase {
			using destory_function = void(*)(CallableBase*);
			constexpr virtual ReturnType invoke(ArgTypes...) const noexcept = 0;
			constexpr virtual CallableBase* copyCallable(CallableBase* location) const noexcept = 0;
			constexpr virtual CallableBase* moveCallable(CallableBase* location) noexcept = 0;
			constexpr virtual destory_function getDestoryFunction() const noexcept = 0;
			constexpr virtual ~CallableBase() noexcept = default;
		};

		template<typename ReturnType, typename... ArgTypes>
		using CallableDestoryFunction = void(*)(CallableBase<ReturnType, ArgTypes...>);
		template <typename Alloc, typename Functor, typename ReturnType, typename... ArgTypes>
			requires(IsAllocator<Alloc>)
		struct Callable : public CallableBase<ReturnType, ArgTypes...> {
			using callable_base = CallableBase<ReturnType, ArgTypes...>;
			using destory_function = void(*)(callable_base*);
		private:
			Functor functor;
		public:

			constexpr Callable(Functor&& functor) noexcept : functor(move(functor)) {}
			constexpr Callable(const Functor& functor) noexcept : functor(functor) {}

			constexpr callable_base* copyCallable(callable_base* location) const noexcept override {
				if (location) {
					std::construct_at<Callable>(static_cast<Callable*>(location), functor);
					return nullptr;
				} else {
					Callable* newCallable = Alloc::template rebind_alloc<Callable>::allocate(1);
					std::construct_at<Callable>(newCallable, functor);
					return static_cast<callable_base*>(newCallable);
				}
			}
			constexpr callable_base* moveCallable(callable_base* location) noexcept override {
				if (location) {
					std::construct_at<Callable, Functor>(static_cast<Callable*>(location), move(functor));
					return nullptr;
				} else {
					Callable* newCallable = Alloc::template rebind_alloc<Callable>::allocate(1);
					std::construct_at<Callable, Functor>(newCallable, move(functor));
					return static_cast<callable_base*>(newCallable);
				}
			}
			constexpr destory_function getDestoryFunction() const noexcept override {
				return [](callable_base* location) -> void {
					Callable* destoryCallable = dynamic_cast<Callable*>(location);
					std::destroy_at<Callable>(destoryCallable);
					Alloc::template rebind_alloc<Callable>::deallocate(destoryCallable, 1);
				};
			}
			constexpr ReturnType invoke(ArgTypes... args) const noexcept override {
				return functor(forward<ArgTypes>(args)...);
			}
		};

		struct PolymorphicFunctorStorageBase {
			constexpr virtual ~PolymorphicFunctorStorageBase() noexcept = default;
		};

		template<class Functor>
		struct PolymorphicFunctorStorage : public PolymorphicFunctorStorageBase {
			Functor functor;
			constexpr PolymorphicFunctorStorage(Functor&& functor) noexcept : functor(move(functor)) {}
			constexpr PolymorphicFunctorStorage(const Functor& functor) noexcept : functor(functor) {}
		};

		template<typename ReturnType, typename... ArgTypes>
		struct ConstexprCallableBase {
		public:
			constexpr virtual ReturnType operator()(PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase, ArgTypes... args) const noexcept = 0;
			constexpr virtual PolymorphicFunctorStorageBase* copyCallableFunctorStorage(const PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase) const noexcept = 0;
			constexpr virtual ConstexprCallableBase* copyCallable() const noexcept = 0;
			constexpr virtual ~ConstexprCallableBase() noexcept = default;
		};

		template <typename Functor, typename ReturnType, typename... ArgTypes>
		struct ConstexprCallable : public ConstexprCallableBase<ReturnType, ArgTypes...> {

			using constexpr_callable_base = ConstexprCallableBase<ReturnType, ArgTypes...>;
			
			constexpr ConstexprCallable() noexcept {}


			constexpr ReturnType operator()(PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase, ArgTypes... args) const noexcept override {
				return dynamic_cast<PolymorphicFunctorStorage<Functor>*>(polymorphicFunctorStorageBase)->functor(forward<ArgTypes>(args)...);
			}
			constexpr PolymorphicFunctorStorageBase* copyCallableFunctorStorage(const PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase) const noexcept override {
				PolymorphicFunctorStorage<Functor>* copyOfFunctorStorage =
					new PolymorphicFunctorStorage<Functor>(
						dynamic_cast<const PolymorphicFunctorStorage<Functor>*>(polymorphicFunctorStorageBase)->functor
					);
				return static_cast<PolymorphicFunctorStorageBase*>(copyOfFunctorStorage);
			}
			constexpr constexpr_callable_base* copyCallable() const noexcept override {
				return new ConstexprCallable();
			}
		};

		template<typename ReturnType, typename... ArgTypes>
		struct BaseConstexprFunctionStorage {

			PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase;
			ConstexprCallableBase<ReturnType, ArgTypes...>* constexprCallable;

			//constructor
			constexpr BaseConstexprFunctionStorage() noexcept = default;
			constexpr BaseConstexprFunctionStorage(PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase, 
				ConstexprCallableBase<ReturnType, ArgTypes...>* constexprCallable) noexcept : 
				polymorphicFunctorStorageBase(polymorphicFunctorStorageBase),  constexprCallable(constexprCallable) {}
		};

		enum class FunctionStorageType {
			functionPtr = 0,
			smallCallable = 1,
			heapCallable = 2,
			constexprCallable = 3,
		};
	}


	template<class Signature, Size Capacity = 32 - alignof(char*), typename Alloc = DefaultAllocator<Signature>>
	class Function {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class Function<ReturnType(ArgTypes...), Capacity, Alloc> final {
	public:
		using result_type = ReturnType;
		using function_ptr_type = ReturnType(*)(ArgTypes...);

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;

		constexpr static Size bufferSize = Capacity;

		using callable_base = impl::CallableBase<ReturnType, ArgTypes...>;
		using callable_destory_function = typename callable_base::destory_function;
		using base_constexpr_function_storage = impl::BaseConstexprFunctionStorage<ReturnType, ArgTypes...>;
	private:
		impl::FunctionStorageType functionStorageType;
		union {
			callable_base* heapCallable;
			function_ptr_type functionPtr;
			Byte smallCallableStorage[bufferSize];
			base_constexpr_function_storage constexprFunctionStorage;
		};
	public:
		//constructor 
		constexpr Function() noexcept : functionStorageType(impl::FunctionStorageType::functionPtr), functionPtr(nullptr) {}

		template<Size OtherCap>
		constexpr Function(const Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept {
			assign<OtherCap>(other);
		}
		template<Size OtherCap>
		constexpr Function(Function<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept {
			assign<OtherCap>(natl::forward<decltype(other)>(other));
		}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
		constexpr Function(Functor&& functor) noexcept {
			assign<Functor>(move(functor));
		}

		//destructors
	private:
		constexpr void destruct() noexcept {
			switch (functionStorageType) {
			case impl::FunctionStorageType::smallCallable:
				std::destroy_at<callable_base>(reinterpret_cast<callable_base*>(smallCallableStorage));
				break;
			case impl::FunctionStorageType::heapCallable: {
				callable_destory_function destoryFunction = heapCallable->getDestoryFunction();
				destoryFunction(heapCallable);
				}
				break;
			case impl::FunctionStorageType::constexprCallable:
				if (std::is_constant_evaluated()) {
					delete constexprFunctionStorage.constexprCallable;
					delete constexprFunctionStorage.polymorphicFunctorStorageBase;
					std::destroy_at<base_constexpr_function_storage>(&constexprFunctionStorage);
				}
				break;
			default:
				break;
			}
		}
	public:

		constexpr ~Function() noexcept {
			destruct();
		}

		//util 
		constexpr Function& self() noexcept { return *this; }
		constexpr const Function& self() const noexcept { return *this; }

		//assignment 
		template<Size OtherCap>
		constexpr Function& operator=(const Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept {
			destruct();
			return assign<OtherCap>(other);
		}
		template<Size OtherCap>
		constexpr Function& operator=(Function<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept {
			destruct();
			return assign<OtherCap>(natl::forward<decltype(other)>(other));
		}
		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
		constexpr Function& operator=(Functor&& functor) noexcept {
			destruct();
			return assign<Functor>(functor);
		}

	private:
		template<Size OtherCap>
		constexpr Function& assign(const Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept {
			if (other.empty()) {
				functionStorageType = impl::FunctionStorageType::functionPtr;
				functionPtr = nullptr;
			} else {
				functionStorageType = other.functionStorageType;
				switch (functionStorageType) {
				case impl::FunctionStorageType::functionPtr:
					functionPtr = other.functionPtr;
					break;
				case impl::FunctionStorageType::smallCallable:
					if constexpr (Capacity >= OtherCap) {
						reinterpret_cast<callable_base*>(other.smallCallableStorage)->copyCallable(reinterpret_cast<callable_base*>(smallCallableStorage));
					} else {
						functionStorageType = impl::FunctionStorageType::heapCallable;
						heapCallable = reinterpret_cast<callable_base*>(other.smallCallableStorage)->copyCallable(nullptr);
					}
					break;
				case impl::FunctionStorageType::heapCallable:
					heapCallable = other.heapCallable->copyCallable(nullptr);
					break;
				case impl::FunctionStorageType::constexprCallable:
					if (std::is_constant_evaluated()) {
						std::construct_at<base_constexpr_function_storage>(
							&constexprFunctionStorage,
							other.constexprFunctionStorage.constexprCallable->copyCallableFunctorStorage(other.constexprFunctionStorage.polymorphicFunctorStorageBase),
							other.constexprFunctionStorage.constexprCallable->copyCallable());
					}
					break;
				}
			}
			return self();
		}

		template<Size OtherCap>
		constexpr Function& assign(Function<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept {
			if (other.empty()) {
				functionStorageType = impl::FunctionStorageType::functionPtr;
				functionPtr = nullptr;
			} else {
				functionStorageType = other.functionStorageType;
				switch (functionStorageType) {
				case impl::FunctionStorageType::functionPtr:
					functionPtr = other.functionPtr;
					break;
				case impl::FunctionStorageType::smallCallable:
					if constexpr (Capacity >= OtherCap) {
						reinterpret_cast<callable_base*>(other.smallCallableStorage)->moveCallable(reinterpret_cast<callable_base*>(smallCallableStorage));
					} else {
						functionStorageType = impl::FunctionStorageType::heapCallable;
						heapCallable = reinterpret_cast<callable_base*>(other.smallCallableStorage)->moveCallable(nullptr);
					}
					break;
				case impl::FunctionStorageType::heapCallable:
					heapCallable = other.heapCallable;
					break;
				case impl::FunctionStorageType::constexprCallable:
					if (std::is_constant_evaluated()) {
						std::construct_at<base_constexpr_function_storage>(
							&constexprFunctionStorage,
							other.constexprFunctionStorage.polymorphicFunctorStorageBase,
							other.constexprFunctionStorage.constexprCallable);
						std::destroy_at<base_constexpr_function_storage>(&other.constexprFunctionStorage);
					}
					break;
				}
				other.functionStorageType = impl::FunctionStorageType::functionPtr;
				other.functionPtr = nullptr;
			}
			return self();
		}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
		constexpr void assign(Functor&& functor) noexcept {
			if constexpr (std::is_function_v<std::remove_pointer_t<Functor>>) {
				functionPtr = static_cast<function_ptr_type>(functor);
				functionStorageType = impl::FunctionStorageType::functionPtr;
				return;
			}

			using FunctorCallableType = impl::Callable<Alloc, Functor, ReturnType, ArgTypes...>;
			if (std::is_constant_evaluated()) {
				functionStorageType = impl::FunctionStorageType::constexprCallable;
				std::construct_at<base_constexpr_function_storage>(
					&constexprFunctionStorage,
					new impl::PolymorphicFunctorStorage(move(functor)),
					static_cast<impl::ConstexprCallableBase<ReturnType, ArgTypes...>*>(new impl::ConstexprCallable<Functor, ReturnType, ArgTypes...>()));
			} else {
				if constexpr (sizeof(FunctorCallableType) <= bufferSize) {
					functionStorageType = impl::FunctionStorageType::smallCallable;
					std::construct_at<FunctorCallableType, Functor>(reinterpret_cast<FunctorCallableType*>(smallCallableStorage), forward<Functor>(functor));
				} else {
					functionStorageType = impl::FunctionStorageType::heapCallable;
					FunctorCallableType* functorCallable = Alloc::template rebind_alloc<FunctorCallableType>::allocate(1);
					std::construct_at<FunctorCallableType>(functorCallable, move(functor));
					heapCallable = static_cast<callable_base*>(functorCallable);
				}
			}
		}
	public:

		constexpr bool empty() const noexcept {
			if (functionStorageType == impl::FunctionStorageType::functionPtr) {
				return !functionPtr;
			} else {
				return false;
			}
		}
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }

		explicit constexpr operator bool() const noexcept {
			return isNotEmpty();
		}

		constexpr ReturnType operator()(ArgTypes... args) const noexcept {
			return invoke(forward<ArgTypes>(args)...);
		}

		constexpr ReturnType invoke(ArgTypes... args) const noexcept {
			switch (functionStorageType) {
			case impl::FunctionStorageType::functionPtr:
				return functionPtr(forward<ArgTypes>(args)...);
			case impl::FunctionStorageType::smallCallable:
				return reinterpret_cast<const callable_base*>(smallCallableStorage)->invoke(forward<ArgTypes>(args)...);
			case impl::FunctionStorageType::heapCallable:
				return heapCallable->invoke(forward<ArgTypes>(args)...);
			case impl::FunctionStorageType::constexprCallable:
				if (std::is_constant_evaluated()) {
					return constexprFunctionStorage.constexprCallable->operator()(constexprFunctionStorage.polymorphicFunctorStorageBase, forward<ArgTypes>(args)...);
				}
			}
		}

		constexpr void swap(Function& other) noexcept {
			natl::swap<Function>(self(), other);
		}
	};

	template<class Signature, Size ByteSize, typename Alloc = DefaultAllocator<Signature>>
	using FunctionByteSize = Function<Signature, ByteSize - alignof(char*), Alloc>;

	template<class Signature, typename Alloc = DefaultAllocator<Signature>>
	using FunctionAlloc = FunctionByteSize<Signature, 32, Alloc>;

}