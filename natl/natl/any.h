#pragma once 

//own
#include "basicTypes.h"
#include "allocator.h"
#include "typeTraits.h"
#include "dataMovement.h"

//interface 
namespace natl {

	namespace impl {
		class AnyStorageConstexprPolymorphic {
		public:
			using any_storage_constexpr_polymorphic = AnyStorageConstexprPolymorphic;
			using copy_function_type = any_storage_constexpr_polymorphic * (*)(const any_storage_constexpr_polymorphic*);
			using destory_function_type = void(*)(any_storage_constexpr_polymorphic*);

			constexpr virtual copy_function_type getCopyFunction() const noexcept = 0;
			constexpr virtual destory_function_type getDestoryFunction() const noexcept = 0;
		};

		template<typename DataType, typename Alloc>
			requires(IsAllocator<Alloc>)
		class AnyStorageConstexpr final : public AnyStorageConstexprPolymorphic {
		public:
			using value_type = DataType;
			using allocator_type = Alloc::template rebind_alloc<AnyStorageConstexpr>;
			using any_storage_constexpr = AnyStorageConstexpr;
			using any_storage_constexpr_polymorphic = AnyStorageConstexprPolymorphic;

			using copy_function_type = any_storage_constexpr_polymorphic::copy_function_type;
			using destory_function_type = any_storage_constexpr_polymorphic::destory_function_type;
		private:
			DataType data;
		public:
			//constructor
			template<typename... ArgTypes>
			constexpr AnyStorageConstexpr(ArgTypes&&... args) noexcept : data(natl::forward<ArgTypes>(args)...) {}

			//creation
			template<typename... ArgTypes>
			constexpr static any_storage_constexpr_polymorphic* createNew(ArgTypes&&... args) noexcept {
				AnyStorageConstexpr* newData = allocator_type::allocate(1);
				construct(newData, natl::forward<ArgTypes>(args)...);
				return static_cast<any_storage_constexpr_polymorphic*>(newData);;
			}
			//polymorphic functions 
			constexpr copy_function_type getCopyFunction() const noexcept override {
				return [](const any_storage_constexpr_polymorphic* anyStoragePolymorphic) -> any_storage_constexpr_polymorphic* {
					const any_storage_constexpr* anyStorage = static_cast<const any_storage_constexpr*>(anyStoragePolymorphic);
					return createNew(anyStorage->getValue());
				};
			}
			constexpr destory_function_type getDestoryFunction() const noexcept override {
				return [](any_storage_constexpr_polymorphic* anyStoragePolymorphic) -> void {
					any_storage_constexpr* anyStorage = static_cast<any_storage_constexpr*>(anyStoragePolymorphic);
					allocator_type::deallocate(anyStorage, 1);
					deconstruct(anyStorage);
				};
			}

			//element access
			constexpr value_type getValue() noexcept {
				return data;
			}
			constexpr const value_type& getValue() const noexcept {
				return data;
			}
		};


		class AnyStoragePolymorphic {
		public:
			using any_storage_polymorphic = AnyStoragePolymorphic;
			using destory_function_type = void(*)(any_storage_polymorphic*, const Bool);
			
			virtual any_storage_polymorphic* copyStorage(Byte* smallBuffer) const noexcept = 0;
			virtual any_storage_polymorphic* moveStorage(Byte* smallBuffer) noexcept = 0;
			virtual destory_function_type getDestoryFunction() const noexcept = 0;
		};

		template<typename DataType, typename Alloc>
			requires(IsAllocator<Alloc>)
		class AnyStorage : public AnyStoragePolymorphic {
		public:
			using value_type = DataType;
			using allocator_type = Alloc::template rebind_alloc<AnyStorage>;
			using any_storage = AnyStorage;
			using any_storage_polymorphic = AnyStoragePolymorphic;

			using destory_function_type = any_storage_polymorphic::destory_function_type;
		private:
			DataType data;
		public:
			//constructor 
			template<typename... ArgTypes>
			constexpr AnyStorage(ArgTypes&&... args) noexcept : data(natl::forward<ArgTypes>(args)...) {}

