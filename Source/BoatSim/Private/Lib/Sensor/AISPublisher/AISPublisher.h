// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include "AISPublisher.generated.h"

/**
 * 
 */
UCLASS()
class AAISPublisher : public AActorBase
{
	GENERATED_BODY()
	
public:
	void SendAISData(void* data, INT32S data_len);
};
