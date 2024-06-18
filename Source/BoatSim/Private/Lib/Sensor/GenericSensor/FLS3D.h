// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/SensorBase.h"
#include <Lib/Sensor/GenericSonarCommIF/ISonarHostIF.h>
#include "FLS3D.generated.h"

/**
 * 
 */
UCLASS()
class AFLS3D : public ASensorBase, public ISonarHostIF
{
	GENERATED_BODY()

public:
	AFLS3D();
protected:
	bool IsFlsOn = false;
	bool   BottomDetectEnabled = false;
	double SquelchSensitivity = 100;
	bool   AutoSquelchEnabled = true;

	SScanResult* pScanResult;
	TArray<AActor*> CachedPlatforms;



	virtual void Run(float delta_time_sec) override;
	virtual void InitSensor() override;
	virtual void BeginPlay() override;
	virtual void OnDataReady() override;
public:


	
	virtual void OnCaptureReady(void* p_data) override;
	virtual void OnRecievedMessage(void* p_commands);
	virtual void* GetOwningActor();
	virtual bool   GetBottomDetectEnabled();
	virtual double GetSquelchSensitivity();
	virtual bool   GetAutoSquelchEnabled();


private:
	float CurrentScanAzimuth;
	double NextScanTime;
	bool IsFullScaned = false;
};
