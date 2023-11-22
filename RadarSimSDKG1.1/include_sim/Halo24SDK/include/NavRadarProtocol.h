//-----------------------------------------------------------------------------
// Copyright (C) 2007-2019 Navico
// Confidential and proprietary.  All rights reserved.
//-----------------------------------------------------------------------------
//! \file
//! Navico radar protocol image related definitions.
//-----------------------------------------------------------------------------
#ifndef NAVICO_PROTOCOL_NRP_NAVRADARPROTOCOL_H
#define NAVICO_PROTOCOL_NRP_NAVRADARPROTOCOL_H

#include "NavTypes.h"

namespace Navico {
namespace Protocol {
namespace NRP {

#include "BytePackOn.h"


typedef uint32_t tUserGainManualAuto;
enum eUserGainManualAuto
{
    eUserGainManual       = 0,
    eUserGainAuto         = 1,
    eUserGainAutoHarbour  = eUserGainAuto,
    eUserGainAutoOffshore = 2,

    eTotalUserGains       = 3
};

enum eUserGainValid
{
    UserGainValid_Mode = 0x01,
    UserGainValid_Level = 0x02,
    UserGainValid_Offset = 0x04,

    UserGainValid_ALL = 0x07
};

///////////////////////////////////////////////////////////////////////////////
//  Guard zones
///////////////////////////////////////////////////////////////////////////////

typedef uint8_t tGuardZoneSelect;

enum {
    cMaxGuardZones = 2   ///< Maximum number of GuardZones able to be supported
};

typedef uint32_t tGuardZoneAlarmType;
enum eGuardZoneAlarmType
{
    eGZAlarmEntry   = 0x0,
    eGZAlarmExit    = 0x1,
    eGZAlarmBoth    = 0x2,

    eTotalGuardZoneAlarmTypes
};

typedef uint32_t tAlarmState;
enum eAlarmState
{
    eAlarmActive    = 0x1,
    eAlarmInactive  = 0x2,
    eAlarmCancelled = 0x3,
};


///////////////////////////////////////////////////////////////////////////////
//  State
///////////////////////////////////////////////////////////////////////////////

enum eScannerState
{
    eOff        = 0,
    eStandby    = 1,
    eTransmit   = 2,
    eWarming    = 3,
    eNoScanner  = 4,
    eDetectingScanner = 5,
};

struct tMode                         ///  Structure for conveying radar mode information (eg. scanner state)
{                                    ///
    uint32_t  state;                 ///< One of State::eScannerState
    uint32_t  ttState;               ///< Timed transmit mode setting
    uint32_t  warmupTime_sec;        ///< Number of seconds remaining for the scanner to warm-up
    uint32_t  ttCount_sec;           ///< Seconds remaining before timed-transmit will cause change between standby and transmit
} BYTE_ALIGNED;

struct tGuardZoneAlarm               ///  Structure for conveying guard-zone alarm information
{                                    ///
    tGuardZoneSelect    zone;        ///< Number of the guard-zone that raised the alarm
    tGuardZoneAlarmType type;        ///< Type of alarm that has been raised
    tAlarmState         state;       ///< Current state of the raised alarm
} BYTE_ALIGNED;

typedef uint32_t tRadarErrorType;
enum eRadarErrorType
{
    eErrorPersistenceCorrupt      = 0x00000001,
    eErrorZeroBearingFault        = 0x00010001,
    eErrorBearingPulseFault       = 0x00010002,
    eErrorMotorNotRunning         = 0x00010003,
    eErrorCommsNotActive          = 0x00010004,
    eErrorMagnetronHeaterVoltage  = 0x00010005,
    eErrorModulationVoltage       = 0x00010006,
    eErrorTriggerFault            = 0x00010007,
    eErrorVideoFault              = 0x00010008,
    eErrorFanFault                = 0x00010009,
    eErrorScannerConfigFault      = 0x0001000A,
    eErrorPowerSupplyTransient    = 0x0001000B,
    eErrorScannerDetectFail       = 0x0001000C,
    eErrorPASoftOverheat          = 0x0001000D,
    eErrorPAHardOverheat          = 0x0001000E,
    eErrorGWDatapathError         = 0x0001000F,
    eErrorPSUOverheat             = 0x00010010,
    eErrorPSUVoltage              = 0x00010011,
    eErrorPSUPower                = 0x00010012,
    eErrorPSUHWFault              = 0x00010013,
    eErrorPSUCommsFault           = 0x00010014,
    eErrorMechanicalFault         = 0x00010015,
    eErrorLEDFault                = 0x00010016,
    eErrorScannerFail             = 0x00010017,
    eErrorRIFault                 = 0x00010018,
    eErrorLowBattery              = 0x00010019,
    eErrorMotorStall              = 0x0001001A,
    eErrorSafetyMode              = 0x0001001B,
    eErrorDopplerFail             = 0x0001001C,
    eErrorInstallCacheMismatch    = 0x0001001D,
    eErrorTrackingUnavailable     = 0x0001001E,
};

struct tRadarError                   ///  Structure for conveying radar errors
{                                    ///
    tRadarErrorType  type;           ///< One of eRadarErrorType enum values
    uint32_t  param1;                ///< Information related to the error
    uint32_t  param2;                ///< Further information related to the error
};


///////////////////////////////////////////////////////////////////////////////
//  Properties
///////////////////////////////////////////////////////////////////////////////

typedef uint32_t tScannerType;
enum eScannerType
{
    eNKE_1065           = 0,    // 2kW
    eNKE_249            = 1,    // 4kW
    eNKE_250_4          = 2,    // 6kW
    eNKE_250_4_NAX      = 3,    // 6kW NAX
    eNKE_2102_6         = 4,    // 10kW
    eNKE_2252_7         = 5,    // 25kW
    eNKE_2252_9         = 6,

