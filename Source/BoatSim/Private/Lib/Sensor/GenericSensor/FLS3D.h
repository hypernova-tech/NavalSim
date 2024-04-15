// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/SensorBase.h"
#include "FLS3D.generated.h"

/**
 * 
 */
UCLASS()
class AFLS3D : public ASensorBase
{
	GENERATED_BODY()

public:
	AFLS3D();
protected:

	SScanResult* pScanResult;




	virtual void Run(float delta_time_sec) override;
	virtual void InitSensor() override;
	virtual void BeginPlay() override;
	virtual void OnDataReady() override;
public:


	
	virtual void OnCaptureReady(void* p_data) override;



private:
	float CurrentScanAzimuth;
	double NextScanTime;
	bool IsFullScaned = false;
};
