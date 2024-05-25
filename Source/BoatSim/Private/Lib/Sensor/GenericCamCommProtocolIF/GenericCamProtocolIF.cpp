// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericCamCommProtocolIF/GenericCamProtocolIF.h"
#include <Lib/SharedMemoryImpl/Windows/SharedMemoryWinImpl.h>

void UGenericCamProtocolIF::BeginPlay()
{
	Super::BeginPlay();

    
    {
        pSharedMemory = NewObject<USharedMemoryWinImpl>(this, USharedMemoryWinImpl::StaticClass(), TEXT("sharedmem"));
        if (pSharedMemory)
        {
            pSharedMemory->RegisterComponent();
 
        }
    }
}

void UGenericCamProtocolIF::Init(void* init_args, int args_size)
{
    pSharedMemory->InitConnection(init_args);
}

void UGenericCamProtocolIF::SendData(void* p_hdr, uint32 hdr_len, void* p_data, uint32 size_in_bytes)
{
    pSharedMemory->SendData((const INT8U * )p_hdr, hdr_len, (const INT8U*)p_data, size_in_bytes);
}
