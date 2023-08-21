// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Lib/Types/Primitives.h"
/**
 * 
 */
class CScreenDepthCalculator
{
public:
	CScreenDepthCalculator();
	~CScreenDepthCalculator();

	FVector2D ComputePixelFromAzimuthElevation(float azimuth_deg, float elevation_deg, float fov_deg, float aspect_ratio);
	
};
