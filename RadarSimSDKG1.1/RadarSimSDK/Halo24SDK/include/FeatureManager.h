//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Interface to interpret and obtain information on supported Radar features.
//-----------------------------------------------------------------------------
#ifndef NAVICO_SDK_FEATUREMANAGER_H
#define NAVICO_SDK_FEATUREMANAGER_H

#include "Feature.h"
#include "FeatureObserver.h"
#include "ExportSDK.h"


namespace Navico {
namespace Protocol {
namespace NRP {
    struct tFeatureManagerImpl; 

//-----------------------------------------------------------------------------
//! \ref tFeatureManager
//! Responsible for caching information on a radars supported features, and notifying
//! registered observers when they change.
//-----------------------------------------------------------------------------
class NAVICO_SDK_EXPORT tFeatureManager
{
public:
    tFeatureManager();
    ~tFeatureManager();

    void Initialize(tScannerType type, uint32_t maxRange_dm);
    bool AddObserver(iFeatureObserver* pObserver);
    bool RemoveObserver(iFeatureObserver* pObserver);

    //-------------------------------------------------------------------------
    //  Getter functions

    const tFeatureBase& GetFeatureCombinedNoiseIFReject();          //! Obsolete feature
    const tFeatureSectorBlanking& GetFeatureSectorBlanking();       //! 
    const tFeatureLevel& GetFeatureIR();                            //!
    const tFeatureLevel& GetFeatureLocalIR();                       //!
    const tFeatureLevel& GetFeatureNoiseReject();                   //!
    const tFeatureLevel& GetFeatureRangeStretch();                  //!
    const tFeatureLevel& GetFeatureTargetStretch();                 //!
    const tFeatureLevel& GetFeatureBeamSharpening();                //!
    const tFeatureLevel& GetFeatureFastScan();                      //!
    const tFeatureLevel& GetFeatureLED();                           //!
    const tFeatureLevel& GetFeaturePerformanceMonitor();            //! Not yet supported
    const tFeatureLevel& GetFeatureStcCurves();                     //!
    const tFeatureUseModes& GetFeatureSupportedUseModes();          //!
    const tFeatureRangeLimits& GetFeatureSidelobeGain();            //!
    const tFeatureRangeLimits& GetFeatureInstrRangeLimits();        //!
    const tFeatureGainLimits& GetFeatureSeaUserGainLimits();        //!
    const tFeatureAntennaTypes& GetFeatureSupportedAntennaTypes();  //!
    const tFeatureLevel& GetFeatureDopplerMode();                   //!
    const tFeatureRangeLimits& GetFeatureDopplerThreshold_cmps();   //!

    tFeatureManagerImpl* GetImpl() { return m_pImpl; }

private:
    tFeatureManagerImpl* m_pImpl;
};

//-----------------------------------------------------------------------------

} // NRP
} // Protocol
} // Navico

#endif  // include guard
