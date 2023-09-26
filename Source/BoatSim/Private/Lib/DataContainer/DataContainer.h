// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataContainer.generated.h"

USTRUCT(BlueprintType)
struct FBlueprintPathEntry {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString Name;
	UPROPERTY(EditAnywhere)
		FString BlueprintPath;
	UPROPERTY(EditAnywhere)
		TAssetPtr<UBlueprint> BlueprintAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		AActor *ActorToSpawn;
};




UCLASS()
class ADataContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataContainer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TArray<FBlueprintPathEntry> BlueprintInfo;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FBlueprintPathEntry FindBlueprintInfo(FString name);


	TArray<FBlueprintPathEntry>* GetBlueprintInfo();

};
