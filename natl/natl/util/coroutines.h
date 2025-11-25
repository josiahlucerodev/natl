#pragma once 


//@begin_non_modules
//std
#include <coroutine>

//own
#include "compilerDependent.h"
//@end_non_modules

//own
#include "basicTypes.h"

//@export
namespace natl {
	template<typename Promise = void>
	using CoroutineHandle = std::coroutine_handle<Promise>;

	struct SuspendAlways {
		constexpr Bool await_ready() const noexcept { return false; }
		constexpr void await_suspend(CoroutineHandle<>) const noexcept {}
		constexpr void await_resume() const noexcept {}
	};

	struct SuspendNever {
		constexpr Bool await_ready() const noexcept { return true; }
		constexpr void await_suspend(CoroutineHandle<>) const noexcept {}
		constexpr void await_resume() const noexcept {}
	};
}