// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "Lib/Utils/CUtil.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include "Lib/Tracker/RadarBasedTracker/RadarBasedTracker.h"


class FRaycastSensorTask
{
	ARadarBase* SensorInstance;
	FLOAT32 DeltaTimeSec;

public:

	static const TCHAR* GetTaskName()
	{
		return TEXT("FRaycastSensorTask");
	}


	FRaycastSensorTask(ARadarBase* InSensorInstance, FLOAT32 delta_time_sec)
		: SensorInstance(InSensorInstance)
	{
		DeltaTimeSec = delta_time_sec;
	}

	static FORCEINLINE TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FRaycastSensorTask, STATGROUP_TaskGraphTasks);

	}

	static  ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::AnyThread;
	}
	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		if (SensorInstance)
		{
			SensorInstance->Scan();
			FFunctionGraphTask::CreateAndDispatchWhenReady(
				[this]()
				{
					SensorInstance->OnAsynTaskComplete();
				},
				TStatId(), MyCompletionGraphEvent, ENamedThreads::GameThread
			);
		}
	}
};


void ARadarBase::SetTrackerEnabled(bool val)
{
	IsTrackerEnabled = val;
}
bool ARadarBase::GetTrackerEnabled()
{
	return IsTrackerEnabled;
}


void ARadarBase::SetScanEnabled(bool val)
{
	IsScanEnabled = val;
}
bool ARadarBase::GetScanEnabled()
{
	return IsScanEnabled;
}



void ARadarBase::BeginPlay()
{
	CUtil::DebugLog("ARadarBase Beginplay");
	Super::BeginPlay();
	SetScanEnabled(false);
	CUtil::FStringToAsciiChar(RadarSerial, Serial, sizeof(Serial));
}

void ARadarBase::InitSensor()
{
	Super::InitSensor();
	pPointVisualizer->CreateRenderTarget(512, 512, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));
	INT32S sector_cnt = (int)(360.0 / HorizontalScanStepAngleDeg + 0.5);
	ScanResultContainer.Init(1, sector_cnt, HorizontalScanStepAngleDeg);
	pScanResult = ScanResultContainer.GetCircular();
	BeamWidthDeg = 18;//todo fixme
	GuardZone.Init(MaxGuardZoneCount);
	BlankingZone.Init(MaxSectorBlankingZoneCount);

	BlankingZone.SetArea(0, 0, 90);
	BlankingZone.SetArea(1, 90, 180);
	BlankingZone.SetArea(2, 180, 270);
	BlankingZone.SetArea(3, 270, 360);

	InitTracker();
	if (UseRenderTargetForDepthCalculation) {
		pSceneCapturer->CreateRenderTexture(this, DepthRenderTargetWidthPx, DepthRenderTargetHeightPx, EPixelFormat::PF_B8G8R8A8);
		
	}
}


void ARadarBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	RadarStateMachine();

}


void ARadarBase::RadarStateMachine()
{
	UpdateTracker();

	/*
	if (RaycastTaskComplete.IsValid()) {
		Visualize(pScanResult, GetActorLocation(), GetActorForwardVector(), GetActorRightVector(), RangeMeter.Y, (void*)pTracker);
	}
	*/
	
	 //RaycastTaskComplete = TGraphTask<FRaycastSensorTask>::CreateTask().ConstructAndDispatchWhenReady(this, 0);
	
	Scan();
	
}

