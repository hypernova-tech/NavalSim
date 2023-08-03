// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"

// Sets default values for this component's properties
UGenericCommIF::UGenericCommIF()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGenericCommIF::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGenericCommIF::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGenericCommIF::SendData(void* p_data, uint32 size_in_bytes)
{
}

void UGenericCommIF::SendConfig()
{
}

