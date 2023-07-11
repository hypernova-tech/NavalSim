// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Origin/MapOrigin.h"

// Sets default values
AMapOrigin::AMapOrigin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapOrigin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AMapOrigin::GetGELocation(FVector pos)
{
	FVector ret = FVector::Zero();

	FVector diff_vec = pos - Center;

	ret = GetActorLocation() + FVector::RightVector * diff_vec.X * 111e3 + FVector::ForwardVector* 111e3*diff_vec.Y;
	//ret.Z += pos.Z;

	return ret;
}

