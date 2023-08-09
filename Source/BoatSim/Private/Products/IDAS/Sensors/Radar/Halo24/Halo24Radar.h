// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "CommIF/Halo24CommIF.h"
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
	PoweredOn,
	ScanedOn,


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
	virtual void OnRecievedCommand(SSIMSDKCommands* p_commands) override;

	void SendSerial();
	UHalo24CommIF* pHalo24CommIF;

	virtual void InitSensor() override;
	
};
