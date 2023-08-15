#pragma once
#include "Primitives.h"



#define MAX_PAYLOAD_BYTE_SIZE 1550

enum ESimSDKDataIDS :INT32U  // if PacketType is command
{
	Response = 1,
	PowerControl,
	ScanOn,
	TransmitControl,
	RangeControl,
	Serials,
	UnlockKeys,
	ConnectRadar,
	DisconnectRadar,
	GainControl,
	SeaClutterControl,
	RainClutterControl,
	SectorBlanking,
	RadarDirection,
	GuardZoneControl,
	FastScanMode,
	ScannerRPM,
	RadarState,
	RadarSetup,
};

struct SSerialData
{
public:
	INT8U SerialLen;
	INT8U SerialKey[17];

	void SetSerial(char* p_serial, INT32U len)
	{
		for (INT32U i = 0; i < len; i++) {
			SerialKey[i] = p_serial[i];
		}
		SerialLen = len;

	}
};

struct SSIMSDKCommands
{
	ESimSDKDataIDS DataID;

};
struct SUnlockKeysPayload
{
	INT8U SerialKey[128];
	INT8U SerialLen;

	INT8U UnlockKey[128];
	INT8U UnlockKeyLen;
};

struct SCommandAckNackResponse
{
	ESimSDKDataIDS Id;
	INT8U Result; //0: failed, 1:ok
	INT8U StreamNo;
	SSerialData SerialData;
};

enum ERadarState :INT8U
{
	eOff = 0,
	eStandby = 1,
	eTransmit = 2,
	eWarming = 3,
	eNoScanner = 4,
	eDetectingScanner = 5,

};

struct SRadarState
{
	SSerialData SerialData;
	ERadarState State;

};

BYTE_ALIGNED_BEGIN
struct SGainControl                       ///  Structure for conveying gain-control mode and level information
{                                         /// 
	INT32U      type;                   ///< One of eUserGainManualAuto enum values
	INT8U       value;                  ///< Manual gain settings (valid only if \c type is eUserGainManual)
} BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SFTCControl                        ///  Structure for conveying FTC control information
{                                         /// 
	INT32U      type;                   ///< unused
	INT8U       value;                  ///< FTC level (0-255)
} BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SGuardZone                         ///  Structure for conveying GuardZone zone setup information
{                                         /// 
	uint32_t      orientation;            ///< Relative to vessel (0) or north/absolute (1)
	uint32_t      rangeStart_m;           ///< Close range from boat (metres)
	uint32_t      rangeEnd_m;             ///< Far range from boat (metres)
	uint16_t      azimuth_ddeg;           ///< Starting angle (10ths of a degree relative to \c orientation reference)
	uint16_t      width_ddeg;             ///< Width angle (10ths of a degree - deci degrees)
} BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SGuardZoneAlarmSetup               ///  Structure for conveying GuardZone alarm setup information
{                                         ///
	INT32U alarmType;        ///< Alarm types
	uint8_t       enabled;                ///< Alarm enabled state
}BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SGuardZones                        ///  Structure for conveying all GuardZone setup information
{                                         ///
	uint8_t       sensitivity;            ///< Sensitivity (low 0-255 high)
	uint8_t       active[2]; ///< true if the corresponding guard-zone is enabled
	SGuardZone    zone[2];
	SGuardZoneAlarmSetup alarmType[2];
} BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SRadarSetupData {

public:

	INT32U      range_dm;               ///< Currently selected range (in 10ths of a metre)
	INT8U       reserved;               ///< unused
	INT8U       useMode;                ///< Use mode
	SGainControl gain[3];
	SFTCControl   ftc;                    ///< Fast-Time-Constant level
	INT32U      tuneType;               ///< Pulse radar only
	INT8U       coarseTune;             ///< Pulse radar only
	INT8U       fineTune;               ///< Pulse radar only
	INT32U      interferenceReject;     ///< Interference-Reject level (off 0-3 high)
	INT32U      targetStretch;          ///< Target-Stretch level 
	INT32U      targetBoost;            ///< Target-Boost, AKA target-emphasis (off 0-2 high)
	INT32U		pwType;					///< Pulse width length type enumeration
	INT32U      pwLength_ns;            ///< Pulse width length in nanoseconds
	SGuardZones   guardzones;             ///< Guard-Zone setup

	SRadarSetupData()
	{
		memset(this, 0, sizeof(SRadarSetupData));

	}

}BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SRadarSetupPayload {
public:
	SSerialData SerialData;
	SRadarSetupData RadarSetupData;

	SRadarSetupPayload()
	{
		memset(this, 0, sizeof(SRadarSetupPayload));

	}
}BYTE_ALIGNED_END;


