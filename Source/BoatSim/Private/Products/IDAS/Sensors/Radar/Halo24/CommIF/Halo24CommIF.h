// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Types/Primitives.h>
#include <Networking.h>
#include <Lib/Utils/CScanResult.h>
#include "Halo24CommIF.generated.h"



/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UHalo24CommIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SendRadarTrack();

public:
	~UHalo24CommIF();
	virtual void SendData(void* p_data, uint32 size_in_bytes) override;
	

	// Inherited via FRunnable
	virtual uint32 Run() override;


	

	

private:

	bool HasNewData;
	FRunnableThread* SenderThread;
	bool IsStoped = false;

	virtual void Stop() override;

	TArray< SScanResult*> CurrentRequest;
	FPlatformNamedPipe* pPipeInstance;
	
};
