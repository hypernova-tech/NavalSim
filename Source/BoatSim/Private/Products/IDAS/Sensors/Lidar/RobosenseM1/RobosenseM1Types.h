#pragma once
#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>


struct SHeader{

public:

	union {
		INT8U Bytes[4];
		INT32U Val;
	}Header;

	union {
		INT8U Bytes[2];
		unsigned int Val;
	}PaketPSN;

	union {
		INT8U Bytes[2];
		short Val;
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
};

struct SMSOPPacket
{
	SHeader Header;
	SLidarDataPacket LidarDataPackets;
	INT8U Tail[3];
};