    e4kWSimulator       = 7,
    eGWTestScanner      = 8,
    eTypeNoScanner      = 9,
    
    eFMCW400_BR24       = 10,
    eFMCW400_Simulator  = 11,
    eFMCW400_HD3G       = 12,
    eFMCW400_HD4G       = 13,
    ePCOMP_HALO         = 14,
    ePROP_PRSRT         = 15,
    ePCOMP_HALO24       = 16,
    ePCOMP_HALO20E      = 17,
    ePCOMP_HALO20P      = 18,

    eTotalScannerTypes
};

inline bool IsFMCWType( eScannerType type )  { return ((type >= eFMCW400_BR24) && (type <= eFMCW400_HD4G));  }
inline bool IsPulseType( eScannerType type ) { return ((type >= eNKE_1065) && (type <= eGWTestScanner) ); }

struct tProperties                      ///  Structure for conveying radar properties (eg. version numbers, on-time, etc)
{                                       ///
    uint16_t  gwRegVersionMinor;        ///< Gateware software register interface, minor version number
    uint16_t  gwRegVersionMajor;        ///< Gateware software register interface, major version number
    uint16_t  gwVersionMinor;           ///< Gateware, minor version number
    uint16_t  gwVersionMajor;           ///< Gateware, major version number
    uint16_t  gwCompileSource;          ///< Gateware, where compiled
    uint16_t  scannerSwVersionBuild;    ///< Antenna software, build number
    uint8_t   supportsFeatures;         ///< Bit-0 == 1, implies Features list is supported
    uint8_t   _reserved;                ///< Reserved
    uint16_t  radarSwVersionMinor;      ///< Radar software, minor version number
    uint32_t  powerCycles;              ///< Number of radar power cycles
    uint16_t  driverVersionMajor;       ///< Driver software, major version number
    uint16_t  driverVersionMinor;       ///< Driver software, minor version number
    uint32_t  scannerId;                ///<
    tScannerType scannerType;           ///< One of eScannerType enum values
    uint32_t  operationTime_hour;       ///< Total transmit time in hours
    uint32_t  warmupTime_sec;           ///< Number of seconds required for scanner to warm-up
    uint32_t  maxRange_dm;              ///< Maximum radar range in 10ths of a metre
    uint16_t  scannerSwVersionMajor;    ///< Antenna software, major version number
    uint16_t  scannerSwVersionMinor;    ///< Antenna software, minor version number
    uint32_t  radarSwVersionFamily;     ///< Radar software, family version number
    uint32_t  radarSwVersionMajor;      ///< Radar software, major version number
    char16_t  buildDate[16];            ///< Date on which radar software was built
    char16_t  buildTime[16];            ///< Time at which radar software was built
    uint32_t  radarProtocolVersion;     ///< Radar NRP protocol version
    uint8_t   supportsScannerDetail;    ///< True if scanner details is supported
    uint8_t   supportsMaxStopTxSectors; ///< Maximum number of StopTx sectors supported
    uint8_t   supportsParking;          ///< True if parking the antenna at a defined bearing is supported
} BYTE_ALIGNED;


///////////////////////////////////////////////////////////////////////////////
//  Setup
///////////////////////////////////////////////////////////////////////////////

enum eUseMode                             ///  Enumeration of use mode
{
    eUseMode_Custom,
    eUseMode_Harbour,
    eUseMode_Offshore,
    eUseMode_Buoy,
    eUseMode_Weather,
    eUseMode_Bird,
    eUseMode_Netfinder,
    eUseMode_SaRT,
    eUseMode_Doppler,
    eUseMode_RTE,

