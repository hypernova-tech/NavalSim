// Fill out your copyright notice in the Description page of Project Settings.


#include "CBoatBase.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Math.h"
#include "Camera/CameraComponent.h"
#include <Lib/SOA/SOAImplementor.h>
#include <Lib/SOA/CommonSOAObservers.h>
#include <Lib/SOACommonObserverArgs/PlatformKinematicData.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Gimbal/GimbalBase.h>
#include <Lib/Utils/CUtil.h>
#include <Lib/Math/CMath.h>

// Sets default values
ACBoatBase::ACBoatBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACBoatBase::BeginPlay()
{
	CMath::UnitTestVec();
	Super::BeginPlay();
	ASOAImplementor::GetInstance()->Subscribe(CommonSOAObservers::PlatformKinematicObserverId,this);
	pCam = Cast<UCameraComponent>(GetComponentByClass<UCameraComponent>());
	pLastData = NewObject< UPlatformKinematicData>();
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
	
}

// Called every frame
void ACBoatBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TimeSec += DeltaTime;
	FVector forward;

	if (pTarget != nullptr) {

		forward = pTarget->GetActorForwardVector();
		if (IsForward) {

			FVector pos = pTarget->GetActorLocation();

			pos += forward * DeltaTime * MovementSpeedUnitPerSec;
			pTarget->SetActorLocation(pos);


		}
		else if (IsBackward) {

			FVector pos = pTarget->GetActorLocation();

			pos -= forward * DeltaTime * MovementSpeedUnitPerSec;
			pTarget->SetActorLocation(pos);

		}

		if (IsLeft) {

			forward = pTarget->GetActorForwardVector();
			FQuat rot_qua = FQuat::MakeFromEuler(FVector(0.0f, 0.0f, 45 * DeltaTime)); // The rotation quaternion
			forward = rot_qua.RotateVector(forward); // Rotate the vector
			FRotator new_rot = forward.Rotation();
			pTarget->SetActorRotation(new_rot);

		}
		else if (IsRight) {
			forward = pTarget->GetActorForwardVector();
			FQuat rot_qua = FQuat::MakeFromEuler(FVector(0.0f, 0.0f, -45 * DeltaTime)); // The rotation quaternion
			forward = rot_qua.RotateVector(forward); // Rotate the vector
			FRotator new_rot = forward.Rotation();
			pTarget->SetActorRotation(new_rot);
		}
	}
	

#if true
	if (pCam != nullptr) {
		if (bUpKeyPressed && bIsRightMousePressed) {

		   
			FVector cam_forward = pCam->GetForwardVector();
			FVector cam_pos = pCam->GetComponentLocation();

			pCam->SetWorldLocation(cam_pos + cam_forward * DeltaTime * TOUE(CamMovementSpeed));


		}
		else if (bDownKeyPressed && bIsRightMousePressed) {
			FVector cam_forward = pCam->GetForwardVector();
			FVector cam_pos = pCam->GetComponentLocation();

			pCam->SetWorldLocation(cam_pos - cam_forward * DeltaTime * TOUE(CamMovementSpeed));
		}
	}
#endif

	UpdateCamTransform(DeltaTime);
	Oscillate();

}


void ACBoatBase::OnForwardKey(bool pressed, bool released)
{
	if (pressed) {
		IsForward = true;
	}
	else {
		if (released) {
			IsForward = false;
		}
	}
}
void ACBoatBase::OnBackwardkKey(bool pressed, bool released)
{
	if (pressed) {
		IsBackward = true;
	}
	else {
		if (released) {
			IsBackward = false;
		}
	}
}
void ACBoatBase::OnLeftKey(bool pressed, bool released)
{
	if (pressed) {
		IsLeft = true;
	}
	else {
		if (released) {
			IsLeft = false;
		}
	}
}
void ACBoatBase::OnRightKey(bool pressed, bool released)
{
	if (pressed) {
		IsRight = true;
	}
	else {
		if (released) {
			IsRight = false;
		}
	}
}




void ACBoatBase::StopPawn()
{
	UPawnMovementComponent* p_comp = GetMovementComponent();
	if (p_comp) {
		p_comp->StopMovementImmediately();
	}

	IsForward = false;
	IsBackward = false;
	IsLeft = false;
	IsRight = false;



}


void ACBoatBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
void ACBoatBase::BindedMoveForward(float val)
{
	
}
void ACBoatBase::BindedMoveRight(float val)
{

}
void ACBoatBase::BindedMouseMoveX(float val)
{

	if (val == 0) {
		return;
	}

	if (pCam && bIsRightMousePressed)
	{
	
		auto p_attached = pCam->GetAttachParentActor();
		
		// Get the current rotation
		FRotator curr_rot = pCam->GetRelativeRotation();
		// Adjust the yaw based on the rate of mouse movement
		FLOAT64 new_ang = curr_rot.Yaw + val;

		curr_rot.Yaw = new_ang;
		// Set the updated rotation to the camera
		pCam->AddRelativeRotation(FRotator(0, 1.3*val, 0));
		// Set the updated rotation to the camera
		InitializeCamRelativeTransform();
		
		
	}else if (pCam && bIsLeftMousePressed) {

		if (!ASystemManagerBase::GetInstance()->IsGizmoActorMoving()) {
			FVector cam_right = pCam->GetRightVector();
			FVector cam_pos = pCam->GetComponentLocation();

			pCam->SetWorldLocation(cam_pos + cam_right * val * (CamMovementSpeed));
			InitializeCamRelativeTransform();
			
		}
		else {
			
		}
		
	
	}
	
}

void ACBoatBase::BindedMouseMoveY(float val)
{
	if (val == 0) {
		return;
	}
	if (pCam && bIsRightMousePressed)
	{

	
		FQuat QuatRotation = pCam->GetComponentQuat();

		FQuat DeltaRotation = FQuat(FRotator(-1.3*val,0,0));
		QuatRotation *= DeltaRotation;
		pCam->SetWorldRotation(QuatRotation.Rotator());
		InitializeCamRelativeTransform();
		
		
	}
	else if (pCam && bIsLeftMousePressed) {

		if (!ASystemManagerBase::GetInstance()->IsGizmoActorMoving()) {
			FVector cam_vec = pCam->GetUpVector();
			FVector cam_pos = pCam->GetComponentLocation();

			pCam->SetWorldLocation(cam_pos + cam_vec * val * (CamMovementSpeed));
			InitializeCamRelativeTransform();
			
		}
		else {
			
		}

	}
}

void ACBoatBase::OnRightMousePressed()
{
	bIsRightMousePressed = true;
}

void ACBoatBase::OnRightMouseReleased()
{
	bIsRightMousePressed = false;
}
void ACBoatBase::OnLeftMousePressed()
{
	bIsLeftMousePressed = true;
}
void ACBoatBase::OnLeftMouseReleased()
{
	bIsLeftMousePressed = false;
}
void ACBoatBase::OnUpKeyPressed()
{
	bUpKeyPressed = true;
}
void ACBoatBase::OnUpKeyReleased()
{
	bUpKeyPressed = false;
}
void ACBoatBase::OnDownKeyPressed()
{
	bDownKeyPressed = true;
}
void ACBoatBase::OnDownKeyReleased()
{
	bDownKeyPressed = false;
}
void ACBoatBase::AdjustCameraDistance(float val)
{
	if (val == 0) {
		return;
	}
	FVector cam_forward = pCam->GetForwardVector();
	FVector cam_pos = pCam->GetComponentLocation();
	pCam->SetWorldLocation(cam_pos + cam_forward * val * CamMovementSpeed);
	InitializeCamRelativeTransform();
}

void ACBoatBase::OnFocusEnter()
{
	auto p_selected = ASystemManagerBase::GetInstance()->GetSelectedActor();

	if (p_selected) {
		FocusCamera(p_selected);
	}
}

void ACBoatBase::AdjustCamView(FVector view_dir)
{
	if (pFocusedActor) {
		pCam->SetWorldLocation(pFocusedActor->GetActorLocation() - view_dir * TOUE(FocusDistanceCamViewScale * FocusDistanceMeter));
		CUtil::CameraLookAt(pCam, pCam->GetComponentLocation() + view_dir * TOUE(FocusDistanceMeter));
	}
	else {
		CUtil::CameraLookAt(pCam, pCam->GetComponentLocation() + view_dir * TOUE(FocusDistanceMeter));
	}
	InitializeCamRelativeTransform();
}

