#pragma once 

//own
#include "../util/basicTypes.h"
#include "../util/allocator.h"
#include "../util/typeTraits.h"
#include "../util/dataMovement.h"
#include "../util/typeInfo.h"
#include "../fundamental/option.h"

//interface 
namespace natl {

	namespace impl {
		struct AnyStorageConstexprPolymorphic {
		public:
			using any_storage_constexpr_polymorphic = AnyStorageConstexprPolymorphic;
			using copy_function_type = any_storage_constexpr_polymorphic * (*)(const any_storage_constexpr_polymorphic*);
			using destory_function_type = void(*)(any_storage_constexpr_polymorphic*);
			using get_type_info_function_type = TypeInfo(*)();

			constexpr virtual copy_function_type getCopyFunction() const noexcept = 0;
			constexpr virtual destory_function_type getDestoryFunction() const noexcept = 0;
			constexpr virtual get_type_info_function_type getTypeInfoFunction() const noexcept = 0;
		};

		template<typename DataType>
		struct AnyStorageConstexpr final : public AnyStorageConstexprPolymorphic {
		public:
			using allocator_type = DefaultAllocator;
			using typed_allocator_type = allocator_type::template rebind<AnyStorageConstexpr>;
			using any_storage_constexpr = AnyStorageConstexpr;
			using any_storage_constexpr_polymorphic = AnyStorageConstexprPolymorphic;
			using value_type = DataType;

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
				AnyStorageConstexpr* newData = typed_allocator_type::allocate(1);
				construct(newData, natl::forward<ArgTypes>(args)...);
				return static_cast<any_storage_constexpr_polymorphic*>(newData);
			}
			//polymorphic functions 
			constexpr copy_function_type getCopyFunction() const noexcept override {
				return [](const any_storage_constexpr_polymorphic* anyStoragePolymorphic) -> any_storage_constexpr_polymorphic* {
					const any_storage_constexpr* anyStorage = static_cast<const any_storage_constexpr*>(anyStoragePolymorphic);
					return createNew(*anyStorage->getPtr());
				};
			}
			constexpr destory_function_type getDestoryFunction() const noexcept override {
				return [](any_storage_constexpr_polymorphic* anyStoragePolymorphic) -> void {
					any_storage_constexpr* anyStorage = static_cast<any_storage_constexpr*>(anyStoragePolymorphic);
					deconstruct(anyStorage);
					typed_allocator_type::deallocate(anyStorage, 1);
				};
			}
			constexpr get_type_info_function_type getTypeInfoFunction() const noexcept override {
				return []() -> TypeInfo {
					return getTypeInfo<DataType>();
				};
			}

			//element access
			constexpr value_type* getPtr() noexcept {
				return &data;
			}
			constexpr const value_type* getPtr() const noexcept {
				return &data;
			}
		};


		struct AnyStoragePolymorphic {
		public:
			using any_storage_polymorphic = AnyStoragePolymorphic;
			using destory_function_type = void(*)(any_storage_polymorphic*, const Bool);
			
			virtual any_storage_polymorphic* copyStorage(Byte* smallBuffer) const noexcept = 0;
			virtual any_storage_polymorphic* moveStorage(Byte* smallBuffer) noexcept = 0;
			virtual destory_function_type getDestoryFunction() const noexcept = 0;
		};

