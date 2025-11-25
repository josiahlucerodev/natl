#pragma once 

//@begin_non_modules
//natl
#include "../util/compilerDependent.h"
//@end_non_modules

//own
#include "../container/functional.h"
#include "../util/uninitialized.h"
#include "thread.h"
#include "atomic.h"

//@export
namespace natl {
	struct ThreadStopData {
		using destory_function_type = void(*)(void*) noexcept;
		Atomic<Bool> isStopRequested;
		void* storagePtr;
		destory_function_type destroyFunction;


		//constructor 
		ThreadStopData(void* storagePtrIn, destory_function_type destroyFunctionIn) noexcept :
			isStopRequested(false), 
			storagePtr(storagePtrIn),
			destroyFunction(destroyFunctionIn) {}

		//destructor
		~ThreadStopData() noexcept = default;

	public:
		[[nodiscard]] constexpr Bool stopRequested() const noexcept {
			return isStopRequested.load();
		}
		constexpr void requestStop() noexcept {
			isStopRequested.store(true);
		}
		destory_function_type getDestoryFunction() noexcept {
			return destroyFunction;
		}
		void* getStoragePtr() noexcept {
			return storagePtr;
		}
	};

	struct ThreadStopAlert {
	private:
		ThreadStopData* threadStopData;
	public:
		//constructor 
		constexpr ThreadStopAlert() noexcept : threadStopData(nullptr) {}
		constexpr ThreadStopAlert(const ThreadStopAlert& other) noexcept : threadStopData(other.threadStopData) {}
		constexpr ThreadStopAlert(ThreadStopAlert&& other) noexcept : threadStopData(other.threadStopData) {
			other.threadStopData = nullptr;
		}
		constexpr ThreadStopAlert(ThreadStopData* threadStopDataIn) noexcept : threadStopData(threadStopDataIn) {}

		//destructor 
		constexpr ~ThreadStopAlert() noexcept = default;

		//util
		constexpr ThreadStopAlert& self() noexcept { return *this; }
		constexpr const ThreadStopAlert& self() const noexcept { return *this; }
		
		//assignment 
		constexpr ThreadStopAlert& operator=(const ThreadStopAlert& other) noexcept {
			threadStopData = other.threadStopData;
			return self();
		}
		constexpr ThreadStopAlert& operator=(ThreadStopAlert&& other) noexcept {
			threadStopData = other.threadStopData;
			other.threadStopData = nullptr;
			return self();
		}

		//observers
		[[nodiscard]] constexpr Bool stopRequested() const noexcept {
			if (threadStopData == nullptr) {
				return false;
			} else {
				return threadStopData->stopRequested();
			}
		}
		[[nodiscard]] constexpr Bool stopPossible() const noexcept {
			return threadStopData != nullptr;
		}

		//modifiers
		constexpr void swap(ThreadStopAlert& other) noexcept {
			natl::swap<ThreadStopAlert>(self(), other);
		}

		//compare 
		[[nodiscard]] constexpr Bool operator==(const ThreadStopAlert& rhs) const noexcept {
			return threadStopData == rhs.threadStopData;
		}
	};

	struct ThreadStopControl {
	private:
		ThreadStopData* threadStopData;
	public:
		//constructor 
		constexpr ThreadStopControl() noexcept : threadStopData(nullptr) {}
		constexpr ThreadStopControl(const ThreadStopControl& other) noexcept : threadStopData(other.threadStopData) {}
		constexpr ThreadStopControl(ThreadStopControl&& other) noexcept : threadStopData(other.threadStopData) {
			other.threadStopData = nullptr;
		}
		constexpr ThreadStopControl(ThreadStopData* threadStopDataIn) noexcept : threadStopData(threadStopDataIn) {}

		//destructor 
		constexpr ~ThreadStopControl() noexcept = default;

		//util 
		constexpr ThreadStopControl& self() noexcept { return *this; }
		constexpr const ThreadStopControl& self() const noexcept { return *this; }

