// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/FLS3D.h"
#include <Lib/Connection/SharedMemory.h>
#include "FLS3DFarSounder.generated.h"


struct SFLSProcessorSettings
{
	unsigned int IsFlsOn;
	double RangeMeter;
	unsigned int BottomDetectionEnabled;
	unsigned int IsAutoSquelchEnabled;
	double SquelchSensitivity;
};

struct SFLSSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int SonarSimIsUpdateData;
	double PosXYZ[3];
	double PosLLH[3];
	double LookDir[3];
	SFLSProcessorSettings FromHostToSim;
	SFLSProcessorSettings FromSimToHost;

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
	void* hDLL;
	INT32S SonarDllInstanceInd;
	bool IsFlsOn = false;
	bool   BottomDetectEnabled = false;
	double SquelchSensitivity = 100;
	bool   AutoSquelchEnabled = true;

	virtual void OnPreTrace(STraceArgs& args) override;
public:

	virtual void Save(ISaveLoader* p_save_loader) override;
	virtual void SaveJSON(CJsonDataContainer& data) override;
};
