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




void ASystemManagerBase::RegisterActor(AActor* p_actor)
{
	AllActors.FindOrAdd(p_actor->GetName(),p_actor);
	ActorList.Add(p_actor);
}

TArray<AActor*> ASystemManagerBase::GetRegisteredActors()
{
	return ActorList;
}
TArray<AActor*> ASystemManagerBase::GetAllActorInWorld()
{
	TArray<AActor*> ret;

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		ret.Add(*It);
	}

	return ret;
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
			ret->SetEnabled(false);
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

INT32S ASystemManagerBase::GetInstanceNo()
{
	return InstanceNo;
}

void ASystemManagerBase::ForceExit()
{
	FGenericPlatformMisc::RequestExit(true);
}




bool ASystemManagerBase::DestroyActor(FString name)
{
	auto p_actor = FindActor(name);
	if (p_actor) {
		p_actor->Destroy();
		RemoveActor(p_actor);
		return true;
	}
	else {
		return false;
	}
}


AActor* ASystemManagerBase::FindActor(FString actor_name)
{

	auto p_actor = AllActors.Find(actor_name);

	if (p_actor == nullptr) {
		FString ActorNameToFind = actor_name;
		for (TActorIterator<AActor> It(GetWorld()); It; ++It)
		{
			if (It->GetName() == ActorNameToFind)
			{
				AActor* FoundActor = *It;
				// Do something with FoundActor
				return FoundActor;
			}
		}
	}
	else {
		return *p_actor;
	}

	return nullptr;
		
	
	
}


bool ASystemManagerBase::RemoveActor(AActor *p_actor)
{
	AllActors.Remove(p_actor->GetName());
	ActorList.Remove(p_actor);
	return true;

}

void ASystemManagerBase::DetectInstance()
{
	FString Value;
	InstanceNo = -1;
	if (FParse::Value(FCommandLine::Get(), TEXT("instance="), Value))
	{
		// Do something with Value
		UE_LOG(LogTemp, Warning, TEXT("MyOption value: %s"), *Value);
		CUtil::DebugLogScreen("Instance: " + Value, 10);
		InstanceNo = CUtil::StringToInt(Value);

	}
	else {
		CUtil::DebugLogScreen("Instance: " + FString("null"), 60);
	}
}
// Called when the game starts or when spawned
void ASystemManagerBase::BeginPlay()
{
	Super::BeginPlay();

	if (pConfigManagerActor != nullptr) {
		pConfigManager = pConfigManagerActor->GetComponentByClass<UConfigManager>();
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		APlatformBase* p_platform = (APlatformBase* )(PlayerController->GetPawn());
		if (p_platform) {
			SetPlatform(p_platform);
		}
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
UConsoleBase* ASystemManagerBase::GetConsole()
{
	if (pConsole == nullptr) {
		
		pConsole = pConsoleActor->FindComponentByClass<UConsoleBase>();
	}
	return pConsole;
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
		RegisterActor(ret);

	
		
		
	}

	return ret;
}

bool ASystemManagerBase::SetMainPlayerController(FString name)
{

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && PlayerController->GetPawn())
	{
		PlayerController->UnPossess();
		auto pactor = FindActor(name);
		if (pactor->IsA<APawn>()) {
			PlayerController->Possess((APawn*)pactor);

			
			
			PlayerController->SetViewTargetWithBlend(pactor, 0.1f);
			APlatformBase* p_platform = (APlatformBase*)(PlayerController->GetPawn());
			if (p_platform) {
				SetPlatform(p_platform);
			}
			return true;
		}
		
	}


	return false;

}

AActor* ASystemManagerBase::GetMainPlayerController()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	return PlayerController->GetPawn();
}
ACBoatBase* ASystemManagerBase::GetPlatform()
{
	return pPlatform;
}
void ASystemManagerBase::SetPlatform(ACBoatBase* p_platform)
{
	pPlatform = p_platform;
	pPlatform->OnControllerChanged();

}
void ASystemManagerBase::StartSimulation()
{
	IsStartReceived = true;
}
void ASystemManagerBase::PauseSimulation()
{
	IsPauseReceived = true;
}
void ASystemManagerBase::ResumeSimulation()
{
	IsResumeReceived = true;
}

void ASystemManagerBase::HandleSimulationStart()
{
	ASystemManagerBase::GetInstance()->EnableAllActors();
}
void ASystemManagerBase::HandleSimulationPause()
{
	ASystemManagerBase::GetInstance()->DisableAllActors();
}
void ASystemManagerBase::HandleSimulationResume()
{
	ASystemManagerBase::GetInstance()->EnableAllActors();
}
void ASystemManagerBase::StateMachine()
{

	auto curr_state = SystemState;
	auto next_state = curr_state;

	switch (curr_state)
	{
	case ESystemState::SystemStateJustLaunched:

		next_state = ESystemState::SystemStateDetectInstance;
		break;
	case ESystemState::SystemStateDetectInstance:
		DetectInstance();
		next_state = ESystemState::SystemStateWaitConfigLoad;
		break;
	case ESystemState::SystemStateWaitConfigLoad:
		if (CanLoadConfig || true) {
			next_state = ESystemState::SystemStateLoadingConfig;

		}
		break;
	case ESystemState::SystemStateLoadingConfig:
		LoadConfig();
		next_state = ESystemState::SystemStateConfigLoaded;
		break;
	case ESystemState::SystemStateConfigLoaded:
		next_state = ESystemState::SystemStateWaitingRun;
		break;
	case ESystemState::SystemStateWaitingRun:
		if (IsStartReceived) {
			next_state = ESystemState::SystemStateRunSimulation;
			IsStartReceived = false;
		}
		break;
	case ESystemState::SystemStateRunSimulation:
		HandleSimulationStart();
		next_state = ESystemState::SystemStateRunning;
		break;
	case ESystemState::SystemStateRunning:
		if (IsPauseReceived) {
			next_state = ESystemState::SystemStatePauseSimulation;
			IsPauseReceived = false;
		}
		break;

	case ESystemState::SystemStatePauseSimulation:
		HandleSimulationPause();
		next_state = ESystemState::SystemStatePaused;
		break;
	case ESystemState::SystemStatePaused:
		if (IsResumeReceived) {
			next_state = ESystemState::SystemStateResumeSimulation;
			IsResumeReceived = false;
		}
		break;
	case ESystemState::SystemStateResumeSimulation:
		HandleSimulationResume();
		next_state = ESystemState::SystemStateResumed;

		break;
	case ESystemState::SystemStateResumed:
		next_state = ESystemState::SystemStateRunning;
		break;
	case ESystemState::SystemStateConfigLoadError:
		break;
	default:
		break;
	}


	SystemState = next_state;
}
