// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Agent/AgentProcessorBase.h"
#include "BurcAgentProcessor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UBurcAgentProcessor : public UAgentProcessorBase
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

	UFUNCTION(BlueprintCallable)
		bool AssignTarget(FString target_name, double duration);

	UStaticMeshComponent* pGun;
	AActor* pTarget;

};
