// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/Image.h>
#include "UIControllerBase.generated.h"


UCLASS()
class AUIControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIControllerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TArray<UImage*> LidarSlots;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetLidarSlotImage(int no, UImage* p_image);

	UFUNCTION(BlueprintCallable)
		UImage* GetLidarSlotImage(int no);

	UFUNCTION(BlueprintCallable)
		int GetLidarSlotCount();
};
