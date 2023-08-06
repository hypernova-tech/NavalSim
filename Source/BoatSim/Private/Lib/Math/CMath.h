#pragma once
#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"


class CMath
{

public:
	static FLOAT64 GetVisibleDistanceOverSurfaceMeter(const FVector& pos, const FVector& dir, FLOAT64 max_surface_penetration_meter);

	static void UnitTestVec();
};