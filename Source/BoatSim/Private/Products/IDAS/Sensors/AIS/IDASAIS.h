// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/AISBase.h"
#include "IIDASAISHostIF.h"
#include "IDASAIS.generated.h"

/**
 * 
 */
UCLASS()
class AIDASAIS : public AAISBase, public IIDASAISHostIF
{
	GENERATED_BODY()
public:
	virtual void OnRecievedMessage(INT8U message_id, void* p_commands);
};
