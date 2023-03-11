//module
export module natl.partitioner;

//std
import std;

export const char* foo() { return "foo"; }

//interface
export namespace natl {
	template<class T>
	class PartitionIterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
	private:
		T* dataPtr;
	public:
		PartitionIterator(T* const dataPtr) : dataPtr(dataPtr) {}
	public:
		T& operator*() const { return *dataPtr; }
		T* operator->() const { return dataPtr; }
		PartitionIterator<T>& operator++() { dataPtr++; return *this; }
		PartitionIterator<T> operator++(int) { PartitionIterator<T> tempIt = *this; ++(*this); return tempIt; }
		bool operator== (const PartitionIterator<T>& right) const noexcept {
			return dataPtr == right.dataPtr;
		};
		bool operator!= (const PartitionIterator<T>& right) const noexcept {
			return dataPtr != right.dataPtr;
		};
	};

	template<class T>
	class ConstPartitionIterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = const T;
		using pointer = const T*;
		using reference = const T&;
	private:
		const T* dataPtr;
	public:
		ConstPartitionIterator(const T* const dataPtr) : dataPtr(dataPtr) {}
	public:
		const T& operator*() const { return *dataPtr; }
		const T* operator->() const { return dataPtr; }
		PartitionIterator<T>& operator++() { dataPtr++; return *this; }
		PartitionIterator<T> operator++(int) { PartitionIterator<T> tempIt = *this; ++(*this); return tempIt; }
		bool operator== (const PartitionIterator<T>& right) const noexcept {
			return dataPtr == right.dataPtr;
		};
		bool operator!= (const PartitionIterator<T>& right) const noexcept {
			return dataPtr != right.dataPtr;
		};
	};

	template <class T>
	class Partition {
	private:
		T* dataPtr;
		std::size_t sizePartition;
	public:
		Partition() : dataPtr(), sizePartition(0) {}
		Partition(T* dataPtr, const std::size_t& size) : dataPtr(dataPtr), sizePartition(size) {}
	public:
		const std::size_t& getSize() const noexcept { return sizePartition; }
		std::size_t size() const noexcept { return sizePartition; }
		T* data() const noexcept { return dataPtr; };
		bool isEmpty() const noexcept { return !bool(sizePartition); }
		bool isNotEmpty() const noexcept { return bool(sizePartition); }
		PartitionIterator<T> begin() const noexcept { return PartitionIterator<T>(dataPtr); }
		PartitionIterator<T> end() const noexcept { return PartitionIterator<T>(dataPtr + sizePartition); }
		PartitionIterator<T> cbegin() const noexcept { return ConstPartitionIterator<T>(dataPtr); }
		PartitionIterator<T> cend() const noexcept { return ConstPartitionIterator<T>(dataPtr + sizePartition); }
		ConstPartitionIterator<T> constBegin() const noexcept { return ConstPartitionIterator<T>(dataPtr); }
		ConstPartitionIterator<T> constEnd() const noexcept { return ConstPartitionIterator<T>(dataPtr + sizePartition); }
		operator bool() const noexcept { return isNotEmpty(); }
	public:
		const T& getConstRefAt(const std::size_t& index) const { return dataPtr[index]; };
		T& getRefAt(const std::size_t& index) { return dataPtr[index]; };
		const T* getConstPtrAt(const std::size_t& index) const { return &dataPtr[index]; };
		T* getPtrAt(const std::size_t& index) { return &dataPtr[index]; };

		const T* getClampedConstRefAt(const std::size_t& index) const { return dataPtr[std::clamp<std::size_t>(index, 0, sizePartition)]; };
		T& getClampedRefAt(const std::size_t& index) { return dataPtr[std::clamp<std::size_t>(index, 0, sizePartition)]; };
		const T* getClampedConstPtrAt(const std::size_t& index) const { return &dataPtr[std::clamp<std::size_t>(index, 0, sizePartition)]; };
		T* getClampedPtrAt(const std::size_t& index) { return &dataPtr[std::clamp<std::size_t>(index, 0, sizePartition)]; };

		std::size_t getFrontIndex() { return 0; }
		std::size_t getBackIndex() { return sizePartition ? 0 : sizePartition - 1; }

		T& front() { return getRefAt(getFrontIndex()); }
		const T& front() const { return getConstRefAt(getFrontIndex()); }
		T& back() { return getRefAt(getBackIndex()); }
		const T& back() const { return getConstRefAt(getBackIndex()); }

		bool has(const std::size_t& index) const { return index < sizePartition; };
		bool notHave(const std::size_t& index) const { return index >= sizePartition; };

		std::optional<std::reference_wrapper<const T>> safeGetConstRefAt(const std::size_t& index) const {
			if (notHave(index)) { return std::optional<std::reference_wrapper<const T>>(); }
			return std::optional<std::reference_wrapper<const T>>(getConstRefAt(index));
		};
		std::optional<std::reference_wrapper<T>> safeGetRefAt(const std::size_t& index) {
			if (notHave(index)) { return std::optional<std::reference_wrapper<T>>(); }
			return std::optional<std::reference_wrapper<T>>(getRefAt(index));
		};

		std::optional<const T*> safeGetConstPtrAt(const std::size_t& index) {
			if (notHave(index)) { return std::optional<const T*>(); }
			return std::optional<const T*>(getConstPtrAt(index));
		};
		std::optional<T*> safeGetPtrAt(const std::size_t& index) {
			if (notHave(index)) { return std::optional<T*>(); }
			return std::optional<T*>(getPtrAt(index));
		};

		std::optional<std::reference_wrapper<T>> safeFront() { return safeGetRefAt(getFrontIndex()); }
		std::optional<std::reference_wrapper<const T>> safeFront() const { return safeGetConstRefAt(getFrontIndex()); }
		std::optional<std::reference_wrapper<T>> safeBack() { return safeGetRefAt(getBackIndex()); }
		std::optional<std::reference_wrapper<const T>> safeBack() const { return safeGetConstRefAt(getBackIndex()); }
	};

	template<class T, std::size_t size>
	class FixedPartitioner {
		T partitionData[size];
		std::size_t partitionIndex;
	public:
		inline std::size_t size() const noexcept { return size; }
		inline std::size_t capacity() const noexcept { return partitionData.capacity(); }
		inline bool isFull() const noexcept { return capacity() < size; };
		inline bool isEmpty() const noexcept { return partitionIndex == 0; };
		inline bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		inline T* data() const noexcept { return partitionData; }
		inline Partition<T> newPartition(const std::size_t& partiationSize) {
			if (capacity() < partitionIndex + partiationSize) {
				return Partition<T>(nullptr, 0);
			}

			Partition<T> partition(&partitionData.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return partition;
		}
	public:
		PartitionIterator<T> begin() const noexcept { return PartitionIterator<T>(data()); }
		PartitionIterator<T> end() const noexcept { return PartitionIterator<T>(data() + size()); }
		ConstPartitionIterator<T> constBegin() const noexcept { return ConstPartitionIterator<T>(partitionData); }
		ConstPartitionIterator<T> constEnd() const noexcept { return ConstPartitionIterator<T>(data() + size()); }
	};

	class DynamicBytePartitioner;

	template<class T>
	class DynamicPartitioner {
	private:
		std::vector<T> data;
		std::size_t partitionIndex;
	public:
		DynamicPartitioner() : data(), partitionIndex(0) {}
	public:
		inline void resize(const std::size_t& newCapacity) { data.resize(newCapacity); }
		inline std::size_t size() const noexcept { return partitionIndex + 1; }
		inline std::size_t capacity() const noexcept { return data.capacity(); }
		inline bool isFull() const noexcept { return capacity() < partitionIndex + 1; };
		inline bool isEmpty() const noexcept { return partitionIndex == 0; };
		inline bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		inline bool shrinkToFit() { data.shrink_to_fit(); }
		inline Partition<T> newPartition(const std::size_t& partiationSize) {
			if (capacity() < partitionIndex + partiationSize) {
				return Partition<T>(nullptr, 0);
			}

			Partition<T> partition(&data.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return partition;
		}
		void swap(DynamicPartitioner<T>& dst) {
			dst.data.swap(data);
			std::size_t tempPartitionIndex = partitionIndex;
			partitionIndex = dst.partitionIndex;
			dst.partitionIndex = tempPartitionIndex;
		}
	public:
		PartitionIterator<T> begin() const noexcept { return PartitionIterator<T>(data.data()); }
		PartitionIterator<T> end() const noexcept { return PartitionIterator<T>(data.data() + size()); }
		ConstPartitionIterator<T> constBegin() const noexcept { return ConstPartitionIterator<T>(data.data()); }
		ConstPartitionIterator<T> constEnd() const noexcept { return ConstPartitionIterator<T>(data.data() + size()); }
	public:
		friend class DynamicBytePartitioner;
	};


	inline std::size_t alignmentOffset(std::uint8_t* ptr, std::size_t alignment) noexcept {
		size_t offset = static_cast<size_t>(reinterpret_cast<std::uintptr_t>(ptr) & (alignment - 1));
		if (offset != 0) { offset = alignment - offset; }// number of bytes to skip 
		return offset;
	}


	class DynamicBytePartitioner : public DynamicPartitioner<std::uint8_t> {
	public:
		DynamicBytePartitioner() : DynamicPartitioner<std::uint8_t>() {}
	public:
		template<class T>
		Partition<T> newPartition(const std::size_t& partiationSize) {
			const std::size_t offset = alignmentOffset(data.data(), alignof(T));
			const std::size_t partiationByteSize = partiationSize * sizeof(T) + offset;
			if (capacity() < partiationByteSize) {
				return Partition<T>(nullptr, 0);
			}

			DynamicPartitioner<std::uint8_t>& castSelf = *static_cast<DynamicPartitioner<std::uint8_t>*>(this);
			Partition<std::uint8_t> bytePartition = castSelf.newPartition(partiationByteSize);
			Partition<T> partition(static_cast<T*>(static_cast<void*>(bytePartition.getPtrAt(offset))), partiationSize);
			return partition;
		}
	};

	template<class T>
	class SubPartitioner {
	private:
		Partition<T> partition;
		std::size_t partitionIndex;
	public:
		SubPartitioner() = default;
		SubPartitioner(const Partition<T>& partition) : partition(partition), partitionIndex(0) {}
	public:
		inline std::size_t size() const noexcept { return partitionIndex + 1; }
		inline std::size_t capacity() const noexcept { return partition.size(); }
		inline bool isFull() const noexcept { return capacity() < size(); };
		inline bool isEmpty() const noexcept { return partitionIndex == 0; };
		inline bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		inline Partition<T> getInternalPartition() const { return partition; };
		inline void assign(const Partition<T>& assignPartition) { partition = assignPartition; partitionIndex = 0; };
	public:
		Partition<T> newPartition(const std::size_t& partiationSize) {
			if (capacity() < partiationSize) {
				return Partition<T>(nullptr, 0);
			}

			Partition<T> outputPartition(partition.getPtrAt(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return outputPartition;
		}
	};

}