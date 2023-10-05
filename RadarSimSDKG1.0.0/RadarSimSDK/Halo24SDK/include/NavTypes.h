//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Platform data-type abstractions
//-----------------------------------------------------------------------------
#ifndef NAVICO_NAVTYPES_H
#define NAVICO_NAVTYPES_H

#include <NavDefines.h>

//-----------------------------------------------------------------------------
// Definition of basic data-types
//-----------------------------------------------------------------------------

#ifdef NAVICO_CXX11

#   include <cstdint>

#elif defined __nios2__
    //-------------------------------------------------------------------------
    // Definition of basic data-types for NIOS compilers
    //-------------------------------------------------------------------------
    
#   include <alt_types.h>
    typedef alt_8    int8_t;
    typedef alt_16   int16_t;
    typedef alt_32   int32_t;
    typedef alt_64   int64_t;

    typedef alt_u8   uint8_t;
    typedef alt_u16  uint16_t;
    typedef alt_u32  uint32_t;
    typedef alt_u64  uint64_t;

    // 7.18.1.2  Minimum-width integer types
    typedef alt_8    int_least8_t;
    typedef alt_16   int_least16_t;
    typedef alt_32   int_least32_t;
    typedef alt_64   int_least64_t;

    typedef alt_u8   uint_least8_t;
    typedef alt_u16  uint_least16_t;
    typedef alt_u32  uint_least32_t;
    typedef alt_u64  uint_least64_t;

    // 7.18.1.3  Fastest minimum-width integer types 
    // Not actually guaranteed to be fastest for all purposes
    // Here we use the exact-width types for 8 and 16-bit ints. 
    typedef alt_8    int_fast8_t;
    typedef alt_16   int_fast16_t;
    typedef alt_32   int_fast32_t;
    typedef alt_64   int_fast64_t;

    typedef alt_u8   uint_fast8_t;
    typedef alt_u16  uint_fast16_t;
    typedef alt_u32  uint_fast32_t;
    typedef alt_u64  uint_fast64_t;

    // 7.18.1.4  Integer types capable of holding object pointers
#   ifndef _INTPTR_T_DEFINED
#   define _INTPTR_T_DEFINED
        typedef alt_32 intptr_t;
#   endif

#   ifndef _UINTPTR_T_DEFINED
#   define _UINTPTR_T_DEFINED
        typedef alt_u32 uintptr_t;
#   endif

    // 7.18.1.5  Greatest-width integer types
    typedef alt_64  intmax_t;
    typedef alt_u64  uintmax_t;
    
#elif defined __GNUC__
    //-------------------------------------------------------------------------
    // Definition of basic datatypes for GNU compilers
    //-------------------------------------------------------------------------

#   include <stdint.h>

#elif defined _MSC_VER

#   if _MSC_VER >= 1600
        // cstdint and stdint.h are available for Visual Studio 2010 and later.
#       ifdef __cplusplus
#           include <cstdint>
#       else
#           include <stdint.h>
#       endif
#   else

        //-------------------------------------------------------------------------
        // Definition of basic datatypes for microsoft compilers prior to VS2010
        //-------------------------------------------------------------------------

        // 7.18.1.1  Exact-width integer types
        typedef __int8   int8_t;
        typedef __int16  int16_t;
        typedef __int32  int32_t;
        typedef __int64  int64_t;

        typedef unsigned __int8   uint8_t;
        typedef unsigned __int16  uint16_t;
        typedef unsigned __int32  uint32_t;
        typedef unsigned __int64  uint64_t;

        // 7.18.1.2  Minimum-width integer types
        typedef __int8   int_least8_t;
        typedef __int16  int_least16_t;
        typedef __int32  int_least32_t;
        typedef __int64  int_least64_t;

        typedef unsigned __int8   uint_least8_t;
        typedef unsigned __int16  uint_least16_t;
        typedef unsigned __int32  uint_least32_t;
        typedef unsigned __int64  uint_least64_t;

        // 7.18.1.3  Fastest minimum-width integer types 
        // Not actually guaranteed to be fastest for all purposes
        // Here we use the exact-width types for 8 and 16-bit ints. 
        typedef __int8   int_fast8_t;
        typedef __int16  int_fast16_t;
        typedef __int32  int_fast32_t;
        typedef __int64  int_fast64_t;

        typedef unsigned __int8   uint_fast8_t;
        typedef unsigned __int16  uint_fast16_t;
        typedef unsigned __int32  uint_fast32_t;
        typedef unsigned __int64  uint_fast64_t;

        // 7.18.1.4  Integer types capable of holding object pointers
#       ifndef _INTPTR_T_DEFINED
#       define _INTPTR_T_DEFINED
#         ifdef _M_X64
            typedef __int64 intptr_t;
#         else
            typedef int intptr_t;
#         endif
#       endif

#       ifndef _UINTPTR_T_DEFINED
#       define _UINTPTR_T_DEFINED
#         ifdef _M_X64
            typedef unsigned __int64 uintptr_t;
#         else
            typedef unsigned int uintptr_t;
#         endif
#       endif

        // 7.18.1.5  Greatest-width integer types
        typedef __int64  intmax_t;
        typedef unsigned __int64  uintmax_t;

        // Support of C++11's char16_t prior to VS2010
        typedef uint16_t char16_t;

#    endif // _MSC_VER >= 1600

#elif defined __TI_COMPILER_VERSION__
    //-------------------------------------------------------------------------
    // Definition of basic datatypes for TI compilers
    //-------------------------------------------------------------------------

#   include <stdint.h>
#   if !defined __cplusplus && defined __STDC__ && !defined bool
#      define bool  int
#      define true  1
#      define false 0
#   endif

#else
    //-------------------------------------------------------------------------
    // Unhandled compiler type
    //-------------------------------------------------------------------------

#   error __FILE__ ": Need to define datatypes for this compiler"
#endif


//-----------------------------------------------------------------------------
// Definition of advanced datatypes
//-----------------------------------------------------------------------------

#ifdef __cplusplus
#   ifndef __TI_COMPILER_VERSION__
#       ifndef tString

            // WinCE compiling on VS2005's STL causes a lot of C4100 warnings in xlocnum - this removes some of them
#           ifdef _WIN32_WCE
#               pragma warning( push )
#               pragma warning( disable : 4100 )
#           endif

#           include <string>
            typedef std::string tString;    // String definition

#           ifdef _WIN32_WCE
#               pragma warning( pop )
#           endif

#       endif
#   endif
#endif


//-----------------------------------------------------------------------------
// Miscellaneous Types
//-----------------------------------------------------------------------------

typedef int32_t  tFixed;       // must be less of 32 bits
typedef int64_t  tLFixed;      // must be less of 64 bits
typedef uint32_t tTimeStamp;   // number of milli-seconds
typedef int32_t  tTimeDelta;

static const tTimeStamp cInvalidTimeStamp = 0xFFFFFFFF;   // constant to represent an invalid timestamp

//-----------------------------------------------------------------------------

#endif  // include guard
