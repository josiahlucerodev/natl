#pragma once

//own
#include "typeTraits.h"
#include "allocator.h"
#include "dataMovement.h"
#include "algorithm.h"
#include "../util/bits.h"
#include "../sync/atomic.h"
#include "../container/functional.h"

//interface
namespace natl {
	template<typename DataType>
	struct Ptr {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
	private:
		DataType* dataPtr;
	public:
		//constructor 
		constexpr Ptr() noexcept : dataPtr(nullptr) {}
		constexpr Ptr(DataType* data) noexcept requires(IsNotConst<DataType>) : dataPtr(&data) {};
		constexpr Ptr(const DataType* data) noexcept requires(IsConst<DataType>) : dataPtr(data) {};

		//destructor
		constexpr ~Ptr() noexcept = default;

		//access
		constexpr reference unwrap() noexcept requires(IsNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference unwrap() const noexcept { return *dataPtr; };
		constexpr reference get() noexcept requires(IsNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference get() const noexcept { return *dataPtr; };
		constexpr pointer getPtr() noexcept requires(IsNotConst<DataType>) { return dataPtr; };
		constexpr const_pointer getPtr() const noexcept { return dataPtr; };
		constexpr reference operator*() noexcept requires(IsNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference operator*() const noexcept { return *dataPtr; };
		constexpr reference operator->() noexcept requires(IsNotConst<DataType>) { return *dataPtr; };
		constexpr const_reference operator->() const noexcept { return *dataPtr; };
	};

	template<typename DataType>
	struct IsTriviallyCompareableV<Ptr<DataType>>
		: TrueType {};

	template<typename DataType>
	struct IsTriviallyRelocatableV<Ptr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyConstructibleV<Ptr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyDestructibleV<Ptr<DataType>>
		: TrueType {};

	template<typename DataType>
	struct IsTriviallyConstRefConstructibleV<Ptr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyMoveConstructibleV<Ptr<DataType>>
		: TrueType {};

	template<typename DataType>
	struct IsTriviallyConstRefAssignableV<Ptr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyMoveAssignableV<Ptr<DataType>>
		: TrueType {};

	template<typename Deleter, typename DataType>
	concept IsDeleter = HasFunctionSignature<Deleter, void, DataType*>;

	template<typename DataType, typename Alloc = DefaultAllocator<DataType>>
	struct DefaultDeleter {
	public:

		//constructor
		constexpr DefaultDeleter() noexcept = default;

		//delete operations
		constexpr void operator()(typename Alloc::pointer ptr) const {
			natl::deconstruct<DataType>(ptr);
			Alloc::deallocate(ptr, 1); 
		}
	};

	template<typename DataType>
	struct NewDeleteDeleter {
	public:
		//constructor
		constexpr NewDeleteDeleter() noexcept = default;

		//delete operations
		constexpr void operator()(DataType* ptr) const {
			delete ptr;
		}
	};

	template<typename DataType, typename Alloc = DefaultAllocator<DataType>, typename Deleter = DefaultDeleter<DataType, Alloc>>
		requires(IsAllocator<Alloc> && IsDeleter<Deleter, DataType>)
	struct UniquePtr {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;

		using element_type = DataType;
		using deleter_type = Deleter;
	private:
		pointer dataPtr;
		[[no_unique_address]] deleter_type deleter;
	public:
		//constructor 
		constexpr UniquePtr() noexcept : dataPtr(nullptr), deleter() {};
		constexpr UniquePtr(const UniquePtr&) noexcept = delete;

		
		constexpr UniquePtr(UniquePtr&& other) noexcept requires(IsEmpty<Deleter>) : dataPtr(other.dataPtr) {
			other.dataPtr = nullptr;
		}
		constexpr UniquePtr(UniquePtr&& other) noexcept requires(IsNotEmpty<Deleter>) : 
			dataPtr(other.dataPtr), deleter(natl::move(deleter)) {
			other.dataPtr = nullptr;
		}

		constexpr UniquePtr(NullptrType) noexcept : dataPtr(nullptr), deleter() {}

		constexpr UniquePtr(pointer ptr) noexcept requires(IsEmpty<Deleter>) { dataPtr = ptr; }
		constexpr UniquePtr(const value_type& value) noexcept requires(IsEmpty<Deleter>) {
			dataPtr = Alloc::allocate(1);
			natl::construct<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(value_type&& value) noexcept requires(IsEmpty<Deleter>) {
			dataPtr = Alloc::allocate(1);
			natl::construct<DataType, DataType>(dataPtr, natl::forward<DataType>(value));
		}

		constexpr UniquePtr(pointer ptr, const deleter_type& deleterIn) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(ptr), deleter(deleterIn) {}
		constexpr UniquePtr(pointer ptr, deleter_type&& deleterIn) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(ptr), deleter(natl::move(deleterIn)) {}
		constexpr UniquePtr(const value_type& value, const deleter_type& deleterIn) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(deleterIn) {
			dataPtr = Alloc::allocate(1);
			natl::construct<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(const value_type& value, deleter_type&& deleterIn) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(natl::move(deleterIn)) {
			dataPtr = Alloc::allocate(1);
			natl::construct<DataType, DataType>(dataPtr, value);
		}
		constexpr UniquePtr(value_type&& value, const deleter_type& deleterIn) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(deleterIn) {
			dataPtr = Alloc::allocate(1);
			natl::construct<DataType, DataType>(dataPtr, forward<DataType>(value));
		}
		constexpr UniquePtr(value_type&& value, deleter_type&& deleterIn) noexcept requires(IsNotEmpty<Deleter>) : dataPtr(nullptr), deleter(natl::move(deleterIn)) {
			dataPtr = Alloc::allocate(1);
			natl::construct<DataType, DataType>(dataPtr, forward<DataType>(value));
		}

		//destructor
	private:
		constexpr void destruct() noexcept {
			if (dataPtr) {
				if constexpr (IsEmpty<Deleter>) {
					Deleter{}(dataPtr);
				} else {
					deleter(dataPtr);
				}
			}
		}
	public:
		constexpr ~UniquePtr() noexcept {
			destruct();
		}

		//util 
		constexpr UniquePtr& self() noexcept { return *this; }
		constexpr const UniquePtr& self() const noexcept { return *this; }

		//assignment 
		constexpr UniquePtr& operator=(const UniquePtr&) = delete;
		constexpr UniquePtr& operator=(UniquePtr&& other) noexcept {
			reset(other.release());
			if constexpr (IsNotEmpty<Deleter>) {
				deleter = move(deleter);
			}
		}
		constexpr UniquePtr& operator=(NullptrType) noexcept {
			reset();
		}

		//modifiers
		constexpr pointer release() noexcept {
			pointer releasePtr = dataPtr;
			dataPtr = nullptr;
			return releasePtr;
		}
		constexpr void reset(pointer ptr = pointer()) noexcept {
			destruct();
			dataPtr = ptr;
		}
		constexpr void swap(UniquePtr& other) noexcept {
			exchange<UniquePtr>(dataPtr, other.dataPtr);
			if constexpr (IsNotEmpty<Deleter>) {
				exchange<Deleter>(deleter, deleter);
			}
		}

		//observers
		constexpr pointer get() noexcept { return dataPtr; }
		constexpr const_pointer get() const noexcept { return dataPtr; }

		constexpr Deleter& get_deleter() noexcept requires(IsNotEmpty<Deleter>) {
			return deleter;
		}
		constexpr const Deleter& get_deleter() const noexcept requires(IsNotEmpty<Deleter>) {
			return deleter;
		}

		constexpr Bool empty() const noexcept { return dataPtr == nullptr; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }
		explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

		constexpr reference operator*() noexcept { return *dataPtr; }
		constexpr const_reference operator*() const noexcept { return *dataPtr; }
		constexpr pointer operator->() noexcept { return dataPtr; }
		constexpr const_pointer operator->() const noexcept { return dataPtr; }

		//compare
		constexpr Bool operator==(const UniquePtr& other) const noexcept { return get() == other.get(); }
		constexpr Bool operator!=(const UniquePtr& other) const noexcept { return get() != other.get(); }
		constexpr Bool operator<(const UniquePtr& other) const noexcept { return get() < other.get(); }
		constexpr Bool operator<=(const UniquePtr& other) const noexcept { return get() <= other.get(); }
		constexpr Bool operator>(const UniquePtr& other) const noexcept { return get() > other.get(); }
		constexpr Bool operator>=(const UniquePtr& other) const noexcept { return get() >= other.get(); }
		constexpr std::compare_three_way_result_t<pointer> operator<=>(const UniquePtr& other) { return get() <=> other.get(); };

		constexpr Bool operator==(NullptrType) const noexcept { return !self(); }
		constexpr Bool operator!=(NullptrType) const noexcept { return static_cast<Bool>(self()); }

		//special
		constexpr Size hash() const noexcept {
			return natl::bitCast<Size, pointer>(dataPtr);
		}
	};

	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyCompareableV<UniquePtr<DataType, Alloc, Deleter>>
		: BoolConstant<IsEmpty<Deleter> || IsTriviallyCompareable<Deleter>> {};

	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyRelocatableV<UniquePtr<DataType, Alloc, Deleter>>
		: BoolConstant<IsEmpty<Deleter> || IsTriviallyRelocatable<Deleter>> {};
	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyConstructibleV<UniquePtr<DataType, Alloc, Deleter>>
		: BoolConstant<IsEmpty<Deleter> || IsTriviallyConstructible<Deleter>> {};
	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyDestructibleV<UniquePtr<DataType, Alloc, Deleter>>
		: FalseType {};

	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyConstRefConstructibleV<UniquePtr<DataType, Alloc, Deleter>>
		: FalseType {};
	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyMoveConstructibleV<UniquePtr<DataType, Alloc, Deleter>>
		: FalseType {};

	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyConstRefAssignableV<UniquePtr<DataType, Alloc, Deleter>>
		: FalseType {};
	template<typename DataType, typename Alloc, typename Deleter>
	struct IsTriviallyMoveAssignableV<UniquePtr<DataType, Alloc, Deleter>>
		: FalseType {};

	template<typename CharT, typename Traits, typename DataType, typename Alloc, typename Deleter>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const UniquePtr<DataType, Alloc, Deleter>& ptr) {
		os << ptr.get();
		return os;
	}

	template<typename PtrDataType, typename SmallDataType>
	struct PackedPtrAndSmallData {
	public:
		using pointer_type = PtrDataType*;
		using small_data_type = SmallDataType;
	private:
		constexpr static Size convertPtrToInt(const PtrDataType* ptr) noexcept {
			return natl::bitCast<Size, const PtrDataType*>(ptr);
		}
		constexpr static PtrDataType* convertIntToPtr(const Size ptr) noexcept {
			return natl::bitCast<PtrDataType*, Size>(ptr);
		}
#ifdef NATL_64BIT

#ifdef NATL_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif // NATL_COMPILER_GCC

		struct PackingType {
			Size smallDataStorage : natl::min<Size>(16, sizeof(SmallDataType) * 8);
			Size ptr : 48;
			constexpr PackingType() noexcept = default;
			constexpr PackingType(SmallDataType smallDataIn, PtrDataType* ptrIn) noexcept : 
				smallDataStorage(toUnderlying<SmallDataType>(smallDataIn)),
				ptr(convertPtrToInt(ptrIn)) {}

			//ptr
			constexpr void setPtr(const PtrDataType* ptrIn) noexcept { 
				ptr = convertPtrToInt(ptrIn);
			}
			constexpr PtrDataType* getPtr() noexcept { 
				return convertIntToPtr(ptr);
			}
			constexpr const PtrDataType* getPtr() const noexcept {
				return convertIntToPtr(ptr);
			}

			//smallData
			constexpr void setSmallData(const SmallDataType smallData) noexcept {
				smallDataStorage = toUnderlying<SmallDataType>(smallData);
			}
			constexpr SmallDataType getSmallData() const noexcept {
				return fromUnderlying<SmallDataType>(static_cast<UnderlyingType<SmallDataType>>(smallDataStorage));
			}
		};

#ifdef NATL_COMPILER_GCC
#pragma GCC diagnostic pop
#endif // NATL_COMPILER_GCC

		constexpr static Bool combindPacking = true;
#else 
		struct PackingType {
			SmallDataType smallDataStorage;
			PtrDataType* ptr;
			constexpr PackingType() noexcept = default;
			constexpr PackingType(const SmallDataType smallData, PtrDataType* ptr) noexcept : smallDataStorage(smallData), ptr(ptr) {}
			constexpr void setPtr(PtrDataType* ptrIn) noexcept { ptr = ptrIn; }
			constexpr PtrDataType* getPtr() noexcept { return ptr; }
			constexpr const PtrDataType* getPtr() const noexcept { return ptr; }
			constexpr void setSmallData(const SmallDataType smallData) noexcept { smallDataStorage = smallData; }
			constexpr SmallDataType getSmallData() const noexcept { return smallDataStorage; }
		};
		constexpr static Bool combindPacking = false;
#endif // NATL_64BIT

		struct ConstexprPackingType {
			SmallDataType smallDataStorage;
			PtrDataType* ptr;
			constexpr ConstexprPackingType() noexcept = default;
			constexpr ConstexprPackingType(const SmallDataType smallDataIn, PtrDataType* ptrIn) noexcept 
				: smallDataStorage(smallDataIn), ptr(ptrIn) {}
		};

		union {
			PackingType packing;
			ConstexprPackingType* constexprPackingPtr;
		};
	public:
		using packed_type = PackingType;
		//constructor 
		constexpr PackedPtrAndSmallData() noexcept {
			if (isConstantEvaluated()) {
				natl::construct(&constexprPackingPtr, new ConstexprPackingType{});
			} else {
				natl::construct<PackingType, PackingType>(&packing, PackingType{});
			}
		}
		constexpr PackedPtrAndSmallData(const PackedPtrAndSmallData& other) noexcept {
			if (isConstantEvaluated()) {
				natl::construct(&constexprPackingPtr, new ConstexprPackingType(other.constexprPackingPtr->smallDataStorage, other.constexprPackingPtr->ptr));
			} else {
				natl::construct(&packing, other.packing);
			}
		}
		constexpr PackedPtrAndSmallData(PackedPtrAndSmallData&& other) noexcept {
			if (isConstantEvaluated()) {
				natl::construct(&constexprPackingPtr, natl::move(other.constexprPackingPtr));
				other.constexprPackingPtr = nullptr;
			} else {
				natl::construct(&packing, other.packing);
				other.packing = PackingType{};
			}
		}
		constexpr PackedPtrAndSmallData(PtrDataType* ptr, SmallDataType smallData) noexcept {
			if (isConstantEvaluated()) {
				natl::construct(&constexprPackingPtr, new ConstexprPackingType(smallData, ptr));
			} else {
				natl::construct(&packing, PackingType(smallData, ptr));
			}
		}

		//destructor 
	private:
		constexpr void destruct() noexcept {
			if (isConstantEvaluated()) {
				delete constexprPackingPtr;
			}
		}
	public:
		constexpr ~PackedPtrAndSmallData() noexcept {
			destruct();
		}

		//util 
		constexpr PackedPtrAndSmallData& self() noexcept { return *this; }
		constexpr const PackedPtrAndSmallData& self() const noexcept { return *this; }

		//assignment 
		constexpr PackedPtrAndSmallData& operator=(const PackedPtrAndSmallData& other) noexcept {
			if (isConstantEvaluated()) {
				constexprPackingPtr = new ConstexprPackingType(*other.constexprPackingPtr);
			} else {
				packing = other.packing;
			}
			return self();

		}
		constexpr PackedPtrAndSmallData& operator=(PackedPtrAndSmallData&& other) noexcept {
			if (isConstantEvaluated()) {
				constexprPackingPtr = natl::move(other.constexprPackingPtr);
				other.constexprPackingPtr = nullptr;
			} else {
				natl::construct<PackingType, PackingType>(packing, other.packing);
				other.packing = PackingType{};
			}
			return self();

		}
		constexpr PackedPtrAndSmallData& operator=(const SmallDataType smallData) noexcept { 
			setSmallData(smallData); 
			return self(); 
		}
		constexpr PackedPtrAndSmallData& operator=(PtrDataType* const ptr) noexcept {
			setPtr(ptr);
			return self();
		}


		//element access 
		constexpr PtrDataType* getPtr() noexcept { 
			if (isConstantEvaluated()) {
				return constexprPackingPtr->ptr;
			} else {
				return packing.getPtr();
			}
		}
		constexpr const PtrDataType* getPtr() const noexcept {
			if (isConstantEvaluated()) {
				return constexprPackingPtr->ptr;
			} else {
				return packing.getPtr();
			}
		}

		constexpr SmallDataType getSmallData() const noexcept {
			if (isConstantEvaluated()) {
				return constexprPackingPtr->smallDataStorage;
			} else {
				return packing.getSmallData();
			}
		}

		//modifiers
		constexpr void setPtr(PtrDataType* const ptr) noexcept {
			if (isConstantEvaluated()) {
				constexprPackingPtr->ptr = ptr;
			} else {
				packing.setPtr(ptr);
			}
		}
		constexpr void setSmallData(const SmallDataType smallData) noexcept {
			if (isConstantEvaluated()) {
				constexprPackingPtr->smallDataStorage = smallData;
			} else {
				packing.setSmallData(smallData);
			}
		}
		
		constexpr void setValues(PtrDataType* ptr, const SmallDataType smallData) noexcept {
			if (isConstantEvaluated()) {
				constexprPackingPtr->ptr = ptr;
				constexprPackingPtr->smallDataStorage = smallData;
			} else {
				packing.setPtr(ptr);
				packing.setSmallData(smallData);
			}
		}

		constexpr void setAsNull() noexcept {
			if (isConstantEvaluated()) {
				constexprPackingPtr->ptr = nullptr;
				constexprPackingPtr->smallDataStorage = SmallDataType();
			} else {
				packing.setPtr(nullptr);
				packing.setSmallData(SmallDataType());
			}
		}
	};

	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyCompareableV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};

	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyRelocatableV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};
	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyConstructibleV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};
	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyDestructibleV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};

	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyConstRefConstructibleV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};
	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyMoveConstructibleV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};

	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyConstRefAssignableV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};
	template<typename PtrDataType, typename SmallDataType>
	struct IsTriviallyMoveAssignableV<PackedPtrAndSmallData<PtrDataType, SmallDataType>>
		: TrueType {};

