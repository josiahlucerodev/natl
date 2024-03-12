#pragma once

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "dataMovement.h"
#include "allocator.h"
#include "peramaterPackOperations.h"

//interface
namespace natl {
	template <typename Functor, typename ReturnType, typename... ArgTypes>
	concept HasFunctionSignature = requires(Functor functor, ArgTypes... args) {
		{ functor(natl::forward<ArgTypes>(args)...) } -> std::same_as<ReturnType>;
	};

	template<typename Functor, typename ReturnType, typename... ArgTypes>
	concept IsConstCallable = HasFunctionSignature<Functor, ReturnType, ArgTypes...> && 
		requires(const Functor functor, ArgTypes... args) {
		{ functor(natl::forward<ArgTypes>(args)...) } -> std::same_as<ReturnType>;
	};

	namespace impl{
		template<typename ReturnType, typename... ArgTypes>
		struct CallableBase {
			using destory_function = void(*)(CallableBase*) noexcept;
			constexpr virtual ReturnType invoke(ArgTypes...) const noexcept = 0;
			constexpr virtual CallableBase* copyCallable(CallableBase* location) const noexcept = 0;
			constexpr virtual CallableBase* moveCallable(CallableBase* location) noexcept = 0;
			constexpr virtual destory_function getDestoryFunction() const noexcept = 0;
			constexpr virtual ~CallableBase() noexcept = default;
		};

		template <typename Alloc, typename Functor, typename ReturnType, typename... ArgTypes>
			requires(IsAllocator<Alloc>)
		struct Callable : public CallableBase<ReturnType, ArgTypes...> {
			using callable_base = CallableBase<ReturnType, ArgTypes...>;
			using destory_function = void(*)(callable_base*) noexcept;
		private:
			mutable Functor functor;
		public:

			constexpr Callable(const Functor& functor) noexcept requires(IsCopyConstructible<Functor>) : functor(functor) {}
			constexpr Callable(Functor&& functor) noexcept : functor(natl::move(functor)) {}

