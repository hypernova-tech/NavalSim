// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Platform/PlatformBase.h"
#include <Lib/SystemManager/SystemManagerBase.h>

void APlatformBase::BeginPlay()
{
	Super::BeginPlay();
	if (AutoPosseses) {
		ASystemManagerBase::GetInstance()->RegisterActor(this);
	}
}