struct SConnectRadar {
	SSerialData SerialData;
	INT8U ImageStreamNo;

public:
	SConnectRadar()
	{
		memset(this, 0, sizeof(SConnectRadar));
	}
};
struct SPowerControl {
	SSerialData SerialData;
	INT8U PowerOn;
};

struct STransmitControl {
	SSerialData SerialData;
	INT8U TransmitOn;
};

struct SRangeControl {
	SSerialData SerialData;
	INT32U RangeMeter;
};

struct SGainMode {
	SSerialData SerialData;
	INT8U Type;
	INT8U Level;
};

struct SSeaClutter {
	SSerialData SerialData;
	INT8U SeaClutterType;
	INT8U SeaClutterLevel;
	INT8U SeaClutterAutoOffset;
	INT8U	UserGainValid;
};


struct SRainClutter {
	SSerialData SerialData;
	INT8U Level;
};

struct SFastScanMode {
	SSerialData SerialData;
	INT8U Level;
};

struct SScanerRPM {
	SSerialData SerialData;
	INT32U Rpm;
};

BYTE_ALIGNED_BEGIN
struct SGuardZoneData {

public:

	SGuardZoneData()
	{
		memset(this, 0, sizeof(SGuardZoneData));
	}



	enum EGuardZoneAlarmType
	{
		eGZAlarmEntry = 0x0,
		eGZAlarmExit = 0x1,
		eGZAlarmBoth = 0x2,

		eTotalGuardZoneAlarmTypes
	};



	struct {
		INT8U IsValid;
		INT8U Zone;
		INT8U State;
	}ZoneControl;

	struct {
		INT8U IsValid;
		INT8U Zone;
		INT32U StartRangeMeter;
		INT32U EndRangeMeter;
		INT16U BearingDeg;
		INT16U BeaeingWidthDeg;

	}ZoneSetup;

	struct {
		INT8U IsValid;
		INT8U Zone;
		INT8U ZoneAlarmType; //eGuardZoneAlarmType
	}ZoneAlarmSetup;

	struct {
		INT8U IsValid;
		INT8U Zone;
		INT8U SuppressAlarm; //0 suppress
	}ZoneAlarmSuppress;

	struct {
		INT8U IsValid;
		INT8U Zone;
		INT8U ZoneAlarmType; //eGuardZoneAlarmType
	}ZoneAlarmCancel;

	struct {
		INT8U IsValid;
		INT8U Level;
	}AlarmSensitivty;



}BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SGuardZonePayload {

public:

	SGuardZonePayload()
	{
		memset(this, 0, sizeof(SGuardZonePayload));
	}
	SSerialData SerialData;
	SGuardZoneData GuardZoneData;

	void SetZoneControl(bool is_valid, INT8U zone, bool state)
	{
		GuardZoneData.ZoneControl.IsValid = is_valid;
		GuardZoneData.ZoneControl.Zone = zone;
		GuardZoneData.ZoneControl.State = state ? 1 : 0;
	}

	void SetZoneSetup(bool is_valid, INT8U zone, INT32U start_range_meter, INT32U end_range_meter, INT16U bearing_deg, INT16U bearing_width_deg)
	{
		GuardZoneData.ZoneSetup.IsValid = is_valid;
		GuardZoneData.ZoneSetup.Zone = zone;
		GuardZoneData.ZoneSetup.StartRangeMeter = start_range_meter;
		GuardZoneData.ZoneSetup.EndRangeMeter = end_range_meter;
		GuardZoneData.ZoneSetup.BearingDeg = bearing_deg;
		GuardZoneData.ZoneSetup.BeaeingWidthDeg = bearing_width_deg;
	}

	void SetZoneAlarmSetup(bool is_valid, INT8U zone, INT8U alarm_type)
	{
		GuardZoneData.ZoneAlarmSetup.IsValid = is_valid;
		GuardZoneData.ZoneAlarmSetup.Zone = zone;
		GuardZoneData.ZoneAlarmSetup.ZoneAlarmType = alarm_type;
	}

	void SetZoneAlarmSuppress(bool is_valid, INT8U zone, bool state)
	{
		GuardZoneData.ZoneAlarmSuppress.IsValid = is_valid;
		GuardZoneData.ZoneAlarmSuppress.Zone = zone;
		GuardZoneData.ZoneAlarmSuppress.SuppressAlarm = state ? 0 : 1;
	}

	void SetZoneAlarmCancel(bool is_valid, INT8U zone, INT8U alarm_type)
	{
		GuardZoneData.ZoneAlarmCancel.IsValid = is_valid;
		GuardZoneData.ZoneAlarmCancel.Zone = zone;
		GuardZoneData.ZoneAlarmCancel.ZoneAlarmType = alarm_type;
	}
	void SetAlarmSensitivty(bool is_valid, INT8U level)
	{
		GuardZoneData.AlarmSensitivty.IsValid = is_valid;
		GuardZoneData.AlarmSensitivty.Level = level;

	}

}BYTE_ALIGNED_END;

