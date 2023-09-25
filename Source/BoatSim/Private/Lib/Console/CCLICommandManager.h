// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct SCommandOptionInfo
{

	FString Option;
	FString Description;

};




class CCLICommandManager
{

private:
	TMap<FString, FString>* pOptions;

	FString ProcessCommand = "process";
	FString ProcessKill = "kill";

	FString CreateCommand = "create";
	FString Name = "name";
	FString Bp = "bp";
	FString Model = "model";

	FString SetCommand = "set";
	FString Position = "position";
	FString RelPosition = "relposition";
	FString Rotation = "rotation";
	FString RelRotation = "relrotation";
	FString Scale = "scale";

	TMap<FString, TArray<SCommandOptionInfo>> CommandInfo;



public:
	CCLICommandManager();

	void SetCommandOptions(TMap<FString, FString>* p_opt);
	TMap<FString, TArray<SCommandOptionInfo>>* GetCommandInfo();


	void PrepareCreateCommandInfo();
	void PrepareProcessCommandInfo();
	void PrepareSetCommandInfo();

	int GetProcessKillInstanceCount();

	bool HasName();

	FString GetName();
	TArray<FString> GetRelativeName();
	FString GetBP();

	FString GetModel();

	bool GetPosition(FVector& vec);

	bool GetRelPosition(FVector& vec);

	bool GetRotation(FVector& vec);

	bool GetRelRotation(FVector& vec);

	bool GetScale(FVector& vec);
};

