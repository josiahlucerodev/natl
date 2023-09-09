#pragma once

//std
#include <iostream>

//own
#include "stringView.h"

//interface
namespace natl {
    constexpr void fatalError(const StringView message) {
        if (std::is_constant_evaluated()) {
            //throw std::logic_error(message.data());
        }
        else {
            std::cout << message.cStr();
            std::cin.get();
            assert(true);
        }
    }
    constexpr void fatalError(const bool conditional, const StringView message) {
        if (conditional) {
            fatalError(message);
        }
    }
    constexpr void fatalErrorVerify(const bool conditional, const StringView message) {
        if (!conditional) {
            fatalError(message);
        }
    }
}