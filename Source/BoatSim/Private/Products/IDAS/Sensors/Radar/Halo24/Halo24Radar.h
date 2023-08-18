// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "CommIF/Halo24CommIF.h"
#include "Halo24Types.h"
#include "Halo24Radar.generated.h"

enum EHalo24States
{
	EHalo24PowerOff,
	EHalo24DetectScanner,
	EHalo24WarmingUp,
	EHalo24Standby,
	EHalo24Transmit,
	EHalo24NoScanner
};

enum EHalo24StateMachineStates
{
	SendSerialKeys,
	UnlockStart,
	WaitUnlock,
	Unlocked,
	WaitImageStreamConnect,
	WaitPoweredOn,
	WaitTransmit,
	Transmitting,


};



/**
 * 
 */
UCLASS()
class AHalo24Radar : public ARadarBase, public IHalo24HostIF
{
	GENERATED_BODY()
	

protected:



	virtual void BeginPlay() override;
	EHalo24StateMachineStates Halo24StateMachine;
	EHalo24States Halo24State;


	virtual void RadarStateMachine() override;
	

	void SendSerial();
	UHalo24CommIF* pHalo24CommIF;

	virtual void InitSensor() override;
	virtual void InitTracker() override;

	void ValidateKeys(INT8U *p_keys, INT8U key_count);
	bool CanUpdateState = false;
	FLOAT32 StateUpdatePeriodSec = 0.1f;
	FLOAT32 LastStateSendTimeSec = 0;
	void UpdateState();
	bool IsKeysVerified = false;
	bool IsImageStreamConnected[2] = { false };
	bool IsPoweredOn = false;
	bool IsTransmitOn = false;
	bool IsTrackingClientStreamConnected[2] = { false };
	ERadarState RadarState = ERadarState::eOff;
	SRadarSetupData RadarSetupData;
	SGuardZoneData GuardZoneData;
	STrackingOwnShipNavigation LastOwnshipData;

	void SetRadarState(ERadarState val);
	void UpdateSetupData();
	void HandleGuardZoneControl(SGuardZoneData* p_data);
	void HandleSectorBlankingData(SSectorBlankingData* p_data);
	bool VerifySerial(char* p_inp);
	virtual void UpdateTracker() override;


public :

	virtual void OnRecievedMessage(SRadarSimSDKPacket* p_commands) override;
	virtual char* GetSerial() override;

	
	
};
