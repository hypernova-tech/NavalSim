// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/SensorBase.h"
#include "LidarBase.generated.h"

/**
 * 
 */
UCLASS()
class ALidarBase : public ASensorBase
{
	GENERATED_BODY()

public:
	ALidarBase();

protected:

	SScanResult* pScanResult;

	virtual void Run(float delta_time_sec) override;
	virtual void InitSensor() override;

	virtual void BeginPlay() override;


	virtual void OnScanCompleted(SScanResult* p_scan_result);

public:




	virtual void OnCaptureReady(void* p_data) override;

private:
	float CurrentScanAzimuth;
	double NextScanTime;
	bool IsFullScaned = false;
};
