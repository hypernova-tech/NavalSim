// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/AISBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Math/CMath.h>
#include <Lib/Utils/CUtil.h>

void AAISBase::BeginPlay()
{
	Super::BeginPlay();
	//Rename((TEXT("ais")));
	
}


void AAISBase::InitSensor()
{
	Super::InitSensor();
	
	TArray<AActor*> actors;

	//ASystemManagerBase::GetInstance()->QueryActors(EActorQueryArgs::AISEnabledActors, actors);
	auto owner = CUtil::GetParentActor(this);
	if (owner) {
		actors.Add(owner);
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
	ProcessEntries();
}

void AAISBase::ProcessEntries()
{
	for (auto &entry : AisEntries) {
	
		if (entry.LastTransmitTimeSec < 0) {
			entry.LastTransmitTimeSec = FApp::GetCurrentTime();
		}

		auto curr_time = FApp::GetCurrentTime();
		auto next_time = (entry.LastTransmitTimeSec + GetAISMessagePublishPeriodSec());

		if(curr_time >= next_time) {
			if (GetAISClassType() == 1) {
				PublishClassAPositionReport(entry.pActor);
				if (GetShoudPublishATON()) {
					PublishATONReport(entry.pActor);
				}
				PublishAISClassAStaticVoyageRelatedData(entry.pActor);
				entry.LastTransmitTimeSec = FApp::GetCurrentTime();

			}else if (GetAISClassType() == 2) {
				PublishClassBPositionReport(entry.pActor);
				if (GetShoudPublishATON()) {
					PublishATONReport(entry.pActor);
				}

				PublishClassBStaticDataReportPartA(entry.pActor);
				PublishClassBStaticDataReportPartB(entry.pActor);
				
				entry.LastTransmitTimeSec = FApp::GetCurrentTime();
			}
		}
		
	}

}

double AAISBase::GetCourseOverGround(AActor* Actor)
{
	FVector Velocity = Actor->GetVelocity();
	// Assuming North is along the positive Y-axis. Adjust according to your game's coordinate system.
	double COG = FMath::Atan2(Velocity.X, Velocity.Y); // Returns radians
	double COGInDegrees = FMath::RadiansToDegrees(COG);

	// Normalize the angle to [0, 360) range
	if (COGInDegrees < 0)
	{
		COGInDegrees += 360.0f;
	}

	return COGInDegrees;
}
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



	pCommIF->SendData(&report, sizeof(SClassAPositionReport));

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
	report.SetLat(pos.X);
	report.SetLon(pos.Y);
	report.SetPositionAccuracy(false);
	report.SetSpeedOverGround(vel.Length());
	report.SetCourseOverGround(GetCourseOverGround(p_act));
	report.SetTrueHeading(rpy_ang.Z);
	pCommIF->SendData(&report, sizeof(SClassBPositionReport));

}
/// <summary>
/// todo aton type
/// </summary>
/// <param name="p_act"></param>
void AAISBase::PublishATONReport(AActorBase* p_act)
{

	SADISAtonReport report;
	memset(&report, 0, sizeof(SADISAtonReport));
	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();

	report.SetMessageID(21);
	report.SetLat(pos.X);
	report.SetLon(pos.Y);
	report.SetPositionAccuracy(false);
	report.SetElectronicFixingPositionDeviceType(EAISPositionFixingDeviceType::CombinedGPSGLONASS);
	pCommIF->SendData(&report, sizeof(SADISAtonReport));
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

	pCommIF->SendData(&report, sizeof(SClassBStaticDataReportPartB));
}


void AAISBase::PublishClassBStaticDataReportPartA(AActorBase* p_act)
{

	SClassBStaticDataReportPartA report;
	memset(&report, 0, sizeof(SClassBStaticDataReportPartA));

	char temp[21];

	report.SetMessageID(24);


	CUtil::FStringToAsciiChar(AISName, temp, 21);
	report.SetName(temp);
	

	pCommIF->SendData(&report, sizeof(SClassBStaticDataReportPartA));
}

void AAISBase::PublishAISClassAStaticVoyageRelatedData(AActorBase* p_act)
{

	SAISClassAStaticVoyageRelatedData report;
	memset(&report, 0, sizeof(SAISClassAStaticVoyageRelatedData));

	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();
	char temp[21];

	report.SetMessageID(24);


	CUtil::FStringToAsciiChar(AISName, temp, 21);
	report.SetName(temp);

	CUtil::FStringToAsciiChar(AISCallSign, temp, 8);
	report.SetCallSign(temp);

	FVector size = p_act->GetActorComputedActorSizeMeter();

	report.SetShipLenght(size.X);
	report.SetShipBeam(size.Y);
	report.SetReferencePointFromStarboard(AISReferencePointFromStarboard);
	report.SetReferencePointPositionAftOfBow(ReferencePointPositionAftOfBow);

	report.SetEstimatedDateOfArrival(FMath::CeilToInt32(AISDataOfArrivalYMD.X), FMath::CeilToInt32(AISDataOfArrivalYMD.Y), FMath::CeilToInt32(AISDataOfArrivalYMD.Z));
	report.SetEstimatedTimeOfArrival(AISTimeOfArrival);
	report.SetDraft(0); //todo fixme

	pCommIF->SendData(&report, sizeof(SAISClassAStaticVoyageRelatedData));
}


int AAISBase::GetAISClassType()
{
	return AISClassType;
}

void AAISBase::SetAISClassType(int val)
{
	AISClassType = val;
}

float AAISBase::GetAISMessagePublishPeriodSec()
{
	return AISMessagePublishPeriodSec;
}

void AAISBase::SetAISMessagePublishPeriodSec(float val)
{
	AISMessagePublishPeriodSec = val;
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

void AAISBase::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);
	FString line;

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISClassType, GetAISClassType());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISShouldPublishAton, GetShoudPublishATON());
	p_save_loader->AddLine(line);


	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISMessagePublishPeriodSec, GetAISMessagePublishPeriodSec());
	p_save_loader->AddLine(line);



	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::AISMMSI, AISMMSI);
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

	data.Add(CCLICommandManager::AISClassType, GetAISClassType());
	data.Add(CCLICommandManager::AISShouldPublishAton, GetShoudPublishATON());
	data.Add(CCLICommandManager::AISMessagePublishPeriodSec, GetAISMessagePublishPeriodSec());


	data.Add(CCLICommandManager::AISMMSI, AISMMSI);
	data.Add(CCLICommandManager::AISName, AISName);
	data.Add(CCLICommandManager::AISVendorId, AISVendorId);
	data.Add(CCLICommandManager::AISCallSign, AISCallSign);
	data.Add(CCLICommandManager::AISReferencePointFromStarboard, AISReferencePointFromStarboard);
	data.Add(CCLICommandManager::ReferencePointPositionAftOfBow, ReferencePointPositionAftOfBow);
	data.Add(CCLICommandManager::AISDateOfArrival, AISDataOfArrivalYMD);
	data.Add(CCLICommandManager::AISTimeOfArrival, AISTimeOfArrival);


}
