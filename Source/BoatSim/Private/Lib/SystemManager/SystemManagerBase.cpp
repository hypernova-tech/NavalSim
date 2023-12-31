// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SystemManager/SystemManagerBase.h"
#include <Lib/Utils/CUtil.h>
#include <CBoatBase.h>
#include <Lib/Sensor/GenericSensor/LidarBase.h>
#include <Lib/PathController/PathController.h>
#include <Lib/Math/CMath.h>
#include "EngineUtils.h"



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

void ASystemManagerBase::OnActorMoved(AActor* p_actor)
{
	auto p_wp = To<AWaypointActor>(p_actor);

	if (p_wp) {
		auto *p_path_actor = p_wp->GetOwnerPath();
		if (p_path_actor) {
			auto* p_path = ToPath(p_path_actor);
			p_path->NotifyWaypointMoved(p_wp);
		}
		
	}
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
	if (AllActors.Find(p_actor->GetName())) {
		return;
	}

	AllActors.FindOrAdd(p_actor->GetName(),p_actor);
	ActorList.Add(p_actor);
	if (p_actor->IsA<ASensorBase>()) {
		auto p_sensor = (ASensorBase*)p_actor;
		Sensors.Add((ASensorBase*)p_actor);

		auto ret = SensorsOfType.Find(p_sensor->SensorType);
		if (ret){
			ret->Add(p_sensor);
		}else{
			TArray<ASensorBase*> new_data;
			new_data.Add(p_sensor);
			SensorsOfType.Add(p_sensor->SensorType, new_data);

		}

	}
	APlatformBase* p_platform = Cast<APlatformBase>(p_actor);

	if (p_platform) {
		
		Platforms.Add(p_platform);
		//if (Platforms.Num() == 1) {
			SetMainPlayerController(p_actor->GetName());
		//}
	}
	


}

