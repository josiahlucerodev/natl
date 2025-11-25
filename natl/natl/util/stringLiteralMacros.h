#pragma once 

#if defined(NATL_COMPILER_EMSCRIPTEN)

#define NATL_MAKE_TEMPLATE_STRING_LITERAL_FROM_STRING_VIEW(stringView) natl::TemplateStringLiteral<stringView.size() + 1>(stringView.c_str())

#elif defined(NATL_COMPILER_GCC)

#define NATL_MAKE_TEMPLATE_STRING_LITERAL_FROM_STRING_VIEW(stringView) natl::TemplateStringLiteral<stringView.size() + 1>(stringView.c_str())

#elif defined(NATL_COMPILER_MSVC)

#define NATL_MAKE_TEMPLATE_STRING_LITERAL_FROM_STRING_VIEW(stringView) natl::makeTemplateStringLiteral<stringView>()

#else
static_assert("natl: macro NATL_MAKE_TEMPLATE_STRING_LITERAL_FROM_STRING_VIEW not implemented");
#endif 