// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Thermal/ThermalMaterialManager.h"
#include <Lib/Console/CCLICommandManager.h>
#include "EngineUtils.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>

void AThermalMaterialManager::BeginPlay()
{
	Super::BeginPlay();
	Rename((TEXT("thermal")));
}


void AThermalMaterialManager::SetThermalMode(EThermalMode mode)
{
	ThermalMode_ = mode;
	UpdateGlobalMaterial();
}



EThermalMode AThermalMaterialManager::GetThermalMode()
{
	return ThermalMode_;
}
bool AThermalMaterialManager::DoesScalarParameterExist(UMaterialInterface* MaterialInterface, FName ParameterName)
{
	if (MaterialInterface)
	{
		TArray<FMaterialParameterInfo> OutParameterInfo;
		TArray<FGuid> OutParameterIds;
		MaterialInterface->GetAllScalarParameterInfo(OutParameterInfo, OutParameterIds);

		for (const FMaterialParameterInfo& ParamInfo : OutParameterInfo)
		{
			if (ParamInfo.Name == ParameterName)
			{
				return true;
			}
		}
	}
	return false;
}
void AThermalMaterialManager::OnStep(float DeltaTime)
{
	Super::OnPreStep(DeltaTime);
	UpdateGlobalMaterial();
}
void AThermalMaterialManager::OnStepScenarioMode(float DeltaTime)
{
	Super::OnStepScenarioMode(DeltaTime);
	UpdateGlobalMaterial();
}
void AThermalMaterialManager::UpdateGlobalMaterial()
{
	float enable_ir_value = 0;
	bool ocean_found = false;
	bool param_exists = false;

	//UpdateMaterialCollections();
	UpdateSkyMaterialThermalBehaviour();
	UpdateVolumeticCloudThermalBehaviour();
	UpdateActorsThermelBehaviour();
}
void AThermalMaterialManager::UpdateActorsThermelBehaviour()
{
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;
		UpdateActorThermalBehaviour(Actor);
	}
}
void AThermalMaterialManager::UpdateVolumeticCloudThermalBehaviour()
{

	if (pVolumeticCloudComp)
	{
		auto owner = pVolumeticCloudComp->GetOwner();
		UMaterialInterface* CloudMaterialInterface = pVolumeticCloudComp->Material; // Assuming the cloud material is at index 0
		bool param_exists = false;
		if (DoesScalarParameterExist(CloudMaterialInterface, FName(*ParamNameEnableIR))) {
			param_exists = true;

		}

	
		UMaterialInstanceDynamic* dyn_mat = Cast<UMaterialInstanceDynamic>(CloudMaterialInterface);
		if (dyn_mat == nullptr) {
			dyn_mat = UMaterialInstanceDynamic::Create(CloudMaterialInterface, this);

		}

		if (dyn_mat)
		{
			// Now you can modify parameters of the dynamic material instance
			SetMaterialParams(dyn_mat, GetTempratureKelvin(), true, false);
			// ... and then apply it back to the post process settings
			pVolumeticCloudComp->SetMaterial(dyn_mat);
		}
	}

}

void AThermalMaterialManager::UpdateMaterialCollections()
{
	UMaterialParameterCollectionInstance* p_collection_instance = GetWorld()->GetParameterCollectionInstance(ThermalMaterialParameterCollection);

	if (p_collection_instance)
	{
		float NewValue = 1.0f; // Replace with the value you want to set
		auto p_sys = ASystemManagerBase::GetInstance();
		double dbl;
		p_sys->GetTimeOfDayHr(0, dbl);
		
		p_collection_instance->SetScalarParameterValue(FName(*ParamNameTimeOfDayHr), dbl);

		
		p_sys->GetTimeOfDuskHr(0, dbl);
		p_collection_instance->SetScalarParameterValue(FName(*ParamNameTimeOfDuskHr), dbl);

		p_sys->GetTimeOfSunSetHr(0, dbl);
		p_collection_instance->SetScalarParameterValue(FName(*ParamNameTimeOfSunSetHr), dbl);


	}
}

