//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Interface for controlling the state of the radar plus receiving spokes and
//! status information.
//-----------------------------------------------------------------------------
#ifndef NAVICO_SDK_IMAGECLIENT_H
#define NAVICO_SDK_IMAGECLIENT_H

#include "ImageClientObserver.h"
#include "ExportSDK.h"

#include <memory>


namespace Navico {
namespace Protocol {
namespace NRP {

class tImageClientImpl;
class tFeatureManager;
class iFeatureObserver;

//-----------------------------------------------------------------------------
//! Class for controlling the state of the radar plus receiving spokes and state
//! information.
//-----------------------------------------------------------------------------
class NAVICO_SDK_EXPORT tImageClient
{
public:
    tImageClient();
    virtual ~tImageClient();

    //-----------------------------------------------------------------------------------
    //! Connects to the Radar with the specified SerialNumber. Any existing connection
    //! will be automatically disconnected first. The radar must be unlocked before this
    //! operation can succeed (see MultiRadarClient.h). This will also create the threads
    //! required to listen on the connections made.
    //! \param pSerialNumber  SerialNumber of the radar to connect to
    //! \param imageStream  Which image-stream/range of the radar to connect to.
    //! \return  0 if successfull, or one of the Protocol::eErrors (ClientErrors.h) enum
    //!     values if failed 
    //-----------------------------------------------------------------------------------
    int Connect( const char * pSerialNumber, unsigned imageStream = 0 );

    //-----------------------------------------------------------------------------------
    //! Disconnects from the radar. Stops all threads and deinitialises all channels.
    //! \return  True if the operation was successful, False otherwise
    //-----------------------------------------------------------------------------------
    bool Disconnect();

