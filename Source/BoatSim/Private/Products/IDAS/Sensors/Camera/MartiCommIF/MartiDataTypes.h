#pragma once
#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>
#include <Lib/Utils/CUtil.h>

#define MAX_PAYLOAD_SIZE 256

#define START_OF_FRAME1 0x55
#define START_OF_FRAME2 0xAA

enum EMartiCommandReportId:INT8U
{
	LosCommand				= 1,
	LosReport				= 2,
	SensorCommand			= 3,
	SensorReport			= 4,
	DTVDefogCommand			= 0x11,
	DTVDefogReport			= 0x12,
	DTVICRCommand			= 0x13,
	DTVICRReport			= 0x14,
};



struct SMartiHeader {

public:

	INT8U StartOfFrame1;
	INT8U StartOfFrame2;
	INT8U Header;
	INT8U MessageSize;
};


#pragma pack(push, 1)
struct SMartiGenericMessage {
public:
	union {
		SMartiHeader Header;
		INT8U AllData[MAX_PAYLOAD_SIZE + sizeof(SMartiHeader)];
	}Message;


	INT8U GetCommandReportId()
	{
		return  Message.Header.Header;
	}

	INT8U* GetPayload()
	{
		return &Message.AllData[sizeof(SMartiHeader)];
	}

	bool SetMessage(INT8U id, INT8U *p_data, INT8U data_len)
	{
		
		Message.Header.StartOfFrame1 = START_OF_FRAME1;
		Message.Header.StartOfFrame2 = START_OF_FRAME2;
		Message.Header.Header = id;
		Message.Header.MessageSize = sizeof(Message.Header) + data_len;
		
		if (data_len <= MAX_PAYLOAD_SIZE) {
			memcpy(&Message.AllData[sizeof(Message.Header)], p_data, data_len);
			auto cs = ComputeChecksum();
			Message.AllData[Message.Header.MessageSize-1] = cs;

		}
		else {
			return false;
		}

		return true;
		
	}

	void SetCs(INT8U cs) 
	{
		Message.AllData[Message.Header.MessageSize-1] = cs;
	}

	INT8U GetCs()
	{
		return Message.AllData[Message.Header.MessageSize-1];
	}

	INT8U ComputeChecksum()
	{
		INT8U cs = 0;
		
		for (INT32S i = 0; i < Message.Header.MessageSize - 1; i++) {
			cs += Message.AllData[i];
		}

		cs = 255-cs + 1;

		return cs;
	}

	bool IsMessageValid()
	{
		auto cs = ComputeChecksum();
		auto curr_cs = GetCs();

		bool ret =  cs == curr_cs;

		return ret;

	}

	
};
#pragma pack(pop)


#pragma pack(push, 1)
struct SMartiLosCommandPayload
{
public:
	INT8U LosModeOfOperation;
	struct {
		INT8U ElavationRateCommandValid : 1;
		INT8U AzimuthRateCommandValid : 1;
		INT8U ElavationPositionCommandValid : 1;
		INT8U AzimuthPositionCommandValid : 1;
		INT8U Reserved : 4;

	}CommandValidty;
	INT8U ReservedBytes1[2];
	FLOAT32 ElavationRateCommand;
	FLOAT32 AzimuthRateCommand;
	FLOAT32 ElevationPositionCommand;
	FLOAT32 AzimuthPositionCommand;


	SMartiLosCommandPayload()
	{

	}
};
#pragma pack(pop)


#pragma pack(push, 1)
struct SMartiLosReportPayload
{
public:

	INT8U LosModeOfOperation;
	struct {
		INT8U ElavationRateCommandValid : 1;
		INT8U AzimuthRateCommandValid : 1;
		INT8U ElavationPositionCommandValid : 1;
		INT8U AzimuthPositionCommandValid : 1;
		INT8U Reserved : 4;

	}CommandReturnValidty;

	INT8U ReservedBytes1[2];
	FLOAT32 ElavationRate;
	FLOAT32 AzimuthRate;
	FLOAT32 ElevationPosition;
	FLOAT32 AzimuthPosition;
	SMartiLosReportPayload()
	{

	}
};
#pragma pack(pop)


enum EDTVFieldOfView:INT8U
{
	DTVFieldOfViewNoChange = 0,
	DTVFieldOfViewNarrow = 1,
	DTVFieldOfViewMiddle = 2,
	DTVFieldOfViewWide = 3,
	DTVFieldOfViewSuperWide = 4
};

enum EThermalFieldOfView:INT8U
{
	ThermalFieldOfViewNoChange = 0,
	ThermalFieldOfViewNarrow = 1,
	ThermalFieldOfViewMiddle = 2,
	ThermalFieldOfViewWide = 3,

};

enum ESystemMode:INT8U
{
	SystemModeNoChange = 0,
	SystemModeOperational = 1,
	SystemModeStandby = 2,
	SystemModeIbit = 3,
	SystemModeShutdown = 4,
	SystemModeInit = 5,
	SystemModeNotReady = 6
};

