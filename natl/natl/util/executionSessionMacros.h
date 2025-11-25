#pragma once

//std
#include <setjmp.h> 

//own
#include "compilerDependent.h"

#define NATL_EXECUTION_SESSION_ERROR(ExecutionSessionVarName) setjmp(natl::getExecutionSessionJumpBuffer(ExecutionSessionVarName))