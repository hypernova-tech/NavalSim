// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/SensorBase.h"
#include "FLS3D.generated.h"

/**
 * 
 */
UCLASS()
class AFLS3D : public ASensorBase
{
	GENERATED_BODY()

public:
	AFLS3D();
protected:

	SScanResult* pScanResult;

	UPROPERTY(EditAnywhere)
		double Frequency;

	UPROPERTY(EditAnywhere)
		FVector2D RangeMeter;

	UPROPERTY(EditAnywhere)
		double NoiseMean;

	UPROPERTY(EditAnywhere)
		double NoiseStdDeviation;

	UPROPERTY(EditAnywhere)
		double FovVerticalDeg;

	UPROPERTY(EditAnywhere)
		double FovHorizontalDeg;

	UPROPERTY(EditAnywhere)
		float HorizontalScanStepAngleDeg;

	UPROPERTY(EditAnywhere)
		float VerticalScanStepAngleDeg = 36;

	UPROPERTY(EditAnywhere)
		float FullScanPeriodSec = 1;
	UPROPERTY(EditAnywhere)
		bool ShowFLSBeam = false;



	virtual void Run(float delta_time_sec) override;

	virtual void BeginPlay() override;

public:


	void SetFrequency(double val);
	double GetFrequency();

	void SetRangeMeter(FVector2D val);
	FVector2D GetRangeMeter();

	void SetNoiseMean(double val);
	double GetNoiseMean();

	void SetNoiseStdDeviation(double val);
	double GetNoiseStdDeviation();

	void SetFovVerticalDeg(double val);
	double GetFovVerticalDeg();

	void SetFovHorizontalDeg(double val);
	double GetFovHorizontalDeg();


private:
	float CurrentScanAzimuth;
	double NextScanTime;
	bool IsFullScaned = false;
};
