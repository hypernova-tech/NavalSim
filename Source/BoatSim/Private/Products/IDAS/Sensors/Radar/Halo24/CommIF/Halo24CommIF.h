// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Types/Primitives.h>
#include <Networking.h>
#include <Lib/Utils/CScanResult.h>
#include <Lib/UDP/UdpConnection.h>
#include <Products/IDAS/Sensors/Radar/Halo24/IHalo24HostIF.h>
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


	void SetHostIF(IHalo24HostIF* p_val);
	void SendSerial(INT8U* p_data, INT32U count);
	SRadarSimSDKPacket* GetPacket();
	void SendResponseAckNack(ESimSDKDataIDS id, char *p_serial, bool is_ack, INT8U stream_no = 0);

private:

	bool HasNewData;
	FRunnableThread* SenderThread;
	bool IsStoped = false;


	virtual void Stop() override;

	TArray< SScanResult*> CurrentRequest;
	FPlatformNamedPipe* pPipeInstance;

	IHalo24HostIF* pHostIF;


protected:
	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;



	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;

	TArray<SRadarSimSDKPacket*> Packets;

	
	void RestorePacket(SRadarSimSDKPacket* p_pack);
	
};
