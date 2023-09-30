#pragma once
#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"


class CMath
{

public:
	static FLOAT64 GetVisibleDistanceOverSurfaceMeter(const FVector& pos, const FVector& dir, FLOAT64 max_surface_penetration_meter);

	static void UnitTestVec();
	static void SetActorRotation(AActor* p_actor, FVector rpy_deg);
	static void SetActorRelativeRotation(AActor* p_actor, FVector rpy_deg);

	static FVector GetActorEulerAnglesRPY(AActor* p_actor);
	static FVector GetActorReleativeEulerAnglesRPY(AActor* p_actor);

	static FVector CoordAxisToEUAxis(ECoordAxis axis);
}; 