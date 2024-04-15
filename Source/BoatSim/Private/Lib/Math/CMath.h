#pragma once
#include "CoreMinimal.h"
#include "Lib/Types/Primitives.h"
#include <random>
using namespace std;

class CMath
{

public:
	static FLOAT64 GetVisibleDistanceOverSurfaceMeter(const FVector& pos, const FVector& dir, FLOAT64 max_surface_penetration_meter);

	static void UnitTestVec();
	static void SetActorRotation(AActor* p_actor, FVector rpy_deg);
	static void SetActorRelativeRotation(AActor* p_actor, FVector rpy_deg);

	static FVector GetActorEulerAnglesRPY(AActor* p_actor);
	static FVector GetActorReleativeEulerAnglesRPY(AActor* p_actor);
	static FVector GetEulerAnglesRPYDeg(const FVector& dir);
	static FVector CoordAxisToEUAxis(ECoordAxis axis);
	static FLOAT64 Remap(FLOAT64 x, FLOAT64 x1, FLOAT64 x2, FLOAT64 y1, FLOAT64 y2);
	static FVector ProjectWorldDirectionToScreenSpace(APlayerController* PC, const FVector& world_dir);
	static FVector ToVec3(const FVector2D& vec2d);
	static bool IsZero(FVector vec, FLOAT64 tolerance);
	static bool IsPointInsideVolume(const FVector& origin, const FVector& point, FLOAT64 min_elevation_deg, FLOAT64 max_elevation_deg, FLOAT64 min_azimuth_deg, FLOAT64 max_azimuth_deg, FLOAT64 min_range_meter, FLOAT64 max_range_meter);
	static void RotateRelative(AActor* p_actor, USceneComponent* p_comp, const FTransform& p_comp_initial_rel_transform, const FVector& rpy_deg);

	static TArray<FVector> GetBoxCornersAndCenter(const FBox& Box);
	static bool CheckBoxInsideVolume(const FBox& box, FVector& origin, FLOAT64 min_elevation_deg, FLOAT64 max_elevation_deg, FLOAT64 min_azimuth_deg, FLOAT64 max_azimuth_deg, FLOAT64 min_range_meter, FLOAT64 max_range_meter);


	static INT32S EncodeSigned32(FLOAT64 val, FLOAT64 lsb);
	static INT16S EncodeSigned16(FLOAT64 val, FLOAT64 lsb);
	static INT16U EncodeUnsigned16(FLOAT64 val, FLOAT64 lsb);

	static void SmoothTransformLerp(const FTransform& CurrentTargetTransform, const FTransform& TargetTransform, float DeltaTime, float InterpSpeed, FTransform& new_transform);
	static inline double GaussianRandom(double mean, double stddev)
	{
		static double z1;
		static int generate = 0;
		generate = !generate;

		// Generate a pair of Gaussian numbers on every other call
		if (!generate) {
			return mean + z1 * stddev;
		}

		double u1, u2;
		do {
			u1 = rand() / (RAND_MAX + 1.0);
			u2 = rand() / (RAND_MAX + 1.0);
		} while (u1 <= 1e-10); // Avoid division by zero

		// Box-Muller transform
		double z0;
		double sqrtlog = sqrt(-2.0 * log(u1));
		z0 = sqrtlog * cos(2.0 * PI * u2);
		z1 = sqrtlog * sin(2.0 * PI * u2);

		return mean + z0 * stddev;
	}

	
	static inline  double GetRandomRange(bool is_normal, double mean, double std, double min, double max)
	{
		double val;
		if (is_normal) {
			val = GaussianRandom(mean, std);
			val = val < min ? min : (val > max ? max : val);

		}
		else {
			val = FMath::RandRange(min, max);
		}
		return val;
	};
	/// <summary>
	/// uniform distribution
	/// </summary>
	/// <param name="min_inclusive"></param>
	/// <param name="max_inclusive"></param>
	/// <returns></returns>
	static inline FLOAT64  GetRandomRange(FLOAT64 min_inclusive, FLOAT64 max_inclusive)
	{
		return FMath::RandRange(min_inclusive, max_inclusive);
	};

}; 