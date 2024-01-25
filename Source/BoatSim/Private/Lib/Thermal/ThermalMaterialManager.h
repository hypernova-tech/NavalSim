// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include <Engine/PostProcessVolume.h>
#include <Components/VolumetricCloudComponent.h>
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "ThermalMaterialManager.generated.h"





/**
 * 
 */

UENUM(BlueprintType)
enum class EThermalMode
{
	ThermalModeVis,
	ThermalModeNIR,
	ThermalModeSWIR,
	ThermalModeMWIR,
	ThermalModeLWIR

};

UCLASS()
class AThermalMaterialManager : public AActorBase
{
	GENERATED_BODY()


private:
	const FString ParamNameEnableIR = "EnableIR";
	const FString ParamNameTempratureKelvin = "TempratureKelvin";
	const FString ParamNameEnableTimeOfDaySim = "EnableTimeOfDaySim";
	const FString ParamNameTimeOfDayHr = "TimeOfDay";
	const FString ParamNameTimeOfDuskHr = "TimeOfDusk";
	const FString ParamNameTimeOfSunSetHr = "TimeOfSunSet";
	const FString ParamNameTimedLightScale = "TimedLightScale";
	const FString ParamNameHeatSourceBase = "HeatSource";
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		EThermalMode ThermalMode_ = EThermalMode::ThermalModeVis;

	UPROPERTY(EditAnywhere)
		APostProcessVolume* pPostProcessVolume;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		UVolumetricCloudComponent* pVolumeticCloudComp = nullptr;

	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> SpecialMaterials;

	UPROPERTY(EditAnywhere)
		UMaterialParameterCollection* ThermalMaterialParameterCollection;

	UPROPERTY(EditAnywhere)
		double EmissiveOverride_ = 0;

	UPROPERTY(EditAnywhere)
		double ExtinctionOverride_ = 1;
	

	void UpdateGlobalMaterial();

	void UpdateActorsThermelBehaviour();

	void UpdateSkyMaterialThermalBehaviour();

	void UpdateVolumeticCloudThermalBehaviour();

	void UpdateMaterialCollections();

	void UpdateHeatSources(UMaterialInstanceDynamic *p_inst);

	void UpdateActorThermalBehaviour(AActor* Actor);

	void SetMaterialParams(UMaterialInstanceDynamic* p_ins, double temprature_kelvin, bool enable_timeofday_sim, bool update_heat_source);

	void UpdateSpecialMaterials();
	bool DoesScalarParameterExist(UMaterialInterface* MaterialInterface, FName ParameterName);

	virtual void OnStep(float DeltaTime) override;
	virtual void OnStepScenarioMode(float DeltaTime) override;
public:
	virtual void OnPreStep(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		bool GetIsVisibleThermalMode();


	virtual void Save(ISaveLoader* p_save_loader) override;
	virtual void SaveJSON(CJsonDataContainer& data) override;

	void SetThermalMode(EThermalMode mode);
	EThermalMode GetThermalMode();

	TArray<AActorBase*> HeatSources;
};
