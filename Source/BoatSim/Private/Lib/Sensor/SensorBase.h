// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericCommIF/GenericCommIF.h"
#include <Lib/PointVisualizer/PointVisualizer.h>
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

	UPROPERTY(EditAnywhere)
		float UpdateFrequency;

	UPROPERTY(EditAnywhere)
		bool Point3DVisualize = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Run(float delta_time_sec);
	UGenericCommIF* pCommIF;
	APointVisualizer* pPointVisualizer;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Visualize(SScanResult* p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter);

};
