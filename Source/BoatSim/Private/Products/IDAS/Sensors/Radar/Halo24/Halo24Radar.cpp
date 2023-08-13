// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/Halo24Radar.h"
#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#include <Lib/Utils/CUtil.h>
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
		next_state = EHalo24StateMachineStates::UnlockStart;
		break;
	case UnlockStart:
		next_state = EHalo24StateMachineStates::WaitUnlock;
		break;
	case WaitUnlock:
		if (IsKeysVerified) {
			next_state = EHalo24StateMachineStates::Unlocked;
		}
		break;
	case Unlocked:
		pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::UnlockKeys, Serial, true);
		next_state = EHalo24StateMachineStates::WaitImageStreamConnect;
		break;
	case WaitImageStreamConnect:
		break;
	case WaitPoweredOn:
		break;
	case WaitScanedOn:
		break;
	default:
		break;
	}
	Halo24StateMachine = next_state;
}

void AHalo24Radar::OnRecievedMessage(SRadarSimSDKPacket* p_pack)
{
	if (p_pack->Header.PacketType == ESimSDKDataIDS::UnlockKeys) {
		SUnlockKeysPayload* p_keys = (SUnlockKeysPayload*)p_pack->Payload;
		ValidateKeys(p_keys->UnlockKey, p_keys->UnlockKeyLen);
	}

	if (p_pack->Header.PacketType == ESimSDKDataIDS::ConnectRadar) {
		SConnectRadar* p_connect_args = (SConnectRadar*)p_pack->Payload;
		if (strcmp((char*)p_connect_args->SerialData.SerialKey, Serial) == 0) {
			IsImageStreamConnected[p_connect_args->ImageStreamNo] = true;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::ConnectRadar, Serial, true, p_connect_args->ImageStreamNo);
		}
	}

	if (p_pack->Header.PacketType == ESimSDKDataIDS::DisconnectRadar) {
		SConnectRadar* p_connect_args = (SConnectRadar*)p_pack->Payload;
		if (strcmp((char*)p_connect_args->SerialData.SerialKey, Serial) == 0) {
			IsImageStreamConnected[p_connect_args->ImageStreamNo] = false;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::DisconnectRadar, Serial, true);
		}
	}

	if (p_pack->Header.PacketType == ESimSDKDataIDS::PowerControl) {
		SPowerControl* p_connect_args = (SPowerControl*)p_pack->Payload;
		if (strcmp((char*)p_connect_args->SerialData.SerialKey, Serial) == 0) {

			if (p_connect_args->PowerOn) {
				IsPoweredOn = true;
			}
			else {
				IsPoweredOn = false;
			}

			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::PowerControl, Serial, true);
		}
	}
}

void AHalo24Radar::SendSerial()
{
	char ret[128] = { 0 };
	CUtil::FStringToAsciiChar(RadarSerial, ret, sizeof(ret));
	pHalo24CommIF->SendSerial((INT8U*)ret, strlen(ret));
}

void AHalo24Radar::InitSensor()
{
	Super::InitSensor();
	pHalo24CommIF = (UHalo24CommIF*)pCommIF;
	pHalo24CommIF->SetHostIF(this);
}

void AHalo24Radar::ValidateKeys(INT8U* p_keys, INT8U key_count)
{
	if (RadarUnlockKey.Len() == key_count) {
		FString str = CUtil::CharToFString((const char*)p_keys);

		if (str == RadarUnlockKey) {
			IsKeysVerified = true;
			return;
		}
	}

	IsKeysVerified = false;
}


