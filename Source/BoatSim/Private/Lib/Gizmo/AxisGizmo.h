// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Lib/Types/Primitives.h>
#include "AxisGizmo.generated.h"




UCLASS()
class AAxisGizmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAxisGizmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<UStaticMeshComponent*> AxisComponents;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	ECoordAxis GetAxis(UPrimitiveComponent* p_hit);
	
};
