//@interface
#include "executionSession.h"

//@begin_non_modules
//std
#include <csetjmp>
//@end_non_modules

//own
#include "allocator.h"
#include "iteration.h"
#include "../container/persistentHashMap.h"

namespace natl {
	//execution session 
	struct ExecutionSession {
	public:
		using global_tracker_map = PersistentHashMap<ExecutionSessionString, GlobalTracker>;
	public:
		ExecutionSessionString name;
		global_tracker_map globalTrackerMap;
		UninitializedValue<jmp_buf> jumpBuffer;
		Option<ExecutionSessionErrorInfo> errorInfo;
	public:
		ExecutionSession(const ExecutionSessionString& nameIn) : name(nameIn), globalTrackerMap(), jumpBuffer() {}
		ExecutionSession(ExecutionSession&&) noexcept = delete;
		ExecutionSession(const ExecutionSession&) noexcept = delete;
		ExecutionSession& operator=(const ExecutionSession&) noexcept = delete;
		ExecutionSession& operator=(ExecutionSession&&) noexcept = delete;
	};

	struct ExecutionSessionStorage {
		constexpr static inline Size smallBufferSize = 16;

		struct Node {
			Array<UninitializedValue<ExecutionSession>, smallBufferSize> executionSessions;
			Node* next;
		};

		Size count;
		Node root;
		Node* head;

		ExecutionSessionStorage() : count(0), root(), head() {}
	};

	thread_local ExecutionSessionStorage executionSessionStorage;


	DynArray<GlobalTracker*, ExecutionSessionAlloc> getOrderedGlobalTrakers(ExecutionSession& executionSession) noexcept {
		DynArray<GlobalTracker*, ExecutionSessionAlloc> output;
		output.resize(executionSession.globalTrackerMap.size());
		for (auto [globalTrackerName, globalTracker] : executionSession.globalTrackerMap) {
			output[globalTracker.index] = &globalTracker;
		}
		return output;
	}

	ObserverPtr<ExecutionSession> pushExecutionSession(const ConstAsciiStringView& name) noexcept {
		ExecutionSession* currentExecutionSession = nullptr;
		if (executionSessionStorage.count == 0) {
			executionSessionStorage.head = &executionSessionStorage.root;
		} 

		if (executionSessionStorage.count != 0) {
			currentExecutionSession = executionSessionStorage.head->executionSessions[executionSessionStorage.count % ExecutionSessionStorage::smallBufferSize].valueAsPtr();
		}

		if (executionSessionStorage.count != 0 && executionSessionStorage.count % ExecutionSessionStorage::smallBufferSize == 0) {
			ExecutionSessionStorage::Node* newNode = ExecutionSessionAlloc::template rebind<ExecutionSessionStorage::Node>::allocate(1);
			natl::construct<ExecutionSessionStorage::Node>(newNode);
			executionSessionStorage.head->next = newNode;
			executionSessionStorage.head = newNode;
		}

		const Size nodeIndex = executionSessionStorage.count;
		executionSessionStorage.count++;
		UninitializedValue<ExecutionSession>& uExecutionSession = executionSessionStorage.head->executionSessions[nodeIndex];
		uExecutionSession.construct(name);
		ExecutionSession& executionSession = uExecutionSession.valueAsRef();

		if (currentExecutionSession) {
			executionSession.globalTrackerMap.reserve(executionSession.globalTrackerMap.size());
			Size index = 0;
			DynArray<GlobalTracker*, ExecutionSessionAlloc> orderedGlobalTrakers = getOrderedGlobalTrakers(executionSession);
			for (GlobalTracker* globalTracker : orderedGlobalTrakers) {
				if (globalTracker->localContagious) {
					auto newGlobalTracker = move(globalTracker->newFunc(makeObserver(globalTracker)));
					newGlobalTracker.index = index;
					newGlobalTracker.name = globalTracker->name;
					executionSession.globalTrackerMap.insert(globalTracker->name, move(newGlobalTracker));
					index++;
					
					if (globalTracker->top) {
						globalTracker->top->setGlobalTracker(globalTracker);
					}
				}
			}
		}

		return ObserverPtr<ExecutionSession>(&executionSession);
	}

	Bool hasExecutionSessions() noexcept {
		return executionSessionStorage.count != 0;
	}

