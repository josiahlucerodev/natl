#pragma once

//own
#include "../processing/format.h"
#include "../system/printFormatted.h"
#include "utility.h"

//interface 
#define NATL_ASSERT(condition, ...) \
    if (!(condition)) { \
		if (natl::isConstantEvaluated()) { natl::constantEvaluatedError(); } \
        natl::printlnf("natl: ", " Assertion failed: ", __VA_ARGS__); \
        natl::terminate(); \
    }
