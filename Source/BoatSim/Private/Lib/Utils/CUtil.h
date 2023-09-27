// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"
#include "Lib/Utils/CScanResult.h"
#include "Lib/Clutter/ClutterDefs.h"
/**
 * 
 */


struct STraceArgs
{
	AActor* p_actor;
	FVector scan_center;
	FVector scan_rpy_world_deg;
	bool is_world;
	float min_range_meter;
	float range_meter;
	float azimuth_start_deg;
	float azimuth_end_deg;

	float elevation_start_deg;
	float elevation_end_deg;
	float azimuth_angle_step_deg;
	float elevation_angle_step_deg;
	float measurement_error_mean;
	float measurement_error_std;
    SClutterParams clutter_params;

	bool show_radar_beam;
	TArray<AActor*> *p_ignore_list;
	TArray<AActor*> additional_ignore_list;
	bool create_scan_line;
	bool use_render_target = false;

};

class CUtil
{
public:
	CUtil();
	~CUtil();


public:
	static bool Trace(AActor* p_actor, bool is_world, float min_range_meter, float range_meter, float azimuth_start_deg, float azimuth_end_deg, float elevation_start_deg, float elevation_end_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg, 
						float measurement_error_mean, float measurement_error_std, const SClutterParams& clutter_params,
						bool show_radar_beam, TArray<AActor*> &ignore_list, bool create_scan_line,  SScanResult* pscan_result);
	static bool Trace(const STraceArgs &args, SScanResult* pscan_result);


	static bool ParseVector2D(const FString& Text, FVector2D& ret);
	static bool ParseVector3D(const FString& Text, FVector& ret);
	static bool ParsePose(const FString& Text, FVector& translation, FVector& rpy);
	static void DebugLog(FString str);
	static void DebugLogScreen(FString str, FLOAT32 duration_sec = 5, FColor col = FColor::Red);
	template <typename T>
	static T* FindComponent(AActor* p_parent);
	template <typename T>
	static T* FindChildComponent(AActor* p_parent);

	static void LookAt(AActor* p_actor, FVector& look_dir);
	static FRotator GetActorRelativeRotation(AActor* p_actor);
	/// <summary>
	/// String conversions
	/// </summary>
	/// <param name="p_str"></param>
	/// <returns></returns>
	static float ConvertToFloat(const char* p_str);
	static FString ConvertToFString(const char* p_str);
	static int StringToInt(FString& str);
	static FString IntToString(INT32S val);
	static float StringToFloat(FString& str);
	static double StringToFloat64(FString& str);
	static FString CharToFString(const char* p_char);
	static void FStringToAsciiChar(const FString& str, char* p_dest, INT32U dest_len);

	/// <summary>
	/// Byte operations
	/// </summary>
	/// <param name="str"></param>
	/// <param name="p_dest"></param>
	/// <param name="dest_capacity"></param>
	static void ByteCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void IPAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static void MacAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity);
	static AActor* GetTopParent(AActor *p_actor);
	static AActor* SpawnObjectFromBlueprint(AActor* p_template, FString blueprint_path, UWorld* p_world, AActor* p_owner, FString name, FVector pos, FVector rot_rpy, FVector scale);
	static FLOAT64 GetTimeSeconds();
	static inline INT16U LittleToBig(INT16U val);
	static inline INT32U LittleToBig(INT32U val);
	static inline INT32U ReverseCopyBytes(INT8U* p_src, INT8U* p_dest, INT32U len);
	static inline FLOAT32 GetRandomRange(FLOAT32 min_inclusive, FLOAT32 max_inclusive);
	static FVector GetActorRPY(AActor* p_actor);
	static AActor* GetParentActor(AActor *p_child);
	static void GetOwnAndParents(AActor* p_child, TArray<AActor*> &ret);
	static FLOAT64 Tick();
	static FLOAT64 Tock(FLOAT64 start_time_sec);
	static void SetParent(AActor* p_child, AActor* p_parent);
	static void SetActorActive(AActor* p_actor, bool is_active);

};