bool ASystemManagerBase::RemoveActor(AActor* p_actor)
{
	AllActors.Remove(p_actor->GetName());
	ActorList.Remove(p_actor);
	if (p_actor->IsA<ASensorBase>()) {
		auto* p_sensor = ToSensorBase(p_actor);
		Sensors.Remove(p_sensor);
		auto ret = SensorsOfType.Find(p_sensor->SensorType);

		if (ret != nullptr) {
			ret->Remove(p_sensor);
		}
		
	}
	return true;

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

void ASystemManagerBase::QueryActors(EActorQueryArgs args, TArray<AActor*>& actors)
{
	if (args == EActorQueryArgs::OnlyAgents) {
		for (auto pactor : ActorList) {
			if (ToAgent(pactor)) {
				
				actors.Add(pactor);
				
			}

		}
	}else if (args == EActorQueryArgs::Gimbals) {
		for (auto pactor : ActorList) {
			if (ToGimbal(pactor)) {

				actors.Add(pactor);

			}

		}
	}
	else if (args == EActorQueryArgs::ActorBasesExceptSensorsAndPaths) {
		 for (auto pactor : ActorList) {
			 if (ToActorBase(pactor)) {
				 if (!ToSensorBase(pactor) && !ToPath(pactor)) {
					 actors.Add(pactor);
				 }
			 }
			
		 }

	 }
	else if (args == EActorQueryArgs::ActorBasesExceptSensorsAndPathsAndGimbals) {
		for (auto pactor : ActorList) {
			if (ToActorBase(pactor)) {
				if (!ToSensorBase(pactor) && !ToGimbal(pactor)) {
					actors.Add(pactor);
				}
			}

		}

	}
	else if (args == EActorQueryArgs::ActorBasesOnlyPaths) {
		 for (auto pactor : ActorList) {
			 if (ToActorBase(pactor)) {
				 if (ToPath(pactor)) {
					 actors.Add(pactor);
				 }
			 }

		 }

	 }
	 else if (args == EActorQueryArgs::ActorBases) {
		 for (auto pactor : ActorList) {
			 if (ToActorBase(pactor)) {
				
				 actors.Add(pactor);
				 
			 }

		 }

	 }
	 else if (args == EActorQueryArgs::Platforms) {
		 for (auto pactor : ActorList) {
			 if (ToPlatform(pactor)) {

				 actors.Add(pactor);

			 }

		 }

	 }

	 
	
}
AAgent* ASystemManagerBase::ToAgent(AActor* p_actor)
{
	if (p_actor->IsA<AAgent>()) {
		return (AAgent*)p_actor;
	}

	return nullptr;
}
AActorBase* ASystemManagerBase::ToActorBase(AActor* p_actor)
{
	if (p_actor->IsA<AActorBase>()) {
		return (AActorBase*)p_actor;
	}

	return nullptr;
}

bool ASystemManagerBase::SetActorEnabled(AActor* p_actor, bool val)
{
	auto p_base = ToActorBase(p_actor);
	if (p_base) {
		p_base->SetEnabled(val);
		return true;
	}

	return false;
}

bool ASystemManagerBase::GetActorEnabled(AActor* p_actor)
{
	auto p_base = ToActorBase(p_actor);
	if (p_base) {
		p_base->GetEnabled();
		return true;
	}

	return false;
}
void ASystemManagerBase::SetSelectedActor(AActor *p_actor)
{
	pSelectedActor = p_actor;
}
AActor* ASystemManagerBase::GetSelectedActor()
{
	return pSelectedActor;
}
bool ASystemManagerBase::IsBakable(AActor* p_actor)
{
	
	return ToActorBase(p_actor) != nullptr;
}
void ASystemManagerBase::SetGizmoTrackedActor(AActor* p_actor)
{
	pUIController->GetGizmoController()->SetTrackedActor(p_actor);
}

bool ASystemManagerBase::IsGizmoActorMoving()
{
	return pUIController->GetGizmoController()->IsGizmoActorMoving();
}


ASensorBase* ASystemManagerBase::ToSensorBase(AActor* p_actor)
{
	if (p_actor->IsA<ASensorBase>()) {
		return (ASensorBase*)p_actor;
	}

	return nullptr;
}
ARadarBase* ASystemManagerBase::ToRadarBase(AActor* p_actor)
{
	if (p_actor->IsA<ARadarBase>()) {
		return (ARadarBase*)p_actor;
	}

	return nullptr;
}

APathController* ASystemManagerBase::ToPath(AActor* p_actor)
{
	if (p_actor->IsA<APathController>()) {
		return (APathController*)p_actor;
	}

	return nullptr;
}
AGimbalBase* ASystemManagerBase::ToGimbal(AActor* p_actor)
{
	if (p_actor->IsA<AGimbalBase>()) {
		return (AGimbalBase*)p_actor;
	}

	return nullptr;
}
APlatformBase* ASystemManagerBase::ToPlatform(AActor* p_actor)
{
	if (p_actor->IsA<APlatformBase>()) {
		return (APlatformBase*)p_actor;
	}

	return nullptr;
}
AWaypointActor* ASystemManagerBase::ToWaypoint(AActor* p_actor)
{
	if (p_actor->IsA<AWaypointActor>()) {
		return (AWaypointActor*)p_actor;
	}

	return nullptr;
}

ATerrainManager* ASystemManagerBase::ToTerrain(AActor* p_actor)
{
	if (p_actor->IsA<ATerrainManager>()) {
		return (ATerrainManager*)p_actor;
	}

	return nullptr;
}


template <typename T>
T* ASystemManagerBase::To(AActor* p_actor)
{
	if (p_actor->IsA<T>()) {
		return (T*)p_actor;
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

bool ASystemManagerBase::SetActorSuppressed(AActor* p_actor, bool val)
{
	auto p_base = ToActorBase(p_actor);
	if (p_base) {
		p_base->SetSuppressed(val);
		return true;
	}

	return false;
}

bool ASystemManagerBase::GetActorSuppressed(AActor* p_actor, bool &val)
{
	auto p_base = ToActorBase(p_actor);
	if (p_base) {
		val = p_base->GetSuppressed();
		return true;
	}

	return false;
}

bool ASystemManagerBase::GetActorStandaloneModeEnabled(AActor* p_actor, bool& val)
{
	auto p_base = ToActorBase(p_actor);
	if (p_base) {
		val = p_base->GetStandaloneModeEnabled();
		return true;
	}

	return false;
}

bool ASystemManagerBase::SetActorStandaloneModeEnabled(AActor* p_actor, bool val)
{
	auto p_base = ToActorBase(p_actor);
	if (p_base) {
		p_base->SetStandaloneModeEnabled(val);
		return true;
	}

	return false;
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
	return DestroyActor(p_actor);
}

bool ASystemManagerBase::DestroyActor(AActor *p_actor)
{

	if (p_actor) {

		auto p_actor_base = ToActorBase(p_actor);
		if (p_actor_base) {
			p_actor_base->OnActorPredestroy();
		}

		TArray<AActor*> AttachedActors;
		p_actor->GetAttachedActors(AttachedActors);

		for (AActor* ChildActor : AttachedActors)
		{
			if (ChildActor)
			{
				ChildActor->Destroy();
				RemoveActor(ChildActor);
			}
		}


		p_actor->Destroy();
		RemoveActor(p_actor);
		return true;
	}
	else {
		return false;
	}
}

UObject* ASystemManagerBase::FindObject(FString obj_name)
{
	return nullptr;
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



USceneComponent* ASystemManagerBase::FindComponent(FString path)
{

	TArray<FString> Parts;
	path.ParseIntoArray(Parts, TEXT("/"), true);

	if (Parts.Num() == 0){

		return nullptr;
	}

	for (int i = Parts.Num()-1; i >= 0; i--) {
		FString name = Parts[i];
		auto p_actor = FindActor(name);

		if (p_actor) {
			FString comp_path = "";
			
			for (int j = i+1 ; j < Parts.Num(); j++) {
				if (comp_path != "") {
					comp_path += "/" + Parts[j];
				}
				else {
					comp_path += Parts[j];
				}
				
			}

			USceneComponent *p_comp = CUtil::FindComponentByPath(p_actor, comp_path);

			return p_comp;
		}
	}

	return nullptr;
}

void ASystemManagerBase::DetectInstance()
{
	FString Value;
	InstanceNo = -1;
	if (FParse::Value(FCommandLine::Get(), TEXT("instance="), Value))
	{
		// Do something with Value
		
		InstanceNo = CUtil::StringToInt(Value);

	}
	else {
		
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("instancestotal="), Value))
	{
		// Do something with Value
		TotalInstanceCount = CUtil::StringToInt(Value);

	}
	else {
		
	}


	CUtil::DebugLogScreen("Instance:" + CUtil::IntToString(InstanceNo) +"/"+ CUtil::IntToString(TotalInstanceCount), 10);

	
}
// Called when the game starts or when spawned
void ASystemManagerBase::BeginPlay()
{
	Super::BeginPlay();
	pSaverLoader = GetComponentByClass<USaverLoaderBase>();
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
		InputMode.SetHideCursorDuringCapture(false);
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
	StateMachine(DeltaTime);
	UpdateLifeTime(DeltaTime);
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

ADataManager* ASystemManagerBase::GetDataManager()
{
	return pDataManager;
}
UConsoleBase* ASystemManagerBase::GetConsole()
{
	if (pConsole == nullptr) {
		
		pConsole = pConsoleActor->FindComponentByClass<UConsoleBase>();
	}
	return pConsole;
}

FString ASystemManagerBase::GenerateUniqueName(FString prefix)
{
	UniqueId++;
	return prefix + CUtil::IntToString(UniqueId);
}

APlayerCameraManager* ASystemManagerBase::GetCameraManager()
{
	APlayerController* player_controller = GetWorld()->GetFirstPlayerController();
	return player_controller->PlayerCameraManager;
}

FLOAT64 ASystemManagerBase::ComputeCameraDistToActorMeter(AActor* p_actor)
{
	return TOW((GetCameraManager()->GetCameraLocation() - p_actor->GetActorLocation()).Length());
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


AActor* ASystemManagerBase::CreateActor(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale, FLOAT64 life_time_sec)
{

	if (CUtil::DoesExist(boat_name, GetWorld())) {
		SendConsoleResponse(boat_name + "already exists, choose a unique name");
		return nullptr;
	}
	auto info = pDataManager->FindBlueprintInfo(model_name);
	
	auto path = info.BlueprintAsset.ToSoftObjectPath().GetAssetPathString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, path);
	CUtil::DebugLog("blueprint path: " + path);




    auto ret = CUtil::SpawnObjectFromBlueprint(nullptr, path, GetWorld(), nullptr, boat_name,world_pos, world_rot, scale);
	
	if (ret != nullptr) {
		RegisterActor(ret);

		if (life_time_sec > 0) {
			ULifeTimeEntryObject* p_entry = NewObject< ULifeTimeEntryObject>();
			p_entry->pActor = ret;
			p_entry->CurrentLifeTimeSec = life_time_sec;
			LifeTimeEntries.Add(p_entry);
		}
	}

	
	return ret;
}

bool ASystemManagerBase::SetMainPlayerController(FString name)
{

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController )
	{
		if (PlayerController->GetPawn()) {
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
		else {
			
			auto pactor = FindActor(name);
			if (pactor->IsA<APawn>()) {
				PlayerController->SetPawn((APawn*)pactor);
				PlayerController->Possess((APawn*)pactor);



				PlayerController->SetViewTargetWithBlend(pactor, 0.1f);
				APlatformBase* p_platform = (APlatformBase*)(PlayerController->GetPawn());
				if (p_platform) {
					SetPlatform(p_platform);
				}
				return true;
			}
		}
		
		
	}


	return false;

}

AActor* ASystemManagerBase::GetMainPlayerController()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	return PlayerController->GetPawn();
}
APlatformBase* ASystemManagerBase::GetPlatform()
{
	return pPlatform;
}


void ASystemManagerBase::SetPlatform(APlatformBase* p_platform)
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

void ASystemManagerBase::StartStartupConnections()
{
	for (auto p_con : Connections) {
		if (p_con->IsActiveAtStartup && !p_con->GetIsRunning()) {
			p_con->InitConnection();
		}
	}
}
void ASystemManagerBase::StartRuntimeConnections()
{
	for (auto p_con : Connections) {
		if (!p_con->IsActiveAtStartup && !p_con->GetIsRunning()) {
			p_con->InitConnection();
		}
	}
}

void ASystemManagerBase::UpdateActorsScenarioMode(float deltatime)
{
	for (auto* p_actor : ActorList) {
		if (p_actor->IsA<AActorBase>()) {
			AActorBase* p_base = (AActorBase*)(p_actor);
			if(p_base->GetIsExternalUpdateScenarioMode())
			p_base->ExternalUpdateScenarioMode(deltatime);
		}
	}
}

void ASystemManagerBase::UpdateActors(float deltatime)
{
	for (auto* p_actor : ActorList) {
		if (p_actor->IsA<AActorBase>()) {
			AActorBase* p_base = (AActorBase*) (p_actor);
			p_base->ExternalUpdate(deltatime);
		}
	}
}
void ASystemManagerBase::StateMachine(float deltatime)
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
		StartStartupConnections();
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
			StartRuntimeConnections();
		}
		else {
			UpdateActorsScenarioMode(deltatime);
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
		UpdateActors(deltatime);
		break;

	case ESystemState::SystemStatePauseSimulation:

		HandleSimulationPause();
		UpdateActors(deltatime);
		next_state = ESystemState::SystemStatePaused;
		break;
	case ESystemState::SystemStatePaused:
		if (IsResumeReceived) {
			next_state = ESystemState::SystemStateResumeSimulation;
			IsResumeReceived = false;
		}
		else {
			UpdateActorsScenarioMode(deltatime);
		}
		break;
	case ESystemState::SystemStateResumeSimulation:
		HandleSimulationResume();
		UpdateActors(deltatime);
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

void ASystemManagerBase::UpdateLifeTime(float deltatime)
{
	for (int i = 0; i < LifeTimeEntries.Num(); i++) {
		auto entry = LifeTimeEntries[i];
		entry->CurrentLifeTimeSec -= deltatime;
		if (entry->CurrentLifeTimeSec <=0) {
			CUtil::DebugLog("Actor is being destroyed " + entry->pActor->GetName());
			DestroyActor(entry->pActor);
			LifeTimeEntries.RemoveAt(i);
			entry->MarkPendingKill();
			
			i--;
		}
	}
}

////
////
/// System API

ISystemAPI* ASystemManagerBase::GetSystemAPI()
{
	return this;
}

bool ASystemManagerBase::SetActorInstanceNo(AActor* p_actor, INT32S instance_no)
{
	auto* p_base = ToActorBase(p_actor);
	if (p_base) {
		p_base->SetAffinityInstanceId(instance_no);
		return true;
	}

	return false;
}

int ASystemManagerBase::GetActorInstanceNo(AActor* p_actor)
{
	auto* p_base = ToActorBase(p_actor);
	if (p_base) {
		return p_base->GetAffinityInstanceId();
	}

	return -1;
}

void ASystemManagerBase::SetConsoleConnection(void* p_connection)
{
	pConsoleConnection = (UUdpConnection*)p_connection;
}
void ASystemManagerBase::SendConsoleResponse(const FString& str)
{
	if (pConsoleConnection != nullptr) {
		pConsoleConnection->SendUDPData(str);
	}
}

ESensorType ASystemManagerBase::StringToSensor(const FString& str)
{
	if (str == "lidar") {
		return ESensorType::Lidar;
	}
	if (str == "radar") {
		return ESensorType::Radar;
	}
	if (str == "fls3d") {
		return ESensorType::FLS3D;
	}
	if (str == "camerair") {
		return ESensorType::CameraIR;
	}
	if (str == "cameravisible") {
		return ESensorType::CameraVisible;
	}
	if (str == "ais") {
		return ESensorType::AIS;
	}

	return ESensorType::Unknown;
}

FString ASystemManagerBase::SensorToString(ESensorType sensor_type)
{
	if (sensor_type == ESensorType::Lidar) {
		return "lidar" ;
	}
	if (sensor_type == ESensorType::Radar ) {
		return "radar";
	}
	if (sensor_type == ESensorType::FLS3D ) {
		return "fls3d";
	}
	if (sensor_type == ESensorType::CameraIR ) {
		return "camerair";
	}
	if (sensor_type == ESensorType::CameraVisible ) {
		return "cameravisible";
	}
	if (sensor_type == ESensorType::AIS ) {
		return "ais";
	}	
	
	if (sensor_type == ESensorType::Unknown) {
		return "unknown";
	}

	return "";

}

TArray<ESensorType> ASystemManagerBase::GetAllSensorTypes()
{
	TArray<ESensorType> ret;
	ret.Add(ESensorType::Lidar);
	ret.Add(ESensorType::Radar);
	ret.Add(ESensorType::FLS3D);
	ret.Add(ESensorType::CameraIR);
	ret.Add(ESensorType::CameraVisible);
	ret.Add(ESensorType::AIS);

	return ret;
}

TArray<ASensorBase*> ASystemManagerBase::GetAllSensors()
{
	return Sensors;
}

void ASystemManagerBase::RegisterConnection(UConnectionBase* p_connection)
{
	Connections.Add(p_connection);
}

INT32U ASystemManagerBase::ConvertToInstancedLocalPort(INT32 port_no)
{
	if (InstanceNo < 0) {
		return port_no;
	}
	else {
		return port_no + InstanceNo;
	}
}



bool ASystemManagerBase::GetSensorScanStepAngleDeg(AActor* p_actor, FVector2D& ret)
{
	auto p_sensor = ToSensorBase(p_actor);
	if (p_sensor) {
		ret.X = p_sensor->GetHorizontalScanStepAngleDeg();
		ret.Y = p_sensor->GetVerticalScanStepAngleDeg();
		return true;
	}
	else {

		return false;
	}
}

bool ASystemManagerBase::SetSensorScanStepAngleDeg(AActor* p_actor, FVector2D ang)
{
	auto p_sensor = ToSensorBase(p_actor);
	if (p_sensor) {
		p_sensor->SetHorizontalScanStepAngleDeg(ang.X);
		p_sensor->SetVerticalScanStepAngleDeg(ang.Y);
		return true;
	}
	else {

		return false;
	}
}

TArray<ASensorBase*> ASystemManagerBase::GetSensorsOfType(ESensorType sensor_type)
{
	TArray<ASensorBase*> *ret;
	ret = SensorsOfType.Find(sensor_type);
	if (ret != nullptr) {
		return *ret;
	}
	
	return TArray<ASensorBase*>();


}
bool ASystemManagerBase::Save(FString fname)
{
	return pSaverLoader->Save(this, fname);


}
bool ASystemManagerBase::Load(FString fname)
{
	return pSaverLoader->Load(this, fname);
}

bool ASystemManagerBase::SetSlotIndex(AActor* p_actor, INT32S slot_index)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->SensorSlotIndex = slot_index;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetSlotIndex(AActor* p_actor, INT32S& slot_index)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		slot_index = p_obj->SensorSlotIndex;
		return true;
	}
	return false;
}


bool ASystemManagerBase::GetConnectionInfo(AActor* p_actor, INT32S ind, SConnectionInfo& conn)
{
	auto p_obj = ToActorBase(p_actor);
	if (p_obj) {
		return p_obj->GetConnnectionInfo(ind,conn);

	}
	return false;
}
bool ASystemManagerBase::SetConnectionInfo(AActor* p_actor, INT32S ind, SConnectionInfo conn)
{
	auto p_obj = ToActorBase(p_actor);
	if (p_obj) {
		return p_obj->SetConnnectionInfo(ind, conn);

	}
	return false;
}

bool ASystemManagerBase::SetHorizontalFov(AActor* p_actor, FLOAT64 angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->FovHorizontalDeg = angle_deg;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetHorizontalFov(AActor* p_actor, FLOAT64& angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		angle_deg = p_obj->FovHorizontalDeg;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetVerticalFov(AActor* p_actor, FLOAT64 angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->FovVerticalDeg = angle_deg;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetVerticalFov(AActor* p_actor, FLOAT64& angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		angle_deg = p_obj->FovVerticalDeg;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetHorizontalScanStepAngleDeg(AActor* p_actor, FLOAT64 angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->HorizontalScanStepAngleDeg = angle_deg;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetHorizontalScanStepAngleDeg(AActor* p_actor, FLOAT64& angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		angle_deg = p_obj->HorizontalScanStepAngleDeg;
		return true;
	}

	return false;

}

bool ASystemManagerBase::SetVerticalScanStepAngleDeg(AActor* p_actor, FLOAT64 angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->VerticalScanStepAngleDeg = angle_deg;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetVerticalScanStepAngleDeg(AActor* p_actor, FLOAT64& angle_deg)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		angle_deg = p_obj->VerticalScanStepAngleDeg;
		return true;
	}
	return false;
}


bool ASystemManagerBase::GetRangeMinMeter(AActor* p_actor, FLOAT64 &val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		val = p_obj->RangeMinMeter;
		return true;
	}
	return false;
}
bool ASystemManagerBase::SetRangeMinMeter(AActor* p_actor, FLOAT64 val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->RangeMinMeter = val;
		return true;
	}
	return false;
}
bool ASystemManagerBase::SetRangeMaxMeter(AActor* p_actor, FLOAT64 val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->RangeMaxMeter = val;
		return true;
	}
	return false;
}
bool ASystemManagerBase::GetRangeMaxMeter(AActor* p_actor, FLOAT64& val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		val = p_obj->RangeMaxMeter;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetMeasurementErrorMean(AActor* p_actor, FLOAT64 val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->MeasurementErrorMean = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetMeasurementErrorStd(AActor* p_actor, FLOAT64 val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->MeasurementErrorUncertainy = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetMeasurementErrorMean(AActor* p_actor, FLOAT64& val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		val = p_obj->MeasurementErrorMean;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetMeasurementErrorStd(AActor* p_actor, FLOAT64& val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		val = p_obj->MeasurementErrorUncertainy ;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetEnableSurfaceDetect(AActor* p_actor, BOOLEAN val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->MeasurementErrorUncertainy = val;
		return true;
	}

	return false;
}

bool ASystemManagerBase::SetEnableSubsurfaceDetect(AActor* p_actor, BOOLEAN val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->EnableSurfaceDetect = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetEnableFoamDetect(AActor* p_actor, BOOLEAN val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->EnableFoamDetect = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetSeaSurfaceDetectionProb(AActor* p_actor, FLOAT64 val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->SeaSurfaceDetectionProbability = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetEnableSurfaceDetect(AActor* p_actor, BOOLEAN& val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->EnableSurfaceDetect = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetEnableSubsurfaceDetect(AActor* p_actor, BOOLEAN& val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->EnableSubsurfaceDetect = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetEnableFoamDetect(AActor* p_actor, BOOLEAN& val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		val = p_obj->EnableFoamDetect;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetSeaSurfaceDetectionProb(AActor* p_actor, FLOAT64& val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		val = p_obj->SeaSurfaceDetectionProbability;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetMaxSurfacePenetration(AActor* p_actor, FLOAT64 val)
{
	auto p_obj = ToSensorBase(p_actor);
	if (p_obj) {
		p_obj->MaxSurfacePenetrationMeter = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarScanLevel(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->FastScanLevel = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarScannerRPM(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->ScannerRPMValue = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarGainType(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->GainType = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarGainLevel(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->GainLevel = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarSeaClutterType(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->SeaClutterType = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarSeaClutterLevel(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->SeaClutterLevel = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarSeaClutterAutoOffset(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->SeaClutterAutoOffset = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarRainClutterLevel(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->RainClutterLevel = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarMaxGuardZoneCount(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->MaxGuardZoneCount = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::SetRadarMaxSectorBlankingZoneCount(AActor* p_actor, INT32S val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		p_obj->MaxSectorBlankingZoneCount = val;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetMaxSurfacePenetration(AActor* p_actor, FLOAT64& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->MaxSurfacePenetrationMeter;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarScanLevel(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->FastScanLevel;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarScannerRPM(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->ScannerRPMValue;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarGainType(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->GainType;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarGainLevel(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->GainLevel;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarSeaClutterType(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->SeaClutterType;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarSeaClutterLevel(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->SeaClutterLevel;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarSeaClutterAutoOffset(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->SeaClutterAutoOffset;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarRainClutterLevel(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->RainClutterLevel;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarMaxGuardZoneCount(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->MaxGuardZoneCount;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetRadarMaxSectorBlankingZoneCount(AActor* p_actor, INT32S& val)
{
	auto p_obj = ToRadarBase(p_actor);
	if (p_obj) {
		val = p_obj->MaxSectorBlankingZoneCount;
		return true;
	}
	return false;
}

bool ASystemManagerBase::GetSpeedMeterPerSec(AActor* p_actor, FLOAT64& val)
{
	auto* p_plt = GetPlatform();

	if (p_plt) {
		val = p_plt->GetMaxSpeedMeterPerSec();
		return true;
	}
	else {
		return false;
	}
}

bool ASystemManagerBase::SetSpeedMeterPerSec(AActor* p_actor, FLOAT64 val)
{
	auto* p_plt = GetPlatform();

	if (p_plt) {
		p_plt->SetMaxSpeedMeterPerSec(val);
		return true;
	}
	else {
		return false;
	}
}

bool ASystemManagerBase::GetTurnRateDegPerSec(AActor* p_actor, FLOAT64& val)
{
	auto* p_plt = GetPlatform();

	if (p_plt) {
		val = p_plt->GetTurnRateDegPerSec();
		return true;
	}
	else {
		return false;
	}
}

bool ASystemManagerBase::SetTurnRateDegPerSec(AActor* p_actor, FLOAT64 val)
{
	auto* p_plt = GetPlatform();

	if (p_plt) {
		p_plt->SetTurnRateDegPerSec(val);
		return true;
	}
	else {
		return false;
	}
}

bool ASystemManagerBase::AddOrModifyWaypointToPath(AActor* p_actor, INT32S wp_ind, FVector position)
{
	if (p_actor->IsA<APathController>()) {
		APathController* p_path = (APathController*)p_actor;

		p_path->AddOrModifyWaypoint(wp_ind, position);

		return true;
	}
	return false;
}

bool ASystemManagerBase::Bake(AActor* p_actor)
{
	
	auto actorbase = ToActorBase(p_actor);
	if (actorbase) {
		actorbase->Bake();
		return true;
	}
	return false;
}
bool ASystemManagerBase::SetPathClosed(AActor* p_actor, bool is_closed)
{
	auto actorbase = ToActorBase(p_actor);
	if (actorbase && p_actor->IsA< APathController>()) {
		APathController* p_path = (APathController*)actorbase;
		p_path->SetIsClosed(is_closed);
		return true;
	}

	return false;
}

bool ASystemManagerBase::SetPathSpeed(AActor* p_actor, FLOAT64 val)
{
	auto actorbase = ToActorBase(p_actor);
	if (actorbase && actorbase->IsA< APathController>()) {
		APathController* p_path = (APathController*)actorbase;
		p_path->SetPathSpeed(val);
		return true;
	}

	return false;
}

bool ASystemManagerBase::AttachToPath(AActor* p_path_actor, FString actor_name)
{
	auto actorbase = ToActorBase(p_path_actor);
	if (actorbase != nullptr && actorbase->IsA< APathController>()) {
		APathController* p_path = (APathController*)actorbase;

		auto attachable = FindActor(actor_name);
		
		if (attachable) {
			p_path->AddAttachedActor(attachable);
		}
		else {
			return false;
		}
		
	}

	return false;
}
bool ASystemManagerBase::DetachFromPath(AActor* p_path_actor, FString actor_name)
{
	auto actorbase = ToActorBase(p_path_actor);
	if (actorbase != nullptr && actorbase->IsA< APathController>()) {
		APathController* p_path = (APathController*)actorbase;

		auto detachable = FindActor(actor_name);

		if (detachable) {
			return p_path->RemoveAttachedActor(detachable);
		}
		else {
			return false;
		}

	}

	return false;
}

bool ASystemManagerBase::GetWaypointPosition(AActor* p_actor, INT32S wp_ind, FVector& position)
{
	auto actorbase = ToActorBase(p_actor);
	if (actorbase != nullptr && actorbase->IsA< APathController>()) {
		APathController* p_path = (APathController*)actorbase;

		if (p_path) {
			auto ret = p_path->GetWaypointPosition(wp_ind, position);
			return ret;
		}
	}

	return false;
}

 bool ASystemManagerBase::GetPathClosed(AActor* p_actor, bool& is_closed)
{
	 auto actorbase = ToActorBase(p_actor);
	 if (actorbase != nullptr && actorbase->IsA< APathController>()) {
		 APathController* p_path = (APathController*)actorbase;

		 if (p_path) {
			 is_closed = p_path->GetIsClosed();
			 return true;
		 }
	 }

	 return false;
}

 bool ASystemManagerBase::GetPathSegmentCount(AActor* p_actor, INT32S& val)
 {
	 auto p_path = ToPath(p_actor);
	 if (p_path) {
		 val = p_path->NumSegments;
		 return true;
	 }
	 return false;
 }
 bool ASystemManagerBase::SetPathSegmentCount(AActor* p_actor, INT32S& val)
 {
	 auto p_path = ToPath(p_actor);
	 if (p_path) {
		 p_path->NumSegments = val;
		 return true;
	 }
	 return false;
 }

 bool ASystemManagerBase::SetPathLineColor(AActor* p_actor, FColor& val)
 {
	 auto p_path = ToPath(p_actor);
	 if (p_path) {
		 p_path->PathColor = val;
		
		 return true;
	 }
	 return false;
 }

bool ASystemManagerBase::GetPathLineColor(AActor* p_actor, FColor& val)
 {
	auto p_path = ToPath(p_actor);
	if (p_path) {
		val = p_path->PathColor;
		return true;
	}
	return false;
 }


 bool ASystemManagerBase::GetPathStraight(AActor* p_actor, bool& val)
 {
	 auto p_path = ToPath(p_actor);
	 if (p_path) {
		 val = p_path->IsStraightLine;
		 return true;
	 }
	 return false;
 }
 bool ASystemManagerBase::SetPathStraight(AActor* p_actor, bool& val)
 {
	 auto p_path = ToPath(p_actor);
	 if (p_path) {
		 p_path->IsStraightLine = val;
		 return true;
	 }
	 return false;
 }


 bool ASystemManagerBase::GetPathTurnRate(AActor* p_actor, FLOAT64& val)
 {
	 auto p_path = ToPath(p_actor);
	 if (p_path) {
		 val = p_path->TurnRateDegPerSec;
		 return true;
	 }
	 return false;
 }
 bool ASystemManagerBase::SetPathTurnRate(AActor* p_actor, FLOAT64 val)
 {
	 auto p_path = ToPath(p_actor);
	 if (p_path) {
		 p_path->TurnRateDegPerSec = val;
		 return true;
	 }
	 return false;
 }

 bool ASystemManagerBase::SetActorPosition(AActor* p_actor, FVector pos)
 {
	 auto p_base = ToActorBase(p_actor);
	 if (p_base) {
		 p_base->SetActorPosition(pos);
	 }
	 else {
		 p_actor->SetActorLocation(pos);
	 }

	 return true;
 }
 bool ASystemManagerBase::SetActorRelPosition(AActor* p_actor, FVector pos)
 {
	 auto p_base = ToActorBase(p_actor);
	 if (p_base) {
		 p_base->SetActorRelPosition(pos);
	 }
	 else {
		 p_actor->SetActorRelativeLocation(pos);
	 }

	 return true;
 }

 bool ASystemManagerBase::SetActorRot(AActor* p_actor, FVector val)
 {
	 auto p_base = ToActorBase(p_actor);
	 if (p_base) {
		 p_base->SetActorRot(val);
	 }
	 else {
		 CMath::SetActorRotation(p_actor, val);
	 }

	 return true;
 }
 bool ASystemManagerBase::SetActorRelRot(AActor* p_actor, FVector val)
 {
	 auto p_base = ToActorBase(p_actor);
	 if (p_base) {
		 p_base->SetActorRelRot(val);
	 }
	 else {
		 CMath::SetActorRelativeRotation(p_actor, val);
	 }

	 return true;
 }

 bool ASystemManagerBase::FocusCamera(AActor* p_actor)
 {
	 auto* p_platform = GetPlatform();
	 if (p_platform != nullptr) {
		 return GetPlatform()->FocusCamera(p_actor);
	 }
	 else {
		 return false;
	 }

	 
 }

 bool ASystemManagerBase::GetIsUIVisible(bool& val)
 {
	 val =  GetUIController()->GetIsUIVisible();
	 return true;
 }

 bool ASystemManagerBase::SetIsUIVisible(bool val)
 {
	 GetUIController()->SetIsUIVisible(val);
	 return true;
 }

 bool ASystemManagerBase::SetEnvSoundEnabled(INT32S region, bool val)
 {
	  pEnvManager->SetEnvSoundFxEnabled(val);
	  if (val) {
		 // FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();
		 // AudioDevice->SetMasterVolume(Volume);
	  }
	
	  return true;

 }

 bool ASystemManagerBase::GetEnvSoundEnabled(INT32S region, bool& val)
 {
	 return pEnvManager->GetEnvSoundFxEnabled();
 }

 bool ASystemManagerBase::SetCloudPercent(INT32S region, FLOAT64 val)
 {
	  pEnvManager->SetCloudLevelPercent(val);
	  return true;
 }

 bool ASystemManagerBase::GetCloudPercent(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetCloudLevelPercent();
	 return true;
 }

 bool ASystemManagerBase::SetRainPercent(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SeRainLevelPercent(val);
	 return true;
 }

 bool ASystemManagerBase::GetRainPercent(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetRainLevelPercent();
	 return true;
 }

 bool ASystemManagerBase::SetSnowPercent(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetSnowLevelPercent(val);
	 return true;
 }

 bool ASystemManagerBase::GetSnowPercent(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetSnowLevelPercent();
	 return true;
 }

 bool ASystemManagerBase::SetDustPercent(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetDustLevelPercent(val);
	 return true;
 }

 bool ASystemManagerBase::GetDustPercent(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetDustLevelPercent();
	 return true;
 }

 bool ASystemManagerBase::SetFogPercent(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetFogLevelPercent(val);
	 return true;
 }

 bool ASystemManagerBase::GetFogPercent(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetFogLevelPercent();
	 return true;
 }

 bool ASystemManagerBase::SetThunderPercent(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetThunderLevelPercent(val);
	 return true;
 }

 bool ASystemManagerBase::GetThunderPercent(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetThunderLevelPercent();
	 return true;
 }

 bool ASystemManagerBase::SetWindSpeedMeterPerSec(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetWindSpeedMetersPerSec(val);
	 return true;
 }

 bool ASystemManagerBase::GetWindSpeedMeterPerSec(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetWindSpeedMetersPerSec();
	 return true;
 }

 bool ASystemManagerBase::SetWindDirectionDeg(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetWindDirectionDeg(val);
	 return true;
 }

 bool ASystemManagerBase::GetWindDirectionDeg(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetWindDirectionDeg();
	 return true;
 }

 bool ASystemManagerBase::SetTimeOfDayHr(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetTimeOfDayHr(val);
	 return true;
 }

 bool ASystemManagerBase::GetTimeOfDayHr(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetTimeOfDayHr();
	 return true;
 }

 bool ASystemManagerBase::SetEnvTimeFlowScale(INT32S region, FLOAT64 val)
 {
	 pEnvManager->SetEnvTimeFlowScale(val);
	 return true;
 }

 bool ASystemManagerBase::GetEnvTimeFlowScale(INT32S region, FLOAT64& val)
 {
	 val = pEnvManager->GetEnvTimeFlowScale();
	 return true;
 }

 bool ASystemManagerBase::SetTerrainHeightMap(AActor* p_actor, FString path)
 {

	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->HeightMapPath = path;
		 return true;
	 }
	 else {
		 
	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainHeightMap(AActor* p_actor, FString& path)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 path = p_terr->HeightMapPath;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::SetTerrainDepthMap(AActor* p_actor, FString path)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->DepthMapPath = path;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainDepthMap(AActor* p_actor, FString& path)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 path = p_terr->DepthMapPath;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::SetTerraiBaseTexture(AActor* p_actor, FString path)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->BaseTexturePath = path;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerraiBaseTexture(AActor* p_actor, FString& path)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 path = p_terr->BaseTexturePath;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::SetTerrainHeightMapMinLevelMeter(AActor* p_actor, FLOAT64 val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->HeightMapMinLevelMeter = val;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainHeightMapMinLevelMeter(AActor* p_actor, FLOAT64& val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 val = p_terr->HeightMapMinLevelMeter;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::SetTerrainHeightMapMaxLevelMeter(AActor* p_actor, FLOAT64 val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->HeightMapMaxLevelMeter = val;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainHeightMapMaxLevelMeter(AActor* p_actor, FLOAT64& val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 val = p_terr->HeightMapMaxLevelMeter;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::SetTerrainDepthMapMinLevelMeter(AActor* p_actor, FLOAT64 val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->DepthMapMinLevelMeter = val;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainDepthMapMinLevelMeter(AActor* p_actor, FLOAT64& val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 val = p_terr->DepthMapMinLevelMeter;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::SetTerrainDepthMapMaxLevelMeter(AActor* p_actor, FLOAT64 val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->DepthMapMaxLevelMeter = val;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainDepthMapMaxLevelMeter(AActor* p_actor, FLOAT64& val)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 val = p_terr->DepthMapMaxLevelMeter;
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 



 bool ASystemManagerBase::SetTerrainTopLeftCornerCoordLLH(AActor* p_actor, FVector vec)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->SetTerrainTopLeftCornerLLH(vec);
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainTopLeftCornerCoordLLH(AActor* p_actor, FVector& vec)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 vec = p_terr->GetTerrainTopLeftCornerLLH();
		 return true;
	 }
	 else {

	 }

	 return false;
 }


 bool ASystemManagerBase::SetTerrainBottomRightCornerCoordLLH(AActor* p_actor, FVector vec)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 p_terr->SetTerrainRightBottomCornerLLH(vec);
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::GetTerrainBottomRightCornerCoordLLH(AActor* p_actor, FVector& vec)
 {
	 auto p_terr = ToTerrain(p_actor);
	 if (p_terr) {
		 vec = p_terr->GetTerrainRightBottomCornerLLH();
		 return true;
	 }
	 else {

	 }

	 return false;
 }

 bool ASystemManagerBase::SetCamSpeed(AActor* p_actor, FLOAT64 val)
 {
	 auto plt = GetPlatform();
	 if (plt != nullptr) {
		 plt->CamMovementSpeed = val;
		 return true;
	 }
	 return false;
 }

 bool ASystemManagerBase::GetCamSpeed(AActor* p_actor, FLOAT64& val)
 {
	 auto plt = GetPlatform();
	 if (plt != nullptr) {
		 val = plt->CamMovementSpeed;
		 return true;
	 }
	 return false;
 }

 bool ASystemManagerBase::SetCamView(ECamView view)
 {
	 auto plt = GetPlatform();
	 if (plt != nullptr) {
		 plt->SetCamView(view);
		 return true;
	 }
	 return false;
 }

 bool ASystemManagerBase::GetCamView(ECamView& view)
 {
	 auto plt = GetPlatform();
	 if (plt != nullptr) {
		 view = plt->GetCamView();
		 return true;
	 }
	 return false;
 }

 bool ASystemManagerBase::GetBeamEnabled(AActor* p_actor, bool& val)
 {
	 auto obj = ToSensorBase(p_actor);
	 if (obj != nullptr) {
		 val = obj->ShowBeam;
		 return true;
	 }
	 return false;
 }

 bool ASystemManagerBase::SetBeamEnabled(AActor* p_actor, bool val)
 {
	 auto obj = ToSensorBase(p_actor);
	 if (obj != nullptr) {
		 obj->ShowBeam = val;
		 return true;
	 }
	 return false;
 }




bool ASystemManagerBase::GetPathSpeed(AActor* p_actor, FLOAT64& val)
{
	auto actorbase = ToActorBase(p_actor);
	if (actorbase != nullptr && actorbase->IsA< APathController>()) {
		APathController* p_path = (APathController*)actorbase;

		if (p_path) {
			val = p_path->GetPathSpeed();
			return true;
		}
	}

	return false;
}
 bool ASystemManagerBase::GetActorAttachedToPath(AActor* p_path, FString& actor_name)
{
	 auto actorbase = ToActorBase(p_path);
	 if (actorbase != nullptr && actorbase->IsA< APathController>()) {
		 APathController* p_path = (APathController*)actorbase;

		 

		 if (p_path) {
			 auto actor_attached = p_path->GetAttachedActor();
			 if (actor_attached) {
				 actor_name = actor_attached->GetName();
				 return true;
			 }
		 }
		 else {
			 return false;
		 }

	 }

	 return false;
}