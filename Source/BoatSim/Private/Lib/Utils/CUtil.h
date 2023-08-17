// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"
#include "Lib/Utils/CScanResult.h"
#include "Lib/Clutter/ClutterDefs.h"
/**
 * 
 */




class CUtil
{
public:
	CUtil();
	~CUtil();


public:
	static bool Trace(AActor* p_actor, bool is_world, float min_range_meter, float range_meter, float azimuth_start_deg, float azimuth_end_deg, float elevation_start_deg, float elevation_end_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg, 
						float measurement_error_mean, float measurement_error_std, const SClutterParams& clutter_params,
						bool show_radar_beam, TArray<AActor*> &ignore_list, bool create_scan_line,  SScanResult* pscan_result);

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
	static FString CharToFString(const char* p_char);
	static void FStringToAsciiChar(const FString& str, char* p_dest, INT32U dest_len);
	static void ByteCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void IPAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void MacAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static AActor* GetTopParent(AActor *p_actor);
	static AActor* SpawnObjectFromBlueprint(FString blueprint_path, UWorld* p_world, AActor* p_owner, FString name, FVector pos, FVector rot_rpy, FVector scale);
	static FLOAT64 GetTimeSeconds();
	static inline INT16U LittleToBig(INT16U val);
	static inline INT32U LittleToBig(INT32U val);
	static inline INT32U ReverseCopyBytes(INT8U* p_src, INT8U* p_dest, INT32U len);
	static inline FLOAT32 GetRandomRange(FLOAT32 min_inclusive, FLOAT32 max_inclusive);

};

