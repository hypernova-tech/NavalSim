// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/SOA/CSOAObserverArgs.h"
#include "PlatformKinematicData.generated.h"

/**
 * 
 */
UCLASS()
class UPlatformKinematicData : public UCSOAObserverArgs
{
	GENERATED_BODY()
	

public:

	int    BotId;
	double BotPosLatDeg;
	double BotPosLonDeg;
	double VelNorthNedMeterSec;
	double VelEastNedMeterPerSec;
	double VelDownMeterPerSec;
	double BotPosNorthMeter;
	double BotPosEastMeter;
	double BotPosDownMeter;
	double EulerPhiDeg;
	double EulerThetaDeg;
	double EulerPsiDeg;
	double BodyVelSurgeMeterPerSec;
	double BodyVelSwayMeterPerSec;
	double BodyVelHeaveMeterPerSec;

	FVector GetLocationLLH()
	{
		return FVector(BotPosLatDeg, BotPosLonDeg, BotPosDownMeter);
	}
	FVector GetEulerRPYDeg()
	{
		return FVector(EulerPhiDeg, EulerThetaDeg,EulerPsiDeg);
	}

	void Copy(UPlatformKinematicData *p_dest) {
		p_dest->BotId = BotId;
		p_dest->BotPosLatDeg = BotPosLatDeg;
		p_dest->BotPosLonDeg = BotPosLonDeg;
		p_dest->VelNorthNedMeterSec = VelNorthNedMeterSec;
		p_dest->VelEastNedMeterPerSec = VelEastNedMeterPerSec;
		p_dest->VelDownMeterPerSec = VelDownMeterPerSec;
		p_dest->BotPosNorthMeter = BotPosNorthMeter;
		p_dest->BotPosEastMeter = BotPosEastMeter;
		p_dest->BotPosDownMeter = BotPosDownMeter;
		p_dest->EulerPhiDeg = EulerPhiDeg;
		p_dest->EulerThetaDeg = EulerThetaDeg;
		p_dest->EulerPsiDeg = EulerPsiDeg;
		p_dest->BodyVelSurgeMeterPerSec = BodyVelSurgeMeterPerSec;
		p_dest->BodyVelSwayMeterPerSec = BodyVelSwayMeterPerSec;
		p_dest->BodyVelHeaveMeterPerSec = BodyVelHeaveMeterPerSec;
	}
};
