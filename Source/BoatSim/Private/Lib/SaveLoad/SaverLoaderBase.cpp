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


	line = CreateCommandWithName(pCLI->SetCommand, p_platform->GetName());
	auto parent = CUtil::GetParentActor(p_platform);
	if (parent != nullptr) {
		AppendOption(line, pCLI->Parent, parent->GetName());
	}
	
	AppendOption(line, pCLI->Position, p_platform->GetActorLocation());
	AppendOption(line, pCLI->Rotation, p_platform->GetActorRotation().Euler());
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
	}
	
	AppendOption(line, pCLI->Position, p_sensor->GetActorLocation());
	AppendOption(line, pCLI->Rotation, p_sensor->GetActorRotation().Euler());
	AppendOption(line, pCLI->Scale, p_sensor->GetActorScale3D());
	AddLine(line);

	p_sensor->Save(this);

#if false
	
	FString cli_value;
	FProperty* Prop = p_sensor->GetClass()->FindPropertyByName(FName("SensorType"));
	if (Prop->HasAnyPropertyFlags(CPF_None))
	{
		// The property has a UPROPERTY declaration.
	}
	
	//auto ret = Prop->FindMetaData("SaveData");

	auto flags = Prop->GetPropertyFlags();
	//auto wrappers = Prop->GetUPropertyWrapper();

	//while (CurrentClass) {


	for (TFieldIterator<FProperty> PropertyIt(p_sensor->GetClass(), EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		auto pro_class = Property->GetClass();
		auto prop_name = Property->GetName();

		
#if false
		if (Property->GetBoolMetaData(TEXT("SaveData"))) {
			const FString& SaveDataValue = Property->GetMetaData(TEXT("SaveData"));
			if (SaveDataValue.Equals(TEXT("true")))
			{

			}
			else {
				continue;
			}
		}
		else {
			continue;
		}

		if (Property->HasMetaData(TEXT("cli"))) {
			const FString& cli_data = Property->GetMetaData(TEXT("cli"));
			if (!cli_data.Equals(TEXT("")))
			{
				cli_value = cli_data;
			}
			else {
				continue;
			}
		}
		else {
			continue;
		}

#endif

		// Get Property Name
		FString PropertyName = Property->GetName();

		// Log Property Name
		UE_LOG(LogTemp, Warning, TEXT("Property Name: %s"), *PropertyName);

		if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
		{
			float Value = *FloatProperty->ContainerPtrToValuePtr<float>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
		{
			double Value = *DoubleProperty->ContainerPtrToValuePtr<double>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			bool bValue = *BoolProperty->ContainerPtrToValuePtr<bool>(p_sensor);
			AppendOption(line, cli_value, bValue);
		}
		else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
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
		else if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
		{
			int32 Value = *EnumProperty->ContainerPtrToValuePtr<int32>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FIntProperty* Int32Property = CastField<FIntProperty>(Property))
		{
			int32 Value = *Int32Property->ContainerPtrToValuePtr<int32>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
		{

			uint8 Value = *ByteProperty->ContainerPtrToValuePtr<uint8>(p_sensor);
			AppendOption(line, cli_value, Value);

		}
		else if (FInt16Property* Int16Property = CastField<FInt16Property>(Property))
		{
			int16 Value = *Int16Property->ContainerPtrToValuePtr<int16>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FUInt16Property* UInt16Property = CastField<FUInt16Property>(Property))
		{
			uint16 Value = *UInt16Property->ContainerPtrToValuePtr<uint16>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FUInt32Property* UInt32Property = CastField<FUInt32Property>(Property))
		{
			uint32 Value = *UInt32Property->ContainerPtrToValuePtr<uint32>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FInt64Property* Int64Property = CastField<FInt64Property>(Property))
		{
			int64 Value = *Int64Property->ContainerPtrToValuePtr<int64>(p_sensor);
			AppendOption(line, cli_value, Value);
		}
		else if (FUInt64Property* UInt64Property = CastField<FUInt64Property>(Property))
		{
			uint64 Value = *UInt64Property->ContainerPtrToValuePtr<uint64>(p_sensor);
			AppendOption(line, cli_value, Value);
		}


	}
#if false
		for (TFieldIterator<UProperty> PropertyIt(p_sensor->GetClass(), EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
		{
			UProperty* Property = *PropertyIt;


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
#endif
		//CurrentClass = Cast<UClass>(CurrentClass->GetSuperStruct());
	//}

#endif



}

void USaverLoaderBase::SaveActor(AActorBase* p_actor, TArray<FString>& cli)
{
	FString line = CreateCommandWithName(pCLI->CreateCommand, p_actor->GetName());

	AppendOption(line, pCLI->Bp, p_actor->GetBlueprintName());
	cli.Add(line);
}

bool USaverLoaderBase::Save(ISystemAPI* p_api, FString file_name)
{
	pSystemApi = p_api;

	pCLI = p_api->GetConsole()->GetCommandManager();

	auto platform = p_api->GetPlatform();
	
	
	if (platform != nullptr) {
		SavePlatform(platform, CLIList);
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



