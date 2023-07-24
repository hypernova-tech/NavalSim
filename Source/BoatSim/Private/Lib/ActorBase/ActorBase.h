// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorBase.generated.h"

UCLASS()
class AActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		int ActorId;

	UPROPERTY(EditAnywhere)
		bool Enabled = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetEnabled(bool val);
	bool GetEnabled();
};
