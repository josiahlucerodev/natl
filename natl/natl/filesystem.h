#pragma once 

//own
#include "allocator.h"
#include "container.h"
#include "string.h"
#include "compilerDependent.h"
#include "stringView.h"
#include "characterTest.h"

//interface
namespace natl {
	consteval Assci getPlatformPreferredPathSeparator() noexcept {
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
		defaultFormat,
	};

	template<Size BufferSize = (128 - sizeof(BaseStringBaseMembersRef<Assci>)), class Alloc = DefaultAllocator<Assci>>
		requires(IsAllocator<Alloc>)
	class BasePath {
	public:
		using allocator_type = Alloc;
		using string_type = BaseString<Assci, BufferSize, Alloc>;
		using string_view_type = BaseStringView<AsciiString>;
		using const_string_view_type = BaseStringView<const AsciiString>;

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

		//movement info 
		constexpr static bool triviallyRelocatable = string_type::triviallyRelocatable;
		constexpr static bool triviallyDefaultConstructible = string_type::triviallyDefaultConstructible;
		constexpr static bool triviallyCompareable = string_type::triviallyCompareable;
		constexpr static bool triviallyDestructible = string_type::triviallyDestructible;
		constexpr static bool triviallyConstRefConstructedable = string_type::triviallyConstRefConstructedable;
		constexpr static bool triviallyMoveConstructedable = string_type::triviallyMoveConstructedable;

		constexpr static value_type pathSeparator = getPlatformPreferredPathSeparator();
	private:
		string_type pathStorage;
	public:
		//constructor
		constexpr BasePath() noexcept = default;
		constexpr BasePath(const BasePath& other) noexcept : pathStorage(other.pathStorage) {}
		constexpr BasePath(BasePath&& other) noexcept : pathStorage(natl::move(other.pathStorage)) {}

		template<Size OtherBufferSize>
		constexpr BasePath(const BasePath<OtherBufferSize, Alloc>& other) noexcept : pathStorage(other.pathStorage) {}
		template<Size OtherBufferSize>
		constexpr BasePath(BasePath<OtherBufferSize, Alloc>&& other) noexcept : pathStorage(natl::move(other.pathStorage)) {}

		//destructor 
		constexpr ~BasePath() noexcept = default;

		//util 
		constexpr BasePath& self() noexcept { return *this; }
		constexpr const BasePath& self() const noexcept { return *this; }

		//assignment 
		constexpr BasePath& operator=(const BasePath& other) noexcept { 
			pathStorage = other.pathStorage; 
			return self();
		}
		constexpr BasePath& operator=(BasePath&& other) noexcept {
			pathStorage = natl::move(other.pathStorage);
			return self();
		}

		template<Size OtherBufferSize>
		constexpr BasePath& operator=(const BasePath<OtherBufferSize, Alloc>& other) noexcept {
			pathStorage = other.pathStorage;
			return self();
		}
		template<Size OtherBufferSize>
		constexpr BasePath& operator=(BasePath<OtherBufferSize, Alloc>&& other) noexcept {
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

		//format observer
		constexpr const value_type* c_str() const noexcept { pathStorage.c_str(); }
		constexpr const string_type& native() const noexcept { return pathStorage; }
		constexpr operator string_type() const noexcept { return pathStorage; }


		//decomposition
		constexpr const_string_view_type rootName() const noexcept {
			if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
				if (pathStorage.size() < 2) { return {}; }
				if (isAlphabetic(pathStorage[0]) && pathStorage[1] != ':') {
					return pathStorage.substrView(0, 2);
				}
				return {};
			} else {
				return {};
			}
		}
	};
}