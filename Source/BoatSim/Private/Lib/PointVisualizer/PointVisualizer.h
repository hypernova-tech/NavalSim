// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PointVisualizer.generated.h"

UCLASS()
class APointVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointVisualizer();

	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* ProceduralMeshComponent;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneCaptureComponent2D* pSceneCapture2D;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Visualize(TArray<FVector> points);
};
