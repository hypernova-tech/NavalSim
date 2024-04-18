// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Sonar/FLS3DFarSounder.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>
#include "Windows/MinWindows.h" // Include this header for Windows-specific functions

typedef int(*SonarCreateInstanceFuncPtr)(char*, int);

void AFLS3DFarSounder::InitSensor()
{
	Super::InitSensor();
	pSharedMemory = GetComponentByClass<USharedMemory>();
	SSharedMemInitArgs args;
	pSharedMemory->SharedMemoryName = ProtocolConverterSharedMemoryName;
	args.Name = pSharedMemory->SharedMemoryName;
	//INT32U vert_cnt = (INT32U)(FovVerticalDeg / VerticalScanStepAngleDeg + 0.5f) + 1;
	//INT32U hor_cnt = (INT32U)(FovHorizontalDeg / HorizontalScanStepAngleDeg + 0.5f) + 1;

	args.size = sizeof(SFLSSharedMemBufferHdr) + 1920*1080 * sizeof(SFLSDataEntry);
	args.HeaderSize = sizeof(SFLSSharedMemBufferHdr);
	pSharedMemory->InitConnection(&args);


	FString BinariesDir = FPaths::ProjectDir() + TEXT("Binaries/Win64/Sonar/");

	FString DLLName = TEXT("SonarSimProj.dll");

	// Construct the full path to the DLL
	FString DLLPath = BinariesDir + DLLName;


	HINSTANCE inst;
	// Load the DLL
	if (ProtocolConverterSharedMemoryName != "") {
		inst = LoadLibrary(*DLLPath);
		if (inst != nullptr)
		{
			// Get function pointer
			SonarCreateInstanceFuncPtr func_ptr = (SonarCreateInstanceFuncPtr)GetProcAddress(inst, "CreateInstance");
			if (func_ptr != nullptr)
			{
				char sm_name[1024];
				CUtil::FStringToAsciiChar(ProtocolConverterSharedMemoryName, sm_name, 1024);

				// Call function
				/////SonarDllInstanceInd = func_ptr(sm_name, args.HeaderSize + args.size);
			}
			hDLL = inst;
			// Unload DLL (optional)
			//FreeLibrary(hDLL);
		}
	}

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
	int ind = 0;
	bool is_ground;
	if (IsFlsOn) {
		for (int i = 0; i < cnt; i++) {

			FVector& data = (sector_info->SectorData[i]);
			EScanObjectType& object_type = sector_info->ObjectType[i];
			SFLSDataEntry* p_curr_entry = &p_entries[ind];
			is_ground = object_type == EScanObjectType::ScanObjectTypeTerrain;

			if (is_ground) {
				if (p_hdr->FromSimToHost.BottomDetectionEnabled) {
					p_curr_entry->X = TOW(data.X);
					p_curr_entry->Y = TOW(-data.Y);
					p_curr_entry->Z = TOW(data.Z);
					p_curr_entry->Info.IsGround = is_ground;
					ind++;
				}
			}
			else {
				p_curr_entry->X = TOW(data.X);
				p_curr_entry->Y = TOW(-data.Y);
				p_curr_entry->Z = TOW(data.Z);
				p_curr_entry->Info.IsGround = is_ground;
				ind++;
			}
		}
	}
	
	p_hdr->DataSize = sizeof(SFLSDataEntry) * ind;

	p_hdr->FromSimToHost.RangeMeter = RangeMaxMeter;
	p_hdr->FromSimToHost.BottomDetectionEnabled = BottomDetectEnabled;
	p_hdr->FromSimToHost.IsAutoSquelchEnabled = AutoSquelchEnabled;
	p_hdr->FromSimToHost.SquelchSensitivity = SquelchSensitivity;
	p_hdr->FromSimToHost.IsFlsOn = IsFlsOn;

	p_hdr->IsUpdated = true;
}

void AFLS3DFarSounder::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);

	SFLSSharedMemBufferHdr* p_hdr = (SFLSSharedMemBufferHdr*)pSharedMemory->GetHeader();

	if (p_hdr->SonarSimIsUpdateData) { // reint data structures if an
		RangeMaxMeter = p_hdr->FromHostToSim.RangeMeter;
		BottomDetectEnabled = p_hdr->FromHostToSim.BottomDetectionEnabled > 0;
		AutoSquelchEnabled = p_hdr->FromHostToSim.IsAutoSquelchEnabled > 0 ;
		SquelchSensitivity = p_hdr->FromHostToSim.SquelchSensitivity;

	}

	//update
	p_hdr->FromHostToSim.RangeMeter = RangeMaxMeter;
	p_hdr->FromHostToSim.BottomDetectionEnabled = BottomDetectEnabled;
	p_hdr->FromHostToSim.IsAutoSquelchEnabled   = AutoSquelchEnabled;
	p_hdr->FromHostToSim.SquelchSensitivity     = SquelchSensitivity;  
	   
	   
}

void AFLS3DFarSounder::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);
}

void AFLS3DFarSounder::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);
}
