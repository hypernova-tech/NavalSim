// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include <Engine/PostProcessVolume.h>
#include <Components/VolumetricCloudComponent.h>
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



	void UpdateGlobalMaterial();

	void UpdateActorsThermelBehaviour();

	void UpdateSkyMaterialThermalBehaviour();

	void UpdateVolumeticCloudThermalBehaviour();

	void UpdateActorThermalBehaviour(AActor* Actor);

	void SetMaterialParams(UMaterialInstanceDynamic* p_ins, double temprature_kelvin);

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
};
