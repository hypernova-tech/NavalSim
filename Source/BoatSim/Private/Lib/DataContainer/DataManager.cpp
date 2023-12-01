// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/DataContainer/DataManager.h"


// Sets default values
ADataManager::ADataManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADataManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FBlueprintPathEntry ADataManager::FindBlueprintInfo(FString name)
{
	for (FBlueprintPathEntry entry : BlueprintInfo) {
		if (entry.Name == name) {
			return entry;
		}
	}

	return FBlueprintPathEntry();
}

TArray<FBlueprintPathEntry>* ADataManager::GetBlueprintInfo()
{
	return &BlueprintInfo;
}

FString ADataManager::GetCategoryName(EBlueprintCategory category)
{
	switch (category)
	{
	case EBlueprintCategory::CategoryUnknown:
		return "Unkown";
	case EBlueprintCategory::CategorySensor:
		return "Sensor";
	case EBlueprintCategory::CategoryPath:
		return "Path";
	case EBlueprintCategory::CategoryActor:
		return "Actor";
	case EBlueprintCategory::CategoryLandVehicle:
		return "Land Vechicles";
	case EBlueprintCategory::CategoryAirVehicle:
		return "Air Vechicles";
	case EBlueprintCategory::CategoryMarineVehicle:
		return "Marine Vechicles";
	case EBlueprintCategory::CategoryVegatation:
		return "Vegatations";
	case EBlueprintCategory::CategoryBuilding:
		return "Buildings";
	case EBlueprintCategory::CategoryConnection:
		return "Connection";
	case EBlueprintCategory::CategoryIsland:
		return "Island";
	case EBlueprintCategory::CategoryHuman:
		return "Human";
	case EBlueprintCategory::CategoryAnimal:
		return "Animal";
	default:
		return "Unkown";
	}



}

