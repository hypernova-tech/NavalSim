#pragma once

#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"

struct SClutterParams
{

public:

	// Use the UPROPERTY macro to expose properties to the Unreal Editor
	bool EnableSubsurfaceScan;

	bool EnableSurfaceDetect;

	bool EnableFoamDetect;

	FLOAT32 SeaSurfaceDetectionProbability; // Unreal typically uses "float" rather than "FLOAT32".

	FLOAT32 MaxSurfacePenetrationMeter; // Unreal typically uses "float" rather than "FLOAT32".

	SClutterParams()
	{
		EnableSubsurfaceScan = false;
		EnableSurfaceDetect = false;
		EnableFoamDetect = false;
		SeaSurfaceDetectionProbability = 0;
		MaxSurfacePenetrationMeter = 0;
	}
};
