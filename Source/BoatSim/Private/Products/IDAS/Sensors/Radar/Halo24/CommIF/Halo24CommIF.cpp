// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#include "HAL/PlatformNamedPipe.h"


UHalo24CommIF::~UHalo24CommIF()
{
	if (SenderThread != nullptr) {
		SenderThread->Kill();
		delete SenderThread;
	}


	
}

void UHalo24CommIF::SendData(void* p_data, uint32 size_in_bytes)
{
	HasNewData = true;
	SScanResult* p_in = (SScanResult*)p_data;
	SScanResult* p_new = new SScanResult();
	p_new->CopyTrackPoint3DOnly(p_in);
	CurrentRequest.Add(p_new);

}

uint32 UHalo24CommIF::Run()
{
	while (!IsStoped) {
		FPlatformProcess::Sleep(0.1);

		if (CurrentRequest.Num() > 0) {
			SendRadarTrack();
		}
	}
	return uint32();
}

void UHalo24CommIF::BeginPlay()
{
	Super::BeginPlay();
	
#if false
	FString PipeName = TEXT("\\\\.\\pipe\\RadarSimPipe");
	pPipeInstance = new FPlatformNamedPipe();
	if (pPipeInstance->Create(PipeName, true, false))
	{
		
		bool ret = pPipeInstance->OpenConnection();
		if (ret) {
			UE_LOG(LogTemp, Warning, TEXT("Pipe server created and waiting for connections..."));
		}
	}
#endif

	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	
}

void UHalo24CommIF::SendRadarTrack()
{
	SScanResult* p_current = CurrentRequest[0];
	CurrentRequest.RemoveAt(0);
	delete p_current;
	return;

	if (pPipeInstance->IsReadyForRW()) {

		

		INT8U data[] = { 0,1,2,3,4,5,6 };
		INT32U fvector_size = sizeof(FVector);
		INT32U total_size = fvector_size * p_current->Track3DCount;
		INT32U struct_size = sizeof(p_current->Track3DWorld);
		FLOAT64 start_time_ref = CUtil::GetTimeSeconds();
		bool ret = pPipeInstance->WriteBytes(total_size, (INT8U*)p_current->Track3DWorld);
		FLOAT64 diff = CUtil::GetTimeSeconds() - start_time_ref;
		CUtil::DebugLog(FText::AsNumber(total_size).ToString());
		delete p_current;
	}

}

void UHalo24CommIF::Stop()
{
	IsStoped = true;
}
