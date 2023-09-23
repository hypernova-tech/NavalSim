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
		ASystemManagerBase::GetInstance()->RegisterActor("world", this);
	}
	else {

		ASystemManagerBase::GetInstance()->RegisterActor(p_owner->GetActorLabel(), this); //todo fixme

	}
	
}

// Called every frame
void AActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorBase::SetEnabled(bool val)
{
	Enabled = val;
}
bool AActorBase::GetEnabled()
{
	return Enabled;
}
