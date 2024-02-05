//C
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <Limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

//C++
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 199711L) || __cplusplus >= 199711L)
#define NATL_CXX_98 
#endif

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus >= 201103L)
#define NATL_CXX_11 
#endif

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#define NATL_CXX_17 
#endif

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
#define NATL_CXX_14 
#endif

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || __cplusplus >= 202002L)
#define NATL_CXX_20 
#endif

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 1000000L) || __cplusplus >= 1000000L)
#define NATL_CXX_23 
#endif

#ifdef NATL_CXX_98
#include <algorithm>
#include <bitset>	  
#include <complex>	  
#include <deque>	  
#include <exception>  
#include <fstream>	  
#include <functional>	  
#include <iomanip>	  
#include <ios>	  
#include <iosfwd>  
#include <iostream>	  
#include <istream>	  
#include <iterator>	  
#include <Limits>	  
#include <list>  
#include <locale>	  
#include <map>	  
#include <memory>	  
#include <new>	  
#include <numeric>  
#include <ostream>	  
#include <queue>	  
#include <set>	  
#include <sstream>	  
#include <stack>  
#include <stdexcept>	  
#include <streambuf>	  
#include <string>	  
#include <utility>	  
#include <typeinfo>  
#include <valarray>	  
#include <vector>  
#endif // NATL_CXX_98

#ifdef NATL_CXX_11
#include <array>
#include <atomic>
#include <cfenv>
#include <chrono>
#include <cinttypes>
#include <condition_variable>
#include <cstdint>
#include <cuchar>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif // NATL_CXX_11

#ifdef NATL_CXX_17
#include <any>
#include <charconv>
#include <execution>
#include <filesystem>
//#include <memory_resource>
#include <optional>
#include <string_view>
#include <variant>
#endif // NATL_CXX_17

#ifdef NATL_CXX_20
#include <bit>
#include <compare>
#include <concepts>
#include <coroutine>
#include <format>
#include <numbers>
#include <ranges>
//#include <source_location>
#include <span>
//#include <syncstream>
#include <version>
#include <barrier>
#include <latch>
#include <semaphore>
//#include <stop_token>
#endif //NATL_CXX_20

#ifdef NATL_CXX_23
#include <expected>
#include <flat_map>
#include <flat_set>
#include <generator>
#include <mdspan>
#include <print>
#include <spanstream>
#include <stacktrace>
#include <stdfloat>
#endif // NATL_CXX_23


#undef NATL_CXX_98 
#undef NATL_CXX_11 
#undef NATL_CXX_17 
#undef NATL_CXX_14 
#undef NATL_CXX_20 
#undef NATL_CXX_23 