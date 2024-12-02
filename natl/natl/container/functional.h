#pragma once

//own
#include "../util/basicTypes.h"
#include "../util/typeTraits.h"
#include "../util/dataMovement.h"
#include "../util/allocator.h"
#include "../util/typePack.h"

//interface
namespace natl {
#ifdef NATL_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4265)
#endif // NATL_COMPILER_MSVC

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
			constexpr Callable(const Functor& functorIn) noexcept requires(IsCopyConstructibleC<Functor>) : functor(functorIn) {}
			constexpr Callable(Functor&& functorIn) noexcept : functor(natl::move(functorIn)) {}

			constexpr callable_base* copyCallable(callable_base* location) const noexcept override {
				if constexpr (IsCopyConstructibleC<Functor>) {
					if (location) {
						natl::construct<Callable>(static_cast<Callable*>(location), functor);
						return nullptr;
					} else {
						Callable* newCallable = Alloc::template rebind_alloc<Callable>::allocate(1);
						natl::construct<Callable>(newCallable, functor);
						return static_cast<callable_base*>(newCallable);
					}
				} else {
					return nullptr;
				}
			}
			constexpr callable_base* moveCallable(callable_base* location) noexcept override {
				if (location) {
					natl::construct<Callable>(static_cast<Callable*>(location), natl::move(functor));
					return nullptr;
				} else {
					Callable* newCallable = Alloc::template rebind_alloc<Callable>::allocate(1);
					natl::construct<Callable>(newCallable, natl::move(functor));
					return static_cast<callable_base*>(newCallable);
				}
			}
			constexpr destory_function getDestoryFunction() const noexcept override {
				return [](callable_base* location) noexcept -> void {
					Callable* destoryCallable = static_cast<Callable*>(location);
					natl::deconstruct<Callable>(destoryCallable);
					Alloc::template rebind_alloc<Callable>::deallocate(destoryCallable, 1);
				};
			}
			constexpr ReturnType invoke(ArgTypes... args) const noexcept override {
				return functor(forward<ArgTypes>(args)...);
			}
		};

		template<typename ReturnType, typename... ArgTypes>
		struct ConstexprCallableBase {
			using invoke_function_type = ReturnType(*)(ConstexprCallableBase*, ArgTypes...) noexcept;
			using copy_functor_storage_function_type = ConstexprCallableBase*(*)(const ConstexprCallableBase*) noexcept;
			using destory_callable_function_type = void(*)(ConstexprCallableBase*) noexcept;

			constexpr virtual invoke_function_type getInvokeFunction() const noexcept = 0;
			constexpr virtual copy_functor_storage_function_type getCopyFunctorStorageFunction() const noexcept = 0;
			constexpr virtual destory_callable_function_type getDestoryFunction() const noexcept = 0;
		};

		template <typename Functor, typename ReturnType, typename... ArgTypes>
		struct ConstexprCallable final : public ConstexprCallableBase<ReturnType, ArgTypes...> {
			using constexpr_callable_base = ConstexprCallableBase<ReturnType, ArgTypes...>;

			using typename constexpr_callable_base::invoke_function_type;
			using typename constexpr_callable_base::copy_functor_storage_function_type;
			using typename constexpr_callable_base::destory_callable_function_type;

			Functor functor;
			constexpr ConstexprCallable(const Functor& functorIn) noexcept requires(IsCopyConstructibleC<Functor>) : functor(functorIn) {}
			constexpr ConstexprCallable(Functor&& functorIn) noexcept : functor(natl::move(functorIn)) {}

			constexpr ~ConstexprCallable() noexcept = default;

			constexpr invoke_function_type getInvokeFunction() const noexcept override {
				return [](constexpr_callable_base* callableBase, ArgTypes... args) noexcept -> ReturnType {
					return static_cast<ConstexprCallable*>(callableBase)->functor(forward<ArgTypes>(args)...);
				};
			}

			constexpr copy_functor_storage_function_type getCopyFunctorStorageFunction() const noexcept override {
				return [](const constexpr_callable_base* callableBase) noexcept -> constexpr_callable_base* {
					if constexpr (IsCopyConstructibleC<Functor>) {
						using constexpr_callable_alloc = DefaultAllocator<ConstexprCallable>;
						ConstexprCallable* newCallable = constexpr_callable_alloc::allocate(1);
						natl::construct<ConstexprCallable>(newCallable, static_cast<const ConstexprCallable*>(callableBase)->functor);

						return static_cast<constexpr_callable_base*>(newCallable);
					} else {
						return nullptr;
					}
				};
			}

			constexpr destory_callable_function_type getDestoryFunction() const noexcept override {
				return [](constexpr_callable_base* callableBase) noexcept -> void {
					ConstexprCallable* constexprCallable = static_cast<ConstexprCallable*>(callableBase);
					natl::deconstruct(constexprCallable);
					natl::DefaultAllocator<ConstexprCallable>::deallocate(constexprCallable, 1);
				};
			}
		};

		enum class FunctionStorageType {
			functionPtr = 0,
			smallCallable = 1,
			heapCallable = 2,
			constexprCallable = 3,
		};
		template<class Signature, Size Capacity, Bool MoveOnly, typename Alloc>
		class FunctionBase {
		public:
			template<typename>
			friend struct FunctionRefBase;
		};

		template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
		class FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc> {
		public:
			using result_type = ReturnType;
			using arg_types = TypePack<ArgTypes...>;
			using allocator_type = Alloc;
			using function_ptr_type = ReturnType(*)(ArgTypes...) noexcept;

			constexpr static Size smallBufferSize = Capacity;

			using callable_base = impl::CallableBase<ReturnType, ArgTypes...>;
			using callable_destory_function = callable_base::destory_function;
			using constexpr_callable_base = ConstexprCallableBase<ReturnType, ArgTypes...>;

			template<typename>
			friend struct FunctionRefBase;
		private:
			impl::FunctionStorageType functionStorageType;
			natl::ui32 numberOfBytesUsed;
			union {
				callable_base* heapCallable;
				function_ptr_type functionPtr;
				Byte smallCallableStorage[smallBufferSize];
				constexpr_callable_base* constexprCallable;
			};
		public:
			//constructor 
			constexpr FunctionBase() noexcept : 
				functionStorageType(impl::FunctionStorageType::functionPtr),
				numberOfBytesUsed(0),
				functionPtr(nullptr) {}

			template<Size OtherCap>
			constexpr FunctionBase(const FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept requires(!MoveOnly) {
				numberOfBytesUsed = 0;
				assign<OtherCap>(other);
			}
			template<Size OtherCap>
			constexpr FunctionBase(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>&& other) noexcept {
				numberOfBytesUsed = 0;
				assign<OtherCap>(natl::forward<decltype(other)>(other));
			}

			template<class Functor>
				requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
			constexpr FunctionBase(Functor&& functor) noexcept {
				numberOfBytesUsed = 0;
				assign<Functor>(natl::move(functor));
			}

			//destructors
			constexpr void internalDeconstruct() noexcept {
				switch (functionStorageType) {
				case impl::FunctionStorageType::smallCallable:
					natl::deconstruct<callable_base>(reinterpret_cast<callable_base*>(smallCallableStorage));
					break;
				case impl::FunctionStorageType::heapCallable: {
					callable_destory_function destoryFunction = heapCallable->getDestoryFunction();
					destoryFunction(heapCallable);
				}
					break;
				case impl::FunctionStorageType::constexprCallable:
					if (isConstantEvaluated()) {
						constexprCallable->getDestoryFunction()(constexprCallable);
					} else {
						unreachable();
					}
					break;
				default:
					break;
				}
			}

			constexpr ~FunctionBase() noexcept {
				internalDeconstruct();
			}

			//util 
			constexpr FunctionBase& self() noexcept { return *this; }
			constexpr const FunctionBase& self() const noexcept { return *this; }

			//assignment 
			template<Size OtherCap>
			constexpr FunctionBase& operator=(const FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept requires(!MoveOnly) {
				internalDeconstruct();
				return assign<OtherCap>(other);
			}
			template<Size OtherCap>
			constexpr FunctionBase& operator=(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>&& other) noexcept {
				internalDeconstruct();
				return assign<OtherCap>(natl::forward<decltype(other)>(other));
			}
			template<class Functor>
				requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...>)
			constexpr FunctionBase& operator=(Functor&& functor) noexcept {
				internalDeconstruct();
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
						numberOfBytesUsed = other.numberOfBytesUsed;
						if (other.numberOfBytesUsed <= Capacity) {
							reinterpret_cast<callable_base*>(other.smallCallableStorage)->copyCallable(reinterpret_cast<callable_base*>(smallCallableStorage));
						} else {
							functionStorageType = impl::FunctionStorageType::heapCallable;
							heapCallable = reinterpret_cast<callable_base*>(other.smallCallableStorage)->copyCallable(nullptr);
						}
						break;
					case impl::FunctionStorageType::heapCallable:
						numberOfBytesUsed = other.numberOfBytesUsed;
						if (other.numberOfBytesUsed <= Capacity) {
							functionStorageType = impl::FunctionStorageType::smallCallable;
							other.heapCallable->copyCallable(reinterpret_cast<callable_base*>(smallCallableStorage));
						} else {
							heapCallable = other.heapCallable->copyCallable(nullptr);
						}
						break;
					case impl::FunctionStorageType::constexprCallable:
						if (isConstantEvaluated()) {
							natl::construct<constexpr_callable_base>(
								&constexprCallable,
								other.constexprCallable->getCopyFunctorStorageFunction()(other.constexprCallable));
						} else {
							unreachable();
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
						numberOfBytesUsed = other.numberOfBytesUsed;
						if (other.numberOfBytesUsed <= Capacity) {
							reinterpret_cast<callable_base*>(other.smallCallableStorage)->moveCallable(reinterpret_cast<callable_base*>(smallCallableStorage));
						} else {
							functionStorageType = impl::FunctionStorageType::heapCallable;
							heapCallable = reinterpret_cast<callable_base*>(other.smallCallableStorage)->moveCallable(nullptr);
						}
						break;
					case impl::FunctionStorageType::heapCallable:
						numberOfBytesUsed = other.numberOfBytesUsed;
						if (other.numberOfBytesUsed <= Capacity) {
							functionStorageType = impl::FunctionStorageType::smallCallable;
							heapCallable->moveCallable(reinterpret_cast<callable_base*>(smallCallableStorage));
						} else {
							heapCallable = other.heapCallable;
						}
						break;
					case impl::FunctionStorageType::constexprCallable:
						if (isConstantEvaluated()) {
							natl::construct<constexpr_callable_base*>(
								&constexprCallable,
								other.constexprCallable);
						} else {
							unreachable();
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
				if constexpr (ConvertibleTo<RemoveReference<Functor>, ReturnType(*)(ArgTypes...) noexcept>) {
					functionPtr = static_cast<function_ptr_type>(functor);
					functionStorageType = impl::FunctionStorageType::functionPtr;
					return self();
				} else {
					using FunctorCallableType = impl::Callable<Alloc, Functor, ReturnType, ArgTypes...>;
					if (isConstantEvaluated()) {
						using constexpr_callable_type = impl::ConstexprCallable<Functor, ReturnType, ArgTypes...>;
						using constexpr_callable_type_alloc = DefaultAllocator<constexpr_callable_type>;
						constexpr_callable_type* newConstexprCallable = constexpr_callable_type_alloc::allocate(1);
						natl::construct<constexpr_callable_type>(newConstexprCallable, natl::move(functor));

						functionStorageType = impl::FunctionStorageType::constexprCallable;
						natl::construct<constexpr_callable_base*>(
							&constexprCallable,
							static_cast<constexpr_callable_base*>(newConstexprCallable));
					} else {
						if constexpr (sizeof(FunctorCallableType) <= smallBufferSize) {
							numberOfBytesUsed = TypeByteSize<FunctorCallableType>;
							functionStorageType = impl::FunctionStorageType::smallCallable;
							natl::construct<FunctorCallableType, Functor>(reinterpret_cast<FunctorCallableType*>(smallCallableStorage), forward<Functor>(functor));
						} else {
							numberOfBytesUsed = TypeByteSize<FunctorCallableType>;
							functionStorageType = impl::FunctionStorageType::heapCallable;
							FunctorCallableType* functorCallable = Alloc::template rebind_alloc<FunctorCallableType>::allocate(1);
							natl::construct<FunctorCallableType>(functorCallable, natl::move(functor));
							heapCallable = static_cast<callable_base*>(functorCallable);
						}
					}
					return self();
				}
			}


			//observers 
			constexpr Bool empty() const noexcept {
				if (functionStorageType == impl::FunctionStorageType::functionPtr) {
					return !functionPtr;
				}
				else {
					return false;
				}
			}
			constexpr Bool isEmpty() const noexcept { return empty(); }
			constexpr Bool isNotEmpty() const noexcept { return !empty(); }
			explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

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
					if (isConstantEvaluated()) {
						return constexprCallable->getInvokeFunction()(constexprCallable, forward<ArgTypes>(args)...);
					} else {
						unreachable();
					}
				default:
					unreachable();
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

	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyCompareableV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyRelocatableV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyConstructibleV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyDestructibleV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyConstRefConstructibleV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyMoveConstructibleV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyConstRefAssignableV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, Bool MoveOnly, typename Alloc>
	struct IsTriviallyMoveAssignableV<impl::FunctionBase<ReturnType(ArgTypes...), Capacity, MoveOnly, Alloc>>
		: FalseType {};

	//main types 
	template<class Signature, Size Capacity = 32 - alignof(char*), typename Alloc = DefaultAllocatorByte>
	class MoveOnlyFunction {
		template<typename>
		friend class FunctionRef;

		template<typename, Size, typename>
		friend class Function;
	};

	template<class Signature, Size Capacity = 32 - alignof(char*), typename Alloc = DefaultAllocatorByte>
	class Function {
	public:
		template<typename>
		friend class FunctionRef;
	};

	template<typename Signature>
	class FunctionRef {};

	namespace impl {
		template<typename Functor>
		struct IsNotNatlFunction : TrueType {};
		template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
		struct IsNotNatlFunction<Function<ReturnType(ArgTypes...), Capacity, Alloc>> : FalseType {};
		template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
		struct IsNotNatlFunction<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>> : FalseType {};
		template<typename Functor>
		constexpr inline Bool IsNotNatlFunctionV = IsNotNatlFunction<Functor>::value;

		template<typename Functor>
		struct IsNotNatlMoveOnlyFunction : TrueType {};
		template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
		struct IsNotNatlMoveOnlyFunction<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>> : FalseType {};
		template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
		struct IsNotNatlMoveOnlyFunction<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>> : FalseType {};
		template<typename Functor>
		constexpr inline Bool IsNotNatlMoveOnlyFunctionV = IsNotNatlMoveOnlyFunction<Functor>::value;

		template<typename Functor>
		struct IsNotNatlFunctionRef : TrueType {};
		template<typename Signature>
		struct IsNotNatlFunctionRef<FunctionRef<Signature>> : FalseType {};
		template<typename Functor>
		constexpr inline Bool IsNotNatlFunctionRefV = FunctionRef<Functor>::value;
	}


	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, true, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) noexcept;

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
		constexpr Bool empty() const noexcept { return functionBase.empty(); }
		constexpr Bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr Bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator Bool() const noexcept { return Bool(functionBase); }
		constexpr function_base& getFunctionBase() noexcept { return functionBase; }
		constexpr const function_base& getFunctionBase() const noexcept { return functionBase; }

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
	struct IsTriviallyCompareableV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyRelocatableV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstructibleV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyDestructibleV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefConstructibleV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveConstructibleV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefAssignableV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveAssignableV<MoveOnlyFunction<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, true, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) const noexcept;

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
		constexpr Bool empty() const noexcept { return functionBase.empty(); }
		constexpr Bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr Bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator Bool() const noexcept { return Bool(functionBase); }
		constexpr function_base& getFunctionBase() noexcept { return functionBase; }
		constexpr const function_base& getFunctionBase() const noexcept { return functionBase; }

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

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyCompareableV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyRelocatableV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstructibleV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyDestructibleV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefConstructibleV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveConstructibleV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefAssignableV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveAssignableV<MoveOnlyFunction<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

	template<class Signature, Size ByteSize, typename Alloc = DefaultAllocatorByte>
	using MoveOnlyFunctionByteSize = MoveOnlyFunction<Signature, ByteSize - alignof(char*), Alloc>;

	template<class Signature, typename Alloc = DefaultAllocatorByte>
	using MoveOnlyFunctionAlloc = MoveOnlyFunctionByteSize<Signature, 32, Alloc>;

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class Function<ReturnType(ArgTypes...), Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, false, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) noexcept;

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
			requires(
			HasFunctionSignature<Functor, ReturnType, ArgTypes...> 
			&& IsCopyConstructibleC<Functor>
			&& impl::IsNotNatlFunctionV<Decay<Functor>>)
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
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> && IsCopyConstructibleC<Functor>)
		constexpr Function& operator=(Functor&& functor) noexcept {
			functionBase = natl::forward<Functor>(functor);
			return self();
		}

		//observers 
		constexpr Bool empty() const noexcept { return functionBase.empty(); }
		constexpr Bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr Bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator Bool() const noexcept { return Bool(functionBase); }
		constexpr function_base& getFunctionBase() noexcept { return functionBase; }
		constexpr const function_base& getFunctionBase() const noexcept { return functionBase; }

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
	struct IsTriviallyCompareableV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyRelocatableV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstructibleV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyDestructibleV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefConstructibleV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveConstructibleV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefAssignableV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveAssignableV<Function<ReturnType(ArgTypes...), Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	class Function<ReturnType(ArgTypes...) const, Capacity, Alloc> final {
	public:
		using function_base = impl::FunctionBase<ReturnType(ArgTypes...), Capacity, false, Alloc>;
		using result_type = function_base::result_type;
		using arg_types = function_base::arg_types;
		using allocator_type = function_base::allocator_type;
		using function_signature = ReturnType(ArgTypes...) const noexcept;

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
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> 
				&& IsCopyConstructibleC<Functor> 
				&& IsConstCallable<Functor, ReturnType, ArgTypes...>)
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
			requires(HasFunctionSignature<Functor, ReturnType, ArgTypes...> 
				&& IsCopyConstructibleC<Functor>
				&& IsConstCallable<Functor, ReturnType, ArgTypes...>)
		constexpr Function& operator=(Functor&& functor) noexcept {
			functionBase = natl::forward<decltype(functor)>(functor);
			return self();
		}

		//observers 
		constexpr Bool empty() const noexcept { return functionBase.empty(); }
		constexpr Bool isEmpty() const noexcept { return functionBase.isEmpty(); }
		constexpr Bool isNotEmpty() const noexcept { return functionBase.isNotEmpty(); }
		explicit constexpr operator Bool() const noexcept { return Bool(functionBase); }
		constexpr function_base& getFunctionBase() noexcept { return functionBase; }
		constexpr const function_base& getFunctionBase() const noexcept { return functionBase; }

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

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyCompareableV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyRelocatableV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstructibleV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyDestructibleV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefConstructibleV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveConstructibleV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyConstRefAssignableV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes, Size Capacity, typename Alloc>
	struct IsTriviallyMoveAssignableV<Function<ReturnType(ArgTypes...) const, Capacity, Alloc>>
		: FalseType {};

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
			constexpr CallableRef(Functor& functorRefIn) noexcept : functorRef(functorRefIn) {}
			constexpr ~CallableRef() noexcept = default;
			constexpr ReturnType invoke(ArgTypes... args) const noexcept override {
				return functor(forward<ArgTypes>(args)...);
			}
		};

		template<typename ReturnType, typename... ArgTypes>
		struct ConstexprCallableRefBase {
			using invoke_function_type = ReturnType(*)(ConstexprCallableRefBase*, ArgTypes...) noexcept;
			using destory_function_type = void(*)(ConstexprCallableRefBase*) noexcept;
			constexpr virtual invoke_function_type getInvokeFunction() const noexcept = 0;
			constexpr virtual destory_function_type getDestroyFunction() const noexcept = 0;
		};

		template <typename Functor, typename ReturnType, typename... ArgTypes>
		struct ConstexprCallableRef final : public ConstexprCallableRefBase<ReturnType, ArgTypes...> {
			using constexpr_callable_ref_base = ConstexprCallableRefBase<ReturnType, ArgTypes...>;
			using typename constexpr_callable_ref_base::invoke_function_type;
			using typename constexpr_callable_ref_base::destory_function_type;

			Functor& functorRef;

			constexpr ConstexprCallableRef() noexcept = default;
			constexpr ConstexprCallableRef(Functor& functorRefIn) noexcept : functorRef(functorRefIn) {}
			constexpr ~ConstexprCallableRef() noexcept = default;

			constexpr virtual invoke_function_type getInvokeFunction() const noexcept override {
				return [](constexpr_callable_ref_base* callableRefBase, ArgTypes... args) -> ReturnType {
					return static_cast<constexpr_callable_ref_base*>(callableRefBase)->functor(forward<ArgTypes>(args)...);
				};
			}
			constexpr virtual destory_function_type getDestroyFunction() const noexcept override {
				return [](constexpr_callable_ref_base* callableRefBase) noexcept -> ReturnType {
					ConstexprCallableRef* callableRef = static_cast<ConstexprCallableRef*>(callableRefBase);
					natl::deconstruct(callableRef);
					natl::DefaultAllocator<ConstexprCallableRef>::deallocate(callableRef, 1);
				};
			}
		};

		enum class FunctionRefStorageType {
			functionPtr = 0,
			callableRef,
			constexprCallableRef,
			callable,
			constexprCallable,
		};

		template<class Signature>
		struct FunctionRefBase {};

		template<typename ReturnType, typename... ArgTypes>
		class FunctionRefBase<ReturnType(ArgTypes...)> final {
		public:
			using result_type = ReturnType;
			using arg_types = TypePack<ArgTypes...>;
			using function_ptr_type = ReturnType(*)(ArgTypes...) noexcept;
		private:
			constexpr static Size smallBufferSize = sizeof(void*) * 2;
			using callable_base = CallableBase<ReturnType, ArgTypes...>;
			using callable_ref_base = CallableRefBase<ReturnType, ArgTypes...>;
			using constexpr_callable_base = ConstexprCallableBase<ReturnType, ArgTypes...>;
			using constexpr_callable_ref_base = ConstexprCallableRefBase<ReturnType, ArgTypes...>;
		private:
			FunctionRefStorageType functionRefStorageType;
			union {
				function_ptr_type functionPtr;
				Byte callableRefStorage[smallBufferSize];
				constexpr_callable_base constexprCallable;
				callable_base* callable;
				constexpr_callable_ref_base constexprCallableRef;
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
			template<Size OtherCap, Bool MoveOnly, typename Alloc>
			constexpr FunctionRefBase(FunctionBase<ReturnType(ArgTypes...), OtherCap, MoveOnly, Alloc>& other) noexcept {
				assign<OtherCap, MoveOnly, Alloc>(other);
			}


			//destructor
			constexpr void destruct() noexcept {
				if (isConstantEvaluated()) {
					if (functionRefStorageType == FunctionRefStorageType::constexprCallableRef) {
						constexprCallableRef.getDestroyFunction()(constexprCallableRef);
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

			template<Size OtherCap, Bool MoveOnly, typename Alloc>
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
					copyNoOverlap<const Byte*, Byte*>(
						static_cast<const Byte*>(other.callableRefStorage), 
						static_cast<const Byte*>(other.callableRefStorage) + smallBufferSize,
						callableRefStorage);
					break;
				case FunctionRefStorageType::constexprCallableRef:
					if (isConstantEvaluated()) {
						constexprCallableRef = other.constexprCallableRef;
					} else {
						unreachable();
					}
					break;
				case FunctionRefStorageType::callable:
					callable = other.callable;
					break;
				case FunctionRefStorageType::constexprCallable:
					if (isConstantEvaluated()) {
						constexprCallable = other.constexprCallable;
					} else {
						unreachable();
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
						static_cast<const Byte*>(other.callableRefStorage),
						static_cast<const Byte*>(other.callableRefStorage) + smallBufferSize,
						callableRefStorage);
					break;
				case FunctionRefStorageType::constexprCallableRef:
					if (isConstantEvaluated()) {
						constexprCallableRef = other.constexprCallableRef;
					} else {
						unreachable();
					}
					break;
				case FunctionRefStorageType::callable:
					other.callable = other.callable;
					break;
				case FunctionRefStorageType::constexprCallable:
					if (isConstantEvaluated()) {
						constexprCallable = other.constexprCallable;
					} else {
						unreachable();
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
				if constexpr(IsConvertibleC<Functor, ReturnType(*)(ArgTypes...)>) {
					functionPtr = functor;
					functionRefStorageType = FunctionRefStorageType::functionPtr;
					return self();
				}

				if (isConstantEvaluated()) {
					using constexpr_callable_ref_type = ConstexprCallableRef<Functor, ReturnType, ArgTypes...>;
					using constexpr_callable_ref_type_alloc = DefaultAllocator<constexpr_callable_ref_type>;
					constexpr_callable_ref_type* constexprCallableRefType = constexpr_callable_ref_type_alloc::allocate(1);
					natl::construct<constexpr_callable_ref_type>(constexprCallableRefType, functor);

					functionRefStorageType = FunctionRefStorageType::constexprCallableRef;
					natl::construct<constexpr_callable_ref_base>(&constexprCallableRef, static_cast<constexpr_callable_ref_base*>(constexprCallableRefType));
				} else {
					using FunctorCallableRefType = CallableRef<Functor, ReturnType, ArgTypes...>;
					functionRefStorageType = FunctionRefStorageType::callableRef;
					natl::construct<FunctorCallableRefType, Functor&>(reinterpret_cast<FunctorCallableRefType*>(callableRefStorage), functor);
				}
				return self();
			}

			template<Size OtherCap, Bool MoveOnly, typename Alloc>
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
					if (isConstantEvaluated()) {
						functionRefStorageType = FunctionRefStorageType::constexprCallable;
						constexprCallable = other.constexprCallable;
					} else {
						unreachable();
					}
					break;
				}
				return self();
			}

		public:

			//observers 
			constexpr Bool empty() const noexcept {
				if (functionRefStorageType == FunctionRefStorageType::functionPtr) {
					return !functionPtr;
				} else {
					return false;
				}
			}
			constexpr Bool isEmpty() const noexcept { return empty(); }
			constexpr Bool isNotEmpty() const noexcept { return !empty(); }
			explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

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
					if (isConstantEvaluated()) {
						return constexprCallableRef->getInvokeFunction()(constexprCallableRef);
					} else {
						unreachable();
					}
					break;
				case FunctionRefStorageType::callable:
					callable->invoke(forward<ArgTypes>(args)...);
					break;
				case FunctionRefStorageType::constexprCallable:
					if (isConstantEvaluated()) {
						return constexprCallable->getInvokeFunction()(constexprCallable);
					} else {
						unreachable();
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

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyCompareableV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyRelocatableV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstructibleV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyDestructibleV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstRefConstructibleV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyMoveConstructibleV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstRefAssignableV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyMoveAssignableV<impl::FunctionRefBase<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	class FunctionRef<ReturnType(ArgTypes...)> final {
	public:
		using function_ref_base = impl::FunctionRefBase<ReturnType(ArgTypes...)>;
		using result_type = function_ref_base::result_type;
		using arg_types = function_ref_base::arg_types;
		using function_signature = ReturnType(ArgTypes...) noexcept;
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
		constexpr Bool empty() const noexcept { return functionRefBase.empty(); }
		constexpr Bool isEmpty() const noexcept { return functionRefBase.isEmpty(); }
		constexpr Bool isNotEmpty() const noexcept { return functionRefBase.isNotEmpty(); }
		explicit constexpr operator Bool() const noexcept { return Bool(functionRefBase); }

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
	struct IsTriviallyCompareableV<FunctionRef<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyRelocatableV<FunctionRef<ReturnType(ArgTypes...)>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstructibleV<FunctionRef<ReturnType(ArgTypes...)>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyDestructibleV<FunctionRef<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstRefConstructibleV<FunctionRef<ReturnType(ArgTypes...)>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyMoveConstructibleV<FunctionRef<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstRefAssignableV<FunctionRef<ReturnType(ArgTypes...)>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyMoveAssignableV<FunctionRef<ReturnType(ArgTypes...)>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	class FunctionRef<ReturnType(ArgTypes...) const> final {
	public:
		using function_ref_base = impl::FunctionRefBase<ReturnType(ArgTypes...)>;
		using result_type = function_ref_base::result_type;
		using arg_types = function_ref_base::arg_types;
		using function_signature = ReturnType(ArgTypes...) const noexcept;
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
		constexpr Bool empty() const noexcept { return functionRefBase.empty(); }
		constexpr Bool isEmpty() const noexcept { return functionRefBase.isEmpty(); }
		constexpr Bool isNotEmpty() const noexcept { return functionRefBase.isNotEmpty(); }
		explicit constexpr operator Bool() const noexcept { return Bool(functionRefBase); }

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

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyCompareableV<FunctionRef<ReturnType(ArgTypes...) const>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyRelocatableV<FunctionRef<ReturnType(ArgTypes...) const>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstructibleV<FunctionRef<ReturnType(ArgTypes...) const>>
		: TrueType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyDestructibleV<FunctionRef<ReturnType(ArgTypes...) const>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstRefConstructibleV<FunctionRef<ReturnType(ArgTypes...) const>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyMoveConstructibleV<FunctionRef<ReturnType(ArgTypes...) const>>
		: FalseType {};

	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyConstRefAssignableV<FunctionRef<ReturnType(ArgTypes...) const>>
		: FalseType {};
	template<typename ReturnType, typename... ArgTypes>
	struct IsTriviallyMoveAssignableV<FunctionRef<ReturnType(ArgTypes...) const>>
		: FalseType {};

#ifdef NATL_COMPILER_MSVC
#pragma warning(pop)
#endif // NATL_COMPILER_MSVC

	template<class Type>
	struct CompareLess { 
		constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs < rhs; }
	};
	template<class Type>
	struct CompareGreater {
		constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs > rhs; }
	};
	template<class Type>
	struct CompareLessEqual {
		constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs <= rhs; }
	};
	template<class Type>
	struct CompareGreaterEqual {
		constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs >= rhs; }
	};
	template<class Type>
	struct CompareEqualTo {
		constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs == rhs; }
	};
	template<class Type>
	struct CompareNotEqualTo {
		constexpr Bool operator()(const Type& lhs, const Type& rhs) const noexcept { return lhs != rhs; }
	};

}