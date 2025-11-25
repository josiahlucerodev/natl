#pragma once 

//own
#include "../util/utility.h"

//@export
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