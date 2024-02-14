// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/CameraBase.h"
#include <Lib/Connection/SharedMemory.h>
#include "MartiCamera.generated.h"


/**
 * 
 */
UCLASS()
class AMartiCamera : public ACameraBase
{
	GENERATED_BODY()


protected:
	USharedMemory* pSharedMemory;
	virtual void OnCaptured() override;
	virtual void OnCaptureReady(void* p_data) override;
public:
	virtual void InitSensor() override;
};
