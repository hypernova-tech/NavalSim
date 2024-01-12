// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Gimbal/GimbalBase.h"
#include <Lib/Utils/CUtil.h>
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>
#include "CBoatBase.h"
#include <Lib/Math/CMath.h>
// Sets default values
AGimbalBase::AGimbalBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGimbalBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

// Called when the game starts or when spawned
void AGimbalBase::BeginPlay()
{
	
	Super::BeginPlay();

	auto owner = GetOwner();
	auto parent = CUtil::GetParentActor(this);
	auto attach = GetAttachParentActor();
	UChildActorComponent* MyChildActorComponent = GetComponentByClass< UChildActorComponent>();
}

void AGimbalBase::OnPreStep(float DeltaTime)
{
	Super::OnPreStep(DeltaTime);
		TArray<USceneComponent*> AttachedComponents;
	this->GetRootComponent()->GetChildrenComponents(true, AttachedComponents);

	for (USceneComponent* ChildComponent : AttachedComponents)
	{
		if (ChildComponent && ChildComponent->GetAttachParent() == this->GetRootComponent())
		{
			
			AttachComp_(ChildComponent);
			// Apply rotation
			
		}
	}
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

double AGimbalBase::GetAxisAngleDeg(EGimbalAxis axis)
{
	return GetAxis(axis)->GetCurrentAngleDeg();
}
FVector AGimbalBase::GetRPYDeg()
{
	FVector ret;
	ret.X = GetAxis(EGimbalAxis::Roll)->GetCurrentAngleDeg();
	ret.Y = GetAxis(EGimbalAxis::Pitch)->GetCurrentAngleDeg();
	ret.Z = GetAxis(EGimbalAxis::Yaw)->GetCurrentAngleDeg();

	return ret;
}


void AGimbalBase::SetCommand_(FVector rpy_deg)
{
	GetAxis(EGimbalAxis::Roll)->SetCommandAngleDeg(rpy_deg.X);
	GetAxis(EGimbalAxis::Pitch)->SetCommandAngleDeg(rpy_deg.Y);
	GetAxis(EGimbalAxis::Yaw)->SetCommandAngleDeg(rpy_deg.Z);
}
void AGimbalBase::EnableAxis_(FVector rpy_en)
{
	GetAxis(EGimbalAxis::Roll)->Enabled = rpy_en.X > 0.5;
	GetAxis(EGimbalAxis::Pitch)->Enabled = rpy_en.Y > 0.5;
	GetAxis(EGimbalAxis::Yaw)->Enabled = rpy_en.Z > 0.5;
}
void AGimbalBase::SetAxisRateDegPerSec_(FVector rpy_en)
{
	GetAxis(EGimbalAxis::Roll)->AngleSpeedDegPerSec = rpy_en.X ;
	GetAxis(EGimbalAxis::Pitch)->AngleSpeedDegPerSec = rpy_en.Y ;
	GetAxis(EGimbalAxis::Yaw)->AngleSpeedDegPerSec = rpy_en.Z ;
}



void AGimbalBase::UpdateAxis(EGimbalAxis axis, float delta_time_sec)
{
	FSGimbalAxisInfo* p_axis = GetAxis(axis);

	if (p_axis->Enabled) {
		double current_angle_deg = p_axis->GetCurrentAngleDeg();
		double cmd = p_axis->GetCommandAngleDeg();

		if (FMath::Abs(current_angle_deg - cmd) > 1e-6) {
			double error = cmd - current_angle_deg;
			double kp = 1.0;


			if (error < 0) {
				kp = -1;
			}


			double next_angle_deg = current_angle_deg + kp * p_axis->AngleSpeedDegPerSec * delta_time_sec;
			next_angle_deg = FMath::Clamp(next_angle_deg, p_axis->MinLimitAngleDeg, p_axis->MaxLimitAngleDeg);

			if (((next_angle_deg - cmd) * (current_angle_deg - cmd)) < 0) {
				next_angle_deg = cmd;
			}

			p_axis->SetCurrentAngleDeg(next_angle_deg);
		}
		else {
			p_axis->SetCurrentAngleDeg(cmd);
		}
	
	}
}


void AGimbalBase::AttachComp_(USceneComponent* p_comp)
{
	FCompEntry entry;
	entry.pComp = p_comp;
	entry.InitialTransform = p_comp->GetRelativeTransform();

	AttachedComps.Add(entry);
}

void AGimbalBase::RemoveAttachedComp_(USceneComponent* p_comp)
{
	//AttachedComps.Remove(p_comp);
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
void AGimbalBase::OnStep(float DeltaTime)
{
	Super::OnStep(DeltaTime);
	GimbalStateMachine(DeltaTime);


}
void AGimbalBase::UpdateAttachedActors()
{
	FVector rpy = GetRPYDeg();
	FQuat QuatRoll(FVector::ForwardVector, rpy.X * DEGTORAD);
	FQuat QuatPitch(FVector::RightVector, rpy.Y * DEGTORAD);
	FQuat QuatYaw(FVector::UpVector, rpy.Z * DEGTORAD);
	FQuat additional_quat =  QuatYaw * QuatPitch * QuatRoll;

	for (FCompEntry& child_entry : AttachedComps)
	{
		FQuat final_quat = /*child_entry.InitialTransform.GetRotation() * */additional_quat;
		CMath::RotateRelative(this, child_entry.pComp, child_entry.InitialTransform, final_quat.Euler());
		//child_entry.pComp->SetRelativeRotation(final_quat.Rotator());
		
	}


}
