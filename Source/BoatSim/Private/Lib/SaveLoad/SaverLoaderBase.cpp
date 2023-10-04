// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SaveLoad/SaverLoaderBase.h"
#include <Lib/Utils/CUtil.h>
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/UnrealType.h"
#include <UObject/UnrealTypePrivate.h>

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

void USaverLoaderBase::SavePlatform(ACBoatBase* p_platform, TArray<FString>& cli)
{

	FString line = CreateCommand(pCLI->CreateCommand);
	AppendOption(line, pCLI->Name, p_platform->GetName());
	AppendOption(line, pCLI->Bp, p_platform->BlueprintName);
	cli.Add(line);


	line = CreateCommand(pCLI->SetCommand);
	auto parent = CUtil::GetParentActor(p_platform);
	if (parent != nullptr) {
		AppendOption(line, pCLI->Parent, parent->GetName());
	}
	
	AppendOption(line, pCLI->Position, p_platform->GetActorLocation());
	AppendOption(line, pCLI->Rotation, p_platform->GetActorRotation().Euler());
	AppendOption(line, pCLI->Scale, p_platform->GetActorScale3D());
	cli.Add(line);

}

void USaverLoaderBase::SaveSensor(ASensorBase* p_sensor, TArray<FString>& cli)
{
	FString line = CreateCommand(pCLI->CreateCommand);
	AppendOption(line, pCLI->Name, p_sensor->GetName());
	AppendOption(line, pCLI->Bp, p_sensor->GetBlueprintName());
	cli.Add(line);
	

	line = CreateCommand(pCLI->SetCommand);

	auto parent = CUtil::GetParentActor(p_sensor);
	if (parent != nullptr) {
		AppendOption(line, pCLI->Parent, parent->GetName());
	}
	
	AppendOption(line, pCLI->Position, p_sensor->GetActorLocation());
	AppendOption(line, pCLI->Rotation, p_sensor->GetActorRotation().Euler());
	AppendOption(line, pCLI->Scale, p_sensor->GetActorScale3D());
	
	FString cli_value;

	UClass* CurrentClass = p_sensor->GetClass();

#if false
	while (CurrentClass && CurrentClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
	{
		CurrentClass = CurrentClass->GetSuperClass();
	}

	if (CurrentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Base C++ Class: %s"), *CurrentClass->GetName());
	}
#endif

	for (TFieldIterator<UProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper, EFieldIteratorFlags::IncludeDeprecated, EFieldIteratorFlags::IncludeInterfaces); PropIt; ++PropIt)
	{
		UProperty* Property = *PropIt;

	}

	//while (CurrentClass) {
		auto cls = CurrentClass;
		for (TFieldIterator<UProperty> PropIt(cls, EFieldIteratorFlags::IncludeSuper, EFieldIteratorFlags::IncludeDeprecated, EFieldIteratorFlags::IncludeInterfaces); PropIt; ++PropIt)
		{
			UProperty* Property = *PropIt;


			if (Property->GetBoolMetaData(TEXT("SaveData"))) {
				const FString& SaveDataValue = Property->GetMetaData(TEXT("SaveData"));
				if (SaveDataValue.Equals(TEXT("true")))
				{

				}
				else {
					continue;
				}
			}

			if (Property->GetBoolMetaData(TEXT("CLI"))) {
				cli_value = Property->GetMetaData(TEXT("CLI"));
			}
			else {
				continue;
			}

			// Get Property Name
			FString PropertyName = Property->GetName();

			// Log Property Name
			UE_LOG(LogTemp, Warning, TEXT("Property Name: %s"), *PropertyName);

			if (UFloatProperty* FloatProperty = Cast<UFloatProperty>(Property))
			{
				float Value = *FloatProperty->ContainerPtrToValuePtr<float>(p_sensor);
				AppendOption(line, cli_value, Value);
			}
			else if (UDoubleProperty* DoubleProperty = Cast<UDoubleProperty>(Property))
			{
				double Value = *DoubleProperty->ContainerPtrToValuePtr<double>(p_sensor);
				AppendOption(line, cli_value, Value);
			}
			else if (UBoolProperty* BoolProperty = Cast<UBoolProperty>(Property))
			{
				bool bValue = *BoolProperty->ContainerPtrToValuePtr<bool>(p_sensor);
				AppendOption(line, cli_value, bValue);
			}
			else if (UStructProperty* StructProperty = Cast<UStructProperty>(Property))
			{
				if (StructProperty->Struct->GetFName() == NAME_Vector)
				{
					FVector Value = *StructProperty->ContainerPtrToValuePtr<FVector>(p_sensor);
					AppendOption(line, cli_value, Value);
				}
				else if (StructProperty->Struct->GetFName() == NAME_Vector2D)
				{
					FVector2D Value = *StructProperty->ContainerPtrToValuePtr<FVector2D>(p_sensor);
					AppendOption(line, cli_value, Value);
				}
			}
			else if (UIntProperty* Int32Property = Cast<UIntProperty>(Property))
			{
				int32 Value = *Int32Property->ContainerPtrToValuePtr<int32>(p_sensor);
				AppendOption(line, cli_value, Value);
			}
			else if (UByteProperty* ByteProperty = Cast<UByteProperty>(Property))
			{

				uint8 Value = *ByteProperty->ContainerPtrToValuePtr<uint8>(p_sensor);
				AppendOption(line, cli_value, Value);

			}
			else if (UInt16Property* Int16Property = Cast<UInt16Property>(Property))
			{
				int16 Value = *Int16Property->ContainerPtrToValuePtr<int16>(p_sensor);
				AppendOption(line, cli_value, Value);
			}
			else if (UUInt16Property* UInt16Property = Cast<UUInt16Property>(Property))
			{
				uint16 Value = *UInt16Property->ContainerPtrToValuePtr<uint16>(p_sensor);
				AppendOption(line, cli_value, Value);
			}
			else if (UUInt32Property* UInt32Property = Cast<UUInt32Property>(Property))
			{
				uint32 Value = *UInt32Property->ContainerPtrToValuePtr<uint32>(p_sensor);
				AppendOption(line, cli_value, Value);
			}
			else if (UInt64Property* Int64Property = Cast<UInt64Property>(Property))
			{
				int64 Value = *Int64Property->ContainerPtrToValuePtr<int64>(p_sensor);
				AppendOption(line, cli_value, Value);
			}
			else if (UUInt64Property* UInt64Property = Cast<UUInt64Property>(Property))
			{
				uint64 Value = *UInt64Property->ContainerPtrToValuePtr<uint64>(p_sensor);
				AppendOption(line, cli_value, Value);
			}


		}
		//CurrentClass = Cast<UClass>(CurrentClass->GetSuperStruct());
	//}

	cli.Add(line);

}

