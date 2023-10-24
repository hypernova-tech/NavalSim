// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include <Lib/SplineFollower/SplineFollower.h>
#include "PathController.generated.h"


/**
 * 
 */
UCLASS()
class APathController : public AActorBase
{
	GENERATED_BODY()


protected:

	USplineFollower* pSplineFollower;
	bool IsClosed;

public:
	virtual void BeginPlay() override;
	virtual void OnStep(float DeltaTime) override;

	virtual void AddOrModifyWaypoint(INT32S wp_ind, FVector wp);
	bool GetWaypointPosition(INT32S ind, FVector& ret);
	virtual void Bake() override;
	void SetIsClosed(bool val);
	bool GetIsClosed();
	void SetPathSpeed(FLOAT64 val);
	FLOAT64 GetPathSpeed();
	void AddAttachedActor(AActor *p_actor);
	AActor* GetAttachedActor();

};
