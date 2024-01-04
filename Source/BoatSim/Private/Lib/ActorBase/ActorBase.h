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
	
public:	
	// Sets default values for this actor's properties
	AActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(EditAnywhere)
		bool IsSaveEnabled = true;

	UPROPERTY(EditAnywhere)
		bool IsSaveLoadCreateEnabled = true;

	UPROPERTY(EditAnywhere)
		FString BlueprintName = "";


	UPROPERTY(EditAnywhere)
		int ActorId;

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
		AActor* ActorGizmo;


protected:

	 virtual void OnStep(float DeltaTime);
	 virtual void OnStepScenarioMode(float DeltaTime);
	 bool CheckAffinity();

	 virtual void OnActorDisabled();
	 virtual void OnActorEnabled();

	 UGenericCommIF* pCommIF;



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
	FString GetBlueprintName();
	virtual void Save(ISaveLoader* p_save_loader);
	virtual void SaveJSON(CJsonDataContainer &data);
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
	
};
