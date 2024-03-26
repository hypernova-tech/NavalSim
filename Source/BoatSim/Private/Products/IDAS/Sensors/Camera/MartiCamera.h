// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/CameraBase.h"
#include <Lib/Connection/SharedMemory.h>
#include "MartiCamera.generated.h"

struct SMartiSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int Height;
	unsigned int Width;
	float ContrastLevel;
	float BrightnessLevel;
	struct {
		unsigned int IsIr : 1;
		unsigned int IsWhiteHot : 1;
		unsigned int EnableDefog : 1;
		unsigned int DefogLevel : 2;
		unsigned int IsICREnabled : 1;
		unsigned int Reserved : 26;
	}ImageInfo;

};

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