			//creation
			template<typename... ArgTypes>
			static void createNewInSmallBuffer(Byte* smallBuffer, ArgTypes&&... args) noexcept {
				any_storage* newData = reinterpret_cast<any_storage*>(smallBuffer);
				construct(newData, natl::forward<ArgTypes>(args)...);
			}

			template<typename... ArgTypes>
			static any_storage_polymorphic* createNewOnHeap(ArgTypes&&... args) noexcept {
				any_storage* newData = allocator_type::allocate(1);
				construct(newData, natl::forward<ArgTypes>(args)...);
				return static_cast<any_storage_polymorphic*>(newData);
			}

			//polymorphic functions 
			any_storage_polymorphic* copyStorage(Byte* smallBuffer) const noexcept override {
				if (smallBuffer != nullptr) {
					createNewInSmallBuffer(smallBuffer, data);
					return nullptr;
				} else {
					return createNewOnHeap(data);
				}
			}
			any_storage_polymorphic* moveStorage(Byte* smallBuffer) noexcept override {
				if (smallBuffer != nullptr) {
					createNewInSmallBuffer(smallBuffer, natl::move(data));
					return nullptr;
				} else {
					return createNewOnHeap(natl::move(data));
				}
			}
			destory_function_type getDestoryFunction() const noexcept override {
				return [](any_storage_polymorphic* anyStoragePolymorphic, const Bool shouldDeallocate) -> void {
					any_storage* anyStorage = static_cast<any_storage*>(anyStoragePolymorphic);
					if (shouldDeallocate == true) {
						allocator_type::deallocate(anyStorage, 1);
					}
					deconstruct(anyStorage);
				};
			}


			//element access
			constexpr value_type getValue() noexcept {
				return data;
			}
			constexpr const value_type& getValue() const noexcept {
				return data;
			}
		};

		enum AnyStorageState : ui8 {
			noValue,
			smallBufferStorage,
			heapStorage,
			constexprStorage,
		};

		template<Size BufferSize, typename DataType>
		concept BaseAnyDoesDataTypeFitInSmallBuffer = sizeof(AnyStorage<DataType, DefaultAllocatorByte>) <= BufferSize;

		#ifdef NATL_64BIT
			constexpr inline Size BaseAnyBaseMemberByteSize = 8;
		#endif // NATL_64BIT

		#ifdef NATL_32BIT
			constexpr inline Size BaseAnyBaseMemberByteSize = 4;
		#endif // NATL_32BIT
	}


	template<Size BufferSize = 32>
	class BaseAny {
	public:
		constexpr static Size bufferSize = BufferSize;
	private:
		using any_storage_constexpr_polymorphic = impl::AnyStorageConstexprPolymorphic;
		using any_storage_polymorphic = impl::AnyStoragePolymorphic;
	private:
#ifdef NATL_64BIT
		impl::AnyStorageState storageState;
		i32 numberOfBytesUsed;
#endif // NATL_64BIT

#ifdef NATL_32BIT
		impl::AnyStorageState storageState : 4;
		ui32 numberOfBytesUsed : 32 - 4;
#endif // NATL_32BIT
		union {
			Dummy dummy;
			Byte smallBufferStorage[BufferSize];
			any_storage_polymorphic* heapStorage;
			any_storage_constexpr_polymorphic* constexprStorage;
		};

		template<Size AnyBufferSize>
		friend class BaseAny;
	public:

		//constructor 
		constexpr BaseAny() noexcept : storageState(impl::AnyStorageState::noValue) {
			constructAsDummy();
		}

		template<Size OtherBufferSize>
		constexpr BaseAny(const BaseAny<OtherBufferSize>& other) noexcept {
			internalAssign(other);
		}
		template<Size OtherBufferSize>
		constexpr BaseAny(BaseAny<OtherBufferSize>&& other) noexcept {
			internalAssign(natl::forward<BaseAny<OtherBufferSize>>(other));
		}
	
