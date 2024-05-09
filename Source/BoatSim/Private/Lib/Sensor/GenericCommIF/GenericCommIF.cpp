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
void UGenericCommIF::SendData(void* p_hdr, uint32 hdr_len, void* p_data, uint32 size_in_bytes)
{

}

void UGenericCommIF::SendConfig()
{
}

TArray<SConnectionEntry> UGenericCommIF::GetConnectionsInfo()
{
	return Connections;
}

bool UGenericCommIF::SetConnectionInfo(INT32S ind, SConnectionInfo info)
{
	if (ind < Connections.Num()) {
		Connections[ind].ConnectionInfo = info;
		((UConnectionBase*)Connections[ind].pOwner)->OnModifiedConnection(info);
	}
	
	return true;
}

void UGenericCommIF::RegisterConnectionInfo(INT32S ind, SConnectionInfo info, UConnectionBase *p_owner)
{
	SConnectionEntry entry;
	entry.ConnectionInfo = info;
	entry.pOwner = p_owner;
	Connections.Add(entry);
}



void UGenericCommIF::OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count)
{

}
