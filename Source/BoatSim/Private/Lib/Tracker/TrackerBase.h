// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"

/**
 * 
 *     eAcquiringTarget = 0,      ///< Attempting to acquire target
    eSafeTarget      = 1,      ///< Target acquired and not on a collision course
    eDangerousTarget = 2,      ///< Target acquired and may be on a collision course
    eLostTarget      = 3,      ///< Target has been lost and needs to be cancelled an reacquired
    eAcquireFailure  = 4,      ///< Failed to acquire a target
    eOutOfRange      = 5,      ///< Target is now out of range
    eLostOutOfRange  = 6,      ///< Target lost due to staying out of range

    eFailAcquireMax  = 0x10,   ///< Acquire failed because no target IDs were free
    eFailAcquirePos  = 0x11,
 */
enum EObjectTrackState
{
	JustCreated,
	Acquiring,
	AcquiredAndSafe,
	AcquiredAndDangerous,
	LostTarget,
	AcquireFailure,
	OutOfRange,
	LostOutOfRange,
	AquireFailedTargetTrackCapacityFull,
};
struct STrackedObjectInfo
{
public:

	EObjectTrackState TrackState;
	INT32U ClientId;
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


	FLOAT64 AcquireStartTimeRefSec;
	FVector TrackLocationWhenCreated;
	
	STrackedObjectInfo()
	{
		TrackState = EObjectTrackState::JustCreated;
	}
	bool IsAcquireTimeout(FLOAT64 timeout_sec)
	{
		return (FApp::GetCurrentTime() - AcquireStartTimeRefSec) > timeout_sec;
	}

	
};


class CTrackerBase
{

protected:

	TArray< STrackedObjectInfo*> TrackedObjects;
	TMap<int, STrackedObjectInfo> TrackClientIdMap;

	STrackedObjectInfo ClosestTrack;
	FLOAT64 CPAMeters;
	FLOAT64 CPATimeSec;
	FLOAT64 IsTowardsCPA;
	FVector OwnShipLocation;
	FVector OwnShipRPY;
	AActor* pOwnShip;




protected:
	STrackedObjectInfo* FindTrackByClientId(INT32S client_track_id);
	void AddTrack(STrackedObjectInfo* p_track);
	void UpdateTrackState(STrackedObjectInfo *p_track);
	bool CTrackerBase::TryAcquire(STrackedObjectInfo* p_track, bool &is_safe_target);

public:
	CTrackerBase();
	~CTrackerBase();


	virtual bool TryTrack(INT32U client_id, FVector pos, FLOAT64 bearing_true_north_deg, FLOAT64 distance_meter);
	virtual bool CancelAll();
	virtual bool CancelTrack(INT32U id);
	virtual void Update();
	virtual TArray< STrackedObjectInfo*>* GetTrackedObjects();
	virtual FLOAT64 GetCPAMeters();
	virtual FLOAT64 GetCPATimeSec();
	virtual BOOLEAN GetIsTowardsCPA();

};
