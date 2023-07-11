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

	int BodId;
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
		return FVector(EulerPsiDeg,EulerPhiDeg, EulerThetaDeg);
	}
};
