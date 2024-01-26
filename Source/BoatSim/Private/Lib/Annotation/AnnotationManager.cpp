// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Annotation/AnnotationManager.h"
#include "EngineUtils.h"
#include <Lib/SystemManager/SystemManagerBase.h>

void AAnnotationManager::BeginPlay()
{
	Super::BeginPlay();
	Rename((TEXT("annotation")));
}

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
		UpdateAnnotation();
		next_state = AnnotationStateWait;
		break;
	case AnnotationStateDisable:
		DisableAnnotation();
		UpdateAnnotation();
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
	IsAnnotationEnabled = true;
	
}

void AAnnotationManager::UpdateAnnotation()
{
	auto p_sys_manager = ASystemManagerBase::GetInstance();

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;

		// Now check for static mesh components in this actor
		TArray<UMeshComponent*> mesh_components;
		
		auto p_actor_base = p_sys_manager->ToActorBase(Actor);

		if (p_actor_base) {
			p_actor_base->UpdateAnnotation(IsAnnotationEnabled);
		}
		else {
			
			//AActorBase::HandleAnnotation(Actor, IsAnnotationEnabled, EReservedAnnotationId::Unassigned);
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

void AAnnotationManager::OnStepScenarioMode(float DeltaTime)
{
	Super::OnStepScenarioMode(DeltaTime);
	StateMachine();
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
