// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/IDAS/Sensors/Radar/Halo24/Halo24Radar.h"
#include "Products/IDAS/Sensors/Radar/Halo24/CommIF/Halo24CommIF.h"
#include <Lib/Utils/CUtil.h>
#include <Products/IDAS/Sensors/Radar/Halo24/Tracker/Halo24RadarTracker.h>
#include <Lib/Math/CMath.h>
#include <Lib/SystemManager/SystemManagerBase.h>



#pragma pack(push, 1)
typedef struct SINSData
{
	uint8_t header[3];
	uint8_t  messageType;
	uint8_t  version;
	uint64_t timestampFeedback;
	uint8_t	 initialized;
	uint8_t  confidenceLevel;
	uint8_t  odoMode;

	int32_t  initPoseLattitude;
	int32_t  initPoseLongitude;
	int16_t  initPoseAltitude;

	uint32_t vehicleLatPosFeedback;
	uint32_t vehicleLongPosFeedback;
	int16_t  vehicleAltFeedback;

	// 0.01 degree +/- 180 degree
	int16_t  vehicleHeadingFeedback;
	int16_t  vehicleRollFeedback;
	int16_t  vehiclePitchFeedback;

	// 1 m resolution
	int32_t  vehicleNorthPositionFeedback;
	int32_t  vehicleEastPositionFeedback;
	int32_t  missionSpeed;

	// 0.01 km/h resolution
	int16_t  vehicleSpeed;
	int16_t  vehicleSpeedNorth;
	int16_t  vehicleSpeedEast;
	int16_t  vehicleSpeedDown;
	uint8_t  	checkSum;

	const double VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR = (0.01 * 1e-3 / 3600);
	const double VEHICLE_RPY_SF = (0.01);
	const double VEHICLE_LAT_LON_SF = (1e-7);
	const double VEHICLE_ALT_SF = (0.1);

public:

	void Init()
	{
		header[0] = 0xca;
		header[1] = 0x68;
		header[2] = 0x47;
	}

	void SetMessageType(uint8_t val)
	{


		messageType = val;
	}
	void SetVehicleSpeed(FVector ned_speed_meter_per_sec)
	{
		vehicleSpeed = (int16_t)(ned_speed_meter_per_sec.Length() * VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR);
		vehicleSpeedNorth = (int16_t)(ned_speed_meter_per_sec.X * VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR);
		vehicleSpeedEast = (int16_t)(ned_speed_meter_per_sec.Y * VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR);
		vehicleSpeedDown = (int16_t)(-ned_speed_meter_per_sec.Z * VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR);
	}

	void SetVehicleRPY(FVector rpy_deg)
	{

		vehicleHeadingFeedback = (int16_t)(rpy_deg.Z * VEHICLE_RPY_SF);
		vehicleRollFeedback = (int16_t)(rpy_deg.X * VEHICLE_RPY_SF);
		vehiclePitchFeedback = (int16_t)(rpy_deg.Y * VEHICLE_RPY_SF);
	}

	void SetVehiclePos(FVector llh)
	{
		vehicleLatPosFeedback = (int16_t)(llh.X * VEHICLE_LAT_LON_SF);
		vehicleLongPosFeedback = (int16_t)(llh.Y * VEHICLE_LAT_LON_SF);
		vehicleAltFeedback = (int16_t)(llh.Z * VEHICLE_ALT_SF);
	}

};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	// ValidType AlignComplete;
	// ValidType HybridVelocityValid;
	// ValidType HybridPositionValid;
	// ValidType HybridAttitudeValid;
	uint8_t a;
	uint8_t b;
}HSSI_OUTPUT_DATA_VALIDITY_WORD_Type;
#pragma pack(pop)

#define VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR  (1)
#define VEHICLE_ANGLE_SF  (DEGTORAD)
#define VEHICLE_RPY_SF  (0.01)
#define VEHICLE_LAT_LON_SF  (DEGTORAD)
#define VEHICLE_ALT_SF  (0.1)

