// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/SensorBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/Console/CCLICommandManager.h>
#include <Lib/SystemManager/SystemManagerBase.h>




// Sets default values
ASensorBase::ASensorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

void ASensorBase::Run(float delta_time_sec)
{
}

void ASensorBase::SensorStateMachine(float delta_time_sec)
{
	ESensorState current_state = SensorState;
	ESensorState  next_state = current_state;

	switch (current_state)
	{
	case Idle:
		if (Enabled) {
			next_state = Init;
		}
		break;
	case Init:
		InitSensor();
		next_state = ESensorState::Running;
		break;
	case Running:
		Run(delta_time_sec);
		if (!Enabled) {
			next_state = ESensorState::Paused;
			PauseSensor();
		}
		break;
	case Paused:
		if (Enabled) {
			next_state = ESensorState::Running;
			ResumeSensor();
		}
		break;
	case Finish:
		FinalizeSensor();
		break;
	default:
		break;
	}

	SensorState = next_state;
}

void ASensorBase::InitSensor()
{
	
	if (SimulatePhysicsEnabled) {

	}
	else {
		this->DisableComponentsSimulatePhysics();

	}

	ASystemManagerBase::GetInstance()->GetUIController()->SetSensorTitleText(SensorSlotIndex, GetName());

}



void ASensorBase::FinalizeSensor()
{
}

SClutterParams ASensorBase::GetClutterParams()
{
	SClutterParams Params;

	Params.EnableFoamDetect = EnableFoamDetect;
	Params.EnableSubsurfaceScan = EnableSubsurfaceDetect;
	Params.EnableSurfaceDetect = EnableSurfaceDetect;
	Params.SeaSurfaceDetectionProbability = SeaSurfaceDetectionProbability;
	Params.MaxSurfacePenetrationMeter = MaxSurfacePenetrationMeter;
	return Params;
}

// Called when the game starts or when spawned
void ASensorBase::BeginPlay()
{
	CUtil::DebugLog("ASensorBase Beginplay");
	Super::BeginPlay();
	CUtil::DebugLog("TArray<AActor*> child_actors");
	TArray<AActor*> child_actors;
	GetAllChildActors(child_actors, true);

	for (AActor* ChildActor : child_actors)
	{
		CUtil::DebugLog("ChildActor");
		// Check if the child actor is of the desired class
		if (ChildActor->IsA<APointVisualizer>())
		{
			pPointVisualizer = Cast<APointVisualizer>(ChildActor);
			// Perform actions with the found child actor
			break;
		}



	}

	for (AActor* ChildActor : child_actors)
	{
		CUtil::DebugLog("ChildActor");
		// Check if the child actor is of the desired class
		if (ChildActor->IsA<ASceneCapturer>())
		{
			pSceneCapturer = Cast<ASceneCapturer>(ChildActor);
			// Perform actions with the found child actor
			break;
		}



	}

	
}

void ASensorBase::OnStep(float DeltaTime)
{
	Super::OnStep(DeltaTime);
	if (Enabled || true) {

		SensorStateMachine(DeltaTime);

	}
}


void ASensorBase::Visualize(SScanResult* p_scan_result, FVector origin, FVector current_forward, FVector current_right, float max_range_meter, void* p_tracker)
{
	auto vis_stat_sec = CUtil::Tick();
	if (pPointVisualizer != nullptr) {

		pPointVisualizer->Visualize(p_scan_result, origin, current_forward, current_right, max_range_meter, p_tracker);
	}

	auto elp_sec = CUtil::Tock(vis_stat_sec);
	//CUtil::DebugLog(FString::Printf(TEXT("vis(ms) : %f"), elp_sec * 1000));
	
}

UGenericCommIF* ASensorBase::GetCommCommIF()
{
	return pCommIF;
}

void ASensorBase::OnCaptureReady(void* p_data)
{
}


void ASensorBase::ResumeSensor()
{
}

void ASensorBase::PauseSensor()
{
}

void ASensorBase::OnDataReady()
{

}


void ASensorBase::SetFrequency(double val)
{
	Frequency = val;
}



double ASensorBase::GetFrequency()
{
	return Frequency;
}

