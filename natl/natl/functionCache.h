#pragma once 

//own
#include "functional.h"
#include "tuple.h"
#include "option.h"
#include "flatHashMap.h"

//interface 
namespace natl {
	template<typename FunctionSignature, Size FunctionByteCapacity = 32 - alignof(char*), typename Alloc = DefaultAllocatorByte>
	class FunctionCache {};

	template<typename ReturnType, typename... ArgTypes, Size FunctionByteCapacity, typename Alloc>
	class FunctionCache<ReturnType(ArgTypes...), FunctionByteCapacity, Alloc> final {
	public:
		using result_type = ReturnType;
		using arg_types = TypePack<ArgTypes...>;
		using function_signature = ReturnType(ArgTypes...) noexcept;
		using function_type = Function<ReturnType(ArgTypes...), FunctionByteCapacity, Alloc>;
		using arg_storage_type = Tuple<RemoveCV<RemoveReference<ArgTypes>>...>;
	private:
		function_type functionStorage;
		Option<arg_storage_type> functionArgumentStroage;
	public:
		//constructor  
		constexpr FunctionCache() noexcept = default;
		constexpr FunctionCache(const FunctionCache& other) noexcept :
			functionStorage(other.functionStorage),
			functionArgumentStroage(other.functionArgumentStroage) {}
		constexpr FunctionCache(FunctionCache&& other) noexcept : 
			functionStorage(natl::move(other.functionStorage)), 
			functionArgumentStroage(natl::move(other.functionArgumentStroage)) {}

		template<typename Functor>
			requires(
				HasFunctionSignature<Functor, ReturnType, ArgTypes...> && 
				IsCopyConstructible<Functor> && IsCopyAssignable<Functor> &&
				requires(const Functor& functor) { { function_type(functor) }; })
		constexpr FunctionCache(const Functor& functor) noexcept : functionStorage(functor), functionArgumentStroage() {}
		template<typename Functor>
			requires(
				HasFunctionSignature<Functor, ReturnType, ArgTypes...> && 
				IsCopyConstructible<Functor> && IsCopyAssignable<Functor> &&
				requires(const Functor& functor) { { function_type(functor) }; })
		constexpr FunctionCache(Functor&& functor) noexcept : functionStorage(natl::move(functor)), functionArgumentStroage() {}

		//destrutor
		constexpr ~FunctionCache() noexcept = default;

		//util 
		constexpr FunctionCache& self() noexcept { return *this; }
		constexpr const FunctionCache& self() const noexcept { return *this; }

		//assignment 
		constexpr FunctionCache& operator=(const FunctionCache& other) noexcept {
			functionStorage = other.functionStorage;
			functionArgumentStroage = other.functionArgumentStroage;
			return self();
		}
		constexpr FunctionCache& operator=(FunctionCache&& other) noexcept {
			functionStorage = natl::move(other.functionStorage);
			functionArgumentStroage = natl::move(other.functionArgumentStroage);
			return self();
		}
		

		//observers 
		constexpr bool hasFunction() noexcept { return functionStorage.isNotEmpty(); }
		constexpr bool doesNotHaveFunction() noexcept { return functionStorage.isEmpty(); }
		constexpr bool hasArguments() noexcept { return functionArgumentStroage.notHaveValue(); }
		constexpr bool doesNotHaveArguments() noexcept { return functionArgumentStroage.hasValue(); }
		constexpr operator bool() const noexcept { return hasFunction() && hasArguments(); }

		constexpr function_type& getFunction() noexcept { return functionStorage; }
		constexpr const function_type& getFunction() const noexcept { return functionStorage; }

		//modifiers 
		template<typename Functor>
			requires(
				HasFunctionSignature<Functor, ReturnType, ArgTypes...> && 
				IsCopyConstructible<Functor> && IsCopyAssignable<Functor> && 
				requires(const Functor& functor) { { function_type(functor) }; })
		constexpr FunctionCache& bindFunction(const Functor& functor) noexcept {
			functionStorage = functor;
			return self();
		}
		template<typename Functor>
			requires(
				HasFunctionSignature<Functor, ReturnType, ArgTypes...> && 
				IsCopyConstructible<Functor> && IsCopyAssignable<Functor> &&
				requires(const Functor& functor) { { function_type(functor) }; })
		constexpr FunctionCache& bindFunction(Functor&& functor) noexcept {
			functionStorage = natl::move(functor);
			return self();
		}
		constexpr FunctionCache& bindArguments(ArgTypes... args) noexcept {
			functionArgumentStroage = natl::move(arg_storage_type(natl::forward<ArgTypes>(args)...));
			return self();
		}

		//calling
		constexpr ReturnType invoke() noexcept {
			return callFunctionWithTuple(functionStorage, functionArgumentStroage.value());
		}
		constexpr ReturnType operator()() noexcept {
			return invoke();
		}
	};


	template<typename Functor, typename... ArgTypes>
	constexpr InvokeResultWithArgs<Functor, ArgTypes...> callFunctionCached(Functor&& functor, ArgTypes&&... arguments) noexcept {
		using ResultType = InvokeResultWithArgs<Functor, ArgTypes...>;
		using ArgsStorageType = Tuple<ArgTypes...>;
		using ResultCacheType = FlatHashMap<ArgsStorageType, ResultType>;

		if (isConstantEvaluated()) {
			return natl::forward<Functor>(functor)(natl::forward<ArgTypes>(arguments)...);
		} else {

#ifdef __EMSCRIPTEN__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++23-extensions"
#endif // __EMSCRIPTEN__

			ArgsStorageType argumentStorage(arguments...);
			thread_local ResultCacheType resultCache;
			Option<typename ResultCacheType::entry_type*> value = resultCache.find(argumentStorage);
			if (value.hasValue()) {
				return value.value()->data;
			} else {
				return resultCache.insert(argumentStorage, natl::forward<Functor>(functor)(natl::forward<ArgTypes>(arguments)...))->data;
			}

#ifdef __EMSCRIPTEN__
#pragma GCC diagnostic pop
#endif // __EMSCRIPTEN__
		}
	}

}