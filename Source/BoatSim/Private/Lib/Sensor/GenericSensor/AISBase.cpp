// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/AISBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Math/CMath.h>
#include <Lib/Utils/CUtil.h>
#include <Lib/Navigation/CNav.h>
#include <Lib/Time/CTime.h>


void AAISBase::BeginPlay()
{
	Super::BeginPlay();
	//Rename((TEXT("ais")));
	
}


void AAISBase::InitSensor()
{
	Super::InitSensor();
	TArray<AActor*> actors;
	ASystemManagerBase::GetInstance()->QueryActors(EActorQueryArgs::AISPublishers, actors);

	if (actors.Num() > 0) {
		pAISPublisher = (AActorBase*)actors[0];
	}
	else {
		pAISPublisher = nullptr;
	}
	
	actors.Reset();

	//ASystemManagerBase::GetInstance()->QueryActors(EActorQueryArgs::AISEnabledActors, actors);
	auto owner = CUtil::GetParentActor(this);
	if (owner) {
		actors.Add(owner);
	}
	else {
		actors.Add(this); // potential error
	}
	
	for (auto actor : actors) {
		FAISEntry entry;
		entry.pActor = Cast<AActorBase>(actor);
		entry.LastTransmitTimeSec = -1;
		AisEntries.Add(entry);
	}

}

void AAISBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	UpdateAISMotionMode();
	InvalidateEntries();
	ProcessEntries();
}

double AAISBase::GetCurrentPusblishPeriod()
{

	if (IsStationary) {
		return AISMessagePublishPeriodSecStationary;
	}
	else {
		return AISMessagePublishPeriodSecMoving;
	}
}

void AAISBase::UpdateAISMotionMode()
{
	auto vel = GetActorVelocityMetersPerSec();

	if (vel.Length() > 0.5f) {
		if (IsStationary) {
			IsMotionModeChanged = true;
		}
		IsStationary = false;
	}
	else {
		if (!IsStationary) {
			IsMotionModeChanged = true;
		}
		IsStationary = true;
	}

}

void AAISBase::InvalidateEntries()
{
	if (IsMotionModeChanged) {
		for (auto& entry : AisEntries) {
			entry.LastTransmitTimeSec = -1;
		}
		IsMotionModeChanged = false;
	}

}

void AAISBase::ProcessEntries()
{
	bool is_first_time = false;
	for (auto &entry : AisEntries) {
	
		if (entry.LastTransmitTimeSec < 0) {
			entry.LastTransmitTimeSec = CTime::GetTimeSecond();
			is_first_time = true;
		}

		auto curr_time = CTime::GetTimeSecond();
		auto next_time = (entry.LastTransmitTimeSec + GetCurrentPusblishPeriod());

		if(curr_time >= next_time || is_first_time) {
			if (GetAISClassType() == 1) {
				///PublishClassAPositionReport(entry.pActor);
				if (GetShoudPublishATON()) {
					PublishATONReport(entry.pActor);
				}
				PublishAISClassAStaticVoyageRelatedData(entry.pActor);
				entry.LastTransmitTimeSec = CTime::GetTimeSecond();

			}else if (GetAISClassType() == 2) {
				////PublishClassBPositionReport(entry.pActor);
				if (GetShoudPublishATON()) {
					////PublishATONReport(entry.pActor);
				}

				///PublishClassBStaticDataReportPartA(entry.pActor);
				///PublishClassBStaticDataReportPartB(entry.pActor);
				
				entry.LastTransmitTimeSec = CTime::GetTimeSecond();
			}
		}
		
	}

}

void AAISBase::SendMessageViaAISPusblisher(INT32U src_addr, INT32U prio, INT32U png, void* p_data, INT32S data_size)
{
	if (pAISPublisher != nullptr) {
		AAISPublisher* p_pub = (AAISPublisher*)pAISPublisher;
		p_pub->SendAISData(src_addr, prio, png, p_data, data_size);
	}	

}

double AAISBase::GetCourseOverGround(AActor* Actor)
{
	FVector Velocity = CUtil::GetActorVelocityMetersPerSec(Actor);
	Velocity.Normalize();
	double COGInDegrees = CNav::ComputeCourseOverGroundDeg(Velocity);
	return COGInDegrees;
}

