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

void USharedMemory::SetDataDimension(unsigned int width, unsigned int Height, bool is_ir, bool is_white_hot, bool defog_enabled, int defog_level, bool is_icr_enabled)
{
    DataWidth           = width;
    DataHeight          = Height;
    IsIR                = is_ir;
    IsWhiteHot          = is_white_hot;
    IsDefogEnabled      = defog_enabled;
    DefogLevel          = defog_level;
    IsICREnabled = is_icr_enabled;
}




