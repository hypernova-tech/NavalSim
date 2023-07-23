// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Gimbal/GimbalBase.h"
#include <Lib/Utils/CUtil.h>

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
	
}



void AGimbalBase::InitGimbal()
{
}

void AGimbalBase::Run(float delta_time_sec)
{
	SetCommand(0, 45, 0);


	UpdateAxis(EGimbalAxis::Roll, delta_time_sec);
	UpdateAxis(EGimbalAxis::Pitch, delta_time_sec);
	UpdateAxis(EGimbalAxis::Yaw, delta_time_sec);

	//UpdateAttachedActors();
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



void AGimbalBase::SetCommand(float roll_angle_deg, float pitch_angle_deg, float yaw_angle_deg)
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
		if (error == 0.0) {
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

void AGimbalBase::UpdateAttachedActors(FVector rotation)
{


	UChildActorComponent* p_comp = (UChildActorComponent*)GetRootComponent();
	//p_comp->SetRelativeRotation(FRotator(90*FMath::Sin(FApp::GetGameTime()), 90 * FMath::Sin(FApp::GetGameTime()), 90 * FMath::Sin(FApp::GetGameTime())));
	p_comp->AddRelativeRotation(FRotator(0.90 * FMath::Sin(FApp::GetGameTime()), 0.90 * FMath::Sin(FApp::GetGameTime()), 0.90 * FMath::Sin(FApp::GetGameTime())));

	return;
	FRotator rot(GetAxisAngleDeg(EGimbalAxis::Pitch), GetAxisAngleDeg(EGimbalAxis::Yaw), GetAxisAngleDeg(EGimbalAxis::Roll));
	FRotator rot2(90, -45, 52);
	FRotator curr = CUtil::GetActorRelativeRotation(this);
	GetRootComponent()->SetRelativeRotation(rot2);
	//SetActorRelativeRotation(rot2.Quaternion());
	FRotator next = CUtil::GetActorRelativeRotation(this);
	FRotator root2 = GetRootComponent()->GetRelativeRotation();


	
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
