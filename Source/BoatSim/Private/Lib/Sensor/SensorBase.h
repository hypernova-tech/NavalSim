// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericCommIF/GenericCommIF.h"
#include <Lib/PointVisualizer/PointVisualizer.h>
#include <Lib/ActorBase/ActorBase.h>
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

enum ESensorState
{
	Init,
	Running,
	Finish,
	
};


UCLASS()
class ASensorBase : public AActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensorBase();

	UPROPERTY(EditAnywhere)
		ESensorType SensorType;

	UPROPERTY(EditAnywhere)
		int SensorSlotIndex;

	UPROPERTY(EditAnywhere)
		float UpdateFrequency;



	UPROPERTY(EditAnywhere)
		bool Point3DVisualize = false;

	UPROPERTY(EditAnywhere)
		bool  SimulatePhysicsEnabled = false;

	UPROPERTY(EditAnywhere)
		bool ShowBeam = false;

	UPROPERTY(EditAnywhere)
		float MeasurementErrorMean = 0.0f;

	UPROPERTY(EditAnywhere)
		float MeasurementErrorUncertainy = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UGenericCommIF* pCommIF;
	APointVisualizer* pPointVisualizer;
	ESensorState SensorState = ESensorState::Init;
	virtual void SensorStateMachine(float delta_time_sec);

	virtual void InitSensor();
	virtual void Run(float delta_time_sec);
	virtual void FinalizeSensor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Visualize(SScanResult* p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter);
	UGenericCommIF* GetCommCommIF();

};