#define offsetof(type, field) ((size_t) &(((type *)0)->field))
// class A static 5
// class B part A static 24, PGN: 129809
// class B part b static 24  PGN: 129810


void AAISBase::PublishClassAPositionReport(AActorBase* p_act)
{
	SClassAPositionReport report;
	memset(&report, 0, sizeof(SClassAPositionReport));
	FVector pos			= p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang		= CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel			= p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel		= p_act->GetActorAngularVelocityRPYDegPerSec();


	report.SetMessageID(1);
	report.SetUserID(AISUserId);
	report.SetLat(pos.X);
	report.SetLon(pos.Y);
	report.SetPositionAccuracy(false);
	report.SetSpeedOverGround(vel.Length());
	report.SetCourseOverGround(GetCourseOverGround(p_act));
	report.SetTrueHeading(rpy_ang.Z);
	report.SetRateOfTurn(ang_vel.Z);




	/*
	report.SetMessageID(1);
	report.SetLat(42.3456789);
	report.SetLon(-42.3456789);
	report.SetPositionAccuracy(false);
	report.SetSpeedOverGround(123.123456);
	report.SetCourseOverGround(16.123456);
	report.SetTrueHeading(96.87456);
	report.SetRateOfTurn(-10.123456);
	*/


	
	SendMessageViaAISPusblisher(AISSrcAddr, AISDefaultMessagePriority, EAISPNGIDs::ClassAPositionReportPNG, &report, sizeof(SClassAPositionReport));

}



void AAISBase::PublishClassBPositionReport(AActorBase* p_act)
{

	SClassBPositionReport report;
	memset(&report, 0, sizeof(SClassBPositionReport));
	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();

	report.SetMessageID(18);
	report.SetUserID(AISUserId);
	report.SetLat(pos.X);
	report.SetLon(pos.Y);
	report.SetPositionAccuracy(false);
	report.SetSpeedOverGround(vel.Length());
	report.SetCourseOverGround(GetCourseOverGround(p_act));
	report.SetTrueHeading(rpy_ang.Z);
	SendMessageViaAISPusblisher(AISSrcAddr, AISDefaultMessagePriority, EAISPNGIDs::ClassBPositionReportPNG, &report, sizeof(SClassBPositionReport));

}



/// <summary>
/// todo aton type
/// </summary>
/// <param name="p_act"></param>
void AAISBase::PublishATONReport(AActorBase* p_act)
{
	char temp[14];
	SADISAtonReport report;
	memset(&report, 0, sizeof(SADISAtonReport));
	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();

	report.SetMessageID(21);
	report.SetUserID(AISUserId);
	report.SetAtonType(AISAtonType);
	report.SetLat(pos.X);
	report.SetLon(pos.Y);
	
	report.SetPositionAccuracy(false);
	report.SetElectronicFixingPositionDeviceType(EAISPositionFixingDeviceType::CombinedGPSGLONASS);

	FVector size = p_act->GetActorComputedActorSizeMeter();


	report.SetAtonStructureLengthOrDiameter(size.X);
	report.SetAtonStructureBeamOrDiameter(size.Y);
	report.SetPositionReferencePointFromStarboard(AISReferencePointFromStarboard);
	report.SetPositionReferencePointFromTrueNorth(ReferencePointPositionAftOfBow); //todo fixme

	CUtil::FStringToAsciiChar(AisAtonName, temp, 14);
	report.SetATONName(temp);



	SendMessageViaAISPusblisher(AISSrcAddr, AISDefaultMessagePriority, EAISPNGIDs::ATONReportPNG, &report, sizeof(SADISAtonReport));
}



void AAISBase::PublishClassBStaticDataReportPartB(AActorBase* p_act)
{

	SClassBStaticDataReportPartB report;
	memset(&report, 0, sizeof(SClassBStaticDataReportPartB));

	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();
	char temp[8];

	report.SetMessageID(24);
	report.SetUserID(AISUserId);
	report.SetTypeOfShipAndCargo(ShipCargoType);

	CUtil::FStringToAsciiChar(AISVendorId, temp, 8);
	report.SetVendorId(temp);

	CUtil::FStringToAsciiChar(AISCallSign, temp, 8);
	report.SetCallSign(temp);

	FVector size = p_act->GetActorComputedActorSizeMeter();

	report.SetShipLenght(size.X);
	report.SetShipBeam(size.Y);
	report.SetReferencePointFromStarboard(AISReferencePointFromStarboard);
	report.SetReferencePointPositionAftOfBow(ReferencePointPositionAftOfBow);
	report.SetMotherShipMMSI(AISMMSI);

	SendMessageViaAISPusblisher(AISSrcAddr, AISDefaultMessagePriority, EAISPNGIDs::ClassBStaticDataReportPartBPNG, &report, sizeof(SClassBStaticDataReportPartB));
}