    eTotalUseModes
};

typedef uint32_t tPulseWidthType;         ///< 32-bits for backward compatibility
enum ePulseWidthType
{
    PWType_Short    = 0,
    PWType_Medium   = 1,
    PWType_Long     = 2,
    PWType_XLong    = 3,
    PWType_XXLong   = 4,

    eTotalPulseWidthTypes
};

typedef uint8_t tDopplerMode;
enum eDopplerMode
{
    eDopplerOff,                          ///< No Doppler colouring
    eDopplerBoth,                         ///< Colour both approaching and receding targets
    eDopplerApproachingOnly,              ///< Only colour approaching targets
};

enum eSetupGainControls
{
    eSetupGain,
    eSetupSea,
    eSetupRain,

    eTotalSetupGains
};

typedef uint8_t tStcCurveType;
enum eStcCurveType                        ///  Enumeration of sea-conditions for STC curves
{                                         ///
    eCalm,
    eModerate,
    eRough,
    
    eTotalStcCurves
};

struct tGainControl                       ///  Structure for conveying gain-control mode and level information
{                                         /// 
    uint32_t      type;                   ///< One of eUserGainManualAuto enum values
    uint8_t       value;                  ///< Manual gain settings (valid only if \c type is eUserGainManual)
} BYTE_ALIGNED;

struct tUserGain                          ///  Structure representing user gain values
{                                         ///
    uint8_t       manualValue;            ///< Manual gain level
    int8_t        autoOffset;             ///< Auto offset value
} BYTE_ALIGNED;

struct tFTCControl                        ///  Structure for conveying FTC control information
{                                         /// 
    uint32_t      type;                   ///< unused
    uint8_t       value;                  ///< FTC level (0-255)
} BYTE_ALIGNED;

struct tGuardZone                         ///  Structure for conveying GuardZone zone setup information
{                                         /// 
    uint32_t      orientation;            ///< Relative to vessel (0) or north/absolute (1)
    uint32_t      rangeStart_m;           ///< Close range from boat (metres)
    uint32_t      rangeEnd_m;             ///< Far range from boat (metres)
    uint16_t      azimuth_ddeg;           ///< Starting angle (10ths of a degree relative to \c orientation reference)
    uint16_t      width_ddeg;             ///< Width angle (10ths of a degree - deci degrees)
} BYTE_ALIGNED;

struct tGuardZoneAlarmSetup               ///  Structure for conveying GuardZone alarm setup information
{                                         ///
    tGuardZoneAlarmType alarmType;        ///< Alarm types
    uint8_t       enabled;                ///< Alarm enabled state
} BYTE_ALIGNED;

struct tGuardZones                        ///  Structure for conveying all GuardZone setup information
{                                         ///
    uint8_t       sensitivity;            ///< Sensitivity (low 0-255 high)
    uint8_t       active[cMaxGuardZones]; ///< true if the corresponding guard-zone is enabled
    tGuardZone    zone[cMaxGuardZones];
    tGuardZoneAlarmSetup alarmType[cMaxGuardZones];
} BYTE_ALIGNED;

struct tSetup                             ///  Structure for conveying radar setup information (eg. range, gains, etc)
{                                         ///
    uint32_t      range_dm;               ///< Currently selected range (in 10ths of a metre)
    uint8_t       reserved;               ///< unused
    uint8_t       useMode;                ///< Use mode
    tGainControl  gain[eTotalSetupGains]; ///< Indexed by eSetupGainControls
    tFTCControl   ftc;                    ///< Fast-Time-Constant level
    uint32_t      tuneType;               ///< Pulse radar only
    uint8_t       coarseTune;             ///< Pulse radar only
    uint8_t       fineTune;               ///< Pulse radar only
    uint32_t      interferenceReject;     ///< Interference-Reject level (off 0-3 high)
    uint32_t      targetStretch;          ///< Target-Stretch level 
    uint32_t      targetBoost;            ///< Target-Boost, AKA target-emphasis (off 0-2 high)
    tPulseWidthType pwType;               ///< Pulse width length type enumeration
    uint32_t      pwLength_ns;            ///< Pulse width length in nanoseconds
    tGuardZones   guardzones;             ///< Guard-Zone setup
} BYTE_ALIGNED;

struct tSetupExtended                     ///  Structure for conveying extra setup information (eg. newer controls, etc)
{                                         ///
    tStcCurveType stcCurveType;           ///< STC curve type for automatic sea clutter rejection (calm 0-2 rough)
    uint8_t       localIR;                ///< Local interference-reject (off 0-3 high)
    uint8_t       fastScanMode;           ///< Faster scanner speed at shorter ranges
    tGainControl  sidelobe;               ///< Sidelobe processing state
    uint16_t      rpmX10;                 ///< Scanner rotation speed
    uint8_t       noiseReject;            ///< Noise rejection (coherent integration) (off 0-2 high)
    uint8_t       beamSharpening;         ///< Beam sharpening (off 0-3 high)
    tUserGain     sea;                    ///< Sea gain with auto offset
    uint8_t       suppressMainBang : 1;   ///< Main bang suppression flag (1 - on)
    uint8_t       reserved_1 : 7;         ///< reserved
    uint8_t       reserved_2;
    
