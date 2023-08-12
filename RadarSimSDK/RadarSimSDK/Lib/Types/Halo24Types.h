#pragma once
#include "Primitives.h"


#define MAX_PAYLOAD_BYTE_SIZE 1550

enum ESimSDKDataIDS :INT32U  // if PacketType is command
{
	Response = 1,
	PowerOn,
	ScanOn,
	TransmitOn,
	PowerOff,
	Serials,
	UnlockKeys,
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
	INT32U Result; //0: failed, 1:ok
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

	void SetResponse(ESimSDKDataIDS id_of_response, bool is_ack)
	{
		Header.PacketType = (ESimSDKDataIDS::Response);
		Header.PayloadSize = sizeof(SCommandAckNackResponse);
		SCommandAckNackResponse res;

		res.Id = id_of_response;
		res.Result = is_ack ? 1 : 0;
		memcpy(Payload, &res, sizeof(SCommandAckNackResponse));

	}
};

