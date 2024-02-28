// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/AISBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Math/CMath.h>



void AAISBase::InitSensor()
{
	Super::InitSensor();
	
	TArray<AActor*> actors;

	ASystemManagerBase::GetInstance()->QueryActors(EActorQueryArgs::AISEnabledActors, actors);

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
	for (auto entry : AisEntries) {
	
		if (entry.LastTransmitTimeSec < 0) {
			entry.LastTransmitTimeSec = FApp::GetCurrentTime();
		}

		if (FApp::GetCurrentTime() >= (entry.LastTransmitTimeSec + entry.pActor->GetAISMessagePublishPeriodSec())) {
			if (entry.pActor->GetAISClassType() == 1) {
				PublishClassAPositionReport(entry.pActor);
				if (entry.pActor->GetShoudPublishATON()) {
					PublishATONReport(entry.pActor);
				}

			}else if (entry.pActor->GetAISClassType() == 2) {
				PublishClassBPositionReport(entry.pActor);
				if (entry.pActor->GetShoudPublishATON()) {
					PublishATONReport(entry.pActor);
				}
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

}
void AAISBase::PublishClassBPositionReport(AActorBase* p_act)
{

	SClassBPositionReport report;
	memset(&report, 0, sizeof(SClassBPositionReport));
	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();

	report.SetMessageID(1);
	report.SetLat(pos.X);
	report.SetLon(pos.Y);
	report.SetPositionAccuracy(false);
	report.SetSpeedOverGround(vel.Length());
	report.SetCourseOverGround(GetCourseOverGround(p_act));
	report.SetTrueHeading(rpy_ang.Z);

}
void AAISBase::PublishATONReport(AActorBase* p_act)
{

	SADISAtonReport report;
	memset(&report, 0, sizeof(SADISAtonReport));
	FVector pos = p_act->GetPositionLatLongHeightMSL();
	FVector rpy_ang = CMath::GetActorEulerAnglesRPY(p_act);
	FVector vel = p_act->GetActorVelocityMetersPerSec();
	FVector ang_vel = p_act->GetActorAngularVelocityRPYDegPerSec();

	report.SetMessageID(1);
	report.SetLat(pos.X);
	report.SetLon(pos.Y);
	report.SetPositionAccuracy(false);
	report.SetElectronicFixingPositionDeviceType(EAISPositionFixingDeviceType::CombinedGPSGLONASS);
}