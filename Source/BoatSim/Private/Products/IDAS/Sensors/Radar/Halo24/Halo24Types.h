#pragma once
#include "Lib/Types/Primitives.h"


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
	GuardZoneSettings,
	GuardZoneAlarmControl,
	FastScanMode,
	ScannerRPM,
};

struct SSerialData
{
	INT8U SerialLen;
	INT8U SerialKey[17];
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
	INT8U Type;
	INT8U Level;
};

struct SSeaClutter {
	INT8U SeaClutterType;
	INT8U SeaClutterLevel;
	INT8U SeaClutterAutoOffset;
	INT8U	UserGainValid;
};

struct SRainClutter {
	INT8U Level;
};

struct SFastScanMode {
	INT8U Level;
};

struct SScanerRPM {
	INT32U Rpm;
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

