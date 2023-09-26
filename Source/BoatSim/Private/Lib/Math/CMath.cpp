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

void CMath::SetActorRotation(AActor* p_actor, FVector rpy_deg)
{
    FQuat Quat = FQuat::MakeFromEuler(rpy_deg);

    // Set the actor's rotation
    p_actor->SetActorRotation(Quat);
}

void CMath::SetActorRelativeRotation(AActor* p_actor, FVector rpy_deg)
{
    FQuat Quat = FQuat::MakeFromEuler(rpy_deg);
    p_actor->SetActorRelativeRotation(Quat);
}

FVector CMath::GetActorEulerAnglesRPY(AActor* p_actor)
{
    
    FRotator ActorRotation = p_actor->GetActorRotation();

    FLOAT64 Roll = ActorRotation.Roll;
    FLOAT64 Pitch = ActorRotation.Pitch;
    FLOAT64 Yaw = ActorRotation.Yaw;


    return FVector(-Roll, -Pitch, Yaw);
}
FVector CMath::GetActorReleativeEulerAnglesRPY(AActor* p_actor)
{
   
    if (p_actor && p_actor->GetRootComponent() && p_actor->GetRootComponent()->GetAttachParent())
    {
        FRotator RelativeRotation = p_actor->GetRootComponent()->GetRelativeRotation();

        FLOAT64 Roll = RelativeRotation.Roll;
        FLOAT64 Pitch = RelativeRotation.Pitch;
        FLOAT64 Yaw = RelativeRotation.Yaw;
        return FVector(-Roll, -Pitch, Yaw);
       
    }
    else {
        return CMath::GetActorEulerAnglesRPY(p_actor);
    }
}