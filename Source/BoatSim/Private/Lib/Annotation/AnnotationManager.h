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
	virtual void StateMachine();
	virtual void EnableAnnotation();
	virtual void DisableAnnotation();

	UFUNCTION(BlueprintCallable)
		virtual void SetAnnotationEnabled_(bool val);

public:
	virtual void OnStep(float DeltaTime) override;
	bool GetIsAnnotationEnabled();

};
