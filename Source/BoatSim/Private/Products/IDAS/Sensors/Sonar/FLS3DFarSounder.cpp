// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Sonar/FLS3DFarSounder.h"
#include <Lib/SystemManager/SystemManagerBase.h>

void AFLS3DFarSounder::InitSensor()
{
	Super::InitSensor();
	pSharedMemory = GetComponentByClass<USharedMemory>();
	SSharedMemInitArgs args;
	args.Name = pSharedMemory->SharedMemoryName;
	//INT32U vert_cnt = (INT32U)(FovVerticalDeg / VerticalScanStepAngleDeg + 0.5f) + 1;
	//INT32U hor_cnt = (INT32U)(FovHorizontalDeg / HorizontalScanStepAngleDeg + 0.5f) + 1;

	args.size = sizeof(SFLSSharedMemBufferHdr) + 1920*1080 * sizeof(SFLSDataEntry);
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

	auto forward = GetActorForwardVector();
	forward.Normalize();

	auto loc = GetPositionXYZMeters();
	p_hdr->PosXYZ[0] = loc.X;
	p_hdr->PosXYZ[1] = -loc.Y;
	p_hdr->PosXYZ[2] = loc.Z;

	auto llh = GetPositionLatLongHeightMSL();

	p_hdr->PosLLH[0] = llh.X;
	p_hdr->PosLLH[1] = llh.Y;
	p_hdr->PosLLH[2] = llh.Z;

	p_hdr->LookDir[0] = forward.X;
	p_hdr->LookDir[1] = -forward.Y;
	p_hdr->LookDir[2] = forward.Z;

	int cnt = sector_info->SectorData.Num();

	for (int i = 0; i < cnt; i++) {
		FVector& data = (sector_info->SectorData[i]);
		EScanObjectType& object_type = sector_info->ObjectType[i];
		SFLSDataEntry  *p_curr_entry = &p_entries[i];
		p_curr_entry->X = TOW(data.X) ;
		p_curr_entry->Y = TOW(-data.Y);
		p_curr_entry->Z = TOW(data.Z);

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
