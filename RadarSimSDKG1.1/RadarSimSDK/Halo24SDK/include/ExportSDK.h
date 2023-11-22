//-----------------------------------------------------------------------------
// Copyright (C) 2007-2016 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Define class and function prefixes for correct import/export of SDK functionality
//-----------------------------------------------------------------------------

#include <NavDefines.h>

#define SIM_SDK_EN


#ifndef NAVICO_SDK_EXPORT
	#  ifdef NAVICO_BUILD_SDK_LIBS
		#ifdef SIM_SDK_EN
			#define define NAVICO_SDK_EXPORT 
		#else
			#define NAVICO_SDK_EXPORT NAVICO_DECL_EXPORT
		#endif
	#  else
		#ifdef SIM_SDK_EN
			#define NAVICO_SDK_EXPORT
		#else
			#define NAVICO_SDK_EXPORT NAVICO_DECL_IMPORT
		#endif
	#  endif
#endif

