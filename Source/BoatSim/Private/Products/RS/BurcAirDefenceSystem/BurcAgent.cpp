// Fill out your copyright notice in the Description page of Project Settings.


#include "Products/RS/BurcAirDefenceSystem/BurcAgent.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>

void ABurcAgent::BeginPlay()
{
}

void ABurcAgent::Fire_()
{
}

void ABurcAgent::AimGun_(FVector pos)
{
}

bool ABurcAgent::AssignTarget_(FString target_name, double duration)
{
	TargetNameName_ = target_name;
	ASystemManagerBase::GetInstance()->SendConsoleResponse("AssignTarget called target_name: "+ target_name+" duration: "+CUtil::FloatToString(duration));
	return false;
}
