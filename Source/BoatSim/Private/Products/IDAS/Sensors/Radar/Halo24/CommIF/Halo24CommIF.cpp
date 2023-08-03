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
	CurrentRequest.Add(p_in);

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
	
	FString PipeName = TEXT("\\\\.\\pipe\\RadarSimPipe");
	pPipeInstance = new FPlatformNamedPipe();
	if (pPipeInstance->Create(PipeName, true, false))
	{
		
		bool ret = pPipeInstance->OpenConnection();
		if (ret) {
			UE_LOG(LogTemp, Warning, TEXT("Pipe server created and waiting for connections..."));
		}
	}

	SenderThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	
}

void UHalo24CommIF::SendRadarTrack()
{
	if (pPipeInstance->IsReadyForRW()) {
		SScanResult* p_current = CurrentRequest[0];
		CurrentRequest.RemoveAt(0);


		INT8U data[] = { 0,1,2,3,4,5,6 };

		bool ret = pPipeInstance->WriteBytes(sizeof(data), data);


	}

}

void UHalo24CommIF::Stop()
{
	IsStoped = true;
}