#pragma pack(push, 1)
typedef struct SANSStabilizationDataMessage
{
	uint8_t xca;
	uint8_t x68;
	uint8_t x47;
	uint8_t hcs;//header checksum

	uint64_t SystemTimer;

	HSSI_OUTPUT_DATA_VALIDITY_WORD_Type HSSI_outputDataValidityWord;

	uint32_t HybridHeadingAngle;
	uint32_t HybridPitchAngle;
	uint32_t HybridRollAngle;
	uint32_t HybridYawAngle;
	uint32_t HybridYawRate;
	uint32_t HybridPitchRate;
	uint32_t HybridRollRate;

	uint64_t HybridLatitude;
	uint64_t HybridLongitude;
	uint32_t HybridAltitudeMSL;

	uint32_t HybridNorthVelocity;
	uint32_t HybridEastVelocity;
	uint32_t HybridVerticalVelocity;
	//FOM_Type HybridFOM;
	uint8_t HybridFOM;
	uint8_t cs;//message checksum


	

public:

	void Init()
	{
		xca = 0xca;
		x68 = 0x68;
		x47 = 0x47;
	}

	void SetVehicleSpeed(FVector ned_speed_meter_per_sec)
	{

		float val;
		uint32_t uval;

		val = ned_speed_meter_per_sec.X * VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR;
	    uval  = *(uint32_t*)&(val);
		CUtil::ReverseCopyBytes((INT8U*) & uval, (INT8U*)&HybridNorthVelocity, 4);

		val = ned_speed_meter_per_sec.Y * VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR;
		uval = *(uint32_t*)&(val);
		CUtil::ReverseCopyBytes((INT8U*)&uval, (INT8U*)&HybridEastVelocity, 4);


		val = -ned_speed_meter_per_sec.Z * VEHICLE_SPEED_METERS_PER_SEC_TO_KM_PER_HR;
		uval = *(uint32_t*)&(val);
		CUtil::ReverseCopyBytes((INT8U*)&uval, (INT8U*)&HybridVerticalVelocity, 4);

	}

	void SetVehicleRPY(FVector rpy_deg)
	{
		float val;

		uint32_t uval;

		val = rpy_deg.Z * VEHICLE_ANGLE_SF;
		uval = *(uint32_t*)&(val);
		CUtil::ReverseCopyBytes((INT8U*)&uval, (INT8U*)&HybridYawAngle, 4);

		val = rpy_deg.X * VEHICLE_ANGLE_SF;
		uval = *(uint32_t*)&(val);
		CUtil::ReverseCopyBytes((INT8U*)&uval, (INT8U*)&HybridRollAngle, 4);

		val = rpy_deg.Y * VEHICLE_ANGLE_SF;
		uval = *(uint32_t*)&(val);
		CUtil::ReverseCopyBytes((INT8U*)&uval, (INT8U*)&HybridPitchAngle, 4);


		HybridHeadingAngle = HybridYawAngle;

	}

	void SetVehicleLocation(FVector llh);
};
#pragma pack(pop)

void SANSStabilizationDataMessage::SetVehicleLocation(FVector llh)
{
	double val;
	INT64U u64val;
	INT32U u32val;
	float fval;

	val = llh.X * VEHICLE_LAT_LON_SF;
	u64val = *(INT64U*)&(val);
	CUtil::ReverseCopyBytes((INT8U*)&u64val, (INT8U*)&HybridLatitude, 8);


	val = llh.Y * VEHICLE_LAT_LON_SF;
	u64val = *(INT64U*)&(val);
	CUtil::ReverseCopyBytes((INT8U*)&u64val, (INT8U*)&HybridLongitude, 8);

	fval = llh.Z * VEHICLE_ALT_SF;
	u32val = *(uint32_t*)&(fval);
	CUtil::ReverseCopyBytes((INT8U*)&u32val, (INT8U*)&HybridAltitudeMSL, 4);



	
}

void AHalo24Radar::BeginPlay()
{
	Super::BeginPlay();
	


}

