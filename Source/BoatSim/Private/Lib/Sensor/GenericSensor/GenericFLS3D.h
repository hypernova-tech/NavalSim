// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/FLS3D.h"
#include "GenericFLS3D.generated.h"

/**
 * 
 */
UCLASS()
class AGenericFLS3D : public AFLS3D
{
	GENERATED_BODY()
		
protected:
	virtual void InitSensor() override;
	void OnDataReady();
	virtual void Run(float delta_time_sec) override;
	virtual void OnPreTrace(STraceArgs& args) override;
public:

	virtual void Save(ISaveLoader* p_save_loader) override;
	virtual void SaveJSON(CJsonDataContainer& data) override;
};
