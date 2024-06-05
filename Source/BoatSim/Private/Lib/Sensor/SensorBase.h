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
#include <Lib/Utils/CUtil.h>
//#include "PointCloudComponent.h"
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
	Idle,
	Init,
	Running,
	Paused,
	Finish,
	
};

UCLASS()
class ASensorBase : public AActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensorBase();

	UPROPERTY(EditAnywhere, meta = (SaveData = "true", cli = "sensortype"))
		ESensorType SensorType;
	UPROPERTY(EditAnywhere, meta = (SaveData = "true", cli = "depthcalc"))
		bool UseRenderTargetForDepthCalculation = false;

	UPROPERTY(EditAnywhere)
		int DepthRenderTargetWidthPx = 960;
	
	UPROPERTY(EditAnywhere)
		int DepthRenderTargetHeightPx = 540;

	UPROPERTY(EditAnywhere)
		int SensorSlotIndex;

	UPROPERTY(EditAnywhere)
		bool GenericSensorOutputEnabled = false;

	UPROPERTY(EditAnywhere)
		float UpdateFrequency;

	UPROPERTY(EditAnywhere)
		bool Point3DVisualize = false;

	UPROPERTY(EditAnywhere)
		bool  SimulatePhysicsEnabled = false;

	UPROPERTY(EditAnywhere)
		bool ShowBeam = false;

	UPROPERTY(EditAnywhere)
		bool ShowPointCloud = false;

	UPROPERTY(EditAnywhere)
		double Frequency = 1.0;

	UPROPERTY(EditAnywhere) 
		double  RangeMinMeter = 5;

	UPROPERTY(EditAnywhere)
		double  RangeMaxMeter = 100;

	UPROPERTY(EditAnywhere)
		double NoiseMean;

	UPROPERTY(EditAnywhere)
		double NoiseStdDeviation;

	UPROPERTY(EditAnywhere)
		double FovVerticalDeg = 10;

	UPROPERTY(EditAnywhere)
		double FovHorizontalDeg = 90;

	UPROPERTY(EditAnywhere)
		double EachScanBeamWidthDeg = 45;

	UPROPERTY(EditAnywhere)
		float HorizontalScanStepAngleDeg;

	UPROPERTY(EditAnywhere)
		float VerticalScanStepAngleDeg = 36;

	UPROPERTY(EditAnywhere)
		float FullScanPeriodSec = 1;

	UPROPERTY(EditAnywhere)
		bool IsNormalDistribution;

	UPROPERTY(EditAnywhere)
		double MeasurementErrorMean = 0.0f;

	UPROPERTY(EditAnywhere)
		double MeasurementErrorUncertainy = 0.0f;

	UPROPERTY(EditAnyWhere)
		double MeasurementErrorUncertainyScale = 1.0;

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
	//UPointCloudComponent* pPointCloud;
	virtual void BeginPlay() override;

	
	APointVisualizer* pPointVisualizer;
	ASceneCapturer* pSceneCapturer;
	ESensorState SensorState = ESensorState::Init;
	
	virtual void OnDataReady();
	virtual void InitSensor();
	virtual void Run(float delta_time_sec); 
	virtual void FinalizeSensor();
	virtual void PauseSensor();
	virtual void ResumeSensor();
	virtual void OnPreTrace(STraceArgs& args);
	virtual TArray<AActor*>* GetScanAdditionalIncludes();
	SClutterParams GetClutterParams();
	FLOAT64 CaptureStartTimeRef;
	TArray<AActor*> ScanAdditionalIncludes;
	virtual void OnStep(float DeltaTime) override;

public:	
	// Called every frame


	virtual void Visualize(SScanResult* p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter, void *p_tracker = nullptr, bool is_reset_chart = true);
	UGenericCommIF* GetCommCommIF();
	void SensorStateMachine(float delta_time_sec);
	virtual void OnCaptureReady(void *p_data);


	void SetFrequency(double val);
	double GetFrequency();

	void SetRangeMeter(FVector2D val);
	FVector2D GetRangeMeter();

	void SetNoiseMean(double val);
	double GetNoiseMean();

	void SetNoiseStdDeviation(double val);
	double GetNoiseStdDeviation();

	void SetFovVerticalDeg(double val);
	double GetFovVerticalDeg();

	void SetFovHorizontalDeg(double val);
	double GetFovHorizontalDeg();



	float GetHorizontalScanStepAngleDeg();
	void SetHorizontalScanStepAngleDeg(float val);

	float GetVerticalScanStepAngleDeg();
	void SetVerticalScanStepAngleDeg(float val);


	virtual void Save(ISaveLoader *p_save_loader) override;
	virtual void SaveJSON(CJsonDataContainer& data) override;


	void ShowPointCloudData(const TArray<FVector>& cloud);

};
