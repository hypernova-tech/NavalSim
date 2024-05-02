#include "AISDataTypes.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/Math/CMath.h>


/// <summary>
/// class A
/// </summary>
/// <param name="id"></param>
void SClassAPositionReport::SetMessageID(INT8U id)
{
	Header.ID.MessageID = (INT8U)id;
}
void SClassAPositionReport::SetUserID(INT32U val)
{
	UserId = val;
}
void SClassAPositionReport::SetTimeStamp(INT8U val)
{
	AccRaimTimeStamp.TimeStamp = val;
}
void SClassAPositionReport::SetPositionAccuracy(bool is_low)
{
	AccRaimTimeStamp.PositionAccuracy = is_low ? 0 : 1;
}
void SClassAPositionReport::SetLat(FLOAT64 lat_deg)
{
	LatDeg = CMath::EncodeSigned32(lat_deg, LAT_SF_DEG);
}
void SClassAPositionReport::SetLon(FLOAT64 lat_deg)
{
	LongDeg = CMath::EncodeSigned32(lat_deg, LON_SF_DEG);
}

void SClassAPositionReport::SetCourseOverGround(FLOAT64 val_deg) {
	FLOAT64 val_rad = val_deg * DEGTORAD;
	if (val_rad < 0) {
		val_rad += 2 * PI;
	}
	Cog = CMath::EncodeUnsigned16(val_rad, COG_SF_RAD);
};
void SClassAPositionReport::SetSpeedOverGround(FLOAT64 val_meters_per_sec) {

	Sog = CMath::EncodeUnsigned16(val_meters_per_sec, SOG_SF_METER_PER_SEC);
};
void SClassAPositionReport::SetTrueHeading(FLOAT64 val_deg)
{
	FLOAT64 val_rad = val_deg * DEGTORAD;

	if (val_rad < 0) {
		val_rad += 2 * PI;
	}
	TrueHeading = CMath::EncodeUnsigned16(val_rad, TRUE_HEADING_SF_RAD);
};

void SClassAPositionReport::SetRateOfTurn(FLOAT64 deg_per_sec)
{
	FLOAT64 val_rad_per_sec = deg_per_sec * DEGTORAD;
	RateOfTurn = CMath::EncodeSigned16(val_rad_per_sec, TURN_RATE_SF_RAD);
};





/// <summary>
/// class B
/// </summary>
/// <param name="id"></param>

void SClassBPositionReport::SetMessageID(INT8U id)
{
	Header.ID.MessageID = (INT8U)id;
}
void SClassBPositionReport::SetUserID(INT32U val)
{
	UserId = val;
}
void SClassBPositionReport::SetTimeStamp(INT8U val)
{
	AccRaimTimeStamp.TimeStamp = val;
}
void SClassBPositionReport::SetPositionAccuracy(bool is_low)
{
	AccRaimTimeStamp.PositionAccuracy = is_low ? 0 : 1;
}
void SClassBPositionReport::SetLat(FLOAT64 lat_deg)
{
	LatDeg = CMath::EncodeSigned32(lat_deg, LAT_SF_DEG);
}
void SClassBPositionReport::SetLon(FLOAT64 lat_deg)
{
	LongDeg = CMath::EncodeSigned32(lat_deg, LON_SF_DEG);
}

void SClassBPositionReport::SetCourseOverGround(FLOAT64 val_deg) {
	FLOAT64 val_rad = val_deg * DEGTORAD;
	if (val_rad < 0) {
		val_rad += 2 * PI;
	}
	Cog = CMath::EncodeUnsigned16(val_rad, COG_SF_RAD);
};
void SClassBPositionReport::SetSpeedOverGround(FLOAT64 val_meters_per_sec) {

	Sog = CMath::EncodeUnsigned16(val_meters_per_sec, SOG_SF_METER_PER_SEC);
};
void SClassBPositionReport::SetTrueHeading(FLOAT64 val_deg)
{
	FLOAT64 val_rad = val_deg * DEGTORAD;

	if (val_rad < 0) {
		val_rad += 2 * PI;
	}
	TrueHeading = CMath::EncodeUnsigned16(val_rad, TRUE_HEADING_SF_RAD);
};