    //-----------------------------------------------------------------------------------
    //! Add an observer for radar state information
    //-----------------------------------------------------------------------------------
    bool AddStateObserver( iImageClientStateObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Remove a previously added radar state observer
    //-----------------------------------------------------------------------------------
    bool RemoveStateObserver( iImageClientStateObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Add an observer for radar images/spokes
    //-----------------------------------------------------------------------------------
    bool AddSpokeObserver( iImageClientSpokeObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Remove a previously added observer for radar images/spokes
    //-----------------------------------------------------------------------------------
    bool RemoveSpokeObserver( iImageClientSpokeObserver * pObserver );

    //-----------------------------------------------------------------------------------
    //! Add an observer for radar features
    //-----------------------------------------------------------------------------------
    bool AddFeatureObserver(iFeatureObserver *pObserver);

    //-----------------------------------------------------------------------------------
    //! Remove a previously added observer for radar features
    //-----------------------------------------------------------------------------------
    bool RemoveFeatureObserver(iFeatureObserver *pObserver);

    //-----------------------------------------------------------------------------------
    //! Gets the radar SDK library version: major.minor.build
    //! \param major The major number of the library version
    //! \param minor The minor number of the library version
    //! \param build The build number of the library version
    //-----------------------------------------------------------------------------------
    static void GetVersion( uint32_t& major, uint32_t& minor, uint32_t& build );

    //-----------------------------------------------------------------------------------
    //! Gets the feature manager
    //-----------------------------------------------------------------------------------
    tFeatureManager& GetFeatureManager();

    /////////////////////////////////////////////////////////////////////////////////////
    //  Commands
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Set the power state of the radar
    //! \param state The new state
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetPower( bool state );

    //-----------------------------------------------------------------------------------
    //! Set the transmission state of the radar. This will also start the scanner rotating
    //! \param state The new state
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetTransmit( bool state );

    //-----------------------------------------------------------------------------------
    //! Set the timed transmission state of the radar. This will cause the radar to 
    //! alternately transmit for a period and then enter standby mode for a period of time.
    //! The transmit and standby periods must be set to valid values before this operation 
    //! will have any effect (see \ref SetTimedTransmitSetup).
    //! \param state The new timed transmit state
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetTimedTransmit( bool state );

    //-----------------------------------------------------------------------------------
    //! Setup the conditions for timed radar transmission.
    //! \param transmitPeriod_sec The number of seconds to transmit for
    //! \param standbyPeriod_sec The number of seconds remain in standby for
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetTimedTransmitSetup( uint32_t transmitPeriod_sec, uint32_t standbyPeriod_sec );

    //-----------------------------------------------------------------------------------
    //! Set the new range in the radar
    //! \param range_m The new range expressed in meters
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetRange( uint32_t range_m );

    //-----------------------------------------------------------------------------------
    //! Send a client watch-dog kick, to make sure the radar doesn't stop sending spokes.
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool SendClientWatchdog();

    //-----------------------------------------------------------------------------------
    //! Enable or disable the automatic sending of client watchdog kicks, when receiving
    //! spokes
    //-----------------------------------------------------------------------------------
    void SetAutoSendClientWatchdog( bool state );

    //---------------------------------------------------------------------------
    //! Set the use mode
    //! \param mode Selected use mode
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetUseMode( eUseMode mode );

    //-----------------------------------------------------------------------------------
    //! Set the gain control
    //! \param type The type of gain can be: eUserGainManual (in this situation the 'level'
    //!     value will be applied), or eUserGainAuto (the radar will work in automatic
    //!     gain mode)
    //! \param level The value to use in manual gain mode (0-255) 
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetGain( eUserGainManualAuto type, uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Set the sea clutter level
    //! \param type The type of sea clutter can be: eUserGainManual (in this situation
    //!     the 'level' value will be applied), eUserGainAutoHarbour (the radar will
    //!     work in auto mode optimised for Harbour situations), or eUserGainAutoOffshore
    //!     (the radar will work in auto mode optimised for offshore situations).
    //! \param level The value to use in eUserGainManual mode (0-255) 
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetSeaClutter( eUserGainManualAuto type, uint8_t level );

    //---------------------------------------------------------------------------
    //! Set the sea clutter level
    //! \param type The type of sea clutter can be: eUserGainManual (in this 
    //! situation the \param level value will be applied), eUserGainAuto
    //! (the radar will work in auto mode).
    //! \param level It is the manual gain value (0-255)
    //! \param autoOffset It is the automatic gain offset value (-128-127)
    //! \param valid It is the bit mask for validity of supplied parameters:
    //!     UserGainValid_Mode = update the manual/auto type,
    //!     UserGainValid_Level = update the manual level,
    //!     UserGainValid_Offset = update the auto offset.
    //!     UserGainValid_ALL = update all three parameters.
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetSeaClutter( eUserGainManualAuto type, uint8_t level, int8_t autoOffset, eUserGainValid valid = UserGainValid_ALL );

    //-----------------------------------------------------------------------------------
    //! Set the type of STC curve that will be applied in automatic sea clutter rejection.
    //! \param type The curve type to use, one of the eStcCurveType enum values.
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetSTCCurveType( eStcCurveType type );

    //-----------------------------------------------------------------------------------
    //! Set the rain clutter rejection level
    //! \param level The manual gain value (0-255) 
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetRain( uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Set the sidelobe control
    //! \param type The type of gain can be: eUserGainManual (in this situation the 'level'
    //!     value will be applied), or eUserGainAuto (the radar will work in auto gain mode)
    //! \param level It is the manual gain value (0-255) 
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetSideLobe( eUserGainManualAuto type, uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Set the FTC level
    //! \param level It is the FTC value (0-255) 
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetFTC( uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Set the interference reject level
    //! \param level It is a value between 0 and 3 - 0 disabled, 3 highest rejection
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetInterferenceReject( uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Set level of local-interference reject
    //! \param level Amount of rejection to apply (none 0 to 3 high)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetLocalIR( uint8_t level );

    //---------------------------------------------------------------------------
    //! Set the Noise Reject level
    //! \param level Amount of noise rejection to apply (none 0 to 3 high) 
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetNoiseReject( uint8_t level );

    //---------------------------------------------------------------------------
    //! Set the Beam Sharpening level
    //! \param level Amount of beam-sharpening to apply (none 0-3 high) 
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetBeamSharpening( uint8_t level );

    //---------------------------------------------------------------------------
    //! Set the Doppler processing mode
    //! \param level Type of Doppler processing to apply 
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetDopplerMode( uint8_t level );

    //---------------------------------------------------------------------------
    //! Set the threshold at which Doppler colouring should be applied to targets
    //! \param threshold_cmps  Absolute level above which colouring should be
    //!     applied, in centimetres per second.
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetDopplerThreshold_cmps( uint16_t threshold_cmps );

    //---------------------------------------------------------------------------
    //! Set the LEDs lighting level
    //! \param level It is a value between 0 and 4, 0 disabled, 4 brightest
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetLEDsLevel( uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Selects between normal and fast scanner rotation speeds. Note that other controls,
    //! such as range and noise-reject, can restrict the maximum RPM available to lower
    //! than this setting.
    //! \param level Selects one of the RPM rates: 0=normal/24rpm, 1=36rpm, 2=48rpm, 3=60rpm
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetFastScanMode( uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Set target stretch/expansion in SW
    //! \param state If true, the target will appear larger, if false, with normal size
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetTargetStretch( bool state );

    //! \param level Amount of target-stretch to apply (0 none, up-to 3 high)
    //! \return True if the operation was successful
    //-----------------------------------------------------------------------------------
    bool SetTargetStretch( uint8_t level );

    //-----------------------------------------------------------------------------------
    //! Set target boost in hardware, changing physical parameters of the radar (pulse 
    //! width or sweep length, for example).
    //! \param level The level of target boost desired. 0 = best target detail, 2 most
    //!     target emphasis.
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetTargetBoost( uint8_t level );


    /////////////////////////////////////////////////////////////////////////////////////
    //  Advanced Commands
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Set scanner rotation speed.
    //! \param rpm Revolutions-per-minute (10 to 60 inclusive)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetScannerRPM( unsigned rpm );

    //-----------------------------------------------------------------------------------
    //! Set the Minimum Signal to Noise Ratio.
    //! \param level_dB Minimum SNR level (-100.0 to +100.0 dB inclusive)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetUserMinSNR( float level_dB );

    //-----------------------------------------------------------------------------------
    //! Set the STC video-aperture level.
    //! \param level_dB Video-aperture level (0.0 to 100.0 inclusive)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetVideoAperture( float level_dB );

    //-----------------------------------------------------------------------------------
    //! Set STC range trimming value (offset to STC range curve starting value).
    //! \param level_dB  Trim level (-100.0 to +100.0 inclusive)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetRangeSTCTrim( float level_dB );

    //-----------------------------------------------------------------------------------
    //! Set STC range rate value (rate of fall for STC range curve).
    //! \param level_dBpDec  Rate level (1.0 to 100.0 inclusive) in dBs per Decade
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetRangeSTCRate( float level_dBpDec );

    //-----------------------------------------------------------------------------------
    //! Set STC sea trimming value (offset to STC sea curve starting value).
    //! \param level_dB  Trim level (-100.0 to +100.0 inclusive)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetSeaSTCTrim( float level_dB );

    //-----------------------------------------------------------------------------------
    //! Set STC sea rate value (rate of fall for STC range curve).
    //! \param level_dBpDec  Rate level (1.0 to 100.0 inclusive) in dBs per Decade
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetSeaSTCRate1( float level_dBpDec );

    //-----------------------------------------------------------------------------------
    //! Set STC sea rate value (rate of fall for STC range curve).
    //! \param level_dBpDec  Rate level (1.0 to 100.0 inclusive) in dBs per Decade
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetSeaSTCRate2( float level_dBpDec );

    //-----------------------------------------------------------------------------------
    //! Set STC rain trimming value (offset to STC rain curve starting value).
    //! \param level_dB  Trim level (-100.0 to +100.0 inclusive)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetRainSTCTrim( float level_dB );

    //-----------------------------------------------------------------------------------
    //! Set STC range rate value (rate of fall for STC range curve).
    //! \param level_dBpDec  Rate level (1.0 to 100.0 inclusive) in dBs per Decade
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetRainSTCRate( float level_dBpDec );

    //---------------------------------------------------------------------------
    //! If the scanner supports this mode, the scanner can be configured not to
    //! transmit in a specific sector of rotation
    //! \param sectorID sector ID
    //! \param state if false, the sector will be in off state, if true will be 
    //! in on state
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetSectorBlanking( uint32_t sectorID, bool state );

    //---------------------------------------------------------------------------
    //! If the scanner supports this mode, the scanner can be configured not to
    //! transmit in a specific sector of rotation
    //! \param sectorID sector ID
    //! \param state if false, the sector will be in off state, if true will be 
    //! in on state
    //! \param startBearing_degrees Start of the sector in degrees
    //! \param endBearing_degrees End of the sector in degrees
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetSectorBlankingSetup( uint32_t sectorID, bool state, uint16_t startBearing_degrees, uint16_t endBearing_degrees );

    //---------------------------------------------------------------------------
    //! Enable or disable the main bang suppression
    //! \param state If false, the main bang suppression will be off, if true it
    //! will be enabled
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetMainBangSuppression( bool state );

    /////////////////////////////////////////////////////////////////////////////////////
    //  Queries
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Query all info from a radar
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool QueryAll();

    //-----------------------------------------------------------------------------------
    //! Query mode info from a radar
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool QueryMode();

    //-----------------------------------------------------------------------------------
    //! Query setup info from a radar
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool QuerySetup();

    //-----------------------------------------------------------------------------------
    //! Query advanced info from a radar
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool QueryAdvancedSetup();

    //-----------------------------------------------------------------------------------
    //! Query properties info from a radar
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool QueryProperties();

    //-----------------------------------------------------------------------------------
    //! Query configuration info from a radar
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool QueryConfiguration();

    /////////////////////////////////////////////////////////////////////////////
    // Pulse Radar Control

    //---------------------------------------------------------------------------
    //! Set the tune from automatic to manual. If auto is set, it will start
    //! auto-tuning from the last manual value set
    //! \param automatic If true, the auto tuning will be used, if false the 
    //! manual value will be used
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetTuneState( bool automatic );

    //---------------------------------------------------------------------------
    //! Set the coarse tune value
    //! \param level The coarse level
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetTuneCoarse( uint8_t level );

    //---------------------------------------------------------------------------
    //! Set the fine tuning value
    //! \param level The fine level
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetTuneFine( uint8_t level );

    //---------------------------------------------------------------------------
    //! Query features information from a radar
    //! \return True if the operation was successfully initiated
    //---------------------------------------------------------------------------
    bool QueryFeatures();

    /////////////////////////////////////////////////////////////////////////////////////
    //  Configuration
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Set the parking angle for open array scanners (this is not supported for the radars
    //! that have enclosed antennas (such as the BR24).
    //! \param angle_deg The parking angle in degrees
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetParkPosition( uint32_t angle_deg );

    //-----------------------------------------------------------------------------------
    //! Set the zero range offset
    //! \param offset The amount of zero range offset (0-0x3ff)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetZeroRangeOffset( uint16_t offset );

    //-----------------------------------------------------------------------------------
    //! Set the zero bearing offset
    //! \param bearing_ddeg The offset expressed in 10ths of a degree (deci-degrees)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetZeroBearingOffset( uint16_t bearing_ddeg );

    //-----------------------------------------------------------------------------------
    //! Set the antenna height
    //! \param antennaHeight_mm Height of the antenna, expressed in millimeters
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetAntennaHeight( uint32_t antennaHeight_mm );

    //---------------------------------------------------------------------------
    //! Set the antenna offset from CCRP
    //! \param xOffset_mm Offset starboard from CCRP to antenna, expressed in millimetres
    //! \param yOffset_mm Offset forward from CCRP to antenna, expressed in millimetres
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetAntennaOffsets( int32_t xOffset_mm, int32_t yOffset_mm );

    //---------------------------------------------------------------------------
    //! Set the antenna type
    //! \param antennaType Type of the antenna, pointer to AntennaTable
    //! \return True if the operation was successful
    //---------------------------------------------------------------------------
    bool SetAntennaType( uint16_t antennaType );

    //-----------------------------------------------------------------------------------
    //! Sets the radar to use factory defaults
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetToFactoryDefaults();


    /////////////////////////////////////////////////////////////////////////////////////
    //  GuardZones
    /////////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------------------
    //! Enable guard zone detection
    //! \param zone Guard zone index/number (0 to cMaxGuardZones-1)
    //! \param state If true, it will be enabled, false it will be disabled
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetGuardZoneEnable( uint8_t zone, bool state );

    //-----------------------------------------------------------------------------------
    //! Sets up the size and location information for a guard zone
    //! \param zone Guard zone index/number (0 to cMaxGuardZones-1)
    //! \param startRange_m Start of the sector, expressed in meters
    //! \param endRange_m End of the sector, expressed in meters
    //! \param bearing_deg Position of centre of the sector, expressed in degrees
    //! \param width_deg Width of the sector, expressed in degrees (use 360 degrees to
    //!     define a circular sector)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetGuardZoneSetup( uint8_t zone, uint32_t startRange_m, uint32_t endRange_m, uint16_t bearing_deg, uint16_t width_deg );

    //-----------------------------------------------------------------------------------
    //! Sets up the type of alarm for each guard zone
    //! \param zone Guard zone index/number (0 to cMaxGuardZones-1)
    //! \param type  The alarm type: eGZAlarmEntry (0) alarm only on targets entering the
    //!     zone, eGZAlarmExit (1) alarm only on targets exiting, eGZAlarmBoth (2) alarm
    //!     on targets both entering and exiting the zone
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetGuardZoneAlarmSetup( uint8_t zone, eGuardZoneAlarmType type );

    //-----------------------------------------------------------------------------------
    //! Suppress an alarm coming from guard zone system
    //! \param zone Guard zone index/number (0 to cMaxGuardZones-1)
    //! \param state If false, suppresses the alarm, if true, unsuppresses the alarm
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetGuardZoneAlarmSuppress( uint8_t zone, bool state );

    //-----------------------------------------------------------------------------------
    //! Cancels an alarm coming from guard zone system
    //! \param zone Guard zone index/number (0 to cMaxGuardZones-1)
    //! \param type Type of alarm we want to cancel: eGZAlarmEntry (0) or eGZAlarmExit (1)
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetGuardZoneAlarmCancel( uint8_t zone, eGuardZoneAlarmType type );

    //-----------------------------------------------------------------------------------
    //! Sets up the sensitivity of guard zones
    //! \param level The level of sensitivity. 0 = no sensitivity, 255 = max sensitivity
    //! \return True if the operation was successfully initiated
    //-----------------------------------------------------------------------------------
    bool SetGuardZoneSensitivity( uint8_t level );

    /////////////////////////////////////////////////////////////////////////////////////
    //  Deprecated
    /////////////////////////////////////////////////////////////////////////////////////

    //bool SetSTCCurveType( uint8_t level )  { return SetSTCCurveType( eStcCurveType(level) ); }
    //bool SetTargetExpansion( bool state )  { return SetTargetStretch( state ); }
    //bool SetFastScanMode( bool state )     { return SetFastScanMode( uint8_t(state) ); }


    /////////////////////////////////////////////////////////////////////////////////////
    //  Implementation
    /////////////////////////////////////////////////////////////////////////////////////

private:
    tImageClient(const tImageClient&);
    tImageClient& operator=(const tImageClient&);

    tFeatureManager*   m_pFeatureManager;
    tImageClientImpl*  m_pImpl;
};

//-----------------------------------------------------------------------------

} // NRP
} // Protocol
} // Navico

#endif  // include guard
