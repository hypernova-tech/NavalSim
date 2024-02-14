// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>

/**
 * 
 */
class CJsonDataContainer
{
public:
	CJsonDataContainer();
	~CJsonDataContainer();


	TMap<FString, FString> Data;


	void Add(FString name, INT32S ret);
	void Add(FString name, INT32U ret);
	void Add(FString name, FLOAT64 ret);
	void Add(FString name, bool ret);
	void Add(FString name, FVector2D ret);
	void Add(FString name, FVector ret);
	void Add(FString name, FString ret);
	void Add(FString name, FColor ret);
	void Add(FString name, FVector4 ret);



};
