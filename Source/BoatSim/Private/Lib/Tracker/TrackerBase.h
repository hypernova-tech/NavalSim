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
	JustCreated = 0,
	Acquiring,
	AcquiredAndSafe,
	AcquiredAndDangerous,
	AcquiredSafeAndTemprorayLoss,
	AcquiredDangerousAndTemprorayLoss,
	LostTarget,
	AcquireFailure,
	OutOfRange,
	LostOutOfRange,
	AquireFailedTargetTrackCapacityFull,
	StateCount
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
	FLOAT64 TargetLossTimeRefSec;
	FVector TrackLocationWhenCreated;
	bool IsOutOfRange;

	FLOAT64 StateEnterTimeSec[EObjectTrackState::StateCount];
	FLOAT64 StateStayDurationSec[EObjectTrackState::StateCount];

	
	STrackedObjectInfo()
	{
		memset(this, 0, sizeof(STrackedObjectInfo));
		TrackState = EObjectTrackState::JustCreated;
		
	}
	bool IsAcquireTimeout(FLOAT64 timeout_sec);
	void HandleTemporayTargetLoss(FLOAT64 timeout_sec, bool& is_permanant_loss);

	void SetState(EObjectTrackState state);

	FLOAT64 GetStateStayDurationSec(EObjectTrackState state);
	FLOAT64 GetCurrentStateStayDuration();

	bool IsAcquired();
	bool IsSafeToDelete();
};




class CTrackerBase
{

protected:

	TArray< STrackedObjectInfo*> TrackedObjects;
	TMap<int, STrackedObjectInfo*> TrackClientIdMap;

	STrackedObjectInfo ClosestTrack;
	FLOAT64 CPAMeters;
	FLOAT64 CPATimeSec;
	bool IsTowardsCPA;
	FVector OwnShipLocation;
	FVector OwnShipVelocity;
	FVector OwnShipRPY;
	AActor* pOwnShip;
	FLOAT64 BearingToTargetDegTrueNorth;
	FVector2D RadarRangeMeter;
	FLOAT64 RadarRangeMeanErrorMeter;
	FLOAT64 RadarRangeErrorStdDevMeter;
	FLOAT64 TryTrackDistanceThresholdMeter = 3.0;
	FLOAT64 TrackerDistanceToleranceMeter = 25.0;
	INT32S MaxTrackCount = 10;
	FLOAT64 TargetAquireTimeOutSec = 5.0;
	FLOAT64 OutOfRangeToLostTimeoutSec = 1.0f;
	FLOAT64 TemprorayAcquireToLostTimeoutSec = 1.0f;
	TArray<INT32S> CancalTrackRequest;
	bool IsCancelAllTrack = false;
	INT32S TrackerId = 1;
protected:
	STrackedObjectInfo* FindTrackByClientId(INT32S client_track_id);
	
	void AddTrack(STrackedObjectInfo* p_track);
	void RemoveTrack(STrackedObjectInfo* p_track);
	void UpdateTrackState(STrackedObjectInfo *p_track);
	bool TryAcquire(STrackedObjectInfo* p_track, bool &is_safe_target);
	bool CheckStillAquired(STrackedObjectInfo* p_track, bool& is_safe_target);
	bool IsTargetOutofRange(STrackedObjectInfo* p_track);
	void ProcessCancelTrackRequests();
	void FillTrackInfo(STrackedObjectInfo* p_track);
	FLOAT64 ComputeTrackBearingDeg(STrackedObjectInfo *p_track);
	FVector GetTrackVelocity(STrackedObjectInfo* p_track);
public:
	CTrackerBase();
	~CTrackerBase();
	bool IsActorInAnyTrack(AActor* p_actor);
	virtual void SetOwnshipData(AActor *p_owner, FVector own_ship_location, FVector rpy_deg, FVector own_ship_vel, FVector2D radar_range_meter, FLOAT64 error_mean_meter, FLOAT64 error_mean_std);
	virtual bool TryTrack(INT32U client_id, FVector pos, FLOAT64 bearing_true_north_deg, FLOAT64 distance_meter);
	virtual bool CancelAll();
	virtual bool CancelTrack(INT32U id);
	virtual void Update();
	virtual void UpdateCPA();
	virtual TArray< STrackedObjectInfo*>* GetTrackedObjects();
	virtual FLOAT64 GetCPAMeters();
	virtual FLOAT64 GetCPATimeSec();
	virtual bool GetIsTowardsCPA();
	INT32S GetTrackedObjectCount();

};
