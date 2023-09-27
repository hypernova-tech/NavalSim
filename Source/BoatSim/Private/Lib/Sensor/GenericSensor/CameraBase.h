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


protected:

	virtual void InitSensor() override;


	USceneCaptureComponent2D* pSceneCapture;

	void PauseSensor() override ;
	void ResumeSensor() override;

public:
	void SetFovDeg(double fov_deg);


};
