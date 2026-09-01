#pragma once

//own
#include "../util/option.h"
#include "../util/bits.h"
#include "../container/bitArray.h"
#include "../container/dynArray.h"
#include "../container/array.h"

//@export
namespace natl {
    namespace impl {
        template<typename DataType, Size vPageSize>
            requires(vPageSize > 0)
        struct PagedSparseArrayPage {
        public:
            using storage_array = Array<UninitializedValue<DataType>, vPageSize>;
            BitArray<vPageSize> bitmap;
            storage_array data;
        };

        template<typename DataType, Size vPageSize, template<typename> typename PageDynArrayType, typename Alloc, typename PageIterator>
        struct PagedSparseArrayIterator {
        public:
            using iterator = PagedSparseArrayIterator;

            using allocator_type = Alloc;
            using typed_allocator_type = Alloc::template rebind<DataType>;

            using value_type = typename typed_allocator_type::value_type;
            using reference = typename typed_allocator_type::reference;
            using const_reference = typename typed_allocator_type::const_reference;
            using pointer = typename typed_allocator_type::pointer;
            using const_pointer = typename typed_allocator_type::const_pointer;
            using difference_type = typename typed_allocator_type::difference_type;
            using size_type = typename typed_allocator_type::size_type;

            using iterator_category = BidirectionalIteratorTag;

            using page = PagedSparseArrayPage<DataType, vPageSize>;
            using page_storage_array = PageDynArrayType<page*>;
            using page_iterator = PageIterator;

        private:
            page_iterator beginPageIter;
            page_iterator endPageIter;
            page_iterator pageIter;
            size_type storagePos;

        public:
            constexpr PagedSparseArrayIterator() noexcept {};
            constexpr PagedSparseArrayIterator(page_iterator beginPageIter, page_iterator endPageIter, page_iterator pageIter, size_type storagePos) noexcept
                : beginPageIter(beginPageIter), endPageIter(endPageIter), pageIter(pageIter), storagePos(storagePos) {}

        private:
            constexpr iterator& self() noexcept { return *this; }
            constexpr const iterator& self() const noexcept { return *this; }

        public:
            constexpr reference operator*() noexcept requires(IsNotConst<value_type>) {
                return (*pageIter)->data[storagePos].value();
            }
            constexpr const_reference operator*() const noexcept {
                return (*pageIter)->data[storagePos].value();
            }
            constexpr pointer operator->() noexcept requires(IsNotConst<value_type>) {
                return &(*pageIter)->data[storagePos].value();
            }
            constexpr const_pointer operator->() const noexcept {
                return &(*pageIter)->data[storagePos].value();
            }
            constexpr reference deref() noexcept requires(IsNotConst<value_type>) {
                return (*pageIter)->data[storagePos].value();
            }
            constexpr const_reference deref() const noexcept {
                return (*pageIter)->data[storagePos].value();
            }
            constexpr natl::Bool isValid() const noexcept {
                return beginPageIter == endPageIter
                    || pageIter == endPageIter
                    || (*pageIter) == nullptr
                    || (*pageIter)->bitmap.test(storagePos);
            }

        private:
            constexpr Bool inc() noexcept {
                if ((*pageIter) == nullptr) {
                    if (pageIter == endPageIter) {
                        return true;
                    }

                    pageIter++;
                    if (pageIter == endPageIter) {
                        return true;
                    }
                    storagePos = 0;
                } else {
                    storagePos++;

                    if (storagePos >= vPageSize) {
                        pageIter++;
                        if (pageIter == endPageIter) {
                            storagePos = 0;
                            return true;
                        }
                        storagePos = 0;
                    }
                }

                if ((*pageIter) == nullptr) {
                    return false;
                }

                return (*pageIter)->bitmap.test(storagePos);
            }
            constexpr Bool dec() noexcept {
                if ((*pageIter) == nullptr) {
                    if (pageIter == beginPageIter) {
                        return true;
                    }
                    pageIter--;
                    storagePos = 0;
                } else {
                    if (storagePos == 0) {
                        if (pageIter == beginPageIter) {
                            return true;
                        }
                        pageIter--;
                        storagePos = 0;
                    } else {
                        storagePos--;
                    }
                }

                if ((*pageIter) == nullptr) {
                    return false;
                }

                return (*pageIter)->bitmap.test(storagePos);
            }

