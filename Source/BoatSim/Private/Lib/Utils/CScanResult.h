// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

#pragma once
#define HORIZOTAL_SCAN_SIZE 1920
#define VERTICAL_SCAN_SIZE 1080

#include <Lib/Types/Primitives.h>


struct SScanInfo
{
	
	
};

struct SSectorInfo
{
	FLOAT64 StartAzimuthDeg;
	FLOAT64 EndAzimuthDeg;
	FLOAT64 AzimuthStepDeg;
	INT32S ScanLineCount;
	TArray<FVector> SectorData;
	TArray<TArray<FVector>*> ScanLines;


public:

	void Init(INT32S scan_line_count, FLOAT64 start_azimuth_deg, FLOAT64 end_azimuth_deg)
	{
		ScanLineCount = scan_line_count;
		StartAzimuthDeg = start_azimuth_deg;
		EndAzimuthDeg = end_azimuth_deg;
		AzimuthStepDeg = (EndAzimuthDeg - StartAzimuthDeg)/(scan_line_count);
		for (INT32S i = 0; i < scan_line_count; i++) {
			ScanLines.Add(new TArray<FVector>());
		}
	}
	void Reset() {
		SectorData.Reset();

		for (INT32S i = 0; i < ScanLines.Num(); i++) {
			ScanLines[i]->Reset();
		}
	
	}

	void Add(FVector& vec)
	{
		SectorData.Add(vec);
	
	}
	void Add(FVector& vec, INT32S azimuth_scan_ind)
	{
		SectorData.Add(vec);
		if (azimuth_scan_ind < ScanLines.Num()) {
			ScanLines[azimuth_scan_ind]->Add(vec);
		}
	}


	bool MapSpoke4Bits(FVector own_ship_pos, FLOAT32 azimuth_deg, FLOAT32 cell_size_meter, INT8U *p_out)
	{
		if (azimuth_deg >= EndAzimuthDeg) {
			return false;
		}

		INT32S ind = (azimuth_deg - StartAzimuthDeg) / AzimuthStepDeg;

		if (ind >= ScanLines.Num()) {
			return false;
		}

		TArray<FVector>* p_data = ScanLines[ind];

	
		for (INT32S i = 0; i < p_data->Num(); i++) {
			FVector pos = (*p_data)[i];

			FLOAT32 dist = TOW( FVector::DistXY(pos, own_ship_pos));
			INT32S sample_ind = dist / cell_size_meter;

			INT32S byte_ind = sample_ind / 2;
			INT32S order = byte_ind & 0x1;

			if (order) {
				p_out[byte_ind] = 0xF;
			}
			else {
				p_out[byte_ind] = 0xF<<4;
			}
			

		}

		return true;
	}

};

struct SSectorContainer
{
	SSectorInfo* pSectors;
	int SectorCount;
	FLOAT64 AzimuthResolutionDeg;



public:
	void Init(int count, FLOAT64 azimuth_resolution_deg)
	{
		FLOAT64 each_sector_width_deg = 360.0 / count;

		pSectors = new SSectorInfo[count];
		SectorCount = count;
		AzimuthResolutionDeg = azimuth_resolution_deg;
		INT32S total_scan_line_per_sector = (INT32S) (each_sector_width_deg/ azimuth_resolution_deg + 0.5);
		
		for (int i = 0; i < count; i++) {
			FLOAT64 start_azimuth_deg = (each_sector_width_deg) * i;
			FLOAT64 end_azimuth_deg = start_azimuth_deg + each_sector_width_deg;
			pSectors[i].Init(total_scan_line_per_sector, start_azimuth_deg, end_azimuth_deg);
		}
	}

	SSectorInfo* GetSector(int no)
	{
		if (no < SectorCount) {
			return &pSectors[no];
		}
		else {
			return nullptr;
		}
		
	}

	void ResetSector(int no) {

		if (no < SectorCount) {
			pSectors[no].Reset();
		}
	}


	void AddToSector(int no, FVector& vec) {

		if (no < SectorCount) {
			pSectors[no].Add(vec);
		}
	}
};

struct SScanResult
{
	int HorizontalCount;
	int VeriticalCount;
	FVector ScanCenter;
	FVector ScanRPYWorld;
	FLOAT32 RangeMeter[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE]; //meter
	FLOAT32 NormalStrength[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE]; //meter
	FVector Point3D[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];

	int SectorCount;
	FLOAT32 ScanAzimuthStepDeg;
	FLOAT32 ScanElevationStepDeg;
	FVector2D AzimuthRange;
	FVector2D ElevationRange;
	


		
	FVector Track3DWorld[HORIZOTAL_SCAN_SIZE * VERTICAL_SCAN_SIZE];
	FLOAT32 TrackRangeMeter[HORIZOTAL_SCAN_SIZE * VERTICAL_SCAN_SIZE]; //meter

	INT32U Track3DCount;

	SSectorContainer SectorContainer;
	int CurrentSector = 0;
	
	FLOAT32 ScanRangeMeter;

public:

	SScanResult() {

	}

	void Init(int sector_cnt, FLOAT64 azimuth_resolution = 0.087890625)
	{
		SectorCount = sector_cnt;
		SectorContainer.Init(SectorCount, azimuth_resolution);
	}


	SSectorContainer* GetSectorContainer()
	{
		return &SectorContainer;
	}

	 void CopyFrom(SScanResult* p_src) {
		memcpy(&Point3D, &p_src->Point3D, sizeof(p_src->Point3D));
	}

	 void CopyTrackPoint3DOnly(SScanResult* p_src)
	 {
		 memcpy(Track3DWorld, p_src->Track3DWorld, sizeof(FVector) * Track3DCount);
		 Track3DCount = p_src->Track3DCount;
	 }

	 FLOAT32 Interpolate(FLOAT32 azimuth_deg, FLOAT32 elevation_deg,FLOAT32 &intensity)
	{
		INT32U azimuth_ind = (azimuth_deg - AzimuthRange.X) / ScanAzimuthStepDeg;
		INT32U elevation_ind = (elevation_deg - ElevationRange.X) / ScanElevationStepDeg;

		if (azimuth_ind < 0 || elevation_ind> 0) {
			intensity = 0;
			////todo fixme
			return 0;
		}
		intensity = NormalStrength[azimuth_ind][elevation_ind];
		return RangeMeter[azimuth_ind][elevation_ind];

	}
     void AddTrackPoint3DList(FVector pos, FLOAT32 range_meter)
	{
		 Track3DWorld[Track3DCount] = pos;
		 TrackRangeMeter[Track3DCount] = range_meter;
		 Track3DCount++;
	}
	 void ResetTrackPoint3DList()
	{
		Track3DCount = 0;
	}

};


class CScanResultContainer
{
	TArray<SScanResult*> ScanResults;
	INT32U CircularAccessInd = 0;


public:
	void Init(INT32S cnt, INT32U sector_cnt, FLOAT64 azimuth_res = 0.087890625)
	{

		for (INT32S i = 0; i < cnt; i++) {
			SScanResult* p_res = new SScanResult();
			p_res->Init(sector_cnt, azimuth_res);
			ScanResults.Add(p_res);
		}

		
	}

	SScanResult* GetCircular()
	{
		auto p_res = ScanResults[CircularAccessInd];

		CircularAccessInd++;
		CircularAccessInd %= ScanResults.Num();

		return p_res;
	}

};
class CScanResult
{
public:
	CScanResult();
	~CScanResult();
};
