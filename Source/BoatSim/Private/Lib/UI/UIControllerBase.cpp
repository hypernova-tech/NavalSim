// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/UI/UIControllerBase.h"

// Sets default values
AUIControllerBase::AUIControllerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUIControllerBase::SetLidarSlotImage(int no, UImage* p_image)
{
	LidarSlots.Add(p_image) ;
}

UImage* AUIControllerBase::GetLidarSlotImage(int no)
{

	return LidarSlots[no];
}
int AUIControllerBase::GetLidarSlotCount()
{
	return LidarSlots.Num();
}