        public:

            constexpr iterator& operator++() noexcept {
                while (!inc()) {}
                return self();
            }
            constexpr iterator operator++(int) noexcept {
                iterator tempIt = self();
                ++self();
                return tempIt;
            }
            constexpr iterator& operator--() noexcept {
                while (!dec()) {}
                return self();
            }
            constexpr iterator operator--(int) noexcept {
                iterator tempIt = self();
                --self();
                return tempIt;
            }

            constexpr Bool operator== (const iterator rhs) const noexcept {
                return pageIter == rhs.pageIter && storagePos == rhs.storagePos;
            }
            constexpr Bool operator!= (const iterator rhs) const noexcept {
                return pageIter != rhs.pageIter || storagePos != rhs.storagePos;
            }
            constexpr Bool operator<(const iterator rhs) const noexcept {
                if (pageIter == rhs.pageIter) {
                    return storagePos < rhs.storagePos;
                } else {
                    return pageIter < rhs;
                }
            }
            constexpr Bool operator>(const iterator rhs) const noexcept {
                if (pageIter == rhs.pageIter) {
                    return storagePos > rhs.storagePos;
                } else {
                    return pageIter > rhs;
                }
            }
            constexpr Bool operator<=(const iterator rhs) const noexcept {
                if (pageIter == rhs.pageIter) {
                    return storagePos <= rhs.storagePos;
                } else {
                    return pageIter <= rhs;
                }
            }
            constexpr Bool operator>=(const iterator rhs) const noexcept {
                if (pageIter == rhs.pageIter) {
                    return storagePos >= rhs.storagePos;
                } else {
                    return pageIter >= rhs;
                }
            }
        };

        template<typename DataType, Size vPageSize, template<typename> typename PageDynArrayType, typename Alloc>
            requires(vPageSize > 0 && IsAllocatorC<Alloc>)
        struct BasePagedSparseArray {
        public:
            using allocator_type = Alloc;
            using typed_allocator_type = Alloc::template rebind<DataType>;

            using value_type = typename typed_allocator_type::value_type;
            using reference = typename typed_allocator_type::reference;
            using const_reference = typename typed_allocator_type::const_reference;
            using pointer = typename typed_allocator_type::pointer;
            using const_pointer = typename typed_allocator_type::const_pointer;
            using difference_type = typename typed_allocator_type::difference_type;
            using size_type = typename typed_allocator_type::size_type;

            constexpr static Size pageSize = vPageSize;

            using page_type = PagedSparseArrayPage<DataType, vPageSize>;
            using page_storage_array = PageDynArrayType<page_type*>;
            using page_allocator = allocator_type::template rebind<page_type>;

            using iterator = PagedSparseArrayIterator<DataType, vPageSize, PageDynArrayType, Alloc, typename page_storage_array::iterator>;
            using const_iterator = PagedSparseArrayIterator<DataType, vPageSize, PageDynArrayType, Alloc, typename page_storage_array::const_iterator>;;
            using reverse_iterator = ReverseIterator<iterator>;
            using const_reverse_iterator = ReverseIterator<const_iterator>;

        private:
            size_type count;
            page_storage_array pages;

        private:
            constexpr BasePagedSparseArray& self() noexcept { return *this; }
            constexpr const BasePagedSparseArray& self() const noexcept { return *this; }

        public:
            constexpr BasePagedSparseArray() noexcept : pages(), count(0) {}
            constexpr BasePagedSparseArray(const BasePagedSparseArray& other) noexcept : pages(), count(0) {
                assign(other);
            };
            constexpr BasePagedSparseArray(BasePagedSparseArray&& other) noexcept : pages(), count(0) {
                assign(forward<BasePagedSparseArray>(other));
            };
            constexpr ~BasePagedSparseArray() noexcept {
                clear();
            }

