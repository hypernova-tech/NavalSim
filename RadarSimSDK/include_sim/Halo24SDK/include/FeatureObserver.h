//-----------------------------------------------------------------------------
// Copyright (C) 2020 Navico Holding AS
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Interface for observing changes to supported Radar features.
//-----------------------------------------------------------------------------
#ifndef NAVICO_PROTOCOL_NRP_FEATUREOBSERVER_H
#define NAVICO_PROTOCOL_NRP_FEATUREOBSERVER_H

#include "Feature.h"
//#include "ExportSDK.h"


namespace Navico {
namespace Protocol {
namespace NRP {

//-----------------------------------------------------------------------------
//! \ref iFeatureObserver callback interface for observing radar features
//! Abstract base class for receiving callbacks when a radars support for a 
//! control changes. See \ref tFeatureBase and derived classes for more information.
//-----------------------------------------------------------------------------
class iFeatureObserver
{
public:
    virtual ~iFeatureObserver() {}
    virtual void UpdateFeature(const tFeatureEnum* pFeature) = 0;
};

//-----------------------------------------------------------------------------

} // NRP
} // Protocol
} // Navico

#endif  // include guard
