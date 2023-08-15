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
	case EHalo24StateMachineStates::SendSerialKeys:
		SendSerial();
		next_state = EHalo24StateMachineStates::UnlockStart;
		break;
	case EHalo24StateMachineStates::UnlockStart:
		next_state = EHalo24StateMachineStates::WaitUnlock;
		SetRadarState(ERadarState::eTransmit);
		break;
	case EHalo24StateMachineStates::WaitUnlock:
		if (IsKeysVerified) {
			next_state = EHalo24StateMachineStates::Unlocked;
		}
		break;
	case EHalo24StateMachineStates::Unlocked:
		pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::UnlockKeys, Serial, true);
		next_state = EHalo24StateMachineStates::WaitImageStreamConnect;
		CanUpdateState = true;
		break;
	case EHalo24StateMachineStates::WaitImageStreamConnect:
		if (IsImageStreamConnected[0] || IsImageStreamConnected[1]) {
			next_state = EHalo24StateMachineStates::WaitPoweredOn;
		}
		break;
	case EHalo24StateMachineStates::WaitPoweredOn:
		if (IsPoweredOn) {
			next_state = EHalo24StateMachineStates::WaitTransmit;
			SetRadarState(ERadarState::eStandby);
		}
		break;
	case EHalo24StateMachineStates::WaitTransmit:
		if (IsTransmitOn) {
			SetRadarState(ERadarState::eTransmit);
			next_state = EHalo24StateMachineStates::Transmitting;
		}
		break;
	case EHalo24StateMachineStates::Transmitting:
		if (IsTransmitOn) {
			next_state = EHalo24StateMachineStates::WaitTransmit;
		}
		if (IsPoweredOn) {
			next_state = EHalo24StateMachineStates::WaitPoweredOn;
		}
		break;
	default:
		break;
	}
	Halo24StateMachine = next_state;
	UpdateState();
}


void AHalo24Radar::UpdateState()
{
	if (CanUpdateState) {
		if ((FApp::GetCurrentTime() - LastStateSendTimeSec) > StateUpdatePeriodSec) {
			pHalo24CommIF->SendRadarState(RadarState, Serial);
			pHalo24CommIF->SendRadarSetup(RadarSetupData, Serial);
			LastStateSendTimeSec = FApp::GetCurrentTime();
		}
		
	}
	

}
void AHalo24Radar::SetRadarState(ERadarState val)
{
	RadarState = val;
}

void AHalo24Radar::UpdateSetupData()
{
	RadarSetupData.range_dm =  //decimeter
	RadarSetupData.pwType = 0;


	RadarSetupData.range_dm = RangeMeter.Y * 0.1f;               ///< Currently selected range (in 10ths of a metre)
	RadarSetupData.useMode = RadarState;               ///< Use mode
	
#if false
	RadarSetupData.gain[3];
	RadarSetupData.   ftc;                    ///< Fast-Time-Constant level
	RadarSetupData.tuneType;               ///< Pulse radar only
	RadarSetupData.coarseTune;             ///< Pulse radar only
	RadarSetupData.fineTune;               ///< Pulse radar only
	RadarSetupData.interferenceReject;     ///< Interference-Reject level (off 0-3 high)
	RadarSetupData.targetStretch;          ///< Target-Stretch level 
	RadarSetupData.targetBoost;            ///< Target-Boost, AKA target-emphasis (off 0-2 high)
	RadarSetupData.pwType;					///< Pulse width length type enumeration
	RadarSetupData.pwLength_ns;            ///< Pulse width length in nanoseconds
	RadarSetupData.guardzones;             ///< Guard-Zone setup

#endif

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

	if (p_pack->Header.PacketType == ESimSDKDataIDS::TransmitControl) {
		STransmitControl* p_connect_args = (STransmitControl*)p_pack->Payload;
		if (strcmp((char*)p_connect_args->SerialData.SerialKey, Serial) == 0) {
			if (p_connect_args->TransmitOn) {
				IsTransmitOn = true;
			}
			else {
				IsTransmitOn = false;
			}
			
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::TransmitControl, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::GainControl) {
		SGainMode* p_args = (SGainMode*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {

			GainType = p_args->Type;
			GainLevel = p_args->Level;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::GainControl, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::RangeControl) {
		SRangeControl* p_connect_args = (SRangeControl*)p_pack->Payload;
		if (strcmp((char*)p_connect_args->SerialData.SerialKey, Serial) == 0) {
	
			RangeMeter.Y = p_connect_args->RangeMeter;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::RangeControl, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::SeaClutterControl) {
		SSeaClutter* p_args = (SSeaClutter*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {

			SeaClutterType = p_args->SeaClutterType;
			SeaClutterLevel = p_args->SeaClutterLevel;
			SeaClutterAutoOffset = p_args->SeaClutterAutoOffset;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::SeaClutterControl, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::RainClutterControl) {
		SRainClutter* p_args = (SRainClutter*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {

			RainClutterLevel = p_args->Level;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::RainClutterControl, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::FastScanMode) {
		SFastScanMode* p_args = (SFastScanMode*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {
			 
			FastScanLevel = p_args->Level;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::FastScanMode, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::ScannerRPM) {
		SScanerRPM* p_args = (SScanerRPM*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {

			ScannerRPMValue = p_args->Rpm;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::ScannerRPM, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::GuardZoneControl) {
		SGuardZonePayload* p_args = (SGuardZonePayload*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {

			memcpy(&GuardZoneData, &p_args->GuardZoneData, sizeof(SGuardZoneData));
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::GuardZoneControl, Serial, true);
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


