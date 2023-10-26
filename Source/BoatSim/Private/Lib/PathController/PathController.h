// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include <Lib/SplineFollower/SplineFollower.h>
#include <Components/SplineMeshComponent.h>
#include "PathController.generated.h"



/**
 * 
 */
UCLASS()
class APathController : public AActorBase
{
	GENERATED_BODY()

public:
	APathController();
protected:

	USplineFollower* pSplineFollower;
	bool IsClosed;
	TArray< USplineMeshComponent*> PathSegments;
	void GenerateDrawablePathSegments();
	bool IsBaked = false;

public:
	UPROPERTY(EditAnywhere)
		int NumSegments = 50;

	UPROPERTY(EditAnywhere)
		UStaticMesh* pMesh;

	UPROPERTY(VisibleAnywhere)
		USplineMeshComponent* pSplineMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Materials")
		UMaterialInterface* pSplineMaterial;

	UPROPERTY(EditAnywhere)
		float LineoffsetAlongNormal = 0;

	UPROPERTY(EditAnywhere)
		bool IsStraightLine = false;

	UPROPERTY(EditAnywhere)
		float TurnRateDegPerSec = 90;

	UPROPERTY(EditAnywhere)
		FColor PathColor = FColor::Red;

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
	void UpdatePathColor();
	void NotifyWaypointMoved(AWaypointActor* p_wp);
	void NotifyWaypointDestroy(AWaypointActor* p_wp);

	virtual void Save(ISaveLoader* p_save_loader) override;

};
