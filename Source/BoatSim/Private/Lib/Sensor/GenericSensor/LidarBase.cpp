// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/LidarBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

ALidarBase::ALidarBase()
{
	
	
}

void ALidarBase::SetFrequency(double val)
{
	Frequency = val;
}

void ALidarBase::OnScanCompleted(SScanResult* p_scan_result)
{
	if (pCommIF != nullptr) {
		pCommIF->SendData(pScanResult, -1);
	}
}

double ALidarBase::GetFrequency()
{
	return Frequency;
}

void ALidarBase::SetRangeMeter(FVector2D val)
{
	RangeMeter = val;
}

FVector2D ALidarBase::GetRangeMeter()
{
	return RangeMeter;
}

void ALidarBase::SetNoiseMean(double val)
{
	NoiseMean = val;
}

double ALidarBase::GetNoiseMean()
{
	return NoiseMean;
}

void ALidarBase::SetNoiseStdDeviation(double val)
{
	NoiseStdDeviation = val;
}

double ALidarBase::GetNoiseStdDeviation()
{
	return NoiseStdDeviation;
}

void ALidarBase::SetFovVerticalDeg(double val)
{
	FovVerticalDeg = val;
}

double ALidarBase::GetFovVerticalDeg()
{
	return FovVerticalDeg;
}

void ALidarBase::SetFovHorizontalDeg(double val)
{
	FovHorizontalDeg = val;
}

double ALidarBase::GetFovHorizontalDeg()
{
	return FovHorizontalDeg;
}



void ALidarBase::BeginPlay()
{
	CUtil::DebugLog("ARadarBase Beginplay");
	Super::BeginPlay();




}

void ALidarBase::InitSensor()
{
	Super::InitSensor();

	pPointVisualizer->CreateRenderTarget(512, 512, ASystemManagerBase::GetInstance()->GetUIController()->GetLidarSlotImage(SensorSlotIndex));
	CurrentScanAzimuth = -FovHorizontalDeg * 0.5;

	pScanResult = new SScanResult();
	pScanResult->Init(1);
	pSceneCapturer->CreateRenderTexture(this, DepthRenderTargetWidthPx, DepthRenderTargetHeightPx, EPixelFormat::PF_B8G8R8A8);
}

void ALidarBase::OnCaptureReady(void* p_data)
{

	auto read_pixed_elp_sec = CUtil::Tock(CaptureStartTimeRef);
	CUtil::DebugLog(FString::Printf(TEXT("lidar capture duration(ms): %f"), 1000 * read_pixed_elp_sec));
}
void ALidarBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	bool is_reset = false;

	if (FApp::GetCurrentTime() >= NextScanTime) {
		float start_azimuth = -FovHorizontalDeg * 0.5;
		float end_azimuth = FovHorizontalDeg * 0.5;
		
		pScanResult->ElevationRange.X = -FovVerticalDeg * 0.5;
		pScanResult->ElevationRange.Y = +FovVerticalDeg * 0.5;


		pScanResult->AzimuthRange.X = -FovHorizontalDeg * 0.5;
		pScanResult->AzimuthRange.Y = +FovHorizontalDeg * 0.5;
		pScanResult->ScanAzimuthStepDeg = HorizontalScanStepAngleDeg;
		pScanResult->ScanElevationStepDeg = VerticalScanStepAngleDeg;

		STraceArgs args;

		args.p_actor = this;
		args.is_world = false;
		args.range_meter = RangeMeter.Y;
		args.min_range_meter = RangeMeter.X;
		args.azimuth_start_deg = start_azimuth;
		args.azimuth_end_deg = end_azimuth;
		args.elevation_start_deg = 0;
		args.elevation_end_deg = FovVerticalDeg;
		args.azimuth_angle_step_deg = HorizontalScanStepAngleDeg;
		args.elevation_angle_step_deg = VerticalScanStepAngleDeg;
		args.measurement_error_mean = MeasurementErrorMean;
		args.measurement_error_std = MeasurementErrorUncertainy;
		args.clutter_params = GetClutterParams();
		args.show_radar_beam = ShowBeam;
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
		Visualize(pScanResult, GetActorLocation(), forward, right, RangeMeter.Y);

		NextScanTime = FApp::GetCurrentTime() + 0.1;

		IsFullScaned = true;

		

		
	}



}
