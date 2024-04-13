#pragma once 

//std
#include <type_traits>

//own
#include "basicTypes.h"

//interface
namespace natl {
	[[nodiscard]] constexpr natl::Bool isConstantEvaluated() noexcept {
		return std::is_constant_evaluated();
	}
}