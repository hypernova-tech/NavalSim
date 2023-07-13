// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/EthernetDataProcessor/EthernetDataProcessorBase.h"

// Sets default values for this component's properties
UEthernetDataProcessorBase::UEthernetDataProcessorBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEthernetDataProcessorBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEthernetDataProcessorBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEthernetDataProcessorBase::OnReceivedData(const TArray<uint8>& data, int32 bytes_read)
{
}

