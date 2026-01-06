#pragma once 

//@begin_non_modules
//c
#include <setjmp.h>
//own
#include "compilerDependent.h"
//@end_non_modules

//own
#include "typeInfo.h"
#include "uninitialized.h"
#include "hash.h"
#include "../container/array.h"
#include "../container/functional.h"
#include "../container/any.h"
#include "../container/string.h"
#include "../util/observerPtr.h"
#include "../util/option.h"
#include "../util/expect.h"

//disable set_jmp warnings 
#ifdef NATL_COMPILER_MSVC
#pragma warning(disable : 4611)
#endif // NATL_COMPILER_MSVC

//@export
namespace natl {
	using JumpBuffer = jmp_buf;
	using ExecutionSessionAlloc = UntrackedStandardAllocator<>;
	using ExecutionSessionString = StringByteSize<32, ExecutionSessionAlloc>;
	using ExecutionSessionErrorValue = AnyByteSize<64>;
	using ExecutionSessionErrorCode = i32;

	struct ExecutionSessionErrorInfo {
		ExecutionSessionErrorCode errorCode;
		ExecutionSessionString errorMessage;
		ExecutionSessionErrorValue errorValue;
	};

	//execution session
	struct ExecutionSession;

	struct NoExecutionSessionError {};

	enum struct PopExecutionSessionError {
		none = 0,
		noExecutionSession,
		wrongName,
	};
	
	enum struct RaiseExecutionSessionErrorFlag {
		none = 0,
		noExecutionSession,
	};

	enum struct GetExecutionSessionErrorFlag {
		none = 0,
		noExecutionSession,
		noErrorValue,
	};

	ObserverPtr<ExecutionSession> pushExecutionSession(const ConstAsciiStringView& name) noexcept;
	PopExecutionSessionError popExecutionSession(const ConstAsciiStringView& name) noexcept;
	Bool hasExecutionSessions() noexcept;
	Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> getTopExecutionSession() noexcept;
	JumpBuffer& getExecutionSessionJumpBuffer(ObserverPtr<ExecutionSession> ExecutionSession) noexcept;

	template<typename Type, typename... ConstructArgs>
	constexpr ExecutionSessionErrorValue makeExecutionSessionError(ConstructArgs&&... constructArgs) noexcept {
		return makeAnyByteSizeAlloc<64, ExecutionSessionAlloc, Type>(forward<ConstructArgs>(constructArgs)...);
	}
	constexpr ExecutionSessionErrorValue makeEmptyExecutionSessionError() noexcept {
		return makeAnyByteSizeAlloc<64, ExecutionSessionAlloc, Empty>();
	}
	RaiseExecutionSessionErrorFlag raiseExecutionSessionError(
		const ExecutionSessionErrorCode& errorCode,
		const ExecutionSessionString& errorMessage, 
		const ExecutionSessionErrorValue& errorValue = makeEmptyExecutionSessionError()) noexcept;
	Expect<ObserverPtr<ExecutionSessionErrorInfo>, GetExecutionSessionErrorFlag> getExecutionSessionError() noexcept;
	void executionSessionErrorHandled() noexcept;
	Bool isExecutionSessionErrorHandling() noexcept;

	struct GlobalTracker;

	struct TopGlobalTracker {
	public:
		OptionPtr<GlobalTracker> globalTracker;

	public:
		constexpr OptionPtr<GlobalTracker> getGlobalTracker() noexcept {
			return globalTracker;
		}
		constexpr void setGlobalTracker(GlobalTracker* globalTrackerIn) noexcept {
			globalTracker = globalTrackerIn;
		}
	};

	//global traker
	struct GlobalTracker {
	public:
		using storage_type = ExecutionSessionErrorValue;
		using new_func_type = FunctionByteSize<GlobalTracker(ObserverPtr<GlobalTracker>), 32, ExecutionSessionAlloc>;
		using destroy_func_type = FunctionByteSize<void(GlobalTracker&), 32, ExecutionSessionAlloc>;
		using new_top_func_type = FunctionByteSize<void(GlobalTracker&), 32, ExecutionSessionAlloc>;

	public:
		Bool localContagious;
		Size index;
		ExecutionSessionString name;
		storage_type storage;
		new_func_type newFunc;
		destroy_func_type destroyFunc;
		new_top_func_type newTopFunc;
		OptionPtr<TopGlobalTracker> top;

	public:
		GlobalTracker(const Bool localContagiousIn,
			const storage_type& storageIn,
			new_func_type newFuncIn,
			destroy_func_type destoryFuncIn,
			new_top_func_type newTopFuncIn,
			OptionPtr<TopGlobalTracker> topIn = nullptr) :
			localContagious(localContagiousIn),
			index(0), name(),
			storage(storageIn),
			newFunc(newFuncIn),
			destroyFunc(destoryFuncIn),
			newTopFunc(newTopFuncIn),
			top(topIn) {}

		GlobalTracker(GlobalTracker&&) noexcept = default;
		GlobalTracker(const GlobalTracker&) noexcept = default;
		GlobalTracker& operator=(const GlobalTracker&) noexcept = default;
		GlobalTracker& operator=(GlobalTracker&&) noexcept = default;

		template<typename Type>
		constexpr OptionPtr<Type> getStoredAs() noexcept {
			return anyCast<Type>(storage);
		}
	};

	template<typename Type, typename... ConstructArgs>
	constexpr GlobalTracker::storage_type makeGlobalTrackerStorage(ConstructArgs&&... constructArgs) noexcept {
		return makeAnyByteSizeAlloc<64, ExecutionSessionAlloc, Type>(forward<ConstructArgs>(constructArgs)...);
	}


	enum struct RegisterGlobalTrakerError {
		none = 0,
		noNewFunc,
		noDestoryFunc,
		noExecutionSession,
		nameConflict,
	};

	enum struct HasGlobalTrakerError {
		none = 0,
		noExecutionSession,
		nameConflict,
	};
	enum struct GetGlobalTrackerError {
		none = 0,
		noExecutionSession,
		notFound,
	};

	//global traker
	Bool hasGlobalTraker(const ConstAsciiStringView& name) noexcept;
	Expect<ObserverPtr<GlobalTracker>, RegisterGlobalTrakerError> registerGlobalTraker(const ConstAsciiStringView& name, GlobalTracker&& globalTraker) noexcept;
	Bool allExecutionSessionsDontHaveGlobalTraker(const ConstAsciiStringView& name) noexcept;
	RegisterGlobalTrakerError registerGlobalTrakerToAll(const ConstAsciiStringView& name, GlobalTracker&& globalTraker) noexcept;
	Expect<ObserverPtr<GlobalTracker>, GetGlobalTrackerError> getGlobalTraker(const ConstAsciiStringView& name) noexcept;
	Bool isGlobalTrackerName(GlobalTracker* globalTraker, const ConstAsciiStringView& name) noexcept;

	Expect<ObserverPtr<GlobalTracker>, RegisterGlobalTrakerError> registerAllocationGlobalTraker(
		const ConstAsciiStringView& name, OptionPtr<TopGlobalTracker> top = nullptr) noexcept;
}