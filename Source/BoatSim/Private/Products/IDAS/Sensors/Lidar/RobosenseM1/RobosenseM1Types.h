#pragma once
#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>
#include <Lib/Utils/CUtil.h>
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

	INT8U ChRadius[2];
	INT8U ChElevation[2];
	INT8U ChAzimuth[2];
	INT8U ChIntensity[1];
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
#pragma pack(push, 1)
struct SMSOPPacket
{
public:

	SHeader Header;
	SLidarDataPacket LidarDataPackets[DATA_BLOCK_COUNT];
	INT8U Tail[3];
public:
	SMSOPPacket() {
		INT32U  header_size = sizeof(Header);
		INT32U  data_packets = sizeof(LidarDataPackets);
		INT32U  tail_size = sizeof(Tail);
		INT32U total_packet = sizeof(SMSOPPacket);
	}

	void SetTimeStamp(INT64U time_micro_sec)
	{
		INT64U seconds = (INT64U)(time_micro_sec /1e6);
		INT64U microseconds = (time_micro_sec - seconds * 1e6);
		CUtil::ReverseCopyBytes((INT8U*)&seconds, Header.TimeStamps.HighTimeSec, 6);
		CUtil::ReverseCopyBytes((INT8U*)&microseconds, Header.TimeStamps.LowTimeSec, 4);

		//memcpy(&Header.TimeStamps.HighTimeSec[0], &seconds, 6);
		//memcpy(&Header.TimeStamps.LowTimeSec[0], &microseconds, 4);
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
		Header.PaketPSN.Val = htons((INT16U)val);
	}

	uint16_t htons(uint16_t value)
	{
		return ((value >> 8) & 0xFF) | ((value << 8) & 0xFF00);
	}

	void ZeroizePayload()
	{
		memset(LidarDataPackets, 0, sizeof(LidarDataPackets));
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SDIFOP
{

public:

	INT8U Header[8];
	INT8U ReservedBits;
	INT8U FrameRateSetting;
	INT8U EthernetIPSrcAddress[4];
	INT8U EthernetIPDestAddress[4];
	INT8U EthernetIPLocalMACAddress[6];
	INT16U MSOPPortNumber;
	INT16U DIFOPPortNumber;
	INT16U HorizontalFOVStartAngle;
	INT16U HorizontalFOVEndAngle;
	INT16U VerticalFOVStartAngle;
	INT16U VerticalFOVEndAngle;
	INT8S FirmwareVersionML[5];
	INT8S FirmwareVersionPS[5];
	INT8U ProductSerialNumber[6];
	INT8U WaveMode;
	INT8U TimeSyncModeSetting;
	INT8U TimeSyncStatus;
	INT8U Time[10];
	INT8U OperatingStatus[31];
	INT8U DiagnoseInformationReserved[29];
	INT8U Reserved1[60];
	INT8U Reserved2[71];

	SDIFOP* Reset()
	{
		memset(this, 0, sizeof(SDIFOP));
		return this;
	}

	SDIFOP* Init()
	{
		Header[1] = 0xA5;
		Header[1] = 0xFF;
		Header[2] = 0x00;
		Header[3] = 0x5A;
		Header[4] = 0x11;
		Header[5] = 0x11;
		Header[6] = 0x55;
		Header[7] = 0x55;

		TimeSyncModeSetting = 0x00; //lidar timing
		TimeSyncStatus = 0x1; //success sync
		return this;
		
	}

	SDIFOP* SetIPAddr(FString src_ip, FString dest_ip, FString mac_adr, INT32U msop_port_number, INT32U difop_port_number)
	{
	
		CUtil::IPAddrCopy(src_ip, EthernetIPSrcAddress, sizeof(EthernetIPSrcAddress));
		CUtil::IPAddrCopy(dest_ip, EthernetIPDestAddress, sizeof(EthernetIPDestAddress));
		CUtil::MacAddrCopy(dest_ip, EthernetIPLocalMACAddress, sizeof(EthernetIPLocalMACAddress));
		MSOPPortNumber = CUtil::LittleToBig(msop_port_number);
		DIFOPPortNumber = CUtil::LittleToBig(difop_port_number);

		return this;
	}

	SDIFOP* SetHorizontalFovStartaAngle(FLOAT32 ang_deg)
	{
		HorizontalFOVStartAngle = CUtil::LittleToBig((INT16U)(ang_deg * 100));
		return this;
	}
	SDIFOP* SetHorizontalFovEndAngle(FLOAT32 ang_deg)
	{
		HorizontalFOVEndAngle = CUtil::LittleToBig((INT16U)(ang_deg * 100));
		return this;
	}

	SDIFOP* SetVerticalFovStartaAngle(FLOAT32 ang_deg)
	{
		VerticalFOVStartAngle = CUtil::LittleToBig((INT16U)(ang_deg * 100));
		return this;
	}
	SDIFOP* SetVerticalFovEndAngle(FLOAT32 ang_deg)
	{
		VerticalFOVEndAngle = CUtil::LittleToBig((INT16U)(ang_deg * 100));
		return this;
	}

} ;
#pragma pack(pop)