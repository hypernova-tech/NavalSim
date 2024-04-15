// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CExecRunnerBase
{
public:
	CExecRunnerBase();
	~CExecRunnerBase();
	bool ResumeProcess(uint32 processID);
	bool RunExecutable(const FString& ExecutableRelativePath, const FString& Arguments);
	void ExecuteWindowsCommand(const FString& Command);
};
