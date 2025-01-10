#pragma once

//own
#include "../fundamental/tuple.h"
#include "basicTypes.h"
#include "typeTraits.h"
#include "dataMovement.h"

//interface
namespace natl {
    struct RepeatIterator {
    public:
        using iterator_concept = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Size;
        using difference_type = PtrDiff;
        using pointer = Size*;
        using reference = Size&;
        using const_reference = const Size&;
        using size_type = Size;
    public:
        Size index;
    public:
        //constructor 
        constexpr RepeatIterator() noexcept : index(0) {}
        constexpr RepeatIterator(const Size indexIn) noexcept : index(indexIn) {}

        //util 
        constexpr RepeatIterator& self() noexcept { return *this; }
        constexpr const RepeatIterator& self() const noexcept { return *this; }

        //element access
        constexpr reference operator*() noexcept { return index; }
        constexpr const_reference operator*() const noexcept { return index; }
        constexpr const_reference operator->() const noexcept { return index; }
        constexpr reference operator[](const size_type index) = delete;

        //compare
        constexpr Bool operator==(const RepeatIterator rhs) const noexcept { return index == rhs.index; }
        constexpr Bool operator!=(const RepeatIterator rhs) const noexcept { return index != rhs.index; }
        constexpr Bool operator<(const RepeatIterator rhs) const noexcept { return index < rhs.index; }
        constexpr Bool operator>(const RepeatIterator rhs) const noexcept { return index > rhs.index; }
        constexpr Bool operator<=(const RepeatIterator rhs) const noexcept { return index <= rhs.index; }
        constexpr Bool operator>=(const RepeatIterator rhs) const noexcept { return index >= rhs.index; }


        //iterator operations 
        constexpr RepeatIterator& operator++() noexcept {
            index++;
            return self();
        }

        constexpr RepeatIterator operator++(int) noexcept {
            RepeatIterator tempIt = self();
            ++self();
            return tempIt;
        }
        constexpr RepeatIterator& operator--() noexcept {
            index--;
            return self();
        }

        constexpr RepeatIterator operator--(int) noexcept {
            RepeatIterator tempIt = self();
            --self();
            return tempIt;
        }
        constexpr RepeatIterator& operator+=(const size_type offset) noexcept {
            index += offset;
            return self();
        }
        constexpr RepeatIterator operator+(const size_type offset) const noexcept {
            RepeatIterator tempIt = self();
            tempIt += offset;
            return tempIt;
        }
        constexpr friend RepeatIterator operator+(const size_type offset, RepeatIterator& rhs) noexcept {
            rhs += offset;
            return rhs;
        }
        constexpr RepeatIterator& operator-=(const size_type offset) noexcept {
            index -= offset;
            return self();
        }
        constexpr RepeatIterator operator-(const size_type offset) const noexcept {
            RepeatIterator tempIt = self();
            tempIt -= offset;
            return tempIt;
        }
    };

    struct Repeat {
    public:
        using value_type = Size;
        using reference = Size&;
        using const_reference = const Size&;
        using pointer = Size*;
        using const_pointer = const Size*;
        using difference_type = PtrDiff;
        using size_type = Size;

        using iterator = RepeatIterator;
        using const_iterator = RepeatIterator;
        using reverse_iterator = RepeatIterator;
        using const_reverse_iterator = RepeatIterator;
    private:
        Size repeatCount;
    public:
        //constructor
        constexpr Repeat() noexcept : repeatCount(0) {}
        constexpr Repeat(const Size count) noexcept : repeatCount(count) {}

        //destructor
        constexpr ~Repeat() noexcept = default;

        //capacity 
        constexpr Size size() const noexcept { return repeatCount; }
        constexpr size_type beginIndex() const noexcept { return 0; };
        constexpr size_type endIndex() const noexcept { return repeatCount; };

