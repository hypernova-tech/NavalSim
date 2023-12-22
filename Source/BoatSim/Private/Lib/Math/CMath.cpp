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
    FVector v = FVector(rpy_deg.X, rpy_deg.Y, rpy_deg.Z);
    FQuat Quat = FQuat::MakeFromEuler(v);

    // Set the actor's rotation
    p_actor->SetActorRotation(Quat);
}

void CMath::SetActorRelativeRotation(AActor* p_actor, FVector rpy_deg)
{
    FVector v = FVector(rpy_deg.X, rpy_deg.Y, rpy_deg.Z);
    FQuat Quat = FQuat::MakeFromEuler(v);
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

FVector CMath::CoordAxisToEUAxis(ECoordAxis axis)
{
    switch (axis) {
    case CoordAxisX:
        return FVector::ForwardVector;
    case CoordAxisY:
        return FVector::RightVector;
    case CoordAxisZ:
        return FVector::UpVector;
    }

    return FVector::ZeroVector;
}
FLOAT64 CMath::Remap(FLOAT64 x, FLOAT64 x1, FLOAT64 x2, FLOAT64 y1, FLOAT64 y2)
{
    // Clamp input to the range [x1, x2]
    x = FMath::Clamp(x, x1, x2);

    // Compute the remapped value
    FLOAT64 remappedValue = y1 + (x - x1) * (y2 - y1) / (x2 - x1);

    // Clamp the remapped value to the range [y1, y2]
    return FMath::Clamp(remappedValue, y1, y2);
}

FVector CMath::ProjectWorldDirectionToScreenSpace(APlayerController* PC, const FVector& world_dir)
{
    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector ViewDirection = CameraRotation.Vector();
    FVector pos = CameraLocation + ViewDirection * 10;
    FVector end = pos + world_dir * 10;

    FVector2D start_pos_screen;
    FVector2D end_pos_screen;

    PC->ProjectWorldLocationToScreen(pos, start_pos_screen);
    PC->ProjectWorldLocationToScreen(end, end_pos_screen);

    FVector ret;
    FVector2D diff_2d = end_pos_screen - start_pos_screen;
    ret.X = diff_2d.X;
    ret.Y = diff_2d.Y;
    ret.Z = 0;

    return ret;

}

FVector CMath::ToVec3(const FVector2D& vec2d)
{
    FVector ret;

    ret.X = vec2d.X;
    ret.Y = vec2d.Y;
    ret.Z = 0;

    return ret;
}

bool CMath::IsZero(FVector vec, FLOAT64 tolerance)
{
    return (FMath::Abs(vec.X)<= tolerance) && (FMath::Abs(vec.Y) <= tolerance) && (FMath::Abs(vec.Z) <= tolerance);
}