#ifdef NATL_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4265)
#endif // NATL_COMPILER_MSVC


	namespace impl {
		enum struct SharedPtrControlBlockState : ui16 {
			seperate = 0, 
			fused, 
			seperatePolymorphic,
			fusedPolymorphic,
			seperatePolymorphicConstexpr,
		};

		struct SharedPtrControlBlockSeperatePolymorphic {
			using control_block_polymorphic_destroy_function = void(*)(SharedPtrControlBlockSeperatePolymorphic*);

			 virtual ~SharedPtrControlBlockSeperatePolymorphic() noexcept = default;
			 virtual control_block_polymorphic_destroy_function getDestoryFunction() const noexcept = 0;
			 virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			 virtual void refIncrement(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
			 virtual void weakRefIncrement(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
			 virtual Size getUseCount(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
			 virtual Bool incrementIfNotZero(SharedPtrControlBlockSeperatePolymorphic*) const noexcept = 0;
		};

		template<typename DataType>
		struct SharedPtrControlBlockSeperate : SharedPtrControlBlockSeperatePolymorphic {
			using control_block_polymorphic_destroy_function = typename SharedPtrControlBlockSeperatePolymorphic::control_block_polymorphic_destroy_function;
			using control_block_deleter_function_type = Function<void(SharedPtrControlBlockSeperate*)>;

			DataType* dataPtr;
			Atomic<Size> useCount;
			Atomic<Size> weakCount;
			Function<void(DataType*)> deleter;
			Function<void(SharedPtrControlBlockSeperate*)> controlBlockDeleter;

			
			template<typename Deleter, typename ControlBlockDeleter>
				requires(IsDeleter<Deleter, DataType> && IsDeleter<ControlBlockDeleter, SharedPtrControlBlockSeperate>)
			 SharedPtrControlBlockSeperate(DataType* dataPtrIn, Deleter&& deleterIn, ControlBlockDeleter&& controlBlockDeleterIn) noexcept
				: dataPtr(dataPtrIn), useCount(1), weakCount(), deleter(deleterIn), controlBlockDeleter(controlBlockDeleterIn) {}

			 ~SharedPtrControlBlockSeperate() noexcept override = default;

			static inline void destory(SharedPtrControlBlockSeperate* controlBlockSeperate) noexcept {
				if (controlBlockSeperate->useCount.load() == 1) {
					controlBlockSeperate->deleter.invoke(controlBlockSeperate->dataPtr);

					controlBlockSeperate->useCount--;
					if (controlBlockSeperate->weakCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockSeperate->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockSeperate);
					}
				}
			}
			static inline void destoryWeak(SharedPtrControlBlockSeperate* controlBlockSeperate) noexcept {
				if (controlBlockSeperate->weakCount.load() == 1) {
					if (controlBlockSeperate->useCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockSeperate->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockSeperate);
						return;
					}
				}
				controlBlockSeperate->weakCount--;
			}
			static inline Bool incrementIfNotZero(SharedPtrControlBlockSeperate* controlBlockSeperate) {
				Size previous = controlBlockSeperate->useCount.load();
				for (;;) {
					if (previous == 0) {
						return false;
					}
					if (controlBlockSeperate->useCount.compare_exchange_weak(previous, previous + 1)) {
						return true;
					}
				}
			}

			control_block_polymorphic_destroy_function getDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					destory(controlBlockSeperate);
				};
			}
			control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) -> void {
					SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
					destoryWeak(controlBlockSeperate);
				};
			}
			void refIncrement(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 controlBlockSeperate->useCount++;
			}
			void weakRefIncrement(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 controlBlockSeperate->weakCount++;
			}
			Size getUseCount(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 return controlBlockSeperate->useCount.load();
			}
			Bool incrementIfNotZero(SharedPtrControlBlockSeperatePolymorphic* controlBlockSeperatePolymorphicPtr) const noexcept override {
				 SharedPtrControlBlockSeperate* controlBlockSeperate = static_cast<SharedPtrControlBlockSeperate*>(controlBlockSeperatePolymorphicPtr);
				 return incrementIfNotZero(controlBlockSeperate);
			}
		};

		struct SharedPtrControlBlockFusedPolymorphic {
			using control_block_polymorphic_destroy_function = void(*)(SharedPtrControlBlockFusedPolymorphic*);

			 virtual ~SharedPtrControlBlockFusedPolymorphic() noexcept = default;
			 virtual control_block_polymorphic_destroy_function getDestoryFunction() const noexcept = 0;
			 virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			 virtual void refIncrement(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
			 virtual void weakRefIncrement(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
			 virtual Size getUseCount(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
			 virtual Bool incrementIfNotZero(SharedPtrControlBlockFusedPolymorphic*) const noexcept = 0;
		};

		template<typename DataType>
		struct SharedPtrControlBlockFused : SharedPtrControlBlockFusedPolymorphic {
			using control_block_polymorphic_destroy_function = typename SharedPtrControlBlockFusedPolymorphic::control_block_polymorphic_destroy_function;
			using control_block_deleter_function_type = Function<void(SharedPtrControlBlockFused*)>;

			using pre_delete_function_type = Function<void(DataType*)>;
			using post_delete_function_type = Function<void(void)>;

			Atomic<Size> useCount;
			Atomic<Size> weakCount;
			Function<void(DataType*)> preDeleteFunction;
			Function<void(void)> postDeleteFunction;
			Function<void(SharedPtrControlBlockFused*)> controlBlockDeleter;

			union {
				DataType data;
			};

			template<typename PreDeleteFunctor, typename PostDeleteFunctor, typename ControlBlockDeleter, typename... ConstructArgTypes>
				requires(HasFunctionSignature<PreDeleteFunctor, void, DataType*> &&
						HasFunctionSignature<PostDeleteFunctor, void> && 
						IsDeleter<ControlBlockDeleter, SharedPtrControlBlockFused> &&
						IsConstructibleC<DataType, ConstructArgTypes...>)
			SharedPtrControlBlockFused(PreDeleteFunctor&& preDeleteFunctorIn, 
				PostDeleteFunctor&& postDeleteFunctorIn, 
				ControlBlockDeleter&& controlBlockDeleterIn, 
				ConstructArgTypes... constructArg) noexcept : 
					useCount(1), weakCount(0),
					preDeleteFunction(forward<PreDeleteFunctor>(preDeleteFunctorIn)),
					postDeleteFunction(forward<PostDeleteFunctor>(postDeleteFunctorIn)),
					controlBlockDeleter(forward<ControlBlockDeleter>(controlBlockDeleterIn)),  
					data(forward<ConstructArgTypes>(constructArg)...) {}

			 ~SharedPtrControlBlockFused() noexcept override = default;

			static inline void destory(SharedPtrControlBlockFused* controlBlockFused) noexcept {
				if (controlBlockFused->useCount.load() == 1) {
					if (controlBlockFused->preDeleteFunction.isNotEmpty()) {
						controlBlockFused->preDeleteFunction.invoke(&controlBlockFused->data);
					}

					natl::deconstruct<DataType>(&controlBlockFused->data);

					if (controlBlockFused->postDeleteFunction.isNotEmpty()) {
						controlBlockFused->postDeleteFunction.invoke();
					}

					controlBlockFused->useCount--;
					if (controlBlockFused->weakCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockFused->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockFused);
					}
				}
			}
			static inline void destoryWeak(SharedPtrControlBlockFused* controlBlockFused) noexcept {
				if (controlBlockFused->weakCount.load() == 1) {
					if (controlBlockFused->useCount.load() == 0) {
						control_block_deleter_function_type controlBlockDeleter = natl::move(controlBlockFused->controlBlockDeleter);
						controlBlockDeleter.invoke(controlBlockFused);
						return;
					}
				}
				controlBlockFused->weakCount--;
			}
			static inline Bool incrementIfNotZero(SharedPtrControlBlockFused* controlBlockFused) noexcept {
				Size previous = controlBlockFused->useCount.load();
				for (;;) {
					if (previous == 0) {
						return false;
					}
					if (controlBlockFused->useCount.compare_exchange_weak(previous, previous + 1)) {
						return true;
					}
				}
			}

			control_block_polymorphic_destroy_function getDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					destory(controlBlockFused);
				};
			}
			control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) -> void {
					SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
					destoryWeak(controlBlockFused);
				};
			}
			void refIncrement(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				controlBlockFused->useCount++;
			}
			void weakRefIncrement(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				controlBlockFused->weakCount++;
			}
			Size getUseCount(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				return controlBlockFused->useCount.load();
			}
			Bool incrementIfNotZero(SharedPtrControlBlockFusedPolymorphic* controlBlockFusedPolymorphicPtr) const noexcept override {
				SharedPtrControlBlockFused* controlBlockFused = static_cast<SharedPtrControlBlockFused*>(controlBlockFusedPolymorphicPtr);
				return incrementIfNotZero(controlBlockFused);
			}
		};

		struct SharedPtrControlBlockSeperatePolymorphicConstexpr {
			using control_block_polymorphic_destroy_function = Bool(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);
			using control_block_polymorphic_increment_function = void(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);
			using control_block_polymorphic_get_count_function = Size(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);
			using control_block_polymorphic_increment_if_not_zero_function = Bool(*)(SharedPtrControlBlockSeperatePolymorphicConstexpr*);

			[[nodiscard]] constexpr virtual control_block_polymorphic_destroy_function getDestroyFunction() const noexcept = 0;
			[[nodiscard]] constexpr virtual control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_increment_function getRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_increment_function getWeakRefIncrementFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_get_count_function getUseCountFunction() const noexcept = 0;
			constexpr virtual control_block_polymorphic_increment_if_not_zero_function getIncrementIfNotZeroFunction() const noexcept = 0;
		};
		
		template<typename DataType> 
		struct SharedPtrControlBlockSeperateConstexpr final : SharedPtrControlBlockSeperatePolymorphicConstexpr {
			using control_block_polymorphic_destroy_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_destroy_function;
			using control_block_polymorphic_increment_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_increment_function;
			using control_block_polymorphic_get_count_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_get_count_function;
			using control_block_polymorphic_increment_if_not_zero_function = SharedPtrControlBlockSeperateConstexpr::control_block_polymorphic_increment_if_not_zero_function;
			
			using pre_delete_function_type = Function<void(DataType*)>;
			using post_delete_function_type = Function<void(void)>;
			using deleter_function_type = Function<void(DataType*)>;
			using control_block_deleter_function_type = Function<void(SharedPtrControlBlockSeperateConstexpr*)>;

			DataType* dataPtr;
			Size useCount;
			Size weakCount;
			pre_delete_function_type preDeleteFunction;
			post_delete_function_type postDeleteFunction;
			deleter_function_type deleter;
			control_block_deleter_function_type controlBlockDeleter;

			constexpr ~SharedPtrControlBlockSeperateConstexpr() noexcept = default;

			constexpr SharedPtrControlBlockSeperateConstexpr(
				DataType* ptr,
				pre_delete_function_type&& preDeleteFunctionIn,
				post_delete_function_type&& postDeleteFunctionIn,
				deleter_function_type&& deleterFunctionIn,
				control_block_deleter_function_type&& controlBlockDeleterIn) noexcept :
				dataPtr(ptr),
				useCount(1),
				weakCount(0),
				preDeleteFunction(natl::move(preDeleteFunctionIn)),
				postDeleteFunction(natl::move(postDeleteFunctionIn)),
				deleter(natl::move(deleterFunctionIn)),
				controlBlockDeleter(natl::move(controlBlockDeleterIn)) {};

			[[nodiscard]] constexpr control_block_polymorphic_destroy_function getDestroyFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> Bool {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					if (controlBlock->useCount == 1) {
						if (controlBlock->preDeleteFunction.isNotEmpty()) {
							controlBlock->preDeleteFunction.invoke(controlBlock->dataPtr);
						}

						controlBlock->deleter.invoke(controlBlock->dataPtr);

						if (controlBlock->postDeleteFunction.isNotEmpty()) {
							controlBlock->postDeleteFunction.invoke();
						}

						controlBlock->useCount--;
						if (controlBlock->weakCount == 0) {
							control_block_deleter_function_type controlBlockDeleterFunc = natl::move(controlBlock->controlBlockDeleter);
							controlBlockDeleterFunc.invoke(controlBlock);
							return true;
						}
					}
					return false;
					};
			}
			[[nodiscard]] constexpr control_block_polymorphic_destroy_function getWeakDestoryFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> Bool {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					if (controlBlock->weakCount == 1) {
						if (controlBlock->weakCount == 0) {
							control_block_deleter_function_type controlBlockDeleterFunc = natl::move(controlBlock->controlBlockDeleter);
							controlBlockDeleterFunc.invoke(controlBlock);
							return true;
						}
					}
					controlBlock->weakCount--;
					return false;
				};
			}
			constexpr control_block_polymorphic_increment_function getRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> void {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					controlBlock->useCount++;
					};
			}
			constexpr control_block_polymorphic_increment_function getWeakRefIncrementFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> void {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					controlBlock->weakCount++;
					};
			}
			constexpr control_block_polymorphic_get_count_function getUseCountFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> Size {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					return controlBlock->useCount;
					};
			}
			constexpr control_block_polymorphic_increment_if_not_zero_function getIncrementIfNotZeroFunction() const noexcept override {
				return [](SharedPtrControlBlockSeperatePolymorphicConstexpr* controlBlockPolymorpic) -> Bool {
					auto controlBlock = static_cast<SharedPtrControlBlockSeperateConstexpr*>(controlBlockPolymorpic);
					if (controlBlock->useCount == 0) {
						return false;
					} else {
						controlBlock->useCount++;
						return true;
					}
				};
			}
		};
	}

