// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lib/Types/Primitives.h"
#include <Lib/SaveLoad/ISaveLoader.h>
#include <Lib/JSON/CJsonDataContainer.h>
#include <Lib/Sensor/GenericCommIF/GenericCommIF.h>
#include <Lib/ActorBase/ActorIF.h>
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
		int AISClassType = 0; // 0: none, 1: class A, 2: class B

	UPROPERTY(EditAnywhere)
		float AISMessagePublishPeriodSec = 10; // 0: none, 1: class A, 2: class B

	UPROPERTY(EditAnywhere)
		bool ShoudPublishATON = true; // 0: none, 1: class A, 2: class B


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

	INT64U ExternalUpdateCount = 0;
	INT64U ExternalUpdateCountScenarioMode = 0;
	double PreviousTime;
	FVector PreviousPosition;
    FRotator PreviousRotation;

	FVector ActorVelocityMetersPerSec;
	FVector ActorAngularVelocityRPYDegPerSec;


	 virtual void OnActorDisabled();
	 virtual void OnActorEnabled();

	 UGenericCommIF* pCommIF;

	 UFUNCTION(BlueprintCallable)
		 void AddManuallyAttach(USceneComponent* p_comp);
	 static  void HandleAnnotation(AActor* p_actor, bool is_enabled, int annotation_id);

	 virtual FVector UpdateActorVelocityMetersPerSec();
	 virtual FVector UpdateActorAngularVelocityRPYDegPerSec();
public:	
	// Called every frame
	
	virtual void Tick(float DeltaTime) override;
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



	int GetAISClassType();
	void SetAISClassType(int val);

	float GetAISMessagePublishPeriodSec();
	void SetAISMessagePublishPeriodSec(float val);

	bool GetShoudPublishATON(); 
	void SetShoudPublishATON(bool val);

	FVector GetPositionLatLongHeightMSL();
	FVector GetPositionXYZMeters();
	FVector GetActorVelocityMetersPerSec();
	FVector GetActorAngularVelocityRPYDegPerSec();

	/// <summary>
	/// save load
	/// </summary>
public:
	FString GetBlueprintName();
	virtual void Save(ISaveLoader* p_save_loader);
	virtual void SaveJSON(CJsonDataContainer& data);

};
