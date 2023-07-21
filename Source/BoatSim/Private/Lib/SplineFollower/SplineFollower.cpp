// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SplineFollower/SplineFollower.h"
#include <Lib/Utils/CUtil.h>

// Sets default values for this component's properties
USplineFollower::USplineFollower()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	


	

	// ...
}


// Called when the game starts
void USplineFollower::BeginPlay()
{
	Super::BeginPlay();
	pSplineComponent = CUtil::FindComponent< USplineComponent>(GetOwner());
	pSplineComponent->DetachFromParent(true);
	pSplineComponent->SetMobility(EComponentMobility::Static);
	SetMobility(EComponentMobility::Movable);

	
	pSplineComponent->ClearSplinePoints(true);

	for (AWaypointActor* p_wp : Waypoints) {
		FVector pos = p_wp->GetActorLocation();
		pSplineComponent->AddSplineWorldPoint(pos);
	}

	pSplineComponent->SetClosedLoop(true, true);
	pSplineComponent->UpdateSpline();
	// ...
	
}

bool Once = false;
// Called every frame
void USplineFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float len = pSplineComponent->GetSplineLength();
	
	FVector pos = pSplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FVector tangent = pSplineComponent->GetTangentAtSplineInputKey(CurrentDistance, ESplineCoordinateSpace::World);
	// ...
	


	// Set the actor's new rotation
	//GetOwner()->SetActorRotation(NewRotation);

	GetOwner()->SetActorLocation(pos);

	CurrentDistance += WORLD_TO_UNREAL( MoveSpeedMetersPerSec) * DeltaTime;
	if (CurrentDistance >= len) {
		CurrentDistance = 0;
	}

	if (!Once) {
		FVector prev_pos = pSplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
		for (float dist = 5; dist < len; dist += 5) {
			FVector pos1 = pSplineComponent->GetLocationAtDistanceAlongSpline(dist, ESplineCoordinateSpace::World);
			DrawDebugLine(GetWorld(), pos1, prev_pos, FColor::Magenta, false, 1000.0f, 0, 10.0f);
			prev_pos = pos1;
		}
		Once = true;
	}


}