			constexpr callable_base* copyCallable(callable_base* location) const noexcept override {
				if constexpr (IsCopyConstructible<Functor>) {
					if (location) {
						std::construct_at<Callable>(static_cast<Callable*>(location), functor);
						return nullptr;
					} else {
						Callable* newCallable = Alloc::template rebind_alloc<Callable>::allocate(1);
						std::construct_at<Callable>(newCallable, functor);
						return static_cast<callable_base*>(newCallable);
					}
				} else {
					return nullptr;
				}
			}
			constexpr callable_base* moveCallable(callable_base* location) noexcept override {
				if (location) {
					std::construct_at<Callable, Functor>(static_cast<Callable*>(location), natl::move(functor));
					return nullptr;
				} else {
					Callable* newCallable = Alloc::template rebind_alloc<Callable>::allocate(1);
					std::construct_at<Callable, Functor>(newCallable, natl::move(functor));
					return static_cast<callable_base*>(newCallable);
				}
			}
			constexpr destory_function getDestoryFunction() const noexcept override {
				return [](callable_base* location) noexcept -> void {
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
			constexpr PolymorphicFunctorStorage(Functor&& functor) noexcept : functor(natl::move(functor)) {}
			constexpr PolymorphicFunctorStorage(const Functor& functor) noexcept : functor(functor) {}
		};

		template<typename ReturnType, typename... ArgTypes>
		struct ConstexprCallableBase {
		public:
			constexpr virtual ReturnType invoke(PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase, ArgTypes... args) const noexcept = 0;
			constexpr virtual PolymorphicFunctorStorageBase* copyCallableFunctorStorage(const PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase) const noexcept = 0;
			constexpr virtual ConstexprCallableBase* copyCallable() const noexcept = 0;
			constexpr virtual ~ConstexprCallableBase() noexcept = default;
		};

		template <typename Functor, typename ReturnType, typename... ArgTypes>
		struct ConstexprCallable : public ConstexprCallableBase<ReturnType, ArgTypes...> {

			using constexpr_callable_base = ConstexprCallableBase<ReturnType, ArgTypes...>;
			
			constexpr ConstexprCallable() noexcept {}

			constexpr ReturnType invoke(PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase, ArgTypes... args) const noexcept override {
				return dynamic_cast<PolymorphicFunctorStorage<Functor>*>(polymorphicFunctorStorageBase)->functor(forward<ArgTypes>(args)...);
			}
			constexpr PolymorphicFunctorStorageBase* copyCallableFunctorStorage(const PolymorphicFunctorStorageBase* polymorphicFunctorStorageBase) const noexcept override {
				if constexpr (IsCopyConstructible<Functor>) {
					PolymorphicFunctorStorage<Functor>* copyOfFunctorStorage =
						new PolymorphicFunctorStorage<Functor>(
							dynamic_cast<const PolymorphicFunctorStorage<Functor>*>(polymorphicFunctorStorageBase)->functor
						);
					return static_cast<PolymorphicFunctorStorageBase*>(copyOfFunctorStorage);
				} else {
					return nullptr;
				}
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
		template<class Signature, Size Capacity, bool MoveOnly, typename Alloc>
		class FunctionBase {
		public:
			template<typename>
			friend struct FunctionRefBase;
		};

		template<typename ReturnType, typename... ArgTypes, Size Capacity, bool MoveOnly, typename Alloc>
		class FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc> {
		public:
			using result_type = ReturnType;
			using arg_types = TypePack<ArgTypes...>;
			using allocator_type = Alloc;
			using function_ptr_type = ReturnType(*)(ArgTypes...) noexcept;

			//movement info 
			constexpr static bool triviallyRelocatable = true;
			constexpr static bool triviallyDefaultConstructible = true;
			constexpr static bool triviallyCompareable = false;
			constexpr static bool triviallyDestructible = false;
			constexpr static bool triviallyConstRefConstructedable = false;
			constexpr static bool triviallyMoveConstructedable = false;

			constexpr static Size smallBufferSize = Capacity;

			using callable_base = impl::CallableBase<ReturnType, ArgTypes...>;
			using callable_destory_function = typename callable_base::destory_function;
			using base_constexpr_function_storage = impl::BaseConstexprFunctionStorage<ReturnType, ArgTypes...>;

			template<typename>
			friend struct FunctionRefBase;
		private:
			impl::FunctionStorageType functionStorageType;
			union {
				callable_base* heapCallable;
				function_ptr_type functionPtr;
				Byte smallCallableStorage[smallBufferSize];
				base_constexpr_function_storage constexprFunctionStorage;
			};
		public:
			//constructor 
			constexpr FunctionBase() noexcept : functionStorageType(impl::FunctionStorageType::functionPtr), functionPtr(nullptr) {}

			template<Size OtherCap>
			constexpr FunctionBase(const FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept requires(!MoveOnly) {
				assign<OtherCap>(other);
			}
			template<Size OtherCap>
			constexpr FunctionBase(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>&& other) noexcept {
				assign<OtherCap>(natl::forward<decltype(other)>(other));
			}

			template<class Functor>
				requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
			constexpr FunctionBase(Functor&& functor) noexcept {
				assign<Functor>(natl::move(functor));
			}

			//destructors
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

			constexpr ~FunctionBase() noexcept {
				destruct();
			}

			//util 
			constexpr FunctionBase& self() noexcept { return *this; }
			constexpr const FunctionBase& self() const noexcept { return *this; }

			//assignment 
			template<Size OtherCap>
			constexpr FunctionBase& operator=(const FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept requires(!MoveOnly) {
				destruct();
				return assign<OtherCap>(other);
			}
			template<Size OtherCap>
			constexpr FunctionBase& operator=(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>&& other) noexcept {
				destruct();
				return assign<OtherCap>(natl::forward<decltype(other)>(other));
			}
			template<class Functor>
				requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
			constexpr FunctionBase& operator=(Functor&& functor) noexcept {
				destruct();
				return assign(natl::move(functor));
			}

			template<Size OtherCap>
			constexpr FunctionBase& assign(const FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept requires(!MoveOnly)  {
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
			constexpr FunctionBase& assign(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>&& other) noexcept {
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
			constexpr FunctionBase& assign(Functor&& functor) noexcept {
				if constexpr (std::is_function_v<std::remove_pointer_t<Functor>>) {
					functionPtr = static_cast<function_ptr_type>(functor);
					functionStorageType = impl::FunctionStorageType::functionPtr;
					return self();
				}

				using FunctorCallableType = impl::Callable<Alloc, Functor, ReturnType, ArgTypes...>;
				if (std::is_constant_evaluated()) {
					functionStorageType = impl::FunctionStorageType::constexprCallable;
					std::construct_at<base_constexpr_function_storage>(
						&constexprFunctionStorage,
						new impl::PolymorphicFunctorStorage(natl::move(functor)),
						static_cast<impl::ConstexprCallableBase<ReturnType, ArgTypes...>*>(new impl::ConstexprCallable<Functor, ReturnType, ArgTypes...>()));
				} else {
					if constexpr (sizeof(FunctorCallableType) <= smallBufferSize) {
						functionStorageType = impl::FunctionStorageType::smallCallable;
						std::construct_at<FunctorCallableType, Functor>(reinterpret_cast<FunctorCallableType*>(smallCallableStorage), forward<Functor>(functor));
					} else {
						functionStorageType = impl::FunctionStorageType::heapCallable;
						FunctorCallableType* functorCallable = Alloc::template rebind_alloc<FunctorCallableType>::allocate(1);
						std::construct_at<FunctorCallableType>(functorCallable, natl::move(functor));
						heapCallable = static_cast<callable_base*>(functorCallable);
					}
				}
				return self();
			}


			//observers 
			constexpr bool empty() const noexcept {
				if (functionStorageType == impl::FunctionStorageType::functionPtr) {
					return !functionPtr;
				}
				else {
					return false;
				}
			}
			constexpr bool isEmpty() const noexcept { return empty(); }
			constexpr bool isNotEmpty() const noexcept { return !empty(); }
			explicit constexpr operator bool() const noexcept { return isNotEmpty(); }

			//calling
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
						return constexprFunctionStorage.constexprCallable->invoke(constexprFunctionStorage.polymorphicFunctorStorageBase, forward<ArgTypes>(args)...);
					}
				}
			}

			//modifiers
			constexpr void swap(FunctionBase& other) noexcept {
				FunctionBase temp = natl::move(other);
				other = natl::move(self());
				self() = natl::move(temp);
			}
		};
	}

	template<class Signature, Size Capacity = 32 - alignof(char*), typename Alloc = DefaultAllocatorByte>
	class MoveOnlyFunction {
		template<typename>
		friend class FunctionRef;

		template<typename, Size, typename>
		friend class Function;
	};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, true, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) noexcept;

		//movement info 
		constexpr static bool triviallyRelocatable = function_base::triviallyRelocatable;
		constexpr static bool triviallyDefaultConstructible = function_base::triviallyDefaultConstructible;
		constexpr static bool triviallyCompareable = function_base::triviallyCompareable;
		constexpr static bool triviallyDestructible = function_base::triviallyDestructible;
		constexpr static bool triviallyConstRefConstructedable = function_base::triviallyConstRefConstructedable;
		constexpr static bool triviallyMoveConstructedable = function_base::triviallyMoveConstructedable;

		constexpr static Size smallBufferSize = function_base::smallBufferSize;

		template<typename>
		friend class FunctionRef;

		template<typename, Size, typename>
		friend class Function;
	private:
		function_base functionBase;
	public:
		//constructor 
		constexpr MoveOnlyFunction() noexcept : functionBase() {}
		constexpr MoveOnlyFunction(const MoveOnlyFunction&) noexcept = delete;

		template<Size OtherCap>
		constexpr MoveOnlyFunction(const MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr MoveOnlyFunction(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}
		template<Size OtherCap>

		constexpr MoveOnlyFunction(const MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept = delete;
		template<Size OtherCap>
		constexpr MoveOnlyFunction(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
		constexpr MoveOnlyFunction(Functor&& functor) noexcept :
			functionBase(natl::move(functor)) {}

		//destructor
		constexpr ~MoveOnlyFunction() noexcept = default;

		//util 
		constexpr MoveOnlyFunction& self() noexcept { return *this; }
		constexpr const MoveOnlyFunction& self() const noexcept { return *this; }

		//assignment 
		constexpr MoveOnlyFunction& operator=(const MoveOnlyFunction&) noexcept = delete;


		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::forward<decltype(other)>(other);
			return self();
		}

		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::forward<decltype(other)>(other);
			return self();
		}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
		constexpr MoveOnlyFunction& operator=(Functor&& functor) noexcept {
			functionBase = natl::forward<decltype(functor)>(functor);
			return self();
		}

		//observers 
		constexpr bool empty() const noexcept { return functionBase.empty(); }
		constexpr bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator bool() const noexcept { return bool(functionBase); }

		//calling
		constexpr ReturnType invoke(ArgTypes... args) noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}
		constexpr ReturnType operator()(ArgTypes... args) noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}

		//modifiers 
		constexpr void swap(MoveOnlyFunction& other) noexcept {
			MoveOnlyFunction temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(temp);
		}
	};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, true, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) const noexcept;

