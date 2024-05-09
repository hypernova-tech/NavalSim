// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/UDP/UdpConnection.h>
#include "IDASAISSCommIF.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UIDASAISSCommIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite)
		UConnectionBase* pConnection;
	bool IsStoped = false;
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;

	// Inherited via FRunnable
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void SendData(void* p_data, uint32 size_in_bytes) override;
	virtual void SendData(void* p_hdr, uint32 hdr_len, void* p_data, uint32 size_in_bytes) override;
};
