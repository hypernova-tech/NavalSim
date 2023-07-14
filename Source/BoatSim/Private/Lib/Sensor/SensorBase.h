// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SensorBase.generated.h"

UENUM(BlueprintType)
enum class ESensorType
{
	Unknown,
	Lidar,
	Radar,
	FLS3D,
	AIS,
	CameraVisible,
	CameraIR
};


UCLASS()
class ASensorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensorBase();

	UPROPERTY(EditAnywhere)
		ESensorType SensorType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
