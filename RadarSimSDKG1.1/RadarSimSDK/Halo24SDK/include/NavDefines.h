//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Common macro definitions
//-----------------------------------------------------------------------------
#ifndef NAVICO_NAVDEFINES_H
#define NAVICO_NAVDEFINES_H


//-----------------------------------------------------------------------------
// Miscellaneous

#ifdef NDEBUG
#   define IS_DEBUG_MODE    false
#   define IS_RELEASE_MODE  true
#else
#   define IS_DEBUG_MODE    true
#   define IS_RELEASE_MODE  false
#endif


//-----------------------------------------------------------------------------
// C Definitions

#ifndef extern_c
#   ifdef __cplusplus
#       define extern_c extern "C"
#   else
#       define extern_c extern
#   endif
#endif

#ifdef __cplusplus
#   define EXTERN_C_BEGIN  extern "C" {
#   define EXTERN_C_END    }
#else
#   define EXTERN_C_BEGIN
#   define EXTERN_C_END
#endif


//-----------------------------------------------------------------------------
// C++11 feature support
//   If the compiler being used is detected to support C++11, then the
// macro NAVICO_CXX11 will be defined to a non-zero value.  If not, then
// NAVICO_CXX11 will remain undefined.  It is assumed that if a compiler
// version can support C++11, then it is enabled.
//   If C++11 support is detected, then specific features can attempt to be
// detected by using clang's __has_feature macros.
// This only looks for a subset of C++11 features:
//   cxx_nullptr
//   cxx_static_assert
//   cxx_constexpr
//   cxx_constexpr

#ifndef __has_feature
#   define __has_feature(x) (x)
#endif

#if !defined(__cplusplus)

// A C source file included this.  Definitely don't support C++11!

#elif defined(__clang__)

// It looks like clang supported lots of C++11 in 2.9, but really kicked off in 3.0
// Keep this before __GNUC__ check because clang can define __GNUC__
#   if __clang_major__ >= 3
#       define NAVICO_CXX11     1
#   endif

#elif defined( __INTEL_COMPILER )

// C++11 support in the Intel Compiler seems to have started with 11.0
// Keep this before __GNUC__ check because icc can define __GNUC__

#   if __INTEL_COMPILER >= 1100
#       define NAVICO_CXX11     1
#   endif

#   define cxx_nullptr            (__INTEL_COMPILER >= 1200)
#   define cxx_static_assert      (__INTEL_COMPILER >= 1100)
#   define cxx_constexpr          (__INTEL_COMPILER >= 1300)
#   define cxx_variadic_templates (__INTEL_COMPILER >= 1300)

#elif defined( __GNUC__ )

#   define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

// C++11 support in gcc sort of kicked off in 4.3
#   if GCC_VERSION >= 40300
#       define NAVICO_CXX11     1
#   endif

#   define cxx_nullptr            (GCC_VERSION >= 40600)
#   define cxx_static_assert      (GCC_VERSION >= 40300)
#   define cxx_constexpr          (GCC_VERSION >= 40600)
#   define cxx_variadic_templates (GCC_VERSION >= 40600)

#elif defined( _MSC_VER )

// C++11 support in Visual Studio more or less kicked off in VS2010 (_MSC_VER >= 1600)
#   if _MSC_VER >= 1600
#       define NAVICO_CXX11     1
#   endif

#   define cxx_nullptr            (_MSC_VER >= 1600)
#   define cxx_static_assert      (_MSC_VER >= 1600)
#   define cxx_constexpr          (0)
#   define cxx_variadic_templates (0)
// No support for constexpr yet in MSVC =/

#else

// Unknown compiler
// Just default to no C++11 mode
#   define cxx_nullptr          0
#   define cxx_static_assert    0
#   define cxx_constexpr        0

#endif

// You may define DISABLE_NAVICO_CXX11 to stop these C++11 workarounds from being
// applied.  This is mainly for third parties, where these might create issues.
#ifndef DISABLE_NAVICO_CXX11

// nullptr workaround
//   Not a 100% perfect implementation of nullptr, but good enough to hold us through until
// all compilers we use support this.
#   if defined(__cplusplus) && !__has_feature(cxx_nullptr)
        const class {
            void operator&() const;
        public:
            template<class T> operator T*() const { return 0; }
            template<class C, class T> operator T C::*() const { return 0; }
        } nullptr = {};
#   endif

