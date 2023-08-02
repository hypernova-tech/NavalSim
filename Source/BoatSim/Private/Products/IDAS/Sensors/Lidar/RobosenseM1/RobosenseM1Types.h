#pragma once
#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>
#define DATA_BLOCK_COUNT 25

struct SHeader{

public:

	union {
		INT8U Bytes[4];
		INT32U Val;
	}Header;

	union {
		INT8U Bytes[2];
		INT16U Val;
	}PaketPSN;

	union {
		INT8U Bytes[2];
		INT16U Val;
	}ProtocolVersion;

	union {
		INT8U Bytes[1];
		INT8U Val;
	}WaveMode;

	union {
		INT8U Bytes[1];
		INT8U Val;
	}TimeSyncMode;

	union {
		INT8U Bytes[10];
		struct {
			INT8U HighTimeSec[6];
			INT8U LowTimeSec[4];
		};
		unsigned int Val;
	}TimeStamps;

	union {
		INT8U Bytes[10];
	}Reserved;

	union {
		INT8U Bytes[1];
		INT8U Val;
	}LidarType;

	union {
		INT8U Bytes[1];
		INT8U Val;
	}MemsTemp;


};
struct SLidarChannelData
{

	INT8U Ch1Radius[2];
	INT8U Ch1Elevation[2];
	INT8U Ch1Azimuth[2];
	INT8U Ch1Intensity[1];
};
struct SLidarDataPacket
{
public:

	INT8U TimeOffset;
	INT8U ReturnSequence;
	SLidarChannelData ChannelData1;
	INT8U Reserved1[2];
	SLidarChannelData ChannelData2;
	INT8U Reserved2[2];
	SLidarChannelData ChannelData3;
	INT8U Reserved3[2];
	SLidarChannelData ChannelData4;
	INT8U Reserved4[2];
	SLidarChannelData ChannelData5;
	INT8U Reserved5[2];


	SLidarDataPacket()
	{

	}
};

struct SMSOPPacket
{
public:

	SHeader Header;
	SLidarDataPacket LidarDataPackets[DATA_BLOCK_COUNT];
	INT8U Tail[3];
public:
	SMSOPPacket() {

	}

	void SetTimeStamp(INT64U time_micro_sec)
	{
		INT64U seconds = (INT64U)(time_micro_sec *1e6);
		INT64U microseconds = (time_micro_sec - seconds * 1e6);
		memcpy(&Header.TimeStamps.HighTimeSec[0], &seconds, 6);
		memcpy(&Header.TimeStamps.LowTimeSec[0], &microseconds, 4);
	}

	void Init()
	{
		Header.Header.Bytes[0] = 0x55;
		Header.Header.Bytes[1] = 0xAA;
		Header.Header.Bytes[2] = 0x5A;
		Header.Header.Bytes[3] = 0xA5;

		Header.LidarType.Val = 0x10;
	}

	void SetPacketSequenceNumber(INT32U val)
	{
		Header.PaketPSN.Val = val;
	}
};