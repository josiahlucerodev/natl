#pragma once 

//own
#include "basicTypes.h"
#include "strongType.h"
#include "compilerDependent.h"
#include "dataMovement.h"
#include "typeTraits.h"
#include "tuple.h"
#include "pointer.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#ifdef NATL_UNIX_PLATFORM
#include <pthread.h>
#include <unistd.h>
#endif // NATL_UNIX_PLATFORM

//interface
namespace natl {
	namespace impl {
		class ThreadIdTag {};
	}
	using ThreadId = StrongType<ui64, impl::ThreadIdTag>;

	struct ThreadFullCustomizationFlag {};

#ifdef NATL_WINDOWS_PLATFORM
	constexpr ThreadId getCurrentThreadId() noexcept {
		if (isConstantEvaluated()) {
			return ThreadId();
		} else {
			return ThreadId(GetCurrentThreadId());
		}
	}

	Size getNumberOfLogicalProcessors() noexcept {
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		DWORD numLogicalProcessors = sysInfo.dwNumberOfProcessors;
		return static_cast<Size>(numLogicalProcessors);
	}


	class Thread {
	public:
		using native_handle_type = HANDLE;
	private:
		native_handle_type threadHandle;
		ThreadId threadId;
	public:
		//constructor 
		constexpr Thread() noexcept : threadHandle(NULL), threadId() {}
		constexpr Thread(const Thread&) noexcept = delete;
		constexpr Thread(Thread&& other) noexcept {
			threadHandle = other.threadHandle;
			threadId = other.threadId;
			other.threadHandle = NULL;
			other.threadId.setValue(0);
		}
	private:
		template <typename FunctorAndArgStorageTuple, Size... Indices>
		static constexpr DWORD WINAPI threadFunction(LPVOID lpParam) noexcept {
			UniquePtr<FunctorAndArgStorageTuple> functorAndArgStoragePtr = reinterpret_cast<FunctorAndArgStorageTuple*>(lpParam);
			FunctorAndArgStorageTuple& functorAndArgStorageRef = *functorAndArgStoragePtr.get();
			natl::invokeFunction(natl::move(functorAndArgStorageRef.template get<Indices>())...);
			return 0;
		}
		template <class FunctorAndArgStorageTuple, Size... Indices>
		static constexpr auto getThreadFunction(natl::IndexSequence<Indices...>) noexcept {
			return &threadFunction<FunctorAndArgStorageTuple, Indices...>;
		}

		template <typename CustomOpStructFunctor, typename StorageTuple>
		static constexpr DWORD WINAPI threadCustomizedFunction(LPVOID lpParam) noexcept {
			StorageTuple* storageTuple = reinterpret_cast<StorageTuple*>(lpParam);
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

			DWORD dwThreadId;
			threadHandle = CreateThread(NULL, 0, createThreadCallingFunction,  reinterpret_cast<LPVOID>(createThreadCallingFunction), 0, &dwThreadId);
			threadId.setValue(static_cast<ui64>(dwThreadId));

			if (threadHandle == NULL) {
				natlTerminate();
			}
		}
		


		template<typename CustomOpStructFunctor, typename StorageTuple>
		explicit Thread(ThreadFullCustomizationFlag, CustomOpStructFunctor, StorageTuple* storageTuple) noexcept {

			constexpr auto createThreadCallingFunction = getThreadCustomizedFunction<CustomOpStructFunctor, StorageTuple>();

			DWORD dwThreadId;
			threadHandle = CreateThread(NULL, 0, createThreadCallingFunction, reinterpret_cast<LPVOID>(storageTuple), 0, &dwThreadId);
			threadId.setValue(static_cast<ui64>(dwThreadId));

			if (threadHandle == NULL) {
				natlTerminate();
			}
		}
		
