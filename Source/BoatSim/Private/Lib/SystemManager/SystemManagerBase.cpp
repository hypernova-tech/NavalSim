// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/SystemManager/SystemManagerBase.h"

// Sets default values
ASystemManagerBase* ASystemManagerBase::pInstance = nullptr;

ASystemManagerBase::ASystemManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pInstance = this;

}

ASystemManagerBase* ASystemManagerBase::GetInstance()
{
	return pInstance;
}

// Called when the game starts or when spawned
void ASystemManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASystemManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMapOrigin* ASystemManagerBase::GetMapOrigin()
{
	return pMapOrigin;
}

