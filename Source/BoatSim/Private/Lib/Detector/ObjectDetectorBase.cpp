// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectDetectorBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Math/CMath.h>
#include <Lib/Time/CTime.h>
CObjectDetectorBase::CObjectDetectorBase()
{
}

void CObjectDetectorBase::StateMachine()
{
	auto curr_state = DetectorState;
	auto next_state = curr_state;

	switch (curr_state)
	{
	case DetectorStateIdle:
		if (StartEn) {
			next_state = DetectorStateDetect;
		}
		break;
	case DetectorStateDetect:
		Detect();
		break;
	default:
		break;
	}

	DetectorState = next_state;



}
void CObjectDetectorBase::SetOwnshipData(AActor* p_owner, CTrackerBase* p_tracker, FVector own_ship_location, FVector rpy_deg, FVector own_ship_vel, FVector2D radar_range_meter, FLOAT64 error_mean_meter, FLOAT64 error_mean_std)
{
	pOwnShip = p_owner;
	OwnShipLocation = own_ship_location;
	OwnShipRPY = rpy_deg;
	OwnShipVelocity = own_ship_vel;
	BearingToTargetDegTrueNorth = rpy_deg.Z;
	DetectorRange = radar_range_meter;
	RangeMeanErrorMeter = error_mean_meter;
	RangeErrorStdDevMeter = error_mean_std;
	StartEn = true;
}

void CObjectDetectorBase::SortActorsByDistance(TArray<AActor*>& Actors, const FVector& ReferencePosition)
{
	// Lambda function to compare distances
	auto DistanceComparer = [&ReferencePosition](const AActor& A, const AActor& B) -> bool
	{
		// Calculate the squared distance to avoid the cost of square root calculations
		float DistanceA = FVector::DistSquared(A.GetActorLocation(), ReferencePosition);
		float DistanceB = FVector::DistSquared(B.GetActorLocation(), ReferencePosition);

		return DistanceA < DistanceB;
	};

	// Sort the array using the lambda function
	Actors.Sort(DistanceComparer);
}

void CObjectDetectorBase::Detect()
{
	TArray<AActor*> actors = ASystemManagerBase::GetInstance()->GetAllActorInWorld();
	CObjectDetectorBase::SortActorsByDistance(actors,pOwnShip->GetActorLocation());


	for (auto p_actor : actors) {

		if (pTracker->GetTrackedObjectCount() >= MaxDetectableObject) {
			break;
		}

		if (pTracker->IsActorInAnyTrack(p_actor)) {
			continue;
		}


		TArray<AActor*> ignore_list;
		CUtil::GetOwnAndParents(pOwnShip, ignore_list);
		FVector dir = p_actor->GetActorLocation() - OwnShipLocation;
		dir.Normalize();

		auto actor = ASystemManagerBase::GetInstance()->GetVisibleActorAt(ignore_list, OwnShipLocation + dir * TOUE(DetectorRange.X), p_actor->GetActorLocation(), DetectorDistanceToleranceMeter);

		if (actor != nullptr && p_actor == actor) {
			if (!pTracker->IsActorInAnyTrack(actor)) {
				FVector rpy_deg = CMath::GetActorEulerAnglesRPY(actor);
				auto dist = (pOwnShip->GetActorLocation() - actor->GetActorLocation()).Length();
				pTracker->TryTrack(0, actor->GetActorLocation(), rpy_deg.Z, TOW(dist));
			}
		}


	}

	// check visible actors

}

void CObjectDetectorBase::Update()
{
	auto curr_time_sec = CTime::GetTimeSecond();

	if ((curr_time_sec - LastDetectTimeSec) > DetectionUpdateIntervalSec) {
		StateMachine();
		LastDetectTimeSec = curr_time_sec;
	}

}