		template<typename DataType, typename Alloc, typename... ArgTypes>
		constexpr BaseAny(TypeArg<DataType>, AllocatorArg<Alloc>, ArgTypes&&... args) noexcept {
			internalAssign(TypeArg<DataType>{}, AllocatorArg<Alloc>{}, natl::forward<ArgTypes>(args)...);
		}


		//destructor 
	private:
		constexpr void internalDeconstruct() noexcept {
			if (isConstantEvaluated()) {
				if (storageState != impl::AnyStorageState::noValue) {
					constexprStorage->getDestoryFunction()(constexprStorage);
					deconstruct(&constexprStorage);
				}
			} else {
				switch (storageState) {
				case impl::AnyStorageState::noValue:
					break;
				case impl::AnyStorageState::smallBufferStorage: {
					any_storage_polymorphic* storage = reinterpret_cast<any_storage_polymorphic*>(smallBufferStorage);
					storage->getDestoryFunction()(storage, false);
					break;
				}
				case impl::AnyStorageState::heapStorage:
					heapStorage->getDestoryFunction()(heapStorage, true);
					break;
				default:
					unreachable();
				}
			}
		}
	public:
		constexpr ~BaseAny() noexcept {
			internalDeconstruct();
		}

		//util 
		constexpr BaseAny& self() noexcept { return *this; }
		constexpr const BaseAny& self() const noexcept { return *this; }

		//assignment 
	private:
		constexpr void constructAsDummy() noexcept {
			if (isConstantEvaluated()) {
				construct<Dummy>(&dummy);
			}
		}

		template<typename DataType, typename Alloc, typename... ArgTypes>
		constexpr void internalAssign(TypeArg<DataType>, AllocatorArg<Alloc>, ArgTypes&&... args) noexcept {
			if (isConstantEvaluated()) {
				storageState = impl::AnyStorageState::constexprStorage;
				using arg_storage_constexpr_type = impl::AnyStorageConstexpr<DataType, Alloc>;
				any_storage_constexpr_polymorphic* newConstexprStorage = arg_storage_constexpr_type::createNew(natl::forward<ArgTypes>(args)...);
				construct(&constexprStorage, newConstexprStorage);
			} else {
				numberOfBytesUsed = TypeByteSize<DataType>;
				using arg_storage = impl::AnyStorage<DataType, Alloc>;
				if constexpr (impl::BaseAnyDoesDataTypeFitInSmallBuffer<BufferSize, DataType>) {
					storageState = impl::AnyStorageState::smallBufferStorage;
					arg_storage::createNewInSmallBuffer(smallBufferStorage, natl::forward<ArgTypes>(args)...);
				} else {
					storageState = impl::AnyStorageState::heapStorage;
					heapStorage = arg_storage::createNewOnHeap(natl::forward<ArgTypes>(args)...);
				}
			}
		}
	
		template<Size OtherBufferSize>
		constexpr void internalAssign(const BaseAny<OtherBufferSize>& other) noexcept {
			if (isConstantEvaluated()) {
				storageState = other.storageState;
				if (other.storageState == impl::AnyStorageState::noValue) {
					constructAsDummy();
				} else {
					any_storage_constexpr_polymorphic* newConstexprStorage = other.constexprStorage->getCopyFunction()(nullptr);
					construct(&constexprStorage, newConstexprStorage);
				}
			} else {
				storageState = other.storageState;
				numberOfBytesUsed = other.numberOfBytesUsed;
				switch (other.storageState) {
				case impl::AnyStorageState::noValue:
					break;
				case impl::AnyStorageState::smallBufferStorage: {
					const any_storage_polymorphic* otherStorage = 
						reinterpret_cast<const any_storage_polymorphic*>(other.smallBufferStorage.storage);
					if (other.numberOfBytesUsed <= BufferSize) {
						otherStorage->copyStorage(smallBufferStorage);
					} else {
						storageState = impl::AnyStorageState::heapStorage;
						heapStorage = otherStorage->copyStorage(nullptr);
					}
					break;
				}
				case impl::AnyStorageState::heapStorage: {
					const any_storage_polymorphic* otherStorage = other.heapStorage;
					if (other.numberOfBytesUsed <= BufferSize) {
						storageState = impl::AnyStorageState::smallBufferStorage;
						otherStorage->copyStorage(smallBufferStorage);
					} else {
						heapStorage = otherStorage->copyStorage(nullptr);
					}
					break;
				}
				default:
					unreachable();
				}
			}
		}

