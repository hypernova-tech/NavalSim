// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/UI/GizmoUIController.h"
#include <Lib/Types/Primitives.h>
#include <Lib/Utils/CUtil.h>
#include <Lib/Math/CMath.h>
#include <Lib/InputManager/InputManagerBase.h>

// Sets default values for this component's properties
UGizmoUIController::UGizmoUIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGizmoUIController::BeginPlay()
{
	Super::BeginPlay();
	AInputManagerBase::GetInstance()->ShowInputInterest(this);

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{

		PC->InputComponent->BindAxis("RotationX", this, &UGizmoUIController::HandleMouseX).bConsumeInput = false;
	
		PC->InputComponent->BindAxis("RotationY", this, &UGizmoUIController::HandleMouseY).bConsumeInput = false;


		PC->InputComponent->BindAction("GizmoMove", IE_Repeat, this, &UGizmoUIController::HandleMove).bConsumeInput = false;
		PC->InputComponent->BindAction("GizmoRotate", IE_Repeat, this, &UGizmoUIController::HandleRotate).bConsumeInput = false;
		PC->InputComponent->BindAction("GizmoScale", IE_Repeat, this, &UGizmoUIController::HandleScale).bConsumeInput = false;

	}
	// ...
	
}


void UGizmoUIController::HandleMove()
{
	GizmoMode = EGizmoMode::GizmoModeMove;
}
void UGizmoUIController::HandleScale()
{
	GizmoMode = EGizmoMode::GizmoModeScale;
}
void UGizmoUIController::HandleRotate()
{
	GizmoMode = EGizmoMode::GizmoModeRotate ;
}



void UGizmoUIController::HandleMouseX(float val)
{
	if (val != 0) {
		val = val;
	}
	LastMouseDeltaX = val;
	CUtil::DebugLog("LastMouseDeltaX " + CUtil::FloatToString(val));
}

void UGizmoUIController::HandleMouseY(float val)
{
	LastMouseDeltaY = val;

}

// Called every frame
void UGizmoUIController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	StateMachine();
	// ...
}

void UGizmoUIController::OnMouseLeftButtonDown(int locationX, int locationY)
{
	IsMouseLeftButtonDown = true;
	IsMouseLeftButtonUp = false;

	MouseLeftButtonDownLocationX = locationX;
	MouseLeftButtonDownLocationY = locationY;

}

void UGizmoUIController::OnMouseLeftButtonUp(int locationX, int locationY)
{
	IsMouseLeftButtonDown = false;
	IsMouseLeftButtonUp = true;

	MouseLeftButtonUpLocationX = locationX;
	MouseLeftButtonUpLocationY = locationY;
}

void UGizmoUIController::SetGizmoActor(AActor* p_actor)
{
	pGizmoActor = (AAxisGizmo*)p_actor;
}

void UGizmoUIController::OnCursorMove()
{
	if (pTrackedActor == nullptr) {
		return;
	}
	auto curr_loc = pTrackedActor->GetActorLocation();
	FRotator rot = pTrackedActor->GetActorRotation();
	FVector2D mouse_drag_2d = FVector2D(LastMouseDeltaX, LastMouseDeltaY);
	FVector mouse_drag = CMath::ToVec3(mouse_drag_2d);
	mouse_drag.Normalize();
	auto pc = GetWorld()->GetFirstPlayerController();
	FVector screen_space_dir;
	if (GizmoMode == EGizmoMode::GizmoModeMove) {
		FLOAT32 move_strength = 250;
		FLOAT32 delta;
		FLOAT32 disp = FVector2D(LastMouseDeltaX, LastMouseDeltaY).Length();
		switch (CurrAxis) {
		case ECoordAxis::CoordAxisX:
			screen_space_dir = CMath::ProjectWorldDirectionToScreenSpace(pc, pTrackedActor->GetActorForwardVector());
			screen_space_dir.Normalize();
			delta = screen_space_dir.Dot(mouse_drag * disp * move_strength);
			pTrackedActor->SetActorLocation(curr_loc + pTrackedActor->GetActorForwardVector()* delta);
			break;

		case ECoordAxis::CoordAxisY:
			screen_space_dir = CMath::ProjectWorldDirectionToScreenSpace(pc, pTrackedActor->GetActorRightVector());
			screen_space_dir.Normalize();
			delta = screen_space_dir.Dot(mouse_drag * disp * move_strength);
			pTrackedActor->SetActorLocation(curr_loc + pTrackedActor->GetActorRightVector() * delta);
			break;

		case ECoordAxis::CoordAxisZ:
			screen_space_dir = CMath::ProjectWorldDirectionToScreenSpace(pc, pTrackedActor->GetActorUpVector());
			screen_space_dir.Normalize();
			delta = screen_space_dir.Dot(mouse_drag * disp * move_strength);
			pTrackedActor->SetActorLocation(curr_loc + pTrackedActor->GetActorUpVector() * delta);

			break;
		}

	}
	else if (GizmoMode == EGizmoMode::GizmoModeRotate) {
	  
		FLOAT32 delta = LastMouseDeltaX;
		FLOAT32 val = LastMouseDeltaX * 4;
		

		switch (CurrAxis) {
		case ECoordAxis::CoordAxisX:


			rot.Roll += val;

			// Set the updated rotation to the camera
			pTrackedActor->SetActorRotation(rot);
			break;

		case ECoordAxis::CoordAxisY:

			rot.Pitch += val;

			// Set the updated rotation to the camera
			pTrackedActor->SetActorRotation(rot);
			break;

		case ECoordAxis::CoordAxisZ:


			// Adjust the yaw based on the rate of mouse movement
			rot.Yaw += val;

			// Set the updated rotation to the camera

			break;
		}
	}
	else if (GizmoMode == EGizmoMode::GizmoModeScale) {
		auto drag = AInputManagerBase::GetInstance()->GetLeftButtonDrag();
		auto tf = drag.X / 512.0;
		CUtil::DebugLog("Drag: " + CUtil::FloatToString(tf));


		auto scale = tf;

		
		pTrackedActor->SetActorScale3D((1+scale) * TrackedActorIntitialScale);

	}

		
	pTrackedActor->SetActorRotation(rot);
	pGizmoActor->SetActorLocation(pTrackedActor->GetActorLocation());
	pGizmoActor->SetActorRotation(pTrackedActor->GetActorRotation());
	


}

