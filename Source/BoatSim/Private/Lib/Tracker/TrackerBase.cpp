// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Tracker/TrackerBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>


CTrackerBase::CTrackerBase()
{
}

CTrackerBase::~CTrackerBase()
{
}

bool CTrackerBase::TryTrack(INT32U client_id, FVector pos, FLOAT64 bearing_true_north_deg, FLOAT64 distance_meter)
{
    if (FindTrackByClientId(client_id) != nullptr) { // first cancel it
        return false;
    }
   
    STrackedObjectInfo* p_track = new STrackedObjectInfo();
    FVector dir = FRotator(0, -bearing_true_north_deg, 0).RotateVector(FVector::ForwardVector);
    FVector target_pos = OwnShipLocation + dir * TOUE(distance_meter);
    p_track->TrackLocationWhenCreated = target_pos;

    AddTrack(p_track);



   
    return false;
}

bool CTrackerBase::CancelAll()
{
    return false;
}

bool CTrackerBase::CancelTrack(INT32U id)
{
    return false;
}

bool CTrackerBase::TryAcquire(STrackedObjectInfo* p_track, bool& is_safe_target)
{
    FVector target_pos = p_track->TrackLocationWhenCreated;
    auto actor = ASystemManagerBase::GetInstance()->GetVisibleActorAt(pOwnShip, OwnShipLocation, target_pos, 5);
    if (actor != nullptr) {
        is_safe_target = true;
        p_track->pActor = actor;
        return true;
    }
    return false;
}

void CTrackerBase::UpdateTrackState(STrackedObjectInfo* p_track)
{
    auto curr_state = p_track->TrackState;
    auto next_state = curr_state;
    bool is_safe_target = true;
    /*,
    * 	JustCreated,
	Acquiring,
	AcquiredAndSafe,
	AcquiredAndDangerous,
	LostTarget,
	AcquireFailure,
	OutOfRange,
	LostOutOfRange,
	AquireFailedTargetTrackCapacityFull,
    */

    switch (curr_state) {
    case EObjectTrackState::JustCreated:
        next_state = EObjectTrackState::Acquiring;
        p_track->AcquireStartTimeRefSec = FApp::GetCurrentTime();
        break;
    case EObjectTrackState::Acquiring:
        if (TryAcquire(p_track, is_safe_target)) {
            if (is_safe_target) {
                next_state = EObjectTrackState::AcquiredAndSafe;
            }
            else {
                next_state = EObjectTrackState::AcquiredAndDangerous;
            }
            
        }
        else {
            if (p_track->IsAcquireTimeout(5)) {
                next_state = EObjectTrackState::AcquireFailure;
            }
        }
        break;
    case EObjectTrackState::AcquiredAndSafe:
        if (TryAcquire(p_track, is_safe_target)) {

        }
        break;
    case EObjectTrackState::AcquiredAndDangerous:
        break;
    case EObjectTrackState::LostTarget:
        break;
    case EObjectTrackState::AcquireFailure:
        break;
    case EObjectTrackState::OutOfRange:
        break;
    case EObjectTrackState::LostOutOfRange:
        break;
    case EObjectTrackState::AquireFailedTargetTrackCapacityFull:
        break;
    }

    p_track->TrackState = next_state;
}

void CTrackerBase::Update()
{

}

TArray<STrackedObjectInfo*>* CTrackerBase::GetTrackedObjects()
{
    return &TrackedObjects;
}

FLOAT64 CTrackerBase::GetCPAMeters()
{
    return CPAMeters;
}

FLOAT64 CTrackerBase::GetCPATimeSec()
{
    return CPATimeSec;
}
BOOLEAN CTrackerBase::GetIsTowardsCPA()
{
    return IsTowardsCPA;
}
STrackedObjectInfo* CTrackerBase::FindTrackByClientId(INT32S client_track_id)
{
    auto ret =  TrackClientIdMap.Find(client_track_id);
    return ret;
}

void CTrackerBase::AddTrack(STrackedObjectInfo* p_track)
{
    TrackedObjects.Add(p_track);
    TrackClientIdMap.Add(p_track->ClientId, p_track);
}
