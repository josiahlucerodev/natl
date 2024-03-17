#pragma once 

//own
#include "typeTraits.h"
#include "container.h"
#include "coroutines.h"
#include "error.h"

//interface
namespace natl {
	template<typename DataType>
	class [[nodiscard]] Generator {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using pointer = value_type*;
		using yielded = reference;

		class GeneratorPromise;
		using handle_type = CoroutineHandle<GeneratorPromise>;

		class GeneratorPromise {
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
			[[noreturn]] void unhandled_exception() noexcept { natlTerminate(); }
			void return_void() noexcept {}
			reference value() const noexcept { return static_cast<reference>(*valuePtr); }
		};

		struct GeneratorSentinel {};

		class GeneratorIterator {
		public:
			using iterator_category = std::input_iterator_tag;
			using difference_type = PtrDiff;
			using value_type = value_type;
			using reference = reference;
			using pointer = pointer;
		private:
			handle_type handle;
		public:
			//constructor
			GeneratorIterator() noexcept : handle(nullptr) {}
			explicit GeneratorIterator(handle_type handle) noexcept : handle(handle) {}

			//destructor 
			~GeneratorIterator() noexcept = default;

			//util
			GeneratorIterator& self() noexcept { return *this; }
			const GeneratorIterator& self() const noexcept { return *this; }
			friend bool operator==(const GeneratorIterator& it, GeneratorSentinel) noexcept {
				return !it.handle || it.handle.done();
			}
			friend bool operator!=(const GeneratorIterator& it, GeneratorSentinel sentinel) noexcept {
				return !(it == sentinel);
			}
			friend bool operator==(GeneratorSentinel sentinel, const GeneratorIterator& it) noexcept {
				return (it == sentinel);
			}
			friend bool operator!=(GeneratorSentinel sentinel, const GeneratorIterator& it) noexcept {
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
		Generator(handle_type handle) : handle(handle) {}
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