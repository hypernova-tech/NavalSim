// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Gimbal/GimbalBase.h"
#include <Lib/Utils/CUtil.h>
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>
#include "CBoatBase.h"
#include <Lib/Math/CMath.h>
#include <Lib/Console/CCLICommandManager.h>
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
	switch (GimbalControlMode) {
	case EGimbalControlMode::PositionWithRateMode:
		UpdateAxisPositionWithRate(EGimbalAxis::Roll, delta_time_sec);
		UpdateAxisPositionWithRate(EGimbalAxis::Pitch, delta_time_sec);
		UpdateAxisPositionWithRate(EGimbalAxis::Yaw, delta_time_sec);
		break;
	case EGimbalControlMode::OnlyRate:
		UpdateAxisOnlyRate(EGimbalAxis::Roll, delta_time_sec);
		UpdateAxisOnlyRate(EGimbalAxis::Pitch, delta_time_sec);
		UpdateAxisOnlyRate(EGimbalAxis::Yaw, delta_time_sec);
		break;
	case EGimbalControlMode::PositionWithoutRate:
		UpdateAxisPositionWithoutRate(EGimbalAxis::Roll, delta_time_sec);
		UpdateAxisPositionWithoutRate(EGimbalAxis::Pitch, delta_time_sec);
		UpdateAxisPositionWithoutRate(EGimbalAxis::Yaw, delta_time_sec);
		break;
	}

	UpdateAttachedActors();
	
}

void AGimbalBase::FinalizeGimbal()
{
}



double AGimbalBase::GetFixedRate(EGimbalAxis axis)
{
	switch (axis)
	{
	case Roll:
		return GetAxis(EGimbalAxis::Roll)->FixedControlRateDegPerSec;
	case Pitch:
		return GetAxis(EGimbalAxis::Pitch)->FixedControlRateDegPerSec;
	case Yaw:
		return GetAxis(EGimbalAxis::Yaw)->FixedControlRateDegPerSec;
	default:
		break;
	}

	return 0;
}

double AGimbalBase::GetAxisAngleDeg(EGimbalAxis axis)
{
	return GetAxis(axis)->GetCurrentAngleDeg();
}


FVector AGimbalBase::GetAxisInitialAngleDeg()
{
	FVector ret;
	ret.X = GetAxis(EGimbalAxis::Roll)->InitialAngleDeg;
	ret.Y = GetAxis(EGimbalAxis::Pitch)->InitialAngleDeg;
	ret.Z = GetAxis(EGimbalAxis::Yaw)->InitialAngleDeg;

	return ret;
}

void AGimbalBase::SetAxisInitialAngleDeg(FVector val)
{

	GetAxis(EGimbalAxis::Roll)->InitialAngleDeg = val.X;
	GetAxis(EGimbalAxis::Pitch)->InitialAngleDeg = val.Y;
	GetAxis(EGimbalAxis::Yaw)->InitialAngleDeg = val.Z;

	
}

FVector AGimbalBase::GetRPYDeg()
{
	FVector ret;
	ret.X = GetAxis(EGimbalAxis::Roll)->GetCurrentAngleDeg();
	ret.Y = GetAxis(EGimbalAxis::Pitch)->GetCurrentAngleDeg();
	ret.Z = GetAxis(EGimbalAxis::Yaw)->GetCurrentAngleDeg();

	return ret;
}
void AGimbalBase::SetRPYDeg(FVector val)
{
	FVector ret;
	GetAxis(EGimbalAxis::Roll)->SetCurrentAngleDeg(val.X);
	GetAxis(EGimbalAxis::Pitch)->SetCurrentAngleDeg(val.Y);
	GetAxis(EGimbalAxis::Yaw)->SetCurrentAngleDeg(val.Z);

}


void AGimbalBase::SetGimbalControlMode(EGimbalControlMode mode)
{
	GimbalControlMode = mode;
}

FVector AGimbalBase::GetAxisMaxLimitAngleDeg()
{
	FVector ret;
	ret.X = GetAxis(EGimbalAxis::Roll)->MaxLimitAngleDeg;
	ret.Y = GetAxis(EGimbalAxis::Pitch)->MaxLimitAngleDeg;
	ret.Z = GetAxis(EGimbalAxis::Yaw)->MaxLimitAngleDeg;

	return ret;
}