            //assignment
            constexpr BasePagedSparseArray& operator=(const BasePagedSparseArray& other) noexcept {
                return assign(other);
            };
            constexpr BasePagedSparseArray& operator=(BasePagedSparseArray& other) noexcept {
                return assign(other);
            };

            constexpr BasePagedSparseArray& assign(const BasePagedSparseArray& other) noexcept {
                clear();
                count = other.count;

                pages.resize(other.pages.size());
                for (size_type i = 0; i < other.pages.size(); i++) {
                    page_type*& newPage = pages[i];
                    page_type *const& copyPage = other.pages[i];

                    if (copyPage == nullptr) {
                        newPage = nullptr;
                        continue;
                    }

                    newPage = createPage();
                    newPage->bitmap = copyPage->bitmap;

                    if (copyPage->bitmap.any()) {
                        for (natl::Size j = 0; j < vPageSize; j++) {
                            if (copyPage->bitmap.test(j)) {
                                newPage->data[j].construct(copyPage->data[j].value());
                            }
                        }
                    }
                }

                return self();
            }

            constexpr BasePagedSparseArray& assign(BasePagedSparseArray&& other) noexcept {
                clear();
                pages = move(other.pages);
                count = other.count;
                other.count = 0;
                return self();
            }

            //iterators
            constexpr iterator begin() noexcept {
                auto iter = iterator(pages.begin(), pages.end(), pages.begin(), 0);
                if (!iter.isValid()) {
                    iter++;
                }
                return iter;
            }
            constexpr const_iterator begin() const noexcept {
                auto iter = const_iterator(pages.begin(), pages.end(), pages.begin(), 0);
                if (!iter.isValid()) {
                    iter++;
                }
                return iter;
            }
            constexpr const_iterator cbegin() const noexcept { return begin(); }
            constexpr iterator end() noexcept { return iterator(pages.begin(), pages.end(), pages.end(), 0); }
            constexpr const_iterator end() const noexcept { return const_iterator(pages.begin(), pages.end(), pages.end(), 0); }
            constexpr const_iterator cend() const noexcept { return end(); }
            constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
            constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
            constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
            constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
            constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
            constexpr const_reverse_iterator crend() const noexcept { return rend(); }

            //capacity
            constexpr size_type size() const noexcept { return count; }
            constexpr Bool empty() const noexcept { return size() == 0; }
            constexpr Bool isEmpty() const noexcept { return empty(); }
            constexpr Bool isNotEmpty() const noexcept { return !empty(); }

            //element access
            constexpr OptionPtr<value_type> get(size_type index) noexcept {
                size_type pageIndex = index / vPageSize;
                size_type offset = index % vPageSize;
                if (pageIndex >= pages.size() || !pages[pageIndex]) {
                    return OptionEmpty{};
                }
                page_type& page = *pages[pageIndex];
                if (!page.bitmap.test(offset)) {
                    return OptionEmpty{};
                }
                return &page.data[offset].value();
            }

            constexpr OptionPtr<const value_type> get(size_type index) const noexcept {
                size_type pageIndex = index / vPageSize;
                size_type offset = index % vPageSize;
                if (pageIndex >= pages.size() || !pages[pageIndex]) {
                    return OptionEmpty{};
                }
                page_type& page = *pages[pageIndex];
                if (!page.bitmap.test(offset)) {
                    return OptionEmpty{};
                }
                return &page.data[offset].value();
            }

            constexpr iterator getIter(size_type index) noexcept {
                size_type pageIndex = index / vPageSize;
                size_type offset = index % vPageSize;
                if (pageIndex >= pages.size() || !pages[pageIndex]) {
                    return end();
                }
                page_type& page = *pages[pageIndex];
                if (!page.bitmap.test(offset)) {
                    return end();
                }
                return iterator(pages.begin(), pages.end(), pages.begin() + pageIndex, offset);
            }

