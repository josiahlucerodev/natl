#pragma once 

//interface 
namespace natl {
    template<typename DataType, typename TypeTag>
    class StrongType {
    public:
        using value_type = DataType;
    private:
        value_type valueStorage;
    public:
        //constructor 
        constexpr StrongType() noexcept = default;
        explicit constexpr StrongType(const value_type& valueIn) noexcept : valueStorage(valueIn) {};

        //destructor 
        constexpr ~StrongType() noexcept = default;

        //element access 
        constexpr value_type& value() & noexcept { return valueStorage; }
        constexpr const value_type& value() const& noexcept { return valueStorage; }
        constexpr value_type&& value() && noexcept { return natl::move(valueStorage); }
        constexpr const value_type&& value() const&& noexcept { return natl::move(valueStorage); }
    };
}