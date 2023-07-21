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
		USplineComponent* pSplineComponent;

	UPROPERTY(EditAnywhere)
		TArray<FVector> Points;

	UPROPERTY(EditAnywhere)
		float MoveSpeedMetersPerSec = 5;

	UPROPERTY(EditAnywhere)
		TArray<AWaypointActor*> Waypoints;
private:


private:

	float CurrentDistance = 0;
		
};
