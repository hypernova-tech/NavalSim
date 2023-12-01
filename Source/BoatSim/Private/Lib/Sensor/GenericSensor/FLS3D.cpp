// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/FLS3D.h"
#include "Lib/Utils/CUtil.h"
#include <Lib/SystemManager/SystemManagerBase.h>

AFLS3D::AFLS3D()
{
}





void AFLS3D::BeginPlay()
{
	CUtil::DebugLog("ARadarBase Beginplay");
	Super::BeginPlay();



	
}

void AFLS3D::InitSensor()
{
	CurrentScanAzimuth = -FovHorizontalDeg * 0.5;
	pPointVisualizer->CreateRenderTarget(512, 512, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));

	pScanResult = new SScanResult();
	pScanResult->Init(1);
	if (UseRenderTargetForDepthCalculation) {
		pSceneCapturer->CreateRenderTexture(this, DepthRenderTargetWidthPx, DepthRenderTargetHeightPx, EPixelFormat::PF_B8G8R8A8);
	}
	
}
void AFLS3D::OnCaptureReady(void* p_data)
{

	auto read_pixed_elp_sec = CUtil::Tock(CaptureStartTimeRef);
	CUtil::DebugLog(FString::Printf(TEXT("fls capture duration(ms): %f"), 1000 * read_pixed_elp_sec));
}


void AFLS3D::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	bool is_reset = false;

	if (FApp::GetCurrentTime() >= NextScanTime) {
		float start_azimuth = -FovHorizontalDeg * 0.5;
		float end_azimuth = FovHorizontalDeg*0.5;
		TArray<AActor*> ignored_actor;


		STraceArgs args;

		args.p_actor = this;
		args.is_world = false;
		args.range_meter = RangeMaxMeter;
		args.min_range_meter = RangeMinMeter;
		args.azimuth_start_deg = start_azimuth;
		args.azimuth_end_deg = end_azimuth;
		args.elevation_start_deg = -FovVerticalDeg;
		args.elevation_end_deg = 0;
		args.azimuth_angle_step_deg = HorizontalScanStepAngleDeg;
		args.elevation_angle_step_deg = VerticalScanStepAngleDeg;
		args.measurement_error_mean = MeasurementErrorMean;
		args.measurement_error_std = MeasurementErrorUncertainy;
		args.clutter_params = GetClutterParams();
		args.show_radar_beam = ShowBeam;
		args.p_ignore_list =  &(ASystemManagerBase::GetInstance()->GetSensorGlobalIgnoreList());
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



		bool ret = false;

		
		ret = CUtil::Trace(args, pScanResult);
		

		//ret = CUtil::Trace(this, false, RangeMeter.X, RangeMeter.Y, start_azimuth, end_azimuth, -FovVerticalDeg, 0, HorizontalScanStepAngleDeg, VerticalScanStepAngleDeg, MeasurementErrorMean, MeasurementErrorUncertainy, GetClutterParams(), ShowBeam, ignored_actor,false, pScanResult);

		if (pCommIF != nullptr) {
			pCommIF->SendData(pScanResult, -1);
		}

		FVector forward = GetActorForwardVector();
		FVector right = GetActorRightVector();
	
		Visualize(pScanResult, GetActorLocation(), forward, right, RangeMaxMeter);

		NextScanTime = FApp::GetCurrentTime() + 0.2;

		IsFullScaned = true;

		//CUtil::DebugLog("Scanning");
	}



}

