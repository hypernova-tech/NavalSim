// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CTime
{
public:
	CTime();
	~CTime();

	/// <summary>
	/// unaffacted from time scaling and time diluation
	/// </summary>
	/// <returns></returns>
	static double GetTimeSecond();
};
