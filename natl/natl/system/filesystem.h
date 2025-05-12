#pragma once 

//own
#include "../util/compilerDependent.h"
#include "../util/allocator.h"
#include "../util/typeTraits.h"
#include "../util/characterTest.h"
#include "../util/iteration.h"
#include "../container/container.h"
#include "../container/dynArray.h"
#include "../container/string.h"
#include "../container/stringView.h"
#include "../container/arrayView.h"
#include "../fundamental/strongType.h"
#include "../fundamental/option.h"
#include "../fundamental/expect.h"

//interface
namespace natl {
	consteval Ascii getPlatformPreferredPathSeparator() noexcept {
		if constexpr (getPlatformType() == ProgramPlatformType::unixPlatform) {
			return '/';
		} else if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
			return '\\';
		} else {
			return '\0';
		}
	}

	constexpr Bool isPlatformPathSeparator(const Ascii character) noexcept {
		return character == '\\' || character == '/';
	}

	namespace impl {
		template<typename CharType> 
		struct BasePathView {
		public:

			using path_view_type = BasePathView<Ascii>;
			using const_path_view_type = BasePathView<const Ascii>;
			using string_view_type = BaseStringView<Ascii>;
			using const_string_view_type = BaseStringView<const Ascii>;
			using array_view_type = ArrayView<Ascii>;
			using const_array_view_type = ArrayView<const Ascii>;

			using string_view_storage_type = Conditional<IsNotConst<CharType>, string_view_type, const_string_view_type>;

			using value_type = string_view_type::value_type;
			using reference = string_view_type::reference;
			using const_reference = string_view_type::const_reference;
			using pointer = string_view_type::pointer;
			using const_pointer = string_view_type::const_pointer;
			using optional_pointer = string_view_type::optional_pointer;
			using optional_const_pointer = string_view_type::optional_const_pointer;
			using difference_type = string_view_type::difference_type;
			using size_type = string_view_type::size_type;

			using iterator = string_view_type::iterator;
			using const_iterator = string_view_type::const_iterator;
			using reverse_iterator = string_view_type::reverse_iterator;
			using const_reverse_iterator = string_view_type::const_reverse_iterator;
		private:
			string_view_storage_type pathStringView;
		public:
			//constructor 
			constexpr BasePathView() noexcept = default;
			constexpr BasePathView(const path_view_type& other) noexcept : pathStringView(other) {}
			constexpr BasePathView(const const_path_view_type& other) noexcept requires(IsConst<CharType>) : pathStringView(other) {}
			constexpr BasePathView(Ascii* stringPtr, const size_type length) noexcept : pathStringView(stringPtr, length) {}
			constexpr BasePathView(const Ascii* stringPtr, const size_type length) noexcept requires(IsConst<CharType>) : pathStringView(stringPtr, length) {}
			explicit constexpr BasePathView(Ascii* str) noexcept : pathStringView(str) {}
			explicit constexpr BasePathView(const Ascii* str) noexcept requires(IsConst<CharType>) : pathStringView(str) {}
			explicit constexpr BasePathView(const string_view_type& str) noexcept : pathStringView(str) {}
			explicit constexpr BasePathView(const const_string_view_type& str) noexcept requires(IsConst<CharType>) : pathStringView(str) {}

			//destructor 
			constexpr ~BasePathView() noexcept = default;

			//util
			constexpr BasePathView& self() noexcept { return *this; }
			constexpr const BasePathView& self() const noexcept { return *this; }

			//convert 
			constexpr string_view_type toStringView() noexcept requires(IsNotConst<CharType>) { return pathStringView; }
			constexpr const_string_view_type toStringView() const noexcept { return pathStringView; }
			constexpr array_view_type toArrayView() noexcept requires(IsNotConst<CharType>) { return array_view_type(pathStringView.data(), size()); }
			constexpr const_array_view_type toArrayView() const noexcept { return const_array_view_type(pathStringView.data(), size()); }
			template<typename StringLike>
				requires(IsConstructibleC<StringLike, const CharType*, Size>)
			constexpr StringLike convertTo() { 
				return StringLike(pathStringView.data(), pathStringView.size()); 
			}

			constexpr operator string_view_type() noexcept { return toStringView(); }
			constexpr operator const_string_view_type() const noexcept { return toStringView(); }
			constexpr operator array_view_type() noexcept { return toArrayView(); }
			constexpr operator const_array_view_type() const noexcept { return toArrayView(); }

			//element access 
			constexpr pointer data() noexcept requires(IsNotConst<CharType>) {
				return pathStringView.data();
			}
			constexpr const_pointer data() const noexcept {
				return pathStringView.data();
			}
			constexpr pointer c_str() const noexcept requires(IsNotConst<CharType>) {
				return pathStringView.c_str();
			}
			constexpr const_pointer c_str() const noexcept {
				return pathStringView.c_str();
			}
			constexpr reference operator[](const Size index) noexcept requires(IsNotConst<CharType>) {
				return pathStringView[index];
			}
			constexpr const_reference operator[](const Size index) const noexcept {
				return pathStringView[index];
			}

			//iterators
			constexpr iterator begin() noexcept requires(IsNotConst<CharType>) { return pathStringView.begin(); }
			constexpr const_iterator begin() const noexcept { return pathStringView.begin(); }
			constexpr const_iterator cbegin() const noexcept { return pathStringView.cbegin(); }
			constexpr iterator end() noexcept requires(IsNotConst<CharType>) { return pathStringView.end(); }
			constexpr const_iterator end() const noexcept { return pathStringView.end(); }
			constexpr const_iterator cend() const noexcept { return pathStringView.cend(); }
			constexpr reverse_iterator rbegin() noexcept requires(IsNotConst<CharType>) { return pathStringView.rbegin(); }
			constexpr const_reverse_iterator rbegin() const noexcept { return pathStringView.rbegin(); }
			constexpr const_reverse_iterator crbegin() const noexcept { return pathStringView.crbegin(); }
			constexpr reverse_iterator rend() noexcept requires(IsNotConst<CharType>) { return pathStringView.rend(); }
			constexpr const_reverse_iterator rend() const noexcept { return pathStringView.rend(); }
			constexpr const_reverse_iterator crend() const noexcept { return pathStringView.crend(); }

			//capacity 
			constexpr size_type size() const noexcept { return pathStringView.size(); }
			constexpr size_type length() const noexcept { return pathStringView.length(); }
			constexpr size_type max_size() const noexcept { return pathStringView.max_size(); }
			constexpr Bool empty() const noexcept { return pathStringView.empty(); }
			constexpr Bool isEmpty() const noexcept { return pathStringView.isEmpty(); }
			constexpr Bool isNotEmpty() const noexcept { return pathStringView.isNotEmpty(); }

			friend struct BasePathView<Ascii>;
			friend struct BasePathView<const Ascii>;

			//compare
			constexpr Bool operator==(const path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView == rhs.pathStringView;
			}
			constexpr Bool operator!=(const path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView != rhs.pathStringView;
			}
			constexpr Bool operator<(const path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView < rhs.pathStringView;
			}
			constexpr Bool operator<=(const path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView <= rhs.pathStringView;
			}
			constexpr Bool operator>(const path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView > rhs.pathStringView;
			}
			constexpr Bool operator>=(const path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView >= rhs.pathStringView;
			}
			constexpr auto operator<=>(const path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView <=> rhs.pathStringView;
			}

			constexpr Bool operator==(const const_path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView == rhs.pathStringView;
			}
			constexpr Bool operator!=(const const_path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView != rhs.pathStringView;
			}
			constexpr Bool operator<(const const_path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView < rhs.pathStringView;
			}
			constexpr Bool operator<=(const const_path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView <= rhs.pathStringView;
			}
			constexpr Bool operator>(const const_path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView > rhs.pathStringView;
			}
			constexpr Bool operator>=(const const_path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView >= rhs.pathStringView;
			}
			constexpr auto operator<=>(const const_path_view_type& rhs) const noexcept {
				const BasePathView& lhs = self();
				return lhs.pathStringView <=> rhs.pathStringView;
			}

			//hash
			constexpr size_type hash() const noexcept {
				return pathStringView.hash();
			}
			constexpr static size_type staticHash(const BasePathView& pathView) noexcept {
				return string_view_type::staticHash(pathView.pathStringView);
			}

			//decomposition
		private:
			constexpr Bool internalHasRootName() const noexcept {
				return isAlphabetic(pathStringView[0]) && pathStringView[1] == ':';
			}

		public:
			constexpr path_view_type rootName() noexcept requires(IsNotConst<CharType>) {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 2) { return {}; }
					if (internalHasRootName()) {
						return path_view_type(pathStringView.substr(0, 2));
					}
					return {};
				} else {
					return {};
				}
			}

			constexpr path_view_type rootDirectory() noexcept requires(IsNotConst<CharType>) {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 1) { return {}; }
					if (isPlatformPathSeparator(pathStringView[0])) {
						return path_view_type(pathStringView.substr(0, 1));
					}
					if (pathStringView.size() < 3) { return {}; }
					if (isPlatformPathSeparator(pathStringView[2])) {
						return path_view_type(pathStringView.substr(2, 1));
					}
					return {};
				} else {
					if (pathStringView.size() < 1) { return {}; }
					if (isPlatformPathSeparator(pathStringView[1])) {
						return path_view_type(pathStringView.substr(0, 1));
					}
					return {};
				}
			}

			constexpr path_view_type rootPath() noexcept requires(IsNotConst<CharType>) {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 1) { return {}; }
					if (isPlatformPathSeparator(pathStringView[0])) {
						return path_view_type(pathStringView.substr(0, 1));
					}

					if (pathStringView.size() < 3) { return {}; }
					if (internalHasRootName() && isPlatformPathSeparator(pathStringView[2])) {
						return path_view_type(pathStringView.substr(0, 3));
					}
					return {};
				} else {
					return rootDirectory();
				}
			}

			constexpr path_view_type relativePath() noexcept requires(IsNotConst<CharType>) {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 2) { return {}; }
					if (isPlatformPathSeparator(pathStringView[0])) {
						return path_view_type(pathStringView.substr(1));
					}
					if (pathStringView.size() < 4) { return {}; }
					if (isPlatformPathSeparator(pathStringView[2])) {
						return path_view_type(pathStringView.substr(3));
					}
					return pathStringView;
				} else {
					if (pathStringView.size() < 2) { return {}; }
					if (isPlatformPathSeparator(pathStringView[1])) {
						return path_view_type(pathStringView.substr(1));
					}
					return pathStringView;
				}
			}

			constexpr path_view_type parentPath() noexcept requires(IsNotConst<CharType>) {
				if (isEmpty()) { return{}; }
				path_view_type filenamePathView = filename();
				return path_view_type(pathStringView.substr(pathStringView.size() - filenamePathView.size()));
			}

			constexpr path_view_type filename() noexcept requires(IsNotConst<CharType>) {
				if (isEmpty()) { return{}; }

				size_type filenameSize = 0;
				for (const Ascii& character : makeReverseIteration(pathStringView)) {
					filenameSize++;
					if (isPlatformPathSeparator(character)) {
						break;
					}
				}

				return path_view_type(pathStringView.substr(pathStringView.size() - filenameSize));
			}

			constexpr path_view_type stem() noexcept requires(IsNotConst<CharType>) {
				if (isEmpty()) { return{}; }

				size_type extensionSize = 0;
				for (const Ascii& character : makeReverseIteration(pathStringView)) {
					extensionSize++;
					if (character == '.') {
						break;
					}

					if (isPlatformPathSeparator(character)) {
						return path_view_type(pathStringView.substr(pathStringView.size() - extensionSize));
					}
				}

				if (extensionSize == size()) { return{}; }
				reverse_iterator stemIter = rbegin() + extensionSize + 1;
				if (isPlatformPathSeparator(*stemIter)) {
					return path_view_type(pathStringView.substr(pathStringView.size() - extensionSize));
				}

				size_type stemSize = 0;
				for (; stemIter < rend(); ++stemIter) {
					stemSize++;
					if (isPlatformPathSeparator(*stemIter)) {
						return path_view_type(pathStringView.substr(pathStringView.size() - (extensionSize + stemSize), stemSize));
					}
				}

				return {};
			}

			constexpr path_view_type extension() noexcept requires(IsNotConst<CharType>) {
				size_type extensionSize = 0;
				for (const Ascii& character : makeReverseIteration(pathStringView)) {
					extensionSize++;
					if (character == '.') {
						return path_view_type(pathStringView.substr(pathStringView.size() - extensionSize));
					}
					if (isPlatformPathSeparator(character)) {
						return {};
					}
				}
				return {};
			}

			constexpr const_path_view_type rootName() const noexcept {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 2) { return {}; }
					if (internalHasRootName()) {
						return const_path_view_type(pathStringView.substr(0, 2));
					}
					return {};
				} else {
					return {};
				}
			}

			constexpr const_path_view_type rootDirectory() const noexcept {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 1) { return {}; }
					if (isPlatformPathSeparator(pathStringView[0])) {
						return const_path_view_type(pathStringView.substr(0, 1));
					}
					if (pathStringView.size() < 3) { return {}; }
					if (isPlatformPathSeparator(pathStringView[2])) {
						return const_path_view_type(pathStringView.substr(2, 1));
					}
					return {};
				} else {
					if (pathStringView.size() < 1) { return {}; }
					if (isPlatformPathSeparator(pathStringView[1])) {
						return const_path_view_type(pathStringView.substr(0, 1));
					}
					return {};
				}
			}

			constexpr const_path_view_type rootPath() const noexcept {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 1) { return {}; }
					if (isPlatformPathSeparator(pathStringView[0])) {
						return const_path_view_type(pathStringView.substr(0, 1));
					}

					if (pathStringView.size() < 3) { return {}; }
					if (internalHasRootName() && isPlatformPathSeparator(pathStringView[2])) {
						return const_path_view_type(pathStringView.substr(0, 3));
					}
					return {};
				} else {
					return rootDirectory();
				}
			}

			constexpr const_path_view_type relativePath() const noexcept {
				if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
					if (pathStringView.size() < 2) { return {}; }
					if (isPlatformPathSeparator(pathStringView[0])) {
						return const_path_view_type(pathStringView.substr(1));
					}
					if (pathStringView.size() < 4) { return {}; }
					if (isPlatformPathSeparator(pathStringView[2])) {
						return const_path_view_type(pathStringView.substr(3));
					}
					return pathStringView;
				} else {
					if (pathStringView.size() < 2) { return {}; }
					if (isPlatformPathSeparator(pathStringView[1])) {
						return const_path_view_type(pathStringView.substr(1));
					}
					return pathStringView;
				}
			}

			constexpr const_path_view_type parentPath() const noexcept {
				if (isEmpty()) { return{}; }
				const_path_view_type filenamePathView = filename();
				return const_path_view_type(pathStringView.substr(pathStringView.size() - filenamePathView.size()));
			}

			constexpr const_path_view_type filename() const noexcept {
				if (isEmpty()) { return{}; }

				size_type filenameSize = 0;
				for (const Ascii& character : makeReverseIteration(pathStringView)) {
					filenameSize++;
					if (isPlatformPathSeparator(character)) {
						break;
					}
				}

				return const_path_view_type(pathStringView.substr(pathStringView.size() - filenameSize));
			}

			constexpr const_path_view_type stem() const noexcept {
				if (isEmpty()) { return{}; }

				size_type extensionSize = 0;
				for (const Ascii& character : makeReverseIteration(pathStringView)) {
					extensionSize++;
					if (character == '.') {
						break;
					}

					if (isPlatformPathSeparator(character)) {
						return const_path_view_type(pathStringView.substr(pathStringView.size() - extensionSize));
					}
				}

				if (extensionSize == size()) { return{}; }
				const_reverse_iterator stemIter = rbegin() + extensionSize + 1;
				if (isPlatformPathSeparator(*stemIter)) {
					return const_path_view_type(pathStringView.substr(pathStringView.size() - extensionSize));
				}

				size_type stemSize = 0;
				for (; stemIter < rend(); ++stemIter) {
					stemSize++;
					if (isPlatformPathSeparator(*stemIter)) {
						return const_path_view_type(pathStringView.substr(pathStringView.size() - (extensionSize + stemSize), stemSize));
					}
				}

				return {};
			}

			constexpr const_path_view_type extension() const noexcept {
				size_type extensionSize = 0;
				for (const Ascii& character : makeReverseIteration(pathStringView)) {
					extensionSize++;
					if (character == '.') {
						return const_path_view_type(pathStringView.substr(pathStringView.size() - extensionSize));
					}
					if (isPlatformPathSeparator(character)) {
						return {};
					}
				}
				return {};
			}



			//queries
			constexpr Bool hasRootName() const noexcept { return rootName().isNotEmpty(); }
			constexpr Bool hasRootPath() const noexcept { return rootPath().isNotEmpty(); }
			constexpr Bool hasRootDirectory() const noexcept { return rootDirectory().isNotEmpty(); }
			constexpr Bool hasRelativePath() const noexcept { return relativePath().isNotEmpty(); }
			constexpr Bool hasParentPath() const noexcept { return parentPath().isNotEmpty(); }
			constexpr Bool hasFilename() const noexcept { return filename().isNotEmpty(); }
			constexpr Bool hasStem() const noexcept { return stem().isNotEmpty(); }
			constexpr Bool hasExtension() const noexcept { return extension().isNotEmpty(); }

			constexpr Bool doesNotHaveRootName() const noexcept { return rootName().isEmpty(); }
			constexpr Bool doesNotHaveRootPath() const noexcept { return rootPath().isEmpty(); }
			constexpr Bool doesNotHaveRootDirectory() const noexcept { return rootDirectory().isEmpty(); }
			constexpr Bool doesNotHaveRelativePath() const noexcept { return relativePath().isEmpty(); }
			constexpr Bool doesNotHaveParentPath() const noexcept { return parentPath().isEmpty(); }
			constexpr Bool doesNotHaveFilename() const noexcept { return filename().isEmpty(); }
			constexpr Bool doesNotHaveStem() const noexcept { return stem().isEmpty(); }
			constexpr Bool doesNotHaveExtension() const noexcept { return extension().isEmpty(); }

			constexpr Bool isAbsolute() const noexcept { return hasRootPath(); }
			constexpr Bool isRelative() const noexcept { return !hasRootPath(); }
		};
	}

	using PathView = impl::BasePathView<Ascii>;
	using ConstPathView = impl::BasePathView<const Ascii>;

	template<typename CharType>
	struct IsCopyableStorageDstT<impl::BasePathView<CharType>> : TrueType {};

	enum struct PathFormat {
		nativeFormat,
		genericFormat,
		standard = genericFormat,
	};

	template<Size BufferSize = (128 - sizeof(BaseStringBaseMembersRef<Ascii>)), typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct BasePath {
	public:
		using allocator_type = Alloc;
		using typed_allocator_type = allocator_type::template rebind<Ascii>;
		using string_type = BaseString<Ascii, BufferSize, Alloc>;

		using path_view_type = PathView;
		using const_path_view_type = ConstPathView;
		using string_view_type = BaseStringView<Ascii>;
		using const_string_view_type = BaseStringView<const Ascii>;
		using array_view_type = ArrayView<Ascii>;
		using const_array_view_type = ArrayView<const Ascii>;

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
		constexpr static PathFormat autoFormat = PathFormat::standard;

		template<Size, typename Alloc>
			requires(IsAllocatorC<Alloc>)
		friend struct BasePath;

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

		constexpr BasePath(string_type&& source, const PathFormat format = autoFormat) noexcept : pathStorage(natl::move(source)) {
			formatPath(format);
		}
		constexpr BasePath(const Ascii* source, const PathFormat format = autoFormat) noexcept : pathStorage(source) {
			formatPath(format);
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const Ascii>)
		constexpr BasePath(const StringViewLike& source, const PathFormat format = autoFormat) noexcept 
			: pathStorage(source.data(), source.size()) {
			formatPath(format);
		}

		constexpr BasePath(allocation_move_adapater&& allocationMoveAdapater, const PathFormat format = autoFormat) noexcept 
			: pathStorage(natl::move(allocationMoveAdapater)) {
			formatPath(format);
		}


		//destructor 
		constexpr ~BasePath() noexcept = default;

		//util 
		constexpr BasePath& self() noexcept { return *this; }
		constexpr const BasePath& self() const noexcept { return *this; }

		//convert 
		constexpr path_view_type toPathView() noexcept { return path_view_type(pathStorage.toStringView()); }
		constexpr const_path_view_type toPathView() const noexcept { return const_path_view_type(pathStorage.toStringView()); }
		constexpr string_view_type toStringView() noexcept { return pathStorage.toStringView(); }
		constexpr const_string_view_type toStringView() const noexcept { return pathStorage.toStringView(); }
		constexpr array_view_type toArrayView() noexcept { return pathStorage.toArrayView(); }
		constexpr const_array_view_type toArrayView() const noexcept { return pathStorage.toArrayView(); }

		constexpr operator path_view_type() noexcept { return toPathView(); }
		constexpr operator const_path_view_type() const noexcept { return toPathView(); }
		constexpr operator string_view_type() noexcept { return toStringView(); }
		constexpr operator const_string_view_type() const noexcept { return toStringView(); }
		constexpr operator array_view_type() noexcept { return toArrayView(); }
		constexpr operator const_array_view_type() const noexcept { return toArrayView(); }

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

		constexpr BasePath& operator=(string_type&& source) noexcept {
			pathStorage = natl::move(source);
			formatPath();
			return self();
		}
		constexpr BasePath& operator=(const Ascii* source) noexcept {
			pathStorage = source;
			formatPath();
			return self();
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const Ascii>)
		constexpr BasePath& operator=(const StringViewLike& source) noexcept {
			pathStorage = source;
			formatPath();
			return self();
		}

		constexpr BasePath& operator=(allocation_move_adapater&& allocationMoveAdapater) noexcept  {
			pathStorage = natl::move(allocationMoveAdapater);
			formatPath();
			return self();
		}

		constexpr BasePath& assign(string_type&& source) noexcept {
			return self() = natl::move(source);
		}
		constexpr BasePath& assign(const Ascii* source) noexcept {
			return self() = source;
		}

		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const Ascii>)
		constexpr BasePath& assign(const StringViewLike& source) noexcept {
			return self() = source;
		}
		constexpr BasePath& assign(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			return self() = natl::move(allocationMoveAdapater);
		}

		//path formatting 
		constexpr BasePath& formatPath(const PathFormat format = autoFormat) noexcept {
			if constexpr (getPlatformType() == ProgramPlatformType::windowsPlatform) {
				if (format == PathFormat::nativeFormat) {
					for (Ascii& character : pathStorage) {
						if (character == '/') {
							character = '\\';
						}
					}
					return self();
				}
			}

			for (Ascii& character : pathStorage) {
				if (character == '\\') {
					character = '/';
				}
			}
			return self();
		}

		//append 
		constexpr BasePath& operator/=(const const_path_view_type& other) noexcept {
			const_path_view_type otherRootName = other.rootName();
			Size otherRootNameSize = otherRootName.size();
			if (other.isAbsolute() || (otherRootName.isNotEmpty() && otherRootName != rootName())) {
				pathStorage = other.toStringView();
				return self();
			} else if (other.hasRootDirectory()) {
				BasePath rootNamePath = rootName();
				pathStorage = rootNamePath.native();
				pathStorage += other.toStringView().substr(otherRootNameSize);
				return self();
			} else if (hasFilename() || (!hasRootDirectory() && isAbsolute())) {
				pathStorage += pathSeparator;
			}
			pathStorage += other.toStringView().substr(otherRootNameSize);
			return self();
		}
		template<Size OtherBufferSize>
		constexpr BasePath& operator/=(const BasePath<OtherBufferSize>& other) noexcept {
			return self() /= other.toPathView();
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const Ascii>)
		constexpr BasePath& operator/=(const StringViewLike& source) noexcept {
			return self() /= BasePath(source);
		}
		constexpr BasePath& operator/=(const Ascii* other) noexcept {
			return self() /= const_string_view_type(other);
		}

		constexpr BasePath& append(const const_path_view_type& other) noexcept {
			return self() /= other;
		}
		template<Size OtherBufferSize>
		constexpr BasePath& append(const BasePath<OtherBufferSize>& other) noexcept {
			return self() /= other;
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const Ascii>)
		constexpr BasePath& append(const StringViewLike& source) noexcept {
			return self() /= source;
		}
		constexpr BasePath& append(const Ascii* other) noexcept {
			return self() /= other;
		}

		//iterators 
		constexpr iterator begin() noexcept { return pathStorage.begin(); }
		constexpr const_iterator begin() const noexcept { return pathStorage.begin(); }
		constexpr const_iterator cbegin() const noexcept { return pathStorage.cbegin(); }
		constexpr iterator end() noexcept { return pathStorage.end(); }
		constexpr const_iterator end() const noexcept { return pathStorage.end(); }
		constexpr const_iterator cend() const noexcept { return pathStorage.cend(); }
		constexpr reverse_iterator rbegin() noexcept { return pathStorage.rbegin(); }
		constexpr const_reverse_iterator rbegin() const noexcept { return pathStorage.rbegin(); }
		constexpr const_reverse_iterator crbegin() const noexcept { return pathStorage.crbegin(); }
		constexpr reverse_iterator rend() noexcept { return pathStorage.rend(); }
		constexpr const_reverse_iterator rend() const noexcept { return pathStorage.rend(); }
		constexpr const_reverse_iterator crend() const noexcept { return pathStorage.crend(); }

		//capacity  
		constexpr size_type size() const noexcept { return pathStorage.size(); }
		constexpr size_type length() const noexcept { return pathStorage.length(); }
		constexpr size_type max_size() const noexcept { return pathStorage.max_size(); }
		constexpr Bool empty() const noexcept { return pathStorage.empty(); }
		constexpr Bool isEmpty() const noexcept { return pathStorage.isEmpty(); }
		constexpr Bool isNotEmpty() const noexcept { return pathStorage.isNotEmpty(); }

		//format observer
		constexpr const value_type* c_str() const noexcept { return pathStorage.c_str(); }
		constexpr const string_type& native() const noexcept { return pathStorage; }
		constexpr operator string_type() const noexcept { return pathStorage; }

		//decomposition
		constexpr path_view_type rootName() noexcept { return toPathView().rootName(); }
		constexpr path_view_type rootDirectory() noexcept { return toPathView().rootDirectory(); }
		constexpr path_view_type relativePath() noexcept { return toPathView().relativePath(); }
		constexpr path_view_type parentPath() noexcept { return toPathView().parentPath(); }
		constexpr path_view_type filename() noexcept { return toPathView().filename(); }
		constexpr path_view_type stem() noexcept { return toPathView().stem(); }
		constexpr path_view_type extension() noexcept { return toPathView().extension(); }

		constexpr const_path_view_type rootName() const noexcept { return toPathView().rootName(); }
		constexpr const_path_view_type rootDirectory() const noexcept { return toPathView().rootDirectory(); }
		constexpr const_path_view_type relativePath() const noexcept { return toPathView().relativePath(); }
		constexpr const_path_view_type parentPath() const noexcept { return toPathView().parentPath(); }
		constexpr const_path_view_type filename() const noexcept { return toPathView().filename(); }
		constexpr const_path_view_type stem() const noexcept { return toPathView().stem(); }
		constexpr const_path_view_type extension() const noexcept { return toPathView().extension(); }

		//queries
		constexpr Bool hasRootName() const noexcept { return toPathView().hasRootName(); }
		constexpr Bool hasRootPath() const noexcept { return toPathView().hasRootPath(); }
		constexpr Bool hasRootDirectory() const noexcept { return toPathView().hasRootDirectory(); }
		constexpr Bool hasRelativePath() const noexcept { return toPathView().hasRelativePath(); }
		constexpr Bool hasParentPath() const noexcept { return toPathView().hasParentPath(); }
		constexpr Bool hasFilename() const noexcept { return toPathView().hasFilename(); }
		constexpr Bool hasStem() const noexcept { return toPathView().hasStem(); }
		constexpr Bool hasExtension() const noexcept { return toPathView().hasExtension(); }

		constexpr Bool doesNotHaveRootName() const noexcept { return toPathView().doesNotHaveRootName(); }
		constexpr Bool doesNotHaveRootPath() const noexcept { return toPathView().doesNotHaveRootPath(); }
		constexpr Bool doesNotHaveRootDirectory() const noexcept { return toPathView().doesNotHaveRootDirectory(); }
		constexpr Bool doesNotHaveRelativePath() const noexcept { return toPathView().doesNotHaveRelativePath(); }
		constexpr Bool doesNotHaveParentPath() const noexcept { return toPathView().doesNotHaveParentPath(); }
		constexpr Bool doesNotHaveFilename() const noexcept { return toPathView().doesNotHaveFilename(); }
		constexpr Bool doesNotHaveStem() const noexcept { return toPathView().doesNotHaveStem(); }
		constexpr Bool doesNotHaveExtension() const noexcept { return toPathView().doesNotHaveExtension(); }

		constexpr Bool isAbsolute() const noexcept { return toPathView().isAbsolute(); }
		constexpr Bool isRelative() const noexcept { return toPathView().isRelative(); }

		//modifiers 
		constexpr void clear() const noexcept { pathStorage.clear(); }
		constexpr void swap(BasePath& other) noexcept {
			pathStorage.swap(other.pathStorage);
		}
		constexpr BasePath& makePreferred() noexcept {
			return formatPath(PathFormat::nativeFormat);
		}
		constexpr BasePath& removeFilename() noexcept {
			path_view_type filenamePathView = filename();
			if(filenamePathView.isNotEmpty()) {
				pathStorage.resize(pathSeparator.size() - filenamePathView.size());
			}
			return self();
		};
		constexpr BasePath& removeExtension() noexcept {
			path_view_type extensionPathView = extension();
			if (extensionPathView.isNotEmpty()) {
				pathStorage.resize(pathSeparator.size() - extensionPathView.size());
			}
			return self();
		};

		constexpr BasePath& replaceFilename(const const_path_view_type& replacement) noexcept {
			removeFilename(); 
			return self() /= replacement;
		}
		template<Size OtherBufferSize>
		constexpr BasePath& replaceFilename(const BasePath<OtherBufferSize>& replacement) noexcept {
			removeFilename();
			return self() /= replacement;
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const Ascii>)
		constexpr BasePath& replaceFilename(const StringViewLike& replacement) noexcept {
			removeFilename();
			return self() /= replacement;
		}

		constexpr BasePath& replaceExtension(const const_path_view_type& replacement) noexcept {
			removeExtension();
			if (replacement.isNotEmpty() && replacement[0] != '.') {
				pathStorage += '.';
			}
			pathStorage += replacement.toStringView();
			return self();
		}
		template<Size OtherBufferSize>
		constexpr BasePath& replaceExtension(const BasePath<OtherBufferSize>& replacement) noexcept {
			return replaceExtension(replacement.toPathView());
		}
		template<typename StringViewLike>
			requires(IsStringViewLike<StringViewLike, const Ascii>)
		constexpr BasePath& replaceExtension(const StringViewLike& replacement) noexcept {
			return replaceFilename(BasePath(replacement));
		}

		//compare
		template<Size RhsBufferSize>
		constexpr Bool operator==(const BasePath<RhsBufferSize, Alloc>& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() == rhs.toPathView();
		}
		template<Size RhsBufferSize>
		constexpr Bool operator!=(const BasePath<RhsBufferSize, Alloc>& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() != rhs.toPathView();
		}
		template<Size RhsBufferSize>
		constexpr Bool operator<(const BasePath<RhsBufferSize, Alloc>& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() < rhs.toPathView();
		}
		template<Size RhsBufferSize>
		constexpr Bool operator<=(const BasePath<RhsBufferSize, Alloc>& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() <= rhs.toPathView();
		}
		template<Size RhsBufferSize>
		constexpr Bool operator>(const BasePath<RhsBufferSize, Alloc>& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() > rhs.toPathView();
		}
		template<Size RhsBufferSize>
		constexpr Bool operator>=(const BasePath<RhsBufferSize, Alloc>& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() >= rhs.toPathView();
		}
		template<Size RhsBufferSize>
		constexpr auto operator<=>(const BasePath<RhsBufferSize, Alloc>& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() <=> rhs.toPathView();
		}

		constexpr Bool operator==(const const_path_view_type& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() == rhs;
		}
		constexpr Bool operator!=(const const_path_view_type& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() != rhs;
		}
		constexpr Bool operator<(const const_path_view_type& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() < rhs;
		}
		constexpr Bool operator<=(const const_path_view_type& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() <= rhs;
		}
		constexpr Bool operator>(const const_path_view_type& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() > rhs;
		}
		constexpr Bool operator>=(const const_path_view_type& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() >= rhs;
		}
		constexpr auto operator<=>(const const_path_view_type& rhs) const noexcept {
			const BasePath& lhs = self();
			return lhs.toPathView() <=> rhs;
		}
	};

	template<Size ByteSize, typename Alloc = DefaultAllocator>
		requires(ByteSize >= 32 && IsAllocatorC<Alloc>)
	using PathByteSize = BasePath<ByteSize - sizeof(BaseStringBaseMembersRef<Ascii>), Alloc>;

	using Path = PathByteSize<128>;
	using Path128 = PathByteSize<128>;
	using Path256 = PathByteSize<256>;
	using Path512 = PathByteSize<512>;

	namespace literals {
		constexpr Path128 operator ""_natl_path(const char* str, StdSize len) noexcept {
			return Path128(ConstStringView{ str, static_cast<Size>(len) });
		}

		constexpr Path128 operator ""_natl_path_128(const char* str, StdSize len) noexcept {
			return Path128(ConstStringView{ str, static_cast<Size>(len) });
		}
		constexpr Path256 operator ""_natl_path_256(const char* str, StdSize len) noexcept {
			return Path256(ConstStringView{ str, static_cast<Size>(len) });
		}
		constexpr Path512 operator ""_natl_path_512(const char* str, StdSize len) noexcept {
			return Path512(ConstStringView{ str, static_cast<Size>(len) });
		}
	}

	template<Size BufferSize, typename Alloc>
	struct IsTriviallyCompareableV<BasePath<BufferSize, Alloc>>
		: IsTriviallyCompareableV<typename BasePath<BufferSize, Alloc>::string_type> {};

	template<Size BufferSize, typename Alloc>
	struct IsTriviallyRelocatableV<BasePath<BufferSize, Alloc>>
		: IsTriviallyRelocatableV<typename BasePath<BufferSize, Alloc>::string_type> {};
	template<Size BufferSize, typename Alloc>
	struct IsTriviallyConstructibleV<BasePath<BufferSize, Alloc>>
		: IsTriviallyConstructibleV<typename BasePath<BufferSize, Alloc>::string_type> {};
	template<Size BufferSize, typename Alloc>
	struct IsTriviallyDestructibleV<BasePath<BufferSize, Alloc>>
		: IsTriviallyDestructibleV<typename BasePath<BufferSize, Alloc>::string_type> {};

	template<Size BufferSize, typename Alloc>
	struct IsTriviallyConstRefConstructibleV<BasePath<BufferSize, Alloc>>
		: IsTriviallyConstRefConstructibleV<typename BasePath<BufferSize, Alloc>::string_type> {};
	template<Size BufferSize, typename Alloc>
	struct IsTriviallyMoveConstructibleV<BasePath<BufferSize, Alloc>>
		: IsTriviallyMoveConstructibleV<typename BasePath<BufferSize, Alloc>::string_type> {};

	template<Size BufferSize, typename Alloc>
	struct IsTriviallyConstRefAssignableV<BasePath<BufferSize, Alloc>>
		: IsTriviallyConstRefAssignableV<typename BasePath<BufferSize, Alloc>::string_type> {};
	template<Size BufferSize, typename Alloc>
	struct IsTriviallyMoveAssignableV<BasePath<BufferSize, Alloc>>
		: IsTriviallyMoveAssignableV<typename BasePath<BufferSize, Alloc>::string_type> {};

	enum struct FileOpenMode {
		readStart, //file must exist
		writeDestroy, //destory existing file content
		readWriteStart,
		readWriteDestory, //destory existing file content

		//appendEnd, //always appends to end
		//appendEndReadWrite //always appends to end
	};

	constexpr const Ascii* fileOpenModeToString(const FileOpenMode openMode) noexcept {
		switch (openMode) {
		case FileOpenMode::readStart:
			return "read start";
		case FileOpenMode::writeDestroy:
			return "write destory";
		case FileOpenMode::readWriteStart:
			return "read write start";
		case FileOpenMode::readWriteDestory:
			return "read write destory";
		default:
			unreachable();
		}
	}

	enum struct FileType {
		none,
		notFound,
		regular,
		directory,
		symbolicLink,
		block,
		character,
		fifo,
		unknown,
	};

	constexpr const Ascii* fileTypeToString(const FileType fileType) noexcept {
		switch (fileType) {
		case FileType::none:
			return "none";
		case FileType::notFound:
			return "not found";
		case FileType::regular:
			return "regular";
		case FileType::directory:
			return "directory";
		case FileType::symbolicLink:
			return "symbolic link";
		case FileType::block:
			return "block";
		case FileType::character:
			return "character";
		case FileType::fifo:
			return "fifo";
		case FileType::unknown:
			return "unknown";
		default:
			unreachable();
		}
	}

	//system 
