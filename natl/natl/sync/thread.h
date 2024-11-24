#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/compilerDependent.h"
#include "../util/dataMovement.h"
#include "../util/typeTraits.h"
#include "../util/pointer.h"
#include "../fundamental/strongType.h"
#include "../fundamental/tuple.h"
#include "../units/standard.h"

//interface
namespace natl {
	namespace impl {
		class ThreadIdTag {};
		class ThreadHandleTag {};
	}
	using ThreadId = StrongType<ui64, impl::ThreadIdTag>;

	struct ThreadFullCustomizationFlag {};

#ifdef NATL_WINDOWS_PLATFORM
	using NativeThreadHandle = void*;
	using NativeThreadFunctionReturnResultType = unsigned long;
	using NativeThreadFunctionParamaterType = void*;
#define NATL_NATIVE_THREAD_FUNCTION_ATTRIBUTE __stdcall
#define NATL_NATIVE_INVALID_THREAD_HANDEL_VALUE 0
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	using NativeThreadHandle = unsigned long;
	using NativeThreadFunctionReturnResultType = void*;
	using NativeThreadFunctionParamaterType = void*;
#define NATL_NATIVE_THREAD_FUNCTION_ATTRIBUTE
#define NATL_NATIVE_INVALID_THREAD_HANDEL_VALUE NativeThreadHandle(0)
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

	namespace impl {
		ThreadId getCurrentThreadIdNativeImpl() noexcept;
	}

	constexpr ThreadId getCurrentThreadId() noexcept {
		if (isConstantEvaluated()) {
			return ThreadId();
		} else {
			return impl::getCurrentThreadIdNativeImpl();
		}
	}

	Size getNumberOfLogicalProcessors() noexcept;

	using NativeThreadFunctionType = NativeThreadFunctionReturnResultType(*)(NativeThreadFunctionParamaterType);

	struct ThreadCreateResult {
		Bool successful;
		ThreadId threadId;
		NativeThreadHandle threadHandel;
	};

	ThreadCreateResult createThread(NativeThreadFunctionType threadFunction, void* paramater) noexcept;
	Bool joinThread(NativeThreadHandle thread) noexcept;
	void thisThreadSleep(const Milliseconds<i64> milliseconds) noexcept;
	void thisThreadYield() noexcept;



	class Thread {
	public:
		using native_handle_type = NativeThreadHandle;
	private:
		native_handle_type threadHandle;
		ThreadId threadId;
	public:
		//constructor 
		constexpr Thread() noexcept : threadHandle(NATL_NATIVE_INVALID_THREAD_HANDEL_VALUE), threadId() {}
		constexpr Thread(const Thread&) noexcept = delete;
		constexpr Thread(Thread&& other) noexcept {
			threadHandle = other.threadHandle;
			threadId = other.threadId;
			other.threadHandle = NATL_NATIVE_INVALID_THREAD_HANDEL_VALUE;
			other.threadId.setValue(0);
		}
	private:
		template <typename FunctorAndArgStorageTuple, Size... Indices>
		static constexpr NativeThreadFunctionReturnResultType NATL_NATIVE_THREAD_FUNCTION_ATTRIBUTE threadFunction(NativeThreadFunctionParamaterType arg) noexcept {
			UniquePtr<FunctorAndArgStorageTuple> functorAndArgStoragePtr = reinterpret_cast<FunctorAndArgStorageTuple*>(arg);
			FunctorAndArgStorageTuple& functorAndArgStorageRef = *functorAndArgStoragePtr.get();
			natl::invokeFunction(natl::move(functorAndArgStorageRef.template get<Indices>())...);
			return 0;
		}
		template <class FunctorAndArgStorageTuple, Size... Indices>
		static constexpr auto getThreadFunction(natl::IndexSequence<Indices...>) noexcept {
			return &threadFunction<FunctorAndArgStorageTuple, Indices...>;
		}

		template <typename CustomOpStructFunctor, typename StorageTuple>
		static constexpr NativeThreadFunctionReturnResultType NATL_NATIVE_THREAD_FUNCTION_ATTRIBUTE threadCustomizedFunction(NativeThreadFunctionParamaterType arg) noexcept {
			StorageTuple* storageTuple = reinterpret_cast<StorageTuple*>(arg);
			CustomOpStructFunctor{}(storageTuple);
			return 0;
		}
		template <typename CustomOpStructFunctor, typename StorageTuple>
		static constexpr auto getThreadCustomizedFunction() noexcept {
			return &threadCustomizedFunction<CustomOpStructFunctor, StorageTuple>;
		}
	public:

		template<typename Functor, typename... ArgTypes>
		explicit Thread(Functor&& functor, ArgTypes&&... args) noexcept {

			using StorageTuple = Tuple<Functor, ArgTypes...>;
			using StorageTupleAlloc = DefaultAllocator<StorageTuple>;
			StorageTuple* storageTuple = StorageTupleAlloc::allocate(1);

			construct(storageTuple, natl::forward<Functor>(functor), natl::forward<ArgTypes>(args)...);
			constexpr auto createThreadCallingFunction = getThreadFunction<StorageTuple>(natl::MakeIndexSequence<1 + sizeof...(ArgTypes)>{});

			ThreadCreateResult threadCreateResult = createThread(createThreadCallingFunction, reinterpret_cast<void*>(storageTuple));
			if (threadCreateResult.successful == false) {
				natl::terminate();
			}
			threadHandle = threadCreateResult.threadHandel;
			threadId = threadCreateResult.threadId;
		}

		template<typename CustomOpStructFunctor, typename StorageTuple>
		explicit Thread(ThreadFullCustomizationFlag, CustomOpStructFunctor, StorageTuple* storageTuple) noexcept {

			constexpr auto createThreadCallingFunction = getThreadCustomizedFunction<CustomOpStructFunctor, StorageTuple>();

			ThreadCreateResult threadCreateResult = createThread(createThreadCallingFunction, reinterpret_cast<void*>(storageTuple));
			if (threadCreateResult.successful == false) {
				natl::terminate();
			}
			threadHandle = threadCreateResult.threadHandel;
			threadId = threadCreateResult.threadId;
		}

		//destructor 
		constexpr ~Thread() noexcept {
			if (!isConstantEvaluated()) {
				if (isActive() == true) {
					natl::terminate();
				}
			}
		}

		//util 
		constexpr Thread& self() noexcept { return *this; }
		constexpr const Thread& self() const noexcept { return *this; }

		//assignment 
		Thread& operator=(const Thread& other) noexcept = delete;
		Thread& operator=(Thread&& other) noexcept {
			if (isActive() == true) {
				natl::terminate();
			}
			threadHandle = other.threadHandle;
			threadId = other.threadId;

			other.threadHandle = NATL_NATIVE_INVALID_THREAD_HANDEL_VALUE;
			other.threadId.setValue(0);
			return self();
		}

		//observers
		constexpr Bool joinable() const noexcept { return threadHandle != 0; }
		constexpr Bool isActive() const noexcept { return joinable(); }
		constexpr ThreadId getId() const noexcept { return threadId; }
		constexpr native_handle_type nativeHandle() noexcept { return threadHandle; };

		//operations
		void join() noexcept {
			if (getId() == getCurrentThreadId()) {
				natl::terminate();
			}
			if (isActive() == false) {
				natl::terminate();
			}
			if (joinThread(threadHandle) == false) {
				natl::terminate();
			}
			threadHandle = NATL_NATIVE_INVALID_THREAD_HANDEL_VALUE;
			threadId.setValue(0);
		}

		constexpr void swap(Thread& other) noexcept {
			natl::swap<Thread>(self(), other);
		}
	};
}