void AHalo24Radar::OnDataReady()
{
	Super::OnDataReady();
	SendINSData();
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
		LastStateSendTimeSec = FPlatformTime::Seconds() + 0.2f;
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


	case  EHalo24StateMachineStates::StandaloneModeEnter:
		SetScanEnabled(true);
		next_state = EHalo24StateMachineStates::StandaloneMode;
		break;
	case EHalo24StateMachineStates::StandaloneMode:
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
		if ((FPlatformTime::Seconds() - LastStateSendTimeSec) > StateUpdatePeriodSec) {
			pHalo24CommIF->SendRadarState(RadarState, Serial);
			pHalo24CommIF->SendRadarSetup(RadarSetupData, Serial);
			LastStateSendTimeSec = FPlatformTime::Seconds();
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
		FRotator rot(0, (LastOwnshipData.DirectionDeg), 0);
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

void AHalo24Radar::SendINSData()
{
	SANSStabilizationDataMessage ins_data;

	auto rolloff = offsetof(SANSStabilizationDataMessage, HybridRollAngle);

	memset(&ins_data, 0, sizeof(SANSStabilizationDataMessage));
	ins_data.Init();

	auto actor_pos = GetPositionLatLongHeightMSL();
	ins_data.SetVehicleLocation(actor_pos);

	auto rpy_deg = CUtil::GetActorRPY(this);
	rpy_deg = CMath::FixAngPlusMinus180(rpy_deg);
	ins_data.SetVehicleRPY(rpy_deg);

	auto vel = GetActorVelocityMetersPerSec();
	ins_data.SetVehicleSpeed(vel);

	pINSConnection->SendData((const INT8U*) & ins_data, sizeof(SANSStabilizationDataMessage));
}

/// <summary>
/// halo24 radar works in full scan mode
/// </summary>
void AHalo24Radar::Scan()
{
	if (!IsAutoScanEnabled) {
		return;
	}
	bool is_reset = false;

	if (CTime::GetTimeSecond() >= NextScanTime) {

		if (IsFullScaned) {

			CurrentScanAzimuth = 0;
			IsFullScaned = false;
			pScanResult = ScanResultContainer.GetCircular();

		}

		FLOAT64 start_azimuth = CurrentScanAzimuth;
		FLOAT64 end_azimuth = EachScanBeamWidthDeg + start_azimuth - HorizontalScanStepAngleDeg;

		if (end_azimuth >= FovHorizontalDeg) {
			end_azimuth = FovHorizontalDeg;
		}


		STraceArgs args;

		FLOAT64 elev = CMath::GetActorEulerAnglesRPY(this).Y;

		args.p_actor = this;
		args.is_world = true;
		args.is_normal_distribution = IsNormalDistribution;
		args.range_meter = RangeMaxMeter;
		args.min_range_meter = RangeMinMeter;
		args.azimuth_start_deg = start_azimuth;
		args.azimuth_end_deg = end_azimuth;
		args.elevation_start_deg = elev - FovVerticalDeg;
		args.elevation_end_deg = elev;
		args.azimuth_angle_step_deg = HorizontalScanStepAngleDeg;
		args.elevation_angle_step_deg = VerticalScanStepAngleDeg;
		args.horizontal_fov_deg = FovHorizontalDeg;
		args.vertical_fov_deg = FovVerticalDeg;
		args.measurement_error_mean = MeasurementErrorMean;
		args.measurement_error_std = MeasurementErrorUncertainy * MeasurementErrorUncertainyScale;
		args.clutter_params = GetClutterParams();
		args.show_beam = ShowBeam;
		args.p_ignore_list = &(ASystemManagerBase::GetInstance()->GetSensorGlobalIgnoreList());
		args.create_scan_line = true;
		args.scan_center = GetActorLocation();
		auto rotator = GetActorRotation();
		args.scan_rpy_world_deg = FVector(rotator.Roll, -rotator.Pitch, -rotator.Yaw);
		auto* p_parent = CUtil::GetParentActor(this);
		if (p_parent != nullptr) {
			args.additional_ignore_list.Add(p_parent);
		}
		args.use_render_target = UseRenderTargetForDepthCalculation;
		args.include_actor_list = GetScanAdditionalIncludes();

		if (!BlankingZone.CheckAnyActiveZone(start_azimuth, end_azimuth)) {
			//bool ret = CUtil::Trace(this, true, RangeMeter.X, RangeMeter.Y, start_azimuth, end_azimuth, 0, FovVerticalDeg, HorizontalScanStepAngleDeg, VerticalScanStepAngleDeg, 
			//						MeasurementErrorMean, MeasurementErrorUncertainy, GetClutterParams(), ShowBeam, 
			//						ASystemManagerBase::GetInstance()->GetSensorGlobalIgnoreList(), true, pScanResult);

			if (args.use_render_target) {
				auto capture_start_sec = CUtil::Tick();
				pSceneCapturer->Capture();
				auto capture_elp_sec = CUtil::Tock(capture_start_sec);

				CaptureStartTimeRef = CUtil::Tick();
				pSceneCapturer->ReadPixels();
			}


			auto trace_start_sec = CUtil::Tick();
			bool ret = CUtil::Trace(args, pScanResult);
			auto trace_elp_sec = CUtil::Tock(trace_start_sec);

			OnDataReady();

			//bool is_reset_chart = CurrentScanAzimuth == 0;
			CurrentScanAzimuth = end_azimuth + HorizontalScanStepAngleDeg;

			Visualize(pScanResult, GetActorLocation(), GetActorForwardVector(), GetActorRightVector(), RangeMaxMeter, (void*)pTracker);

			CUtil::DebugLog("Scanning");
		}
		else {

			int sector_ind = start_azimuth / (FovHorizontalDeg / pScanResult->SectorCount);
			SSectorInfo* p_current_sektor = pScanResult->GetSectorContainer()->GetSector(sector_ind);
			p_current_sektor->Reset();
			OnDataReady();
			CurrentScanAzimuth = end_azimuth;

		}


		if (CurrentScanAzimuth >= FovHorizontalDeg) {

			IsFullScaned = true;
			if (RadarTransmissionDurationSec != 0) {
				NextScanTime = CTime::GetTimeSecond() + RadarTransmissionDurationSec;
			}

		}
		else {
			double total_scan_period_sec = (1.0 / Frequency - RadarTransmissionDurationSec);

			if (total_scan_period_sec < 0) {
				CUtil::DebugLog("Adjust Transmittion Duration and Frequency Reasonably");
				total_scan_period_sec = 10e-3;
			}

			NextScanTime = CTime::GetTimeSecond() + total_scan_period_sec / ScanResultContainer.GetSectorCount();
		}

	}

}


void AHalo24Radar::Save(ISaveLoader* p_save_load)
{
	Super::Save(p_save_load);
	//p_save_load->AddLine(line);
}
void AHalo24Radar::OnRecievedMessage(void* raw_pack)
{
	SRadarSimSDKPacket* p_pack = (SRadarSimSDKPacket*)raw_pack;

	if (p_pack->Header.PacketType == ESimSDKDataIDS::QueryRadars) {
		SendSerial();
	}
	else if (p_pack->Header.PacketType == ESimSDKDataIDS::UnlockKeys) {
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
			FLOAT64 bearing_deg = 0;
			if (p_args->BearingType == 1) {
				bearing_deg = (INT32S)p_args->BearingDeg;
			}
			else {
				auto rpy = CMath::GetActorEulerAnglesRPY(GetOwner());
				bearing_deg = FMath::Fmod(rpy.Z + (INT32S)p_args->BearingDeg, 360.0);
			}
			bool ret = pTracker->TryTrack(p_args->Id, GetActorLocation(), bearing_deg, p_args->RangeMeter);
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
	
	FovHorizontalDeg = 360;
	EachScanBeamWidthDeg = 45;
	Super::InitSensor();

	GuardZone.Init(MaxGuardZoneCount);
	BlankingZone.Init(MaxSectorBlankingZoneCount);

	BlankingZone.SetArea(0, 0, 90);
	BlankingZone.SetArea(1, 90, 180);
	BlankingZone.SetArea(2, 180, 270);
	BlankingZone.SetArea(3, 270, 360);


	pHalo24CommIF = (UHalo24CommIF*)pCommIF;
	pHalo24CommIF->SetHostIF(this);

	if (StandaloneModeEnabled) {
		Halo24StateMachine = EHalo24StateMachineStates::StandaloneModeEnter;
	}
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


