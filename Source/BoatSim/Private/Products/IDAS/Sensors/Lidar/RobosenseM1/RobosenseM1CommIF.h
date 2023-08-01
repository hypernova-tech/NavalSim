// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Utils/CScanResult.h>
#include "RobosenseM1CommIF.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URobosenseM1CommIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SendMainStreamOutputPacket();

public:

	virtual void SendData(void* p_data, uint32 size_in_bytes) override;
	

	// Inherited via FRunnable
	virtual uint32 Run() override;


	SScanResult* pLastScanResult;

	const FLOAT32 SensorAzimuthStepAngle = 0.01f;
	const FLOAT32 SensorElevationStepAngle = 0.01f;


	const FLOAT32 RadiusScaleFactorMete = 5e-3f;
	const FLOAT32 ElevationScaleFactor = 0.01f;
	const FLOAT32 AzimuthScaleFactor = 0.01f;
	const INT32U  IntensitiyScaleFactor = 255;
private:

	bool HasNewData;
	TArray< SScanResult*> CurrentRequest;


};
