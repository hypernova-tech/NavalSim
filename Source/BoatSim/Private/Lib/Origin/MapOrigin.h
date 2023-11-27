// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapOrigin.generated.h"

UCLASS()
class AMapOrigin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapOrigin();

	UPROPERTY(EditAnywhere)
		FVector Center;
	UPROPERTY(EditAnywhere)
		int Speed;

private:
	static AMapOrigin* pInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool IsCenterChangedOnce = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static AMapOrigin* GetMapOrigin();

	FVector GetGELocation(FVector pos);
	void ChangeCenterCoordinateOnce(FVector new_center);



};