void UGizmoUIController::ShowGizmoActor(bool val)
{
	CUtil::SetActorActive(pGizmoActor, val);
}

void UGizmoUIController::SetGizmoMode(EGizmoMode mode)
{
	GizmoMode = mode;
}

AActor* UGizmoUIController::FindGizmoAtClickPosition(int locationX, int locationY)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector WorldLocation;
	FVector WorldDirection;

	if (PlayerController)
	{
		PlayerController->DeprojectScreenPositionToWorld(
			locationX, locationY,
			WorldLocation,
			WorldDirection
		);
	}

	FHitResult HitResult;

	// Define start and end points of the ray
	FVector Start = WorldLocation;
	FVector End = Start + (WorldDirection * TOUE(1000));  // MaxTraceDistance is up to you, e.g., 10000.0f

	CUtil::DrawDebugRay(GetWorld(), Start, End, FColor::Red, 10, 0.75);

	// Perform the line trace
	// The ECC_Visibility channel will typically work, but you can change this if necessary
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_GameTraceChannel1
	);

	if (bHit)
	{
		// HitResult now contains information about what was hit
		AActor* ClickedActor = HitResult.GetActor();

		if (ClickedActor)
		{
			CurrAxis = pGizmoActor->GetAxis(HitResult.GetComponent());
			CUtil::DebugLogScreen(ClickedActor->GetName());
			return ClickedActor;


		}
	}
	else {

	}

	return nullptr;
}

void UGizmoUIController::MoveWithCursor()
{

}

void UGizmoUIController::SetTrackedActor(AActor *p_tracked)
{
	pTrackedActor = p_tracked;
	pGizmoActor->SetActorLocation(pTrackedActor->GetActorLocation());
	pGizmoActor->SetActorRotation(pTrackedActor->GetActorRotation());
	TrackedActorIntitialScale = pTrackedActor->GetActorScale3D();
}
void UGizmoUIController::StateMachine()
{
	auto current_state = GizmoState;
	auto next_state = current_state;

	switch (current_state)
	{
	case GizmoStateIdle:

		if (pGizmoActor &&pTrackedActor) {
			next_state = GizmoStateCheckInput;
		}
		break;

	case GizmoStateCheckInput:
		if (IsMouseLeftButtonUp) {
			next_state = GizmoStateDetect;
		}
		break;
	case GizmoStateDetect:
		if (IsMouseLeftButtonDown) {
			auto ret = FindGizmoAtClickPosition(MouseLeftButtonDownLocationX, MouseLeftButtonDownLocationY);
			if (ret) {
				next_state = GizmoStateMoving;
			}
		}
	
		break;
	case GizmoStateMoving:
		
		if (IsMouseLeftButtonUp) {
			next_state = GizmoStateDetect;
		}
		else {
			OnCursorMove();
		}
			
		break;
	default:
		break;
	}
	GizmoState = next_state;

}