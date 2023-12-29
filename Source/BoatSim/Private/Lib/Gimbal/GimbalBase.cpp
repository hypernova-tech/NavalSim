// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Gimbal/GimbalBase.h"
#include <Lib/Utils/CUtil.h>
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>
#include "CBoatBase.h"
// Sets default values
AGimbalBase::AGimbalBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void AGimbalBase::BeginPlay()
{
	Super::BeginPlay();

	GetRootComponent()->SetMobility(EComponentMobility::Movable);

#if false
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	
	for (AActor* Actor : Actors) {
		if (Actor->GetClass()->GetFName() == "CBoatBase") {
			Actor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			break;
		}
	}

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		// It->GetClass() returns the UClass of the actor
		if (It->GetClass()->GetName() == "ACBoatBase")
		{
			// You now have a pointer to the actor you were looking for
			AActor* FoundActor = *It;

			FoundActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			break;
		}
	}

#endif
}



void AGimbalBase::InitGimbal()
{
	
}

void AGimbalBase::Run(float delta_time_sec)
{



	UpdateAxis(EGimbalAxis::Roll, delta_time_sec);
	UpdateAxis(EGimbalAxis::Pitch, delta_time_sec);
	UpdateAxis(EGimbalAxis::Yaw, delta_time_sec);

	UpdateAttachedActors();
}

void AGimbalBase::FinalizeGimbal()
{
}


FSGimbalAxisInfo* AGimbalBase::GetAxis(EGimbalAxis axis)
{
	switch (axis)
	{
	case Roll:
		return &RollAxis;
	case Pitch:
		return &PitchAxis;
	case Yaw:
		return &YawAxis;
	default:
		break;
	}

	return nullptr;
}

float AGimbalBase::GetAxisAngleDeg(EGimbalAxis axis)
{
	return GetAxis(axis)->GetCurrentAngleDeg();
}



void AGimbalBase::SetCommand_(float roll_angle_deg, float pitch_angle_deg, float yaw_angle_deg)
{
	GetAxis(EGimbalAxis::Roll)->SetCommandAngleDeg(roll_angle_deg);
	GetAxis(EGimbalAxis::Pitch)->SetCommandAngleDeg(pitch_angle_deg);
	GetAxis(EGimbalAxis::Yaw)->SetCommandAngleDeg(yaw_angle_deg);
}


void AGimbalBase::UpdateAxis(EGimbalAxis axis, float delta_time_sec)
{
	FSGimbalAxisInfo* p_axis = GetAxis(axis);

	if (p_axis->Enabled) {
		double current_angle_deg = p_axis->GetCurrentAngleDeg();
		double error = p_axis->GetCommandAngleDeg() - current_angle_deg;
		double kp = 1.0;
		if (FMath::Abs(error) < 1) {
			kp = 0;
		}
		else if (error < 0) {
			kp = -1;
		}
	    current_angle_deg = current_angle_deg + kp * p_axis->AngleSpeedDegPerSec * delta_time_sec;
		current_angle_deg = FMath::Clamp(current_angle_deg, p_axis->MinLimitAngleDeg, p_axis->MaxLimitAngleDeg);
		p_axis->SetCurrentAngleDeg(current_angle_deg);
	}
}

void AGimbalBase::UpdateAttachedActors()
{
	
	FRotator rot(GetAxisAngleDeg(EGimbalAxis::Pitch), GetAxisAngleDeg(EGimbalAxis::Yaw), GetAxisAngleDeg(EGimbalAxis::Roll));
	SetActorRelativeRotation(rot);


	
}




void AGimbalBase::GimbalStateMachine(float delta_time_sec)
{
	EGimbalState current_state = GimbalState;
	EGimbalState  next_state = current_state;

	switch (current_state)
	{
	case GimbalInit:
		InitGimbal();
		next_state = EGimbalState::GimbalRunning;
		break;
	case GimbalRunning:
		Run(delta_time_sec);
		break;
	case GimbalFinish:
		FinalizeGimbal();
		break;
	default:
		break;
	}

	GimbalState = next_state;
}
// Called every frame
void AGimbalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GimbalStateMachine(DeltaTime);

}