BYTE_ALIGNED_BEGIN
struct SSectorBlankingData
{
	struct {
		INT8U IsValid;
		INT8U SectorId;
		INT8U State;
	}SectorControl;

	struct {
		INT8U IsValid;
		INT8U SectorId;
		INT8U State;
		INT16U StartBearingDeg;
		INT16U EndBearingDeg;
	}SectorSetup;

}BYTE_ALIGNED_END;


struct SSectorBlankingPayload
{
public:

	SSectorBlankingPayload()
	{
		memset(this, 0, sizeof(SGuardZonePayload));
	}


	SSerialData Serial;
	SSectorBlankingData BlankingData;


	void SetSectorControl(bool is_valid, INT8U sector_id, bool state)
	{
		BlankingData.SectorControl.IsValid = is_valid;
		BlankingData.SectorControl.SectorId = sector_id;
		BlankingData.SectorControl.State = state;
	}

	void SetSectorSetup(bool is_valid, INT8U sector_id, bool state, INT16U start_bearing_deg, INT16U end_bearing_deg)
	{
		BlankingData.SectorSetup.IsValid = is_valid;
		BlankingData.SectorSetup.SectorId = sector_id;
		BlankingData.SectorSetup.State = state;
		BlankingData.SectorSetup.StartBearingDeg = start_bearing_deg;
		BlankingData.SectorSetup.EndBearingDeg = end_bearing_deg;
	}
};



struct SRadarSimSDKPacket
{
	struct {
		ESimSDKDataIDS PacketType;
		INT32U PayloadSize;
	}Header;

	INT8U Payload[MAX_PAYLOAD_BYTE_SIZE + 1];
public:

	void Init()
	{
		Payload[Header.PayloadSize] = 0;
	}
	INT32U GetHeaderSize()
	{
		return sizeof(Header);
	}

	INT32U GetPayloadSize()
	{
		return Header.PayloadSize;
	}

	INT32U GetTransmitSize()
	{
		return GetHeaderSize() + Header.PayloadSize;
	}

	void SetID(ESimSDKDataIDS id)
	{
		Header.PacketType = id;
	}

	void SetPayload(ESimSDKDataIDS id, INT8U* p_data, INT32U payload_size_bytes)
	{
		if (payload_size_bytes > MAX_PAYLOAD_BYTE_SIZE) {
			return;
		}
		memcpy(Payload, p_data, payload_size_bytes);
		Header.PacketType = id;
		Header.PayloadSize = payload_size_bytes;
		Payload[payload_size_bytes] = 0;
	}
	void SetPayloadSize(INT32U payload_size_bytes)
	{
		if (payload_size_bytes > MAX_PAYLOAD_BYTE_SIZE) {
			return;
		}
		Header.PayloadSize = payload_size_bytes;
		Payload[payload_size_bytes] = 0;
	}
	void SetPayload(INT8U* p_data, INT32U payload_size_bytes)
	{
		if (payload_size_bytes > MAX_PAYLOAD_BYTE_SIZE) {
			return;
		}
		memcpy(Payload, p_data, payload_size_bytes);
		Header.PayloadSize = payload_size_bytes;
		Payload[payload_size_bytes] = 0;
	}

	void SetResponse(ESimSDKDataIDS id_of_response, char* p_serial, bool is_ack, INT8U stream_no = 0)
	{
		Header.PacketType = (ESimSDKDataIDS::Response);
		Header.PayloadSize = sizeof(SCommandAckNackResponse);
		SCommandAckNackResponse res;

		res.Id = id_of_response;
		res.Result = is_ack ? 1 : 0;
		res.StreamNo = stream_no;
		strcpy((char*)res.SerialData.SerialKey, p_serial);
		res.SerialData.SerialLen = strlen(p_serial);
		memcpy(Payload, &res, sizeof(SCommandAckNackResponse));

	}

	template<typename T>
	void PrepareMessage(ESimSDKDataIDS id, T* p_payload) {
		if (sizeof(T) > MAX_PAYLOAD_BYTE_SIZE) {
			return;
		}
		memcpy(Payload, (INT8U*)p_payload, sizeof(T));
		Header.PacketType = id;
		Header.PayloadSize = sizeof(T);
		Payload[sizeof(T)] = 0;
	}
};