void ASensorBase::SetRangeMeter(FVector2D val)
{
	RangeMinMeter = val.X;
	RangeMaxMeter = val.Y;
}

FVector2D ASensorBase::GetRangeMeter()
{
	return FVector2D(RangeMinMeter, RangeMaxMeter);
}

void ASensorBase::SetNoiseMean(double val)
{
	NoiseMean = val;
}

double ASensorBase::GetNoiseMean()
{
	return NoiseMean;
}

void ASensorBase::SetNoiseStdDeviation(double val)
{
	NoiseStdDeviation = val;
}

double ASensorBase::GetNoiseStdDeviation()
{
	return NoiseStdDeviation;
}

void ASensorBase::SetFovVerticalDeg(double val)
{
	FovVerticalDeg = val;
}

double ASensorBase::GetFovVerticalDeg()
{
	return FovVerticalDeg;
}

void ASensorBase::SetFovHorizontalDeg(double val)
{
	FovHorizontalDeg = val;
}

double ASensorBase::GetFovHorizontalDeg()
{
	return FovHorizontalDeg;
}

float  ASensorBase::GetHorizontalScanStepAngleDeg()
{
	return HorizontalScanStepAngleDeg;

}
void  ASensorBase::SetHorizontalScanStepAngleDeg(float val)
{
	HorizontalScanStepAngleDeg = val;
}

float  ASensorBase::GetVerticalScanStepAngleDeg()
{
	return VerticalScanStepAngleDeg;
}
void  ASensorBase::SetVerticalScanStepAngleDeg(float val)
{
	VerticalScanStepAngleDeg = val;
}

void ASensorBase::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);
	FString line;
	
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::SensorSlotIndex, SensorSlotIndex);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::RangeMin, RangeMinMeter);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::RangeMax, RangeMaxMeter);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::VericalFov,					FovVerticalDeg);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::HorizontalFov,				FovHorizontalDeg);
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::HorizontalScanStepAngleDeg,	HorizontalScanStepAngleDeg);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::VerticalScanStepAngleDeg,		VerticalScanStepAngleDeg);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::MeasurementErrorMean,			MeasurementErrorMean);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::MeasurementErrorStd,			MeasurementErrorUncertainy);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::EnableSurfaceDetect,			EnableSurfaceDetect);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::EnableSubsurfaceDetect,		EnableSubsurfaceDetect);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::EnableFoamDetect,				EnableFoamDetect);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::SeaSurfaceDetectionProb,		SeaSurfaceDetectionProbability);
	p_save_loader->AddLine(line);
	
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::MaxSurfacePenetration,		MaxSurfacePenetrationMeter);
	p_save_loader->AddLine(line);

}



void ASensorBase::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);
	data.Add(CCLICommandManager::Beam, ShowBeam);
	data.Add(CCLICommandManager::SensorSlotIndex, SensorSlotIndex);
	data.Add(CCLICommandManager::VericalFov, FovVerticalDeg);
	data.Add(CCLICommandManager::HorizontalFov, FovHorizontalDeg);
	data.Add(CCLICommandManager::HorizontalScanStepAngleDeg, HorizontalScanStepAngleDeg);
	data.Add(CCLICommandManager::VerticalScanStepAngleDeg, VerticalScanStepAngleDeg);
	data.Add(CCLICommandManager::MeasurementErrorMean, MeasurementErrorMean);
	data.Add(CCLICommandManager::MeasurementErrorStd, MeasurementErrorUncertainy);
	data.Add(CCLICommandManager::EnableSurfaceDetect, EnableSurfaceDetect);
	data.Add(CCLICommandManager::EnableSubsurfaceDetect, EnableSubsurfaceDetect);
	data.Add(CCLICommandManager::EnableFoamDetect, EnableFoamDetect);
	data.Add(CCLICommandManager::SeaSurfaceDetectionProb, SeaSurfaceDetectionProbability);
	data.Add(CCLICommandManager::MaxSurfacePenetration, MaxSurfacePenetrationMeter);
	data.Add(CCLICommandManager::RangeMin, RangeMinMeter);
	data.Add(CCLICommandManager::RangeMax, RangeMaxMeter);

	
}
