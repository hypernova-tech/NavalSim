// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/FLS3D.h"
#include <Lib/Connection/SharedMemory.h>
#include "FLS3DFarSounder.generated.h"


struct SFLSSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int SonarSimIsUpdateData;
	float SonarSimVerticalFovDeg; // from SonarSim to UnrealEngine
	float SonarSimHorizontalFovDeg; // from SonarSim to UnrealEngine
	float SonarSimIsSquelchEnabled; // from SonarSim to UnrealEngine
};

struct SFLSDataEntry
{
public:
	struct {
		INT32U IsGround : 1;
		INT32U Reserved : 31;
	}Info;
	FLOAT32 X;
	FLOAT32 Y;
	FLOAT32 Z;
};

/**
 * 
 */
UCLASS()
class AFLS3DFarSounder : public AFLS3D
{
	GENERATED_BODY()
	
protected:
	USharedMemory* pSharedMemory;
	virtual void InitSensor() override;
	void OnDataReady();
	virtual void Run(float delta_time_sec) override;
};
