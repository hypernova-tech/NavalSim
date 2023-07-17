// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Sensor/GenericSensor/RadarBase.h"
#include "Lib/Utils/CUtil.h"


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
	pScanResult = new SScanResult();
}


void ARadarBase::Run(float delta_time_sec)
{
	Super::Run(delta_time_sec);

	bool ret = CUtil::Trace(this, RangeMeter.Y, FovHorizontalDeg, FovVerticalDeg, HorizontalScanStepAngleDeg, VerticalScanStepAngleDeg, pScanResult);
	
	if (pCommIF != nullptr) {
		pCommIF->SendData(pScanResult, -1);
	}

}
void ARadarBase::Visualize(TArray<FVector> points, FVector center)
{
	Super::Visualize(points, center);

}