    // version 2 fields
    tDopplerMode  dopplerMode;            ///< Doppler colouring mode
    uint16_t      dopplerThreshold_cmps;  ///< Doppler color only targets above this speed threshold
} BYTE_ALIGNED;

struct tSerialNumber                      ///  Structure for conveying full length serial-numbers
{                                         ///
    uint8_t       serialNumber[64];       ///< Nul terminated ascii serial-number, normally numeric
} BYTE_ALIGNED;


///////////////////////////////////////////////////////////////////////////////
//  Configuration
///////////////////////////////////////////////////////////////////////////////

enum {
    cMaxBlankSectors = 6                  ///< Maximum number of stop sectors able to be supported
};

struct tSigned24
{
    uint16_t  lsw;
    int8_t    msb;

    int32_t Get() const { return ((int32_t(msb) << 16) | lsw);  }
    void Set( int32_t value )
    {
        lsw = value;
        msb = (value >> 16);
    }
} BYTE_ALIGNED;

struct tBlankSector                       ///  Structure for conveying StopTx sector information
{                                         ///
    uint8_t       state;                  ///< Non-zero if this stop-tx sector is active/enabled
    uint16_t      sectorStart_ddeg;       ///< Sector start (deci-degrees, or 10ths of a degree)
    uint16_t      sectorEnd_ddeg;         ///< Sector end (deci-degrees, or 10ths of a degree)
} BYTE_ALIGNED;


struct tConfiguration                     ///  Structure for conveying radar configuration information (eg. installation params)
{                                         ///
    uint32_t      zeroRange;              ///< Pulse radars only
    uint16_t      zeroBearing_ddeg;       ///< Zero bearing offset (deci-degrees, or 10ths of a degree)
    uint16_t      parkPosition_ddeg;      ///< Parking angle of the radar (deci-degrees, or 10ths of a degree)
    uint32_t      antennaHeight_mm;       ///< Height of the antenna above the water (millimetres)
    uint32_t      cableLength;            ///< No longer supported
    uint8_t       antennaType;            ///< Antenna type (from AntennaType table)
    uint8_t       ledLevel;               ///< LED intensity level
    tSigned24     antennaOffsetX_mm;      ///< reserved (was 'antennaSubType')
    tSigned24     antennaOffsetY_mm;      ///< reserved (was 'mainBangSuppression')
    uint32_t      timedTransmitPeriod_s;  ///< Timed-transmit transmit period (seconds)
    uint32_t      timedStandbyPeriod_s;   ///< Timed-transmit standby period (seconds)
    tBlankSector  blankSectors[cMaxBlankSectors]; ///< Indexed by stop-tx sector number
    uint16_t      _3;                     ///< reserved (was wasted blank sector data)
} BYTE_ALIGNED;


///////////////////////////////////////////////////////////////////////////////
//  Navigation
///////////////////////////////////////////////////////////////////////////////

// Navigation will be supported in future SDK release
struct tNavDataItem
{
} BYTE_ALIGNED;

typedef uint16_t  tNavItemsCount;

template< unsigned MaxItems >
struct tNavDataItems
{
    static const unsigned cMaxItems = MaxItems;

