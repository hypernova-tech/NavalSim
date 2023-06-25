// Fill out your copyright notice in the Description page of Project Settings.


#include "CBoatBase.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Math.h"
// Sets default values
ACBoatBase::ACBoatBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACBoatBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACBoatBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSec += DeltaTime;
	FVector forward;
	forward = GetActorForwardVector();
	if (IsForward) {

		FVector pos = GetActorLocation();

		pos += forward * DeltaTime * MovementSpeedUnitPerSec;
		SetActorLocation(pos);
		//DoTrace();
#if false
		if (LidarComp) {
			if (GetWorld()->TimeSeconds >= NextLidarTimeSec) {
				LidarComp->Trace();
				NextLidarTimeSec = GetWorld()->TimeSeconds + 0.1f;
			}

		}
#endif
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
#if false
	LidarComp = (UCLidarComp*)GetComponentByClass(UCLidarComp::StaticClass());
	if (LidarComp)
	{

	}
#endif
}


// Called to bind functionality to input
void ACBoatBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

}

void ACBoatBase::Oscillate()
{
	FVector pos = GetActorLocation();
	float oscillation = OscillationAmplitude* FMath::Sin(2 * PI * TimeSec * OscillationFreq);
		
	pos += FVector::UpVector * oscillation;
	SetActorLocation(pos);
}

