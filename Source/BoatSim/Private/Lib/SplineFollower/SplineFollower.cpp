// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SplineFollower/SplineFollower.h"
#include <Lib/Utils/CUtil.h>

// Sets default values for this component's properties
USplineFollower::USplineFollower()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void USplineFollower::BeginPlay()
{
	Super::BeginPlay();
	pSplineComponent = CUtil::FindComponent< USplineComponent>(GetOwner());
	//pSplineComponent->DetachFromParent(true);
	//pSplineComponent->SetMobility(EComponentMobility::Static);
	//SetMobility(EComponentMobility::Movable);

	
	pSplineComponent->ClearSplinePoints(true);


#if false
	for (AWaypointActor* p_wp : Waypoints) {
		FVector pos = p_wp->GetActorLocation();
		pSplineComponent->AddSplineWorldPoint(pos);
	}

	pSplineComponent->SetClosedLoop(true, true);
	pSplineComponent->UpdateSpline();

#endif

	
}

bool Once = false;
// Called every frame
void USplineFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USplineFollower::OnStep(float DeltaTime)
{
	
	float len = pSplineComponent->GetSplineLength();

	FVector pos = pSplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FVector tangent = pSplineComponent->GetDirectionAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

	pAttachedObject->SetActorLocation(pos);
	FRotator rot = tangent.Rotation();
	pAttachedObject->SetActorRotation(rot);
	CurrentDistance += WORLD_TO_UNREAL(MoveSpeedMetersPerSec) * DeltaTime;
	if (CurrentDistance >= len) {
		CurrentDistance = 0;
	}
}

void USplineFollower::AddWaypoint(FVector vec, AWaypointActor* p_wp_actor)
{
	pSplineComponent->AddSplineWorldPoint(TOUE(vec));
	Waypoints.Add(p_wp_actor);
}

void USplineFollower::ModifyWaypoint(INT32S wp_ind, FVector pos)
{
	if (wp_ind < GetWaypointCount()) {
		pSplineComponent->SetLocationAtSplinePoint(wp_ind, pos, ESplineCoordinateSpace::World);
	}
}

void USplineFollower::BuildPath(bool is_loop)
{
	pSplineComponent->SetClosedLoop(is_loop, true);
	pSplineComponent->UpdateSpline();
}

INT32S USplineFollower::GetWaypointCount()
{

	return Waypoints.Num();
}

