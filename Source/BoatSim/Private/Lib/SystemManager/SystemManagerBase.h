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

	

	UPROPERTY(EditAnywhere)
		AMapOrigin* pMapOrigin;

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
	virtual void LoadConfig();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AMapOrigin* GetMapOrigin();
	ASOAImplementor* GetSOAImplementor();
	AUIControllerBase* GetUIController();
	ADataContainer* GetDataContainer();
	static  ASystemManagerBase* GetInstance();

	TArray<AActor*>& GetSensorGlobalIgnoreList();
	TArray<AActor*>& GetMoveableActorList();

	void RegisterActor(FString owner, AActorBase* p_actor);

	template <typename T>
	T* FindActor(FString owner, FString actor_name);



	void EnableAllActors();
	void DisableAllActors();

	bool AddBoat(FString model_name, FString boat_name,FVector world_pos, FVector world_rot, FVector scale);
	INT64U GetTimeStamp();
	AActor* GetVisibleActorAt(const TArray<AActor*>& ignore_list, FVector from, FVector to, FLOAT64 tolerance_meter);
private:

	static ASystemManagerBase* pInstance;
	TMap<FString, TMap<FString, AActorBase*>> AllActors;
	TArray<AActorBase*> ActorList;


	
};
