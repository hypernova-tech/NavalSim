// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericCommIF/GenericCommIF.h"
#include <Lib/Types/Primitives.h>
#include <Networking.h>
#include <Lib/Utils/CScanResult.h>
#include <Lib/UDP/UdpConnection.h>
#include <Products/IDAS/Sensors/Radar/Halo24/IHalo24HostIF.h>
#include <Lib/Sensor/GenericRadarCommProtocolF/GenericRadarCommProtocolIF.h>
#include "Lib/Tracker/TrackerBase.h"
#include "Halo24CommIF.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UHalo24CommIF : public UGenericRadarCommProtocolIF
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SendRadarTrack() override;
	IRadarHostIF* pRadarHostIF;
public:

	// Inherited via FRunnable
	void SendSerial(INT8U* p_data, INT32U count);
	SRadarSimSDKPacket* GetPacket();
	void SendResponseAckNack(ESimSDKDataIDS id, char *p_serial, bool is_ack, INT8U stream_no = 0);
	void SendRadarState(ERadarState radar_state, char* p_serial);
	void SendRadarSetup(const SRadarSetupData& setup, char* p_serial);
	virtual void SetHostIF(IHostIF* p_host_if) override;
	
private:


protected:

	TArray<SRadarSimSDKPacket*> Packets;

	void RestorePacket(SRadarSimSDKPacket* p_pack);
	int SpokeSequanceNumber = 0;
	int PrevSequenceNumber = -1;
	char SerialKey[32];
	virtual void OnReceivedConnectionData(void* connection, INT8U* p_data, INT32U count) override;

	
};
