// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"
#include "Lib/Utils/CScanResult.h"

/**
 * 
 */

#define RADTODEG							57.295779513082323 
#define DEGTORAD							0.017453292519943
#define WORLDTOUNREALUNIT				    100
#define UNREALTOWORLDLUNIT				    0.01

#define WORLD_TO_UNREAL(val) (val * WORLDTOUNREALUNIT)
#define UNREAL_TO_WORLD(val) (val * UNREALTOWORLDLUNIT)

class CUtil
{
public:
	CUtil();
	~CUtil();


public:
	static bool Trace(AActor* p_actor, bool is_world, float min_range_meter, float range_meter, float azimuth_start_deg, float azimuth_end_deg, float elevation_start_deg, float elevation_end_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg, bool show_radar_beam, TArray<AActor*> &ignore_list, SScanResult* pscan_result);
	static bool ParallelTrace(AActor* p_actor, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg);

	static float ConvertToFloat(const char* p_str);
	static FString ConvertToFString(const char* p_str);
	static bool ParseVector2D(const FString& Text, FVector2D& ret);
	static bool ParseVector3D(const FString& Text, FVector& ret);
	static bool ParsePose(const FString& Text, FVector& translation, FVector& rpy);
	static void DebugLog(FString str);
	template <typename T>
	static T* FindComponent(AActor* p_parent);
	template <typename T>
	static T* FindChildComponent(AActor* p_parent);

	static void LookAt(AActor* p_actor, FVector& look_dir);
	static FRotator GetActorRelativeRotation(AActor* p_actor);
	static int StringToInt(FString& str);
	static float StringToFloat(FString& str);
	static double StringToFloat64(FString& str);
	static void ByteCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void IPAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void MacAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static AActor* GetTopParent(AActor *p_actor);
	static AActor* SpawnObjectFromBlueprint(FString blueprint_path, UWorld* p_world, AActor* p_owner, FString name, FVector pos, FVector rot_rpy, FVector scale);
};