		//assignment 
		constexpr ThreadStopControl& operator=(const ThreadStopControl& other) noexcept {
			threadStopData = other.threadStopData;
			return self();
		}
		constexpr ThreadStopControl& operator=(ThreadStopControl&& other) noexcept {
			threadStopData = other.threadStopData;
			other.threadStopData = nullptr;
			return self();
		}

		//observers
		[[nodiscard]] constexpr ThreadStopAlert getStopAlert() const noexcept {
			return ThreadStopAlert(threadStopData);
		}
		[[nodiscard]] constexpr Bool stopRequested() const noexcept {
			if (threadStopData == nullptr) {
				return false;
			} else {
				return threadStopData->stopRequested();
			}
		}		
		[[nodiscard]] constexpr Bool stopPossible() const noexcept {
			return threadStopData != nullptr;
		}

		//modifiers
		constexpr Bool requestStop() noexcept {
			if (threadStopData == nullptr) {
				return false;
			} else {
				threadStopData->requestStop();
				return true;
			}
		}
		constexpr void swap(ThreadStopControl& other) noexcept {
			natl::swap<ThreadStopControl>(self(), other);
		}

		//compare 
		[[nodiscard]] constexpr Bool operator==(const ThreadStopControl& rhs) const noexcept {
			return threadStopData == rhs.threadStopData;
		}
	};

	namespace impl {
		template<typename Functor, typename... ArgTypes>
		concept DoesFunctorTakeSmartThreadStopAlert =
			requires(Functor&& functor, ThreadStopAlert&& stopAlert, ArgTypes&&... args) {
				{ functor(natl::forward<ThreadStopAlert>(stopAlert), natl::forward<ArgTypes>(args)...) };
		};
	}

	struct SmartThread {
	public:
		using native_handle_type = Thread::native_handle_type;
	private:
		Thread thread;
		ThreadStopData* threadStopData;
	public:
		//constructor 
		constexpr SmartThread() noexcept : thread() {}
		constexpr SmartThread(const SmartThread&) noexcept = delete;
		constexpr SmartThread(SmartThread&& other) noexcept : thread(natl::move(other.thread)) {}

	private:
		template<typename StorageTuple, Size NumberOfArguments>
		struct StopAlertFunctionOp {
			template<Size... Indices>
			constexpr void callStorageTupleFunction(StorageTuple& storageTuple, IndexSequence<Indices...>) const noexcept {
				invokeFunction(
					storageTuple.template get<0>().valueAsRef(), //functor 
					ThreadStopAlert(storageTuple.getLast().valueAsPtr()), //StopAlert 
					storageTuple.template get<Indices + 1>().valueAsRef()... //Function Args
				);

				deconstruct(storageTuple.template get<0>().valueAsPtr());
				(deconstruct(&storageTuple.template get<Indices + 1>().valueAsRef()), ...);
			}

			constexpr void operator()(StorageTuple* storageTuple) const noexcept {
				callStorageTupleFunction(*storageTuple, MakeIndexSequence<NumberOfArguments>{});
			}
		};

		template<Size Index, typename StorageTuple, typename... ArgTypes>
		constexpr void constructStorageTupleArgs(StorageTuple&) const noexcept {}

		template<Size Index, typename StorageTuple, typename ArgType, typename... RestArgTypes>
		constexpr void constructStorageTupleArgs(StorageTuple& storageTuple, ArgType&& arg, RestArgTypes&&... args) const noexcept {
			construct(storageTuple.template get<Index>().valueAsPtr(), natl::forward<ArgType>(arg));
			constructStorageTupleArgs<Index + 1, StorageTuple, RestArgTypes...>(storageTuple, args...);
		}
	public:

