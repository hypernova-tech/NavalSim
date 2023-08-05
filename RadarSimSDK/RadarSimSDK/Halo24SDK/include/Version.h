//-----------------------------------------------------------------------------
// Copyright (C) 2020 Navico Holding AS
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! @mainpage Navico Radar Software-Development-Kit
//!
//! Library for interfacing with one or more Navico Radars, controlling their
//! functionality and converting the real time image data they output into an
//! easily understandable image.
//!
//! \n \n \b Commercial \b Use \b Only:
//! Navico assumes no responsibility for use of this software. It is intended
//! for Commercial use only (i.e. not intended for aviation, military or medical
//! applications)
//-----------------------------------------------------------------------------
//!
//! \file
//! Radar SDK version number
//-----------------------------------------------------------------------------
#ifndef NAVICO_SDK_VERSION_H
#define NAVICO_SDK_VERSION_H

namespace Navico {
namespace Version {

    enum {
        gMajor = 3,
        gMinor = 5,
        gBuild = 1,
    };

}  // namespace Version
}  // namespace Navico

#endif  // include guard
// V3.5.01 : built from: SDK_3.5.01-2-g1f2bf60 hash: 1f2bf608022a01b5cd69ecf6e08ebcbbe027dd60 remote: origin https://bitbucket.navico.com/scm/rad/radar.git (fetch)
