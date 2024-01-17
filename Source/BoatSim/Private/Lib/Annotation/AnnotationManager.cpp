// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Annotation/AnnotationManager.h"
#include "EngineUtils.h"

void AAnnotationManager::StateMachine()
{
	auto curr_state = State;
	auto next_state = curr_state;

	switch (curr_state)
	{
	case AnnotationStateIdle:
		if (AnnotationModeRequest == EAnnotationMode::AnnotationModeEnable) {
			next_state = AnnotationStateEnable;
		}
		else if (AnnotationModeRequest == EAnnotationMode::AnnotationModeDisable) {
			next_state = AnnotationStateDisable;
		}
		break;
	case AnnotationStateEnable:
		EnableAnnotation();
		AnnotationModeRequest = EAnnotationMode::AnnotationModeUnknown;
		next_state = AnnotationStateWait;
		break;
	case AnnotationStateDisable:
		DisableAnnotation();
		AnnotationModeRequest = EAnnotationMode::AnnotationModeUnknown;
		next_state = AnnotationStateWait;
		break;
	case AnnotationStateWait:
		if (AnnotationModeRequest == EAnnotationMode::AnnotationModeEnable) {
			next_state = AnnotationStateEnable;
		}
		else if (AnnotationModeRequest == EAnnotationMode::AnnotationModeDisable) {
			next_state = AnnotationStateDisable;
		}
	default:
		break;
	}

	State = next_state;
}

void AAnnotationManager::EnableAnnotation()
{

	float UseAnnotationValue;
	IsAnnotationEnabled = true;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;

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
						// If the material is a dynamic instance, you can directly check parameters
						UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(Material_if);
						if (DynMaterial)
						{
						
							if (DynMaterial->GetScalarParameterValue(FName("UseAnnotation"), UseAnnotationValue))
							{
								if (UseAnnotationValue)
								{
									
								}
							}
						}
					}
					else {


						UMaterialInterface* MaterialInterface = MeshComp->GetMaterial(i);
						if (MaterialInterface)
						{
							// Try casting to UMaterial or UMaterialInstance
							UMaterial* Material = Cast<UMaterial>(MaterialInterface);
							UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(MaterialInterface);

							if (Material)
							{
								if (Material->GetScalarParameterValue(FName("UseAnnotation"), UseAnnotationValue))
								{
									if (UseAnnotationValue)
									{

									}
								}
							}
							else if (MaterialInstance)
							{
								if (MaterialInstance->GetScalarParameterValue(FName("UseAnnotation"), UseAnnotationValue))
								{
									if (UseAnnotationValue)
									{

									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void AAnnotationManager::DisableAnnotation()
{
	IsAnnotationEnabled = false;
}

void AAnnotationManager::SetAnnotationEnabled_(bool val)
{
	if (val) {
		AnnotationModeRequest = EAnnotationMode::AnnotationModeEnable;
	}
	else {
		AnnotationModeRequest = EAnnotationMode::AnnotationModeDisable;
	}
}

void AAnnotationManager::OnStep(float DeltaTime)
{
	Super::OnStep(DeltaTime);
	StateMachine();
}

bool AAnnotationManager::GetIsAnnotationEnabled()
{
	return IsAnnotationEnabled;
}