void AThermalMaterialManager::UpdateHeatSources(UMaterialInstanceDynamic *p_inst)
{

	for (int i = 0; i < HeatSources.Num(); i++) {
		if (i <= 4) {
			auto heat_src = HeatSources[i];
			FVector4 vec4;
			auto loc = HeatSources[i]->GetActorLocation();
			vec4.X = loc.X;
			vec4.Y = loc.Y;
			vec4.Z = loc.Z;
			
			vec4.W = heat_src->GetTempratureKelvin();
			p_inst->SetVectorParameterValue(FName(*(ParamNameHeatSourceBase + CUtil::IntToString(i+1))), vec4);
		}
	}
}

void AThermalMaterialManager::UpdateSkyMaterialThermalBehaviour()
{
	FPostProcessSettings& PostProcessSettings = pPostProcessVolume->Settings;

	for (auto& Blendable : PostProcessSettings.WeightedBlendables.Array)
	{
		if (UMaterialInterface* MaterialInterface = Cast<UMaterialInterface>(Blendable.Object))
		{

		

			// Now you have a reference to the material interface
			// You can cast it to UMaterialInstance or UMaterialInstanceDynamic if needed

			UMaterialInstanceDynamic* dyn_mat = Cast<UMaterialInstanceDynamic>(MaterialInterface);
			if (dyn_mat == nullptr) {
				dyn_mat = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			
			}

			if (dyn_mat)
			{
				// Now you can modify parameters of the dynamic material instance
				SetMaterialParams(dyn_mat, GetTempratureKelvin(), false, false);
				// ... and then apply it back to the post process settings
				Blendable.Object = dyn_mat;
			}
			
		}
	}
}

void AThermalMaterialManager::UpdateActorThermalBehaviour(AActor* Actor)
{
	double temp_kelvin = GetTempratureKelvin();
	if (Actor->IsA<AActorBase>()) {
		AActorBase* p_actor_base = (AActorBase*)Actor;
		temp_kelvin = p_actor_base->GetTempratureKelvin();
	}

	// Now check for static mesh components in this actor
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	Actor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	for (UStaticMeshComponent* MeshComp : StaticMeshComponents)
	{
		// Do something with each static mesh component
		// For example, print the name of the mesh component
		if (MeshComp)
		{
			int32 NumMaterials = MeshComp->GetNumMaterials();

			
			for (int32 i = 0; i < NumMaterials; ++i)
			{
				UMaterialInterface* Material_if = MeshComp->GetMaterial(i);
				if (Material_if)
				{
					bool param_exists = false;
					if (DoesScalarParameterExist(Material_if, FName(*ParamNameEnableIR))) {
						param_exists = true;

					}
					if (param_exists) {


						// If the material is a dynamic instance, you can directly check parameters
						UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(Material_if);
						bool new_material = false;
						if (DynMaterial == nullptr) {
							DynMaterial = UMaterialInstanceDynamic::Create(Material_if, MeshComp);
							new_material = true;
						}
						else {
							new_material = false;
						}

						if (DynMaterial)
						{
							{
								SetMaterialParams(DynMaterial, temp_kelvin, false, true);
								if (new_material) {
									MeshComp->SetMaterial(i, DynMaterial);
								}
							}
						}
					}
				}
			}
		}
	}
}

