// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/UDP/UdpConnection.h>
#include "UDPBaseCommIF.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UUDPBaseCommIF : public UGenericCommIF
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;
	
};
