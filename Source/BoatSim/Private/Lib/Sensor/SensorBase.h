// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Async/TaskGraphInterfaces.h"
#include "GameFramework/Actor.h"
#include "GenericCommIF/GenericCommIF.h"
#include <Lib/PointVisualizer/PointVisualizer.h>
#include <Lib/ActorBase/ActorBase.h>
#include "Lib/Clutter/ClutterDefs.h"
#include <Lib/ScreenDepth/SceneCapturer.h>

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
		bool UseRenderTargetForDepthCalculation = false;

	UPROPERTY(EditAnywhere)
		int DepthRenderTargetWidthPx = 960;
	
	UPROPERTY(EditAnywhere)
		int DepthRenderTargetHeightPx = 540;

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
		double MeasurementErrorMean = 0.0f;

	UPROPERTY(EditAnywhere)
		double MeasurementErrorUncertainy = 0.0f;

	// Use the UPROPERTY macro to expose properties to the Unreal Editor
	UPROPERTY(EditAnywhere, Category = "Clutter Params")
		bool EnableSubsurfaceDetect;

	UPROPERTY(EditAnywhere, Category = "Clutter Params")
		bool EnableSurfaceDetect;

	UPROPERTY(EditAnywhere, Category = "Clutter Params")
		bool EnableFoamDetect;

	UPROPERTY(EditAnywhere, Category = "Clutter Params")
		double SeaSurfaceDetectionProbability; // Unreal typically uses "float" rather than "FLOAT32".

	UPROPERTY(EditAnywhere, Category = "Clutter Params")
		double MaxSurfacePenetrationMeter; 


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UGenericCommIF* pCommIF;
	APointVisualizer* pPointVisualizer;
	ASceneCapturer* pSceneCapturer;
	ESensorState SensorState = ESensorState::Init;
	
	virtual void OnDataReady();
	virtual void InitSensor();
	virtual void Run(float delta_time_sec);
	virtual void FinalizeSensor();
	SClutterParams GetClutterParams();
	FLOAT64 CaptureStartTimeRef;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Visualize(SScanResult* p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter, void *p_tracker = nullptr);
	UGenericCommIF* GetCommCommIF();
	void SensorStateMachine(float delta_time_sec);
	virtual void OnCaptureReady(void *p_data);
};
