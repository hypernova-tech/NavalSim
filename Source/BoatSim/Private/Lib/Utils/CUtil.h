// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CUtil
{
public:
	CUtil();
	~CUtil();


public:
	static bool Trace(AActor* p_actor, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg);
	static bool ParallelTrace(AActor* p_actor, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg);

	
};
