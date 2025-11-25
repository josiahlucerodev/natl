#pragma once 

//@begin_non_modules
#include <coroutine>
//@end_non_modules

//own
#include "../util/typeTraits.h"
#include "../util/coroutines.h"
#include "../util/error.h"
#include "../util/iterators.h"
#include "../container/container.h"

//@export
namespace natl {
	template<typename DataType>
	struct [[nodiscard]] Generator {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using pointer = value_type*;
		using yielded = reference;

		struct GeneratorPromise;
		using handle_type = CoroutineHandle<GeneratorPromise>;

		struct GeneratorPromise {
			pointer valuePtr;
		public:
			//constructor 
			GeneratorPromise() noexcept = default;

			//destructor 
			~GeneratorPromise() noexcept = default;

			//util 
			GeneratorPromise& self() noexcept { return *this; }
			const GeneratorPromise& self() const noexcept { return *this; }

			//co_functions
			Generator get_return_object() noexcept { return handle_type::from_promise(*this); }
			SuspendAlways initial_suspend() const noexcept { return SuspendAlways{}; }
			SuspendAlways final_suspend() noexcept { return SuspendAlways{}; }
			template<typename U = value_type>
			SuspendAlways yield_value(RemoveReference<value_type>& value) noexcept requires(!IsRValueReference<value_type>) {
				valuePtr = addressof(value);
				return SuspendAlways{};
			}
			SuspendAlways yield_value(RemoveReference<value_type>&& value) noexcept {
				valuePtr = addressof(value);
				return SuspendAlways{};
			}
			template<typename U> 
			SuspendNever await_transform(U&& value) noexcept = delete;
			[[noreturn]] void unhandled_exception() noexcept { natl::terminate(); }
			void return_void() noexcept {}
			reference value() const noexcept { return static_cast<reference>(*valuePtr); }
		};

		struct GeneratorSentinel {};

		struct GeneratorIterator {
		public:
			using iterator_category = InputIteratorTag;
			using difference_type = PtrDiff;
			using value_type = DataType;
			using reference = DataType&;
			using pointer = value_type*;
		private:
			handle_type handle;
		public:
			//constructor
			GeneratorIterator() noexcept : handle(nullptr) {}
			explicit GeneratorIterator(handle_type handleIn) noexcept : handle(handleIn) {}

			//destructor 
			~GeneratorIterator() noexcept = default;

			//util
			GeneratorIterator& self() noexcept { return *this; }
			const GeneratorIterator& self() const noexcept { return *this; }
			friend Bool operator==(const GeneratorIterator& it, GeneratorSentinel) noexcept {
				return !it.handle || it.handle.done();
			}
			friend Bool operator!=(const GeneratorIterator& it, GeneratorSentinel sentinel) noexcept {
				return !(it == sentinel);
			}
			friend Bool operator==(GeneratorSentinel sentinel, const GeneratorIterator& it) noexcept {
				return (it == sentinel);
			}
			friend Bool operator!=(GeneratorSentinel sentinel, const GeneratorIterator& it) noexcept {
				return it != sentinel;
			}

			GeneratorIterator& operator++() noexcept {
				handle.resume();
				return self();
			}

			reference operator*() const noexcept {
				return handle.promise().value();
			}
			pointer operator->() const noexcept {
				return addressof(handle.promise().value());
			}
		};

	private:
		handle_type handle;
	public:
		using promise_type = GeneratorPromise;
		using iterator = GeneratorIterator;
	public:
		//constructor 
		Generator(handle_type handleIn) : handle(handleIn) {}
		Generator(const Generator&) noexcept = delete;
		Generator(Generator&& other) noexcept : handle(other.handle) { other.handle = nullptr; }

		//destructor 
		~Generator() noexcept { 
			if (handle) { 
				handle.destroy(); 
			} 
		};

		//util 
		Generator& self() noexcept { return *this; }
		const Generator& self() const noexcept { return *this; }

		//assignment 
		Generator& operator=(Generator other) noexcept {
			swap(other);
			return self();
		}

		//iterators 
		iterator begin() noexcept {
			if (handle) {
				handle.resume();
			}
			return iterator(handle);
		}
		GeneratorSentinel end() noexcept {
			return GeneratorSentinel();
		}

		//modifiers 
		void swap(Generator& other) noexcept {
			natl::swap(handle, other.handle);
		}
	};
}