/// <summary>
/// ATON
/// </summary>
/// <param name="id"></param>
void SADISAtonReport::SetMessageID(INT8U id)
{
	Header.ID.MessageID = (INT8U)id;
}
void SADISAtonReport::SetUserID(INT32U val)
{
	UserId = val;
}
void SADISAtonReport::SetTimeStamp(INT8U val)
{
	AccRaimTimeStamp.TimeStamp = val;
}
void SADISAtonReport::SetPositionAccuracy(bool is_low)
{
	AccRaimTimeStamp.PositionAccuracy = is_low ? 0 : 1;
}
void SADISAtonReport::SetLat(FLOAT64 lat_deg)
{
	LatDeg = CMath::EncodeSigned32(lat_deg, LAT_SF_DEG);
}
void SADISAtonReport::SetLon(FLOAT64 lat_deg)
{
	LongDeg = CMath::EncodeSigned32(lat_deg, LON_SF_DEG);
}



void SADISAtonReport::SetAtonStructureLengthOrDiameter(FLOAT64 len_meter)
{
	AtonStructureLengthOrDiameter = CMath::EncodeUnsigned16(len_meter, ATON_STRUCTURE_LENGTH_DIAMTER_SF_METER);
};
void SADISAtonReport::SetAtonStructureBeamOrDiameter(FLOAT64 len_meter)
{
	AtonStructureBeamOrDiameter = CMath::EncodeUnsigned16(len_meter, ATON_STRUCTURE_BEAM_DIAMTER_SF_METER);
};

void SADISAtonReport::SetPositionReferencePointFromStarboard(FLOAT64 len_meter)
{
	PositionReferencePointFromStarboard = CMath::EncodeUnsigned16(len_meter, POSITION_REFERENCE_POINT_FROM_STARBOARD_STRUCTURE_EDGE_RADIUS_SF_METER);
};
void SADISAtonReport::SetPositionReferencePointFromTrueNorth(FLOAT64 len_meter)
{
	PositionReferencePointFromTrueNorth = CMath::EncodeUnsigned16(len_meter, POSITION_REFERENCE_POINT_FROM_TRUE_NORTH_FACING_STRUCTURE_EDGE_RADIUS_SF_METER);
};


void SADISAtonReport::SetElectronicFixingPositionDeviceType(EAISPositionFixingDeviceType val)
{
	Descrite.ElectronicFixingPositionFixingDeviceType = val;
}

void SADISAtonReport::SetATONName(char* p_name)
{
	AtonName[0] = strlen(p_name) + 2;
	AtonName[1] = 1;
	int len = strlen(p_name) > 14 ? 14 : strlen(p_name);

	for (int i = 0; i < len; i++) {
		AtonName[2 + i] = p_name[i];
	}
}



/// <summary>
/// SClassBStaticDataReportPartB functions
/// </summary>
/// <param name="id"></param>

void SClassBStaticDataReportPartB::SetMessageID(INT8U id)
{
	Header.ID.MessageID = (INT8U)id;
}

void SClassBStaticDataReportPartB::SetUserID(INT32U val)
{
	UserId = val;
}

void SClassBStaticDataReportPartB::SetTypeOfShipAndCargo(INT8U val)
{
	TypeOfShipAndCargo = val;
}

void SClassBStaticDataReportPartB::SetVendorId(char* name)
{
	memset(VendorId, 0, sizeof(VendorId));
	int len = strlen(name);
	if (len >= 7) {
		memcpy(VendorId, name, 7);
		VendorId[7] = 0;
	}
	else {
		memcpy(VendorId, name, len);
		VendorId[len] = 0;
	}
}

void SClassBStaticDataReportPartB::SetCallSign(char* name)
{
	memset(CallSign, 0, sizeof(CallSign));
	int len = strlen(name);
	if (len >= 7) {
		memcpy(CallSign, name, 7);
		CallSign[7] = 0;
	}
	else {
		memcpy(CallSign, name, len);
		CallSign[len] = 0;
	}
}

void SClassBStaticDataReportPartB::SetShipLenght(float val)
{
	ShipLength = CMath::EncodeUnsigned16(val, SHIP_LENGTH_SF_METER);
}

void SClassBStaticDataReportPartB::SetShipBeam(float val)
{
	ShipBeam = CMath::EncodeUnsigned16(val, SHIP_BEAM_SF_METER);
}

void SClassBStaticDataReportPartB::SetReferencePointFromStarboard(float val)
{
	ReferencePointFromStarboard = CMath::EncodeUnsigned16(val, REFERENCE_POINT_POSITION_FROM_STARTBOARD_SF_METER);
}

