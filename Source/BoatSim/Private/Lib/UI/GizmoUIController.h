// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Lib/Gizmo/AxisGizmo.h>
#include "GizmoUIController.generated.h"


enum EGizmoState
{
	GizmoStateIdle,
	GizmoStateCheckInput,
	GizmoStateDetect,
	GizmoStateMoving,
};
UENUM(BlueprintType)
enum EGizmoMode
{
	GizmoModeMove,
	GizmoModeRotate,
	GizmoModeScale,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGizmoUIController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGizmoUIController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	EGizmoState GizmoState = EGizmoState::GizmoStateIdle;

	virtual void StateMachine();

	
	AAxisGizmo* pGizmoActor;
	AActor* pTrackedActor;

	void MoveWithCursor();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
		void OnMouseLeftButtonDown(int locationX, int locationY);

	UFUNCTION(BlueprintCallable)
		void OnMouseLeftButtonUp(int locationX, int locationY);

	UFUNCTION(BlueprintCallable)
		void SetGizmoActor(AActor* p_actor);

	UFUNCTION(BlueprintCallable)
		void ShowGizmoActor(bool val);

	UFUNCTION(BlueprintCallable)
		void SetGizmoMode(EGizmoMode mode);

	

	void SetTrackedActor(AActor* p_actor);
	void OnCursorMove();
	AActor* FindGizmoAtClickPosition(int locationX, int locationY);

private:

	ECoordAxis CurrAxis;
	EGizmoMode GizmoMode = EGizmoMode::GizmoModeMove;

	bool IsMouseLeftButtonDown;
	bool IsMouseLeftButtonUp;

	int MouseLeftButtonDownLocationX;
	int MouseLeftButtonDownLocationY;

	int MouseLeftButtonUpLocationX;
	int MouseLeftButtonUpLocationY;

	FLOAT32 LastMouseDeltaX ;
	FLOAT32 LastMouseDeltaY;

	int MousePosXWhenClicked;
	int MousePosYWhenClicked;


	void HandleMouseX(float Value);
	void HandleMouseY(float Value);

	void HandleMove();
	void HandleScale();
	void HandleRotate();

	FVector TrackedActorIntitialScale;
};
