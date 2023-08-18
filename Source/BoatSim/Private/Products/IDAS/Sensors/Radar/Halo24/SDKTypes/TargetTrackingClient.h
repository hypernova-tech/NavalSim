//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file TargetTrackingClient.h
//! 
//! Interface to configure, control and report Radar Target-Tracking data
//-----------------------------------------------------------------------------
#ifndef NAVICO_TARGETTRACKINGCLIENT_H
#define NAVICO_TARGETTRACKINGCLIENT_H

#include "ExportSDK.h"
#include "NavRadarProtocol.h"
#include "TargetTrackingObserver.h"

using namespace std;
#include <list>

namespace Navico {
namespace Protocol {
namespace NRP {
    struct tTargetTrackingClientImpl;

typedef iTargetTrackingObserver iTargetTrackingClientObserver;
typedef iTargetTrackingStateObserver iTargetTrackingClientStateObserver;

//-----------------------------------------------------------------------------
//! Class for acquiring and tracking radar targets
//-----------------------------------------------------------------------------
class NAVICO_SDK_EXPORT tTargetTrackingClient
{
public:
    tTargetTrackingClient();
    virtual ~tTargetTrackingClient();

    //-----------------------------------------------------------------------------------
    //! Connects to the Radar with the specified 'serialNumber'. Any existing connection
    //! will be automatically disconnected first. The radar must be unlocked before this
    //! operation can succeed (see MultiRadarClient.h). This will also create the threads
    //! required to listen on the connections made.
    //! \param pSerialNumber  SerialNumber of the radar to connect to
    //! \param imageStream  Which image-stream/range of the radar to connect to.
    //! \return  0 if successful, or one of the eErrors (ClientErrors.h) enums if failed 
    //-----------------------------------------------------------------------------------
    int Connect( const char * pSerialNumber, unsigned imageStream = 0 );

    //-----------------------------------------------------------------------------------
    //! Disconnects from the radar. Stops all threads and releases all channels.
    //! \return True if the operation was successful, False otherwise
    //-----------------------------------------------------------------------------------
    bool Disconnect();

    //-----------------------------------------------------------------------------------
    //! Add an observer to the State channel
    //-----------------------------------------------------------------------------------
    bool AddStateObserver( iTargetTrackingStateObserver* pObserver );

    //-----------------------------------------------------------------------------------
    //! Remove an observer to the State channel
    //-----------------------------------------------------------------------------------
    bool RemoveStateObserver( iTargetTrackingStateObserver* pObserver );

    //-----------------------------------------------------------------------------------
    //! Add an observer to the target tracking channel
    //-----------------------------------------------------------------------------------
    bool AddTargetTrackingObserver( iTargetTrackingObserver* pObserver );

    //-----------------------------------------------------------------------------------
    //! Remove an observer to the target tracking channel
    //-----------------------------------------------------------------------------------
    bool RemoveTargetTrackingObserver( iTargetTrackingObserver* pObserver );


    /////////////////////////////////////////////////////////////////////////////////////
    //  Commands
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Acquire a target
    //! \param id The client id for a target.
    //! \param range_m The range where the radar should look to start tracking, in metres.
    //! \param bearing_deg The bearing where the radar should look to start tracking, in
    //!     degrees.
    //! \param bearingType If eAbsolute, the bearing will be interpreted as true north
    //!     bearing, if eRelative the bearing will be interpreted as relative to the heading
    //!     of the vessel.
    //! \return  True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool Acquire( uint32_t id, uint32_t range_m, uint16_t bearing_deg, eBearingType bearingType );

    //-----------------------------------------------------------------------------------
    //! Cancel a tracked target. Also required to release server-ID's of targets that have
    //! been lost.
    //! \param serverID  The id of the target to be canceled.
    //! \return  True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool Cancel( uint32_t serverID );

    //-----------------------------------------------------------------------------------
    //! Cancel tracking of all targets
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool CancelAll();

