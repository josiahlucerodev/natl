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

//NATL_IN_DEBUG
#ifdef _DEBUG
#define NATL_IN_DEBUG
#endif

#ifdef NDEBUG
#define NATL_IN_DEBUG
#endif