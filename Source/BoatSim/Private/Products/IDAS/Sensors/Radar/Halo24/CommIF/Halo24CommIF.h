// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Types/Primitives.h>
#include <Networking.h>
#include <Lib/Utils/CScanResult.h>
#include <Lib/UDP/UdpConnection.h>
#include <Products/IDAS/Sensors/Radar/Halo24/IHalo24HostIF.h>
#include "Lib/Tracker/TrackerBase.h"
#include "Halo24CommIF.generated.h"

struct STargetTrackStatusData{
	TArray< STrackedObjectInfo*>* Tracks;
	FLOAT64 ClosestPointOfApproachMeters;
	FLOAT64 TimeToClosestPointOfApproachSec;
	BOOLEAN TowardsCPA; //(0 target moving away from CPA, 1 towards CPA)

};

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
	void SendRadarState(ERadarState radar_state, char* p_serial);
	void SendRadarSetup(const SRadarSetupData& setup, char* p_serial);
	void SendTrackedObjects(const STargetTrackStatusData &info, char* p_serial);

private:

	bool HasNewData;
	FRunnableThread* SenderThread;
	bool IsStoped = false;


	virtual void Stop() override;

	TArray< SScanResult*> CurrentRequest;

	IHalo24HostIF* pHostIF;
	FCriticalSection Mutex;

protected:
	UPROPERTY(BlueprintReadWrite)
		UUdpConnection* pUDPConnection;



	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;

	TArray<SRadarSimSDKPacket*> Packets;

	
	void RestorePacket(SRadarSimSDKPacket* p_pack);
	int SpokeSequanceNumber = 0;
	int PrevSequenceNumber = -1;
	char SerialKey[32];
	
	
};
