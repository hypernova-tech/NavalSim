// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/FLS3D.h"
#include "Lib/Utils/CUtil.h"
#include <Lib/SystemManager/SystemManagerBase.h>

AFLS3D::AFLS3D()
{
}

void AFLS3D::SetFrequency(double val)
{
	Frequency = val;
}

double AFLS3D::GetFrequency()
{
	return Frequency;
}

void AFLS3D::SetRangeMeter(FVector2D val)
{
	RangeMeter = val;
}

FVector2D AFLS3D::GetRangeMeter()
{
	return RangeMeter;
}

void AFLS3D::SetNoiseMean(double val)
{
	NoiseMean = val;
}

double AFLS3D::GetNoiseMean()
{
	return NoiseMean;
}

void AFLS3D::SetNoiseStdDeviation(double val)
{
	NoiseStdDeviation = val;
}

double AFLS3D::GetNoiseStdDeviation()
{
	return NoiseStdDeviation;
}

void AFLS3D::SetFovVerticalDeg(double val)
{
	FovVerticalDeg = val;
}

double AFLS3D::GetFovVerticalDeg()
{
	return FovVerticalDeg;
}

void AFLS3D::SetFovHorizontalDeg(double val)
{
	FovHorizontalDeg = val;
}

double AFLS3D::GetFovHorizontalDeg()
{
	return FovHorizontalDeg;
}



void AFLS3D::BeginPlay()
{
	CUtil::DebugLog("ARadarBase Beginplay");
	Super::BeginPlay();



	
}

void AFLS3D::InitSensor()
{
	CurrentScanAzimuth = -FovHorizontalDeg * 0.5;

	pScanResult = new SScanResult();
	pScanResult->Init(1);
}



void AFLS3D::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	bool is_reset = false;




	if (FApp::GetCurrentTime() >= NextScanTime) {
		float start_azimuth = -FovHorizontalDeg * 0.5;
		float end_azimuth = FovHorizontalDeg*0.5;
		TArray<AActor*> ignored_actor;
		bool ret = CUtil::Trace(this, false, RangeMeter.X, RangeMeter.Y, start_azimuth, end_azimuth, 0, FovVerticalDeg, HorizontalScanStepAngleDeg, VerticalScanStepAngleDeg, ShowBeam, ignored_actor,pScanResult);

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

