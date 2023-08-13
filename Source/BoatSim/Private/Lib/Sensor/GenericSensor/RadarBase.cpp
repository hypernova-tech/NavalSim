// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "Lib/Utils/CUtil.h"
#include <Lib/SystemManager/SystemManagerBase.h>


void ARadarBase::SetFrequency(double val)
{
	Frequency = val;
}

double ARadarBase::GetFrequency()
{
	return Frequency;
}

void ARadarBase::SetRangeMeter(FVector2D val)
{
	RangeMeter = val;
}

FVector2D ARadarBase::GetRangeMeter()
{
	return RangeMeter;
}

void ARadarBase::SetNoiseMean(double val)
{
	NoiseMean = val;
}

double ARadarBase::GetNoiseMean()
{
	return NoiseMean;
}

void ARadarBase::SetNoiseStdDeviation(double val)
{
	NoiseStdDeviation = val;
}

double ARadarBase::GetNoiseStdDeviation()
{
	return NoiseStdDeviation;
}

void ARadarBase::SetFovVerticalDeg(double val)
{
	FovVerticalDeg = val;
}

double ARadarBase::GetFovVerticalDeg()
{
	return FovVerticalDeg;
}

void ARadarBase::SetFovHorizontalDeg(double val)
{
	FovHorizontalDeg = val;
}

double ARadarBase::GetFovHorizontalDeg()
{
	return FovHorizontalDeg;
}



void ARadarBase::BeginPlay()
{
	CUtil::DebugLog("ARadarBase Beginplay");
	Super::BeginPlay();
	
	CUtil::FStringToAsciiChar(RadarSerial, Serial, sizeof(Serial));
}

void ARadarBase::InitSensor()
{
	Super::InitSensor();
	pScanResult = new SScanResult();
	pScanResult->Init(10);
}


void ARadarBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);
	RadarStateMachine();

}

void ARadarBase::RadarStateMachine()
{
	Scan();
}
void ARadarBase::Scan()
{
	bool is_reset = false;

	if (IsFullScaned) {

		CurrentScanAzimuth = 0;
		IsFullScaned = false;
	}


	if (FApp::GetCurrentTime() >= NextScanTime) {
		float start_azimuth = CurrentScanAzimuth;
		float end_azimuth = 36 + start_azimuth;

		if (end_azimuth >= 359.99) {
			end_azimuth = 359.99;
			IsFullScaned = true;
		}


		bool ret = CUtil::Trace(this, true, RangeMeter.X, RangeMeter.Y, start_azimuth, end_azimuth, 0, FovVerticalDeg, HorizontalScanStepAngleDeg, VerticalScanStepAngleDeg, MeasurementErrorMean, MeasurementErrorUncertainy, GetClutterParams(), ShowBeam, ASystemManagerBase::GetInstance()->GetSensorGlobalIgnoreList(), pScanResult);

		if (pCommIF != nullptr) {
			pCommIF->SendData(pScanResult, -1);
		}

		CurrentScanAzimuth = end_azimuth;

		Visualize(pScanResult, GetActorLocation(), GetActorForwardVector(), GetActorRightVector(), RangeMeter.Y);

		NextScanTime = FApp::GetCurrentTime() + 0.1;

		CUtil::DebugLog("Scanning");
	}

}