// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataDefs.h"
#include "DataManager.generated.h"

USTRUCT(BlueprintType)
struct FBlueprintPathEntry 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString Name;
	UPROPERTY(EditAnywhere)
		TAssetPtr<UBlueprint> BlueprintAsset;
	UPROPERTY(EditAnywhere)
		EBlueprintCategory Category;

};




UCLASS()
class ADataManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADataManager();

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
	FString GetCategoryName(EBlueprintCategory category);
};