#ifdef NATL_WINDOWS_PLATFORM
	using NativeFileHandle = void*;
#define NATL_NATIVE_INVALID_FILE_HANDEL_VALUE ((NativeFileHandle)(long long*) - 1)
#endif // NATL_WINDOWS_PLATFORM

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	using NativeFileHandle = GenericInt;
#define NATL_NATIVE_INVALID_FILE_HANDEL_VALUE GenericInt(-1);
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

	struct FileHandle {
	public:
		using native_handle_type = NativeFileHandle;
	private:
		mutable NativeFileHandle fileHandle;
	public:
		//constructor 
		constexpr FileHandle() noexcept : fileHandle() {}
		constexpr FileHandle(NativeFileHandle fileHandleIn) noexcept : fileHandle(fileHandleIn) {}

		//destructor 
		constexpr ~FileHandle() noexcept = default;

		//observers 
		constexpr Bool isHandleOpen() const noexcept { return fileHandle != NATL_NATIVE_INVALID_FILE_HANDEL_VALUE; }
		constexpr Bool isHandleNotOpen() const noexcept { return !isHandleOpen(); }

		//access 
		constexpr NativeFileHandle nativeHandle() const noexcept { return fileHandle; }

		//modifiers 
		constexpr void setHandleNull() noexcept { fileHandle = NATL_NATIVE_INVALID_FILE_HANDEL_VALUE; }
	};

	namespace impl {
		struct FileOffsetTag {};
		struct FileCountTag {};
	}
	using FileOffset = StrongType<Size, impl::FileOffsetTag>;
	using FileCount = StrongType<Size, impl::FileCountTag>;

	Option<Size> getFileSize(const FileHandle& file) noexcept;
	FileHandle openFile(const Ascii* filePath, const FileOpenMode openMode) noexcept;
	Bool closeFile(FileHandle& file) noexcept;
	Option<Size> readFile(const FileHandle& file, const FileOffset offset, const FileCount count, Byte* dst) noexcept;
	Option<Size> writeFile(const FileHandle& file, const FileOffset offset, const FileCount count, const Byte* src) noexcept;
	Bool doesFileExist(const Ascii* fileName) noexcept;
	Bool isBlockFile(const Ascii* fileName) noexcept;
	Bool isDirectory(const Ascii* fileName) noexcept;
	Bool isFifo(const Ascii* fileName) noexcept;
	Bool isRegularFile(const Ascii* fileName) noexcept;
	Bool isSymbolicLink(const Ascii* fileName) noexcept;
	FileType getFileType(const Ascii* fileName) noexcept;

	Option<Size> getWorkingDirectorySize() noexcept;
	Bool getWorkingDirectory(Ascii* dst, const Size dstSize) noexcept;

	template<typename PathLike>
		requires(IsDynamicArrayLike<PathLike, Ascii>)
	PathLike getWorkingDirectoryIn() noexcept {
		Option<Size> workingDirectorySize = getWorkingDirectorySize();

		if (workingDirectorySize.doesNotHaveValue()) {
			return PathLike{};
		}

		PathLike path;
		path.reserve(workingDirectorySize.value());
		path.resize(workingDirectorySize.value() - 1);

		Bool result = getWorkingDirectory(static_cast<Ascii*>(path.data()), workingDirectorySize.value());
		if (result == false) {
			return PathLike{};
		}

		return path;
	}

	struct File {
	public:
		using native_handle_type = NativeFileHandle;
	private:
		FileHandle fileHandle;
	public:
		//constructor 
		constexpr File() noexcept = default;
		constexpr File(const File&) noexcept = delete;
		constexpr File(File&& other) noexcept : fileHandle(natl::move(other.fileHandle)) {
			other.fileHandle.setHandleNull();
		}
		File(const Ascii* path, const FileOpenMode openMode) noexcept {
			open(path, openMode);
		}

		//destructor 
		constexpr ~File() noexcept { 
			if (!isConstantEvaluated()) {
				close(); 
			}
		};

		//util 
		constexpr File& self() noexcept { return *this; }
		constexpr const File& self() const noexcept { return *this; }

		//convert 
		constexpr operator FileHandle() noexcept { return handle(); }
		constexpr operator const FileHandle() const noexcept { return handle(); }

		//assignment 
		constexpr File& operator=(const File&) noexcept = delete;
		constexpr File& operator=(File&& other) noexcept {
			fileHandle = natl::move(other.fileHandle);
			other.fileHandle.setHandleNull();
			return self();
		}
		

		//observers 
		constexpr Bool isOpen() const noexcept { return fileHandle.isHandleOpen(); }
		constexpr Bool isNotOpen() const noexcept { return fileHandle.isHandleNotOpen(); }

		Option<Size> fileSize() const noexcept {
			return getFileSize(fileHandle);
		}
		constexpr FileHandle handle() noexcept { return fileHandle; }
		constexpr const FileHandle handle() const noexcept { return fileHandle; }
		
		//file operations 
		Bool open(const Ascii* path, const FileOpenMode openMode) noexcept {
			fileHandle = openFile(path, openMode);
			return fileHandle.isHandleOpen();
		}
		Bool close() noexcept {
			return closeFile(fileHandle);
		}
		Option<Size> read(const FileOffset offset, const FileCount count, Byte* dst) const noexcept {
			return readFile(fileHandle, offset, count, dst);
		}
		Option<Size> read(const FileCount count, Byte* dst) const noexcept {
			return readFile(fileHandle, FileOffset(0), count, dst);
		}
		Option<Size> write(const FileOffset offset, const FileCount count, const Byte* src) noexcept {
			return writeFile(fileHandle, offset, count, src);
		}
		Option<Size> write(const FileCount count, const Byte* src) noexcept {
			return writeFile(fileHandle, FileOffset(0), count, src);
		}
	};

	enum struct LoadAllFileContentError {
		fileNotOpen,
		getFileSizeFailed,
		fileReadFailed,
		none,
	};

	constexpr const Ascii* loadAllFileContentErrorToString(const LoadAllFileContentError error) noexcept {
		switch (error) {
		case LoadAllFileContentError::fileNotOpen:
			return "file not open";
		case LoadAllFileContentError::getFileSizeFailed:
			return "get file size failed";
		case LoadAllFileContentError::fileReadFailed:
			return "file read failed";
		case LoadAllFileContentError::none:
			return "none";
		default:
			unreachable();
		}
	}

	template<typename DynArrayLike>
		requires(IsByteDynamicArrayLike<DynArrayLike>)
	[[nodiscard]] inline LoadAllFileContentError loadAllFileContent(File& file, DynArrayLike& dstArray) noexcept {
		if (file.isNotOpen()) { return LoadAllFileContentError::fileNotOpen; }

		natl::Option<Size> fileSize = getFileSize(file);
		if (fileSize.doesNotHaveValue()) { return LoadAllFileContentError::getFileSizeFailed; }
		if (fileSize.value() == 0) { return LoadAllFileContentError::none; }

		dstArray.resize(dstArray.size() + fileSize.value());
		Byte* dst = reinterpret_cast<Byte*>(dstArray.data());
		Option<Size> fileReadSuccessful = readFile(file, FileOffset(0), FileCount(fileSize.value()), dst);
		if (!fileReadSuccessful || fileReadSuccessful.value() != fileSize.value()) { return LoadAllFileContentError::fileReadFailed; }

		return LoadAllFileContentError::none;
	}
}