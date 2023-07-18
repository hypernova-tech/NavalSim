// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Utils/CScanResult.h"

/**
 * 
 */




class CUtil
{
public:
	CUtil();
	~CUtil();


public:
	static bool Trace(AActor* p_actor,float min_range_meter, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg, SScanResult* pscan_result);
	static bool ParallelTrace(AActor* p_actor, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg);

	static float ConvertToFloat(const char* p_str);
	static FString ConvertToFString(const char* p_str);
	static bool ParseVector2D(const FString& Text, FVector2D& ret);
	static bool ParseVector3D(const FString& Text, FVector& ret);
	static bool ParsePose(const FString& Text, FVector& translation, FVector& rpy);
	static void DebugLog(FString str);
};