void AGimbalBase::SetAxisMaxLimitAngleDeg(FVector val)
{

	GetAxis(EGimbalAxis::Roll)->MaxLimitAngleDeg = val.X;
	GetAxis(EGimbalAxis::Pitch)->MaxLimitAngleDeg = val.Y;
	GetAxis(EGimbalAxis::Yaw)->MaxLimitAngleDeg = val.Z;


}

FVector AGimbalBase::GetAxisMinLimitAngleDeg()
{
	FVector ret;
	ret.X = GetAxis(EGimbalAxis::Roll)->MinLimitAngleDeg;
	ret.Y = GetAxis(EGimbalAxis::Pitch)->MinLimitAngleDeg;
	ret.Z = GetAxis(EGimbalAxis::Yaw)->MinLimitAngleDeg;

	return ret;
}

void AGimbalBase::SetAxisMinLimitAngleDeg(FVector ret)
{
	
	GetAxis(EGimbalAxis::Roll)->MinLimitAngleDeg = ret.X;
	GetAxis(EGimbalAxis::Pitch)->MinLimitAngleDeg = ret.Y;
	GetAxis(EGimbalAxis::Yaw)->MinLimitAngleDeg = ret.Z;


}



FVector AGimbalBase::GetAxisIsFreeRotateEnabled()
{
	FVector val;

	val.X = GetAxis(EGimbalAxis::Roll)->IsFreeRotateEnabled;
	val.Y = GetAxis(EGimbalAxis::Pitch)->IsFreeRotateEnabled;
	val.Z = GetAxis(EGimbalAxis::Yaw)->IsFreeRotateEnabled;

	return val;


}

void AGimbalBase::Save(ISaveLoader* p_save_loader)
{
	Super::Save(p_save_loader);
	FString line;
	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalMode, GetGimbalControlMode());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalInitialAng, GetAxisInitialAngleDeg());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalAxisEnabled, GetAxisEnabled_());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalRate, GetAxisRateDegPerSec_());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalFixedRate, GetAxisFixedRateDegPerSec_());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalMaxLimitAng, GetAxisMaxLimitAngleDeg());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalMinLimitAng, GetAxisMinLimitAngleDeg());
	p_save_loader->AddLine(line);

	line = p_save_loader->CreateCommandWithName(CCLICommandManager::SetCommand, GetName());
	p_save_loader->AppendOption(line, CCLICommandManager::GimbalFreeRotateEnabled, GetAxisIsFreeRotateEnabled());
	p_save_loader->AddLine(line);
}

void AGimbalBase::SaveJSON(CJsonDataContainer& data)
{
	Super::SaveJSON(data);

	data.Add(CCLICommandManager::GimbalMode, GetGimbalControlMode());
	data.Add(CCLICommandManager::GimbalInitialAng, GetAxisInitialAngleDeg());
	data.Add(CCLICommandManager::GimbalAng, GetRPYDeg());
	data.Add(CCLICommandManager::GimbalAxisEnabled, GetAxisEnabled_());
	data.Add(CCLICommandManager::GimbalRate, GetAxisRateDegPerSec_());
	data.Add(CCLICommandManager::GimbalFixedRate, GetAxisFixedRateDegPerSec_());
	data.Add(CCLICommandManager::GimbalMaxLimitAng, GetAxisMaxLimitAngleDeg());
	data.Add(CCLICommandManager::GimbalMinLimitAng, GetAxisMinLimitAngleDeg());
	data.Add(CCLICommandManager::GimbalFreeRotateEnabled, GetAxisIsFreeRotateEnabled());
}

void AGimbalBase::SetAxisIsFreeRotateEnabled(FVector val)
{


	GetAxis(EGimbalAxis::Roll)->IsFreeRotateEnabled = val.X > 0.5f;
	GetAxis(EGimbalAxis::Pitch)->IsFreeRotateEnabled = val.Y > 0.5f;
	GetAxis(EGimbalAxis::Yaw)->IsFreeRotateEnabled = val.Z > 0.5f;

	


}

EGimbalControlMode AGimbalBase::GetGimbalControlMode()
{
	return GimbalControlMode;
}


void AGimbalBase::SetCommand_(FVector rpy_deg)
{
	GetAxis(EGimbalAxis::Roll)->SetCommandAngleDeg(rpy_deg.X);
	GetAxis(EGimbalAxis::Pitch)->SetCommandAngleDeg(rpy_deg.Y);
	GetAxis(EGimbalAxis::Yaw)->SetCommandAngleDeg(rpy_deg.Z);
}

