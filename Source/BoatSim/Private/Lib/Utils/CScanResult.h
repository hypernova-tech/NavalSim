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
struct SScanResult
{
	int HorizontalCount;
	int VeriticalCount;
	FVector ScanCenter;
	float Range[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];
	FVector Point3D[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];

	

	//SScanElement ScanData[HORIZOTAL_SCAN_SIZE][VERTICAL_SCAN_SIZE];
	

	TArray< FVector> Point3DList;
	TArray<FVector2D> Point2DScreen;
};

class CScanResult
{
public:
	CScanResult();
	~CScanResult();
};
