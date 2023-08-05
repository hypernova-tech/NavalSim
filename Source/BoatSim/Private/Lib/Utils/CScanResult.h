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
	TArray<FVector> SectorData;

public:
	void Reset() {
		SectorData.Reset();
	}

	void Add(FVector& vec)
	{
		SectorData.Add(vec);
	}
};

struct SSectorContainer
{
	SSectorInfo* pSectors;
	int SectorCount;


public:
	void Init(int count)
	{
		pSectors = new SSectorInfo[count];
		SectorCount = count;
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
	FLOAT32 RangeMeter[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE]; //meter
	FLOAT32 NormalStrength[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE]; //meter
	FVector Point3D[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];

	int SectorCount;
	FLOAT32 ScanAzimuthStepDeg;
	FLOAT32 ScanElevationStepDeg;
	FVector2D AzimuthRange;
	FVector2D ElevationRange;
	


	//SScanElement ScanData[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];
	
	FVector Track3DWorld[HORIZOTAL_SCAN_SIZE * VERTICAL_SCAN_SIZE];
	FLOAT32 TrackRangeMeter[HORIZOTAL_SCAN_SIZE * VERTICAL_SCAN_SIZE]; //meter

	INT32U Track3DCount;
	//TArray< FVector> Point3D;
	SSectorContainer SectorContainer;

public:

	SScanResult() {

	}

	void Init(int sector_cnt)
	{
		SectorCount = sector_cnt;
		SectorContainer.Init(SectorCount);
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

class CScanResult
{
public:
	CScanResult();
	~CScanResult();
};