            constexpr const_iterator getIter(size_type index) const noexcept {
                size_type pageIndex = index / vPageSize;
                size_type offset = index % vPageSize;
                if (pageIndex >= pages.size() || !pages[pageIndex]) {
                    return false;
                }
                page_type& page = *pages[pageIndex];
                if (!page.bitmap.test(offset)) {
                    return end();
                }
                return const_iterator(pages.begin(), pages.end(), pages.begin() + pageIndex, offset);
            }

        private:
            constexpr static page_type* createPage() noexcept {
                page_type* page = page_allocator::allocate(1);
                construct(page);
                return page;
            }
            constexpr static void destoryPage(page_type* page) noexcept {
                if constexpr (!IsTriviallyDestructibleC<DataType>) {
                    if (page->bitmap.any()) {
                        for (Size i = 0; i < vPageSize; i++) {
                            if (page->bitmap.test(i)) {
                                page->data[i].deconstruct();
                            }
                        }
                    }
                }

                deconstruct(page);
                page_allocator::deallocate(page, 1);
            }

        public:
            //modifiers
            constexpr iterator insert(size_type index, const value_type& value) noexcept {
                size_t pageIndex = index / vPageSize;
                size_t offset = index % vPageSize;

                if (pageIndex >= pages.size()) {
                    pages.resize(pageIndex + 1);
                }
                if (!pages[pageIndex]) {
                    pages[pageIndex] = createPage();
                }

                page_type& page = *pages[pageIndex];
                if (page.bitmap.test(offset)) {
                    page.data[offset].value() = value;
                } else {
                    count++;
                    page.bitmap.set(offset);
                    page.data[offset].construct(value);
                }

                return iterator(pages.begin(), pages.end(), pages.begin() + pageIndex, offset);
            }

            constexpr iterator insert(size_type index, value_type&& value) noexcept {
                size_t pageIndex = index / vPageSize;
                size_t offset = index % vPageSize;

                if (pageIndex >= pages.size()) {
                    pages.resize(pageIndex + 1);
                }
                if (!pages[pageIndex]) {
                    pages[pageIndex] = createPage();
                }

                page_type& page = *pages[pageIndex];
                if (page.bitmap.test(offset)) {
                    page.data[offset].value() = forward<value_type>(value);
                } else {
                    count++;
                    page.bitmap.set(offset);
                    page.data[offset].construct(forward<value_type>(value));
                }

                return iterator(pages.begin(), pages.end(), pages.begin() + pageIndex, offset);
            }

            constexpr natl::Bool erase(size_type index) noexcept {
                size_type pageIndex = index / vPageSize;
                size_type offset = index % vPageSize;
                if (pageIndex >= pages.size() || !pages[pageIndex]) {
                    return false;
                }
                page_type& page = *pages[pageIndex];
                if (!page.bitmap.test(offset)) {
                    return false;
                }

                count--;
                page.bitmap.reset(offset);
                page.data[offset].deconstruct();
                return true;
            }

            constexpr void clear() noexcept {
                count = 0;

                for (page_type* page : pages) {
                    if (page != nullptr) {
                        destoryPage(page);
                    }
                }
                pages.clear();
            }
        };
    }


    constexpr inline natl::Size defaultPagedSparseArrayPageSize = 1024;

    template<typename DataType, Size vPageSize = defaultPagedSparseArrayPageSize, typename Alloc = DefaultAllocator>
        requires(vPageSize > 0 && IsAllocatorC<Alloc>)
    using PagedSparseArray = impl::BasePagedSparseArray<DataType, vPageSize, DynArrayUnboundTypeT<Alloc>::template type, Alloc>;

    template<typename DataType, Size bufferSize, Size vPageSize = defaultPagedSparseArrayPageSize, typename Alloc = DefaultAllocator>
        requires(vPageSize > 0 && IsAllocatorC<Alloc>)
    using SmallPagedSparseArray = impl::BasePagedSparseArray<DataType, vPageSize, SmallDynArrayUnboundTypeT<bufferSize, Alloc>::template type, Alloc>;
}