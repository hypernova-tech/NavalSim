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
	static FLOAT64 Remap(FLOAT64 x, FLOAT64 x1, FLOAT64 x2, FLOAT64 y1, FLOAT64 y2);
	static FVector ProjectWorldDirectionToScreenSpace(APlayerController* PC, const FVector& world_dir);
	static FVector ToVec3(const FVector2D& vec2d);
}; 