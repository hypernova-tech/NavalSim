// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/PointCloud/PointCloudCommProtocolIF.h"

UPointCloudCommProtocolIF::~UPointCloudCommProtocolIF()
{
	if (pThread != nullptr) {
		pThread->Kill();
		delete pThread;
	}
}
void UPointCloudCommProtocolIF::BeginPlay()
{
	Super::BeginPlay();

	pThread = FRunnableThread::Create(this, *(GetOwner()->GetName()));
	RegisterConnectionInfo(0, pUDPConnection->GetConnectionInfo(), pUDPConnection);
}

void UPointCloudCommProtocolIF::SendData(void* p_data, uint32 size_in_bytes)
{
	Mutex.Lock();

	SScanResult* p_in = (SScanResult*)p_data;
	SScanResult* p_new = p_in;
	CurrentRequest.Add(p_new);

	Mutex.Unlock();
}

uint32 UPointCloudCommProtocolIF::Run()
{
	while (!IsStoped) {
		FPlatformProcess::Sleep(0.02);
		Mutex.Lock();
		bool has_data = CurrentRequest.Num() > 0;
		Mutex.Unlock();
		if (has_data) {
			SendPointCloud();
		}

	}
	return uint32();
}

void UPointCloudCommProtocolIF::Stop()
{
	IsStoped = true;
}


#if true
void UPointCloudCommProtocolIF::SendPointCloud()
{
	Mutex.Lock();

	SScanResult* p_current = CurrentRequest[0];
	bool is_full_scanned = p_current->CurrentSector == (p_current->SectorCount - 1);


	FLOAT32 each_spoke_step = 360.0f / 4096;
	FLOAT32 each_cell_size = p_current->ScanRangeMeter / 1024.0f;
	CurrentRequest.RemoveAt(0);


	auto* p_current_sector = p_current->GetSectorContainer()->GetSector(p_current->CurrentSector);


	

	Mutex.Unlock();



	

}
#endif