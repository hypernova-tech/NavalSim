// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <Components/SplineComponent.h>
#include <Lib/Waypoint/WaypointActor.h>
#include "SplineFollower.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USplineFollower : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineFollower();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere)
		AActor* pAttachedObject;

	UPROPERTY(EditAnywhere)
		USplineComponent* pSplineComponent;

	UPROPERTY(EditAnywhere)
		TArray<FVector> Points;

	UPROPERTY(EditAnywhere)
		float MoveSpeedMetersPerSec = 5;

	UPROPERTY(EditAnywhere)
		TArray<AWaypointActor*> Waypoints;

	UPROPERTY(EditAnywhere)
		float TurnRateDegPerSec = 90;

	virtual void OnStep(float delta_time);
private:


private:

	float CurrentDistance = 0;


public:

	void AddWaypoint(FVector vec, AWaypointActor* p_wp_actor);
	void ModifyWaypoint(INT32S wp_ind, FVector pos);
	void BuildPath(bool is_loop);
	INT32S GetWaypointCount();
	FVector GetWaypointLocation(INT32S i);
	void UpdateWaypointsPositions();
	void RemoveWaypoint(AWaypointActor* p_wp);
	bool RemoveAttachedActor(AActor *p_actor);
		
};