void ACBoatBase::CamLookAtTarget()
{
	
}

FTransform* ACBoatBase::GetCamRelativeOffset()
{
	return &RelativeTransform;
}

void ACBoatBase::SetCamRelativeOffsetTranslation(FVector val)
{
	RelativeTransform.SetTranslation(val);
}

FVector ACBoatBase::GetCamRelativeOffsetTranslation()
{
	return RelativeTransform.GetTranslation();
}

void ACBoatBase::SetCamRelativeOffsetRPYDeg(FVector val)
{

	FRotator Rotator = FRotator(val.Y, val.Z, val.X);
	RelativeTransform.SetRotation(Rotator.Quaternion());
}

FVector ACBoatBase::GetCamRelativeOffsetRPYDeg()
{
	FRotator Rotator = RelativeTransform.GetRotation().Rotator();
	return FVector(Rotator.Roll, Rotator.Pitch, Rotator.Yaw);
}

AActor* ACBoatBase::GetTarget()
{
	return pTarget;
}

void ACBoatBase::SetTarget(AActor* p_val)
{
	pTarget = p_val; 
	pFocusedActor = pTarget;
	FocusCamera(pTarget);
	InitializeCamRelativeTransform();
	
}
void ACBoatBase::TopView()
{


	AdjustCamView(-FVector::UpVector);
	

}
void ACBoatBase::LeftView()
{
	AdjustCamView(-FVector::LeftVector);


}
void ACBoatBase::RightView()
{
	AdjustCamView(-FVector::RightVector);


}

void ACBoatBase::FrontView()
{
	AdjustCamView(-FVector::ForwardVector);
}
void ACBoatBase::BackView()
{
	AdjustCamView(FVector::ForwardVector);
}
void ACBoatBase::Perpective()
{
}

void ACBoatBase::InitializeCamRelativeTransform()
{
	if (pCam && pTarget)
	{
		// Get the world transform of both the camera and the target
		FTransform CameraWorldTransform = pCam->GetComponentTransform();
		FTransform TargetWorldTransform = pTarget->GetActorTransform();

		// Calculate the relative transform of the camera to the target
		RelativeTransform = CameraWorldTransform.GetRelativeTransform(TargetWorldTransform);
	}
}
void ACBoatBase::UpdateCamTransform(float delta_time)
{

	if (pTarget && pCam)
	{
		FTransform TargetTransform = pTarget->GetActorTransform();
		FTransform NewCameraTransform = RelativeTransform * TargetTransform;
		if (InterpCam) {
			FTransform interped;
			CMath::SmoothTransformLerp(pCam->GetComponentTransform(), NewCameraTransform, delta_time, CamInterpSpeed, interped);
			pCam->SetWorldLocationAndRotation(interped.GetLocation(), interped.GetRotation());

		}
		else {
			pCam->SetWorldLocationAndRotation(NewCameraTransform.GetLocation(), NewCameraTransform.GetRotation());
		}
	
	}
}
bool ACBoatBase::FocusCamera(AActor* p_actor)
{
	CUtil::CameraLookAt(pCam, p_actor,  TOUE(FocusDistanceMeter));
	pFocusedActor = p_actor;
	InitializeCamRelativeTransform();
	return true;
}


void ACBoatBase::UpdateKinematicData()
{
	bool should_unlock = true;
	//Mutex.Lock();
	if (IsDirty) {
#if true
		AMapOrigin* p_origin = ASystemManagerBase::GetInstance()->GetMapOrigin();
		
		FVector llh = pLastData->GetLocationLLH();
		p_origin->ChangeCenterCoordinateOnce(llh);
		FVector new_pos = p_origin->ConvertLLHToUEXYZ(llh);
		FVector euler = pLastData->GetEulerRPYDeg();
		
		//Mutex.Unlock();
		should_unlock = false;
#endif
		IsDirty = false;
		
		//Theta += 1 / 60.0 * AnglueSpeedDegPerSec;
		//FVector pos = R * (FMath::Cos(Theta * PI / 180) * FVector::RightVector + FMath::Cos(Theta * PI / 180) * FVector::ForwardVector);
		//FVector rot = FVector(0,0 , Theta);
	

		SetActorLocation(new_pos);
		SetActorRotation(FRotator(euler.Y, euler.Z, euler.X));
	
		
	}
	if (should_unlock) {
		//Mutex.Unlock();
	}
	
}

