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
	pPointVisualizer->CreateRenderTarget(512, 512, ASystemManagerBase::GetInstance()->GetUIController()->GetLidarSlotImage(SensorSlotIndex));
	CurrentScanAzimuth = -FovHorizontalDeg * 0.5;

	pScanResult = new SScanResult();
	pScanResult->Init(1);
}

void ALidarBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	bool is_reset = false;

	if (FApp::GetCurrentTime() >= NextScanTime) {
		float start_azimuth = -FovHorizontalDeg * 0.5;
		float end_azimuth = FovHorizontalDeg * 0.5;
		
		bool ret = CUtil::Trace(this, false, RangeMeter.X, RangeMeter.Y, start_azimuth, end_azimuth, 0, FovVerticalDeg, HorizontalScanStepAngleDeg, VerticalScanStepAngleDeg, ShowFLSBeam, ASystemManagerBase::GetInstance()->GetSensorGlobalIgnoreList(), pScanResult);

		if (pCommIF != nullptr) {
			pCommIF->SendData(pScanResult, -1);
		}

		FVector forward = GetActorForwardVector();
		FVector right = GetActorRightVector();

		Visualize(pScanResult, GetActorLocation(), forward, right, RangeMeter.Y);

		NextScanTime = FApp::GetCurrentTime() + 0.1;

		IsFullScaned = true;

		CUtil::DebugLog("Scanning");
	}



}