		//movement info 
		constexpr static bool triviallyRelocatable = function_base::triviallyRelocatable;
		constexpr static bool triviallyDefaultConstructible = function_base::triviallyDefaultConstructible;
		constexpr static bool triviallyCompareable = function_base::triviallyCompareable;
		constexpr static bool triviallyDestructible = function_base::triviallyDestructible;
		constexpr static bool triviallyConstRefConstructedable = function_base::triviallyConstRefConstructedable;
		constexpr static bool triviallyMoveConstructedable = function_base::triviallyMoveConstructedable;

		constexpr static Size smallBufferSize = function_base::smallBufferSize;
	private:
		function_base functionBase;
	public:
		//constructor 
		constexpr MoveOnlyFunction() noexcept : functionBase() {}
		constexpr MoveOnlyFunction(const MoveOnlyFunction&) noexcept = delete;

		template<Size OtherCap>
		constexpr MoveOnlyFunction(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}
		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>&& IsConstCallable<Functor, ReturnType, ArgTypes...>)
		constexpr MoveOnlyFunction(Functor&& functor) noexcept : functionBase(natl::move(functor)) {}

		//destructor
		constexpr ~MoveOnlyFunction() noexcept = default;

		//util 
		constexpr MoveOnlyFunction& self() noexcept { return *this; }
		constexpr const MoveOnlyFunction& self() const noexcept { return *this; }

		//assignment 
		constexpr MoveOnlyFunction& operator=(const MoveOnlyFunction&) noexcept = delete;

		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept = delete;

		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr MoveOnlyFunction& operator=(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::forward<decltype(other)>(other);
			return self();
		}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> && IsConstCallable<Functor, ReturnType, ArgTypes...>)
		constexpr MoveOnlyFunction& operator=(Functor&& functor) noexcept {
			functionBase = natl::forward<decltype(functor)>(functor);
			return self();
		}

		//observers 
		constexpr bool empty() const noexcept { return functionBase.empty(); }
		constexpr bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator bool() const noexcept { return bool(functionBase); }

