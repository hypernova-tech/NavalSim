// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include "EnvManager.generated.h"

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
		double CloudPercent = 0;

	UPROPERTY(EditAnywhere)
		double FogPercent = 0;

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

	virtual void SaveJSON(CJsonDataContainer& data);
	virtual void Save(ISaveLoader* p_save_loader);
};
