// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/UDP/UdpConnection.h>
#include <Products/IDAS/Sensors/Camera/IMartiHostIF.h>
#include "MartiCommIF.generated.h"



/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UMartiCommIF : public UGenericCommIF, public FRunnable
{
	GENERATED_BODY()
	


protected:

	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;
	bool IsStoped = false;
	IMartiHostIF* pHostIf;

public:
	// Inherited via FRunnable
	virtual uint32 Run() override;
	virtual void Stop() override;

	void SetHostIf(IMartiHostIF* p_val);
};
