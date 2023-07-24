// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SystemManager/SystemManagerBase.h"

// Sets default values
ASystemManagerBase* ASystemManagerBase::pInstance = nullptr;

ASystemManagerBase::ASystemManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pInstance = this;


	

}

ASystemManagerBase* ASystemManagerBase::GetInstance()
{
	return pInstance;
}

TArray<AActor*>& ASystemManagerBase::GetSensorGlobalIgnoreList()
{
	return SensorGlobalIgnoreList;
}

TArray<AActor*>& ASystemManagerBase::GetMoveableActorList()
{
	return MoveableActorList;
}




void ASystemManagerBase::RegisterActor(FString owner, AActorBase* p_actor)
{
	TMap<FString, AActorBase*>& all_actors = AllActors.FindOrAdd(owner);
	FString actor_label = p_actor->GetActorLabel();
	all_actors.Add(actor_label, p_actor);
}

template <typename T>
T* ASystemManagerBase::FindActor(FString owner, FString actor_name)
{

	TMap<FString, AActorBase*>* p_actors = AllActors.Find(owner);
		
	if (p_actors != nullptr) {
		AActorBase** p_actor = p_actors->Find(actor_name);
		if (p_actor != nullptr) {
			if (p_actor[0] != nullptr) {
				T* p_res = Cast<T>(p_actor[0]);
				return p_res;
			}
			
		}
		
	}
	
	return nullptr;
	
}


// Called when the game starts or when spawned
void ASystemManagerBase::BeginPlay()
{
	Super::BeginPlay();

	if (pConfigManagerActor != nullptr) {
		pConfigManager = pConfigManagerActor->GetComponentByClass<UConfigManager>();
	}

	if (pConfigManager != nullptr) {
		pConfigManager->ParseXML();
		LoadConfig();
	}
	
	GEngine->GameViewport->ConsoleCommand(TEXT("YourCommand"));
	
}


// Called every frame
void ASystemManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMapOrigin* ASystemManagerBase::GetMapOrigin()
{
	return pMapOrigin;
}

ASOAImplementor* ASystemManagerBase::GetSOAImplementor()
{
	return pSoaImplementor;
}

AUIControllerBase* ASystemManagerBase::GetUIController()
{
	return pUIController;
}


void ASystemManagerBase::LoadConfig()
{
	TArray< UDescBase*>& all_desc = pConfigManager->GetAllDesc();

	for(UDescBase *p_desc :all_desc) {

	}

}