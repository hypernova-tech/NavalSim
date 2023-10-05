//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Defines the macro for byte alignment, so we have only one place for
//! setting the packing capabilities on
//---------------------------------------------------------------------------

#ifdef BYTE_PACK_ON
#   error __FILE__ ": Packing was on...."
#endif
#define BYTE_PACK_ON


#if defined _MSC_VER
#   pragma warning( disable:4103 )   // disable warning for alignment changed
#   pragma pack(push)
#   pragma pack(1)
#	define BYTE_ALIGNED

#elif defined __GNUC__
#	define BYTE_ALIGNED __attribute__ ((aligned (1),packed))

#elif defined __TI_COMPILER_VERSION__ 
#	define BYTE_ALIGNED

#else
#   error __FILE__ ": Need to define a pack method for this compiler"

#endif
