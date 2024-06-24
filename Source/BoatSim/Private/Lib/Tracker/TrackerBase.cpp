// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Tracker/TrackerBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>


bool STrackedObjectInfo::IsAcquireTimeout(FLOAT64 timeout_sec)
{
    return (FApp::GetCurrentTime() - AcquireStartTimeRefSec) > timeout_sec;
}
void STrackedObjectInfo::HandleTemporayTargetLoss(FLOAT64 timeout_sec, bool& is_permanant_loss)
{
    if (TargetLossTimeRefSec < 0) {
        TargetLossTimeRefSec = FApp::GetCurrentTime();
    }
    if ((FApp::GetCurrentTime() - TargetLossTimeRefSec) > timeout_sec) {
        is_permanant_loss = true;
    }
    else {
        is_permanant_loss = false;
    }
}

void STrackedObjectInfo::SetState(EObjectTrackState state)
{
    if (TrackState != state) {
        StateEnterTimeSec[state] = FApp::GetCurrentTime();
        StateStayDurationSec[state] = 0;
    }
    else {
        StateStayDurationSec[state] = FApp::GetCurrentTime() - StateEnterTimeSec[state];
    }
    TrackState = state;
}

FLOAT64 STrackedObjectInfo::GetStateStayDurationSec(EObjectTrackState state)
{
    return StateStayDurationSec[state];
}
FLOAT64 STrackedObjectInfo::GetCurrentStateStayDuration()
{
    return StateStayDurationSec[TrackState];
}

bool STrackedObjectInfo::IsAcquired()
{
    return	TrackState == EObjectTrackState::AcquiredAndSafe ||
        TrackState == EObjectTrackState::AcquiredAndDangerous ||
        TrackState == EObjectTrackState::AcquiredSafeAndTemprorayLoss ||
        TrackState == EObjectTrackState::AcquiredDangerousAndTemprorayLoss;

}

bool STrackedObjectInfo::IsSafeToDelete()
{
    return TrackState == EObjectTrackState::LostOutOfRange ||
           TrackState == EObjectTrackState::LostTarget ||
           TrackState == EObjectTrackState::AcquireFailure;
}


CTrackerBase::CTrackerBase()
{
}

CTrackerBase::~CTrackerBase()
{
}

TArray<STrackedObjectInfo*>* CTrackerBase::GetTrackedObjects()
{
    return &TrackedObjects;
}

INT32S CTrackerBase::GetTrackedObjectCount()
{
    return TrackedObjects.Num();
}

FLOAT64 CTrackerBase::GetCPAMeters()
{
    return CPAMeters;
}

FLOAT64 CTrackerBase::GetCPATimeSec()
{
    return CPATimeSec;
}
bool CTrackerBase::GetIsTowardsCPA()
{
    return IsTowardsCPA;
}
STrackedObjectInfo* CTrackerBase::FindTrackByClientId(INT32S client_track_id)
{
    STrackedObjectInfo** ret = TrackClientIdMap.Find(client_track_id);
    if (ret == nullptr) {
        return nullptr;
    }
    return *ret;
}
bool CTrackerBase::IsActorInAnyTrack(AActor* p_actor)
{
    for (auto track : TrackedObjects) {
        if (track->pActor == p_actor) {
            return true;
        }
    }

    return false;

}
void CTrackerBase::AddTrack(STrackedObjectInfo* p_track)
{
    TrackedObjects.Add(p_track);
    TrackClientIdMap.Add(p_track->ClientId, p_track);
}

void CTrackerBase::RemoveTrack(STrackedObjectInfo* p_track)
{
    TrackedObjects.Remove(p_track);
    TrackClientIdMap.Remove(p_track->ClientId);

    delete p_track;

}
void CTrackerBase::SetOwnshipData(AActor *p_owner, FVector own_ship_location, FVector rpy_deg, FVector own_ship_vel, FVector2D radar_range_meter, FLOAT64 error_mean_meter, FLOAT64 error_mean_std)
{
    pOwnShip = p_owner;
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
#if false
    if (FindTrackByClientId(client_id) != nullptr) { // first cancel it
        return false;
    }
#endif

    if (TrackedObjects.Num() >= MaxTrackCount) {
        return false;
    }
   
    STrackedObjectInfo* p_track = new STrackedObjectInfo();
    p_track->ClientId = client_id;
    p_track->TrackerId = TrackerId++;

    FQuat quat(FVector::UpVector, bearing_true_north_deg * DEGTORAD);

    FVector dir = quat * FVector::ForwardVector;
    FVector target_pos = pos + dir * TOUE(distance_meter); //// todo fix me on ship or current position
    p_track->TrackLocationWhenCreated = target_pos;

    AddTrack(p_track);

    return false;
}

bool CTrackerBase::CancelAll()
{
    IsCancelAllTrack = true;
    return true;
}

bool CTrackerBase::CancelTrack(INT32U id)
{
    auto p_track = CTrackerBase::FindTrackByClientId((INT32S)id);

    if (p_track != nullptr) {
        CancalTrackRequest.Add(id);
        return true;
    }
    return false;
}
void CTrackerBase::ProcessCancelTrackRequests()
{
    if (IsCancelAllTrack) {
        while (TrackedObjects.Num() > 0) {
            RemoveTrack(TrackedObjects[0]);
        }
        IsCancelAllTrack = false;
    }
    else {
        for (INT32S i : CancalTrackRequest) {
            auto p_track = CTrackerBase::FindTrackByClientId((INT32S)i);
            if (p_track != nullptr) {
                RemoveTrack(p_track);
            }
        }
    }

    CancalTrackRequest.Reset();
    
}
bool CTrackerBase::TryAcquire(STrackedObjectInfo* p_track, bool& is_safe_target)
{

    FVector target_pos = p_track->TrackLocationWhenCreated;

    FLOAT64  dist = (target_pos - OwnShipLocation).Length();
    FVector dir = (target_pos - OwnShipLocation);
    dir.Normalize();

    if (TOW(dist) <= RadarRangeMeter.X) {
        return false;
    }

    TArray<AActor*> ignore_list;
    
    CUtil::GetOwnAndParents(pOwnShip, ignore_list);

    auto actor = ASystemManagerBase::GetInstance()->GetVisibleActorAt(ignore_list, OwnShipLocation + dir * TOUE(RadarRangeMeter.X), target_pos, TrackerDistanceToleranceMeter);
    
    if (actor != nullptr) {
        if (!IsActorInAnyTrack(actor)) {
            is_safe_target = true;
            p_track->pActor = actor;
            return true;
        }      
    }
    return false;
}

