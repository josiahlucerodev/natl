/*header
#pragma once
//own
#include "pch.h"
endHeader*/

//module
export module natl.functional;
//std
import std;
//endModule

//interface
export namespace natl {
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

	template<class Iter, class Func = Function<void(typename std::iterator_traits<Iter>::value_type)>>
	void rightFold(const Iter begin, const Iter end, Func func) {
		for (Iter it = begin; it != end; it++) {
			func.invoke(*it);
		}
	}
	template<class Iter, class Func = Function<void(typename std::iterator_traits<Iter>::value_type)>>
	void leftFold(const Iter rbegin, const Iter rend, Func func) {
		for (Iter it = rbegin; it != rend; it++) {
			func.invoke(*it);
		}
	}
	template<class Iter, typename BroadcastType = typename std::iterator_traits<Iter>::value_type>
	void broadcast(const Iter begin, const Iter end, BroadcastType broadcastValue) {
		for (Iter it = begin; it != end; it++) {
			*it = broadcastValue;
		}
	}
	template<class Iter, typename SetType = typename std::iterator_traits<Iter>::value_type>
	void set(const Iter begin, const Iter end, SetType setType) {
		for (Iter it = begin; it != end; it++) {
			*it = setType;
		}
	}
	template<class Iter, class Func = Function<void(typename std::iterator_traits<Iter>::value_type)>>
	void forEach(const Iter begin, const Iter end, Func func) {
		for (Iter it = begin; it != end; it++) {
			func.invoke(*it);
		}
	}
	template<typename Func, class SizeType = std::size_t>
	void repeat(Func func, SizeType count) {
		for (uint32_t i = 0; i < count; i++) {
			func();
		}
	}
	template<class Iter, class Func = Function<bool(typename std::iterator_traits<Iter>::value_type)>>
	bool all(const Iter rbegin, const Iter rend, Func func) {
		for (Iter it = rbegin; it != rend; it++) {
			if (!func.invoke(*it)) {
				false;
			}
		}
		return true;
	}
	template<class Iter, class Func = Function<bool(typename std::iterator_traits<Iter>::value_type)>>
	bool notAll(const Iter rbegin, const Iter rend, Func func) {
		for (Iter it = rbegin; it != rend; it++) {
			if (func.invoke(*it)) {
				false;
			}
		}
		return true;
	}

	//Container
	template<typename Container, 
		typename Iter = Container::iterator,
		class Func = Function<void(typename std::iterator_traits<Iter>::value_type)>>
	void rightFoldContainer(Container& container, Func func) {
		rightFold<Iter, Func>(container.begin(), container.end(), func);
	}
	template<typename Container,
		typename Iter = Container::iterator,
		class Func = Function<void(typename std::iterator_traits<Iter>::value_type)>>
	void leftFoldContainer(Container& container, Func func) {
		leftFold<Iter, Func>(container.rbegin(), container.rend(), func);
	}
	template<typename Container,
		typename Iter = Container::iterator, 
		typename BroadcastType = typename std::iterator_traits<Iter>::value_type>
	void broadcastContainer(Container& container, const BroadcastType& broadcastValue) {
		broadcast<Iter>(container.begin(), container.end(), broadcastValue);
	}
	template<typename Container,
		typename Iter = Container::iterator, 
		typename SetType = typename std::iterator_traits<Iter>::value_type>
	void setContainer(Container& container, SetType setType) {
		set<Iter>(container.begin(), container.end(), setType);
	}
	template<typename Container,
		typename Iter = Container::iterator,
		class Func = Function<void(typename std::iterator_traits<Iter>::value_type)>>
	void forEachContainer(Container& container, Func func) {
		forEach<Iter>(container.begin(), container.end(), func);
	}
	template<typename Container,
		typename Iter = Container::iterator,
		class Func = Function<bool(typename std::iterator_traits<Iter>::value_type)>>
	bool allContainer(Container& container, Func func) {
		return all<Iter, Func>(container.begin(), container.end(), func);
	}
	template<typename Container,
		typename Iter = Container::iterator,
		class Func = Function<bool(typename std::iterator_traits<Iter>::value_type)>>
	bool notAllContainer(Container& container, Func func) {
		return notAll<Iter, Func>(container.begin(), container.end(), func);
	}
}