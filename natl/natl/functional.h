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
		Function(FunctionPtrType function) : functionPtr(function) {}

		void getFunctionPtr(FunctionPtrType function) const noexcept { return functionPtr; }
		void assign(FunctionPtrType function) noexcept { functionPtr = function; }
		bool isEmpty() const noexcept { return functionPtr; }
		bool isNotEmpty() const noexcept { return !bool(functionPtr); }
		void reset() noexcept { functionPtr = nullptr; }

		ReturnType invoke(TypeArgs... args) const {
			if (functionPtr == nullptr) {
				return ReturnType();
			}
			return functionPtr(std::forward<TypeArgs>(args)...);
		}
		void swap(Function<ReturnType(TypeArgs...)>& function) {
			FunctionPtrType tempFunctionPtr = function.functionPtr;
			function.functionPtr = functionPtr;
			functionPtr = tempFunctionPtr;
		}

		operator bool() { return isNotEmpty(); }
		void operator=(FunctionPtrType function) noexcept { assign(function); }
		bool operator==(FunctionPtrType function) const noexcept { return functionPtr == function; }
		bool operator!=(FunctionPtrType function) const noexcept { return functionPtr != function; }
		ReturnType operator()(TypeArgs... args) const { invoke(std::forward<TypeArgs>(args)...); }
	public:
		FunctionPtrType function;
	};

	
	template<typename Func, class... ArgsType>
	Function<void(ArgsType...)> newVoidFuncition(Func func) {
		return Function<void(ArgsType...)>([](ArgsType... args) { func(std::forward<ArgsType...>(args)); });
	}
}