// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lib/UDP/IConnectionDataReceiver.h"
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
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void SendData(void* p_data, uint32 size_in_bytes);
	virtual void SendConfig();
	
		
};
