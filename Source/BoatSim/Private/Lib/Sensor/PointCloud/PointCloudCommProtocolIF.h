// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Utils/CScanResult.h>
#include <Lib/UDP/UdpConnection.h>
#include "PointCloudCommProtocolIF.generated.h"



/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UPointCloudCommProtocolIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()

public:
	// Inherited via FRunnable
	virtual ~UPointCloudCommProtocolIF();
	virtual uint32 Run() override;
	virtual void Stop() override;

protected:
	FCriticalSection Mutex;
	TArray< SScanResult*> CurrentRequest;
	FRunnableThread* pThread;
	bool IsStoped = false;
	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;
	virtual void BeginPlay() override;
	virtual void SendData(void* p_data, uint32 size_in_bytes) override;
	void SendPointCloud();
	
};
