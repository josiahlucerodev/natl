#pragma once

//own
#include "basicTypes.h"
#include "tuple.h"

//interface
namespace natl {
    class RepeatIterator {
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
        constexpr bool operator==(const RepeatIterator rhs) const noexcept { return index == rhs.index; }
        constexpr bool operator!=(const RepeatIterator rhs) const noexcept { return index != rhs.index; }
        constexpr bool operator<(const RepeatIterator rhs) const noexcept { return index < rhs.index; }
        constexpr bool operator>(const RepeatIterator rhs) const noexcept { return index > rhs.index; }
        constexpr bool operator<=(const RepeatIterator rhs) const noexcept { return index <= rhs.index; }
        constexpr bool operator>=(const RepeatIterator rhs) const noexcept { return index >= rhs.index; }


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

    class Repeat {
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

    class RepeatFromTo {
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
            constexpr void operator()(RemoveReferenceT<IterationRange>& iterationRange) noexcept {
                ++iterationRange;
            }
        };

        template<template<typename Iterators> typename IteratorAction, typename... Iterators>
        struct CallActionOnIteratorsStructFunc {
            constexpr void operator()(RemoveReferenceT<Iterators>&... iterationRanges) noexcept {
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
    class CombinedIterationIterator {};

    template<typename... DataTypes, typename... IterationRanges>
    class CombinedIterationIterator<TypePack<DataTypes...>, TypePack<IterationRanges...>> {
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
        constexpr bool operator==(const CombinedIterationIterator& rhs) const noexcept { return iterators == rhs.iterators; }
        constexpr bool operator!=(const CombinedIterationIterator& rhs) const noexcept { return iterators != rhs.iterators; }
        constexpr bool operator<(const CombinedIterationIterator& rhs) const noexcept { return iterators < rhs.iterators; }
        constexpr bool operator>(const CombinedIterationIterator& rhs) const noexcept { return iterators > rhs.iterators; }
        constexpr bool operator<=(const CombinedIterationIterator& rhs) const noexcept { return iterators <= rhs.iterators; }
        constexpr bool operator>=(const CombinedIterationIterator& rhs) const noexcept { return iterators >= rhs.iterators; }

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
            ConditionalT<!IsLValueReferenceV<IterationRange>,
                RemoveReferenceT<IterationRange>,
                RemoveReferenceT<IterationRange>&
        >;
        template<typename IterationRange> 
        struct IteratorAccessBegin {
            constexpr typename IterationRange::iterator operator()(RemoveReferenceT<IterationRange>& iterationRange) noexcept {
                return iterationRange.begin();
            }
        };
        template<typename IterationRange>
        struct IteratorAccessEnd {
            constexpr typename IterationRange::iterator operator()(RemoveReferenceT<IterationRange>& iterationRange) noexcept {
                return iterationRange.end();
            }
        };
        template<template<typename IterationRange> typename IteratorAccess, typename... IterationRanges>
        struct IterationRangesToIteratorsStructFunc {
            constexpr Tuple<typename IterationRanges::iterator...> operator()(RemoveReferenceT<IterationRanges>... iterationRanges) noexcept {
                using iterator_storage = Tuple<typename IterationRanges::iterator...>;
                return iterator_storage(IteratorAccess<IterationRanges>{}(iterationRanges)...);
            }
        };
    }
    
    template<typename... Types>
    class CombinedIteration {};

    template<typename... DataTypes, typename... IterationRanges>
        requires((sizeof...(DataTypes) == sizeof...(IterationRanges)) && (IsConvertible<DataTypes, IterationRanges> && ...))
    class CombinedIteration<TypePack<DataTypes...>, TypePack<IterationRanges...>> {
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
    class ReverseIteration {
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
        constexpr iterator begin() noexcept requires(IsNotConstV<IterationRange>) { return iterationRangeRef.rbegin(); }
        constexpr const_iterator begin() const noexcept { return iterationRangeRef.rbegin(); }
        constexpr const_iterator cbegin() const noexcept { return iterationRangeRef.rbegin(); }
        constexpr iterator end() noexcept requires(IsNotConstV<IterationRange>) { return iterationRangeRef.rend(); }
        constexpr const_iterator end() const noexcept { return iterationRangeRef.rend(); }
        constexpr const_iterator cend() const noexcept { return iterationRangeRef.rend(); }

        constexpr reverse_iterator rbegin() noexcept requires(IsNotConstV<IterationRange>) { return iterationRangeRef.begin(); }
        constexpr const_reverse_iterator rbegin() const noexcept { return iterationRangeRef.begin(); }
        constexpr const_reverse_iterator crbegin() const noexcept { return iterationRangeRef.begin(); }
        constexpr reverse_iterator rend() noexcept requires(IsNotConstV<IterationRange>) { return iterationRangeRef.end(); }
        constexpr const_reverse_iterator rend() const noexcept { return iterationRangeRef.end(); }
        constexpr const_reverse_iterator crend() const noexcept { return iterationRangeRef.end(); }
    };

    template<typename IterationRange>
    constexpr ReverseIteration<IterationRange> makeReverseIteration(IterationRange& iterationRange) noexcept {
        return ReverseIteration<IterationRange>(iterationRange);
    }
}