// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Agent/Agent.h"
#include "BurcAgent.generated.h"

/**
 * 
 */
UCLASS()
class ABurcAgent : public AAgent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void AimGun(FVector pos);

	UPROPERTY(EditAnywhere)
		FString BulletAgentName;

	UFUNCTION(BlueprintCallable,Category = "Console")
		bool AssignTarget(FString target_name, double duration);

	UStaticMeshComponent* pGun;
	AActor* pTarget;
};
