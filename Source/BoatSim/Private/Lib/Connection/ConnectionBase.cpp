// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Connection/ConnectionBase.h"

// Sets default values for this component's properties
UConnectionBase::UConnectionBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UConnectionBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UConnectionBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UConnectionBase::InitConnection()
{
	IsRunning = true;
}

void UConnectionBase::AddConnectionDataReceiver(IConnectionDataReceiver* p_rcv)
{
	ConnectionDataReceivers.Add(p_rcv);
}

bool UConnectionBase::GetIsRunning()
{
	return IsRunning;
}

void UConnectionBase::OnModifiedConnection(SConnectionInfo info)
{
}