		template<Size OtherBufferSize>
		constexpr void internalAssign(BaseAny<OtherBufferSize>&& other) noexcept {
			if (isConstantEvaluated()) {
				storageState = other.storageState;
				if (other.storageState == impl::AnyStorageState::noValue) {
					constructAsDummy();
				} else {
					construct(&constexprStorage, other.constexprStorage);
					other.constructAsDummy();
				}
			} else {
				storageState = other.storageState;
				numberOfBytesUsed = other.numberOfBytesUsed;
				switch (other.storageState) {
				case impl::AnyStorageState::noValue:
					break;
				case impl::AnyStorageState::smallBufferStorage: {
					any_storage_polymorphic* otherStorage =
						reinterpret_cast<any_storage_polymorphic*>(other.smallBufferStorage);
					if (other.numberOfBytesUsed <= BufferSize) {
						otherStorage->moveStorage(smallBufferStorage);
					} else {
						storageState = impl::AnyStorageState::heapStorage;
						heapStorage = otherStorage->moveStorage(nullptr);
					}
					other.internalDeconstruct();
					break;
				}
				case impl::AnyStorageState::heapStorage: {
					any_storage_polymorphic* otherStorage = other.heapStorage;
					if (other.numberOfBytesUsed <= BufferSize) {
						storageState = impl::AnyStorageState::smallBufferStorage;
						otherStorage->moveStorage(smallBufferStorage);
						other.internalDeconstruct();
					} else {
						heapStorage = otherStorage;
					}	
					break;
				}
				default:
					unreachable();
				}
				other.storageState = impl::AnyStorageState::noValue;
			}
		}

		template<Size OtherBufferSize>
		constexpr BaseAny operator=(const BaseAny<OtherBufferSize>& other) noexcept {
			internalAssign(other);
			return self();
		}
		template<Size OtherBufferSize>
		constexpr BaseAny operator=(BaseAny<OtherBufferSize>&& other) noexcept {
			internalAssign(natl::forward<BaseAny<OtherBufferSize>>(other));
			return self();
		}

		//modifiers 
		constexpr void reset() noexcept {
			internalDeconstruct();
			constructAsDummy();
			storageState = impl::AnyStorageState::noValue;
		}

		//observers 
		constexpr Bool hasValue() const noexcept {
			return storageState != impl::AnyStorageState::noValue;
		}
		constexpr Bool doesNotHaveValue() const noexcept {
			return storageState == impl::AnyStorageState::noValue;
		}
	};

	using Any = BaseAny<32 - impl::BaseAnyBaseMemberByteSize>;
	static_assert(TypeByteSize<Any> == 32, "natl: Any should be 32 bytes");

	template<Size ByteSize = 32>
		requires(ByteSize >= 16)
	using AnyByteSize = BaseAny<ByteSize - impl::BaseAnyBaseMemberByteSize>;

	template<class DataType, class... ArgTypes>
	constexpr Any makeAny(ArgTypes&&... args) noexcept {
		return Any(TypeArg<DataType>{}, AllocatorArg<DefaultAllocator<DataType>>{}, natl::forward<ArgTypes>(args)...);
	}
	template<Size ByteSize, class DataType, class... ArgTypes>
	constexpr AnyByteSize<ByteSize> makeAnyByteSize(ArgTypes&&... args) noexcept {
		return AnyByteSize<ByteSize>(TypeArg<DataType>{}, AllocatorArg<DefaultAllocator<DataType>>{}, natl::forward<ArgTypes>(args)...);
	}
}