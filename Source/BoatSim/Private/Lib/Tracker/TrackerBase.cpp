// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Tracker/TrackerBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>


CTrackerBase::CTrackerBase()
{
}

CTrackerBase::~CTrackerBase()
{
}

void CTrackerBase::SetOwnshipData(FVector own_ship_location, FVector rpy_deg, FVector own_ship_vel, FVector2D radar_range_meter, FLOAT64 error_mean_meter, FLOAT64 error_mean_std)
{
    OwnShipLocation = own_ship_location;
    OwnShipRPY = rpy_deg;
    OwnShipVelocity = own_ship_vel;
    BearingToTargetDegTrueNorth = rpy_deg.Z;
    RadarRangeMeter = radar_range_meter;
    RadarRangeMeanErrorMeter = error_mean_meter;
    RadarRangeErrorStdDevMeter = error_mean_std;
}


bool CTrackerBase::TryTrack(INT32U client_id, FVector pos, FLOAT64 bearing_true_north_deg, FLOAT64 distance_meter)
{
    if (FindTrackByClientId(client_id) != nullptr) { // first cancel it
        return false;
    }

    if (TrackedObjects.Num() >= MaxTrackCount) {
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

bool CTrackerBase::CheckStillAquired(STrackedObjectInfo* p_track, bool& is_safe_target)
{
    auto actor = ASystemManagerBase::GetInstance()->GetVisibleActorAt(pOwnShip, OwnShipLocation, p_track->pActor->GetActorLocation(), 5);
    if (actor != nullptr) {
        if (actor == p_track->pActor) {
            return true;
        }
        
    }

    return false;
}

bool CTrackerBase::IsTargetOutofRange(STrackedObjectInfo* p_track)
{
    if (p_track->pActor != nullptr) {
        FLOAT64 dist_meter = TOW((p_track->pActor->GetActorLocation() - OwnShipLocation).Length());
        if (dist_meter > RadarRangeMeter.Y || dist_meter< RadarRangeMeter.X) {
            return true;
        }
    }

    return false;
}
void CTrackerBase::UpdateTrackState(STrackedObjectInfo* p_track)
{
    auto curr_state = p_track->TrackState;
    auto next_state = curr_state;
    bool is_safe_target = true;

    p_track->IsOutOfRange = IsTargetOutofRange(p_track);


    switch (curr_state) {
    case EObjectTrackState::JustCreated:
        next_state = EObjectTrackState::Acquiring;
        p_track->AcquireStartTimeRefSec = FApp::GetCurrentTime();
        break;
    case EObjectTrackState::Acquiring:
        if (TryAcquire(p_track, is_safe_target)) {
            if (!p_track->IsOutOfRange) {
                if (is_safe_target) {
                    next_state = EObjectTrackState::AcquiredAndSafe;
                }
                else {
                    next_state = EObjectTrackState::AcquiredAndDangerous;
                }
            }
            else {
                next_state = EObjectTrackState::OutOfRange;
            }
 
        }
        else {
            if (p_track->GetCurrentStateStayDuration() > TargetAquireTimeOutSec) {
                next_state = EObjectTrackState::AcquireFailure;
            }
        }
        break;
    case EObjectTrackState::AcquiredAndSafe:
        if (CheckStillAquired(p_track, is_safe_target)) {
            if (!p_track->IsOutOfRange) {
                if (is_safe_target) {
                    next_state = EObjectTrackState::AcquiredAndSafe;
                }
                else {
                    next_state = EObjectTrackState::AcquiredAndDangerous;
                }
            }
            else {
                next_state = EObjectTrackState::OutOfRange;
            }
        }
        else {
            next_state = EObjectTrackState::AcquiredSafeAndTemprorayLoss;
        }
        break;
    case EObjectTrackState::AcquiredAndDangerous:
        if (CheckStillAquired(p_track, is_safe_target)) {
            if (!p_track->IsOutOfRange) {
                if (is_safe_target) {
                    next_state = EObjectTrackState::AcquiredAndSafe;
                }
                else {
                    next_state = EObjectTrackState::AcquiredAndDangerous;
                }
            }
            else {
                next_state = EObjectTrackState::OutOfRange;
            }
        }
        else {
            next_state = EObjectTrackState::AcquiredDangerousAndTemprorayLoss;
        }
        break;
    case EObjectTrackState::AcquiredSafeAndTemprorayLoss:
        if (p_track->GetCurrentStateStayDuration() > TemprorayAcquireToLostTimeoutSec) {
            next_state = EObjectTrackState::LostTarget;
        }
        if (p_track->IsOutOfRange) {
            next_state = EObjectTrackState::OutOfRange;
        }
        break;
    case EObjectTrackState::AcquiredDangerousAndTemprorayLoss:
        if (p_track->GetCurrentStateStayDuration() > TemprorayAcquireToLostTimeoutSec) {
            next_state = EObjectTrackState::LostTarget;
        }
        if (p_track->IsOutOfRange) {
            next_state = EObjectTrackState::OutOfRange;
        }
        break;
    case EObjectTrackState::LostTarget:
        break;
    case EObjectTrackState::AcquireFailure:
        break;
    case EObjectTrackState::OutOfRange:
        if (p_track->IsOutOfRange) {
            if (p_track->GetCurrentStateStayDuration() > OutOfRangeToLostTimeoutSec) {
                next_state = EObjectTrackState::LostOutOfRange;
            }
        }
        else {
            if (CheckStillAquired(p_track, is_safe_target)) {
                if (is_safe_target) {
                    next_state = EObjectTrackState::AcquiredAndSafe;
                }
                else {
                    next_state = EObjectTrackState::AcquiredAndDangerous;
                }
            }
        }
        break;
    case EObjectTrackState::LostOutOfRange:
        break;
    case EObjectTrackState::AquireFailedTargetTrackCapacityFull:
        break;
    }

    p_track->SetState(next_state);
}

void CTrackerBase::Update()
{
    FLOAT64 closest_track_dist = MAX_FLT;
    FLOAT64 closest_track_bearing_true_north;
    AActor* p_closest_actor = nullptr;

    for (auto p_track : TrackedObjects) {
        UpdateTrackState(p_track);

        if (p_track->IsAcquired()) {
            auto target_vec = p_track->pActor->GetTargetLocation() - OwnShipLocation;

            auto dist = (target_vec).Length();

            if (dist > closest_track_dist) {
                dist = closest_track_dist;
                target_vec.Normalize();
                closest_track_bearing_true_north = FMath::Atan2(target_vec.X, target_vec.Y) * RADTODEG;
                p_closest_actor = p_track->pActor;
            }
        }
    }

    if (p_closest_actor != nullptr) {

        FVector relative_vel = OwnShipVelocity - p_closest_actor->GetVelocity();
        FVector relative_pos = p_closest_actor->GetActorLocation() - OwnShipLocation;

        auto relative_dist = relative_pos.Length();
        auto relative_speed = relative_vel.Length();

        relative_vel.Normalize();
        relative_pos.Normalize();

        auto dotp = FVector::DotProduct(relative_pos, relative_vel);

        if (dotp > 0) {
            IsTowardsCPA = true;
        }
        else {
            IsTowardsCPA = false;
        }

        CPAMeters = TOW(closest_track_dist);
        if (IsTowardsCPA) {
            if (relative_speed == 0) {
                CUtil::DebugLog("relative_speed is zero");
                relative_speed = 1e-12;
            }
            CPATimeSec = CPAMeters / relative_speed;
        }
        else {
            CPATimeSec = -1;
        }
    }
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
    STrackedObjectInfo** ret =  TrackClientIdMap.Find(client_track_id);
    return *ret;
}

void CTrackerBase::AddTrack(STrackedObjectInfo* p_track)
{
    TrackedObjects.Add(p_track);
    TrackClientIdMap.Add(p_track->ClientId, p_track);
}