void SClassBStaticDataReportPartB::SetReferencePointPositionAftOfBow(float val)
{
	ReferencePointPositionAftOfBow = CMath::EncodeUnsigned16(val, REFERENCE_POINT_POSITION_AFTOfBO_SF_METER);
}

void SClassBStaticDataReportPartB::SetMotherShipMMSI(INT32U val)
{
	MotherShipMMSI = val;
}















void SAISClassAStaticVoyageRelatedData::SetMessageID(INT8U id)
{
	Header.ID.MessageID = (INT8U)id;
}

void SAISClassAStaticVoyageRelatedData::SetUserID(INT32U val)
{
	UserId = val;
}

void SAISClassAStaticVoyageRelatedData::SetTypeOfShipAndCargo(INT8U val)
{
	TypeOfShipAndCargo = val;
}

void SAISClassAStaticVoyageRelatedData::SetName(char* name)
{
	memset(Name, 0, sizeof(Name));
	int len = strlen(name);
	if (len >= (sizeof(Name)-1)) {
		memcpy(Name, name, sizeof(Name) - 1);
		Name[sizeof(Name) - 1] = 0;
	}
	else {
		memcpy(Name, name, len);
		Name[len] = 0;
	}
}

void SAISClassAStaticVoyageRelatedData::SetCallSign(char* name)
{
	memset(CallSign, 0, sizeof(CallSign));
	int len = strlen(name);
	if (len >= 7) {
		memcpy(CallSign, name, 7);
		CallSign[7] = 0;
	}
	else {
		memcpy(CallSign, name, len);
		CallSign[len] = 0;
	}
}

void SAISClassAStaticVoyageRelatedData::SetShipLenght(float val)
{
	ShipLength = CMath::EncodeUnsigned16(val, SHIP_LENGTH_SF_METER);
}

void SAISClassAStaticVoyageRelatedData::SetShipBeam(float val)
{
	ShipBeam = CMath::EncodeUnsigned16(val, SHIP_BEAM_SF_METER);
}

void SAISClassAStaticVoyageRelatedData::SetReferencePointFromStarboard(float val)
{
	ReferencePointFromStarboard = CMath::EncodeUnsigned16(val, REFERENCE_POINT_POSITION_FROM_STARTBOARD_SF_METER);
}

void SAISClassAStaticVoyageRelatedData::SetReferencePointPositionAftOfBow(float val)
{
	ReferencePointPositionAftOfBow = CMath::EncodeUnsigned16(val, REFERENCE_POINT_POSITION_AFTOfBO_SF_METER);
}

void SAISClassAStaticVoyageRelatedData::SetEstimatedDateOfArrival(int year, int month, int day)
{
	int total_day = (year - 1970) * 365 + month * 30 + day;
	EstimatedDateOfArrival = total_day;
}
void SAISClassAStaticVoyageRelatedData::SetEstimatedTimeOfArrival(float val)
{
	EstimatedTimeOfArrival = CMath::EncodeUnsigned16(val, ESTIMATED_TIME_OF_ARRIVAL_SF_SEC);
}

void SAISClassAStaticVoyageRelatedData::SetDraft(float val)
{
	Draft = CMath::EncodeUnsigned16(val, DRAFT_SF_METER);
}
void SAISClassAStaticVoyageRelatedData::SetDestination(char* name)
{
	memset(Destination, 0, sizeof(Destination));

	int len = strlen(name);
	if (len >= (sizeof(Destination) - 1)) {
		memcpy(Destination, name, sizeof(Destination) - 1);
		Destination[sizeof(Destination) - 1] = 0;
	}
	else {
		memcpy(Destination, name, len);
		Destination[len] = 0;
	}

}

/// <summary>
/// SClassBStaticDataReportPartA
/// </summary>
/// <param name="id"></param>
void SClassBStaticDataReportPartA::SetMessageID(INT8U id)
{
	Header.ID.MessageID = (INT8U)id;
}

void SClassBStaticDataReportPartA::SetUserID(INT32U val)
{
	UserId = val;
}

void SClassBStaticDataReportPartA::SetName(char* name)
{
	
	memset(Name, 0, sizeof(Name));
	int len = strlen(name);
	if (len >= (sizeof(Name) - 1)) {
		memcpy(Name, name, sizeof(Name) - 1);
		Name[sizeof(Name) - 1] = 0;
	}
	else {
		memcpy(Name, name, len);
		Name[len] = 0;
	}
	

}
