// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>

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
	bool Ret;
	bool BoolVal;
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

	static  FString AnnotationCommand;
	static  FString Query;
	static  FString SaveEnabled;

	static  FString CreateCommand;
	static  FString Name;
	static  FString Bp;
	static  FString Model;
	static  FString Actor;

	static FString DestroyCommand ;

	static FString SetCommand;
	static FString Active;
	static FString Enabled;
	static FString ActorId;
	static FString Suppressed;
	static FString StandaloneModeEnabled;
	static FString Instance;
	static FString Position;
	static FString RelPosition;
	static FString Rotation;
	static FString RelRotation;
	static FString Scale;
	static FString Parent;
	static FString SensorFrequency;
	static FString FullPath;
	static FString SharedMemName;
	static FString GStreamerIP;
	static FString GStreamerPort;
	static FString MotionLogEnabled;
	static FString Tick;
	static FString Controller;
	static FString SensorSlotIndex;
	static FString Beam;
	static FString PointCloud3D;
	static FString RangeMin;
	static FString RangeMax;
	static FString Selected;
	static FString Focused;
	static FString UIVisible;
	static FString IsBlockingObject;
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
	static FString CamFollow;
	static FString CamFollowTranslation;
	static FString CamFollowEuler;

	static FString AISClassType;
	static FString AISMessagePublishPeriodSecStationary;
	static FString AISMessagePublishPeriodSecMoving;
	static FString AISShouldPublishAton;
	static FString AISVendorId;
	static FString AISCallSign;
	static FString AISReferencePointFromStarboard;
	static FString ReferencePointPositionAftOfBow;
	static FString AISMMSI;
	static FString AISName;
	static FString AISDateOfArrival;
	static FString AISTimeOfArrival;
	static FString AISUserId;
	static FString AISShipOrCargoType;

	static FString VericalFov;
	static FString HorizontalFov;
	static FString HorizontalScanStepAngleDeg;
	static FString VerticalScanStepAngleDeg;
	static FString IsNormalDistribution;
	static FString MeasurementErrorMean;
	static FString MeasurementErrorStd;
	static FString EnableSurfaceDetect;
	static FString EnableSubsurfaceDetect;
	static FString EnableFoamDetect;
	static FString SeaSurfaceDetectionProb;
	static FString MaxSurfacePenetration;

	static FString GimbalMode;
	static FString GimbalCmd;
	static FString GimbalAng;
	static FString GimbalInitialAng;
	static FString GimbalAxisEnabled;
	static FString GimbalRate;
	static FString GimbalFixedRate;
	static FString GimbalMaxLimitAng;
	static FString GimbalMinLimitAng;
	static FString GimbalFreeRotateEnabled;

	

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
	static FString ThermalMode;
	static FString TempratureKelvin;
	static FString IsHeatSource;
	static FString AnnotationId;
	static FString AnnotateChildren;
	static FString TerrHMap;
	static FString TerrDMap;
	static FString TerrBaseTexture;
	static FString TerrHMapMinLvlMt;
	static FString TerrHMapMaxLvlMt;
	static FString TerrDMapMinLvlMt;
	static FString TerrDMapMaxLvlMt;
	static FString TerrTopLeftLLH;
	static FString TerrBottomRightLLH;

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
	static FString MapOriginLat;
	static FString MapOriginLon;
	static FString MapOriginHeight;


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

	static FString ExecCommand;
	static FString Property;
	static FString Properties;
	static FString PropertyValue;
	static FString Func;
	static FString FuncCategory;
	static FString FuncParam1;
	static FString FuncParam2;
	static FString FuncParam3;
	static FString FuncParam4;
	static FString FuncParam5;
	static FString FuncParam6;
	static FString FuncParam7;
	static FString FuncParam8;
	static FString FuncParam9;
	static FString FuncParam10;

	static FString FuncValue1;
	static FString FuncValue2;
	static FString FuncValue3;
	static FString FuncValue4;
	static FString FuncValue5;
	static FString FuncValue6;
	static FString FuncValue7;
	static FString FuncValue8;
	static FString FuncValue9;
	static FString FuncValue10;
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

	bool GetValue(FString& name, bool& val);
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

