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

	pSplineComponent = GetOwner()->GetComponentByClass<USplineComponent>(); // CUtil::FindComponent< USplineComponent>(GetOwner());

	



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
	if (pAttachedObject == nullptr) {
		return;
	}
	float len = pSplineComponent->GetSplineLength();

	FVector pos = pSplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FVector tangent = pSplineComponent->GetDirectionAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

	pAttachedObject->SetActorLocation(pos);
	FRotator rot = tangent.Rotation();

	// Get the current rotation
	FRotator currentRot = pAttachedObject->GetActorRotation();


	// Interpolate from the current rotation to the target rotation
	FRotator smoothRot = FMath::RInterpConstantTo(currentRot, rot, DeltaTime, TurnRateDegPerSec);



	pAttachedObject->SetActorRotation(smoothRot);
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

FVector USplineFollower::GetWaypointLocation(INT32S i)
{
	return Waypoints[i]->GetActorLocation();
}

void USplineFollower::UpdateWaypointsPositions()
{
	pSplineComponent->ClearSplinePoints();
	for (auto wp : Waypoints) {

		pSplineComponent->AddSplineWorldPoint(wp->GetActorLocation());
	}


	pSplineComponent->UpdateSpline();
}

void USplineFollower::RemoveWaypoint(AWaypointActor* p_wp)
{

	INT32S ind = 0;

	for (auto wp : Waypoints) {

		if (wp == p_wp) {
			pSplineComponent->RemoveSplinePoint(ind);
			Waypoints.Remove(p_wp);
			return;
		}
		ind++;
	}
}

bool USplineFollower::RemoveAttachedActor(AActor* p_actor)
{
	if (pAttachedObject == p_actor) {
		pAttachedObject = nullptr;
		return true;
	}
	else {
		return false;
	}
}