    tNavItemsCount    itemCount;
    tNavDataItem      items[cMaxItems];

    tNavDataItems() : itemCount(0) {}
} BYTE_ALIGNED;

enum eNavigationState
{
    NavState_Compass,
    NavState_STW,
    NavState_CTW,
    NavState_SOG,
    NavState_COG,
    NavState_Position,

    eNavState_COUNT
};
typedef tNavDataItems< eNavState_COUNT > tNavigationState;


///////////////////////////////////////////////////////////////////////////////
//  Target Tracking
///////////////////////////////////////////////////////////////////////////////

typedef uint32_t tBearingType;
enum eBearingType
{
    eRelative   = 0,         ///< Relative to boat direction (bearing zero)
    eAbsolute   = 1,         ///< Relative to North (either magnetic or true)
    
    eTotalBearingTypes
};

typedef uint32_t tHeadingType;
enum eHeadingType 
{ 
    eNoHeading     = 0,      ///< No heading available
    eMagneticNorth = 1,      ///< Relative to magnetic north
    eTrueNorth     = 2,      ///< Relative to true north

    eTotalHeadingTypes
};

typedef uint32_t tTargetType;
enum eTargetType
{
    eTargetTypeVessel = 0,
};

typedef uint32_t tSpeedType;
enum eSpeedType
{
    eNoSpeed         = 0,    ///< Unused
    eSpeedOverGround = 1,    ///< Speed Over Ground (SOG), eg. from GPS
    eWaterSpeed      = 2,    ///< Speed relative to the Water
    
    eTotalSpeedTypes
};

typedef uint32_t tDirectionType;
enum eDirectionType
{
    eNoDirection      = 0,   ///< Unused
    eCourseOverGround = 1,   ///< Course-Over-Ground (COG), eg. from GPS
    eHeadingMagnetic  = 2,   ///< Heading is relative to magnetic-north
    eHeadingTrue      = 3,   ///< Heading is relative to true-north
    
    eTotalDirectionTypes
};

enum eMagVarType
{
    eNoMagVar         = 0,   ///< Unused
    eValidMagVar      = 1,   ///< Magnetic Variation is present, eg. from true north

