// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/Halo24Radar.h"
#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#
void AHalo24Radar::BeginPlay()
{
	Super::BeginPlay();


}

void AHalo24Radar::RadarStateMachine()
{
	Super::RadarStateMachine();
	EHalo24StateMachineStates curr_state = Halo24StateMachine;
	EHalo24StateMachineStates next_state = curr_state;

	switch (curr_state)
	{
	case SendSerialKeys:
		SendSerial();
		break;
	case UnlockStart:
		break;
	case WaitUnlock:
		break;
	case Unlocked:
		break;
	case PoweredOn:
		break;
	case ScanedOn:
		break;
	default:
		break;
	}
	Halo24StateMachine = next_state;
}

void AHalo24Radar::OnRecievedCommand(SSIMSDKCommands* p_commands)
{

}

void AHalo24Radar::SendSerial()
{
	pHalo24CommIF->SendSerial((INT8U*)*RadarSerial, RadarSerial.Len());
}

void AHalo24Radar::InitSensor()
{
	Super::InitSensor();
	pHalo24CommIF = (UHalo24CommIF*)pCommIF;
	pHalo24CommIF->SetHostIF(this);
}