		template<typename Functor, typename... ArgTypes>
		SmartThread(Functor&& functor, ArgTypes&&... args) noexcept {
			if constexpr (impl::DoesFunctorTakeSmartThreadStopAlert<Functor, ArgTypes...>) {
				using FunctorStorage = FunctorBaseStorage<Functor>;
				using StorageTuple = Tuple<
					UninitializedValue<FunctorStorage>, 
					UninitializedValue<ArgTypes>..., 
					UninitializedValue<ThreadStopData>
				>;

				StorageTuple* storageTuple = DefaultAllocator::template rebind<StorageTuple>::allocate(1);
				StorageTuple& storageTupleRef = *storageTuple;
				construct<StorageTuple>(storageTuple);
				construct<FunctorStorage, Functor>(storageTupleRef.template get<0>().valueAsPtr(), natl::forward<Functor>(functor));
				constructStorageTupleArgs<1, StorageTuple, ArgTypes...>(*storageTuple, natl::forward<ArgTypes>(args)...);

				auto destoryFunction = [](void* storagePtr) noexcept -> void {
					StorageTuple* storageTupleToDestroy = reinterpret_cast<StorageTuple*>(storagePtr);
					DefaultAllocator::template rebind<StorageTuple>::deallocate(storageTupleToDestroy, 1);
				};
				threadStopData = storageTupleRef.getLast().valueAsPtr();
				construct<ThreadStopData>(threadStopData, reinterpret_cast<void*>(storageTuple), destoryFunction);

				thread = natl::move(
					Thread(
						ThreadFullCustomizationFlag{},
						StopAlertFunctionOp<StorageTuple, sizeof...(args)>{},
						storageTuple
					)
				);
			} else {
				thread = natl::move(
					Thread(
						natl::forward<Functor>(functor),
						natl::forward<ArgTypes>(args)...
					)
				);
				threadStopData = nullptr;
			}
		}

		SmartThread(Thread&& threadIn, ThreadStopData* threadStopDataIn) noexcept {
			thread = natl::move(threadIn);
			threadStopData = threadStopDataIn;
		}

		//destructor 
		constexpr ~SmartThread() noexcept {
			if (!isConstantEvaluated()) {
				if (thread.isActive() == true) {
					requestStop();
					internalJoinOperation();
				}
			}
		}

		//util 
		constexpr SmartThread& self() noexcept { return *this; }
		constexpr const SmartThread& self() const noexcept { return *this; }

		//assignment 
		SmartThread& operator=(const SmartThread& other) noexcept = delete;
		SmartThread& operator=(SmartThread&& other) noexcept {
			natl::swap<Thread>(thread, other.thread);
			natl::swap<ThreadStopData*>(threadStopData, other.threadStopData);
			return self();
		}

		//observers
		constexpr Bool joinable() const noexcept { return thread.joinable(); }
		constexpr Bool isActive() const noexcept { return thread.isActive(); }
		constexpr ThreadId getId() const noexcept { return thread.getId(); }
		constexpr native_handle_type nativeHandle() noexcept { return thread.nativeHandle(); };

		//operations
	private:
		void internalJoinOperation() noexcept {
			thread.join();
			ThreadStopData::destory_function_type destoryFunction = threadStopData->getDestoryFunction();
			void* storagePtr = threadStopData->getStoragePtr();
			deconstruct<ThreadStopData>(threadStopData);
			destoryFunction(storagePtr);
		}
	public:
		void join() noexcept {
			if (!isConstantEvaluated()) {
				internalJoinOperation();
			}
		}

		//signaling 
		constexpr ThreadStopControl getStopControl() noexcept {
			return ThreadStopControl(threadStopData);
		}
		constexpr ThreadStopAlert getStopAlert() noexcept {
			return ThreadStopAlert(threadStopData);
		}

		//observers
		[[nodiscard]] constexpr Bool stopRequested() const noexcept {
			if (threadStopData == nullptr) {
				return false;
			} else {
				return threadStopData->stopRequested();
			}
		}
		[[nodiscard]] constexpr Bool stopPossible() const noexcept {
			return threadStopData != nullptr;
		}

		//modifiers 
		constexpr Bool requestStop() noexcept {
			if (threadStopData == nullptr) {
				return false;
			} else {
				threadStopData->requestStop();
				return true;
			}
		}
		constexpr void swap(SmartThread& other) noexcept {
			natl::swap<SmartThread>(self(), other);
		}
	};
}