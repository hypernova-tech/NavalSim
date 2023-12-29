// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/RS/BurcAirDefenceSystem/BurcAgentProcessor.h"
#include <Lib/SystemManager/SystemManagerBase.h>

void UBurcAgentProcessor::BeginPlay()
{
	Super::BeginPlay();
	auto def = pOwner->FindDef("gun");
	if (def.pObj) {
		pGun = (UStaticMeshComponent*)def.pObj;
	}
}

void UBurcAgentProcessor::Fire()
{
	FString bullet_name = ASystemManagerBase::GetInstance()->GenerateUniqueName("bullet");
	ASystemManagerBase::GetInstance()->CreateActor(BulletAgentName, bullet_name, FVector::ZeroVector, FVector::ZeroVector, FVector::OneVector);
}

void UBurcAgentProcessor::AimGun(FVector pos)
{
}

bool UBurcAgentProcessor::AssignTarget(FString target_name, double duration)
{
	return false;
}
