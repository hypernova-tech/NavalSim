// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SaveLoad/SaverLoaderBase.h"
#include <Lib/Utils/CUtil.h>
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/UnrealType.h"
#include <UObject/UnrealTypePrivate.h>
#include <Lib/Sensor/SensorBase.h>
#include <Lib/Math/CMath.h>

// Sets default values for this component's properties
USaverLoaderBase::USaverLoaderBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USaverLoaderBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USaverLoaderBase::NewLine()
{
	CLIList.Add("");
}



void USaverLoaderBase::AppendOption(FString &line, FString option, FString value)
{
	line += " --" + option + " " + value;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, bool value)
{
	FString val_str = value ? "1" : "0";
	line += " --" + option + " " + val_str;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, INT32U value)
{
	FString val_str = CUtil::IntToString(value);
	line += " --" + option + " " + val_str;
}
void USaverLoaderBase::AppendOption(FString &line, FString option, INT32S value)
{
	FString val_str = CUtil::IntToString(value);
	line += " --" + option + " " + val_str;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, INT64U value)
{
	FString val_str = CUtil::IntToString(value);
	line += " --" + option + " " + val_str;
}
void USaverLoaderBase::AppendOption(FString &line, FString option, INT64S value)
{
	FString val_str = CUtil::IntToString(value);
	line += " --" + option + " " + val_str;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, INT16U value)
{
	FString val_str = CUtil::IntToString((INT32U)value);
	line += " --" + option + " " + val_str;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, FLOAT32 value)
{
	FString val_str = CUtil::FloatToString(value);
	line += " --" + option + " " + val_str;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, FLOAT64 value)
{
	FString val_str = CUtil::FloatToString(value);
	line += " --" + option + " " + val_str;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, FVector2D value)
{
	FString val_str = "\"" + CUtil::FloatToString(value.X) + " " + CUtil::FloatToString(value.Y) + "\"";
	line += " --" + option + " " + val_str;
}

void USaverLoaderBase::AppendOption(FString &line, FString option, FVector value)
{
	FString val_str = "\"" + CUtil::FloatToString(value.X) + " " + CUtil::FloatToString(value.Y) +" "+CUtil::FloatToString(value.Z)+ "\"";
	line += " --" + option + " " + val_str;
}
void USaverLoaderBase::AppendOption(FString& line, FString option, FColor color)
{
	FString val_str = "\"" + CUtil::IntToString(color.R) + " " + CUtil::IntToString(color.G) + " " + CUtil::IntToString(color.B) + " " + CUtil::IntToString(color.A) + "\"";
	line += " --" + option + " " + val_str;
}


void USaverLoaderBase::AddLine(FString line)
{
	CLIList.Add(line);
}

// Called every frame
void USaverLoaderBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	StateMachine();
	// ...
}

FString USaverLoaderBase::CreateCommand(FString cmd)
{
	return cmd;
}

FString USaverLoaderBase::CreateCommandWithName(FString cmd, FString name)
{
	FString ret = cmd;
	AppendOption(ret, CCLICommandManager::Name, name);
	return ret;
}

void USaverLoaderBase::SavePlatform(ACBoatBase* p_platform, TArray<FString>& cli)
{

	FString line;

	


	if (p_platform->GetTarget()) {
		line = CreateCommand(pCLI->SetCommand);
		AppendOption(line, pCLI->CamFollow, p_platform->GetTarget()->GetName());
		AddLine(line);

		line = CreateCommand(pCLI->SetCommand);
		AppendOption(line, pCLI->CamFollowTranslation, p_platform->GetCamRelativeOffsetTranslation());
		AddLine(line);

		line = CreateCommand(pCLI->SetCommand);
		AppendOption(line, pCLI->CamFollowEuler, p_platform->GetCamRelativeOffsetRPYDeg());
		AddLine(line);

		NewLine();
	}
	
	
	


}

void USaverLoaderBase::SaveSensor(ASensorBase* p_sensor, TArray<FString>& cli)
{

	p_sensor->Save(this);

#if false
	FString line = CreateCommand(pCLI->CreateCommand);
	AppendOption(line, pCLI->Name, p_sensor->GetName());
	AppendOption(line, pCLI->Bp, p_sensor->GetBlueprintName());
	AddLine(line);
#endif
}


void USaverLoaderBase::SaveActor(AActorBase* p_actor, TArray<FString>& cli)
{
	p_actor->Save(this);
	
}



