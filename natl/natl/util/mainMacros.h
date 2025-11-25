#pragma once

#define NATL_CPP_MAIN() \
    int main(int argc, char** argv) { \
        return static_cast<int>(cpp_main(natl::cppMainArguments(argc, argv))); \
    }