// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"

/**
 * 
 */
class CNav
{
public:
	CNav();
	~CNav();

	static FLOAT64 ComputeCourseOverGroundDeg(FVector dir_ned);
};
