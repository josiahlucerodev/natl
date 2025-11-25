#pragma once 

// own
#include "utility.h"
#include "../container/stringView.h"
#include "../container/smallDynArray.h"

//@export
namespace natl {
    inline natl::ConstArrayView<natl::ConstAsciiStringView> cppMainArguments(int argc, char** argv) {
        static natl::SmallDynArray<natl::ConstAsciiStringView, 255> argStorage;
        argStorage.resize(argc);
        for (natl::Size i = 0; i < argc; ++i) {
            argStorage[i] = argv[i];
        }
        return argStorage;
    }

    using MainArgsView = natl::ConstArrayView<natl::ConstAsciiStringView>;

    enum struct ExitCode : int {
        success = 0,
        failure = 1,
    };

    [[noreturn]] inline void exit(const ExitCode c) noexcept {
        natl::rexit(static_cast<int>(c));
    }
}
