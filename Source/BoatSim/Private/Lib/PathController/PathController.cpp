// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/PathController/PathController.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

void APathController::BeginPlay()
{
	Super::BeginPlay();
	pSplineFollower  = FindComponentByClass<USplineFollower>();
}

void APathController::OnStep(float DeltaTime)
{
	Super::OnStep(DeltaTime);
	pSplineFollower->OnStep(DeltaTime);

}

void APathController::AddOrModifyWaypoint(INT32S wp_ind,  FVector wp)
{
	if (wp_ind >= pSplineFollower->GetWaypointCount()) {
		FString name = GetName() + "wp" + CUtil::IntToString(wp_ind);
		auto wp_actor = ASystemManagerBase::GetInstance()->CreateActor("WP", name, TOUE(wp), FVector::ZeroVector, FVector::OneVector);
		pSplineFollower->AddWaypoint(wp, (AWaypointActor*)wp_actor);
	}
	else {
		pSplineFollower->ModifyWaypoint(wp_ind, wp);
	}

}

bool APathController::GetWaypointPosition(INT32S ind, FVector &ret)
{
	if(ind < pSplineFollower->Waypoints.Num()){
		ret = pSplineFollower->Waypoints[ind]->GetActorLocation();
		return true;
	}
	return false;
}

void APathController::Bake()
{
	pSplineFollower->BuildPath(IsClosed);
}

void APathController::SetIsClosed(bool val)
{
	IsClosed = val;
}

bool APathController::GetIsClosed()
{
	return IsClosed;
}

void APathController::SetPathSpeed(FLOAT64 val)
{
	pSplineFollower->MoveSpeedMetersPerSec = val;
}
FLOAT64 APathController::GetPathSpeed()
{
	return pSplineFollower->MoveSpeedMetersPerSec;
}

void APathController::AddAttachedActor(AActor* p_actor)
{
	pSplineFollower->pAttachedObject = p_actor;
}

AActor* APathController::GetAttachedActor()
{
	return pSplineFollower->pAttachedObject;
}

