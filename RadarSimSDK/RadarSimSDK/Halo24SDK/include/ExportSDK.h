//-----------------------------------------------------------------------------
// Copyright (C) 2007-2016 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Define class and function prefixes for correct import/export of SDK functionality
//-----------------------------------------------------------------------------

#include <NavDefines.h>

#ifndef NAVICO_SDK_EXPORT
#  ifdef NAVICO_BUILD_SDK_LIBS
#    define NAVICO_SDK_EXPORT NAVICO_DECL_EXPORT
#  else
#    define NAVICO_SDK_EXPORT NAVICO_DECL_IMPORT
#  endif
#endif