		template<typename DataType, typename Alloc>
			requires(IsAllocatorC<Alloc>)
		struct AnyStorage final : public AnyStoragePolymorphic {
		public:
			using value_type = DataType;
			using typed_allocator_type = Alloc::template rebind<AnyStorage>;
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
				any_storage* newData = typed_allocator_type::allocate(1);
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
					deconstruct(anyStorage);
					if (shouldDeallocate == true) {
						typed_allocator_type::deallocate(anyStorage, 1);
					}
				};
			}


			//element access
			constexpr value_type* getPtr() noexcept {
				return &data;
			}
			constexpr const value_type* getPtr() const noexcept {
				return &data;
			}
		};

		enum AnyStorageState : ui8 {
			noValue,
			smallBufferStorage,
			heapStorage,
			constexprStorage,
		};

		template<Size BufferSize, typename DataType>
		concept BaseAnyDoesDataTypeFitInSmallBuffer = sizeof(AnyStorage<DataType, DefaultAllocator>) <= BufferSize;

		#ifdef NATL_64BIT
			constexpr inline Size BaseAnyBaseMemberByteSize = 16;
		#endif // NATL_64BIT

		#ifdef NATL_32BIT
			constexpr inline Size BaseAnyBaseMemberByteSize = 12;
		#endif // NATL_32BIT
	}


	template<Size BufferSize = 32>
	struct BaseAny {
	public:
		constexpr static Size bufferSize = BufferSize;
	private:
		using any_storage_constexpr_polymorphic = impl::AnyStorageConstexprPolymorphic;
		using any_storage_polymorphic = impl::AnyStoragePolymorphic;
	
	private:
		impl::AnyStorageState storageState;
		ui32 numberOfBytesUsed;
		const TypeInfo* typeInfo;
		union {
			Dummy dummy;
			Byte smallBufferStorage[BufferSize];
			any_storage_polymorphic* heapStorage;
			any_storage_constexpr_polymorphic* constexprStorage;
		};

		template<Size AnyBufferSize>
		friend struct BaseAny;

	private:
		constexpr void constructBaseInit() noexcept {
			storageState = impl::AnyStorageState::noValue;
			numberOfBytesUsed = 0;
			typeInfo = nullptr;
		}
	public:

		//constructor 
		constexpr BaseAny() noexcept : 
			storageState(impl::AnyStorageState::noValue),
			numberOfBytesUsed(0),
			typeInfo(nullptr) {
			constructAsDummy();
		}

		constexpr BaseAny(const BaseAny& other) noexcept {
			constructBaseInit();
			assign(other);
		}

		constexpr BaseAny(BaseAny&& other) noexcept {
			constructBaseInit();
			assign(forward<BaseAny>(other));
		}

		template<Size OtherBufferSize>
		constexpr BaseAny(const BaseAny<OtherBufferSize>& other) noexcept {
			constructBaseInit();
			assign(other);
		}
		template<Size OtherBufferSize>
		constexpr BaseAny(BaseAny<OtherBufferSize>&& other) noexcept {
			constructBaseInit();
			assign(natl::forward<BaseAny<OtherBufferSize>>(other));
		}
	
		template<typename DataType, typename Alloc, typename... ArgTypes>
		constexpr BaseAny(TypeArg<DataType>, AllocatorArg<Alloc>, ArgTypes&&... args) noexcept {
			constructBaseInit();
			assign(TypeArg<DataType>{}, AllocatorArg<Alloc>{}, natl::forward<ArgTypes>(args)...);
		}


		//destructor 
	private:
		constexpr void deconstruct() noexcept {
			if (isConstantEvaluated()) {
				if (storageState != impl::AnyStorageState::noValue) {
					constexprStorage->getDestoryFunction()(constexprStorage);
					natl::deconstruct(&constexprStorage);
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
			deconstruct();
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
		constexpr void assign(TypeArg<DataType>, AllocatorArg<Alloc>, ArgTypes&&... args) noexcept {
			if (isConstantEvaluated()) {
				storageState = impl::AnyStorageState::constexprStorage;
				using arg_storage_constexpr_type = impl::AnyStorageConstexpr<Decay<DataType>>;
				any_storage_constexpr_polymorphic* newConstexprStorage = arg_storage_constexpr_type::createNew(natl::forward<ArgTypes>(args)...);
				construct(&constexprStorage, newConstexprStorage);
			} else {
				typeInfo = getTypeInfoPtr<Decay<DataType>>();
				numberOfBytesUsed = TypeByteSize<DataType>;
				using arg_storage = impl::AnyStorage<Decay<DataType>, Alloc>;
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
		constexpr void assign(const BaseAny<OtherBufferSize>& other) noexcept {
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
				typeInfo = other.typeInfo;
				switch (other.storageState) {
				case impl::AnyStorageState::noValue:
					break;
				case impl::AnyStorageState::smallBufferStorage: {
					const any_storage_polymorphic* otherStorage = 
						reinterpret_cast<const any_storage_polymorphic*>(other.smallBufferStorage);
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
		constexpr void assign(BaseAny<OtherBufferSize>&& other) noexcept {
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
				typeInfo = other.typeInfo;
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
					other.deconstruct();
					break;
				}
				case impl::AnyStorageState::heapStorage: {
					any_storage_polymorphic* otherStorage = other.heapStorage;
					if (other.numberOfBytesUsed <= BufferSize) {
						storageState = impl::AnyStorageState::smallBufferStorage;
						otherStorage->moveStorage(smallBufferStorage);
						other.deconstruct();
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
	public:

		constexpr BaseAny operator=(const BaseAny& other) noexcept {
			constructBaseInit();
			assign(other);
			return self();
		}
		constexpr BaseAny operator=(BaseAny&& other) noexcept {
			constructBaseInit();
			assign(forward<BaseAny>(other));
			return self();
		}

		template<Size OtherBufferSize>
		constexpr BaseAny operator=(const BaseAny<OtherBufferSize>& other) noexcept {
			deconstruct();
			assign(other);
			return self();
		}
		template<Size OtherBufferSize>
		constexpr BaseAny operator=(BaseAny<OtherBufferSize>&& other) noexcept {
			deconstruct();
			assign(natl::forward<BaseAny<OtherBufferSize>>(other));
			return self();
		}

		//modifiers 
		constexpr void reset() noexcept {
			deconstruct();
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

		//access 
		template<typename DataType>
		constexpr OptionPtr<DataType> getAs() noexcept {
			if (isConstantEvaluated()) {
				if (doesNotHaveValue()) {
					return OptionEmpty{};
				}

				if (constexprStorage->getTypeInfoFunction()() != getTypeInfo<Decay<DataType>>()) {
					return OptionEmpty{};
				}

				using any_storage_type = impl::AnyStorageConstexpr<Decay<DataType>>;
				any_storage_type* anyStorage = static_cast<any_storage_type*>(constexprStorage);
				return anyStorage->getPtr();
			} else {
				switch (storageState) {
				case impl::AnyStorageState::noValue:
					return OptionEmpty{};
				case impl::AnyStorageState::smallBufferStorage: {
					if (*typeInfo != getTypeInfo<Decay<DataType>>()) {
						return OptionEmpty{};
					}

					using any_storage_type = impl::AnyStorage<Decay<DataType>, DefaultAllocator>;
					any_storage_type* anyStorage = reinterpret_cast<any_storage_type*>(smallBufferStorage);
					return anyStorage->getPtr();
				}
				case impl::AnyStorageState::heapStorage: {
					if (*typeInfo != getTypeInfo<Decay<DataType>>()) {
						return OptionEmpty{};
					}

					using any_storage_type = impl::AnyStorage<Decay<DataType>, DefaultAllocator>;
					any_storage_type* anyStorage = reinterpret_cast<any_storage_type*>(heapStorage);
					return anyStorage->getPtr();
				}
				default:
					unreachable();
				}
			}
		}
	};

	using Any = BaseAny<32 - impl::BaseAnyBaseMemberByteSize>;
	static_assert(TypeByteSize<Any> == 32, "natl: Any should be 32 bytes");

	template<Size ByteSize = 32>
		requires(ByteSize >= 16)
	using AnyByteSize = BaseAny<ByteSize - impl::BaseAnyBaseMemberByteSize>;

	template<typename DataType, typename... ArgTypes>
	constexpr Any makeAny(ArgTypes&&... args) noexcept {
		return Any(TypeArg<DataType>{}, AllocatorArg<DefaultAllocator>{}, natl::forward<ArgTypes>(args)...);
	}
	template<Size ByteSize, typename DataType, typename... ArgTypes>
	constexpr AnyByteSize<ByteSize> makeAnyByteSize(ArgTypes&&... args) noexcept {
		return AnyByteSize<ByteSize>(TypeArg<DataType>{}, AllocatorArg<DefaultAllocator>{}, natl::forward<ArgTypes>(args)...);
	}

	template<Size ByteSize, typename Alloc, typename DataType, typename... ArgTypes>
		requires(IsAllocatorC<Alloc>)
	constexpr AnyByteSize<ByteSize> makeAnyByteSizeAlloc(ArgTypes&&... args) noexcept {
		return AnyByteSize<ByteSize>(TypeArg<DataType>{}, AllocatorArg<Alloc>{}, natl::forward<ArgTypes>(args)...);
	}

	template<typename DataType, Size BufferSize>
	constexpr OptionPtr<DataType> anyCast(BaseAny<BufferSize>& baseAny) noexcept {
		return baseAny.template getAs<DataType>();
	}

}