// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>

/**
 * 
 */
class IPlatform
{
public:
	IPlatform();
	~IPlatform();


	virtual void SetMaxSpeedMeterPerSec(FLOAT32 speed) = 0;
	virtual FLOAT32 GetMaxSpeedMeterPerSec() = 0;
};
