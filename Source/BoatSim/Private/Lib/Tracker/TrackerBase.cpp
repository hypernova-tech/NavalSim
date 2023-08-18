// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Tracker/TrackerBase.h"

CTrackerBase::CTrackerBase()
{
}

CTrackerBase::~CTrackerBase()
{
}

bool CTrackerBase::TryTrack(INT32U client_id, FVector pos, FLOAT64 bearing_true_north_deg, FLOAT64 distance_meter)
{
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

void CTrackerBase::Update()
{
}

TArray<STrackedObjectInfo*> CTrackerBase::GetTrackedObjects()
{
    return TrackedObjects;
}
