// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CommonSOAObservers
{
public:
	CommonSOAObservers();
	~CommonSOAObservers();


	static const int PlatformKinematicObserverId = 1;

private:
	static CommonSOAObservers* pInstance;
};
