#pragma once 

//own
#include "../util/compilerDependent.h"

//interface 
namespace natl {
	namespace impl {
		void enableSessionNullptrHandlingImpl() noexcept;
	}

	constexpr void enableExecutionSessionNullptrHandling() noexcept {
		if (!isConstantEvaluated()) {
			impl::enableSessionNullptrHandlingImpl();
		}
	}
}