// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/SensorBase.h"
#include <Components/SceneCaptureComponent2D.h>
#include "CameraBase.generated.h"


/**
 * 
 */
UCLASS()
class ACameraBase : public ASensorBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		int SensorWidth = 1920;
		
	UPROPERTY(EditAnywhere)
		int SensorHeight = 1080;

	UPROPERTY(EditAnywhere)
		float FieldOfViewDeg;

	UPROPERTY(EditAnywhere)
		bool IsWhiteHot = true;

	UPROPERTY(EditAnywhere)
		bool IsDefogEnabled = false;

	UPROPERTY(EditAnywhere)
		int DefogLevel = 0;

	UPROPERTY(EditAnywhere)
		bool IsICREnabled = false;

	UPROPERTY(EditAnywhere)
		int  BrightnessLevel = 50; // 0-100

	UPROPERTY(EditAnywhere)
		int  ContrastLevel = 50;// 0-100

	UPROPERTY(EditAnywhere)
		float  SceneCaptureTextureTargetGamma = 1.6; // very import when capture scene
	

	FLOAT64 LastCaptureTimeSec;

protected:
	virtual void BeginPlay() override;
	virtual void InitSensor() override;


	USceneCaptureComponent2D* pSceneCapture;

	void PauseSensor() override ;
	void ResumeSensor() override;
	virtual void Run(float delta_time_sec) override;
	void CaptureScreen();

	INT64U CaptureIndex = 0;
	virtual void OnCaptured();
	virtual void OnCaptureReady(void* p_data) override;
public:
	void SetFovDeg(double fov_deg);
	void UpdateFov();
	virtual void Save(ISaveLoader* p_save_loader) override;
	virtual void SaveJSON(CJsonDataContainer& data) override;

};