    eTotalMagVarTypes
};


struct tTargetTrackingAlarmSetup      ///  Structure for conveying target-tracking alarm setup information
{                                     ///
    uint32_t  safeZoneDistance_m;     ///< Safe distance from boat, in metres
    uint32_t  safeZoneTime_sec;       ///< Safe amount of seconds before target gets within safeZoneDistance_m
} BYTE_ALIGNED;

struct tTargetTrackingProperties      ///  Structure for conveying target-tracking properties (eg. type of heading used)
{                                     ///
    tHeadingType  headingType;        ///< Compass heading type being used, one of eHeadingType enums
} BYTE_ALIGNED;

//---------------------------------------------------------------------------

typedef uint32_t tTargetState;
enum eTargetState              ///  Enumerations for Target-Tracking target states
{                              ///
    eAcquiringTarget = 0,      ///< Attempting to acquire target
    eSafeTarget      = 1,      ///< Target acquired and not on a collision course
    eDangerousTarget = 2,      ///< Target acquired and may be on a collision course
    eLostTarget      = 3,      ///< Target has been lost and needs to be cancelled an reacquired
    eAcquireFailure  = 4,      ///< Failed to acquire a target
    eOutOfRange      = 5,      ///< Target is now out of range
    eLostOutOfRange  = 6,      ///< Target lost due to staying out of range

    eFailAcquireMax  = 0x10,   ///< Acquire failed because no target IDs were free
    eFailAcquirePos  = 0x11,   ///< Acquire failed because the position was invalid

    eBadState        = 0xBAD15BAD,
};

typedef uint32_t tAcquireType;
enum eAcquireType
{
    eManualAcquired  = 0,      ///< Target was manually acquired
    eAutoAcquired    = 1,      ///< Not currently supported, any non-zero value implies automatic acquisition
};

typedef uint8_t tTargetSource;
enum eTargetSource
{
    TSource_Simulated = 0,
    TSource_Real      = 1,
};

struct tTargetInfo             ///  Structure for conveying target-tracking target information (eg. course and location)
{                              ///
    uint32_t  distance_m;      ///< Distance to target in metres
    uint32_t  bearing_ddeg;    ///< Target bearing in 10ths of a degree (deci-degrees)
    uint32_t  course_ddeg;     ///< Target course in 10ths of a degree (deci-degrees)
    uint32_t  speed_dmps;      ///< Target speed in 10ths of a metre per second (deci-metres/second)
} BYTE_ALIGNED;

struct tTrackedTarget                 ///  Structure for conveying all target-tracking target information
{                                     ///
    uint8_t       targetValid;        ///< 0x00 invalid, 0x01 valid
    tTargetSource targetSource;       ///< One of eTargetSource enum values (eg. simulated or real)
    tTargetType   targetType;         ///< One of eTargetType enum values
    uint32_t      targetID;           ///< Client assigned target-ID (provided by user in manual-acquire0
    int32_t       serverTargetID;     ///< Server assigned target-ID (negative when invalid acquire - eFailAcquire* states)
    tTargetState  targetState;        ///< One of eTargetState enum values
    tAcquireType  autoAcquired;       ///< One of eAcquireType enum values (manually or automatically acquired)

    tTargetInfo   infoRelative;       ///< Target details expressed relative to the boats speed and direction
    tTargetInfo   infoAbsolute;       ///< Target details expressed independent of the boat (relative to true north)
    uint8_t       infoAbsoluteValid;  ///< Whether 'infoAbsolute' is valid (0 absolute info invalid, 1 valid)

    int32_t       CPA_m;              ///< Closest point of approach expressed in meters
    int32_t       TCPA_sec;           ///< Time to the closest point of approach expressed in seconds
    uint8_t       towardsCPA;         ///< CPA direction (0 target moving away from CPA, 1 towards CPA)
} BYTE_ALIGNED;


//-----------------------------------------------------------------------------
#include "BytePackOff.h"

}  // namespace NRP
}  // namespace Protocol
}  // namespace Navico

#endif  // include guard
