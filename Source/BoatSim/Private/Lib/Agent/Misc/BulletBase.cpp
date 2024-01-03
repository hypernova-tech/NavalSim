// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Agent/Misc/BulletBase.h"
#include <Lib/Utils/CUtil.h>

void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
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


double ABulletBase::GetSpeed()
{
	return CUtil::GetRandomRange(MinInitialSpeedMetersPerSec_, MaxInitialSpeedMetersPerSec_);
}
