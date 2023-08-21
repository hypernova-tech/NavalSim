// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ScreenDepth/ScreenDepthCalculator.h"

CScreenDepthCalculator::CScreenDepthCalculator()
{
}

CScreenDepthCalculator::~CScreenDepthCalculator()
{
}
FVector2D CScreenDepthCalculator::ComputePixelFromAzimuthElevation(float azimuth_deg, float elevation_deg, float fov_deg, float aspect_ratio)
{

    // Ensure Azimuth and Elevation are in radians. If they're in degrees, convert them:
    auto Azimuth = FMath::DegreesToRadians(azimuth_deg);
    auto Elevation = FMath::DegreesToRadians(elevation_deg);
    auto fov_radian = FMath::DegreesToRadians(fov_deg);

    // Ensure FOV is in radians. If it's in degrees, uncomment the following line:
    // FOV = FMath::DegreesToRadians(FOV);

    // 1. Compute Tangent of Half FOV
    float tanHalfFOV = FMath::Tan(fov_radian * 0.5f);

    // 2. Compute Direction Vector from Azimuth and Elevation
    FVector dir(FMath::Cos(Elevation) * FMath::Cos(Azimuth),
        FMath::Cos(Elevation) * FMath::Sin(Azimuth),
        FMath::Sin(Elevation));

    // 3. Project the Direction Vector to NDC
    float PixelNDC_X = dir.X / (dir.Z * tanHalfFOV * aspect_ratio);
    float PixelNDC_Y = dir.Y / (dir.Z * tanHalfFOV);

    return FVector2D(PixelNDC_X, PixelNDC_Y);
}