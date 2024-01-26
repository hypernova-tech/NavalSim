// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/ActorBase/ActorBase.h"
#include "AnnotationManager.generated.h"


enum EAnnotationState
{
	AnnotationStateIdle,
	AnnotationStateEnable,
	AnnotationStateDisable,
	AnnotationStateWait
};


enum EAnnotationMode
{
	AnnotationModeUnknown,
	AnnotationModeEnable,
	AnnotationModeDisable,
};

enum EReservedAnnotationId
{
	Sea = 1,
	Sky = 2,
	Land = 3,
	Unassigned = 255
};
/**
 * 
 */
UCLASS()
class AAnnotationManager : public AActorBase
{
	GENERATED_BODY()
private: 


	UPROPERTY(EditAnywhere)
		bool IsAnnotationEnabled = false;

	EAnnotationMode  AnnotationModeRequest;

protected:
	EAnnotationState State = EAnnotationState::AnnotationStateIdle;
	virtual void BeginPlay() override;
	virtual void StateMachine();
	virtual void EnableAnnotation();
	void UpdateAnnotation();
	virtual void DisableAnnotation();

	UFUNCTION(BlueprintCallable)
		virtual void SetAnnotationEnabled_(bool val);

public:
	virtual void OnStepScenarioMode(float DeltaTime) override;
	virtual void OnStep(float DeltaTime) override;
	bool GetIsAnnotationEnabled();

};
