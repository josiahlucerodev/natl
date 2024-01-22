//NATL_FORCE_INLINE
#ifdef _MSC_VER
#define NATL_FORCE_INLINE __forceinline
#endif

#ifdef __GNUC__
#define NATL_FORCE_INLINE [[gnu::always_inline]]
#endif

#ifdef __clang__
#define NATL_FORCE_INLINE [[clang::always_inline]] 
#endif

#ifdef __EMSCRIPTEN__ 
#define NATL_FORCE_INLINE __attribute__((always_inline)) 
#endif