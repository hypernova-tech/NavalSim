#pragma once
#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>
#include <Lib/ActorBase/ActorBase.h>
#include <Lib/Tracker/TrackerBase.h>

enum EDetectorState
{
	DetectorStateIdle,
	DetectorStateDetect
};

class CObjectDetectorBase
{
	INT32S MaxDetectableObject = 50;
	FVector OwnShipLocation;
	FVector OwnShipVelocity;
	FVector OwnShipRPY;
	AActor* pOwnShip;
	FLOAT64 BearingToTargetDegTrueNorth;
	FVector2D DetectorRange;
	FLOAT64 RangeMeanErrorMeter;
	FLOAT64 RangeErrorStdDevMeter;
	FLOAT64 DetectorDistanceToleranceMeter = 50;
	FLOAT64 DetectionUpdateIntervalSec = 1.0;


protected:
	bool StartEn = false;

	CTrackerBase* pTracker;
	EDetectorState DetectorState = EDetectorState::DetectorStateIdle;
	FLOAT64 LastDetectTimeSec;
	void StateMachine();
	void Detect();
	static void SortActorsByDistance(TArray<AActor*>& Actors, const FVector& ReferencePosition);
public:
	CObjectDetectorBase();
	void SetOwnshipData(AActor* p_owner, CTrackerBase* p_tracker, FVector own_ship_location, FVector rpy_deg, FVector own_ship_vel, FVector2D radar_range_meter, FLOAT64 error_mean_meter, FLOAT64 error_mean_std);
	void Update();
};

