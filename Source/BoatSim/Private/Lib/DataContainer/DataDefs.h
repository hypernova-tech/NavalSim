// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

// DONT FORGET TO UPDATE FString ADataManager::GetCategoryName(EBlueprintCategory category)
UENUM(BlueprintType)
enum class EBlueprintCategory : uint8
{
	CategoryUnknown = 0,
	CategoryReserved1 = 1,
	CategorySensor = 2,
	CategoryPath = 3,
	CategoryReserved2 = 4,
	CategoryActor = 5,
	CategoryLandVehicle = 6,
	CategoryAirVehicle = 7,
	CategoryMarineVehicle = 8,
	CategoryVegatation = 9,
	CategoryBuilding = 10,
	CategoryConnection = 11,
	CategoryIsland = 12,
	CategoryHuman = 13,
	CategoryAnimal = 14,
	CategoryEnvironment = 15,
	CategoryTerrain = 16,
	CategoryPrimitives =17,
	CategoryGimbal = 18,
	CategoryVFX = 19,
	CategoryLast = 255
};



class DataDefs
{
public:
	DataDefs();
	~DataDefs();
};
