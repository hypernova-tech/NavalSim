// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/SensorBase.h"

// Sets default values
ASensorBase::ASensorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASensorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASensorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

