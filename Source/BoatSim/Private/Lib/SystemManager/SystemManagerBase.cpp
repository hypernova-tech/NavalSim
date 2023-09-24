// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SystemManager/SystemManagerBase.h"
#include <Lib/Utils/CUtil.h>
#include <CBoatBase.h>

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

AActor* ASystemManagerBase::GetFloor()
{

	return pFloor;
}


TArray<AActor*>& ASystemManagerBase::GetMoveableActorList()
{
	return MoveableActorList;
}




void ASystemManagerBase::RegisterActor(FString owner, AActor* p_actor)
{
	TMap<FString, AActor*>& all_actors = AllActors.FindOrAdd(owner);
	FString actor_label =  p_actor->GetName(); 
	all_actors.Add(actor_label, p_actor);
	ActorList.Add(p_actor);
}

AActorBase* ASystemManagerBase::ToActorBase(AActor* p_actor)
{
	if (p_actor->IsA<AActorBase>()) {
		return (AActorBase*)p_actor;
	}

	return nullptr;
}

void ASystemManagerBase::EnableAllActors()
{

	for (AActor* p_actor : ActorList) {
		auto ret = ToActorBase(p_actor);

		if (ret) {
			ret->SetEnabled(true);
		}
	}
	
	
}
void ASystemManagerBase::DisableAllActors()
{

	for (AActor* p_actor : ActorList) {
		auto ret = ToActorBase(p_actor);

		if (ret) {
			ret->SetEnabled(true);
		}
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

void ASystemManagerBase::SetCanLoadConfig(bool val)
{
	CanLoadConfig = val;
}

bool ASystemManagerBase::GetCanLoadConfig()
{
	return CanLoadConfig;
}

AActor* ASystemManagerBase::FindActor(TArray<FString> relative_name) 
{
	FString owner = "world";
	FString actor_name = relative_name[0];

	if (relative_name.Num() > 1) {
		owner = relative_name[0];
		actor_name = relative_name[1];
		
	}

	TMap<FString, AActor*>* p_actors = AllActors.Find(owner);

	if (p_actors != nullptr) {
		AActor** p_actor = p_actors->Find(actor_name);
		if (p_actor != nullptr) {
			if (p_actor[0] != nullptr) {
				
				return p_actor[0];
			}

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

	FString Value;
	if (FParse::Value(FCommandLine::Get(), TEXT("instance="), Value))
	{
		// Do something with Value
		UE_LOG(LogTemp, Warning, TEXT("MyOption value: %s"), *Value);
		CUtil::DebugLogScreen("Instance: " + Value,60);
	}
	else {
		CUtil::DebugLogScreen("Instance: " + FString("null"), 60);
	}
	
	GEngine->GameViewport->ConsoleCommand(TEXT("YourCommand"));
	
}


// Called every frame
void ASystemManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StateMachine();
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


bool ASystemManagerBase::LoadConfig()
{
#if false
	ACBoatBase* p_boat_base;

	if (pConfigManager != nullptr) {
		pConfigManager->ParseXML();

		TArray<USceneElementDescBase*>& scene_descs = pConfigManager->GetSceneElementDescs();

		for (auto p_scene_element_desc : scene_descs) {
			if (pConfigManager->IsPlatform(p_scene_element_desc->Owner)) {
				p_boat_base = NewObject< ACBoatBase>();
				p_boat_base->SetActorLocation(p_scene_element_desc->InstanceSubobjectTemplates);

			}
			auto pltform_desc = pConfigManager->GetPlatformDesc(p_scene_element_desc->Owner);
			for (auto param : pltform_desc->GetParams()) {
				
			}
		}

		return true;
	}
	else {

		return false;
	}
#endif

	return false;



}


AActor* ASystemManagerBase::CreateActor(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale)
{
	auto info = pDataContainer->FindBlueprintInfo(model_name);
	
	auto path = info.BlueprintAsset.ToSoftObjectPath().GetAssetPathString();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, path);
	CUtil::DebugLog("blueprint path: " + path);




    auto ret = CUtil::SpawnObjectFromBlueprint(info.ActorToSpawn, path, GetWorld(), nullptr, boat_name,world_pos, world_rot, scale);
	
	if (ret != nullptr) {
		RegisterActor("world", ret);

		/*
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController && PlayerController->GetPawn())
		{
			PlayerController->UnPossess();
			PlayerController->Possess((APawn*)ret);
		}
		
		*/
		
		
	}

	return ret;
}

void ASystemManagerBase::StateMachine()
{

	auto curr_state = SystemState;
	auto next_state = curr_state;

	switch (curr_state)
	{
	case SystemStateWaitConfigLoad:
		if (CanLoadConfig || true) {
			next_state = SystemStateLoadingConfig;

		}
		break;
	case SystemStateLoadingConfig:
		LoadConfig();
		next_state = SystemStateConfigLoaded;
		break;
	case SystemStateConfigLoaded:
		break;
	case SystemStateWaitingRun:
		break;
	case SystemStateRunning:
		break;
	case SystemStatePaused:
		break;
	case SystemStateResumed:
		break;
	case SystemStateConfigLoadError:
		break;
	default:
		break;
	}


	SystemState = next_state;
}
