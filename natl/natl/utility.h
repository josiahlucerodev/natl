#pragma once 

//std
#include <type_traits>

//interface
namespace natl {
	[[nodiscard]] constexpr bool isConstantEvaluated() noexcept {
		return std::is_constant_evaluated();
	}
}