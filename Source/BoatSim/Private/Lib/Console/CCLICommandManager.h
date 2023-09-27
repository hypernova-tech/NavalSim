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

	FString DestroyCommand = "destroy";

	FString SetCommand = "set";
	FString Active = "active";
	FString Instance = "instance";
	FString Position = "position";
	FString RelPosition = "relposition";
	FString Rotation = "rotation";
	FString RelRotation = "relrotation";
	FString Scale = "scale";
	FString Parent = "parent";
	FString Controller = "controller";


	FString GetCommand = "get";

	FString PrintCommand	= "print";
	FString Actors			= "actors";
	FString Sensors			= "sensors";
	FString SensorType		= "sensortype";
	FString SensorTypes		= "sensortypes";

	FString SimCommand = "sim";
	FString Start = "start";
	FString Pause = "pause";
	FString Resume = "resume";


	TMap<FString, TArray<SCommandOptionInfo>> CommandInfo;



public:
	CCLICommandManager();

	void SetCommandOptions(TMap<FString, FString>* p_opt);
	TMap<FString, TArray<SCommandOptionInfo>>* GetCommandInfo();


	void PrepareCreateCommandInfo();
	void PrepareDestroyCommandInfo();
	void PrepareProcessCommandInfo();
	void PrepareSetCommandInfo();
	void PrepareGetCommandInfo();
	void PreparePrintCommandInfo();
	void PrepareSimCommandInfo();



	int GetProcessKillInstanceCount();

	bool HasName();
	bool HasBP();
	bool HasActors();
	bool HasSensors();

	FString GetName();
	TArray<FString> GetRelativeName();
	FString GetBP();

	FString GetModel();

	bool GetPosition(FVector& vec);

	bool GetRelPosition(FVector& vec);

	bool GetRotation(FVector& vec);

	bool GetRelRotation(FVector& vec);

	bool GetScale(FVector& vec);

	bool GetActive(bool& val);

	bool GetInstance(int& val);
	bool GetSensorType(FString& val);
	bool HasSensorTypes();
	bool HasPosition();
	bool HasScale();
	bool HasRelposition();
	bool HasRotation();
	bool HasRelrotation();
	bool HasParent();
	bool HasController();


	bool GetStart(bool& is_start);
	bool GetPause(bool& is_pause);
	bool GetResume(bool& is_resume);

	bool GetParent(FString& val);
	bool GetContoroller(FString& val);
};

