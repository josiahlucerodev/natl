#pragma once 

//own
#include "typeTraits.h"
#include "mutex.h"
#include "tuple.h"

//interface 
namespace natl {
	template<typename DataType, typename MutexType>
	class SynchronizedValueBase {
	public:
		using value_type = DataType;
		using mutex_type = MutexType;
	private:
		MutexType rwMutex;
		DataType data;
	public:
		//constructor
		constexpr SynchronizedValueBase(const SynchronizedValueBase&) = delete;
		template<typename... ArgTypes> 
		constexpr SynchronizedValueBase(ArgTypes&&... args) noexcept :
			rwMutex(), data(natl::forward<ArgTypes>(args)...) {}

		//destructor 
		constexpr ~SynchronizedValueBase() noexcept = default;
		
		//util 
		constexpr SynchronizedValueBase& self() noexcept { return *this; }
		constexpr const SynchronizedValueBase& self() const noexcept { return *this; }

		//assignment
		constexpr SynchronizedValueBase& operator=(const SynchronizedValueBase&) = delete;

		//access 
		constexpr DataType blockingGetCopy() noexcept {
			rwMutex.lock();
			DataType copyOfdata = data;
			rwMutex.unlock();
			return copyOfdata;
		}

		template<typename, typename>
		friend class UpdateGuardBase;
	};

	template<typename DataType, typename MutexType>
	class UpdateGuardBase {
	public:
		using value_type = DataType;
		using mutex_type = MutexType;
		using synchronized_value_type = SynchronizedValueBase<RemoveConstT<DataType>, MutexType>;
	private:
		synchronized_value_type& synchronizedValueRef;
	public:
		//constructor 
		constexpr UpdateGuardBase() noexcept = delete;
		constexpr UpdateGuardBase(const UpdateGuardBase&) noexcept = delete;
		constexpr UpdateGuardBase(synchronized_value_type& synchronizedValueRefIn) noexcept
			: synchronizedValueRef(synchronizedValueRefIn) {
			synchronizedValueRef.rwMutex.lock();
		};
		constexpr UpdateGuardBase(const synchronized_value_type& synchronizedValueRefIn) noexcept
			requires(IsConstV<DataType>) : synchronizedValueRef(synchronizedValueRefIn) {
			synchronizedValueRef.rwMutex.lock();
		};

		//destructor 
		constexpr ~UpdateGuardBase() noexcept {
			synchronizedValueRef.rwMutex.unlock();
		}

		//util 
		constexpr UpdateGuardBase& self() noexcept { return *this; }
		constexpr const UpdateGuardBase& self() const noexcept { return *this; }

		//assignment
		constexpr UpdateGuardBase& operator=(const UpdateGuardBase&) = delete;

		//access
		constexpr DataType& operator->() noexcept { return synchronizedValueRef.data; }
		constexpr DataType& value() noexcept { return synchronizedValueRef.data; }
	};

	template<typename DataType>
	using SynchronizedValue = SynchronizedValueBase<DataType, Mutex>;
	template<typename DataType>
	using UpdateGuard = UpdateGuardBase<DataType, Mutex>;

	template<typename DataType>
	using RecursiveSynchronizedValue = SynchronizedValueBase<DataType, RecursiveMutex>;
	template<typename DataType>
	using RecursiveUpdateGuard = UpdateGuardBase<DataType, RecursiveMutex>;

	template<typename SynchronizedValueType>
	using UpdateGuardOf = UpdateGuardBase<typename SynchronizedValueType::value_type, typename SynchronizedValueType::mutex_type>;
	template<typename UpdateGuardType>
	using SynchronizedValueOf = SynchronizedValueBase<typename UpdateGuardType::value_type, typename UpdateGuardType::mutex_type>;


	namespace impl {
		template<typename Functor, typename... SynchronizedValueTypes, Size... Indices>
		constexpr auto synchronizedApplyImpl(IndexSequence<Indices...>, Functor&& functor,  SynchronizedValueTypes&... synchronizedValues) noexcept {
			natl::Tuple<UpdateGuardOf<SynchronizedValueTypes>...> gaurdStorageTuple(synchronizedValues...);
			return natl::forward<Functor>(functor)(gaurdStorageTuple.template get<Indices>().value()...);
		}
	}

	template<typename Functor, typename... ArgTypes, typename... MutexTypes>
	constexpr auto synchronizedApply(Functor&& functor, SynchronizedValueBase<ArgTypes, MutexTypes>&... synchronizedValues) noexcept {
		return impl::synchronizedApplyImpl(MakeIndexSequence<sizeof...(ArgTypes)>{},
			natl::forward<Functor>(functor), synchronizedValues...);
	}
}