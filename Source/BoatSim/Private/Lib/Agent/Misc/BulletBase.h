// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Agent/Agent.h"
#include "BulletBase.generated.h"

/**
 * 
 */
UCLASS()
class ABulletBase : public AAgent
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere)
		double MaxInitialSpeedMetersPerSec_ = 50;
	UPROPERTY(EditAnywhere)
		double MinInitialSpeedMetersPerSec_ = 50;
	virtual void BeginPlay() override;
	virtual void OnStep(float DeltaTime) override;



protected:
	UStaticMeshComponent* pBody;

public:
	void SetLinearVelocity(FVector val);
	FVector GetLinearVelocity();

	double GetSpeed();
};
