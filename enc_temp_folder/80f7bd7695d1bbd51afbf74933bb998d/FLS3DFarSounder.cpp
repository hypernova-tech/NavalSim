// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Sonar/FLS3DFarSounder.h"

void AFLS3DFarSounder::InitSensor()
{
	Super::InitSensor();
	pSharedMemory = GetComponentByClass<USharedMemory>();
	SSharedMemInitArgs args;
	args.Name = pSharedMemory->SharedMemoryName;
	INT32U vert_cnt = (INT32U)(FovVerticalDeg / VerticalScanStepAngleDeg + 0.5f) + 1;
	INT32U hor_cnt = (INT32U)(FovHorizontalDeg / HorizontalScanStepAngleDeg + 0.5f) + 1;

	args.size = vert_cnt * hor_cnt * sizeof(SFLSDataEntry);
	args.HeaderSize = sizeof(SFLSSharedMemBufferHdr);
	pSharedMemory->InitConnection(&args);
}

void AFLS3DFarSounder::OnDataReady()
{
	Super::OnDataReady();
	// flush scan result
	auto sector_info = pScanResult->SectorContainer.GetSector(pScanResult->CurrentSector);
	INT8U* p_mem = (INT8U*)pSharedMemory->GetHeader();
	SFLSSharedMemBufferHdr* p_hdr = (SFLSSharedMemBufferHdr*)p_mem;
	SFLSDataEntry* p_entries = (SFLSDataEntry*)(p_mem + sizeof(SFLSSharedMemBufferHdr));

	int cnt = sector_info->SectorData.Num();

	for (int i = 0; i < cnt; i++) {
		FVector& data = sector_info->SectorData[i];
		EScanObjectType& object_type = sector_info->ObjectType[i];
		SFLSDataEntry  *p_curr_entry = &p_entries[i];
		p_curr_entry->X = data.X;
		p_curr_entry->Y = data.Y;
		p_curr_entry->Z = data.Z;
		p_curr_entry->Info.IsGround = object_type == EScanObjectType::ScanObjectTypeTerrain;
	}
	p_hdr->DataSize = sizeof(SFLSDataEntry) * cnt;
	p_hdr->IsUpdated = true;
}

void AFLS3DFarSounder::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);

	SFLSSharedMemBufferHdr* p_hdr = (SFLSSharedMemBufferHdr*)pSharedMemory->GetHeader();

	if (p_hdr->SonarSimIsUpdateData) { // reint data structures if an
		if (p_hdr->SonarSimVerticalFovDeg > 0 && p_hdr->SonarSimHorizontalFovDeg > 0){
			FovVerticalDeg = p_hdr->SonarSimVerticalFovDeg;
			FovHorizontalDeg = p_hdr->SonarSimHorizontalFovDeg;
			p_hdr->SonarSimIsUpdateData = 0;
		}
	}
}
