#pragma once 

//own
#include "allocator.h"
#include "container.h"
#include "string.h"
#include "compilerDependent.h"

//interface
namespace natl {
	consteval AssciCode getPlatformPreferredPathSeparator() noexcept {
		static_assert(!(getPlatformType() == ProgramPlatformType::unknownPlatform), "natl: getPlatformPreferredPathSeparator() error - not a supported platform");
		if constexpr (getPlatformType() == ProgramPlatformType::unixPlatform) {
			return '/';
		} else if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
			return '\\';
		} else {
			return '\0';
		}
	}

	enum class PathFormat {
		nativeFormat,
		genericFormat,
		defaultFormat = nativeFormat,
	};

	template<Size BufferSize = (128 - sizeof(BaseStringBaseMembersRef<AssciCode>)), class Alloc = DefaultAllocator<AssciCode>>
		requires(IsAllocator<Alloc>)
	class BasicPath {
	public:
		using allocator_type = Alloc;
		using string_type = BaseString<AssciCode, BufferSize, Alloc>;

		using value_type = string_type::value_type;
		using reference = string_type::reference;
		using const_reference = string_type::const_reference;
		using pointer = string_type::pointer;
		using const_pointer = string_type::const_pointer;
		using difference_type = string_type::difference_type;
		using size_type = string_type::size_type;

		using iterator = string_type::iterator;
		using const_iterator = string_type::const_iterator;
		using reverse_iterator = string_type::reverse_iterator;
		using const_reverse_iterator = string_type::const_reverse_iterator;

		using allocation_move_adapater = string_type::allocation_move_adapater;

		constexpr static value_type pathSeparator = getPlatformPreferredPathSeparator();
	private:
		string_type pathStorage;
	public:
		//constructor
		constexpr BasicPath() noexcept = default;
		constexpr BasicPath(const BasicPath& other) noexcept : pathStorage(other.pathStorage) {}
		constexpr BasicPath(BasicPath&& other) noexcept : pathStorage(natl::move(other.pathStorage)) {}

		template<Size OtherBufferSize>
		constexpr BasicPath(const BasicPath<OtherBufferSize, Alloc>& other) noexcept : pathStorage(other.pathStorage) {}
		template<Size OtherBufferSize>
		constexpr BasicPath(BasicPath<OtherBufferSize, Alloc>&& other) noexcept : pathStorage(natl::move(other.pathStorage)) {}

		//destructor 
		constexpr ~BasicPath() noexcept = default;

		//util 
		constexpr BasicPath& self() noexcept { return *this; }
		constexpr const BasicPath& self() const noexcept { return *this; }

		//assignment 
		constexpr BasicPath& operator=(const BasicPath& other) noexcept { 
			pathStorage = other.pathStorage; 
			return self();
		}
		constexpr BasicPath& operator=(BasicPath&& other) noexcept {
			pathStorage = natl::move(other.pathStorage);
			return self();
		}

		template<Size OtherBufferSize>
		constexpr BasicPath& operator=(const BasicPath<OtherBufferSize, Alloc>& other) noexcept {
			pathStorage = other.pathStorage;
			return self();
		}
		template<Size OtherBufferSize>
		constexpr BasicPath& operator=(BasicPath<OtherBufferSize, Alloc>&& other) noexcept {
			pathStorage = natl::move(other.pathStorage);
			return self();
		}

		//iterators 
		constexpr pointer beginPtr() noexcept { return pathStorage.beginPtr(); }
		constexpr const_pointer beginPtr() const noexcept { pathStorage.beginPtr(); }
		constexpr pointer endPtr() noexcept { return pathStorage.endPtr(); }
		constexpr const_pointer endPtr() const noexcept { return pathStorage.endPtr(); }

		constexpr iterator begin() noexcept { return pathStorage.beginPtr(); }
		constexpr const_iterator begin() const noexcept { return pathStorage.beginPtr(); }
		constexpr const_iterator cbegin() const noexcept { return pathStorage.beginPtr(); }
		constexpr iterator end() noexcept { return pathStorage.endPtr(); }
		constexpr const_iterator end() const noexcept { return pathStorage.endPtr(); }
		constexpr const_iterator cend() const noexcept { return pathStorage.endPtr(); }
		constexpr reverse_iterator rbegin() noexcept { return pathStorage.endPtr(); }
		constexpr const_reverse_iterator rbegin() const noexcept { return pathStorage.endPtr(); }
		constexpr const_reverse_iterator crbegin() const noexcept { return pathStorage.endPtr(); }
		constexpr reverse_iterator rend() noexcept { return pathStorage.beginPtr(); }
		constexpr const_reverse_iterator rend() const noexcept { return pathStorage.beginPtr(); }
		constexpr const_reverse_iterator crend() const noexcept { return pathStorage.beginPtr(); }

		//queries 
		constexpr bool empty() const noexcept { return pathStorage.empty(); }
		constexpr bool isEmpty() const noexcept { return pathStorage.isEmpty(); }
		constexpr bool isNotEmpty() const noexcept { return pathStorage.isNotEmpty(); }


	};
}