// static_assert workaround
//   Do the standard template instantiation error static assert, which means that we don't
// get the message in the compiler output.  That's okay, because most of the compilers we're supporting
// do support static_assert(), so the use of this should slowly fade away
#   if defined(__cplusplus) && !__has_feature(cxx_static_assert)
        template< bool > class tNavicoStaticAssert;
        template<> class tNavicoStaticAssert< true > {};

#       define static_assert( assert_condition, assert_message_unused ) ( tNavicoStaticAssert< (assert_condition) >() )
#   endif

#endif // DISABLE_NAVICO_CXX11

// Convenience macros for unused variables
#define NAVICO_UNUSED(param)  (void)(param)
#ifdef __cplusplus
    // unused return value from a function (used to deliberately suppress some of gcc's warnings)
    template <class T> void NAVICO_UNUSED_RETURN( const T& ) {}
#endif

#define NAVICO_NONCOPY(className) \
    private: \
        className( const className& ); \
        className& operator=( const className& );

#define NAVICO_NONCOPYMOVE(className) \
    private: \
        className( const className& ); \
        className& operator=( const className& ); \
        className( className&& ); \
        className& operator=( className&& );

#define NAVICO_CONCAT_DIRECT( a, b )  a##b
#define NAVICO_CONCAT( a, b )  NAVICO_CONCAT_DIRECT( a, b )

#define NAVICO_TOSTRING_DIRECT( a )  #a
#define NAVICO_TOSTRING( a )  NAVICO_TOSTRING_DIRECT( a )

//-----------------------------------------------------------------------------
// Platform Specific Implementation Helpers

#ifndef PLATFORM_NAME
#   if defined(_WIN32) || defined(_WIN32_WCE)
#       define PLATFORM_WIN32
#       define PLATFORM_NAME WIN32
#       pragma warning( disable : 4996 )   /// disable security/deprecation warning
#   elif defined(POSIX) || defined(__linux) // some Linux distributions do not define POSIX
#       define PLATFORM_POSIX
#       define PLATFORM_NAME POSIX
#   else
#       error "Unknown platform"
#   endif
#endif

//-----------------------------------------------------------------------------
// Compiler Workarounds

#if (defined(_WIN32_WCE) && (_WIN32_WCE <= 420)) || (defined(_MSC_VER) && (_MSC_VER <= 1200))
#   define NavClassConst(name,type,value)  enum { name = (type)(value) };
#   define NavStd  
#   pragma warning( disable : 4786 )   /// disable truncated to '255' characters in debug information warning
#else
#   define NavClassConst(name,type,value)  static const type name = (value);
#   define NavStd  std
#endif

#if (defined(_MSC_VER) && (_MSC_VER <= 1600))
#   define NAVICO_BOOL_OPERATOR( test )                 \
        private:                                        \
            class tDummyClass;                          \
            typedef const tDummyClass* tDummyClassPtr;  \
        public:                                         \
            operator tDummyClassPtr() const { return (test)? reinterpret_cast< tDummyClassPtr >( this ) : nullptr; }
#else
#   define NAVICO_BOOL_OPERATOR( test )                 \
        explicit operator bool() const { return (test); }
#endif

//-----------------------------------------------------------------------------
// Export Definitions

#define NAVICO_DECL_STATIC

#ifndef NAVICO_DECL_EXPORT
#   if defined(NAVICO_DECL_STATICLIBS)
#       define NAVICO_DECL_EXPORT
#   else
#       if defined(_MSC_VER)
#           define NAVICO_DECL_EXPORT __declspec(dllexport)
#       elif defined(__GNUC__)
            // To use this, you must have also set the default visibility to hidden and set this define
#           if defined(NAVICO_VISIBILITY_HIDDEN)
#               define NAVICO_DECL_EXPORT __attribute__ ((visibility ("default")))
#           else
#               define NAVICO_DECL_EXPORT
#           endif
#       else
#           define NAVICO_DECL_EXPORT
#       endif
#   endif
#endif

#ifndef NAVICO_DECL_IMPORT
#   if defined(NAVICO_DECL_STATICLIBS)
#       define NAVICO_DECL_IMPORT
#   else
#       if defined(_MSC_VER)
#           define NAVICO_DECL_IMPORT __declspec(dllimport)
#       elif defined(__GNUC__)
#           define NAVICO_DECL_IMPORT
#       else
#           define NAVICO_DECL_IMPORT
#       endif
#   endif
#endif

//-----------------------------------------------------------------------------

#endif  // include guard
