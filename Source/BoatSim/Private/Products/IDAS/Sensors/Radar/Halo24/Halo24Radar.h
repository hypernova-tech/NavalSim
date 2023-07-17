// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "Halo24Radar.generated.h"

/**
 * 
 */
UCLASS()
class AHalo24Radar : public ARadarBase
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;
};
