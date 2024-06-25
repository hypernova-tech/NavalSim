// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "GenericRadar.generated.h"

/**
 * 
 */
UCLASS()
class AGenericRadar : public ARadarBase
{
	GENERATED_BODY()	

protected:

	virtual void BeginPlay() override;
	virtual void InitSensor() override;
	virtual void Scan();
	virtual void UpdateTracker() override;
};
