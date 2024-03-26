// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Connection/SharedMemory.h"
#include "SharedMemory.h"


void USharedMemory::InitConnection(void* p_args)
{
    Super::InitConnection(p_args);

}

bool USharedMemory::SendData(const INT8U* p_bytes, INT32U count)
{
    return false;
}

bool USharedMemory::SendData(const INT8U* p_hdr, INT32U header_size, const INT8U* p_bytes, INT32U count)
{
    return false;
}

void* USharedMemory::GetHeader()
{
    return nullptr;
}