    //-----------------------------------------------------------------------------------
    //! Sets the vessel speed and direction for use by target-tracking. This is a legacy interface,
    //! prefer to use SetOwnBoatNavigation instead.
    //! \param speedType Type of speed supplied: eSpeedOverGround (1), eWaterSpeed (2) is
    //!     the water speed. Only one kind should be provided
    //! \param speed_dmps Speed magnitude expressed in 10ths of a metre per second (ie.
    //!     deci-metres per second)
    //! \param directionType Type of direction supplied: eCoureOverGround (1); eHeadingMagnetic
    //!     (2); or eHeadingTrue (3)
    //! \param direction_deg Direction of the vessel expressed in degrees.
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool SetBoatSpeed( eSpeedType speedType, uint32_t speed_dmps, eDirectionType directionType, uint32_t direction_deg );

    //-----------------------------------------------------------------------------------
    //! Sets own-vessel navigation data for use by target-tracking. The data will be transmitted 
    //! using the NRP protocol as well as the new N2KOverEthernet protocol (see SetN2KOverEthernetPriority
    //! method below for how to disable this second option).
    //! \param speedType  Speed kind can be fed to the target tracking module: eSOG (0x01) is the
    //!     speed of the ground, eWaterSpeed (0x02) is the water speed.
    //! \param speed_dmps  Speed magnitude expressed in decimetres per second.
    //! \param directionType  Type of direction supplied: eCOG (0x01) is the course of the ground,
    //!     eHDG (0x02) is the heading that comes from an heading sensor.
    //! \param direction_ddeg  Direction of the vessel expressed in decidegrees (10ths of a degree).
    //! \param magvarType  Whether a valid magnetic-variation value has been provided.
    //! \param magvar_ddeg  Amount of magnetic-variation currently required expressed in decidegrees
    //!     (10ths of a degree).
    //! \return True if the operation was successful **/
    //-----------------------------------------------------------------------------------
    bool SetOwnVesselNavigation(
        eSpeedType     speedType,     uint32_t speed_dmps,
        eDirectionType directionType, uint32_t direction_ddeg,
        eMagVarType    magvarType,    int32_t  magvar_ddeg );

    //-----------------------------------------------------------------------------------
    // Sets the priority of own-vessel navigation data, sent using a NMEA2000 Ethernet based
    // protocol.
    // \param priority  Priority of navigation data. Defaults to 5 which is the same priority
    //      as Navico MFDs (Multi Function Displays). Accepts values in the range 0-7. A value
    //      of zero will disable navigation data from being sent using this protocol.
    //-----------------------------------------------------------------------------------
    bool SetN2KOverEthernetPriority( uint8_t priority );


    /////////////////////////////////////////////////////////////////////////////////////
    //  Queries
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Query all info from TargetTracking module
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool QueryAll();

    //-----------------------------------------------------------------------------------
    //! Query all targets info from TargetTracking module
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool QueryAllTargets();

    //-----------------------------------------------------------------------------------
    //! Query setup info from TargetTracking module
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool QueryAlarmSetup();

    //-----------------------------------------------------------------------------------
    //! Query properties info from TargetTracking module
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool QueryProperties();


    /////////////////////////////////////////////////////////////////////////////////////
    //  Configuration
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Sets the danger zone distance. Used in connection with the danger zone time (see
    //! SetDangerTime) to provide early warning of a possible collision with a target. 
    //! \param range_m Danger distance in metres
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool SetDangerDistance( uint32_t range_m );

    //-----------------------------------------------------------------------------------
    //! Sets the danger zone time. Used in connection with the danger zone distance (see
    //! SetDangerDistance) to provide early warning of a possible collision with a target. 
    //! \param time_sec Danger time in seconds
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool SetDangerTime( uint32_t time_sec );

private:
    tTargetTrackingClient( const tTargetTrackingClient& );
    tTargetTrackingClient& operator=( const tTargetTrackingClient& );

    tTargetTrackingClientImpl*  m_pImpl;

    
    
    
    ////// HYPERNOVA
    char ConnectedSerialNumber[128];
    list< iTargetTrackingStateObserver*> StateObservers;
    list< iTargetTrackingObserver*> TrackingObservers;





};

//-----------------------------------------------------------------------------

}  // NRP
}  // Protocol
}  // Navico

#endif  // include guard
