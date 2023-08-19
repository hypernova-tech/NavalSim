// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SystemManager/SystemManagerBase.h"
#include <Lib/Utils/CUtil.h>

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
	ActorList.Add(p_actor);
}

void ASystemManagerBase::EnableAllActors()
{

	for (AActorBase* p_actor : ActorList) {
		p_actor->SetEnabled(true);
	}
	
	
}
void ASystemManagerBase::DisableAllActors()
{

	for (AActorBase* p_actor : ActorList) {
		p_actor->SetEnabled(false);
	}


}

INT64U ASystemManagerBase::GetTimeStamp()
{
	FLOAT64 time = FApp::GetGameTime();
	INT64U microsec = (time * 1e6);

	return microsec;
}

AActor* ASystemManagerBase::GetVisibleActorAt(const TArray<AActor *> & ignore_list, FVector from, FVector to, FLOAT64 dist_meter)
{
	FCollisionQueryParams query_params;
	for (auto pactor : ignore_list) {
		query_params.AddIgnoredActor(pactor);
	}
	FHitResult result;

#if false
	for (AActor* p_temp : *args.p_ignore_list) {
		query_params.AddIgnoredActor(p_temp);
	}
#endif
	query_params.bTraceComplex = false;

	bool ret = GetWorld()->LineTraceSingleByChannel(result, from, to, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());
	if (ret) {
		FLOAT64 object_dist_meter = TOW((result.Location - to).Length());
		
		if ((to - result.Location).Length() <= TOUE(dist_meter)) {
			auto hit =  result.GetActor();
			return hit;
		}
	}


	return nullptr;

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

ADataContainer* ASystemManagerBase::GetDataContainer()
{
	return pDataContainer;
}


void ASystemManagerBase::LoadConfig()
{
	TArray< UDescBase*>& all_desc = pConfigManager->GetAllDesc();

	for(UDescBase *p_desc :all_desc) {

	}

}

bool ASystemManagerBase::AddBoat(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale)
{
	auto info = pDataContainer->FindBlueprintInfo(model_name);
	
	CUtil::SpawnObjectFromBlueprint(info.BlueprintAsset.ToSoftObjectPath().GetAssetPathString(), GetWorld(), nullptr, boat_name,world_pos, world_rot, scale);
	return false;
}

