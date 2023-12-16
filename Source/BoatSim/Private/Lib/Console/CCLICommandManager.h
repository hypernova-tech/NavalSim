// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */


enum  EOptionType
{
	TypeBool,
	TypeInt8,
	TypeInt8U,
	TypeInt32,
	TypeInt32U,
	TypeFloat,
	TypeDouble,
	TypeString,
	TypeDoublFVector2D,
	TypeDoublFVector,
};

struct SCommandOptionInfo
{

	FString Option;
	FString Description;
	FString ParameterName;
	EOptionType OptionType;
	
};

struct SParsedOption
{
	BOOLEAN Ret;
	BOOLEAN BoolVal;
	INT32S IntVal;
	INT64S Int64Val;
	FLOAT64 DoubleVal;


};


class CCLICommandManager
{

private:
	TMap<FString, FString>* pOptions;

public:

	static  FString WorkspaceCommand;
	static  FString SaveFile;
	static  FString LoadFile;

	static  FString ProcessCommand;
	static  FString ProcessKill;

	static  FString CreateCommand;
	static  FString Name;
	static  FString Bp;
	static  FString Model;

	static FString DestroyCommand ;

	static FString SetCommand;
	static FString Active;
	static FString Enabled;
	static FString Instance;
	static FString Position;
	static FString RelPosition;
	static FString Rotation;
	static FString RelRotation;
	static FString Scale;
	static FString Parent;
	static FString Controller;
	static FString SensorSlotIndex;
	static FString Beam;
	static FString RangeMin;
	static FString RangeMax;
	static FString Selected;
	static FString Focused;
	static FString UIVisible;
	static FString IPAddr1;
	static FString LocalPort1;
	static FString RemotePort1;
	static FString IPAddr2;
	static FString LocalPort2;
	static FString RemotePort2;
	static FString CamSpeed;
	static FString PlatformSpeed;
	static FString PlatformTurnRate;
	static FString CamView;

	static FString VericalFov;
	static FString HorizontalFov;
	static FString HorizontalScanStepAngleDeg;
	static FString VerticalScanStepAngleDeg;
	static FString MeasurementErrorMean;
	static FString MeasurementErrorStd;
	static FString EnableSurfaceDetect;
	static FString EnableSubsurfaceDetect;
	static FString EnableFoamDetect;
	static FString SeaSurfaceDetectionProb;
	static FString MaxSurfacePenetration;

	static FString RadarScanLevel;
	static FString RadarScannerRPM;
	static FString RadarGainType;
	static FString RadarGainLevel;
	static FString RadarSeaClutterType;
	static FString RadarSeaClutterLevel;
	static FString RadarSeaClutterAutoOffset;
	static FString RadarRainClutterLevel;
	static FString RadarMaxGuardZoneCount;
	static FString RadarMaxSectorBlankingZoneCount;
	
	static FString CamWidthPx;
	static FString CamHeightPx;

	static FString RainPercent;
	static FString CloudPercent;
	static FString SnowPercent;
	static FString DustPercent;
	static FString FogPercent;
	static FString ThunderPercent;
	static FString EnvSoundFxEnabled;
	static FString WindSpeed;
	static FString WindDirection;
	static FString TimeOfDayHour;
	static FString EnvTimeFlowScale;

	static FString TerrHMap;
	static FString TerrDMap;
	static FString TerrBaseTexture;
	static FString TerrHMapMinLvlMt;
	static FString TerrHMapMaxLvlMt;
	static FString TerrDMapMinLvlMt;
	static FString TerrDMapMaxLvlMt;
	static FString TerrWidthDeg;
	static FString TerrLengthDeg;
	static FString TerrLowerLeftCornerXYZ;
	static FString TerrLowerLeftCornerLLH;

	static FString Wp;
	static FString WpPos;
	static FString Closed;
	static FString Bake;
	static FString Speed;
	static FString Attach;
	static FString Detach;
	static FString Straight;
	static FString LineColor;
	static FString SegmentCount;
	static FString TurnRate;

	static FString GetCommand;
	static FString All;
	static FString PrintCommand;
	static FString Actors;
	static FString ActorBases;
	static FString Sensors;
	static FString SensorType;
	static FString SensorTypes;
	static FString Paths;

	static FString SimCommand;
	static FString Start;
	static FString Pause;
	static FString Resume;



	SCommandOptionInfo* FindCommandOptionInfo(FString option);

	TMap<FString, TArray<SCommandOptionInfo>> CommandInfo;



public:
	CCLICommandManager();

	void SetCommandOptions(TMap<FString, FString>* p_opt);
	TMap<FString, TArray<SCommandOptionInfo>>* GetCommandInfo();

	void PrepareWorkspaceCommandInfo();
	void PrepareCreateCommandInfo();
	void PrepareDestroyCommandInfo();
	void PrepareProcessCommandInfo();
	void PrepareSetCommandInfo();
	void PrepareGetCommandInfo();
	void PreparePrintCommandInfo();
	void PrepareSimCommandInfo();



	int GetProcessKillInstanceCount();


	bool HasA(FString str);

	bool GetValue(FString& name, BOOLEAN& val);
	bool GetValue(FString& name, FVector& val);
	bool GetValue(FString& name, FColor& val);
	bool GetValue(FString& name, FVector2D& val);
	bool GetValue(FString& name, FLOAT64& val);
	bool GetValue(FString& name, INT32S& val);
	bool GetValue(FString& name, FString& val);
	
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
	bool GetEnabled(bool& val);

	bool GetInstance(int& val);
	bool GetSensorType(FString& val);
	bool HasSensorTypes();
	bool HasPosition();
	bool HasScale();
	bool HasEnabled();
	bool HasRelposition();
	bool HasRotation();
	bool HasRelrotation();
	bool HasParent();
	bool HasController();
	bool HasScanStep();




	bool GetStart(bool& is_start);
	bool GetPause(bool& is_pause);
	bool GetResume(bool& is_resume);

	bool GetParent(FString& val);
	bool GetContoroller(FString& val);
};

