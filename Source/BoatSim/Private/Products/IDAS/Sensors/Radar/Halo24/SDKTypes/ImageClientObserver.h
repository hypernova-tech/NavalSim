//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Defines observer base classes for tImageClient services
//-----------------------------------------------------------------------------
#ifndef NAVICO_PROTOCOL_NRP_IMAGECLIENTOBSERVER_H
#define NAVICO_PROTOCOL_NRP_IMAGECLIENTOBSERVER_H

#include "NavRadarProtocol.h"
#include "NavRadarSpoke.h"


namespace Navico {
namespace Protocol {
namespace NRP {
namespace Spoke {
    typedef t9174Spoke tSpokeV9174;
}

//-----------------------------------------------------------------------------
//! Structure for transferring advanced STC curve setting data
//-----------------------------------------------------------------------------
struct tAdvancedSTCState
{
    float  rangeSTCRef_dB;        ///< Range curve starting/reference value
    float  seaSTCRef_dB;          ///< Sea curve starting/reference value
    float  rainSTCRef_dB;         ///< Rain curve starting/reference value

    float  rangeSTCTrim_dB;       ///< Range curve starting value adjustment
    float  seaSTCTrim_dB;         ///< Sea curve starting value adjustment
    float  rainSTCTrim_dB;        ///< Rain curve starting value adjustment
    
    float  rangeSTCRate_dBpDec;   ///< Range curve slope/rate-of-fall
    float  seaSTCRate1_dBpDec;    ///< Sea curve slope/rate-of-fall 1
    float  seaSTCRate2_dBpDec;    ///< Sea curve slope/rate-of-fall 2
    float  rainSTCRate_dBpDec;    ///< Rain curve slope/rate-of-fall

    float  userMinSNR_dB;         ///< Minimum STC signal to noise ratio 
    float  videoAperture_dB;      ///< Video aperture level
};

//-----------------------------------------------------------------------------
//! \ref tImageClient callback interface for observing radar image data
//-----------------------------------------------------------------------------
class iImageClientSpokeObserver
{
public:
    virtual ~iImageClientSpokeObserver() {}

    virtual void UpdateSpoke( const Spoke::tSpokeV9174 *pSpoke ) = 0;
};

//-----------------------------------------------------------------------------
//! \ref tImageClient callback interface for observing changes to a radars
//! state and settings
//-----------------------------------------------------------------------------
class iImageClientStateObserver
{
public:
    virtual ~iImageClientStateObserver() {}

    virtual void UpdateMode( const tMode* pMode ) = 0;
    virtual void UpdateSetup( const tSetup* pSetup ) = 0;
    virtual void UpdateSetupExtended( const tSetupExtended* pSetupExtended, int version ) = 0;
    virtual void UpdateProperties( const tProperties* pProperties ) = 0;
    virtual void UpdateConfiguration( const tConfiguration* pConfiguration ) = 0;
    virtual void UpdateGuardZoneAlarm( const tGuardZoneAlarm* pAlarm ) = 0;
    virtual void UpdateRadarError( const tRadarError* pError ) = 0;
    virtual void UpdateAdvancedState( const tAdvancedSTCState* pState ) = 0;
};

//-----------------------------------------------------------------------------

} // NRP
} // Protocol
} // Navico

#endif  // include guard