void ARadarBase::OnAsynTaskComplete()
{
	Visualize(pScanResult, GetActorLocation(), GetActorForwardVector(), GetActorRightVector(), RangeMaxMeter, (void*)pTracker);
}
void ARadarBase::Save(ISaveLoader* p_save_load)
{
	Super::Save(p_save_load);


	FString line;


	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarScanLevel,						FastScanLevel);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarScannerRPM,					ScannerRPMValue);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarGainType,						GainType);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarGainLevel,						GainLevel);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarSeaClutterType,				SeaClutterType);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarSeaClutterLevel,				SeaClutterLevel);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarSeaClutterAutoOffset,			SeaClutterAutoOffset);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarRainClutterLevel,				RainClutterLevel);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarMaxGuardZoneCount,				MaxGuardZoneCount);
	p_save_load->AddLine(line);

	line = p_save_load->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_load->AppendOption(line, CCLICommandManager::RadarMaxSectorBlankingZoneCount,	MaxSectorBlankingZoneCount);
	p_save_load->AddLine(line);







}
void ARadarBase::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);

	data.Add(CCLICommandManager::RadarScanLevel, FastScanLevel);
	data.Add(CCLICommandManager::RadarScannerRPM, ScannerRPMValue);
	data.Add(CCLICommandManager::RadarGainType, GainType);
	data.Add(CCLICommandManager::RadarGainLevel, GainLevel);
	data.Add(CCLICommandManager::RadarSeaClutterType, SeaClutterType);
	data.Add(CCLICommandManager::RadarSeaClutterLevel, SeaClutterLevel);
	data.Add(CCLICommandManager::RadarSeaClutterAutoOffset, SeaClutterAutoOffset);
	data.Add(CCLICommandManager::RadarRainClutterLevel, RainClutterLevel);
	data.Add(CCLICommandManager::RadarMaxGuardZoneCount, MaxGuardZoneCount);
	data.Add(CCLICommandManager::RadarMaxSectorBlankingZoneCount, MaxSectorBlankingZoneCount);

}
void ARadarBase::OnDataReady()
{
	if (pCommIF != nullptr) {
		pCommIF->SendData(pScanResult, -1);
	}
}
void ARadarBase::InitTracker()
{
	pTracker = new CRadarBasedTracker();
}
void ARadarBase::UpdateTracker()
{
	if (IsTrackerEnabled) {
		if (UseSimulationDataAsOwnShip) {
			pTracker->SetOwnshipData(this, GetActorLocation(), CUtil::GetActorRPY(this), GetVelocity(), GetRangeMeter(), NoiseMean, NoiseStdDeviation);
		}
		
		pTracker->Update();
	}
	
}


void ARadarBase::OnCaptureReady(void* p_data)
{
	
	auto read_pixed_elp_sec = CUtil::Tock(CaptureStartTimeRef);
	CUtil::DebugLog(FString::Printf(TEXT("radar capture duration(ms): %f"), 1000 * read_pixed_elp_sec));
}



void ARadarBase::Scan()
{
	if (!IsScanEnabled) {
		return;
	}
	bool is_reset = false;

	if (IsFullScaned) {

		CurrentScanAzimuth = 0;
		IsFullScaned = false;
		pScanResult = ScanResultContainer.GetCircular();
	}


	if (FApp::GetCurrentTime() >= NextScanTime) {
		FLOAT64 start_azimuth = CurrentScanAzimuth;
		FLOAT64 end_azimuth = BeamWidthDeg + start_azimuth;

		if (end_azimuth >= 359.99) {
			end_azimuth = 359.99;
			IsFullScaned = true;
		}

		STraceArgs args;

		args.p_actor = this;
		args.is_world = true;
		args.range_meter = RangeMaxMeter;
		args.min_range_meter = RangeMinMeter;
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
		args.create_scan_line = true;
		args.scan_center = GetActorLocation();
		auto rotator = GetActorRotation();
		args.scan_rpy_world_deg = FVector(rotator.Roll, -rotator.Pitch, -rotator.Yaw);
		auto* p_parent = CUtil::GetParentActor(this);
		if (p_parent != nullptr) {
			args.additional_ignore_list.Add(p_parent);
		}
		args.use_render_target = UseRenderTargetForDepthCalculation;

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
	

			CurrentScanAzimuth = end_azimuth + HorizontalScanStepAngleDeg;

			Visualize(pScanResult, GetActorLocation(), GetActorForwardVector(), GetActorRightVector(), RangeMaxMeter, (void*)pTracker);

			NextScanTime = FApp::GetCurrentTime() + 0.1;

			CUtil::DebugLog("Scanning");
		}
		else {

			int sector_ind = start_azimuth / (360.0 / pScanResult->SectorCount);
			SSectorInfo* p_current_sektor = pScanResult->GetSectorContainer()->GetSector(sector_ind);
			p_current_sektor->Reset();

			CurrentScanAzimuth = end_azimuth;
			NextScanTime = FApp::GetCurrentTime() + 0.125;
		}

	
	}

}