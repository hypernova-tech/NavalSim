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

	FString Name = "name";
	FString Bp = "bp";
	FString Model = "model";

	FString Position = "position";
	FString RelPosition = "relposition";
	FString Rotation = "rotation";
	FString RelRotation = "relrotation";
	FString Scale = "scale";

	TMap<FString, TArray<SCommandOptionInfo>> CommandInfo;



public:
	CCLICommandManager();

	void SetCommandInfo(TMap<FString, FString>* p_opt);

	void PrepareCreateCommandInfo();

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

