#pragma once

//own
#include "../processing/format.h"
#include "../system/printFormatted.h"
#include "utility.h"

//@export
namespace natl {
    template<typename... ArgTypes>
    constexpr void cassertPre(const natl::Bool condition, ArgTypes&&... args) noexcept {
        if (!condition) {
            if (natl::isConstantEvaluated()) { 
                natl::constantEvaluatedError(); 
            } 
            natl::printlnf("natl: ", " Assertion failed: ", natl::forward<ArgTypes>(args)...);
            natl::terminate();
        }
    }
}
