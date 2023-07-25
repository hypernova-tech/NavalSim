// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/DataContainer/DataContainer.h"

// Sets default values
ADataContainer::ADataContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADataContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADataContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FBlueprintPathEntry ADataContainer::FindBlueprintInfo(FString name)
{
	for (FBlueprintPathEntry entry : BlueprintInfo) {
		if (entry.Name == name) {
			return entry;
		}
	}

	return FBlueprintPathEntry();
}