bool CTrackerBase::CheckStillAquired(STrackedObjectInfo* p_track, bool& is_safe_target)
{
    TArray<AActor*> ignore_list;
    CUtil::GetOwnAndParents(pOwnShip, ignore_list);

    auto actor = ASystemManagerBase::GetInstance()->GetVisibleActorAt(ignore_list, OwnShipLocation, p_track->pActor->GetActorLocation(), TrackerDistanceToleranceMeter);
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
    FillTrackInfo(p_track);
}
FLOAT64 CTrackerBase::ComputeTrackBearingDeg(STrackedObjectInfo* p_track)
{
    FVector target_pos_to_own_ship = p_track->pActor->GetTargetLocation() - OwnShipLocation;
    auto val = FMath::Atan2(target_pos_to_own_ship.Y, target_pos_to_own_ship.X)* RADTODEG;
    if (val < 0) {
        val += 360;
    }
    return val;
}

FVector CTrackerBase::GetTrackVelocity(STrackedObjectInfo* p_track)
{
    FVector val;

    if (p_track->pActor->IsA<AActorBase>()) {
        AActorBase* p_actor_base = (AActorBase*)p_track->pActor;
        val = p_actor_base->GetActorVelocityMetersPerSec();
    }
    else {
        val = p_track->pActor->GetVelocity();
    }
    return val;
}
void CTrackerBase::FillTrackInfo(STrackedObjectInfo* p_track)
{
    /*

    FLOAT64 AbsoluteDistanceMeter;
    FLOAT64 AbsoluteBearingDeg;
    FLOAT64 AbsoulteTargetSpeedMetersPerSec;
    FLOAT64 AbsoulteTargetCourseDeg;

    FLOAT64 RelativeDistanceMeter;
    FLOAT64 RelativeBearingDeg;
    FLOAT64 RelativeTargetSpeedMetersPerSec;
    FLOAT64 RelativeTargetCourseDeg;

    */
    if (p_track->pActor == nullptr) {
        p_track->AbsoluteDistanceMeter = 0;
        p_track->AbsoluteBearingDeg = 0;
        p_track->AbsoulteTargetSpeedMetersPerSec = 0;
        p_track->AbsoulteTargetCourseDeg = 0;

        p_track->RelativeDistanceMeter = 0;
        p_track->RelativeBearingDeg = 0;
        p_track->RelativeTargetSpeedMetersPerSec = 0;
        p_track->RelativeTargetCourseDeg = 0;
        return;
    }
    FVector target_pos_to_own_ship = p_track->pActor->GetTargetLocation() - OwnShipLocation;
    FVector vel = GetTrackVelocity(p_track);
    FVector owner_vel = CUtil::GetActorVelocityMetersPerSec(pOwnShip);

    p_track->AbsoluteDistanceMeter = TOW(target_pos_to_own_ship.Length());
    p_track->AbsoluteBearingDeg = ComputeTrackBearingDeg(p_track);
    p_track->AbsoulteTargetSpeedMetersPerSec = vel.Length();
    p_track->AbsoulteTargetCourseDeg = FMath::Atan2(vel.Y, vel.X) * RADTODEG;

    p_track->RelativeDistanceMeter = p_track->AbsoluteDistanceMeter;    
    p_track->RelativeBearingDeg = CUtil::GetActorRPY(pOwnShip).Z - CUtil::GetActorRPY(p_track->pActor).Z;
    p_track->RelativeTargetSpeedMetersPerSec = (vel - owner_vel).Length();
    auto rel_vel = (vel - owner_vel);
    p_track->RelativeTargetCourseDeg = FMath::Atan2(rel_vel.Y, rel_vel.X) * RADTODEG;

}
void CTrackerBase::UpdateCPA()
{
    FLOAT64 closest_track_dist = MAX_FLT;
    FLOAT64 closest_track_bearing_true_north;
    AActor* p_closest_actor = nullptr;

    for (auto p_track : TrackedObjects) {

        if (p_track->IsAcquired()) {
            auto target_vec = p_track->pActor->GetTargetLocation() - OwnShipLocation;

            auto dist = (target_vec).Length();

            if (dist < closest_track_dist) {
                closest_track_dist = dist;
                target_vec.Normalize();
                closest_track_bearing_true_north = FMath::Atan2(target_vec.Y, target_vec.X) * RADTODEG;
                p_closest_actor = p_track->pActor;
                closest_track_dist = dist;
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
void CTrackerBase::Update()
{

    ProcessCancelTrackRequests();

    for (auto p_track : TrackedObjects) {
        UpdateTrackState(p_track);
    }
    TArray<STrackedObjectInfo*> object_to_remove;

    for (auto p_track : TrackedObjects) {
        if (p_track->IsSafeToDelete()) {
            object_to_remove.Add(p_track);
        }
    }

    for (auto p_track : object_to_remove)
    {
        TrackedObjects.Remove(p_track);
    }

    UpdateCPA();
    
}
