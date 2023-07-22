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
		int SensorWidth;
		
	UPROPERTY(EditAnywhere)
		int SensorHeight;

	UPROPERTY(EditAnywhere)
		float FieldOfViewDeg;


protected:

	virtual void InitSensor() override;


	USceneCaptureComponent2D* pSceneCapture;


};