void AThermalMaterialManager::SetMaterialParams(UMaterialInstanceDynamic* p_ins, double temprature_kelvin, bool enable_timeofday_sim, bool update_heat_source)
{


	switch (ThermalMode_) {
	case 	EThermalMode::ThermalModeVis:
		p_ins->SetScalarParameterValue(FName(*ParamNameEnableIR), 0);
		p_ins->SetScalarParameterValue(FName(*ParamNameEnableTimeOfDaySim), 0);
		break;
	case	EThermalMode::ThermalModeNIR:
	case	EThermalMode::ThermalModeSWIR:
	case	EThermalMode::ThermalModeMWIR:
	case	EThermalMode::ThermalModeLWIR:
		p_ins->SetScalarParameterValue(FName(*ParamNameEnableIR), 1);
		p_ins->SetScalarParameterValue(FName(*ParamNameTempratureKelvin), temprature_kelvin);
		p_ins->SetScalarParameterValue(FName(*ParamNameEnableTimeOfDaySim), enable_timeofday_sim ? 1 : 0);
		
		break;
	}

	double timeofdayhr;
	auto p_sys = ASystemManagerBase::GetInstance();
	p_sys->GetTimeOfDayHr(0, timeofdayhr);


	
	
	//p_ins->SetScalarParameterValue(FName(*ParamNameTimeOfDayHr), timeofdayhr);

	double timeofduskhr;
	p_sys->GetTimeOfDuskHr(0, timeofduskhr);
	//p_ins->SetScalarParameterValue(FName(*ParamNameTimeOfDuskHr), timeofduskhr);

	double timeofsunsethr;
	p_sys->GetTimeOfSunSetHr(0, timeofsunsethr);
	//p_ins->SetScalarParameterValue(FName(*ParamNameTimeOfSunSetHr), timeofsunsethr);


	float emissive_scale = 1;
	float tf = 0;
	float extinction_scale = 1;

	double mid_hour = 16;

	if (timeofdayhr >= 0 && timeofdayhr <= timeofduskhr) {
		tf = timeofdayhr / timeofduskhr;
		emissive_scale = FMath::Lerp(0.0001, 0.00001, tf);
		extinction_scale = FMath::Lerp(0.01, 0.1, tf);
		
	}
	else if (timeofdayhr >= timeofduskhr && timeofdayhr <= mid_hour) {
		tf = (timeofdayhr - timeofduskhr) / (mid_hour - timeofduskhr);
		emissive_scale = FMath::Lerp(0.0001, 0.01, tf);
		extinction_scale = FMath::Lerp(0.1, 0.5, tf);
	}
	else if (timeofdayhr >= mid_hour && timeofdayhr <= timeofsunsethr) {
		tf = (timeofdayhr - mid_hour) / (timeofsunsethr - mid_hour);
		emissive_scale = FMath::Lerp(0.01, 0.002, tf);
		extinction_scale = FMath::Lerp(0.5, 1, tf);
	}
	else if (timeofdayhr >= timeofsunsethr && timeofdayhr <= 24) {
		tf = (timeofdayhr - timeofsunsethr) / (24 - timeofsunsethr);
		emissive_scale = FMath::Lerp(0.002, 0.0001, tf);
		extinction_scale = FMath::Lerp(1, 0.01, tf);
	}
	if (EmissiveOverride_ == 0) {
		p_ins->SetScalarParameterValue(FName(*ParamNameTimedLightScale), 1 * emissive_scale);
	}
	else {
		p_ins->SetScalarParameterValue(FName(*ParamNameTimedLightScale), (float)EmissiveOverride_);
	}
	UpdateHeatSources(p_ins);

	
	//p_ins->SetScalarParameterValue("ExtinctionScale", (float)ExtinctionOverride_);
	

}

void AThermalMaterialManager::UpdateSpecialMaterials()
{
	for (auto Material_if : SpecialMaterials) {
		if (Material_if)
		{
			
		
		}
	}
}

void AThermalMaterialManager::OnPreStep(float DeltaTime)
{
	Super::OnPreStep(DeltaTime);
	HeatSources = ASystemManagerBase::GetInstance()->GetHeatSources();
}

bool AThermalMaterialManager::GetIsVisibleThermalMode()
{
	if (ThermalMode_ == EThermalMode::ThermalModeVis) {
		return true;
	}

	return false;
}

void AThermalMaterialManager::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);

	FString line;

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::ThermalMode, (INT32S)(ThermalMode_));
	p_save_loader->AddLine(line);

}

void AThermalMaterialManager::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);
	data.Add(CCLICommandManager::ThermalMode, (INT32S)(ThermalMode_));
	
}
