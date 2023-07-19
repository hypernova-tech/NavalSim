// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

#pragma once
#define HORIZOTAL_SCAN_SIZE 1920
#define VERTICAL_SCAN_SIZE 1080

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
	float Range[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];
	FVector Point3D[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];

	int SectorCount;
	


	//SScanElement ScanData[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];
	

	TArray< FVector> Point3DList;
	TArray<FVector2D> Point2DScreen;
	SSectorContainer SectorContainer;

public:

	SScanResult() {

	}

	void Init(int sector_cnt)
	{
		SectorCount = sector_cnt;
		SectorContainer.Init(SectorCount);
	}
	void Reset2DPoint()
	{
		Point2DScreen.Reset();
	}

	SSectorContainer* GetSectorContainer()
	{
		return &SectorContainer;
	}

};

class CScanResult
{
public:
	CScanResult();
	~CScanResult();
};
