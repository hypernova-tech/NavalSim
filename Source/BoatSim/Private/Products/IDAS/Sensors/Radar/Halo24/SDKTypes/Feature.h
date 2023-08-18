//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Define types and structures used to report supported radar features and attributes
//-----------------------------------------------------------------------------
#ifndef NAVICO_SDK_FEATURE_H
#define NAVICO_SDK_FEATURE_H

#include "NavRadarProtocol.h"

namespace Navico {
namespace Protocol {
namespace NRP {

//-----------------------------------------------------------------------------
//! Basic enum and type definition
//-----------------------------------------------------------------------------
typedef uint16_t tFeatureEnum;
enum eFeatureEnum
{
    eFeatureEnum_CombinedNoiseIFReject   =  1,
    eFeatureEnum_SupportedUseModes       =  2,
    eFeatureEnum_IRControl               =  3,
    eFeatureEnum_NoiseRejectControl      =  4,
    eFeatureEnum_RangeStretchControl     =  5,
    eFeatureEnum_STCCurveControl         =  6,
    eFeatureEnum_BeamSharpeningControl   =  7,
    eFeatureEnum_FastScanControl         =  8,
    eFeatureEnum_SidelobeGainControl     =  9,
    eFeatureEnum_SupportedAntennas       = 10,
    eFeatureEnum_InstrRangeLimits        = 11,
    eFeatureEnum_LocalIRControl          = 12,
    eFeatureEnum_LEDControl              = 13,
    eFeatureEnum_TargetStretchControl    = 14,
    eFeatureEnum_SeaUserGainLimits       = 15,
    eFeatureEnum_SectorBlanking          = 16,
    eFeatureEnum_PerformanceMonitor      = 17,
    eFeatureEnum_DopplerModeControl      = 18,
    eFeatureEnum_DopplerThresholdControl = 19,
};

struct tAntennaType
{
    uint16_t size;
};

//-----------------------------------------------------------------------------
//! Feature Classes Definition
//-----------------------------------------------------------------------------
struct tFeatureBase
{
    tFeatureEnum featureEnum;     // The type of feature
    bool     enabled;             // Whether the feature is enabled
    bool     supported;           // Whether the feature is supported
};

struct tFeatureSectorBlanking : tFeatureBase
{
    uint8_t  sectorCount;
};

struct tFeatureLevel : tFeatureBase
{
    uint8_t  maxLevel;
    uint32_t mask;
};

struct tFeatureUseModes : tFeatureBase
{
    static const int cUseMode_Total = 10;

    uint8_t  useModeCount;
    eUseMode useModes[cUseMode_Total];
};

struct tFeatureRangeLimits : tFeatureBase
{
    uint32_t minimum;
    uint32_t maximum;
};

struct tFeatureGainLimits : tFeatureBase
{
    uint8_t  manualLevelMin;
    uint8_t  manualLevelMax;
    int8_t   autoOffsetMin;
    int8_t   autoOffsetMax;
};

struct tFeatureAntennaTypes : tFeatureBase
{
    static const int cAntennaType_Total = 8;

    uint8_t  count;
    tAntennaType antennaTypes[cAntennaType_Total];
};

//-----------------------------------------------------------------------------

} // NRP
} // Protocol
} // Navico

#endif  // include guard
