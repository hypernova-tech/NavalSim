// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ActorBase/ActorBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Utils/CUtil.h>

// Sets default values
AActorBase::AActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void AActorBase::BeginPlay()
{
	Super::BeginPlay();
	AActor *p_owner = CUtil::GetTopParent(this);

	if (p_owner == nullptr) {
		ASystemManagerBase::GetInstance()->RegisterActor(this);
	}
	else {

		ASystemManagerBase::GetInstance()->RegisterActor(this); //todo fixme

	}
	
}

void AActorBase::OnStep(float DeltaTime)
{

}

bool AActorBase::CheckAffinity()
{
	if (AffinityInstanceId == -1) {
		return true;
	}
	else {
		return ASystemManagerBase::GetInstance()->GetInstanceNo() == AffinityInstanceId;
	}

	return false;
}

void AActorBase::OnActorDisabled()
{
}

void AActorBase::OnActorEnabled()
{
}

// Called every frame
void AActorBase::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	if (!IsExternalUpdate) {
		if (CheckAffinity()) {
			OnStep(DeltaTime);
		}
	}
	
}

void AActorBase::SetEnabled(bool val)
{
	Enabled = val;
	if (val) {
		OnActorEnabled();
	}else {
		OnActorDisabled();
	}
}
bool AActorBase::GetEnabled()
{
	return Enabled;
}

void AActorBase::ExternalUpdate(float DeltaTime)
{
	if (CheckAffinity()) {
		OnStep(DeltaTime);
	}
	
}

void AActorBase::SetAffinityInstanceId(int val)
{
	AffinityInstanceId = val;
}

int AActorBase::GetAffinityInstanceId()
{
	return AffinityInstanceId;
}
