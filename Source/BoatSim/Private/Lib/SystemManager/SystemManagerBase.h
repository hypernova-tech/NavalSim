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
#include "CBoatBase.h"
#include <Lib/Console/ConsoleBase.h>
#include <Lib/SaveLoad/SaverLoaderBase.h>
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
class  ASystemManagerBase : public AActor, public ISystemAPI
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
		AActor* pConsoleActor;


		UConsoleBase* pConsole = nullptr;
		

	UPROPERTY(EditAnywhere)
		AUIControllerBase* pUIController;



	UPROPERTY(EditAnywhere)
		TArray<AActor*> SensorGlobalIgnoreList;

	UConfigManager* pConfigManager;
	virtual bool LoadConfig();

	virtual void UpdateActors(float deltatime);
	virtual void StateMachine(float deltatime);

	ESystemState SystemState = ESystemState::SystemStateJustLaunched;
	bool CanLoadConfig = false;

	INT32S InstanceNo = -1;
	INT32S TotalInstanceCount = 1;

	virtual void HandleSimulationStart();
	virtual void HandleSimulationPause();
	virtual void HandleSimulationResume();
	void ComputeFPS(float DeltaTime);
	void StartStartupConnections();
	void StartRuntimeConnections();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	AMapOrigin* GetMapOrigin();

	UFUNCTION(BlueprintCallable)
	ASOAImplementor* GetSOAImplementor();

	UFUNCTION(BlueprintCallable)
	AUIControllerBase* GetUIController();

	UFUNCTION(BlueprintCallable)
	ADataContainer* GetDataContainer();





	static  ASystemManagerBase* GetInstance();

	
private:

	static ASystemManagerBase* pInstance;
	TMap<FString,  AActor*> AllActors;
	TArray<AActor*> ActorList;
	TArray<ASensorBase*> Sensors;
	TMap<ESensorType, TArray<ASensorBase*> > SensorsOfType;
	bool IsStartReceived = false;
	bool IsResumeReceived = false;
	bool IsPauseReceived = false;
	APlatformBase* pPlatform;
	UUdpConnection* pConsoleConnection;

	TArray< UConnectionBase*> Connections;
	AActor* pSelectedActor;
	USaverLoaderBase* pSaverLoader;

	/// <summary>
	///  system API
	/// </summary>
public:
	virtual ISystemAPI* GetSystemAPI();

	UFUNCTION(BlueprintCallable)
	virtual	UConsoleBase* GetConsole();

	virtual TArray<AActor*>& GetSensorGlobalIgnoreList();
	virtual AActor* GetFloor();
	virtual TArray<AActor*>& GetMoveableActorList();

	virtual AActor* CreateActor(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale);
	virtual void RegisterActor(AActor* p_actor);
	virtual bool RemoveActor(AActor* p_actor);
	virtual TArray<AActor*> GetRegisteredActors();
	virtual TArray<AActor*> GetAllActorInWorld();
	virtual AActor* FindActor(FString actor_name);
	virtual bool DestroyActor(FString name);
	virtual AActorBase* ToActorBase(AActor* p_actor);
	virtual ASensorBase* ToSensorBase(AActor* p_actor);
	virtual void EnableAllActors();
	virtual void DisableAllActors();
	virtual bool SetActorEnabled(AActor* p_actor, bool val);
	virtual bool GetActorEnabled(AActor* p_actor);
	virtual void SetSelectedActor(AActor* p_actor) ;
	virtual AActor* GetSelectedActor();
	virtual INT64U GetTimeStamp();
	virtual AActor* GetVisibleActorAt(const TArray<AActor*>& ignore_list, FVector from, FVector to, FLOAT64 tolerance_meter);



	virtual void SetCanLoadConfig(bool val);
	virtual bool GetCanLoadConfig();

	virtual INT32S GetInstanceNo();
	virtual void ForceExit();

	virtual  bool  SetMainPlayerController(FString name);
	virtual AActor* GetMainPlayerController();

	UFUNCTION(BlueprintCallable)
	virtual ACBoatBase* GetPlatform();

	UFUNCTION(BlueprintCallable)
	virtual void SetPlatform(ACBoatBase* p_platform);


	virtual void StartSimulation();
	virtual void ResumeSimulation();
	virtual void PauseSimulation();


	virtual bool SetActorInstanceNo(AActor *p_actor, INT32S instance_no);
	virtual int GetActorInstanceNo(AActor* p_actor);

	virtual void SetConsoleConnection(void* p_connection);
	virtual void SendConsoleResponse(const FString& str);
	virtual ESensorType StringToSensor(const FString& str);
	virtual FString SensorToString(ESensorType sensor_type);
	virtual TArray<ESensorType> GetAllSensorTypes();
	virtual TArray<ASensorBase*> GetAllSensors() ;
	virtual TArray<ASensorBase*> GetSensorsOfType(ESensorType sensor_type);

	virtual void RegisterConnection(UConnectionBase* p_connection);

	virtual INT32U ConvertToInstancedLocalPort(INT32 port_no);

	virtual bool GetSensorScanStepAngleDeg(AActor* p_actor, FVector2D& ret);
	virtual bool SetSensorScanStepAngleDeg(AActor* p_actor, FVector2D ang);

	virtual bool Save(FString fname);
	virtual bool Load(FString fname);
};
