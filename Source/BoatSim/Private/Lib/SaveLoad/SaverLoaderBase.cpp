// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SaveLoad/SaverLoaderBase.h"
#include <Lib/Utils/CUtil.h>
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/UnrealType.h"
#include <UObject/UnrealTypePrivate.h>
#include <Lib/Sensor/SensorBase.h>

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

void USaverLoaderBase::AppendOption(FString &line, FString option, BOOLEAN value)
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

void USaverLoaderBase::AddLine(FString line)
{
	CLIList.Add(line);
}

// Called every frame
void USaverLoaderBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	FString line = CreateCommand(pCLI->CreateCommand);
	AppendOption(line, pCLI->Name, p_platform->GetName());
	AppendOption(line, pCLI->Bp, p_platform->BlueprintName);
	AddLine(line);

	line = CreateCommand(pCLI->SetCommand);

	if (CUtil::IsPossedByPlayerController(GetWorld(), p_platform)) {
		AppendOption(line, pCLI->Controller, p_platform->GetName());
		AddLine(line);
	}
	
	

	line = CreateCommandWithName(pCLI->SetCommand, p_platform->GetName());
	auto parent = CUtil::GetParentActor(p_platform);
	if (parent != nullptr) {
		AppendOption(line, pCLI->Parent, parent->GetName());
		AddLine(line);
	}
	
	line = CreateCommandWithName(pCLI->SetCommand, p_platform->GetName());
	AppendOption(line, pCLI->Position, p_platform->GetActorLocation());
	AddLine(line);

	line = CreateCommandWithName(pCLI->SetCommand, p_platform->GetName());
	AppendOption(line, pCLI->Rotation, p_platform->GetActorRotation().Euler());
	AddLine(line);

	line = CreateCommandWithName(pCLI->SetCommand, p_platform->GetName());
	AppendOption(line, pCLI->Scale, p_platform->GetActorScale3D());
	AddLine(line);
	NewLine();

}

void USaverLoaderBase::SaveSensor(ASensorBase* p_sensor, TArray<FString>& cli)
{
	FString line = CreateCommand(pCLI->CreateCommand);
	AppendOption(line, pCLI->Name, p_sensor->GetName());
	AppendOption(line, pCLI->Bp, p_sensor->GetBlueprintName());
	AddLine(line);
	

	line = CreateCommandWithName(pCLI->SetCommand, p_sensor->GetName());

	auto parent = CUtil::GetParentActor(p_sensor);
	if (parent != nullptr) {
		AppendOption(line, pCLI->Parent, parent->GetName());
		AddLine(line);
	}
	
	line = CreateCommandWithName(pCLI->SetCommand, p_sensor->GetName());
	AppendOption(line, pCLI->Position, TOW(p_sensor->GetActorLocation()));
	AddLine(line);

	line = CreateCommandWithName(pCLI->SetCommand, p_sensor->GetName());
	AppendOption(line, pCLI->Rotation, p_sensor->GetActorRotation().Euler());
	AddLine(line);
	
	line = CreateCommandWithName(pCLI->SetCommand, p_sensor->GetName());
	AppendOption(line, pCLI->Scale, p_sensor->GetActorScale3D());
	AddLine(line);

	p_sensor->Save(this);



}


void USaverLoaderBase::SaveActor(AActorBase* p_actor, TArray<FString>& cli)
{
	FString line = CreateCommand(pCLI->CreateCommand);
	AppendOption(line, pCLI->Name, p_actor->GetName());
	AppendOption(line, pCLI->Bp, p_actor->GetBlueprintName());
	AddLine(line);


	line = CreateCommandWithName(pCLI->SetCommand, p_actor->GetName());

	auto parent = CUtil::GetParentActor(p_actor);
	if (parent != nullptr) {
		AppendOption(line, pCLI->Parent, parent->GetName());
		AddLine(line);
	}

	line = CreateCommandWithName(pCLI->SetCommand, p_actor->GetName());
	AppendOption(line, pCLI->Position, TOW(p_actor->GetActorLocation()));
	AddLine(line);

	line = CreateCommandWithName(pCLI->SetCommand, p_actor->GetName());
	AppendOption(line, pCLI->Rotation, p_actor->GetActorRotation().Euler());
	AddLine(line);

	line = CreateCommandWithName(pCLI->SetCommand, p_actor->GetName());
	AppendOption(line, pCLI->Scale, p_actor->GetActorScale3D());
	AddLine(line);

	p_actor->Save(this);
}

bool USaverLoaderBase::Save(ISystemAPI* p_api, FString file_name)
{
	pSystemApi = p_api;

	pCLI = p_api->GetConsole()->GetCommandManager();

	auto platform = p_api->GetPlatform();
	
	
	if (platform != nullptr) {
		SavePlatform(platform, CLIList);
	}


	auto non_sensors = p_api->QueryActors(EActorQueryArgs::ActorBasesExceptSensors);

	for (auto p_actor : non_sensors) {
		SaveActor((AActorBase*)p_actor, CLIList);
		NewLine();
	}

	auto sensors = p_api->GetAllSensors();

	for (auto sensor : sensors) {
		SaveSensor(sensor, CLIList);
		NewLine();
	}


	// Combine the array into a single string where each array element gets its own line
	FString CombinedString = FString::Join(CLIList, TEXT("\n"));

	// Save the string to the file
	FString FilePath = FPaths::Combine(FPaths::ProjectDir(), *file_name);
	bool ret =  FFileHelper::SaveStringToFile(CombinedString, *FilePath);



	CLIList.Reset();
	return ret;

}




bool USaverLoaderBase::Load(ISystemAPI* p_api, FString file_name)
{
	pSystemApi = p_api;
	pCLI = p_api->GetConsole()->GetCommandManager();
	TArray<FString> lines;

	FString test = "1,2,3";
	char dest[128];
	CUtil::FStringToAsciiChar(test, dest, sizeof(dest));

	test = "set --name viscam1 --parent gimbal1 --position \" - 80690.000000 - 40990.000000 1970.000000\"";// --rotation "0.000000 45.000000 0.000000" --scale "1.000000 1.000000 1.000000"";
	memset(dest, 0, sizeof(dest));
	CUtil::FStringToAsciiChar(test, dest, sizeof(dest));



	test = "set --name viscam1 --parent gimbal1 --position \" - 80690.000000 - 40990.000000 1970.000000\" --rotation \"0.000000 45.000000 0.000000\"";
	memset(dest, 0, sizeof(dest));

	// Convert FString to UTF-8 encoded string
	FString Utf8Str = test.ReplaceEscapedCharWithChar();

	// Wrap the converted string in double quotes
	
	CUtil::FStringToAsciiChar(Utf8Str, dest, sizeof(dest));

	test = "set --name viscam1 --parent gimbal1 --position \" - 80690.000000 - 40990.000000 1970.000000\" --rotation \"0.000000 45.000000 0.000000\" --scale \"1.000000 1.000000 1.000000\"";
	memset(dest, 0, sizeof(dest));
	CUtil::FStringToAsciiChar(test, dest, sizeof(dest));

	FString FilePath = FPaths::Combine(FPaths::ProjectDir(), *file_name);

	if (FFileHelper::LoadFileToStringArray(lines, *FilePath))
	{
		// Successfully loaded the file
		for (const FString& line : lines)
		{
			if (line == "") {
				continue;
			}
			FString inp = line;
			p_api->SendConsoleResponse(inp);
			p_api->GetConsole()->Command(inp);
			CUtil::DebugLog(inp);
		}

		return true;
	}

	return false;
}



