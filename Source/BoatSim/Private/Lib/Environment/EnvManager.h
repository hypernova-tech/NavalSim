// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include "Engine/PostProcessVolume.h"
#include "EnvManager.generated.h"

USTRUCT(BlueprintType)
struct FThermalColor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FLinearColor VisColor;
	UPROPERTY(EditAnywhere)
		FLinearColor SWIRColor;
	UPROPERTY(EditAnywhere)
		FLinearColor MWIRColor;
	UPROPERTY(EditAnywhere)
		FLinearColor LWIRColor;

};

/**
 * 
 */
UCLASS()
class AEnvManager : public AActorBase
{
	GENERATED_BODY()


public:
	AEnvManager();
	virtual void BeginPlay() override;
protected:



	UPROPERTY(EditAnywhere)
		double TimeOfDayHour = 12;

	UPROPERTY(EditAnywhere)
		double EnvTimeFlowScale = 1;

	UPROPERTY(EditAnywhere)
		double CloudPercent = 0;

	UPROPERTY(EditAnywhere)
		double FogPercent = 0;


	UPROPERTY(EditAnywhere)
		FVector2D FogColorIntensityVisible;

	UPROPERTY(EditAnywhere)
		FVector2D FogColorIntensityIR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor  FogInScatteringSunVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor  FogInScatteringSunIR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FThermalColor RayleighScatteringColorDay;

	UPROPERTY(EditAnywhere)
		double RainPercent = 0;

	UPROPERTY(EditAnywhere)
		double SnowPercent = 0;

	UPROPERTY(EditAnywhere)
		double DustPercent = 0;

	UPROPERTY(EditAnywhere) // azimuth from north in deg
		double ThunderLevelPercent = 0;

	UPROPERTY(EditAnywhere)
		bool EnvSoundFxEnabled = true;

	UPROPERTY(EditAnywhere)
		double WindSpeedMetersPerSec = 0;

	UPROPERTY(EditAnywhere) // azimuth from north in deg
		double WindDirectionDeg = 0;
	
	UPROPERTY(EditAnywhere)
		APostProcessVolume* pPostProcessVolume;

	UPROPERTY(EditAnywhere)
		AActor* pSea;

	UPROPERTY(EditAnywhere)
		AActor* pSky;


	
	bool IsOceanDynamicsDirty = false;
	INT64U FrameCountOceanDynamicDirty;
	void HandleDynamicOcean();

public:

	UFUNCTION(BlueprintCallable)
		void SetStartingDate(int year, int month, int day, int hour, int min, double sec);
	
	UFUNCTION(BlueprintCallable)
		void SetEnvSoundFxEnabled(bool val);

	UFUNCTION(BlueprintCallable)
		bool GetEnvSoundFxEnabled();

	UFUNCTION(BlueprintCallable)
		FDateTime GetStartingDateTime();

	UFUNCTION(BlueprintCallable)
	FDateTime GetCurrentDateTime();

	UFUNCTION(BlueprintCallable)
		void SetFogLevelPercent(double level);

	UFUNCTION(BlueprintCallable)
		double GetFogLevelPercent();

	UFUNCTION(BlueprintCallable)
		FVector2D GetFogColorIntensity();

	UFUNCTION(BlueprintCallable)
		FLinearColor GetFogInScatteringSunColor();


	UFUNCTION(BlueprintCallable)
		FLinearColor GetRayleighScatteringColorDay();


	UFUNCTION(BlueprintCallable)
		void SeRainLevelPercent(double level);

	UFUNCTION(BlueprintCallable)
		double GetRainLevelPercent();

	UFUNCTION(BlueprintCallable)
		void SetSnowLevelPercent(double level);

	UFUNCTION(BlueprintCallable)
		double GetSnowLevelPercent();
	
	UFUNCTION(BlueprintCallable)
		void SetWindSpeedMetersPerSec(double level);

	UFUNCTION(BlueprintCallable)
		double GetWindSpeedMetersPerSec();

	UFUNCTION(BlueprintCallable)
		void SetWindDirectionDeg(double level);

	UFUNCTION(BlueprintCallable)
		double GetWindDirectionDeg();

	UFUNCTION(BlueprintCallable)
		FVector GetWindDirectionVector();

	UFUNCTION(BlueprintCallable)
		void SetDustLevelPercent(double level);

	UFUNCTION(BlueprintCallable)
		double GetDustLevelPercent();
	
	UFUNCTION(BlueprintCallable)
		void SetCloudLevelPercent(double level);

	UFUNCTION(BlueprintCallable)
		double GetCloudLevelPercent();

	UFUNCTION(BlueprintCallable)
		void SetThunderLevelPercent(double level);

	UFUNCTION(BlueprintCallable)
		double GetThunderLevelPercent();

	UFUNCTION(BlueprintCallable)
		void SetTimeOfDayHr(double level);

	UFUNCTION(BlueprintCallable)
		double GetTimeOfDayHr();

	UFUNCTION(BlueprintCallable)
		void SetEnvTimeFlowScale(double level);

	UFUNCTION(BlueprintCallable)
		double GetEnvTimeFlowScale();

	UFUNCTION(BlueprintCallable)
		bool GetIsOceanDynamicsDirty();

	virtual void SaveJSON(CJsonDataContainer& data);
	virtual void Save(ISaveLoader* p_save_loader);
	virtual void OnStep(float DeltaTime) override;
	virtual void OnStepScenarioMode(float DeltaTime) override;
	virtual void UpdateAnnotation(bool is_enabled) override;

	APostProcessVolume* GetPostProcessVolume();
};
