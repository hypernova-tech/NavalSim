// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/LidarBase.h"
#include "LidarRobosenseM1.generated.h"

/**
 * 
 */
UCLASS()
class ALidarRobosenseM1 : public ALidarBase
{
	GENERATED_BODY()


protected:
	virtual void OnScanCompleted(SScanResult* p_scan_result) override;
	virtual void InitSensor() override;

public:
	virtual void Save(ISaveLoader* p_save_load) override;

	// Inherited via IHostIF

};
