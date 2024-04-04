#pragma once
#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>


#define MAX_PAYLOAD_SIZE 256

#define LAT_SF_DEG 1e-7
#define LON_SF_DEG 1e-7
#define COG_SF_RAD 1e-4
#define SOG_SF_METER_PER_SEC			1e-2
#define TRUE_HEADING_SF_RAD				1e-4
#define TURN_RATE_SF_RAD				0.03125e-3
#define ATON_STRUCTURE_LENGTH_DIAMTER_SF_METER 1e-1
#define ATON_STRUCTURE_BEAM_DIAMTER_SF_METER 1e-1
#define POSITION_REFERENCE_POINT_FROM_STARBOARD_STRUCTURE_EDGE_RADIUS_SF_METER 1e-1
#define POSITION_REFERENCE_POINT_FROM_TRUE_NORTH_FACING_STRUCTURE_EDGE_RADIUS_SF_METER 1e-1


enum EAISPositionFixingDeviceType
{
	Undefined = 0,
	GPS,
	GLONASS,
	CombinedGPSGLONASS,
	LoranC,
	Chyaka,
	IntegratedNavSystem,
	Surveyed,
	Galileo,

};



struct SAISHeader {

public:

	struct {
		INT8U MessageID : 6;
		INT8U AISRepeaterIndicator : 2;

	}ID;
};


#pragma pack(push, 1)
struct SClassAPositionReport {
	SAISHeader Header;
	INT32U UserId;
	INT32U LongDeg;
	INT32U LatDeg;

	struct {
		INT8U PositionAccuracy : 1;
		INT8U AISRAIMFlag : 1;
		INT8U TimeStamp : 6;

	}AccRaimTimeStamp;
	INT16U Cog;
	INT16U Sog;
	
	INT8U CommState[3];

	
	INT16U TrueHeading;
	INT16U RateOfTurn;
	struct {
		INT32U NavStatus : 4;
		INT32U SpecialManeuverIndicator : 2;
		INT32U NMEAReserved1 : 2;
		INT32U AISSpare : 3;
		INT32U NMEAReserved2 : 5;
		INT32U SequenceId : 1;
	};

public:

	void SetMessageID(INT8U id);
	void SetTimeStamp(INT8U val);
	void SetPositionAccuracy(bool is_low);
	void SetLat(FLOAT64 lat_deg);
	void SetLon(FLOAT64 lat_deg);

	void SetCourseOverGround(FLOAT64 val_deg);
	void SetSpeedOverGround(FLOAT64 val_meters_per_sec);
	void SetTrueHeading(FLOAT64 val_deg);