enum EDefogCommand:INT8U
{
	ReadDefogLevel = 0,
	SetDefogLevel1 = 1,
	SetDefogLevel2 = 2,
	SetDefogLevel3 = 3,
	SetDefogOff = 4,
};

enum EDefogStatus:INT8U
{
	DefogNotUsed = 0,
	DefogLevel1 = 1,
	DefogLevel2 = 2,
	DefogLevel3 = 3,
	DefogOff = 4,
};

enum ELosModOfOperation:INT8U
{
	LosModOfOperationNoChange		= 0,
	LosModOfOperationPark			= 1, // + aşağı baktırıyor
	LosModOfOperationSleveRate		= 2, // +dön
	LosModOfOperationSlevePosition  = 3, // + bak
	LosModOfOperationForward		= 4, // + kullanıyor,sıfırda kalıyor*/
	LosModOfOperationIdle			= 5,
	LosModOfOperationPoint			= 6, 
	LosModOfOperationCuedPoint		= 7, 
	LosModOfOperationScan			= 8, 
	LosModOfOperationTargetTrack		= 9,
	LosModOfOperationDriftCompensation = 10,// motor drift compensation offset girme
	LosModOfOperationStow				= 11, 
	LosModOfOperationReserved			= 12,
	LosModOfOperationStabilizasyon		= 13,// ?
	LosModOfOperationChickenHead		= 14, // aynı noktaya bakırım
};

#pragma pack(push, 1)
struct SModeContol {
	INT8U DtvAutoManualMode : 2;
	INT8U ThermalAutoManualMode : 2;
	INT8U ThermalPolarity : 2;
	INT8U Reserved : 2;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SCamControl1 {
	INT8U DtvFocus : 3;
	INT8U ThermalFocus : 3;
	INT8U ActiveVideoSelection : 2; //1: thermal 2: dtv
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SCamControl2 {
	INT8U DtvZoom : 3;
	INT8U ThermalZoom : 3;
	INT8U Reserved : 2;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SMartiSensorCommandPayload
{
public:

	
	union {
		INT8U Command;
		EDTVFieldOfView Bits;
	}DTVFieldOfView;

	union {
		INT8U Command;
		EThermalFieldOfView Bits;
	}ThermalFieldOfView;

	
	SModeContol ModeContol;

	INT8U	DTVBrightness;
	INT8U	ThermalBrightness;
	INT8U	DTVContrast;
	INT8U	ThermalContrast;

	union {
		INT8U Mode;
		EThermalFieldOfView Bits;
	}SystemMode;
	SCamControl1 CamControl1;
	SCamControl2 CamControl2;



	SMartiSensorCommandPayload()
	{

	}
};
#pragma pack(pop)



#pragma pack(push, 1)
struct SMartiSensorReportPayload
{
public:


	union {
		INT8U Val;
		EDTVFieldOfView Bits;
	}DTVFieldOfView;

	union {
		INT8U Val;
		EThermalFieldOfView Bits;
	}ThermalFieldOfView;

	struct {
		INT8U ActiveVideoStatus : 2; //1: thermal, 2: dtv
		INT8U Reserved : 1;
		INT8U DtvAutoManualMode : 2;
		INT8U ThermalAutoManualMode : 2;
	}Video;

	struct {
		INT8U SystemModeStatus : 6; // ESystemMode
		INT8U ThermalPolarity : 2; // 1 white hot, 2 black hot
	}ModeContolReturn;


	struct {
		INT8U DtvFocus : 3; // ESystemMode
		INT8U Reserved : 5;
	}FocusMode;
	
	INT8U ReservedBytes1[2];

	INT8U ThermalZoomDegreeInteger;
	INT8U ThermalZoomDegreeFraction;

	INT8U DTVZoomDegreeInteger;
	INT8U DTVZoomDegreeFraction;

	INT8U  DTVBrightness;
	INT8U  ThermalBrightness;
	INT8U  DTVContrast;
	INT8U  ThermalContrast;
	

	SMartiSensorReportPayload()
	{

	}
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SDTVDefogCommandPayload
{
public:
	union {
		INT8U Command;
		EDefogCommand Bits;
	}DTVDefogCommand;
	
	SDTVDefogCommandPayload()
	{

	}
};
#pragma pack(pop)


#pragma pack(push, 1)
struct SDTVDefogStatus
{
public:


	union {
		INT8U Command;
		EDefogStatus Bits;
	}DTVDefogStatus;




	INT8U Checksum;

	SDTVDefogStatus()
	{

	}
};
#pragma pack(pop)



#pragma pack(push, 1)
struct SDTVICRCommandPayload
{
public:


	INT8U DTVICRCommand;

	SDTVICRCommandPayload()
	{

	}
};
#pragma pack(pop)


#pragma pack(push, 1)
struct SDTVICRReportPayload
{
public:

	INT8U DTVICRStatus;

	SDTVICRReportPayload()
	{

	}
};
#pragma pack(pop)