void AAISBase::PublishClassBStaticDataReportPartA(AActorBase* p_act)
{

	SClassBStaticDataReportPartA report;
	memset(&report, 0, sizeof(SClassBStaticDataReportPartA));

	char temp[21];

	report.SetMessageID(24);
	report.SetUserID(AISUserId);

	CUtil::FStringToAsciiChar(AISName, temp, 21);
	report.SetName(temp);
	

	SendMessageViaAISPusblisher(AISSrcAddr, AISDefaultMessagePriority, EAISPNGIDs::ClassBStaticDataReportPartAPNG, &report, sizeof(SClassBStaticDataReportPartA));
}

void AAISBase::PublishAISClassAStaticVoyageRelatedData(AActorBase* p_act)
{

	SAISClassAStaticVoyageRelatedData report;
	memset(&report, 0, sizeof(SAISClassAStaticVoyageRelatedData));

	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();
	char temp[20];

	report.SetMessageID(5);
	report.SetUserID(AISUserId);
	report.SetTypeOfShipAndCargo(ShipCargoType);

	CUtil::FStringToAsciiChar(AISName, temp, sizeof(report.Name));
	report.SetName(temp);

	CUtil::FStringToAsciiChar(AISCallSign, temp, sizeof(report.CallSign));
	report.SetCallSign(temp);

	FVector size = p_act->GetActorComputedActorSizeMeter();

	report.SetShipLenght(size.X);
	report.SetShipBeam(size.Y);
	report.SetReferencePointFromStarboard(AISReferencePointFromStarboard);
	report.SetReferencePointPositionAftOfBow(ReferencePointPositionAftOfBow);

	report.SetEstimatedDateOfArrival(FMath::CeilToInt32(AISDataOfArrivalYMD.X), FMath::CeilToInt32(AISDataOfArrivalYMD.Y), FMath::CeilToInt32(AISDataOfArrivalYMD.Z));
	report.SetEstimatedTimeOfArrival(AISTimeOfArrival);

	FVector origin;
	FVector extend;

	p_act->GetActorBounds(true, origin, extend, false);
	double lowest_pt = origin.Z - extend.Z;
	double draft = 0 - lowest_pt;

	if (draft < 0) {
		draft = 0;
	}

	report.SetDraft(TOW(draft)); //todo fixme

	CUtil::FStringToAsciiChar(AISDestination, temp, sizeof(report.Destination));
	report.SetDestination(temp);
	//CUtil::DebugLog("timex: "+CUtil::FloatToString(CTime::GetTimeSecond()));
	SendMessageViaAISPusblisher(AISSrcAddr, AISDefaultMessagePriority, EAISPNGIDs::AISClassAStaticVoyageRelatedDataPNG,&report, sizeof(SAISClassAStaticVoyageRelatedData));
}


int AAISBase::GetAISClassType()
{
	return AISClassType;
}

void AAISBase::SetAISClassType(int val)
{
	AISClassType = val;
}

float AAISBase::GetAISMessagePublishPeriodSecStationary()
{
	return AISMessagePublishPeriodSecStationary;
}

void AAISBase::SetAISMessagePublishPeriodSecStationary(float val)
{
	AISMessagePublishPeriodSecStationary = val;
}

float AAISBase::GetAISMessagePublishPeriodSecMoving()
{
	return AISMessagePublishPeriodSecMoving;

}
void AAISBase::SetAISMessagePublishPeriodSecMoving(float val)
{
	AISMessagePublishPeriodSecMoving = val;
}

bool AAISBase::GetShoudPublishATON()
{
	return ShoudPublishATON;
}

void AAISBase::SetShoudPublishATON(bool val)
{
	ShoudPublishATON = val;
}

