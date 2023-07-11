// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSOAObserverArgs.h"

/**
 * 
 */
class ISOAObserver
{
public:
	ISOAObserver();
	virtual ~ISOAObserver();

	virtual void Update(UCSOAObserverArgs*p_args);
};
