// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"

/**
 * 
 */
enum EObjectTrackState
{
	Acquiring,
	Acquired,
	Lost,
};
struct STrackedObjectInfo
{
	EObjectTrackState TrackState;
	INT32U ClinetId;
	INT32U TrackerId;
	AActor* pActor;
	FLOAT64 AbsoluteDistanceMeter;
	FLOAT64 AbsoluteBearingDeg;
	FLOAT64 AbsoulteTargetSpeedMetersPerSec;
	FLOAT64 AbsoulteTargetCourseDeg;

	FLOAT64 RelativeDistanceMeter;
	FLOAT64 RelativeBearingDeg;
	FLOAT64 RelativeTargetSpeedMetersPerSec;
	FLOAT64 RelativeTargetCourseDeg;
	
};
class CTrackerBase
{

protected:

	TArray< STrackedObjectInfo*> TrackedObjects;
public:
	CTrackerBase();
	~CTrackerBase();


	virtual bool TryTrack(INT32U client_id, FVector pos, FLOAT64 bearing_true_north_deg, FLOAT64 distance_meter);
	virtual bool CancelAll();
	virtual bool CancelTrack(INT32U id);
	virtual void Update();
	virtual TArray< STrackedObjectInfo*> GetTrackedObjects();
};