bool USaverLoaderBase::Save(ISystemAPI* p_api, FString file_name)
{
	pSystemApi = p_api;

	pCLI = p_api->GetConsole()->GetCommandManager();

	
	TArray<AActor*> actor_as_platforms;
	p_api->QueryActors(EActorQueryArgs::ActorAsPlatform, actor_as_platforms);

	for (auto val : actor_as_platforms) {
		AActorBase* p_base = (AActorBase*)val;

		if (p_base && p_base->GetIsSaveEnabled()) {
			SaveActor(p_base, CLIList);
			NewLine();
		}
	}

	TArray<AActor*> non_sensors;
	p_api->QueryActors(EActorQueryArgs::ActorBasesExceptSensorsAndPathsAndGimbalsActorAsPlatform, non_sensors);

	for (auto p_actor : non_sensors) {
		AActorBase* p_actor_base = (AActorBase*)p_actor;

		if (p_actor_base && p_actor_base->GetIsSaveEnabled()) {
			SaveActor((AActorBase*)p_actor, CLIList);
			NewLine();
		}

	}

	TArray<AActor*> gimbals;
	p_api->QueryActors(EActorQueryArgs::Gimbals, gimbals);

	for (auto p_gimbal : gimbals) {
		AActorBase* p_actor_base = (AActorBase*)p_gimbal;

		if (p_actor_base && p_actor_base->GetIsSaveEnabled()) {
			SaveActor(p_actor_base, CLIList);
			NewLine();
		}

	}

	auto sensors = p_api->GetAllSensors();

	for (auto sensor : sensors) {
		if (sensor->GetIsSaveEnabled()) {
			SaveSensor(sensor, CLIList);
			NewLine();
		}
		
	}
	TArray<AActor*> path;
	p_api->QueryActors(EActorQueryArgs::ActorBasesOnlyPaths, path);

	for (auto p_path : path) {
		AActorBase* p_actor_base = (AActorBase*)p_path;

		if (p_actor_base && p_actor_base->GetIsSaveEnabled()) {
			SaveActor((AActorBase*)p_path, CLIList);
			NewLine();
		}
	}

	TArray<AActor*> platforms;

	p_api->QueryActors(EActorQueryArgs::Platforms, platforms);

	for (auto platform : platforms) {
		SavePlatform((APlatformBase*)platform, CLIList);
	}


	// Combine the array into a single string where each array element gets its own line
	FString CombinedString = FString::Join(CLIList, TEXT("\n"));

	// Save the string to the file
	FString FilePath = FPaths::Combine(FPaths::ProjectDir(), *file_name);
	p_api->SendConsoleResponse("File Path Proj: " + FilePath);
	p_api->SendConsoleResponse("Save  Proj: " + FPaths::ProjectSavedDir());
	p_api->SendConsoleResponse("Engine Save  Dir: " + FPaths::EngineSavedDir());
	p_api->SendConsoleResponse("ShouldSaveToUserDir: " + FPaths::ShouldSaveToUserDir());
	bool ret =  FFileHelper::SaveStringToFile(CombinedString, *FilePath);



	CLIList.Reset();
	return ret;

}




bool USaverLoaderBase::Load(ISystemAPI* p_api, FString file_name)
{
	pSystemApi = p_api;
	pCLI = p_api->GetConsole()->GetCommandManager();
	
	FString FilePath = FPaths::Combine(FPaths::ProjectDir(), *file_name);

	if (FFileHelper::LoadFileToStringArray(CommandLines, *FilePath))
	{
		State = LoaderStateMachineProcess;
		CurrentCommandIndex = 0;
		return true;
	}

	return false;
}

void USaverLoaderBase::ProcessCurrentCommand(INT32S& tick_delay)
{
	auto curr_cmd = CommandLines[CurrentCommandIndex];
	if (curr_cmd.Contains("--" + CCLICommandManager::Parent)) {
		ParentingCommands.Add(curr_cmd);
	}
	else {
		pSystemApi->SendConsoleResponse(curr_cmd);
		pSystemApi->GetConsole()->Command(curr_cmd, tick_delay);
		CUtil::DebugLog(curr_cmd);
	}

}
void USaverLoaderBase::ProcessParentingCommands()
{
	INT32S curr_delay = 0;
	for (auto cmd : ParentingCommands) {
		pSystemApi->SendConsoleResponse(cmd);
		pSystemApi->GetConsole()->Command(cmd, curr_delay);
		CUtil::DebugLog(cmd);
	}
	ParentingCommands.Reset();
}
void USaverLoaderBase::StateMachine()
{
	auto curr_state = State;
	auto next_state = curr_state;
	INT32S tick_delay = 0;

	switch (curr_state)
	{
	case LoaderStateMachineIdle:
		break;
	case LoaderStateMachineProcess:
		while (true) {
			ProcessCurrentCommand(tick_delay);
			CurrentCommandIndex++;

			if (CurrentCommandIndex >= CommandLines.Num()) {
				next_state = LoaderStateMachineLoadComplete;
				break;
			}
			else {
				if (tick_delay > 0) {
					next_state = LoaderStateMachineDelay;
					TickDelayCount = tick_delay;
					break;
				}
			}
		}
	
		break;
	case LoaderStateMachineDelay:
		TickDelayCount--;
		if (TickDelayCount == 0) {
			next_state = LoaderStateMachineProcess;
		}
		break;
	case LoaderStateMachineLoadComplete:
		ProcessParentingCommands();
		pSystemApi->GetConsole()->SendBlueprints();
		pSystemApi->GetConsole()->SendActorBases();
		next_state = LoaderStateMachineWait;
		break;
	case LoaderStateMachineWait:
		break;
	default:
		break;
	}

	State = next_state;
}



