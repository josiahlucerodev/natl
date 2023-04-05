#pragma once

//std
#include <string_view>
#include <iostream>

//interface
namespace natl {
    constexpr void fatalError(const std::string_view message) {
        if (std::is_constant_evaluated()) {
            //throw std::logic_error(message.data());
        }
        else {
            std::cout << message;
            std::cin.get();
            assert(true);
        }
    }
    constexpr void fatalError(const bool conditional, const std::string_view message) {
        if (conditional) {
            fatalError(message);
        }
    }
    constexpr void fatalErrorVerify(const bool conditional, const std::string_view message) {
        if (!conditional) {
            fatalError(message);
        }
    }
}