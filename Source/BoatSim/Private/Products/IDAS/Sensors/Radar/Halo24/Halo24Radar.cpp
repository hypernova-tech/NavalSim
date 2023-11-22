// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/Halo24Radar.h"
#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#include <Lib/Utils/CUtil.h>
#include <Products/IDAS/Sensors/Radar/Halo24/Tracker/Halo24RadarTracker.h>

void AHalo24Radar::BeginPlay()
{
	Super::BeginPlay();
	


}

void AHalo24Radar::OnDataReady()
{
	Super::OnDataReady();
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
		LastStateSendTimeSec = FApp::GetCurrentTime() + 0.2f;
		break;
	case EHalo24StateMachineStates::WaitImageStreamConnect:
		if (IsImageStreamConnected[0] || IsImageStreamConnected[1]) {
			next_state = EHalo24StateMachineStates::WaitPoweredOn;
		}
		break;
	case EHalo24StateMachineStates::WaitPoweredOn:
		SetScanEnabled(false);
		if (IsPoweredOn) {
			next_state = EHalo24StateMachineStates::WaitTransmit;
			SetRadarState(ERadarState::eStandby);
		}
		break;
	case EHalo24StateMachineStates::WaitTransmit:
		if (IsTransmitOn) {
			SetRadarState(ERadarState::eTransmit);
			SetScanEnabled(true);
			next_state = EHalo24StateMachineStates::Transmitting;
		}
		break;
	case EHalo24StateMachineStates::Transmitting:
		if (!IsTransmitOn) {
			SetScanEnabled(true);
			next_state = EHalo24StateMachineStates::WaitTransmit;
		}
		if (!IsPoweredOn) {
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


	RadarSetupData.range_dm = RangeMaxMeter * 0.1f;               ///< Currently selected range (in 10ths of a metre)
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
void AHalo24Radar::HandleGuardZoneControl(SGuardZoneData* p_data)
{
	if (p_data->ZoneControl.IsValid) {
		GuardZone.SetZoneState(p_data->ZoneControl.Zone, p_data->ZoneControl.State ? EZoneState::Active : EZoneState::Inactive);
	}

	if (p_data->ZoneSetup.IsValid) {
		GuardZone.SetArea(p_data->ZoneSetup.Zone, p_data->ZoneSetup.BearingDeg, p_data->ZoneSetup.BearingDeg + p_data->ZoneSetup.BeaeingWidthDeg);
	}

	if (p_data->ZoneAlarmSetup.IsValid) {
		GuardZone.SetZoneAlarm(p_data->ZoneAlarmSetup.Zone, (EZoneAlarm)p_data->ZoneAlarmSetup.ZoneAlarmType);
	}
	if (p_data->ZoneAlarmCancel.IsValid) {
		GuardZone.CancelZoneAlarm(p_data->ZoneAlarmSetup.Zone);
	}

	if (p_data->ZoneAlarmSuppress.IsValid) {
		GuardZone.CancelZoneAlarm(p_data->ZoneAlarmSetup.Zone);
	}

	if (p_data->AlarmSensitivty.IsValid) {
		GuardZone.SetZoneSensitivity(p_data->AlarmSensitivty.Level / 255.0f);
	}
}

void AHalo24Radar::HandleSectorBlankingData(SSectorBlankingData* p_data)
{
	if (p_data->SectorControl.IsValid) {
		BlankingZone.SetZoneState(p_data->SectorControl.SectorId, p_data->SectorControl.State ? EZoneState::Active : EZoneState::Inactive);
	}

	if (p_data->SectorSetup.IsValid) {
		BlankingZone.SetArea(p_data->SectorSetup.SectorId, p_data->SectorSetup.StartBearingDeg, p_data->SectorSetup.EndBearingDeg);
	}
}

bool AHalo24Radar::VerifySerial(char* p_inp)
{
	return strcmp(p_inp, Serial) == 0;
}

void AHalo24Radar::UpdateTracker()
{
	if (!UseSimulationDataAsOwnShip) {
		FRotator rot(0, -(360 - (INT32S)LastOwnshipData.DirectionDeg), 0);
		FVector vel = rot.RotateVector(FVector::ForwardVector) * LastOwnshipData.SpeedMetersPerSec;
		pTracker->SetOwnshipData(this, GetActorLocation(), CUtil::GetActorRPY(this), vel, GetRangeMeter(), NoiseMean, NoiseStdDeviation);
	}
	
	Super::UpdateTracker();
	STargetTrackStatusData track_status = { 0 };


	track_status.Tracks = pTracker->GetTrackedObjects();
	track_status.ClosestPointOfApproachMeters = pTracker->GetCPAMeters();
	track_status.TimeToClosestPointOfApproachSec = pTracker->GetCPATimeSec();
	track_status.TowardsCPA = pTracker->GetIsTowardsCPA();

	pHalo24CommIF->SendTrackedObjects(track_status,Serial);

	
}


char* AHalo24Radar::GetSerial()
{
	return Serial;
}
void AHalo24Radar::Save(ISaveLoader* p_save_load)
{
	Super::Save(p_save_load);
	//p_save_load->AddLine(line);
}
void AHalo24Radar::OnRecievedMessage(SRadarSimSDKPacket* p_pack)
{
	if (p_pack->Header.PacketType == ESimSDKDataIDS::UnlockKeys) {
		SUnlockKeysPayload* p_keys = (SUnlockKeysPayload*)p_pack->Payload;
		ValidateKeys(p_keys->UnlockKey, p_keys->UnlockKeyLen);
	}

	else if (p_pack->Header.PacketType == ESimSDKDataIDS::ConnectRadar) {
		SConnectRadar* p_connect_args = (SConnectRadar*)p_pack->Payload;
		if (strcmp((char*)p_connect_args->SerialData.SerialKey, Serial) == 0) {
			IsImageStreamConnected[p_connect_args->ImageStreamNo] = true;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::ConnectRadar, Serial, true, p_connect_args->ImageStreamNo);
		}
	}

	else if (p_pack->Header.PacketType == ESimSDKDataIDS::DisconnectRadar) {
		SConnectRadar* p_connect_args = (SConnectRadar*)p_pack->Payload;
		if (strcmp((char*)p_connect_args->SerialData.SerialKey, Serial) == 0) {
			IsImageStreamConnected[p_connect_args->ImageStreamNo] = false;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::DisconnectRadar, Serial, true);
		}
	}

	else if (p_pack->Header.PacketType == ESimSDKDataIDS::PowerControl) {
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

	else if (p_pack->Header.PacketType == ESimSDKDataIDS::TransmitControl) {
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
	
			RangeMaxMeter = p_connect_args->RangeMeter;
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
			HandleGuardZoneControl(&p_args->GuardZoneData);
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::GuardZoneControl, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::SectorBlanking) {
		SSectorBlankingPayload* p_args = (SSectorBlankingPayload*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {
			HandleSectorBlankingData(&p_args->BlankingData);
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::SectorBlanking, Serial, true);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::ConnectTrackingClient) {
		SConnectTrackingClientPayload* p_args = (SConnectTrackingClientPayload*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {
			IsTrackingClientStreamConnected[p_args->ImageStreamNo] = true;
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::ConnectTrackingClient, Serial, true, p_args->ImageStreamNo);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::TrackingAcquire) {
		STrackingAcquireTarget* p_args = (STrackingAcquireTarget*)p_pack->Payload;
		if (strcmp((char*)p_args->SerialData.SerialKey, Serial) == 0) {
			bool ret = pTracker->TryTrack(p_args->Id, GetActorLocation(), 360-(INT32S)p_args->BearingDeg, p_args->RangeMeter);
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::TrackingAcquire, Serial, ret);
		}
	}
	else	if (p_pack->Header.PacketType == ESimSDKDataIDS::TrackingCancel) {
		STrackingCancelTarget* p_args = (STrackingCancelTarget*)p_pack->Payload;
		if (VerifySerial((char*)p_args->SerialData.SerialKey)) {
			bool ret = false;

			if (p_args->CancelAll) {
				ret = pTracker->CancelAll();
			}
			else {
					ret = pTracker->CancelTrack(p_args->Id);
			}
			
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::TrackingCancel, Serial, ret);
		}
	}
	else if (p_pack->Header.PacketType == ESimSDKDataIDS::TrackingOwnshipData) {
		STrackingOwnShipNavigation* p_args = (STrackingOwnShipNavigation*)p_pack->Payload;
		if (VerifySerial((char*)p_args->SerialData.SerialKey)) {
			memcpy(&LastOwnshipData, p_args, sizeof(LastOwnshipData));
			pHalo24CommIF->SendResponseAckNack(ESimSDKDataIDS::TrackingOwnshipData, Serial, true);
			SetTrackerEnabled(true);
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

void AHalo24Radar::InitTracker()
{

	pTracker = new CHalo24RadarTracker();

}

void AHalo24Radar::ValidateKeys(INT8U* p_keys, INT8U key_count)
{

	//aoutCUtil::HexStringToByteArray(RadarUnlockKey)
	if (RadarUnlockKey.Len()/2 == key_count) {
		FString str = CUtil::CharToHexString(p_keys, key_count);

		if (str == RadarUnlockKey) {
			IsKeysVerified = true;
			return;
		}
	}

	IsKeysVerified = false;
}


