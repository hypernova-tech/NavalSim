#include "CMath.h"

FLOAT64 CMath::GetVisibleDistanceOverSurfaceMeter(const FVector& pos, const FVector& dir, FLOAT64 max_surface_penetration_meter)
{
    
    if (dir.Z < 0) {
        FLOAT32 range = FMath::Sqrt(dir.X * dir.X + dir.Y * dir.Y)/ -dir.Z * pos.Z;
        range = TOW(range) + (max_surface_penetration_meter);
        return range;
    }
    else {
        return -1.0f;
    }
}

void CMath::UnitTestVec()
{
    {
        FLOAT64 azimuth_deg = 0;
        FLOAT64 elevation_deg = 30;

        FRotator euler_yaw(0, azimuth_deg, 0);
        FRotator euler_pitch(elevation_deg, 0, 0);


        //FQuat qua = euler_rot.Quaternion();
        FVector look_dir = FVector::ForwardVector;

        FVector temp_dir = euler_pitch.RotateVector(look_dir);
        FVector new_dir = euler_yaw.RotateVector(temp_dir);
    }


    {
        FLOAT64 azimuth_deg = 30;
        FLOAT64 elevation_deg = 0;

        FRotator euler_yaw(0, azimuth_deg, 0);
        FRotator euler_pitch(elevation_deg, 0, 0);


        //FQuat qua = euler_rot.Quaternion();
        FVector look_dir = FVector::ForwardVector;

        FVector temp_dir = euler_pitch.RotateVector(look_dir);
        FVector new_dir = euler_yaw.RotateVector(temp_dir);
        new_dir = new_dir;
    }


    {

        FLOAT64 azimuth_deg = 0;
        FLOAT64 elevation_deg = 30;
        FVector look_dir = FVector::ForwardVector;

        FQuat QuatPitch(FVector::RightVector, elevation_deg * DEGTORAD);
        FQuat Yaw(FVector::UpVector, azimuth_deg * DEGTORAD);

        FVector temp = QuatPitch * (look_dir);
        FVector new_dir = Yaw * temp;
        new_dir = new_dir;
    }
}
