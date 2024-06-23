// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/GenericRadar.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "Lib/Utils/CUtil.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include "Lib/Tracker/RadarBasedTracker/RadarBasedTracker.h"
#include <Lib/Math/CMath.h>


void AGenericRadar::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGenericRadar::InitSensor()
{
	EachScanBeamWidthDeg = FovHorizontalDeg;
	Super::InitSensor();
	CurrentScanAzimuth = -FovHorizontalDeg * 0.5;
}



void AGenericRadar::Scan()
{


	if (!IsAutoScanEnabled) {
		return;
	}
	bool is_reset = false;

	if (FApp::GetCurrentTime() >= NextScanTime) {

		if (IsFullScaned) {

			CurrentScanAzimuth = -EachScanBeamWidthDeg * 0.5;
			IsFullScaned = false;
			pScanResult = ScanResultContainer.GetCircular();

		}

		FLOAT64 start_azimuth = CurrentScanAzimuth;
		FLOAT64 end_azimuth = EachScanBeamWidthDeg + start_azimuth - HorizontalScanStepAngleDeg;

		
		
		STraceArgs args;

		FLOAT64 elev = CMath::GetActorEulerAnglesRPY(this).Y;

		args.p_actor = this;
		args.is_world = false;
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

		//// correnct the local--> world angle
		if (!BlankingZone.CheckAnyActiveZone(start_azimuth, end_azimuth)) {
	
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


		if (CurrentScanAzimuth >= FovHorizontalDeg*0.5) {

			IsFullScaned = true;
			if (RadarTransmissionDurationSec != 0) {
				NextScanTime = FApp::GetCurrentTime() + RadarTransmissionDurationSec;
			}

		}
		else {
			double total_scan_period_sec = (1.0 / Frequency - RadarTransmissionDurationSec);

			if (total_scan_period_sec < 0) {
				CUtil::DebugLog("Adjust Transmittion Duration and Frequency Reasonably");
				total_scan_period_sec = 10e-3;
			}

			NextScanTime = FApp::GetCurrentTime() + total_scan_period_sec / ScanResultContainer.GetSectorCount();
		}

	}

}
