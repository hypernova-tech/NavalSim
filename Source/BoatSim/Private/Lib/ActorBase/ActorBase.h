// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lib/Types/Primitives.h"
#include <Lib/SaveLoad/ISaveLoader.h>
#include <Lib/JSON/CJsonDataContainer.h>
#include <Lib/Sensor/GenericCommIF/GenericCommIF.h>
#include <Lib/ActorBase/ActorIF.h>
#include "CMotionLogger.h"
#include "ActorBase.generated.h"





UCLASS()
class AActorBase : public AActor
{
	GENERATED_BODY()


protected:

	virtual void OnPreStep(float DeltaTime);
	virtual void OnStep(float DeltaTime);
	virtual void OnStepScenarioMode(float DeltaTime);
	virtual void OnPreStepScenarioMode(float DeltaTime);
	bool CheckAffinity();
	
public:	
	// Sets default values for this actor's properties
	AActorBase();

	UPROPERTY(EditAnywhere)
		bool IsSaveLoadCreateEnabled = true;
protected:
	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere)
		bool IsSaveEnabled = true;


	UPROPERTY(EditAnywhere)
		bool UsePrefixName = false;

	UPROPERTY(EditAnywhere)
		FString PrefixName = "";

	UPROPERTY(EditAnywhere)
		FString BlueprintName = "";

	UPROPERTY(EditAnywhere)
		int TickRequiredForCreation = 0;

	UPROPERTY(EditAnywhere)
		int ActorId;

	UPROPERTY(EditAnywhere)
		bool IsRegisterEnabled = true;


	UPROPERTY(EditAnywhere)
		bool IsExternalUpdate = true;

	UPROPERTY(EditAnywhere)
		bool IsExternalUpdateScenarioMode = false;

	UPROPERTY(EditAnywhere)
		bool Suppressed = false;

	UPROPERTY(EditAnywhere)
		bool Enabled = false;

	UPROPERTY(EditAnywhere)
		bool StandaloneModeEnabled = false;

	UPROPERTY(EditAnywhere)
		int AffinityInstanceId = -1;

	UPROPERTY(EditAnywhere)
		bool IsBlockingObject = true;

	UPROPERTY(EditAnywhere)
		bool IsMotionLogEnabled = false;

	UPROPERTY(EditAnywhere)
		double MotionLogLifeTimeSec = 60;

	UPROPERTY(EditAnywhere)
		double MotionLogMinDistanceMeter = 5;

	UPROPERTY(EditAnywhere)
		AActor* ActorGizmo;

	UPROPERTY(EditAnywhere)
		bool IsHeatSource_ = false;

	UPROPERTY(EditAnywhere)
		double TempratureKelvin = 300;

	UPROPERTY(EditAnywhere)
		int AnnotationId_ = 0;

	UPROPERTY(EditAnywhere)
		bool AnnotateChildrenActors_ = false;

	UPROPERTY(EditAnywhere)
		TArray<USceneComponent*> ManuallyAttachComponents;

	UPROPERTY(EditAnywhere)
		bool KeepPhysicsEnabledComponentRelativeTransformConstant = false;



	INT64U ExternalUpdateCount = 0;
	INT64U ExternalUpdateCountScenarioMode = 0;
	double VelPreviousTime;
	double AngVelPreviousTime;
	FVector PreviousPosition;
    FRotator PreviousRotation;

	FVector ActorVelocityMetersPerSec;
	FVector ActorAngularVelocityRPYDegPerSec;
	CMotionLogger MotionLogger;

	 virtual void OnActorDisabled();
	 virtual void OnActorEnabled();

	 UGenericCommIF* pCommIF;

	 UFUNCTION(BlueprintCallable)
		 void AddManuallyAttach(USceneComponent* p_comp);
	 static  void HandleAnnotation(AActor* p_actor, bool is_enabled, int annotation_id);

	 virtual FVector UpdateActorVelocityMetersPerSec();
	 virtual FVector UpdateActorAngularVelocityRPYDegPerSec();
	 virtual void UpdateMotionLog();


public:	
	// Called every frame

	UPROPERTY(EditAnywhere)
		FString ProtocolConverterSharedMemoryName = "";

	UPROPERTY(EditAnywhere)
		FString GStreamerIP = "";

	UPROPERTY(EditAnywhere)
		int GStreamerPort = 0;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetActorId(int val);
	virtual int GetActorId();

	virtual void SetEnabled(bool val) ;
	virtual bool GetEnabled() ;

	virtual void SetSuppressed(bool val);
	virtual bool GetSuppressed();

	virtual void SetStandaloneModeEnabled(bool val);
	virtual bool GetStandaloneModeEnabled();

	bool GetIsExternalUpdateScenarioMode();
	void ExternalUpdate(float DeltaTime);
	void ExternalUpdateScenarioMode(float DeltaTime);



	void SetAffinityInstanceId(int val);
	int GetAffinityInstanceId();



	void ShowActorGizmo(bool val);
	virtual void Bake();
	virtual void OnActorPredestroy();
	void SetIsSaveEnabled(bool val);
	bool GetIsSaveEnabled();

	virtual void SetActorPosition(FVector pos);
	virtual void SetActorRelPosition(FVector pos);
	virtual void SetActorRot(FVector rpy_deg);
	virtual void SetActorRelRot(FVector rpy_deg);

	bool GetConnnectionInfo(INT32S ind, SConnectionInfo& info);
	bool SetConnnectionInfo(INT32S ind, SConnectionInfo info);
	void SaveConnection(FString& line, FString ip_addr_param, FString local_port_param, FString remote_port_param, ISaveLoader* p_save_loader, SConnectionInfo& conn);
	
	UFUNCTION(BlueprintCallable)
	void AddChild(USceneComponent* p_comp);

	bool GetIsHeatSource();

	double	GetTempratureKelvin();
	virtual void	SetTempratureKelvin(double val);

	UFUNCTION(BlueprintCallable)
		bool GetIsHeatSource_();
	UFUNCTION(BlueprintCallable)
		void  SetIsHeatSource_(bool val);



	UFUNCTION(BlueprintCallable)
	int GetAnnotationId_();
	UFUNCTION(BlueprintCallable)
	virtual void  SetAnnotationId_(int val);
	
	UFUNCTION(BlueprintCallable)
		void  SetAnnotateChildrenActors_(bool val);

	UFUNCTION(BlueprintCallable)
		bool  GetAnnotateChildrenActors_();


	virtual void UpdateAnnotation(bool is_enabled);

	FVector GetPositionLatLongHeightMSL();
	FVector GetPositionXYZMeters();
	FVector GetActorVelocityMetersPerSec();
	FVector GetActorAngularVelocityRPYDegPerSec();

	virtual bool IsColldingWithBlockingObject();
	virtual bool CanMoveTowards(FVector dir);

	bool GetIsBlockingObject();
	void SetIsBlockingObject(bool val);

	void SetIsMotionLogEnabled(bool val);
	bool GetIsMotionLogEnabled();

	void SetMotionLogLifeTimeSec(double val);
	double GetMotionLogLifeTimeSec();

	void SetMotionLogMinDistanceMeter(double val);
	double GetMotionLogMinDistanceMeter();

	CMotionLogger* GetMotionLogger();



	/// <summary>
	/// save load
	/// </summary>
public:
	FString GetBlueprintName();
	virtual void Save(ISaveLoader* p_save_loader);
	virtual void SaveJSON(CJsonDataContainer& data);

};
