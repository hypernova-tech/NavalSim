// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeoReferencingSystem.h"
#include "MapOrigin.generated.h"

enum ECoordSystem {

	CoordSystemUNDEF = 0,
	CoordSystemXYZ = 1,
	CoordSystemLLH_WGS84 = 2
};


UCLASS()
class AMapOrigin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapOrigin();

	UPROPERTY(EditAnywhere)
		FVector CenterLLH;
	UPROPERTY(EditAnywhere)
		int Speed;

	AGeoReferencingSystem* pGeoReferencingSystem;
private:
	static AMapOrigin* pInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool IsCenterChangedOnce = false;
	void UpdateGeoreferencingCenter();
	void Test();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static AMapOrigin* GetMapOrigin();

	FVector ConvertLLHToUEXYZ(FVector pos);
	FVector ConvertUEXYZToLLH(FVector pos);
	
	void ChangeCenterCoordinateOnce(FVector new_center);



};