		//calling
		constexpr ReturnType invoke(ArgTypes... args) const noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}
		constexpr ReturnType operator()(ArgTypes... args) const noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}

		//modifiers 
		constexpr void swap(MoveOnlyFunction& other) noexcept {
			MoveOnlyFunction temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(temp);
		}
	};

	template<class Signature, Size ByteSize, typename Alloc = DefaultAllocatorByte>
	using MoveOnlyFunctionByteSize = MoveOnlyFunction<Signature, ByteSize - alignof(char*), Alloc>;

	template<class Signature, typename Alloc = DefaultAllocatorByte>
	using MoveOnlyFunctionAlloc = MoveOnlyFunctionByteSize<Signature, 32, Alloc>;

	template<class Signature, Size Capacity = 32 - alignof(char*), typename Alloc = DefaultAllocatorByte>
	class Function {
	public:
		template<typename>
		friend class FunctionRef;
	};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class Function<ReturnType(ArgTypes...), Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, false, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) noexcept;

		//movement info 
		constexpr static bool triviallyRelocatable = function_base::triviallyRelocatable;
		constexpr static bool triviallyDefaultConstructible = function_base::triviallyDefaultConstructible;
		constexpr static bool triviallyCompareable = function_base::triviallyCompareable;
		constexpr static bool triviallyDestructible = function_base::triviallyDestructible;
		constexpr static bool triviallyConstRefConstructedable = function_base::triviallyConstRefConstructedable;
		constexpr static bool triviallyMoveConstructedable = function_base::triviallyMoveConstructedable;

		constexpr static Size smallBufferSize = function_base::smallBufferSize;

		template<typename>
		friend class FunctionRef;
	private:
		function_base functionBase;
	public:
		//constructor 
		constexpr Function() noexcept : functionBase() {}

		template<Size OtherCap>
		constexpr Function(const Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept : 
			functionBase(other.functionBase) {}
		template<Size OtherCap>
		constexpr Function(Function<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}
		template<Size OtherCap>
		constexpr Function(const Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept :
			functionBase(other.functionBase) {}
		template<Size OtherCap>
		constexpr Function(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}

		template<Size OtherCap>
		constexpr Function(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}
		template<Size OtherCap>
		constexpr Function(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> && IsCopyConstructible<Functor>)
		constexpr Function(Functor&& functor) noexcept : functionBase(natl::move(functor)) {}

		//destructor
		constexpr ~Function() noexcept = default;

		//util 
		constexpr Function& self() noexcept { return *this; }
		constexpr const Function& self() const noexcept { return *this; }

		//assignment 
		template<Size OtherCap>
		constexpr Function& operator=(const Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept {
			functionBase = other.functionBase;
			return self();
		}
		template<Size OtherCap>
		constexpr Function& operator=(Function<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::move(other.functionBase);
			return self();
		}
		template<Size OtherCap>
		constexpr Function& operator=(const Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept {
			functionBase = other.functionBase;
			return self();
		}
		template<Size OtherCap>
		constexpr Function& operator=(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::move(other.functionBase);
			return self();
		}

		template<Size OtherCap>
		constexpr Function& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr Function& operator=(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::move(other.functionBase);
			return self();
		}

		template<Size OtherCap>
		constexpr Function& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr Function& operator=(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::move(other.functionBase);
			return self();
		}

		template<typename Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> && IsCopyConstructible<Functor>)
		constexpr Function& operator=(Functor&& functor) noexcept {
			functionBase = natl::forward<Functor>(functor);
			return self();
		}

		//observers 
		constexpr bool empty() const noexcept { return functionBase.empty(); }
		constexpr bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator bool() const noexcept { return bool(functionBase); }

		//calling
		constexpr ReturnType invoke(ArgTypes... args) noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}
		constexpr ReturnType operator()(ArgTypes... args) noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}

		//modifiers 
		constexpr void swap(Function& other) noexcept {
			Function temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(temp);
		}
	};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class Function<ReturnType(ArgTypes...) const, Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, false, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) const noexcept;

		//movement info 
		constexpr static bool triviallyRelocatable = function_base::triviallyRelocatable;
		constexpr static bool triviallyDefaultConstructible = function_base::triviallyDefaultConstructible;
		constexpr static bool triviallyCompareable = function_base::triviallyCompareable;
		constexpr static bool triviallyDestructible = function_base::triviallyDestructible;
		constexpr static bool triviallyConstRefConstructedable = function_base::triviallyConstRefConstructedable;
		constexpr static bool triviallyMoveConstructedable = function_base::triviallyMoveConstructedable;

		constexpr static Size smallBufferSize = function_base::smallBufferSize;
	private:
		function_base functionBase;
	public:
		//constructor 
		constexpr Function() noexcept : functionBase() {}

		template<Size OtherCap>
		constexpr Function(const Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr Function(Function<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept = delete;

		template<Size OtherCap>
		constexpr Function(const Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept :
			functionBase(other.functionBase) {}
		template<Size OtherCap>
		constexpr Function(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}

		template<Size OtherCap>
		constexpr Function(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) = delete;
		template<Size OtherCap>
		constexpr Function(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept :
			functionBase(natl::forward<decltype(other.functionBase)>(other.functionBase)) {}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> && IsCopyConstructible<Functor> && IsConstCallable<Functor, ReturnType, ArgTypes...>)
		constexpr Function(Functor&& functor) noexcept : functionBase(natl::move(functor)) {}

		//destructor
		constexpr ~Function() noexcept = default;

		//util 
		constexpr Function& self() noexcept { return *this; }
		constexpr const Function& self() const noexcept { return *this; }

		//assignment 
		template<Size OtherCap>
		constexpr Function& operator=(const Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr Function& operator=(Function<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept = delete;

		template<Size OtherCap>
		constexpr Function& operator=(const Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept {
			functionBase = other.functionBase;
			return self();
		}
		template<Size OtherCap>
		constexpr Function& operator=(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::forward<decltype(other)>(other);
			return self();
		}

		template<Size OtherCap>
		constexpr Function& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr Function& operator=(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>&& other) noexcept = delete;

		template<Size OtherCap>
		constexpr Function& operator=(const MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap>
		constexpr Function& operator=(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>&& other) noexcept {
			functionBase = natl::move(other.functionBase);
			return self();
		}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> && IsCopyConstructible<Functor> && IsConstCallable<Functor, ReturnType, ArgTypes...>)
		constexpr Function& operator=(Functor&& functor) noexcept {
			functionBase = natl::forward<decltype(functor)>(functor);
			return self();
		}

		//observers 
		constexpr bool empty() const noexcept { return functionBase.empty(); }
		constexpr bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator bool() const noexcept { return bool(functionBase); }

		//calling
		constexpr ReturnType invoke(ArgTypes... args) const noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}
		constexpr ReturnType operator()(ArgTypes... args) const noexcept {
			return functionBase.invoke(forward<ArgTypes>(args)...);
		}

		//modifiers 
		constexpr void swap(Function& other) noexcept {
			Function temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(temp);
		}
	};

	template<class Signature, Size ByteSize, typename Alloc = DefaultAllocatorByte>
	using FunctionByteSize = Function<Signature, ByteSize - alignof(char*), Alloc>;

	template<class Signature, typename Alloc = DefaultAllocatorByte>
	using FunctionAlloc = FunctionByteSize<Signature, 32, Alloc>;

	namespace impl {
		template<typename ReturnType, typename... ArgTypes>
		struct CallableRefBase {
			constexpr virtual ReturnType invoke(ArgTypes...) const noexcept = 0;
		};

		template <typename Functor, typename ReturnType, typename... ArgTypes>
		struct CallableRef : public CallableRefBase<ReturnType, ArgTypes...> {
			using callable_ref_base = CallableBase<ReturnType, ArgTypes...>;
		private:
			Functor& functorRef;
		public:
			constexpr CallableRef(Functor& functorRef) noexcept : functorRef(functorRef) {}
			constexpr ~CallableRef() noexcept = default;
			constexpr ReturnType invoke(ArgTypes... args) const noexcept override {
				return functor(forward<ArgTypes>(args)...);
			}
		};

		struct PolymorphicFunctorRefStorageBase {
			//constexpr virtual ~PolymorphicFunctorRefStorageBase() noexcept = default;
		};

		template<class Functor>
		struct PolymorphicFunctorRefStorage : public PolymorphicFunctorRefStorageBase {
			Functor& functorRef;
			constexpr PolymorphicFunctorRefStorage(Functor& functorRef) noexcept : functorRef(functorRef) {}
		};

		template<typename ReturnType, typename... ArgTypes>
		struct ConstexprCallableRefBase {
		public:
			constexpr virtual ReturnType invoke(PolymorphicFunctorRefStorageBase* polymorphicFunctorRefStorageBase, ArgTypes... args) const noexcept = 0;
		};

		template <typename Functor, typename ReturnType, typename... ArgTypes>
		struct ConstexprCallableRef : public ConstexprCallableRefBase<ReturnType, ArgTypes...> {
			using constexpr_callable_ref_base = ConstexprCallableRefBase<ReturnType, ArgTypes...>;

			constexpr ConstexprCallableRef() noexcept = default;
			constexpr ~ConstexprCallableRef() noexcept = default;

			constexpr ReturnType invoke(PolymorphicFunctorRefStorageBase* polymorphicFunctorRefStorageBase, ArgTypes... args) const noexcept override {
				return dynamic_cast<PolymorphicFunctorRefStorage<Functor>*>(polymorphicFunctorRefStorageBase)->functor(forward<ArgTypes>(args)...);
			}
		};

		template<typename ReturnType, typename... ArgTypes>
		struct BaseConstexprFunctionRefStorage {
			using constexpr_callable_ref_base = ConstexprCallableRefBase<ReturnType, ArgTypes...>;

			PolymorphicFunctorRefStorageBase* polymorphicFunctorRefStorageBase;
			constexpr_callable_ref_base* constexpreCallableRef;

			//constructor
			constexpr BaseConstexprFunctionRefStorage() noexcept = default;
			constexpr BaseConstexprFunctionRefStorage(
				PolymorphicFunctorRefStorageBase* polymorphicFunctorRefStorageBase, 
				constexpr_callable_ref_base* constexpreCallableRef) noexcept :
				polymorphicFunctorRefStorageBase(polymorphicFunctorRefStorageBase), 
				constexpreCallableRef(constexpreCallableRef) {}
		};


		enum class FunctionRefStorageType {
			functionPtr = 0,
			callableRef,
			constexprCallableRef,
			callable,
			constexprCallable,
		};

		template<class Signature>
		class FunctionRefBase {};

		template<typename ReturnType, typename... ArgTypes>
		class FunctionRefBase<ReturnType(ArgTypes...)> final {
		public:
			using result_type = ReturnType;
			using arg_types = TypePack<ArgTypes...>;
			using function_ptr_type = ReturnType(*)(ArgTypes...) noexcept;

			//movement info 
			constexpr static bool triviallyRelocatable = true;
			constexpr static bool triviallyDefaultConstructible = true;
			constexpr static bool triviallyCompareable = false;
			constexpr static bool triviallyDestructible = true;
			constexpr static bool triviallyConstRefConstructedable = true;
			constexpr static bool triviallyMoveConstructedable = true;

		private:
			constexpr static Size smallBufferSize = sizeof(void*) * 2;
			using callable_base = CallableBase<ReturnType, ArgTypes...>;
			using callable_ref_base = CallableRefBase<ReturnType, ArgTypes...>;
			using base_constexpr_function_storage = BaseConstexprFunctionStorage<ReturnType, ArgTypes...>;
			using base_constexpr_function_ref_storage = BaseConstexprFunctionRefStorage<ReturnType, ArgTypes...>;
		private:
			FunctionRefStorageType functionRefStorageType;
			union {
				function_ptr_type functionPtr;
				Byte callableRefStorage[smallBufferSize];
				base_constexpr_function_ref_storage constexprFunctionRefStorage;
				callable_base* callable;
				base_constexpr_function_storage constexprFunctionStorage;
			};
		public:
			//constructor
			constexpr FunctionRefBase() noexcept = default;
			constexpr FunctionRefBase(const FunctionRefBase& other) noexcept {
				assign(other);
			}
			constexpr FunctionRefBase(FunctionRefBase&& other) noexcept {
				assign(natl::forward(other));
			}
			template<Size OtherCap, bool MoveOnly, typename Alloc>
			constexpr FunctionRefBase(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept {
				assign<OtherCap, MoveOnly, Alloc>(other);
			}


			//destructor
			constexpr void destruct() noexcept {
				if (std::is_constant_evaluated()) {
					if (functionRefStorageType == FunctionRefStorageType::constexprCallableRef) {
						delete constexprFunctionRefStorage.constexpreCallableRef;
						delete constexprFunctionRefStorage.polymorphicFunctorRefStorageBase;
						std::destroy_at<base_constexpr_function_ref_storage>(&constexprFunctionRefStorage);
					}
				}
			}
			constexpr ~FunctionRefBase() noexcept {
				destruct();
			}

			//util 
			constexpr FunctionRefBase& self() noexcept { return *this; }
			constexpr const FunctionRefBase& self() const noexcept { return *this; }

			//assignment 
			constexpr FunctionRefBase& operator=(const FunctionRefBase& other) noexcept {
				destruct();
				return assign(other);
			}
			constexpr FunctionRefBase& operator=(FunctionRefBase&& other) noexcept {
				destruct();
				return assign(natl::forward(other));
			}

			template<class Functor>
				requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
			constexpr FunctionRefBase& operator=(Functor& functor) noexcept {
				destruct();
				return assign(functor);
			}

			template<Size OtherCap, bool MoveOnly, typename Alloc>
			constexpr FunctionRefBase& operator=(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept {
				destruct();
				return assign<OtherCap, MoveOnly, Alloc>(other);
			}


		private:
			//assignment 
			constexpr FunctionRefBase& assign(const FunctionRefBase& other) noexcept {
				functionRefStorageType = other.functionRefStorageType;
				switch (functionRefStorageType) {
				case FunctionRefStorageType::functionPtr:
					functionPtr = other.functionPtr;
					break;
				case FunctionRefStorageType::callableRef:
					copyNoOverlap<const Byte*, Byte*>(&other.callableRefStorage, &other.callableRefStorage + smallBufferSize, callableRefStorage);
					break;
				case FunctionRefStorageType::constexprCallableRef:
					if (std::is_constant_evaluated()) {
						constexprFunctionRefStorage = constexprFunctionRefStorage;
					}
					break;
				case FunctionRefStorageType::callable:
					callable = callable;
					break;
				case FunctionRefStorageType::constexprCallable:
					if (std::is_constant_evaluated()) {
						constexprFunctionStorage = constexprFunctionStorage;
					}
					break;
				}

				return self();
			}
			constexpr FunctionRefBase& assign(FunctionRefBase&& other) noexcept {
				functionRefStorageType = other.functionRefStorageType;
				switch (functionRefStorageType) {
				case FunctionRefStorageType::functionPtr:
					functionPtr = other.functionPtr;
					break;
				case FunctionRefStorageType::callableRef:
					copyNoOverlap<const Byte*, Byte*>(
						&other.callableRefStorage,
						&other.callableRefStorage + smallBufferSize,
						callableRefStorage);
					break;
				case FunctionRefStorageType::constexprCallableRef:
					if (std::is_constant_evaluated()) {
						constexprFunctionRefStorage = constexprFunctionRefStorage;
					}
					break;
				case FunctionRefStorageType::callable:
					callable = callable;
					break;
				case FunctionRefStorageType::constexprCallable:
					if (std::is_constant_evaluated()) {
						constexprFunctionStorage = constexprFunctionStorage;
					}
					break;
				}

				other.functionRefStorageType = FunctionRefStorageType::functionPtr;
				other.functionPtr = nullptr;

				return self();
			}

			template<class Functor>
				requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
			constexpr FunctionRefBase& assign(Functor& functor) noexcept {
				if constexpr(std::is_convertible_v<Functor, ReturnType(ArgTypes...)>) {
					functionPtr = functor;
					functionRefStorageType = FunctionRefStorageType::functionPtr;
					return self();
				}

				if (std::is_constant_evaluated()) {
					functionRefStorageType = FunctionRefStorageType::constexprCallableRef;
					std::construct_at<base_constexpr_function_ref_storage>(
						&constexprFunctionRefStorage,
						new PolymorphicFunctorRefStorage(functor),
						static_cast<ConstexprCallableRefBase<ReturnType, ArgTypes...>*>(new ConstexprCallableRef<Functor, ReturnType, ArgTypes...>()));
				} else {
					using FunctorCallableRefType = CallableRef<Functor, ReturnType, ArgTypes...>;
					functionRefStorageType = FunctionRefStorageType::callableRef;
					std::construct_at<FunctorCallableRefType, Functor&>(reinterpret_cast<FunctorCallableRefType*>(callableRefStorage), functor);
				}
				return self();
			}

			template<Size OtherCap, bool MoveOnly, typename Alloc>
			constexpr FunctionRefBase& assign(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept {
				switch (other.functionStorageType) {
				case FunctionStorageType::functionPtr:
					functionRefStorageType = FunctionRefStorageType::functionPtr;
					functionPtr = other.functionPtr;
					break;
				case FunctionStorageType::smallCallable:
					functionRefStorageType = FunctionRefStorageType::callable;
					callable = reinterpret_cast<callable_base*>(other.smallCallableStorage);
					break;
				case FunctionStorageType::heapCallable:
					functionRefStorageType = FunctionRefStorageType::callable;
					callable = other.heapCallable;
					break;
				case FunctionStorageType::constexprCallable:
					if (std::is_constant_evaluated()) {
						functionRefStorageType = FunctionRefStorageType::constexprCallable;
						constexprFunctionStorage = other.constexprFunctionStorage;
					}
					break;
				}
				return self();
			}

		public:

			//observers 
			constexpr bool empty() const noexcept {
				if (functionRefStorageType == FunctionRefStorageType::functionPtr) {
					return !functionPtr;
				} else {
					return false;
				}
			}
			constexpr bool isEmpty() const noexcept { return empty(); }
			constexpr bool isNotEmpty() const noexcept { return !empty(); }
			explicit constexpr operator bool() const noexcept { return isNotEmpty(); }

			//calling
			constexpr ReturnType operator()(ArgTypes... args) const noexcept {
				return invoke(forward<ArgTypes>(args)...);
			}

			constexpr ReturnType invoke(ArgTypes... args) const noexcept {
				switch (functionRefStorageType) {
				case FunctionRefStorageType::functionPtr:
					return functionPtr(forward<ArgTypes>(args)...);
				case FunctionRefStorageType::callableRef:
					return reinterpret_cast<const callable_ref_base*>(callableRefStorage)->invoke(forward<ArgTypes>(args)...);
				case FunctionRefStorageType::constexprCallableRef:
					if (std::is_constant_evaluated()) {
						constexprFunctionRefStorage.constexpreCallableRef->invoke(constexprFunctionRefStorage.polymorphicFunctorRefStorageBase, forward<ArgTypes>(args)...);
					}
					break;
				case FunctionRefStorageType::callable:
					callable->invoke(forward<ArgTypes>(args)...);
					break;
				case FunctionRefStorageType::constexprCallable:
					if (std::is_constant_evaluated()) {
						return constexprFunctionStorage.constexprCallable->invoke(constexprFunctionStorage.polymorphicFunctorStorageBase, forward<ArgTypes>(args)...);
					}
					break;
				}
			}

			//modifiers
			constexpr void swap(FunctionRefBase& other) noexcept {
				FunctionRefBase temp = natl::move(other);
				other = natl::move(self());
				self() = natl::move(temp);
			}
		};
	}

	template<class Signature>
	class FunctionRef {};

	template<typename ReturnType, typename... ArgTypes>
	class FunctionRef<ReturnType(ArgTypes...)> final {
	public:
		using function_ref_base = impl::FunctionRefBase<ReturnType(ArgTypes...)>;
		using result_type = function_ref_base::result_type;
		using arg_types = function_ref_base::arg_types;
		using function_signature = ReturnType(ArgTypes...) noexcept;
		//movement info 
		constexpr static bool triviallyRelocatable = function_ref_base::triviallyRelocatable;
		constexpr static bool triviallyDefaultConstructible = function_ref_base::triviallyDefaultConstructible;
		constexpr static bool triviallyCompareable = function_ref_base::triviallyCompareable;
		constexpr static bool triviallyDestructible = function_ref_base::triviallyDestructible;
		constexpr static bool triviallyConstRefConstructedable = function_ref_base::triviallyConstRefConstructedable;
		constexpr static bool triviallyMoveConstructedable = function_ref_base::triviallyMoveConstructedable;

	private:
		function_ref_base functionRefBase;
	public:
		//constructor 
		constexpr FunctionRef() noexcept : functionRefBase() {}
		constexpr FunctionRef(const FunctionRef& other) noexcept : 
			functionRefBase(other.functionRefBase) {}
		constexpr FunctionRef(FunctionRef&& other) noexcept :
			functionRefBase(natl::forward(other.functionRefBase)) {}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept : 
			functionRefBase(other.functionBase) {}
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept :
			functionRefBase(other.functionBase) {}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept :
			functionRefBase(other.functionBase) {}
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept :
			functionRefBase(other.functionBase) {}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
		constexpr FunctionRef(Functor& functor) noexcept : 
			functionRefBase(functor) {}

		//destructor
		constexpr ~FunctionRef() noexcept = default;

		//util 
		constexpr FunctionRef& self() noexcept { return *this; }
		constexpr const FunctionRef& self() const noexcept { return *this; }

		//assignment 
		constexpr FunctionRef& operator=(const FunctionRef& other) noexcept {
			functionRefBase = other.functionRefBase;
			return self();
		}
		constexpr FunctionRef& operator=(FunctionRef&& other) noexcept {
			functionRefBase = natl::forward(other.functionRefBase);
			return self();
		}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept {
			functionRefBase = other.functionBase;
			return self();
		}
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept {
			functionRefBase = other.functionBase;
			return self();
		}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept {
			functionRefBase = other.functionBase;
			return self();
		}
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept {
			functionRefBase = other.functionBase;
			return self();
		}
		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
		constexpr FunctionRef& operator=(Functor& functor) noexcept {
			functionRefBase = functor;
			return self();
		}

		//observers 
		constexpr bool empty() const noexcept { return functionRefBase.empty(); }
		constexpr bool isEmpty() const noexcept { return functionRefBase.isEmpty(); }
		constexpr bool isNotEmpty() const noexcept { return functionRefBase.isNotEmpty(); }
		explicit constexpr operator bool() const noexcept { return bool(functionRefBase); }

		//calling
		constexpr ReturnType invoke(ArgTypes... args) noexcept {
			return functionRefBase.invoke(forward<ArgTypes>(args)...);
		}
		constexpr ReturnType operator()(ArgTypes... args) noexcept {
			return functionRefBase.invoke(forward<ArgTypes>(args)...);
		}

		//modifiers 
		constexpr void swap(FunctionRef& other) noexcept {
			FunctionRef temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(temp);
		}
	};

	template<typename ReturnType, typename... ArgTypes>
	class FunctionRef<ReturnType(ArgTypes...) const> final {
	public:
		using function_ref_base = impl::FunctionRefBase<ReturnType(ArgTypes...)>;
		using result_type = function_ref_base::result_type;
		using arg_types = function_ref_base::arg_types;
		using function_signature = ReturnType(ArgTypes...) const noexcept;
		//movement info 
		constexpr static bool triviallyRelocatable = function_ref_base::triviallyRelocatable;
		constexpr static bool triviallyDefaultConstructible = function_ref_base::triviallyDefaultConstructible;
		constexpr static bool triviallyCompareable = function_ref_base::triviallyCompareable;
		constexpr static bool triviallyDestructible = function_ref_base::triviallyDestructible;
		constexpr static bool triviallyConstRefConstructedable = function_ref_base::triviallyConstRefConstructedable;
		constexpr static bool triviallyMoveConstructedable = function_ref_base::triviallyMoveConstructedable;

	private:
		function_ref_base functionRefBase;
	public:
		//constructor 
		constexpr FunctionRef() noexcept : functionRefBase() {}
		constexpr FunctionRef(const FunctionRef& other) noexcept :
			functionRefBase(other.functionRefBase) {}
		constexpr FunctionRef(FunctionRef&& other) noexcept :
			functionRefBase(natl::forward(other.functionRefBase)) {}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept :
			functionRefBase(other.functionBase) {}
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept :
			functionRefBase(other.functionBase) {}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept :
			functionRefBase(other.functionBase) {}
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept :
			functionRefBase(other.functionBase) {}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> && IsConstCallable<Functor, ReturnType, ArgTypes...>)
		constexpr FunctionRef(Functor& functor) noexcept :
			functionRefBase(functor) {}

		//destructor
		constexpr ~FunctionRef() noexcept = default;

		//util 
		constexpr FunctionRef& self() noexcept { return *this; }
		constexpr const FunctionRef& self() const noexcept { return *this; }

		//assignment 
		constexpr FunctionRef& operator=(const FunctionRef& other) noexcept {
			functionRefBase = other.functionRefBase;
			return self();
		}
		constexpr FunctionRef& operator=(FunctionRef&& other) noexcept {
			functionRefBase = natl::forward(other.functionRefBase);
			return self();
		}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(MoveOnlyFunction<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(MoveOnlyFunction<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept {
			functionRefBase = other.functionBase;
			return self();
		}

		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(Function<ReturnType(ArgTypes...), OtherCap, Alloc>& other) noexcept = delete;
		template<Size OtherCap, typename Alloc>
		constexpr FunctionRef& operator=(Function<ReturnType(ArgTypes...) const, OtherCap, Alloc>& other) noexcept {
			functionRefBase = other.functionBase;
			return self();
		}

		template<class Functor>
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>&& IsConstCallable<Functor, ReturnType, ArgTypes...>)
		constexpr FunctionRef& operator=(Functor& functor) noexcept {
			functionRefBase = functor;
			return self();
		}


		//observers 
		constexpr bool empty() const noexcept { return functionRefBase.empty(); }
		constexpr bool isEmpty() const noexcept { return functionRefBase.isEmpty(); }
		constexpr bool isNotEmpty() const noexcept { return functionRefBase.isNotEmpty(); }
		explicit constexpr operator bool() const noexcept { return bool(functionRefBase); }

		//calling
		constexpr ReturnType invoke(ArgTypes... args) const noexcept {
			return functionRefBase.invoke(forward<ArgTypes>(args)...);
		}
		constexpr ReturnType operator()(ArgTypes... args) const noexcept {
			return functionRefBase.invoke(forward<ArgTypes>(args)...);
		}

		//modifiers 
		constexpr void swap(FunctionRef& other) noexcept {
			FunctionRef temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(temp);
		}

	};
	
	template<class Type>
	struct LessCompare { 
		constexpr bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs < rhs; }
	};
	template<class Type>
	struct GreaterCompare {
		constexpr bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs > rhs; }
	};
	template<class Type>
	struct LessEqualCompare {
		constexpr bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs <= rhs; }
	};
	template<class Type>
	struct GreaterEqualCompare {
		constexpr bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs >= rhs; }
	};
	template<class Type>
	struct EqualToCompare {
		constexpr bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs == rhs; }
	};
	template<class Type>
	struct NotEqualToCompare {
		constexpr bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs != rhs; }
	};

}