		//destructor 
		constexpr ~Thread() noexcept {
			if (!isConstantEvaluated()) {
				if (isActive() == true) {
					natlTerminate();
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
				natlTerminate();
			}
			threadHandle = other.threadHandle;
			threadId = other.threadId;

			other.threadHandle = NULL;
			other.threadId.setValue(0);
			return self();
		}

		//observers
		constexpr Bool joinable() const noexcept { return threadHandle != NULL; }
		constexpr Bool isActive() const noexcept { return joinable(); }
		constexpr ThreadId getId() const noexcept { return threadId; }
		constexpr native_handle_type nativeHandle() noexcept { return threadHandle; };

		//operations
		void join() noexcept {
			if (getId() == getCurrentThreadId()) {
				natlTerminate();
			}
			if (isActive() == false) {
				natlTerminate();
			}

			WaitForSingleObject(threadHandle, INFINITE);
			CloseHandle(threadHandle);

			threadHandle = NULL;
			threadId.setValue(0);
		}
		
		constexpr void swap(Thread& other) noexcept {
			natl::swap<Thread>(self(), other);
		}
	};
#endif // NATL_WINDOWS_PLATFORM

#ifdef NATL_UNIX_PLATFORM
	constexpr ThreadId getCurrentThreadId() noexcept {
		if (isConstantEvaluated()) {
			return ThreadId();
		} else {
			return ThreadId(static_cast<Size>(pthread_self()));
		}
	}

	Size getNumberOfLogicalProcessors() noexcept {
		return static_cast<Size>(sysconf(_SC_NPROCESSORS_ONLN));
	}

	class Thread {
	public:
		using native_handle_type = pthread_t;
	private:
		native_handle_type threadHandle;
	public:
		//constructor 
		constexpr Thread() noexcept : threadHandle(0) {}
		constexpr Thread(const Thread&) noexcept = delete;
		constexpr Thread(Thread&& other) noexcept {
			threadHandle = other.threadHandle;
			other.threadHandle = 0;
		}
	private:
		template <typename FunctorAndArgStorageTuple, Size... Indices>
		static constexpr void* threadFunction(void* arg) noexcept {
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
		static constexpr void* threadCustomizedFunction(void* arg) noexcept {
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

			pthread_t threadId;
			if (pthread_create(&threadId, nullptr, createThreadCallingFunction, reinterpret_cast<void*>(storageTuple)) != 0) {
				natlTerminate();
			}
			threadHandle = static_cast<Size>(threadId);
		}

		template<typename CustomOpStructFunctor, typename StorageTuple>
		explicit Thread(ThreadFullCustomizationFlag, CustomOpStructFunctor, StorageTuple* storageTuple) noexcept {

			constexpr auto createThreadCallingFunction = getThreadCustomizedFunction<CustomOpStructFunctor, StorageTuple>();

			pthread_t threadId;
			if (pthread_create(&threadId, nullptr, createThreadCallingFunction, reinterpret_cast<void*>(storageTuple)) != 0) {
				natlTerminate();
			}
			threadHandle = static_cast<Size>(threadId);
		}

		//destructor 
		constexpr ~Thread() noexcept {
			if (!isConstantEvaluated()) {
				if (isActive() == true) {
					natlTerminate();
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
				natlTerminate();
			}
			threadHandle = other.threadHandle;
			other.threadHandle = 0;
			return self();
		}

		//observers
		constexpr Bool joinable() const noexcept { return threadHandle != 0; }
		constexpr Bool isActive() const noexcept { return joinable(); }
		constexpr ThreadId getId() const noexcept { return ThreadId(static_cast<Size>(threadHandle)); }
		constexpr native_handle_type nativeHandle() noexcept { return threadHandle; };

		//operations
		void join() noexcept {
			if (getId() == getCurrentThreadId()) {
				natlTerminate();
			}
			if (isActive() == false) {
				natlTerminate();
			}
			if (pthread_join(static_cast<pthread_t>(threadHandle), nullptr) != 0) {
				natlTerminate();
			}
			threadHandle = 0;
		}

		constexpr void swap(Thread& other) noexcept {
			natl::swap<Thread>(self(), other);
		}
	};

#endif // NATL_UNIX_PLATFORM

}