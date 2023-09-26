// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Lib/Origin/MapOrigin.h>
#include <Lib/SOA/SOAImplementor.h>
#include <Lib/Config/ConfigManager.h>
#include <Lib/UI/UIControllerBase.h>
#include <Lib/ActorBase/ActorBase.h>
#include <Lib/DataContainer/DataContainer.h>
#include "SystemManagerBase.generated.h"

enum ESystemState
{
	SystemStateJustLaunched,
	SystemStateDetectInstance,
	SystemStateWaitConfigLoad,
	SystemStateLoadingConfig,
	SystemStateConfigLoaded,
	SystemStateWaitingRun,
	SystemStateRunSimulation,
	SystemStateRunning,
	SystemStatePauseSimulation,
	SystemStatePaused,
	SystemStateResumeSimulation,
	SystemStateResumed,
	SystemStateConfigLoadError,

};


UCLASS()
class  ASystemManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASystemManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DetectInstance();

	UPROPERTY(EditAnywhere)
		AMapOrigin* pMapOrigin;

	UPROPERTY(EditAnywhere)
		AActor* pFloor;

	UPROPERTY(EditAnywhere)
		ADataContainer* pDataContainer;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> MoveableActorList;

	UPROPERTY(EditAnywhere)
		ASOAImplementor* pSoaImplementor;

	UPROPERTY(EditAnywhere)
		AActor* pConfigManagerActor;

	UPROPERTY(EditAnywhere)
		AUIControllerBase* pUIController;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> SensorGlobalIgnoreList;

	UConfigManager* pConfigManager;
	virtual bool LoadConfig();


	virtual void StateMachine();

	ESystemState SystemState = ESystemState::SystemStateJustLaunched;
	bool CanLoadConfig = false;

	INT32S InstanceNo;

	virtual void HandleSimulationStart();
	virtual void HandleSimulationPause();
	virtual void HandleSimulationResume();


	bool RemoveActor(AActor*p_actor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AMapOrigin* GetMapOrigin();
	ASOAImplementor* GetSOAImplementor();
	AUIControllerBase* GetUIController();
	ADataContainer* GetDataContainer();
	static  ASystemManagerBase* GetInstance();

	TArray<AActor*>& GetSensorGlobalIgnoreList();
	AActor* GetFloor();
	TArray<AActor*>& GetMoveableActorList();

	void RegisterActor(AActor* p_actor);
	TArray<AActor*> GetRegisteredActors();
	TArray<AActor*> GetAllActorInWorld();
	
	AActor* FindActor(FString actor_name);



	bool DestroyActor(FString name);

	AActorBase* ToActorBase(AActor* p_actor);
	void EnableAllActors();
	void DisableAllActors();

	AActor* CreateActor(FString model_name, FString boat_name,FVector world_pos, FVector world_rot, FVector scale);
	INT64U GetTimeStamp();
	AActor* GetVisibleActorAt(const TArray<AActor*>& ignore_list, FVector from, FVector to, FLOAT64 tolerance_meter);



	void SetCanLoadConfig(bool val);
	bool GetCanLoadConfig();

	INT32S GetInstanceNo();
	void ForceExit();

	virtual  bool  SetMainPlayerController(FString name);
	virtual AActor* GetMainPlayerController();

	virtual void StartSimulation();
	virtual void ResumeSimulation();
	virtual void PauseSimulation();
private:

	static ASystemManagerBase* pInstance;
	TMap<FString,  AActor*> AllActors;
	TArray<AActor*> ActorList;
	bool IsStartReceived = false;
	bool IsResumeReceived = false;
	bool IsPauseReceived = false;

	
};
