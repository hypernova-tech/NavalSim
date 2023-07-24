// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Lib/Origin/MapOrigin.h>
#include <Lib/SOA/SOAImplementor.h>
#include <Lib/Config/ConfigManager.h>
#include <Lib/UI/UIControllerBase.h>
#include <Lib/ActorBase/ActorBase.h>
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
		ASOAImplementor* pSoaImplementor;

	UPROPERTY(EditAnywhere)
		AActor* pConfigManagerActor;

	UPROPERTY(EditAnywhere)
		AUIControllerBase* pUIController;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> SensorGlobalIgnoreList;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> MoveableActorList;

	UConfigManager* pConfigManager;
	virtual void LoadConfig();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AMapOrigin* GetMapOrigin();
	ASOAImplementor* GetSOAImplementor();
	AUIControllerBase* GetUIController();
	static  ASystemManagerBase* GetInstance();

	TArray<AActor*>& GetSensorGlobalIgnoreList();
	TArray<AActor*>& GetMoveableActorList();

	void RegisterActor(FString owner, AActorBase* p_actor);

	template <typename T>
	T* FindActor(FString owner, FString actor_name);

private:

	static ASystemManagerBase* pInstance;
	TMap<FString, TMap<FString, AActorBase*>> AllActors;
	
};
