// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"

#include "SerialCommIF.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USerialCommIF : public UConnectionBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void *hSerialHandle;
	
public:
	virtual bool SendData(const INT8U* p_bytes, INT32U count) override;
};
