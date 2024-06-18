// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Utils/CScanResult.h>
#include <Lib/UDP/UdpConnection.h>
#include "GenericLidarCommIF.generated.h"

/**
 * 
 */

 /**
  *
  */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGenericLidarCommIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SendMainStreamOutputPacket();


public:
	~UGenericLidarCommIF();
	virtual void SendData(void* p_data, uint32 size_in_bytes) override;

	// Inherited via FRunnable
	virtual uint32 Run() override;


	SScanResult* pLastScanResult;


	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;

protected:

	bool HasNewData;
	TArray< SScanResult*> CurrentRequest;


	FRunnableThread* SenderThread;
	bool IsStoped = false;

	virtual void Stop() override;
	int PointCloudCaptureInd = 0;

	void RenderPointCloud(const TArray<FVector>& pts);

};

