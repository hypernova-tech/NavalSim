// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Lib/Connection/ConnectionDefs.h>
#include "Lib/UDP/IConnectionDataReceiver.h"
#include <Lib/Connection/ConnectionBase.h>
#include "GenericCommIF.generated.h"





UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGenericCommIF : public UActorComponent, public IConnectionDataReceiver
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGenericCommIF();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;
	TArray< SConnectionEntry> Connections;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void SendData(void* p_data, uint32 size_in_bytes);
	virtual void SendData(void* p_hdr, uint32 hdr_len, void* p_data, uint32 size_in_bytes);
	virtual void SendConfig();
	
	virtual TArray< SConnectionEntry> GetConnectionsInfo();
	virtual bool SetConnectionInfo(INT32S ind, SConnectionInfo info);
	void RegisterConnectionInfo(INT32S ind, SConnectionInfo info, UConnectionBase *p_owner);

		
};
