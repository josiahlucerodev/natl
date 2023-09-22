#pragma once

//std
#include <typeTraits.h>

//interface
namespace natl {
	template<typename ReturnType, typename... TypeArgs>
	class Function {};

	template<typename ReturnType, typename... TypeArgs>
	class Function<ReturnType(TypeArgs...)> final {
	public:
		using FunctionPtrType = ReturnType(*)(TypeArgs...);
	private:
		FunctionPtrType functionPtr;
	public:
		constexpr Function() : functionPtr(nullptr) {}
		constexpr Function(FunctionPtrType function) : functionPtr(function) {}

		constexpr void getFunctionPtr(FunctionPtrType function) const noexcept { return functionPtr; }
		constexpr void assign(FunctionPtrType function) noexcept { functionPtr = function; }
		constexpr bool isEmpty() const noexcept { return functionPtr; }
		constexpr bool isNotEmpty() const noexcept { return !bool(functionPtr); }
		constexpr void reset() noexcept { functionPtr = nullptr; }

		constexpr ReturnType invoke(TypeArgs... args) const {
			if (functionPtr == nullptr) {
				return ReturnType();
			}
			return functionPtr(std::forward<TypeArgs>(args)...);
		}
		constexpr void swap(Function<ReturnType(TypeArgs...)>& function) noexcept {
			FunctionPtrType tempFunctionPtr = function.functionPtr;
			function.functionPtr = functionPtr;
			functionPtr = tempFunctionPtr;
		}

		constexpr operator bool() { return isNotEmpty(); }
		constexpr void operator=(FunctionPtrType function) noexcept { assign(function); }
		constexpr bool operator==(FunctionPtrType function) const noexcept { return functionPtr == function; }
		constexpr bool operator!=(FunctionPtrType function) const noexcept { return functionPtr != function; }
		constexpr ReturnType operator()(TypeArgs... args) const { invoke(std::forward<TypeArgs>(args)...); }
	public:
		FunctionPtrType function;
	};

	
	template<typename Func, class... ArgsType>
	constexpr Function<void(ArgsType...)> newVoidFuncition(Func func) noexcept {
		return Function<void(ArgsType...)>([](ArgsType... args) { func(std::forward<ArgsType...>(args)); });
	}
}