void ACBoatBase::SetCamView(ECamView view)
{
	switch (view) {
	case ECamView::CamViewTop:
		TopView();
		break;


	case ECamView::CamViewLeft:
		LeftView();
		break;

	case ECamView::CamViewRight:
		RightView();
		break;
	case ECamView::CamViewFront:
		FrontView();
		break;
	case ECamView::CamViewBack:
		BackView();
		break;
	}

}

ECamView ACBoatBase::GetCamView()
{
	return ECamView();
}

void ACBoatBase::OnControllerChanged()
{
	pCam = Cast<UCameraComponent>(GetComponentByClass<UCameraComponent>());
}

void ACBoatBase::Oscillate()
{
	return;
	FVector pos = GetActorLocation();
	float oscillation = OscillationAmplitude* FMath::Sin(2 * PI * TimeSec * OscillationFreq);
		
	pos += FVector::UpVector * oscillation;
	SetActorLocation(pos);
}

void ACBoatBase::SetMaxSpeedMeterPerSec(FLOAT32 speed)
{
	MovementSpeedUnitPerSec = TOUE( speed) ;
}

FLOAT32 ACBoatBase::GetMaxSpeedMeterPerSec()
{
	return TOW(MovementSpeedUnitPerSec);
}

void ACBoatBase::SetTurnRateDegPerSec(FLOAT32 val)
{
	TurnRateDegPerSec = val;
}

FLOAT32 ACBoatBase::GetTurnRateDegPerSec()
{
	return TurnRateDegPerSec;
}

// Called to bind functionality to input
void ACBoatBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	FInputAxisBinding axis_binding;



	PlayerInputComponent->BindAxis("MoveForward", this, &ACBoatBase::BindedMoveForward).bConsumeInput = false;
	PlayerInputComponent->BindAxis("MoveRight", this, &ACBoatBase::BindedMoveRight).bConsumeInput = false;
	PlayerInputComponent->BindAction("RightMouseButtonClick", IE_Pressed, this, &ACBoatBase::OnRightMousePressed).bConsumeInput = false;
	PlayerInputComponent->BindAction("RightMouseButtonClick", IE_Released, this, &ACBoatBase::OnRightMouseReleased).bConsumeInput = false;
	PlayerInputComponent->BindAction("LeftMouseButtonClick", IE_Pressed, this, &ACBoatBase::OnLeftMousePressed).bConsumeInput = false;
	PlayerInputComponent->BindAction("LeftMouseButtonClick", IE_Released, this, &ACBoatBase::OnLeftMouseReleased).bConsumeInput = false;
	PlayerInputComponent->BindAction("UpKey", IE_Pressed, this, &ACBoatBase::OnUpKeyPressed).bConsumeInput = false;
	PlayerInputComponent->BindAction("UpKey", IE_Released, this, &ACBoatBase::OnUpKeyReleased).bConsumeInput = false;
	PlayerInputComponent->BindAction("DownKey", IE_Pressed, this, &ACBoatBase::OnDownKeyPressed).bConsumeInput = false;
	PlayerInputComponent->BindAction("DownKey", IE_Released, this, &ACBoatBase::OnDownKeyReleased).bConsumeInput = false;

	PlayerInputComponent->BindAction("Focus", IE_Pressed, this, &ACBoatBase::OnFocusEnter).bConsumeInput = false;
	PlayerInputComponent->BindAxis("Zoom", this, &ACBoatBase::AdjustCameraDistance).bConsumeInput = false;
	PlayerInputComponent->BindAxis("PltRotationX", this, &ACBoatBase::BindedMouseMoveX).bConsumeInput = false;
	PlayerInputComponent->BindAxis("PltRotationY", this, &ACBoatBase::BindedMouseMoveY).bConsumeInput = false;


}