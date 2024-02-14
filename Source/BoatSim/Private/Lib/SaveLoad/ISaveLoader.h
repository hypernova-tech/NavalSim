// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>

/**
 * 
 */
class ISaveLoader
{
public:
	ISaveLoader();
	~ISaveLoader();

	virtual FString CreateCommand(FString cmd) = 0;
	virtual FString CreateCommandWithName(FString cmd, FString name) = 0;
	virtual void NewLine() = 0;
	virtual void AddLine(FString line) = 0;
	virtual void AppendOption(FString& line, FString option, FString value)=0;
	virtual void AppendOption(FString& line, FString option, bool value) = 0;
	virtual void AppendOption(FString& line, FString option, INT32U value) = 0;
	virtual void AppendOption(FString& line, FString option, INT32S value) = 0;
	virtual void AppendOption(FString& line, FString option, INT64U value) = 0;
	virtual void AppendOption(FString& line, FString option, INT64S value) = 0;
	virtual void AppendOption(FString& line, FString option, INT16U value) = 0;
	virtual void AppendOption(FString& line, FString option, FLOAT32 value) = 0;
	virtual void AppendOption(FString& line, FString option, FLOAT64 value) = 0;
	virtual void AppendOption(FString& line, FString option, FVector2D value) = 0;
	virtual void AppendOption(FString& line, FString option, FVector value) = 0;
	virtual void AppendOption(FString& line, FString option, FColor value) = 0;

};
