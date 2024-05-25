// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Connection/SharedMemory.h>
#include "GenericCamProtocolIF.generated.h"


struct SGenericCamSharedMemBufferHdr {
	unsigned int IsUpdated;
	unsigned int DataSize;
	unsigned int Height;
	unsigned int Width;


};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGenericCamProtocolIF : public UGenericCommIF
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	USharedMemory* pSharedMemory;

public:
	virtual void Init(void* init_args, int args_size) override;
	virtual void SendData(void* p_hdr, uint32 hdr_len, void* p_data, uint32 size_in_bytes) override;
};
