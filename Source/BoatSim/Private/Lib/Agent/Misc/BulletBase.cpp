// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Agent/Misc/BulletBase.h"
#include <Lib/Utils/CUtil.h>

void ABulletBase::BeginPlay()
{
	UStaticMeshComponent *p_comp = CUtil::FindChildComponent<UStaticMeshComponent>(this);
	if (p_comp) {
		pBody = p_comp;
	}
}

void ABulletBase::OnStep(float DeltaTime)
{
	Super::OnStep(DeltaTime);
}

void ABulletBase::SetLinearVelocity(FVector val)
{
	if (pBody) {
		pBody->SetPhysicsLinearVelocity(val);
	}
}

FVector ABulletBase::GetLinearVelocity()
{
	if (pBody) {
		FVector ret = pBody->GetPhysicsLinearVelocity();
		return ret;
	}

	return FVector::ZeroVector;
}

void ABulletBase::SetInitialSpeedMetersPerSec_(double val)
{
	InitialSpeedMetersPerSec_ = val;
}

double ABulletBase::GetInitialSpeedMetersPerSec_()
{
	return InitialSpeedMetersPerSec_;
}
