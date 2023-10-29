// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Lib/ActorBase/ActorBase.h"
#include "Agent.generated.h"

UCLASS()
class AAgent : public AActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAgent();
	void OnConstruction(const FTransform& Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UStaticMesh* pStaticMesh;

	UStaticMeshComponent* MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