#ifdef NATL_COMPILER_MSVC
#pragma warning(pop)
#endif // NATL_COMPILER_MSVC

	struct SharedPtrFusedConstruct {};

	template<typename DataType>
	struct SharedPtr;

	//weak ptr 
	template<typename DataType>
	struct WeakPtr {
	public:
		using element_type = DataType;
		using element_pointer = DataType*;

		using control_block_seperate = impl::SharedPtrControlBlockSeperate<DataType>;
		using control_block_seperate_polymorphic = impl::SharedPtrControlBlockSeperatePolymorphic;
		using control_block_fused = impl::SharedPtrControlBlockFused<DataType>;
		using control_block_fused_polymorphic = impl::SharedPtrControlBlockFusedPolymorphic;
		using control_block_seperate_polymorphic_constexpr = impl::SharedPtrControlBlockSeperatePolymorphicConstexpr;
	private:
		using weak_ptr_pointer_and_control_block_state = PackedPtrAndSmallData<DataType, impl::SharedPtrControlBlockState>;
		weak_ptr_pointer_and_control_block_state dataPtrAndControlBlockState;
		union {
			control_block_seperate* controlBlockSeperate;
			control_block_fused* controlBlockFused;
			control_block_seperate_polymorphic* controlBlockSeperatePolymorphic;
			control_block_fused_polymorphic* controlBlockFusedPolymorphic;
			control_block_seperate_polymorphic_constexpr* controlBlockSeperatePolymorphicConstexpr;
		};
	public:
		//constructor 
		constexpr WeakPtr() noexcept : dataPtrAndControlBlockState(nullptr, impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}
		constexpr WeakPtr(NullptrType) noexcept : dataPtrAndControlBlockState(nullptr, impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}

	private:
		template<typename OtherType>
		constexpr void constructCopy(const OtherType& other) noexcept {
			if (other.dataPtr == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				dataPtrAndControlBlockState = other.dataPtrAndControlBlockState;

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockSeperate = other.controlBlockSeperate;
					controlBlockSeperate->weakCount++;
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockFused = other.controlBlockFused;
					controlBlockFused->weakCount++;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					controlBlockSeperatePolymorphic->weakRefIncrement(controlBlockSeperatePolymorphic);
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					controlBlockFusedPolymorphic->weakRefIncrement(controlBlockFusedPolymorphic);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						controlBlockSeperatePolymorphicConstexpr->getWeakRefIncrementFunction()(controlBlockSeperatePolymorphicConstexpr);
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
		}
		template<typename OtherType>
		constexpr void constructMove(OtherType&& other) noexcept {
			if (other.dataPtrAndControlBlockState.getPtr() == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				dataPtrAndControlBlockState = other.dataPtrAndControlBlockState;

				switch (other.dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockSeperate = other.controlBlockSeperate;
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockFused = other.controlBlockFused;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}

			other.dataPtr = nullptr;
		}
		template<typename OtherType>
		constexpr void constructPolymorphicCopy(const OtherType& other) noexcept {
			if (other.dataPtrAndControlBlockState.getSmallData() == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				dataPtrAndControlBlockState.setPtr(static_cast<DataType*>(dataPtrAndControlBlockState.getPtr()));

				switch (other.dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
					controlBlockSeperatePolymorphic->weakRefIncrement(controlBlockSeperatePolymorphic);
					break;
				case impl::SharedPtrControlBlockState::fused:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
					controlBlockFusedPolymorphic->weakRefIncrement(controlBlockFusedPolymorphic);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					controlBlockSeperatePolymorphic->weakRefIncrement(controlBlockSeperatePolymorphic);
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					controlBlockFusedPolymorphic->weakRefIncrement(controlBlockFusedPolymorphic);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr);
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						controlBlockSeperatePolymorphicConstexpr->getWeakRefIncrementFunction()(controlBlockSeperatePolymorphicConstexpr);
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
		}
		template<typename OtherType>
		constexpr void constructPolymorphicMove(const OtherType& other) noexcept {
			if (other.dataPtrAndControlBlockState.getPtr() == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				dataPtrAndControlBlockState.setPtr(static_cast<DataType*>(other.dataPtrAndControlBlockState.getPtr()));

				switch (other.dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
					break;
				case impl::SharedPtrControlBlockState::fused:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr);
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}

			other.dataPtr = nullptr;
		}
	public:

		constexpr WeakPtr(const WeakPtr& other) noexcept {
			constructCopy<WeakPtr>(other);
		}
		constexpr WeakPtr(WeakPtr&& other) noexcept {
			constructMove<WeakPtr>(forward<WeakPtr>(other));
		}

		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr WeakPtr(const WeakPtr<OtherDataType>& other) noexcept {
			constructPolymorphicCopy<WeakPtr<OtherDataType>>(other);
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr WeakPtr(WeakPtr<OtherDataType>&& other) noexcept {
			constructPolymorphicMove<WeakPtr<OtherDataType>>(forward<WeakPtr<OtherDataType>>(other));
		}

		constexpr WeakPtr(const SharedPtr<DataType>& other) noexcept {
			constructCopy<SharedPtr<DataType>>(other);
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr WeakPtr(const SharedPtr<DataType>& other) noexcept {
			constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
		}
	private:
		constexpr void destruct() noexcept {
			if (dataPtrAndControlBlockState.getPtr()) {
				switch (dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					control_block_seperate::destoryWeak(controlBlockSeperate);
					break;
				case impl::SharedPtrControlBlockState::fused:
					control_block_fused::destoryWeak(controlBlockFused);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic->getWeakDestoryFunction()(controlBlockSeperatePolymorphic);
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic->getWeakDestoryFunction()(controlBlockFusedPolymorphic);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr->getWeakDestoryFunction()(controlBlockSeperatePolymorphicConstexpr);
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
		}
	public:
		//destructor
		constexpr ~WeakPtr() noexcept {
			destruct();
		}

		//util 
		constexpr WeakPtr& self() noexcept { return *this; }
		constexpr const WeakPtr& self() const noexcept { return *this; }


		//assignment
		constexpr WeakPtr& operator=(const WeakPtr& other) noexcept {
			destruct();
			constructCopy<WeakPtr>(other);
			return self();
		}
		constexpr WeakPtr& operator=(WeakPtr&& other) noexcept {
			destruct();
			constructMove<WeakPtr>(forward<WeakPtr>(other));
			return self();
		}

		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr WeakPtr& operator=(const WeakPtr<OtherDataType>& other) noexcept {
			destruct();
			constructPolymorphicCopy<WeakPtr<OtherDataType>>(other);
			return self();
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr WeakPtr& operator=(WeakPtr<OtherDataType>&& other) noexcept {
			destruct();
			constructPolymorphicMove<WeakPtr<OtherDataType>>(forward<WeakPtr<OtherDataType>>(other));
			return self();
		}

		constexpr WeakPtr& operator=(const SharedPtr<DataType>& other) noexcept {
			destruct();
			constructCopy<SharedPtr<DataType>>(other);
			return self();
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr WeakPtr& operator=(const SharedPtr<DataType>& other) noexcept {
			destruct();
			constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
			return self();
		}

		//modifiers
		constexpr void reset() noexcept {
			destruct();
			dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
			controlBlockSeperate = nullptr;
		}

		constexpr void swap(WeakPtr& other) noexcept {
			weak_ptr_pointer_and_control_block_state dataPtrAndControlBlockStateTemp = other.dataPtrAndControlBlockState;
			other.dataPtrAndControlBlockState = dataPtrAndControlBlockState;
			dataPtrAndControlBlockState = dataPtrAndControlBlockStateTemp;

			switch (dataPtrAndControlBlockState.getSmallData()) {
			case impl::SharedPtrControlBlockState::seperate: {
				control_block_seperate* controlBlockSeperateTemp = other.controlBlockSeperate;
				other.controlBlockSeperate = controlBlockSeperate;
				controlBlockSeperate = controlBlockSeperateTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::fused: {
				control_block_fused* controlBlockFusedTemp = other.controlBlockFused;
				other.controlBlockFused = controlBlockFused;
				controlBlockFused = controlBlockFusedTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::seperatePolymorphic: {
				control_block_seperate_polymorphic* controlBlockSeperatePolymorphicTemp = other.controlBlockSeperatePolymorphic;
				other.controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphic;
				controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphicTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::fusedPolymorphic: {
				control_block_fused_polymorphic* controlBlockFusedPolymorphicTemp = other.controlBlockFusedPolymorphic;
				other.controlBlockFusedPolymorphic = controlBlockFusedPolymorphic;
				controlBlockFusedPolymorphic = controlBlockFusedPolymorphicTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr: {
				if (isConstantEvaluated()) {
					control_block_seperate_polymorphic_constexpr* controlBlockSeperatePolymorphicConstexprTemp = other.controlBlockSeperatePolymorphicConstexpr;
					other.controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexpr;
					controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexprTemp;
				} else {
					unreachable();
				}
				break;
			}
			default:
				unreachable();
			}
		}

		//observers
		constexpr Size use_count() const noexcept {
			if (dataPtrAndControlBlockState.getPtr()) {
				switch (dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					return controlBlockSeperate->useCount.load();
				case impl::SharedPtrControlBlockState::fused:
					return controlBlockFused->useCount.load();
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					return controlBlockSeperatePolymorphic->getUseCount(controlBlockSeperatePolymorphic);
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					return controlBlockFusedPolymorphic->getUseCount(controlBlockFusedPolymorphic);
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						return controlBlockSeperatePolymorphicConstexpr->getUseCountFunction()(controlBlockSeperatePolymorphicConstexpr);
					} else {
						unreachable();
					}
				default:
					unreachable();
				}
			}
			return 0;
		}

		constexpr Bool expired() const noexcept {
			return use_count() == 0;
		}
		constexpr SharedPtr<DataType> lock() const noexcept {
			if (isConstantEvaluated()) {
				return expired() ? natl::move(SharedPtr<DataType>()) : natl::move(SharedPtr<DataType>(self()));
			} else {
				return expired() ? SharedPtr<DataType>() : SharedPtr<DataType>(self());
			}
		}

		constexpr Bool empty() const noexcept { return dataPtrAndControlBlockState.getPtr() == nullptr; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }
		explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

		friend SharedPtr<DataType>;
	};

	template<typename DataType>
	struct IsTriviallyCompareableV<WeakPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct IsTriviallyRelocatableV<WeakPtr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyConstructibleV<WeakPtr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyDestructibleV<WeakPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct IsTriviallyConstRefConstructibleV<WeakPtr<DataType>>
		: FalseType {};
	template<typename DataType>
	struct IsTriviallyMoveConstructibleV<WeakPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct IsTriviallyConstRefAssignableV<WeakPtr<DataType>>
		: FalseType {};
	template<typename DataType>
	struct IsTriviallyMoveAssignableV<WeakPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct SharedPtr {
	public:
		using element_type = DataType;
		using element_pointer = DataType*;

		using control_block_seperate = impl::SharedPtrControlBlockSeperate<DataType>;
		using control_block_seperate_polymorphic = impl::SharedPtrControlBlockSeperatePolymorphic;
		using control_block_fused = impl::SharedPtrControlBlockFused<DataType>;
		using control_block_fused_polymorphic = impl::SharedPtrControlBlockFusedPolymorphic;
		using control_block_seperate_polymorphic_constexpr = impl::SharedPtrControlBlockSeperatePolymorphicConstexpr;
		using pointer_and_control_block_state = PackedPtrAndSmallData<element_type, impl::SharedPtrControlBlockState>;
	private:
		pointer_and_control_block_state dataPtrAndControlBlockState;
		union {
			control_block_seperate* controlBlockSeperate;
			control_block_fused* controlBlockFused;
			control_block_seperate_polymorphic* controlBlockSeperatePolymorphic;
			control_block_fused_polymorphic* controlBlockFusedPolymorphic;
			control_block_seperate_polymorphic_constexpr* controlBlockSeperatePolymorphicConstexpr;
		};
	public:
		using weak_type = WeakPtr<DataType>;
	private:
		template<typename OtherType, Bool IncrementRefCount = true>
		constexpr void constructCopy(const OtherType& other) noexcept {
			if (other.dataPtrAndControlBlockState.getPtr() == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				dataPtrAndControlBlockState = other.dataPtrAndControlBlockState;

				switch (other.dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockSeperate = other.controlBlockSeperate;
					if constexpr (IncrementRefCount) {
						controlBlockSeperate->useCount++;
					}
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockFused = other.controlBlockFused;
					if constexpr (IncrementRefCount) {
						controlBlockFused->useCount++;
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockSeperatePolymorphic->refIncrement(controlBlockSeperatePolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockFusedPolymorphic->refIncrement(controlBlockFusedPolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						if constexpr (IncrementRefCount) {
							controlBlockSeperatePolymorphicConstexpr->getRefIncrementFunction()(controlBlockSeperatePolymorphicConstexpr);
						}
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
		}
		template<typename OtherType>
		constexpr void constructMove(OtherType&& other) noexcept {
			if (other.dataPtrAndControlBlockState.getPtr() == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				dataPtrAndControlBlockState = other.dataPtrAndControlBlockState;

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					controlBlockSeperate = other.controlBlockSeperate;
					break;
				case impl::SharedPtrControlBlockState::fused:
					controlBlockFused = other.controlBlockFused;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}

			other.dataPtr = nullptr;
		}
		template<typename OtherType, Bool IncrementRefCount = true>
		constexpr void constructPolymorphicCopy(const OtherType& other) noexcept {
			if (other.dataPtrAndControlBlockState.getPtr() == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				other.dataPtrAndControlBlockState.setPtr(static_cast<DataType*>(other.dataPtrAndControlBlockState.getPtr()));

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					other.dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
					if constexpr (IncrementRefCount) {
						controlBlockSeperatePolymorphic->refIncrement(controlBlockSeperatePolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::fused:
					other.dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
					if constexpr (IncrementRefCount) {
						controlBlockFusedPolymorphic->refIncrement(controlBlockFusedPolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					other.dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockSeperatePolymorphic->refIncrement(controlBlockSeperatePolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					other.dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					if constexpr (IncrementRefCount) {
						controlBlockFusedPolymorphic->refIncrement(controlBlockFusedPolymorphic);
					}
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						other.dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr);
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
						if constexpr (IncrementRefCount) {
							controlBlockSeperatePolymorphicConstexpr->getRefIncrementFunction()(controlBlockSeperatePolymorphicConstexpr);
						}
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
		}
		template<typename OtherType>
		constexpr void constructPolymorphicMove(const OtherType& other) noexcept {
			if (other.dataPtrAndControlBlockState == nullptr) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				dataPtrAndControlBlockState.setPtr(static_cast<DataType*>(other.dataPtrAndControlBlockState.getPtr()));

				switch (other.controlBlockState) {
				case impl::SharedPtrControlBlockState::seperate:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(other.controlBlockSeperate);
					break;
				case impl::SharedPtrControlBlockState::fused:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = static_cast<control_block_fused_polymorphic*>(other.controlBlockFused);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphic);
					controlBlockSeperatePolymorphic = other.controlBlockSeperatePolymorphic;
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::fusedPolymorphic);
					controlBlockFusedPolymorphic = other.controlBlockFusedPolymorphic;
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						dataPtrAndControlBlockState.setSmallData(impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr);
						controlBlockSeperatePolymorphicConstexpr = other.controlBlockSeperatePolymorphicConstexpr;
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}

			other.dataPtr = nullptr;
		}

	public:
		//constructor
		constexpr SharedPtr() noexcept : dataPtrAndControlBlockState(nullptr, impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}
		constexpr SharedPtr(NullptrType) noexcept : dataPtrAndControlBlockState(nullptr, impl::SharedPtrControlBlockState::seperate), controlBlockSeperate() {}

		constexpr SharedPtr(const SharedPtr& other) noexcept {
			constructCopy<SharedPtr>(other);
		}
		constexpr SharedPtr(SharedPtr&& other) noexcept {
			constructCopy<SharedPtr>(forward<SharedPtr>(other));
		}

		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(const SharedPtr<OtherDataType>& other) noexcept {
			constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(SharedPtr<OtherDataType>&& other) noexcept {
			constructPolymorphicMove<SharedPtr<OtherDataType>>(forward<SharedPtr<OtherDataType>>(other));
		}

		constexpr SharedPtr(const WeakPtr<DataType>& other) noexcept {
			if (other.isNotEmpty() && incrementIfNotZero<WeakPtr<DataType>>(other)) {
				constructCopy<WeakPtr<DataType>, false>(other);
			} else {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			}
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(const WeakPtr<OtherDataType>& other) noexcept {
			if (other.isNotEmpty() && incrementIfNotZero<WeakPtr<DataType>>(other)) {
				constructPolymorphicCopy<WeakPtr<OtherDataType>, false>(other);
			} else {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			}
		}



	private:
		template<typename OtherType>
		constexpr Bool incrementIfNotZero(const OtherType& other) noexcept {
			switch (dataPtrAndControlBlockState.getSmallData()) {
			case impl::SharedPtrControlBlockState::seperate:
				return control_block_seperate::incrementIfNotZero(other.controlBlockSeperate);
			case impl::SharedPtrControlBlockState::fused:
				return control_block_fused::incrementIfNotZero(other.controlBlockFused);
			case impl::SharedPtrControlBlockState::seperatePolymorphic:
				return other.controlBlockSeperatePolymorphic->incrementIfNotZero(other.controlBlockSeperatePolymorphic);
			case impl::SharedPtrControlBlockState::fusedPolymorphic:
				return other.controlBlockFusedPolymorphic->incrementIfNotZero(other.controlBlockFusedPolymorphic);
			case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
				if (isConstantEvaluated()) {
					return other.controlBlockSeperatePolymorphicConstexpr->getIncrementIfNotZeroFunction()(other.controlBlockSeperatePolymorphicConstexpr);
				} else {
					unreachable();
				}
				break;
			default:
				unreachable();
			}
		}

		template<typename LocalDataType, typename Alloc>
			requires(IsAllocator<Alloc>)
		constexpr void constructContexpr(
			DataType* ptr, 
			typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::pre_delete_function_type&& preDeleteFunction,
			typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::post_delete_function_type&& postDeleteFunction,
			typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::deleter_function_type&& deleterFunction) noexcept {
			using SharedPtrControlBlockSeperateConstexprType = impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>;
			dataPtrAndControlBlockState.setValues(ptr, impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr);
			
			using control_block_seperate_data_constexpr_alloc = Alloc::template rebind_alloc<SharedPtrControlBlockSeperateConstexprType>;
			typename SharedPtrControlBlockSeperateConstexprType::control_block_deleter_function_type controlBlockDeleter =
				[](SharedPtrControlBlockSeperateConstexprType* controlBlockPtr) {
				natl::deconstruct(controlBlockPtr);
				control_block_seperate_data_constexpr_alloc::deallocate(controlBlockPtr, 1);
			};

			SharedPtrControlBlockSeperateConstexprType* controlBlock = control_block_seperate_data_constexpr_alloc::allocate(1);
			natl::construct(controlBlock, 
				ptr,
				natl::move(preDeleteFunction), 
				natl::move(postDeleteFunction), 
				natl::move(deleterFunction), 
				natl::move(controlBlockDeleter));

			controlBlockSeperatePolymorphicConstexpr = static_cast<impl::SharedPtrControlBlockSeperatePolymorphicConstexpr*>(controlBlock);
		}

		template<typename LocalDataType>
		constexpr typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::pre_delete_function_type constructConstexprNullPreDelete() noexcept {
			return typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::pre_delete_function_type();
		}

		template<typename LocalDataType>
		constexpr typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::post_delete_function_type constructConstexprNullPostDelete() noexcept {
			return typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::post_delete_function_type();
		}
		template<typename LocalDataType, typename Alloc>
			requires(IsAllocator<Alloc>)
		constexpr typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::deleter_function_type constructConstexprStandardDeleter() noexcept {
			return 
				typename impl::SharedPtrControlBlockSeperateConstexpr<LocalDataType>::deleter_function_type(
					[](LocalDataType* ptr) {
						using LocalDataTypeAlloc = typename Alloc::template rebind_alloc<LocalDataType>;
						LocalDataTypeAlloc::deallocate(ptr, 1);
					}
				);
		}


		template<typename Alloc>
			requires(IsAllocator<Alloc>)
		constexpr void construct(DataType* ptr) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<DataType, Alloc>(ptr,
					constructConstexprNullPreDelete<DataType>(),
					constructConstexprNullPostDelete<DataType>(),
					constructConstexprStandardDeleter<DataType, Alloc>());
				return;
			}

			dataPtrAndControlBlockState.setValues(ptr, impl::SharedPtrControlBlockState::seperate);
			using control_block_seperate_alloc = Alloc::template rebind_alloc<control_block_seperate>;
			controlBlockSeperate = control_block_seperate_alloc::allocate(1);
			natl::construct<control_block_seperate>(controlBlockSeperate, ptr, DefaultDeleter<DataType, Alloc>(), DefaultDeleter<control_block_seperate, control_block_seperate_alloc>());
		}

		template<typename Alloc, typename Deleter>
			requires(IsAllocator<Alloc>&& IsDeleter<Deleter, DataType>)
		constexpr void construct(DataType* ptr, Deleter&& deleter) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<DataType, Alloc>(ptr,
					constructConstexprNullPreDelete<DataType>(),
					constructConstexprNullPostDelete<DataType>(),
					natl::move(deleter));
				return;
			}

			dataPtrAndControlBlockState.setValues(ptr, impl::SharedPtrControlBlockState::seperate);
			using control_block_seperate_alloc = Alloc::template rebind_alloc<control_block_seperate>;
			controlBlockSeperate = control_block_seperate_alloc::allocate(1);
			natl::construct<control_block_seperate>(controlBlockSeperate, ptr, deleter, DefaultDeleter<control_block_seperate, control_block_seperate_alloc>());
		}
		template<typename OtherDataType, typename Alloc>
			requires(IsAllocator<Alloc> && IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr void construct(OtherDataType* ptr) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<OtherDataType, Alloc>(ptr,
					constructConstexprNullPreDelete<OtherDataType>(),
					constructConstexprNullPostDelete<OtherDataType>(),
					constructConstexprStandardDeleter<OtherDataType, Alloc>());
				return;
			}


			OtherDataType* dataPtr = static_cast<OtherDataType*>(ptr);
			dataPtrAndControlBlockState.setValues(dataPtr, impl::SharedPtrControlBlockState::seperatePolymorphic);
			using other_control_block_type = impl::SharedPtrControlBlockSeperate<OtherDataType>;
			using other_control_block_type_alloc = Alloc::template rebind_alloc<other_control_block_type>;
			other_control_block_type* otherControlBlockSeperate = other_control_block_type_alloc::allocate(1);
			natl::construct<other_control_block_type>(otherControlBlockSeperate, dataPtr, DefaultDeleter<DataType, Alloc>(), DefaultDeleter<other_control_block_type, other_control_block_type_alloc>());
			controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(otherControlBlockSeperate);
		}

		template<typename OtherDataType, typename Alloc, typename Deleter>
			requires(IsAllocator<Alloc>&& IsDeleter<Deleter, DataType>)
		constexpr void construct(OtherDataType* ptr, Deleter&& deleter) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<OtherDataType, Alloc>(ptr,
					constructConstexprNullPreDelete<OtherDataType>(),
					constructConstexprNullPostDelete<OtherDataType>(),
					natl::move(deleter));
				return;
			}

			OtherDataType* dataPtr = static_cast<OtherDataType*>(ptr);
			dataPtrAndControlBlockState.setValues(dataPtr, impl::SharedPtrControlBlockState::seperatePolymorphic);
			using other_control_block_type = impl::SharedPtrControlBlockSeperate<OtherDataType>;
			using other_control_block_type_alloc = Alloc::template rebind_alloc<other_control_block_type>;
			other_control_block_type* otherControlBlockSeperate = other_control_block_type_alloc::allocate(1);
			natl::construct<other_control_block_type>(otherControlBlockSeperate, dataPtr, deleter, DefaultDeleter<other_control_block_type, other_control_block_type_alloc>());
			controlBlockSeperatePolymorphic = static_cast<control_block_seperate_polymorphic*>(otherControlBlockSeperate);
		}

		template<typename Deleter>
		constexpr void construct(UniquePtr<DataType, Deleter>&& other) {
			if (other.isEmpty()) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				if (IsEmpty<Deleter>) {
					Deleter deleter = Deleter();
					construct<DefaultAllocatorByte, Deleter>(other, move<Deleter>(deleter));
				} else {
					construct<DefaultAllocatorByte, Deleter>(other, move<Deleter>(other.get_deleter()));
				}
			}
		}
		template<typename OtherDataType, typename Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr void construct(UniquePtr<DataType, Deleter>&& other) {
			if (other.isEmpty()) {
				dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
				controlBlockSeperate = nullptr;
			} else {
				if (IsEmpty<Deleter>) {
					Deleter deleter = Deleter();
					construct<OtherDataType, DefaultAllocatorByte, Deleter>(other, move<Deleter>(deleter));
				} else {
					construct<OtherDataType, DefaultAllocatorByte, Deleter>(other, move<Deleter>(other.get_deleter()));
				}
			}
		}
	public:

		template<typename Alloc>
			requires(IsAllocator<Alloc>)
		explicit constexpr SharedPtr(DataType* ptr, Alloc) noexcept {
			construct<Alloc>(ptr);
		}

		template<typename Alloc, typename Deleter>
			requires(IsAllocator<Alloc>&& IsDeleter<Deleter, DataType>)
		constexpr SharedPtr(DataType* ptr, Deleter&& deleter, Alloc) noexcept {
			construct<DataType, Deleter>(ptr, forward<Deleter>(deleter));
		}

		template<typename Alloc, typename... ConstructArgTypes>
			requires(IsAllocator<Alloc>&& IsConstructibleC<DataType, ConstructArgTypes...>)
		constexpr SharedPtr(SharedPtrFusedConstruct, Alloc, ConstructArgTypes... constructArg) noexcept {
			if (isConstantEvaluated()) {
				constructContexpr<DataType, Alloc>(
					Alloc::template rebind_alloc<DataType>::allocate(1),
					constructConstexprNullPreDelete<DataType>(),
					constructConstexprNullPostDelete<DataType>(),
					constructConstexprStandardDeleter<DataType, Alloc>());
				return;
			}


			using control_block_fused_alloc = Alloc::template rebind_alloc<control_block_fused>;
			controlBlockFused = control_block_fused_alloc::allocate(1);

			natl::construct<control_block_fused>(controlBlockFused,
				typename control_block_fused::pre_delete_function_type(),
				typename control_block_fused::post_delete_function_type(),
				DefaultDeleter<control_block_fused, control_block_fused_alloc>(),
				natl::forward<ConstructArgTypes>(constructArg)...);

			dataPtrAndControlBlockState.setValues(&controlBlockFused->data, impl::SharedPtrControlBlockState::fused);
		}

		template<typename Deleter>
		constexpr SharedPtr(UniquePtr<DataType, Deleter>&& other) {
			construct<Deleter>(forward<UniquePtr<DataType, Deleter>>(other));
		}
		template<typename OtherDataType, typename Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr(UniquePtr<OtherDataType, Deleter>&& other) {
			construct<OtherDataType, Deleter>(forward<UniquePtr<OtherDataType, Deleter>>(other));
		}

	private:
		constexpr void destruct() noexcept {
			if (dataPtrAndControlBlockState.getPtr()) {
				switch (dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					control_block_seperate::destory(controlBlockSeperate);
					break;
				case impl::SharedPtrControlBlockState::fused:
					control_block_fused::destory(controlBlockFused);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					controlBlockSeperatePolymorphic->getDestoryFunction()(controlBlockSeperatePolymorphic);
					break;
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					controlBlockFusedPolymorphic->getDestoryFunction()(controlBlockFusedPolymorphic);
					break;
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						controlBlockSeperatePolymorphicConstexpr->getDestroyFunction()(controlBlockSeperatePolymorphicConstexpr);
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
		}
	public:

		//destructor
		constexpr ~SharedPtr() noexcept {
			destruct();
		}

		//util 
		constexpr SharedPtr& self() noexcept { return *this; }
		constexpr const SharedPtr& self() const noexcept { return *this; }

		//assignment 
		constexpr SharedPtr& operator=(const SharedPtr& other) noexcept {
			destruct();
			constructCopy<SharedPtr>(other);
			return self();
		}
		constexpr SharedPtr& operator=(SharedPtr&& other) noexcept {
			destruct();
			constructMove<SharedPtr>(other);
			return self();
		}

		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr& operator=(const SharedPtr<OtherDataType>& other) noexcept {
			destruct();
			constructPolymorphicCopy<SharedPtr<OtherDataType>>(other);
			return self();
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr& operator=(SharedPtr<OtherDataType>&& other) noexcept {
			destruct();
			constructPolymorphicMove<SharedPtr<OtherDataType>>(forward<SharedPtr<OtherDataType>>(other));
			return self();
		}

		template<typename Deleter>
		constexpr SharedPtr& operator=(UniquePtr<DataType, Deleter>&& other) {
			destruct();
			construct<Deleter>(forward<UniquePtr<DataType, Deleter>>(other));
			return self();
		}
		template<typename OtherDataType, typename Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr SharedPtr& operator=(UniquePtr<OtherDataType, Deleter>&& other) {
			destruct();
			construct<OtherDataType, Deleter>(forward<UniquePtr<OtherDataType, Deleter>>(other));
			return self();
		}

		//modifiers
		constexpr void reset() noexcept {
			destruct();
			dataPtrAndControlBlockState.setValues(nullptr, impl::SharedPtrControlBlockState::seperate);
			controlBlockSeperate = nullptr;
		}
		constexpr void reset(DataType* ptr) noexcept {
			destruct();
			construct<DefaultAllocatorByte>(ptr);
		}
		template<typename OtherDataType>
			requires(IsPolymorphicCastable<OtherDataType, DataType>)
		constexpr void reset(OtherDataType* ptr) noexcept {
			destruct();
			construct<OtherDataType, DefaultAllocatorByte>(ptr);
		}

		template<typename OtherDataType, typename Deleter>
			requires(IsPolymorphicCastable<OtherDataType, DataType> && IsDeleter<Deleter, OtherDataType>)
		constexpr void reset(OtherDataType* ptr, Deleter&& deleter) {
			destruct();
			construct<OtherDataType, Deleter, DefaultAllocatorByte>(ptr, forward<Deleter>(deleter));
		}

		constexpr void swap(SharedPtr& other) noexcept {
			pointer_and_control_block_state dataPtrAndControlBlockStateTemp = other.dataPtrAndControlBlockState;
			other.dataPtrAndControlBlockState = dataPtrAndControlBlockState;
			dataPtrAndControlBlockState = dataPtrAndControlBlockStateTemp;

			switch (dataPtrAndControlBlockState.getSmallData()) {
			case impl::SharedPtrControlBlockState::seperate: {
				control_block_seperate* controlBlockSeperateTemp = other.controlBlockSeperate;
				other.controlBlockSeperate = controlBlockSeperate;
				controlBlockSeperate = controlBlockSeperateTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::fused: {
				control_block_fused* controlBlockFusedTemp = other.controlBlockFused;
				other.controlBlockFused = controlBlockFused;
				controlBlockFused = controlBlockFusedTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::seperatePolymorphic: {
				control_block_seperate_polymorphic* controlBlockSeperatePolymorphicTemp = other.controlBlockSeperatePolymorphic;
				other.controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphic;
				controlBlockSeperatePolymorphic = controlBlockSeperatePolymorphicTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::fusedPolymorphic: {
				control_block_fused_polymorphic* controlBlockFusedPolymorphicTemp = other.controlBlockFusedPolymorphic;
				other.controlBlockFusedPolymorphic = controlBlockFusedPolymorphic;
				controlBlockFusedPolymorphic = controlBlockFusedPolymorphicTemp;
				break;
			}
			case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr: {
				if (isConstantEvaluated()) {
					control_block_seperate_polymorphic_constexpr* controlBlockSeperatePolymorphicConstexprTemp = other.controlBlockSeperatePolymorphicConstexpr;
					other.controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexpr;
					controlBlockSeperatePolymorphicConstexpr = controlBlockSeperatePolymorphicConstexprTemp;
				} else {
					unreachable();
				}
			}
			default:
				unreachable();
			}
		}

		//observers
		constexpr element_type* get() noexcept {
			return dataPtrAndControlBlockState.getPtr();
		}
		constexpr const element_type* get() const noexcept {
			return dataPtrAndControlBlockState.getPtr();
		}
		constexpr DataType& operator*() noexcept {
			return *get();
		}
		constexpr const DataType& operator*() const noexcept {
			return *get();
		}
		constexpr DataType* operator->() noexcept {
			return get();
		}
		constexpr const DataType* operator->() const noexcept {
			return get();
		}

		constexpr Size use_count() const noexcept {
			if (dataPtrAndControlBlockState.getPtr()) {
				switch (dataPtrAndControlBlockState.getSmallData()) {
				case impl::SharedPtrControlBlockState::seperate:
					return controlBlockSeperate->useCount.load();
				case impl::SharedPtrControlBlockState::fused:
					return controlBlockFused->useCount.load();
				case impl::SharedPtrControlBlockState::seperatePolymorphic:
					return controlBlockSeperatePolymorphic->getUseCount(controlBlockSeperatePolymorphic);
				case impl::SharedPtrControlBlockState::fusedPolymorphic:
					return controlBlockFusedPolymorphic->getUseCount(controlBlockFusedPolymorphic);
				case impl::SharedPtrControlBlockState::seperatePolymorphicConstexpr:
					if (isConstantEvaluated()) {
						return controlBlockSeperatePolymorphicConstexpr->getUseCountFunction()(controlBlockSeperatePolymorphicConstexpr);
					} else {
						unreachable();
					}
					break;
				default:
					unreachable();
				}
			}
			return 0;
		}

		constexpr Bool empty() const noexcept { return get() == nullptr; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }
		explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }
	};

	template<typename DataType>
	struct IsTriviallyCompareableV<SharedPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct IsTriviallyRelocatableV<SharedPtr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyConstructibleV<SharedPtr<DataType>>
		: TrueType {};
	template<typename DataType>
	struct IsTriviallyDestructibleV<SharedPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct IsTriviallyConstRefConstructibleV<SharedPtr<DataType>>
		: FalseType {};
	template<typename DataType>
	struct IsTriviallyMoveConstructibleV<SharedPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct IsTriviallyConstRefAssignableV<SharedPtr<DataType>>
		: FalseType {};
	template<typename DataType>
	struct IsTriviallyMoveAssignableV<SharedPtr<DataType>>
		: FalseType {};

	template<typename DataType>
	struct ObserverPtr {
	public:
		using element_type = DataType;
	private:
		element_type* dataPtr;
	public:
		//construtor
		constexpr ObserverPtr() noexcept = default;
		constexpr ObserverPtr(const ObserverPtr&) noexcept = default;
		constexpr ObserverPtr(ObserverPtr&&) noexcept = default;
		constexpr explicit ObserverPtr(element_type* ptr) noexcept : dataPtr(ptr) {}

		//destructor
		constexpr ~ObserverPtr() noexcept = default;

		//util 
		constexpr ObserverPtr& self() noexcept { return *this; }
		constexpr const ObserverPtr& self() const noexcept { return *this; }

		//assignment 
		constexpr ObserverPtr& operator=(const ObserverPtr&) noexcept = default;
		constexpr ObserverPtr& operator=(ObserverPtr&&) noexcept = default;

		//modifiers
		[[nodiscard]] constexpr element_type* release() noexcept {
			element_type* tempDataPtr = dataPtr;
			dataPtr = nullptr;
			return dataPtr;
		}
		constexpr void reset(element_type* p = nullptr) noexcept {
			dataPtr = p;
		}
		constexpr void swap(ObserverPtr& other) noexcept {
			element_type* tempDataPtr = other.dataPtr;
			other.dataPtr = dataPtr;
			dataPtr = tempDataPtr;
		}

		//observers
		constexpr element_type* get() const noexcept { return dataPtr; }
		constexpr explicit operator Bool() const noexcept { return dataPtr; }
		constexpr element_type& operator*() const { return dataPtr; }
		constexpr element_type* operator->() const noexcept { return dataPtr; }
		constexpr Bool isNull() noexcept { return dataPtr == nullptr; }
		constexpr Bool isNotNull() noexcept { return dataPtr != nullptr; }

		//conversions
		constexpr explicit operator element_type* () const noexcept { return dataPtr; }

		//compare 
		constexpr Bool operator==(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr == other.dataPtr;
		}
		constexpr Bool operator!=(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr != other.dataPtr;
		}
		constexpr Bool operator<(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr < other.dataPtr;
		}
		constexpr Bool operator>(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr > other.dataPtr;
		}
		constexpr Bool operator<=(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr <= other.dataPtr;
		}
		constexpr Bool operator>=(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr >= other.dataPtr;
		}
		constexpr auto operator<=>(const ObserverPtr<DataType>& other) noexcept {
			return dataPtr <=> other.dataPtr;
		}
	};

	template<typename DataType>
	constexpr ObserverPtr<DataType> makeObserver(DataType* ptr) noexcept {
		return ObserverPtr<DataType>(ptr);
	}
}