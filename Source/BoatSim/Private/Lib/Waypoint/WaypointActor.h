// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Lib/ActorBase/ActorBase.h>
#include "WaypointActor.generated.h"

UCLASS()
class AWaypointActor : public AActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypointActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AActor* pOwnerPath;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AActor* GetOwnerPath();
	void SetOwnerPath(AActor *p_path);

	virtual void OnActorPredestroy() override;
};
