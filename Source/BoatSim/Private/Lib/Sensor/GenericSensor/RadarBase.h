// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/SensorBase.h"
#include "Lib/Utils/CScanResult.h"
#include <Lib/Zone/ZoneContainer.h>
#include <Lib/Tracker/TrackerBase.h>
#include <Lib/Sensor/GenericRadarCommProtocolF/IRadarHostIF.h>
#include "RadarBase.generated.h"

/**
 * 
 */
UCLASS()
class ARadarBase : public ASensorBase, public IRadarHostIF
{
	GENERATED_BODY()
	

protected:
	CScanResultContainer ScanResultContainer;
	SScanResult *pScanResult;

	
	
	CZoneContainer GuardZone;
	CZoneContainer BlankingZone;



	CTrackerBase* pTracker = nullptr;

	virtual void Run(float delta_time_sec) override;
	virtual void InitSensor() override;
	virtual void BeginPlay() override;

	
	virtual void RadarStateMachine();
	virtual void OnDataReady() override;
	virtual void InitTracker();
	virtual void UpdateTracker();

	FGraphEventRef RaycastTaskComplete;

	float CurrentScanAzimuth;
	double NextScanTime;
	bool IsFullScaned = false;

	bool IsTrackerEnabled = false;


public:
	UPROPERTY(EditAnywhere)
		FString RadarSerial = "";

	char  Serial[128];

	UPROPERTY(EditAnywhere)
		bool UseSimulationDataAsOwnShip = true;

	UPROPERTY(EditAnywhere)
		FString RadarUnlockKey = "";

	UPROPERTY(EditAnywhere)
		int  ImageStreamCount = 2;


	UPROPERTY(EditAnywhere)
		bool IsAutoScanEnabled = false;

	UPROPERTY(EditAnywhere)
		double  RadarTransmissionDurationSec = 0.3;

	UPROPERTY(EditAnywhere, Category = "Scan")
		uint8 FastScanLevel;

	UPROPERTY(EditAnywhere, Category = "Scan")
		uint32 ScannerRPMValue;

	UPROPERTY(EditAnywhere, Category = "Gain")
		uint32 GainType = 2;

	UPROPERTY(EditAnywhere, Category = "Gain")
		uint32 GainLevel = 1;

	UPROPERTY(EditAnywhere, Category = "SeaClutter")
		uint8 SeaClutterType;

	UPROPERTY(EditAnywhere, Category = "SeaClutter")
		uint8 SeaClutterLevel;

	UPROPERTY(EditAnywhere, Category = "SeaClutter")
		uint8 SeaClutterAutoOffset;

	UPROPERTY(EditAnywhere, Category = "RainClutter")
		uint8 RainClutterLevel;

	UPROPERTY(EditAnywhere, Category = "Guardzone")
		uint8 MaxGuardZoneCount = 2;

	UPROPERTY(EditAnywhere, Category = "SectorBlanking")
		uint8 MaxSectorBlankingZoneCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TerrainBorderThreshold = 50;
	virtual void Scan();
	

	void SetTrackerEnabled(bool val);
	bool GetTrackerEnabled();

	void SetScanEnabled(bool val);
	bool GetScanEnabled();

	virtual void OnCaptureReady(void* p_data) override;

	void OnAsynTaskComplete();

	virtual void Save(ISaveLoader* p_save_load) override;
	virtual void SaveJSON(CJsonDataContainer& data) override;

private:


	virtual void OnRecievedMessage(void* p_commands);
	virtual char* GetSerial() ;
	virtual double GetGain() ;
	virtual double GetRainClutterLevel() ;
	virtual double GetSeaClutterLevel() ;
	virtual void* GetOwningActor() ;
	virtual double GetTerrainBorderThreshold();
	bool IsFullScan();

};
