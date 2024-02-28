// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/AIS/IDASAISSCommIF.h"

void UIDASAISSCommIF::BeginPlay()
{
}

void UIDASAISSCommIF::OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count)
{
}
uint32 UIDASAISSCommIF::Run()
{
	while (!IsStoped) {
		FPlatformProcess::Sleep(0.01);
	}
	return uint32();
}

void UIDASAISSCommIF::Stop()
{
}