        //iterators 
        constexpr const_iterator begin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator end() const noexcept { return const_iterator(endIndex()); }
        constexpr const_iterator cend() const noexcept { return const_iterator(endIndex()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
    };

    struct RepeatFromTo {
    public:
        using value_type = Size;
        using reference = Size&;
        using const_reference = const Size&;
        using pointer = Size*;
        using const_pointer = const Size*;
        using difference_type = PtrDiff;
        using size_type = Size;

        using iterator = RepeatIterator;
        using const_iterator = RepeatIterator;
        using const_reverse_iterator = RepeatIterator;
    private:
        Size repeatFrom;
        Size repeatTo;
    public:
        //constructor
        constexpr RepeatFromTo() noexcept : repeatFrom(0), repeatTo(0) {}
        constexpr RepeatFromTo(const Size repeatFromIn, const Size repeatToIn) noexcept : repeatFrom(repeatFromIn), repeatTo(repeatToIn) {}
        constexpr RepeatFromTo(const Size count) noexcept : repeatFrom(0), repeatTo(count) {}

        //destructor 
        constexpr ~RepeatFromTo() noexcept = default;

        //capacity 
        constexpr Size size() const noexcept { return repeatTo - repeatFrom; }
        constexpr size_type beginIndex() const noexcept { return repeatFrom; };
        constexpr size_type endIndex() const noexcept { return repeatTo + 1; };

        //iterators 
        constexpr const_iterator begin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator end() const noexcept { return const_iterator(endIndex()); }
        constexpr const_iterator cend() const noexcept { return const_iterator(endIndex()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
    };

    namespace impl {
        template<typename IterationRange>
        struct IteratorActionPreIncrement {
            constexpr void operator()(RemoveReference<IterationRange>& iterationRange) noexcept {
                ++iterationRange;
            }
        };

        template<template<typename Iterators> typename IteratorAction, typename... Iterators>
        struct CallActionOnIteratorsStructFunc {
            constexpr void operator()(RemoveReference<Iterators>&... iterationRanges) noexcept {
                (IteratorAction<Iterators>{}(iterationRanges), ...);
            }
        };

        template<typename Iterator> 
        struct IteratorValueAccessDerefStructFunc {
            constexpr typename Iterator::value_type operator()(Iterator iterator) noexcept {
                return *iterator;
            }
        };

        template<template<typename Iterator> typename IteratorValueAccess, typename... Iterators>
        struct IteratorsToValue {
            constexpr Tuple<typename Iterators::value_type...> operator()(Iterators... iterators) noexcept {
                using value_storage = Tuple<typename Iterators::value_type...>;
                return value_storage(IteratorValueAccess<Iterators>{}(iterators)...);
            }
        };
    }

    template<typename... Types>
    struct CombinedIterationIterator {};

    template<typename... DataTypes, typename... IterationRanges>
    struct CombinedIterationIterator<TypePack<DataTypes...>, TypePack<IterationRanges...>> {
    public:
        //using value_type = Tuple<typename IterationRanges::iterator::value_type...>;
        using value_type = Tuple<DataTypes...>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = PtrDiff;
        using size_type = Size;
    private:
        Tuple<typename IterationRanges::iterator...> iterators;
    public:
        //constructor 
        constexpr CombinedIterationIterator() noexcept = default;
        constexpr CombinedIterationIterator(typename IterationRanges::iterator... iteratorsIn) noexcept : iterators(iteratorsIn...) {}
        constexpr CombinedIterationIterator(const Tuple<typename IterationRanges::iterator...>& iteratorsIn) noexcept : iterators(iteratorsIn) {}

        //destructor 
        constexpr ~CombinedIterationIterator() noexcept = default;

        //util 
        constexpr CombinedIterationIterator& self() noexcept { return *this; }
        constexpr const CombinedIterationIterator& self() const noexcept { return *this; }

        //element access 
        constexpr value_type operator*() noexcept { 
            return callFunctionWithTuple(
                impl::IteratorsToValue<impl::IteratorValueAccessDerefStructFunc, typename IterationRanges::iterator...>{},
                iterators);
        }
        constexpr value_type operator*() const noexcept { 
            return callFunctionWithTuple(
                impl::IteratorsToValue<impl::IteratorValueAccessDerefStructFunc, typename IterationRanges::iterator...>{},
                iterators);
        }

        //compare
        constexpr Bool operator==(const CombinedIterationIterator& rhs) const noexcept { return iterators == rhs.iterators; }
        constexpr Bool operator!=(const CombinedIterationIterator& rhs) const noexcept { return iterators != rhs.iterators; }
        constexpr Bool operator<(const CombinedIterationIterator& rhs) const noexcept { return iterators < rhs.iterators; }
        constexpr Bool operator>(const CombinedIterationIterator& rhs) const noexcept { return iterators > rhs.iterators; }
        constexpr Bool operator<=(const CombinedIterationIterator& rhs) const noexcept { return iterators <= rhs.iterators; }
        constexpr Bool operator>=(const CombinedIterationIterator& rhs) const noexcept { return iterators >= rhs.iterators; }

        //iterator operations
        constexpr CombinedIterationIterator& operator++() noexcept {
            callFunctionWithTuple(
                impl::CallActionOnIteratorsStructFunc<impl::IteratorActionPreIncrement, typename IterationRanges::iterator...>{},
                iterators);
            return self();
        }
    };

    namespace impl {
        template<typename IterationRange>
        using CombindedIterationIterationRangeStorageType =
            Conditional<!IsLValueReference<IterationRange>,
                RemoveReference<IterationRange>,
                RemoveReference<IterationRange>&
        >;
        template<typename IterationRange> 
        struct IteratorAccessBegin {
            constexpr typename IterationRange::iterator operator()(RemoveReference<IterationRange>& iterationRange) noexcept {
                return iterationRange.begin();
            }
        };
        template<typename IterationRange>
        struct IteratorAccessEnd {
            constexpr typename IterationRange::iterator operator()(RemoveReference<IterationRange>& iterationRange) noexcept {
                return iterationRange.end();
            }
        };
        template<template<typename IterationRange> typename IteratorAccess, typename... IterationRanges>
        struct IterationRangesToIteratorsStructFunc {
            constexpr Tuple<typename IterationRanges::iterator...> operator()(RemoveReference<IterationRanges>... iterationRanges) noexcept {
                using iterator_storage = Tuple<typename IterationRanges::iterator...>;
                return iterator_storage(IteratorAccess<IterationRanges>{}(iterationRanges)...);
            }
        };
    }
    
    template<typename... Types>
    struct CombinedIteration {};

    template<typename... DataTypes, typename... IterationRanges>
        requires((sizeof...(DataTypes) == sizeof...(IterationRanges)) && (IsConvertible<DataTypes, IterationRanges> && ...))
    struct CombinedIteration<TypePack<DataTypes...>, TypePack<IterationRanges...>> {
    public:
        //using value_type = Tuple<typename IterationRanges::iterator::value_type...>;
        using value_type = Tuple<DataTypes...>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = PtrDiff;
        using size_type = Size;

        using iterator = CombinedIterationIterator<TypePack<DataTypes...>, TypePack<IterationRanges...>>;
        using const_iterator = CombinedIterationIterator<TypePack<const DataTypes...>, TypePack<const IterationRanges...>>;
    private:
        Tuple<impl::CombindedIterationIterationRangeStorageType<IterationRanges>...> iterationRanges;
    public:
        //constructor 
        constexpr CombinedIteration() noexcept = default;
        constexpr CombinedIteration(impl::CombindedIterationIterationRangeStorageType<IterationRanges>... iterationRangesIn) noexcept : iterationRanges(iterationRangesIn...) {}

        //destructor 
        constexpr ~CombinedIteration() noexcept = default;

        //iterators 
        constexpr iterator begin() noexcept {
            return iterator(
                callFunctionWithTuple(
                    impl::IterationRangesToIteratorsStructFunc<impl::IteratorAccessBegin, IterationRanges...>{}, 
                    iterationRanges)
            );
        }
        constexpr const_iterator begin() const noexcept {
            return const_iterator(
                callFunctionWithTuple(
                    impl::IterationRangesToIteratorsStructFunc<impl::IteratorAccessBegin, IterationRanges...>{},
                    iterationRanges)
            );
        }
        constexpr const_iterator cbegin() const noexcept {
            return begin();
        }
        constexpr iterator end() noexcept {
            return iterator(
                callFunctionWithTuple(
                    impl::IterationRangesToIteratorsStructFunc<impl::IteratorAccessEnd, IterationRanges...>{}, 
                    iterationRanges)
            );
        }
        constexpr const_iterator end() const noexcept {
            return const_iterator(
                callFunctionWithTuple(
                    impl::IterationRangesToIteratorsStructFunc<impl::IteratorAccessEnd, IterationRanges...>{},
                    iterationRanges)
            );
        }
        constexpr const_iterator cend() const noexcept {
            return end();
        }

        /*
        constexpr const_iterator begin() const noexcept { return const_iterator(beginIndex()); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(beginIndex()); }
        constexpr iterator end() const noexcept { return const_iterator(endIndex()); }
        constexpr const_iterator end() const noexcept { return const_iterator(endIndex()); }
        constexpr const_iterator cend() const noexcept { return const_iterator(endIndex()); }
        constexpr iterator rbegin() noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr iterator rend() noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
        */
    };

    template<typename... DataTypes>
    constexpr auto makeCombindedIteration(auto... iterationRanges) noexcept {
        using combined_iteration_type = CombinedIteration<TypePack<DataTypes...>, TypePack<decltype(iterationRanges)...>>;
        return combined_iteration_type(iterationRanges...);
    }

    template<typename IterationRange> 
    struct ReverseIteration {
    public:
        using value_type = IterationRange::value_type;
        using reference = IterationRange::reference;
        using const_reference = IterationRange::const_reference;
        using pointer = IterationRange::pointer;
        using const_pointer = IterationRange::const_pointer;
        using difference_type = IterationRange::difference_type;
        using size_type = IterationRange::size_type;

        using iterator = IterationRange::reverse_iterator;
        using const_iterator = IterationRange::const_reverse_iterator;
        using reverse_iterator = IterationRange::iterator;
        using const_reverse_iterator = IterationRange::const_iterator;
    private:
        IterationRange& iterationRangeRef;
    public:
        //constructor
        constexpr ReverseIteration(IterationRange& iterationRange) noexcept : iterationRangeRef(iterationRange) {};

        //destructor
        constexpr ~ReverseIteration() noexcept = default;

        //iterators 
        constexpr iterator begin() noexcept requires(IsNotConst<IterationRange>) { return iterationRangeRef.rbegin(); }
        constexpr const_iterator begin() const noexcept { return iterationRangeRef.rbegin(); }
        constexpr const_iterator cbegin() const noexcept { return iterationRangeRef.rbegin(); }
        constexpr iterator end() noexcept requires(IsNotConst<IterationRange>) { return iterationRangeRef.rend(); }
        constexpr const_iterator end() const noexcept { return iterationRangeRef.rend(); }
        constexpr const_iterator cend() const noexcept { return iterationRangeRef.rend(); }

        constexpr reverse_iterator rbegin() noexcept requires(IsNotConst<IterationRange>) { return iterationRangeRef.begin(); }
        constexpr const_reverse_iterator rbegin() const noexcept { return iterationRangeRef.begin(); }
        constexpr const_reverse_iterator crbegin() const noexcept { return iterationRangeRef.begin(); }
        constexpr reverse_iterator rend() noexcept requires(IsNotConst<IterationRange>) { return iterationRangeRef.end(); }
        constexpr const_reverse_iterator rend() const noexcept { return iterationRangeRef.end(); }
        constexpr const_reverse_iterator crend() const noexcept { return iterationRangeRef.end(); }
    };

    template<typename IterationRange>
    constexpr ReverseIteration<IterationRange> makeReverseIteration(IterationRange& iterationRange) noexcept {
        return ReverseIteration<IterationRange>(iterationRange);
    }

    template<typename Container>
    struct BackInsertIterator {
    public:
        using allocator_type = Container::allocator_type;

        using value_type = typename allocator_type::value_type;
        using reference = typename allocator_type::reference;
        using const_reference = typename allocator_type::const_reference;
        using pointer = typename allocator_type::pointer;
        using const_pointer = typename allocator_type::const_pointer;
        using difference_type = typename allocator_type::difference_type;
        using size_type = typename allocator_type::size_type;

        using iterator_category = std::output_iterator_tag;
    private:
        Container* container;
    public:
        //constructor
        explicit constexpr BackInsertIterator(Container& containerIn) noexcept : container(&containerIn) {}
        explicit constexpr BackInsertIterator(Container* containerIn) noexcept : container(containerIn) {}

        //destructor 
        constexpr ~BackInsertIterator() noexcept = default;

        //util 
        constexpr BackInsertIterator& self() { return *this; }
        constexpr const BackInsertIterator& self() const { return *this; }

        //modifiers 
        constexpr void reserve(const size_type newCapacity) noexcept {
            if constexpr (HasReserve<Container>) {
                container->reserve(newCapacity);
            }
        }

        //iterator operation 
        constexpr BackInsertIterator& operator=(const value_type& value) noexcept {
            container->pushBack(value);
            return self();
        }
        constexpr BackInsertIterator& operator=(value_type&& value) noexcept {
            container->pushBack(natl::move(value));
            return self();
        }

        //no op 
        constexpr BackInsertIterator& operator*() noexcept { return self(); }
        constexpr BackInsertIterator& operator++() noexcept { return self(); };
        constexpr BackInsertIterator& operator++(int) noexcept { return self(); };
    };

    template<typename Container>
    constexpr BackInsertIterator<Container> backInserter(Container& container) noexcept {
        return BackInsertIterator<Container>(&container);
    }
    

    namespace impl {
        template<typename DataType>
        struct TypeErasedBackInsertIteratorData {
        public:
            using reserve_function_type = void(*)(void*, const Size) noexcept;
            using push_back_function_type = void(*)(void*, const DataType&) noexcept;
            using push_back_move_function_type = void(*)(void*, DataType&&) noexcept;

            void* container;
            push_back_function_type pushBackFunction;
            push_back_move_function_type pushBackMoveFunction;
            reserve_function_type reserveFunction;
        };

        template<typename DataType, typename Container>
        void populateTypeErasedBackInsertIteratorData(TypeErasedBackInsertIteratorData<DataType>& dataDst, Container* containerIn) noexcept {
            dataDst.container = reinterpret_cast<void*>(containerIn);
            dataDst.pushBackFunction = [](void* containerTypeErasedPtr, const DataType& value) noexcept -> void {
                Container* containerPtr = reinterpret_cast<Container*>(containerTypeErasedPtr);
                containerPtr->pushBack(value);
            };
            dataDst.pushBackMoveFunction = [](void* containerTypeErasedPtr, DataType&& value) noexcept -> void {
                Container* containerPtr = reinterpret_cast<Container*>(containerTypeErasedPtr);
                containerPtr->pushBack(natl::move(value));
            };
            dataDst.reserveFunction = [](void* containerTypeErasedPtr, const Size newCapacity) noexcept -> void {
                Container* containerPtr = reinterpret_cast<Container*>(containerTypeErasedPtr);
                if (HasReserve<DataType>) {
                    containerPtr->reserve(newCapacity);
                }
            };
        }


        template <typename DataType>
        struct TypeErasedBackInsertIteratorDataConstexprPolymorphic {
        public:
            using reserve_function_type = void(*)(TypeErasedBackInsertIteratorDataConstexprPolymorphic*, const Size);
            using push_back_function_type = void(*)(TypeErasedBackInsertIteratorDataConstexprPolymorphic*, const DataType&);
            using push_back_move_function_type = void(*)(TypeErasedBackInsertIteratorDataConstexprPolymorphic*, DataType&&);
            using copy_function_type = TypeErasedBackInsertIteratorDataConstexprPolymorphic * (*)(TypeErasedBackInsertIteratorDataConstexprPolymorphic*);
            using destory_function_type = void(*)(TypeErasedBackInsertIteratorDataConstexprPolymorphic*);

            constexpr virtual reserve_function_type getReserveFunction() noexcept = 0;
            constexpr virtual push_back_function_type getPushBackFunction() noexcept = 0;
            constexpr virtual push_back_move_function_type getPushBackMoveFunction() noexcept = 0;
            constexpr virtual copy_function_type getCopyFunction() noexcept = 0;
            constexpr virtual destory_function_type getDestoryFunction() noexcept = 0;
        };

        template<typename DataType, typename Container>
        struct TypeErasedBackInsertIteratorDataConstexpr final : public TypeErasedBackInsertIteratorDataConstexprPolymorphic<DataType> {
        public:
            using allocator_type = DefaultAllocator<TypeErasedBackInsertIteratorDataConstexpr>;
            using data_constexpr_polymorphic = TypeErasedBackInsertIteratorDataConstexprPolymorphic<DataType>;
            using reserve_function_type = data_constexpr_polymorphic::reserve_function_type;
            using push_back_function_type = data_constexpr_polymorphic::push_back_function_type;
            using push_back_move_function_type = data_constexpr_polymorphic::push_back_move_function_type;
            using copy_function_type = data_constexpr_polymorphic::copy_function_type;
            using destory_function_type = data_constexpr_polymorphic::destory_function_type;

            Container* container;

            constexpr TypeErasedBackInsertIteratorDataConstexpr(Container* containerIn) noexcept : container(containerIn) {}

            constexpr static data_constexpr_polymorphic* createNew(Container* newContainer) noexcept {
                TypeErasedBackInsertIteratorDataConstexpr* newData = allocator_type::allocate(1);
                construct(newData, newContainer);
                return static_cast<data_constexpr_polymorphic*>(newData);;
            }
            constexpr reserve_function_type getReserveFunction() noexcept override {
                return [](data_constexpr_polymorphic* dataPolymorphic, const Size newCapacity) noexcept -> void {
                    TypeErasedBackInsertIteratorDataConstexpr* data = static_cast<TypeErasedBackInsertIteratorDataConstexpr*>(dataPolymorphic);
                    if constexpr (HasReserve<Container>) {
                        data->container->reserve(newCapacity);
                    }
                };
            };
            constexpr push_back_function_type getPushBackFunction() noexcept override {
                return [](data_constexpr_polymorphic* dataPolymorphic, const DataType& value) noexcept -> void {
                    TypeErasedBackInsertIteratorDataConstexpr* data = static_cast<TypeErasedBackInsertIteratorDataConstexpr*>(dataPolymorphic);
                    data->container->pushBack(value);
                };
            };
            constexpr push_back_move_function_type getPushBackMoveFunction() noexcept override {
                return [](data_constexpr_polymorphic* dataPolymorphic, DataType&& value) noexcept -> void {
                    TypeErasedBackInsertIteratorDataConstexpr* data = static_cast<TypeErasedBackInsertIteratorDataConstexpr*>(dataPolymorphic);
                    data->container->pushBack(natl::move(value));
                };
            };
            constexpr copy_function_type getCopyFunction() noexcept override {
                return [](data_constexpr_polymorphic* dataPolymorphic) noexcept -> data_constexpr_polymorphic* {
                    TypeErasedBackInsertIteratorDataConstexpr* data = static_cast<TypeErasedBackInsertIteratorDataConstexpr*>(dataPolymorphic);
                    return createNew(data->container);
                };
            };
            constexpr destory_function_type getDestoryFunction() noexcept override {
                return [](data_constexpr_polymorphic* dataPolymorphic) noexcept -> void {
                    TypeErasedBackInsertIteratorDataConstexpr* data = static_cast<TypeErasedBackInsertIteratorDataConstexpr*>(dataPolymorphic);
                    deconstruct(data);
                    allocator_type::deallocate(data, 1);
                };
            }
        };

        template<typename DataType, typename Container>
        constexpr void populateTypeErasedBackInsertIteratorDataConstexpr(
            TypeErasedBackInsertIteratorDataConstexprPolymorphic<DataType>*& dataPtrDst,
            Container* container) noexcept {
            using iterator_data_constexpr = TypeErasedBackInsertIteratorDataConstexpr<DataType, Container>;
            dataPtrDst = iterator_data_constexpr::createNew(container);
        }

    }


    template<typename DataType>
    struct TypeErasedBackInsertIterator {
    public:
        using value_type = DataType;
        using reference = DataType&;
        using const_reference = const DataType&;
        using pointer = DataType*;
        using const_pointer = const DataType*;
        using difference_type = PtrDiff;
        using size_type = Size;

        using iterator_category = std::output_iterator_tag;

        using iterator_data = impl::TypeErasedBackInsertIteratorData<DataType>;
        using iterator_data_constexpr_polymorphic = impl::TypeErasedBackInsertIteratorDataConstexprPolymorphic<DataType>;
    private:
        union {
            iterator_data data;
            iterator_data_constexpr_polymorphic* dataConstexprPolymorphic;
        };
    public:
        //constructor
        constexpr TypeErasedBackInsertIterator() noexcept {
            if (isConstantEvaluated()) {
                construct(dataConstexprPolymorphic);
            } else {
                construct(data);
            }
        }
        constexpr TypeErasedBackInsertIterator(const TypeErasedBackInsertIterator& other) noexcept {
            if (isConstantEvaluated()) {
                dataConstexprPolymorphic = other.dataConstexprPolymorphic->getCopyFunction()(other.dataConstexprPolymorphic);
            } else {
                data = other.data;
            }
        }
        constexpr TypeErasedBackInsertIterator(TypeErasedBackInsertIterator&& other) noexcept {
            if (isConstantEvaluated()) {
                dataConstexprPolymorphic = other.dataConstexprPolymorphic;
                other.dataConstexprPolymorphic = nullptr;
            } else {
                data = other.data;
                other.data = iterator_data();
            }
        }

        template<typename Container>
        explicit constexpr TypeErasedBackInsertIterator(Container* container) noexcept {
            if (isConstantEvaluated()) {
                construct<iterator_data_constexpr_polymorphic*>(&dataConstexprPolymorphic);
                populateTypeErasedBackInsertIteratorDataConstexpr<DataType, Container>(dataConstexprPolymorphic, container);
            } else {
                populateTypeErasedBackInsertIteratorData<DataType, Container>(data, container);
            }
        }
        template<typename Container>
        explicit constexpr TypeErasedBackInsertIterator(Container& container) noexcept :
            TypeErasedBackInsertIterator(&container) {}

        //destructor 
        constexpr ~TypeErasedBackInsertIterator() noexcept {
            if (isConstantEvaluated()) {
                if (dataConstexprPolymorphic != nullptr) {
                    dataConstexprPolymorphic->getDestoryFunction()(dataConstexprPolymorphic);
                }
            } else {
                deconstruct(&data);
            }
        }

        //util 
        constexpr TypeErasedBackInsertIterator& self() { return *this; }
        constexpr const TypeErasedBackInsertIterator& self() const { return *this; }

        //modifiers 
        constexpr void reserve(const size_type newCapacity) noexcept {
            if (isConstantEvaluated()) {
                dataConstexprPolymorphic->getReserveFunction()(dataConstexprPolymorphic, newCapacity);
            } else {
                data.reserveFunction(data.container, newCapacity);
            }
        }

        //iterator operation 
        constexpr TypeErasedBackInsertIterator& operator=(const value_type& value) noexcept {
            if (isConstantEvaluated()) {
                dataConstexprPolymorphic->getPushBackFunction()(dataConstexprPolymorphic, value);
            } else {
                data.pushBackFunction(data.container, value);
            }
            return self();
        }
        constexpr TypeErasedBackInsertIterator& operator=(value_type&& value) noexcept {
            if (isConstantEvaluated()) {
                dataConstexprPolymorphic->getPushBackMoveFunction()(dataConstexprPolymorphic, natl::move(value));
            } else {
                data.pushBackMoveFunction(data.container, natl::move(value));
            }
            return self();
        }

        //no op 
        constexpr TypeErasedBackInsertIterator& operator*() noexcept { return self(); }
        constexpr TypeErasedBackInsertIterator& operator++() noexcept { return self(); };
        constexpr TypeErasedBackInsertIterator& operator++(int) noexcept { return self(); };
    };
}