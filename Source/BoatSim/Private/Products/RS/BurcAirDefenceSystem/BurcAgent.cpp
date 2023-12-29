// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/RS/BurcAirDefenceSystem/BurcAgent.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

void ABurcAgent::BeginPlay()
{
}

void ABurcAgent::Fire()
{
}

void ABurcAgent::AimGun(FVector pos)
{
}

bool ABurcAgent::AssignTarget(FString target_name, double duration)
{
	ASystemManagerBase::GetInstance()->SendConsoleResponse("AssignTarget called target_name: "+ target_name+" duration: "+CUtil::FloatToString(duration));
	return false;
}
