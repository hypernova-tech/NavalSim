// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/EthernetDataProcessor/EthernetDataProcessorBase.h"
#include "IDASEthernetDataProcessor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UIDASEthernetDataProcessor : public UEthernetDataProcessorBase
{
	GENERATED_BODY()
	

public:
	virtual void OnReceivedData(const TArray<uint8>& data, int32 bytes_read) override;
};
