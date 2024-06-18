// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/LidarBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

ALidarBase::ALidarBase()
{
	
	
}



void ALidarBase::BeginPlay()
{
	CUtil::DebugLog("ARadarBase Beginplay");
	Super::BeginPlay();




}


void ALidarBase::InitSensor()
{
	Super::InitSensor();

	pPointVisualizer->CreateRenderTarget(512, 512, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));
	CurrentScanAzimuth = -FovHorizontalDeg * 0.5;

	pScanResult = new SScanResult();
	pScanResult->Init(1);
	if (UseRenderTargetForDepthCalculation) {
		pSceneCapturer->CreateRenderTexture(this, DepthRenderTargetWidthPx, DepthRenderTargetHeightPx, EPixelFormat::PF_B8G8R8A8);
	}
	pCommIF->SetHostIF(this);
	
}

void ALidarBase::OnCaptureReady(void* p_data)
{

	auto read_pixed_elp_sec = CUtil::Tock(CaptureStartTimeRef);
	CUtil::DebugLog(FString::Printf(TEXT("lidar capture duration(ms): %f"), 1000 * read_pixed_elp_sec));
}

void ALidarBase::OnRecievedMessage(void* p_commands)
{
}

void* ALidarBase::GetOwningActor()
{
	return this;
}

void ALidarBase::OnScanCompleted(SScanResult* p_scan_result)
{
	if (pCommIF != nullptr) {
		pCommIF->SendData(pScanResult, -1);
	}
}
void ALidarBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	bool is_reset = false;

	if (FApp::GetCurrentTime() >= NextScanTime) {
		float start_azimuth = -FovHorizontalDeg * 0.5;
		float end_azimuth = FovHorizontalDeg * 0.5;
		auto reset_start_sec = CUtil::Tick();
		//pScanResult->ResetBuffers();
		auto reset_end_sec = CUtil::Tick();
		CUtil::DebugLog("ALidarBase: ResetBuffers " + CUtil::FloatToString((reset_end_sec-reset_start_sec) * 1000));
		
		pScanResult->ElevationRange.X = -FovVerticalDeg * 0.5;
		pScanResult->ElevationRange.Y = +FovVerticalDeg * 0.5;


		pScanResult->AzimuthRange.X = -FovHorizontalDeg * 0.5;
		pScanResult->AzimuthRange.Y = +FovHorizontalDeg * 0.5;
		pScanResult->ScanAzimuthStepDeg = HorizontalScanStepAngleDeg;
		pScanResult->ScanElevationStepDeg = VerticalScanStepAngleDeg;

		STraceArgs args;

		args.p_actor = this;
		args.is_world = false;
		args.is_normal_distribution = IsNormalDistribution;
		args.range_meter = RangeMaxMeter;
		args.min_range_meter = RangeMinMeter;
		args.azimuth_start_deg = start_azimuth;
		args.azimuth_end_deg = end_azimuth;
		args.elevation_start_deg = pScanResult->ElevationRange.X;
		args.elevation_end_deg = pScanResult->ElevationRange.Y;
		args.azimuth_angle_step_deg = HorizontalScanStepAngleDeg;
		args.elevation_angle_step_deg = VerticalScanStepAngleDeg;
		args.horizontal_fov_deg = FovHorizontalDeg;
		args.vertical_fov_deg = FovVerticalDeg;
		args.measurement_error_mean = MeasurementErrorMean;
		args.measurement_error_std = MeasurementErrorUncertainy;
		args.clutter_params = GetClutterParams();
		args.show_beam = ShowBeam;
		args.p_ignore_list = &(ASystemManagerBase::GetInstance()->GetSensorGlobalIgnoreList());
		args.create_scan_line = false;
		args.scan_center = GetActorLocation();
		auto rotator = GetActorRotation();
		args.scan_rpy_world_deg = FVector(rotator.Roll, -rotator.Pitch, -rotator.Yaw);
		auto* p_parent = CUtil::GetParentActor(this);
		if (p_parent != nullptr) {
			args.additional_ignore_list.Add(p_parent);
		}
		args.use_render_target = UseRenderTargetForDepthCalculation;

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

		//bool ret = CUtil::Trace(this, false, RangeMeter.X, RangeMeter.Y, start_azimuth, end_azimuth, 0, FovVerticalDeg, HorizontalScanStepAngleDeg, VerticalScanStepAngleDeg, MeasurementErrorMean, MeasurementErrorUncertainy, GetClutterParams(), ShowBeam, ASystemManagerBase::GetInstance()->GetSensorGlobalIgnoreList(), false, pScanResult);

	

		FVector forward = GetActorForwardVector();
		FVector right = GetActorRightVector();
		OnScanCompleted(pScanResult);
		Visualize(pScanResult, GetActorLocation(), forward, right, RangeMaxMeter);

		NextScanTime = FApp::GetCurrentTime() + 1.0/Frequency;

		IsFullScaned = true;

		

		
	}



}
