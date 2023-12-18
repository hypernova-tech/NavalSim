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
	BoatCam = Cast<UCameraComponent>(GetComponentByClass<UCameraComponent>());
	pLastData = NewObject< UPlatformKinematicData>();

	
}

// Called every frame
void ACBoatBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TimeSec += DeltaTime;
	FVector forward;
	forward = GetActorForwardVector();
	if (IsForward) {

		FVector pos = GetActorLocation();

		pos += forward * DeltaTime * MovementSpeedUnitPerSec;
		SetActorLocation(pos);
	

	}
	else if (IsBackward) {

		FVector pos = GetActorLocation();

		pos -= forward * DeltaTime * MovementSpeedUnitPerSec;
		SetActorLocation(pos);

	}

	if (IsLeft) {

		forward = GetActorForwardVector();
		FQuat rot_qua = FQuat::MakeFromEuler(FVector(0.0f, 0.0f, 45 * DeltaTime)); // The rotation quaternion
		forward = rot_qua.RotateVector(forward); // Rotate the vector
		FRotator new_rot = forward.Rotation();
		SetActorRotation(new_rot);

	}
	else if (IsRight) {
		forward = GetActorForwardVector();
		FQuat rot_qua = FQuat::MakeFromEuler(FVector(0.0f, 0.0f, -45 * DeltaTime)); // The rotation quaternion
		forward = rot_qua.RotateVector(forward); // Rotate the vector
		FRotator new_rot = forward.Rotation();
		SetActorRotation(new_rot);
	}

#if true
	if (BoatCam != nullptr) {
		if (bUpKeyPressed && bIsRightMousePressed) {

		   
			FVector cam_forward = BoatCam->GetForwardVector();
			FVector cam_pos = BoatCam->GetComponentLocation();

			BoatCam->SetWorldLocation(cam_pos + cam_forward * DeltaTime * TOUE(CamMovementSpeed));


		}
		else if (bDownKeyPressed && bIsRightMousePressed) {
			FVector cam_forward = BoatCam->GetForwardVector();
			FVector cam_pos = BoatCam->GetComponentLocation();

			BoatCam->SetWorldLocation(cam_pos - cam_forward * DeltaTime * TOUE(CamMovementSpeed));
		}
	}
#endif

	
	Oscillate();
	UpdateKinematicData();
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
void ACBoatBase::BindedMouseX(float val)
{

	if (BoatCam && bIsRightMousePressed)
	{
		

		// Get the current rotation
		FRotator NewRotation = BoatCam->GetComponentRotation();

		// Adjust the yaw based on the rate of mouse movement
		FLOAT64 new_ang = NewRotation.Yaw + val;

		if (new_ang > 360) {
			new_ang = new_ang - 360;
		}
		else if (new_ang < 0) 
		{
			new_ang += 360;
		}
		NewRotation.Yaw = new_ang;

		// Set the updated rotation to the camera
		BoatCam->SetWorldRotation(NewRotation);

		
	}else if (BoatCam && bIsLeftMousePressed) {
		FVector cam_right = BoatCam->GetRightVector();
		FVector cam_pos = BoatCam->GetComponentLocation();

		BoatCam->SetWorldLocation(cam_pos + cam_right * val * (CamMovementSpeed));
	}
	
}

void ACBoatBase::BindedMouseY(float val)
{

	if (BoatCam && bIsRightMousePressed)
	{
		
		
		
		FQuat QuatRotation = BoatCam->GetComponentQuat();

		FQuat DeltaRotation = FQuat(FRotator(-val,0,0));
		QuatRotation *= DeltaRotation;
		BoatCam->SetRelativeRotation(QuatRotation.Rotator());

		return;
		
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
	FVector cam_forward = BoatCam->GetForwardVector();
	FVector cam_pos = BoatCam->GetComponentLocation();

	BoatCam->SetWorldLocation(cam_pos + cam_forward * val * CamMovementSpeed);
}

void ACBoatBase::OnFocusEnter()
{
	auto p_selected = ASystemManagerBase::GetInstance()->GetSelectedActor();

	if (p_selected) {
		FocusCamera(p_selected);
	}
}
void ACBoatBase::TopView()
{
	if (pFocusedActor) {
		BoatCam->SetWorldLocation(pFocusedActor->GetActorLocation() + FVector::UpVector * TOUE(3*FocusDistanceMeter));
		CUtil::CameraLookAt(BoatCam, BoatCam->GetComponentLocation() - FVector::UpVector * TOUE(FocusDistanceMeter));
	}
	else {
		CUtil::CameraLookAt(BoatCam, BoatCam->GetComponentLocation() - FVector::UpVector * TOUE(FocusDistanceMeter));
	}
	

}
void ACBoatBase::LeftView()
{
	if (pFocusedActor) {
		BoatCam->SetWorldLocation(FVector::UpVector * TOUE(5) + pFocusedActor->GetActorLocation() + FVector::LeftVector * TOUE(3 * FocusDistanceMeter));
		CUtil::CameraLookAt(BoatCam, BoatCam->GetComponentLocation() - FVector::LeftVector * TOUE(FocusDistanceMeter));
	}
	else {
		CUtil::CameraLookAt(BoatCam, BoatCam->GetComponentLocation() - FVector::LeftVector * TOUE(FocusDistanceMeter));
	}
}
void ACBoatBase::RightView()
{
	if (pFocusedActor) {
		BoatCam->SetWorldLocation(FVector::UpVector * TOUE(5) + pFocusedActor->GetActorLocation() + FVector::RightVector * TOUE(3 * FocusDistanceMeter));
		CUtil::CameraLookAt(BoatCam, BoatCam->GetComponentLocation() - FVector::RightVector * TOUE(FocusDistanceMeter));
	}
	else {
		CUtil::CameraLookAt(BoatCam, BoatCam->GetComponentLocation() - FVector::RightVector * TOUE(FocusDistanceMeter));
	}
}
void ACBoatBase::Perpective()
{
}
bool ACBoatBase::FocusCamera(AActor* p_actor)
{
	CUtil::CameraLookAt(BoatCam, p_actor,  TOUE(FocusDistanceMeter));
	pFocusedActor = p_actor;
	return true;
}




void ACBoatBase::Update(UCSOAObserverArgs* p_args)
{
	

	if (p_args->GetSubjectId() == CommonSOAObservers::PlatformKinematicObserverId) {
		UPlatformKinematicData* p_kinematic = Cast<UPlatformKinematicData>(p_args);
		
		if (p_kinematic != nullptr) {
			
			
			//Mutex.Lock();
			pLastData = p_kinematic;
			//p_kinematic->Copy(pLastData);
			IsDirty = true;
			//Mutex.Unlock();
			UpdateKinematicData();

		}
	}

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
	}

}

ECamView ACBoatBase::GetCamView()
{
	return ECamView();
}

void ACBoatBase::OnControllerChanged()
{
	BoatCam = Cast<UCameraComponent>(GetComponentByClass<UCameraComponent>());
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
	PlayerInputComponent->BindAxis("RotationX", this, &ACBoatBase::BindedMouseX).bConsumeInput = false;
	PlayerInputComponent->BindAxis("RotationY", this, &ACBoatBase::BindedMouseY).bConsumeInput = false;
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
}