	PopExecutionSessionError popExecutionSession(const ConstAsciiStringView& name) noexcept {
		if (!hasExecutionSessions()) {
			return PopExecutionSessionError::noExecutionSession;
		}

		UninitializedValue<ExecutionSession>& uExecutionSession = executionSessionStorage.head->executionSessions[(executionSessionStorage.count - 1) % ExecutionSessionStorage::smallBufferSize];
		ExecutionSession& executionSession = uExecutionSession.valueAsRef();
		if (executionSession.name != name) {
			return PopExecutionSessionError::wrongName;
		}

		DynArray<GlobalTracker*, ExecutionSessionAlloc> orderedGlobalTrakers = getOrderedGlobalTrakers(executionSession);
		for (GlobalTracker* globalTracker : orderedGlobalTrakers) {
			if (globalTracker) {
				globalTracker->destroyFunc(*globalTracker);
			}
		}

		uExecutionSession.deconstruct();
		executionSessionStorage.count--;

		if (executionSessionStorage.count > ExecutionSessionStorage::smallBufferSize
			&& executionSessionStorage.count % ExecutionSessionStorage::smallBufferSize) {
			ExecutionSessionAlloc::template rebind<ExecutionSessionStorage::Node>::deallocate(executionSessionStorage.head, 1);

			ExecutionSessionStorage::Node* node = &executionSessionStorage.root;
			while (node) {
				executionSessionStorage.head = node;
				if (node->next) {
					node = node->next;
				} else {
					node = nullptr;
				}
			}
		}
		
		Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> topExecutionSessionExpect = getTopExecutionSession();
		if (topExecutionSessionExpect.hasValue()) {
			for (auto [globalTrackerName, globalTracker] : topExecutionSessionExpect.value()->globalTrackerMap) {
				if (globalTracker.top) {
					globalTracker.top.getPtr()->setGlobalTracker(&globalTracker);
					if (globalTracker.newTopFunc) {
						globalTracker.newTopFunc(globalTracker);
					}
				}
			}
		}

		return PopExecutionSessionError::none;
	}

	Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> getTopExecutionSession() noexcept {
		if (!hasExecutionSessions()) {
			return unexpected(NoExecutionSessionError{});
		}
		const Size index = (executionSessionStorage.count - 1) % ExecutionSessionStorage::smallBufferSize;
		ExecutionSession* executionSession = executionSessionStorage.head->executionSessions[index].valueAsPtr();
		return ObserverPtr<ExecutionSession>(executionSession);
	}

	JumpBuffer& getExecutionSessionJumpBuffer(ObserverPtr<ExecutionSession> executionSession) noexcept {
		return executionSession->jumpBuffer.valueAsRef();
	}

	thread_local Bool executionSessionErrorHandlingFlag = false;
	RaiseExecutionSessionErrorFlag raiseExecutionSessionError(const ExecutionSessionErrorCode& errorCode,
		const ExecutionSessionString& errorMessage, const ExecutionSessionErrorValue& errorValue) noexcept {
		if (!hasExecutionSessions()) {
			return RaiseExecutionSessionErrorFlag::noExecutionSession;
		}

		Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> executionSessionExpect = getTopExecutionSession();
		if (!executionSessionExpect.hasValue()) {
			return RaiseExecutionSessionErrorFlag::noExecutionSession;
		}
		ObserverPtr<ExecutionSession> executionSession = executionSessionExpect.value();
		executionSession->errorInfo = ExecutionSessionErrorInfo(errorCode, errorMessage, errorValue);
		executionSessionErrorHandlingFlag = true;
		longjmp(executionSession->jumpBuffer.value(), errorCode);
	}

	Expect<ObserverPtr<ExecutionSessionErrorInfo>, GetExecutionSessionErrorFlag> getExecutionSessionError() noexcept {
		if (!hasExecutionSessions()) {
			return unexpected(GetExecutionSessionErrorFlag::noExecutionSession);
		}

		Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> executionSessionExpect = getTopExecutionSession();
		if (!executionSessionExpect.hasValue()) {
			return unexpected(GetExecutionSessionErrorFlag::noExecutionSession);
		}

		ObserverPtr<ExecutionSession> executionSession = executionSessionExpect.value();
		if (executionSession->errorInfo.doesNotHaveValue()) {
			return unexpected(GetExecutionSessionErrorFlag::noErrorValue);
		}

		return ObserverPtr<ExecutionSessionErrorInfo>(executionSession->errorInfo.valueAsPtr());
	}
	void executionSessionErrorHandled() noexcept {
		executionSessionErrorHandlingFlag = false;
	}
	Bool isExecutionSessionErrorHandling() noexcept {
		return executionSessionErrorHandlingFlag;
	}