FVector AGimbalBase::GetCommand_()
{
	FVector ret;

	ret.X = GetAxis(EGimbalAxis::Roll)->GetCommandAngleDeg();
	ret.Y = GetAxis(EGimbalAxis::Pitch)->GetCommandAngleDeg();
	ret.Z = GetAxis(EGimbalAxis::Yaw)->GetCommandAngleDeg();

	return ret;
}
void AGimbalBase::EnableAxis_(FVector rpy_en)
{
	GetAxis(EGimbalAxis::Roll)->Enabled = rpy_en.X > 0.5;
	GetAxis(EGimbalAxis::Pitch)->Enabled = rpy_en.Y > 0.5;
	GetAxis(EGimbalAxis::Yaw)->Enabled = rpy_en.Z > 0.5;
}
FVector AGimbalBase::GetAxisEnabled_()
{
	FVector ret;

	ret.X = GetAxis(EGimbalAxis::Roll)->Enabled;
	ret.Y = GetAxis(EGimbalAxis::Pitch)->Enabled;
	ret.Z = GetAxis(EGimbalAxis::Yaw)->Enabled;

	return ret;
}
void AGimbalBase::SetAxisRateDegPerSec_(FVector rpy_en)
{
	GetAxis(EGimbalAxis::Roll)->AngleSpeedDegPerSec = rpy_en.X ;
	GetAxis(EGimbalAxis::Pitch)->AngleSpeedDegPerSec = rpy_en.Y ;
	GetAxis(EGimbalAxis::Yaw)->AngleSpeedDegPerSec = rpy_en.Z ;
}

FVector AGimbalBase::GetAxisRateDegPerSec_()
{
	FVector ret;
	ret.X = GetAxis(EGimbalAxis::Roll)->AngleSpeedDegPerSec;
	ret.Y = GetAxis(EGimbalAxis::Pitch)->AngleSpeedDegPerSec;
	ret.Z = GetAxis(EGimbalAxis::Yaw)->AngleSpeedDegPerSec;

	return ret;
}


void AGimbalBase::SetAxisFixedRateDegPerSec_(FVector rpy_en)
{
	GetAxis(EGimbalAxis::Roll)->FixedControlRateDegPerSec = rpy_en.X;
	GetAxis(EGimbalAxis::Pitch)->FixedControlRateDegPerSec = rpy_en.Y;
	GetAxis(EGimbalAxis::Yaw)->FixedControlRateDegPerSec = rpy_en.Z;
}

FVector AGimbalBase::GetAxisFixedRateDegPerSec_()
{
	FVector ret;
	ret.X = GetAxis(EGimbalAxis::Roll)->FixedControlRateDegPerSec;
	ret.Y = GetAxis(EGimbalAxis::Pitch)->FixedControlRateDegPerSec;
	ret.Z = GetAxis(EGimbalAxis::Yaw)->FixedControlRateDegPerSec;

	return ret;
}

void AGimbalBase::UpdateAxisPositionWithRate(EGimbalAxis axis, float delta_time_sec)
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

void AGimbalBase::UpdateAxisPositionWithoutRate(EGimbalAxis axis, float delta_time_sec)
{
	FSGimbalAxisInfo* p_axis = GetAxis(axis);
	auto rate = GetFixedRate(axis);

	if (p_axis->Enabled) {
		double current_angle_deg = p_axis->GetCurrentAngleDeg();
		double cmd = p_axis->GetCommandAngleDeg();

		if (FMath::Abs(current_angle_deg - cmd) > 1e-6) {
			double error = cmd - current_angle_deg;
			double kp = 1.0;


			if (error < 0) {
				kp = -1;
			}


			double next_angle_deg = current_angle_deg + kp * rate * delta_time_sec;
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
void AGimbalBase::UpdateAxisOnlyRate(EGimbalAxis axis, float delta_time_sec)
{
	FSGimbalAxisInfo* p_axis = GetAxis(axis);

	if (p_axis->Enabled) {

		double current_angle_deg = p_axis->GetCurrentAngleDeg();
		double next_angle_deg = current_angle_deg + p_axis->AngleSpeedDegPerSec * delta_time_sec;

		if (p_axis->IsFreeRotateEnabled) {
			if (next_angle_deg > 360) {
				next_angle_deg = next_angle_deg - 360;
			}
			else if (next_angle_deg < 0) {
				next_angle_deg += 360;
			};
		}
		else {
			next_angle_deg = FMath::Clamp(next_angle_deg, p_axis->MinLimitAngleDeg, p_axis->MaxLimitAngleDeg);
		}
		
		p_axis->SetCurrentAngleDeg(next_angle_deg);
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
