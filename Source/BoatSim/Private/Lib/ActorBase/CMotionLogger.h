// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <list>
#include <Lib/Types/Primitives.h>

class CMotionLogEntry
{
public:
	FLOAT64 TimeTagSec;
	FVector Position;
};



/**
 * 
 */
class CMotionLogger
{

protected:
	std::list< CMotionLogEntry*> Entries;
	CMotionLogEntry* pLastEntry = nullptr;
	FLOAT64 MotionLogUpdateIntervalSec = 1;
	FLOAT64 LastMotionLogUpdateTimeSec;
	CMotionLogEntry* AddEntry(double time, const FVector& pos);
public:
	CMotionLogger();
	~CMotionLogger();

	
	void Log(double time, const FVector& pos, FLOAT32 min_distance);	
	void Update(double curr_time_sec, double life_time_duration);
	void GetPoints ( TArray<FVector>& vecs);
};





