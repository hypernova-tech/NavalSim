// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Navigation/CNav.h"

CNav::CNav()
{
}

CNav::~CNav()
{
}

FLOAT64 CNav::ComputeCourseOverGroundDeg(FVector dir_ned)
{
    FLOAT64 AngleRad = FMath::Atan2(dir_ned.Y, dir_ned.X);

    // Convert the angle to degrees
    FLOAT64 AngleDeg = FMath::RadiansToDegrees(AngleRad);

    // Normalize the angle to the range [0, 360)
    FLOAT64 COG = FMath::Fmod(AngleDeg + 360.0f, 360.0f);

    return COG;
}
FLOAT64 CNav::ComputeBearingDeg(FVector dir_ned)
{
    FLOAT64 AngleRad = FMath::Atan2(dir_ned.Y, dir_ned.X);

    // Convert the angle to degrees
    FLOAT64 AngleDeg = FMath::RadiansToDegrees(AngleRad);

    // Normalize the angle to the range [0, 360)
    AngleDeg = FMath::Fmod(AngleDeg + 360.0f, 360.0f);

    return AngleDeg;
}