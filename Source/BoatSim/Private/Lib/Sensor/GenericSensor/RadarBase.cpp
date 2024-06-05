// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "Lib/Utils/CUtil.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include "Lib/Tracker/RadarBasedTracker/RadarBasedTracker.h"
#include <Lib/Math/CMath.h>


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
	IsAutoScanEnabled = val;
}
bool ARadarBase::GetScanEnabled()
{
	return IsAutoScanEnabled;
}



void ARadarBase::BeginPlay()
{
	Super::BeginPlay();
	//SetScanEnabled(false);
	CUtil::FStringToAsciiChar(RadarSerial, Serial, sizeof(Serial));
}

void ARadarBase::InitSensor()
{
	Super::InitSensor();
	pPointVisualizer->CreateRenderTarget(512, 512, ASystemManagerBase::GetInstance()->GetUIController()->GetSensorSlotImage(SensorSlotIndex));
	
	INT32S sector_cnt = (int)(FovHorizontalDeg / EachScanBeamWidthDeg);
	ScanResultContainer.Init(16, sector_cnt, HorizontalScanStepAngleDeg);
	pScanResult = ScanResultContainer.GetCircular();

	InitTracker();
	if (UseRenderTargetForDepthCalculation) {
		pSceneCapturer->CreateRenderTexture(this, DepthRenderTargetWidthPx, DepthRenderTargetHeightPx, EPixelFormat::PF_B8G8R8A8);
		
	}

	pCommIF->SetHostIF(this);
}


void ARadarBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	RadarStateMachine();

}


void ARadarBase::RadarStateMachine()
{
	UpdateTracker();
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
	

	if (!IsAutoScanEnabled) {
		return;
	}
	bool is_reset = false;

	if (FApp::GetCurrentTime() >= NextScanTime) {

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
		args.elevation_start_deg = elev - FovVerticalDeg ;
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
		args.use_render_target	= UseRenderTargetForDepthCalculation;
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

void ARadarBase::OnRecievedMessage(void* p_commands)
{
}

char* ARadarBase::GetSerial()
{
	return Serial;
}
double ARadarBase::GetGain()
{
	return GainLevel;
}
double ARadarBase::GetRainClutterLevel()
{
	return RainClutterLevel;
}
double ARadarBase::GetSeaClutterLevel()
{
	return SeaClutterLevel;
}
AActor* ARadarBase::GetOwningActor()
{
	return this;
}