	void SetRateOfTurn(FLOAT64 deg_per_sec);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SClassBPositionReport {
	SAISHeader Header;
	INT32U UserId;
	INT32U LongDeg;
	INT32U LatDeg;
	struct {
		INT8U PositionAccuracy : 1;
		INT8U AISRAIMFlag : 1;
		INT8U TimeStamp : 6;

	}AccRaimTimeStamp;
	INT16U Cog;
	INT16U Sog;
	/*	struct {
		INT32U AisCommState : 19;
		INT32U AisTransciverInfo : 5;
	}CommState;*/
	INT8U CommState[3];

	INT16U TrueHeading;

	struct {
		INT32U ReservedForRegionalApplications1 : 8;
		INT32U ReservedForRegionalApplications2 : 2;
		INT32U ClassBUnitFlag : 1; // 0: Class B Sotdma unit; 1: class B CS unit
		INT32U ClassBDisplayFlag : 1;// 0:no display available; 1: integrated with embedded display unit
		INT32U ClassBDSCFlag : 1;// 0:not equipped with DSC func; 1: equipped with DSC func
		INT32U ClassBBandFlag : 1;// 0:capable of operating over the upper 525kHz band; 1: capable of operating over all marine band
		INT32U ClassBMsg22Flag : 1;// 0:no freq managment via mes 22, 1: freq managment via mes 22
		INT32U ModeFlag : 1; // 0: station operating in autonomous mode, 1: station operating assigned mode
		INT32U CommStateSelectorFlag : 1; // 0: SOTDMA comm state, 1: ITDMA comm state flows
		INT32U NMEAReserved : 7;
		INT32U SequenceID : 1;
	};

	void SetMessageID(INT8U id);
	void SetTimeStamp(INT8U val);
	void SetPositionAccuracy(bool is_low);
	void SetLat(FLOAT64 lat_deg);
	void SetLon(FLOAT64 lat_deg);
	void SetCourseOverGround(FLOAT64 val_deg);
	void SetSpeedOverGround(FLOAT64 val_meters_per_sec);
	void SetTrueHeading(FLOAT64 val_deg);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SADISAtonReport { // aids to navigation reports
	SAISHeader Header;
	INT32U UserId;
	INT32U LongDeg;
	INT32U LatDeg;
	struct {
		INT8U PositionAccuracy : 1;
		INT8U AISRAIMFlag : 1;
		INT8U TimeStamp : 6;

	}AccRaimTimeStamp;
	INT16U AtonStructureLengthOrDiameter;
	INT16U AtonStructureBeamOrDiameter;
	INT16U PositionReferencePointFromStarboard;
	INT16U PositionReferencePointFromTrueNorth;
	struct {
		INT32U AtonType : 5;
		INT32U OffPositionIndicator : 1;
		INT32U VirtualAtonFlag : 1;
		INT32U AssistedModeFlag : 1;
		INT32U AISSpare : 1;
		INT32U ElectronicFixingPositionFixingDeviceType : 4;
		INT32U NMEAReserved1 : 3;
		INT32U AtonStatus : 8;
		INT32U AisTransciverInfo : 5;
		INT32U NMEAReserved2 : 3;

	}Descrite;
	INT8S AtonName[16];

	void SetMessageID(INT8U id);
	void SetTimeStamp(INT8U val);
	void SetPositionAccuracy(bool is_low);
	void SetLat(FLOAT64 lat_deg);
	void SetLon(FLOAT64 lat_deg);
	void SetAtonStructureLengthOrDiameter(FLOAT64 len_meter);
	void SetAtonStructureBeamOrDiameter(FLOAT64 len_meter);
	void SetPositionReferencePointFromStarboard(FLOAT64 len_meter);
	void SetPositionReferencePointFromTrueNorth(FLOAT64 len_meter);
	void SetElectronicFixingPositionDeviceType(EAISPositionFixingDeviceType val);
	void SetATONName(char* p_name);
	
	
};
#pragma pack(pop)

//AIS Class B "CS" Static Data Report, Part B
#pragma pack(push, 1)
struct SClassBStaticDataReport {
	SAISHeader Header;
	INT32U UserId;
	INT32U LongDeg;
	INT32U LatDeg;

	struct {
		INT8U PositionAccuracy : 1;
		INT8U AISRAIMFlag : 1;
		INT8U TimeStamp : 6;

	}AccRaimTimeStamp;
	INT16U Cog;
	INT16U Sog;

	INT8U CommState[3];


	INT16U TrueHeading;
	INT16U RateOfTurn;
	struct {
		INT32U NavStatus : 4;
		INT32U SpecialManeuverIndicator : 2;
		INT32U NMEAReserved1 : 2;
		INT32U AISSpare : 3;
		INT32U NMEAReserved2 : 5;
		INT32U SequenceId : 1;
	};

public:

	void SetMessageID(INT8U id);
	void SetTimeStamp(INT8U val);
	void SetPositionAccuracy(bool is_low);
	void SetLat(FLOAT64 lat_deg);
	void SetLon(FLOAT64 lat_deg);

	void SetCourseOverGround(FLOAT64 val_deg);
	void SetSpeedOverGround(FLOAT64 val_meters_per_sec);
	void SetTrueHeading(FLOAT64 val_deg);

	void SetRateOfTurn(FLOAT64 deg_per_sec);
};
#pragma pack(pop)