void AAISBase::SetAISMMSI(int val)
{
	AISMMSI = val;
}

int AAISBase::GetAISMMSI()
{
	return AISMMSI;
}

void AAISBase::SetAISName(FString val)
{
	AISName = val;
}

FString AAISBase::GetAISName()
{
	return AISName;
}

void AAISBase::SetDateOfArrivalYearMonthDay(FVector val)
{
	AISDataOfArrivalYMD = val;
}

FVector AAISBase::GetDateOfArrivalYearMonthDay()
{
	return AISDataOfArrivalYMD;
}

void AAISBase::SetTimeOfArrivalSec(double val)
{
	AISTimeOfArrival = val;
}

double AAISBase::GetTimeOfArrivalSec()
{
	return AISTimeOfArrival;
}

void AAISBase::SetAISVendorId(FString val)
{
	AISVendorId = val;
}

FString AAISBase::GetAISVendorId()
{
	return AISVendorId;
}

void AAISBase::SetAISCallSign(FString val)
{
	AISCallSign = val;
}

FString AAISBase::GetAISCallSign()
{
	return AISCallSign;
}

void AAISBase::SetAISReferencePointFromStarboard(double val)
{
	AISReferencePointFromStarboard = val;
}

double AAISBase::GetAISReferencePointFromStarboard()
{
	return AISReferencePointFromStarboard;
}

void AAISBase::SetReferencePointPositionAftOfBow(double val)
{
	ReferencePointPositionAftOfBow = val;
}

double AAISBase::GetReferencePointPositionAftOfBow()
{
	return ReferencePointPositionAftOfBow;
}

void AAISBase::SetAISUserId(int val)
{
	AISUserId = val;
}

int AAISBase::GetAISUserId()
{
	return AISUserId;
}

void AAISBase::SetShipCargoType(int val)
{
	ShipCargoType = val;
}

int AAISBase::GetShipCargoType()
{
	return ShipCargoType;
}

void AAISBase::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);
	FString line;

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISUserId, AISUserId);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISClassType, GetAISClassType());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISShouldPublishAton, GetShoudPublishATON());
	p_save_loader->AddLine(line);


	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISMessagePublishPeriodSecStationary, GetAISMessagePublishPeriodSecStationary());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISMessagePublishPeriodSecMoving, GetAISMessagePublishPeriodSecMoving());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISMMSI, AISMMSI);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISShipOrCargoType, ShipCargoType);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISName, AISName);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISVendorId, AISVendorId);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISCallSign, AISCallSign);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISReferencePointFromStarboard, AISReferencePointFromStarboard);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::ReferencePointPositionAftOfBow, ReferencePointPositionAftOfBow);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISDateOfArrival, AISDataOfArrivalYMD);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISTimeOfArrival, AISTimeOfArrival);
	p_save_loader->AddLine(line);


}

void AAISBase::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);
	data.Add(CCLICommandManager::AISUserId, AISUserId);
	data.Add(CCLICommandManager::AISClassType, GetAISClassType());
	data.Add(CCLICommandManager::AISShouldPublishAton, GetShoudPublishATON());
	data.Add(CCLICommandManager::AISMessagePublishPeriodSecStationary, GetAISMessagePublishPeriodSecStationary());
	data.Add(CCLICommandManager::AISMessagePublishPeriodSecMoving, GetAISMessagePublishPeriodSecMoving());

	data.Add(CCLICommandManager::AISMMSI, AISMMSI);
	data.Add(CCLICommandManager::AISName, AISName);
	data.Add(CCLICommandManager::AISVendorId, AISVendorId);
	data.Add(CCLICommandManager::AISCallSign, AISCallSign);
	data.Add(CCLICommandManager::AISShipOrCargoType, ShipCargoType);
	data.Add(CCLICommandManager::AISReferencePointFromStarboard, AISReferencePointFromStarboard);
	data.Add(CCLICommandManager::ReferencePointPositionAftOfBow, ReferencePointPositionAftOfBow);
	data.Add(CCLICommandManager::AISDateOfArrival, AISDataOfArrivalYMD);
	data.Add(CCLICommandManager::AISTimeOfArrival, AISTimeOfArrival);


}