	//global traker
	Expect<ObserverPtr<GlobalTracker>, RegisterGlobalTrakerError> registerGlobalTraker(const ConstAsciiStringView& name, GlobalTracker&& globalTraker) noexcept {
		if (globalTraker.newFunc.isEmpty()) {
			return unexpected(RegisterGlobalTrakerError::noNewFunc);
		}
		if (globalTraker.destroyFunc.isEmpty()) {
			return unexpected(RegisterGlobalTrakerError::noDestoryFunc);
		}

		if (!hasExecutionSessions()) {
			return unexpected(RegisterGlobalTrakerError::noExecutionSession);
		}

		Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> executionSessionExpect = getTopExecutionSession();
		if (!executionSessionExpect.hasValue()) {
			return unexpected(RegisterGlobalTrakerError::noExecutionSession);
		}

		ObserverPtr<ExecutionSession> executionSession = executionSessionExpect.value();
		globalTraker.index = executionSession->globalTrackerMap.size();
		globalTraker.name = name;

		MapInsertResult insertResult = executionSession->globalTrackerMap.insert(name, move(globalTraker));
		if (insertResult.insertFailed()) {
			return unexpected(RegisterGlobalTrakerError::nameConflict);
		}

		GlobalTracker* globalTrackerLocation = &insertResult.iter().deref().value();
		if (globalTrackerLocation->top.hasValue()) {
			globalTrackerLocation->top.get()->setGlobalTracker(globalTrackerLocation);
		}

		return ObserverPtr<GlobalTracker>(globalTrackerLocation);
	}

	Bool hasGlobalTraker(const ConstAsciiStringView& name) noexcept {
		if (!hasExecutionSessions()) {
			return false;
		}

		Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> executionSessionExpect = getTopExecutionSession();
		if (!executionSessionExpect.hasValue()) {
			return false;
		}

		return executionSessionExpect.value()->globalTrackerMap.contains(name);
	}

	Bool allExecutionSessionsDontHaveGlobalTraker(const ConstAsciiStringView& name) noexcept {
		if (executionSessionStorage.count == 0) {
			return true;
		}

		ExecutionSessionStorage::Node* node = &executionSessionStorage.root;
		for (const Size index : Repeat(executionSessionStorage.count)) {
			if (node->executionSessions[index % ExecutionSessionStorage::smallBufferSize].value().globalTrackerMap.contains(name)) {
				return false;
			}
			if (index != 0 && index % ExecutionSessionStorage::smallBufferSize == 0) {
				node = node->next;
			}
		}

		return true;
	}

	RegisterGlobalTrakerError registerGlobalTrakerToAll(const ConstAsciiStringView& name, GlobalTracker&& globalTraker) noexcept {
		if (globalTraker.newFunc.isEmpty()) {
			return RegisterGlobalTrakerError::noNewFunc;
		}
		if (globalTraker.destroyFunc.isEmpty()) {
			return RegisterGlobalTrakerError::noDestoryFunc;
		}

		if (!hasExecutionSessions()) {
			return RegisterGlobalTrakerError::noExecutionSession;
		}


		if (!allExecutionSessionsDontHaveGlobalTraker(name)) {
			return RegisterGlobalTrakerError::nameConflict;
		}

		ExecutionSessionStorage::Node* node = &executionSessionStorage.root;
		for (const Size index : Repeat(executionSessionStorage.count)) {
			ExecutionSession& ExecutionSession = node->executionSessions[index % ExecutionSessionStorage::smallBufferSize].valueAsRef();

			auto newGlobalTracker = move(globalTraker.newFunc(makeObserver(&globalTraker)));
			newGlobalTracker.index = ExecutionSession.globalTrackerMap.size();
			newGlobalTracker.name = name;
			ExecutionSession.globalTrackerMap.insert(name, move(newGlobalTracker));

			if (index != 0 && index % ExecutionSessionStorage::smallBufferSize == 0) {
				node = node->next;
			}
		}

		return RegisterGlobalTrakerError::none;
	}

	Expect<ObserverPtr<GlobalTracker>, GetGlobalTrackerError> getGlobalTraker(const ConstAsciiStringView& name) noexcept {
		if (!hasExecutionSessions()) {
			return unexpected(GetGlobalTrackerError::noExecutionSession);
		}

		Expect<ObserverPtr<ExecutionSession>, NoExecutionSessionError> executionSessionExpect = getTopExecutionSession();
		if (!executionSessionExpect.hasValue()) {
			return unexpected(GetGlobalTrackerError::noExecutionSession);
		}

		auto nameAndGlobalTracker = executionSessionExpect.value()->globalTrackerMap.find(name);
		if (!nameAndGlobalTracker.hasValue()) {
			return unexpected(GetGlobalTrackerError::notFound);
		}
		GlobalTracker* globalTracker = nameAndGlobalTracker.value().valueAsPtr();
		return ObserverPtr<GlobalTracker>(globalTracker);
	}

	Bool isGlobalTrackerName(GlobalTracker* globalTraker, const ConstAsciiStringView& name) noexcept {
		if (globalTraker == nullptr) {
			return false;
		}

		return name == globalTraker->name;
	}
}