void USaverLoaderBase::SaveActor(AActorBase* p_actor, TArray<FString>& cli)
{
	FString line = CreateCommand(pCLI->CreateCommand);
	AppendOption(line, pCLI->Name, p_actor->GetName());
	AppendOption(line, pCLI->Bp, p_actor->GetBlueprintName());
	cli.Add(line);
}

bool USaverLoaderBase::Save(ISystemAPI* p_api, FString file_name)
{
	pSystemApi = p_api;

	pCLI = p_api->GetConsole()->GetCommandManager();

	auto platform = p_api->GetPlatform();
	TArray<FString> cli;
	
	if (platform != nullptr) {
		SavePlatform(platform, cli);
	}


	auto sensors = p_api->GetAllSensors();

	for (auto sensor : sensors) {
		SaveSensor(sensor, cli);
	}


	// Combine the array into a single string where each array element gets its own line
	FString CombinedString = FString::Join(cli, TEXT("\n"));

	// Save the string to the file
	bool ret =  FFileHelper::SaveStringToFile(CombinedString, *file_name);
	return ret;

}

bool USaverLoaderBase::Load(ISystemAPI* p_api, FString file_name)
{
	pSystemApi = p_api;
	pCLI = p_api->GetConsole()->GetCommandManager();
	TArray<FString> lines;

	if (FFileHelper::LoadFileToStringArray(lines, *file_name))
	{
		// Successfully loaded the file
		for (const FString& line : lines)
		{
			CUtil::DebugLog(line);
		}

		return true;
	}

	return false;
}



