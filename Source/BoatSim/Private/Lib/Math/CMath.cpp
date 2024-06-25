#include "CMath.h"
#include <random>

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
    FVector v = FVector(-rpy_deg.X, -rpy_deg.Y, rpy_deg.Z);
    FQuat Quat = FQuat::MakeFromEuler(v);
    
    // Set the actor's rotation
    p_actor->SetActorRotation(Quat);
}

void CMath::SetActorRelativeRotation(AActor* p_actor, FVector rpy_deg)
{
    FVector v = FVector(-rpy_deg.X, -rpy_deg.Y, rpy_deg.Z);
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

FVector CMath::FindRPYFromVectors(const FVector& Vec, const FVector& Dir)
{
    // Ensure vectors are normalized
    FVector VecNormalized = Vec.GetSafeNormal();
    FVector DirNormalized = Dir.GetSafeNormal();

    // Calculate the quaternion for rotation
    FQuat RotationQuat = FQuat::FindBetween(VecNormalized, DirNormalized);

    // Convert quaternion to Euler angles (Roll, Pitch, Yaw)
    FRotator RotatorFromQuat = RotationQuat.Rotator();

    FLOAT64 Roll = RotatorFromQuat.Roll;
    FLOAT64 Pitch = RotatorFromQuat.Pitch;
    FLOAT64 Yaw = RotatorFromQuat.Yaw;
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
    return remappedValue;//FMath::Clamp(remappedValue, y1, y2);
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
bool CMath::IsZero(double  val, FLOAT64 tolerance)
{
    return (FMath::Abs(val) <= tolerance);
}
bool CMath::IsZero(FVector vec, FLOAT64 tolerance)
{
    return (FMath::Abs(vec.X)<= tolerance) && (FMath::Abs(vec.Y) <= tolerance) && (FMath::Abs(vec.Z) <= tolerance);
}
bool CMath::IsPointInsideVolume(bool is_local, const FVector& origin, const FVector dir, const FVector& point, FLOAT64 min_elevation_deg, FLOAT64 max_elevation_deg, FLOAT64 min_azimuth_deg, FLOAT64 max_azimuth_deg, FLOAT64 min_range_meter, FLOAT64 max_range_meter, FVector& local_rpy)
{
    // Convert elevation and azimuth ranges from degrees to radians
   

    // Calculate vector from origin to point
    FVector direction_to_pt = point - origin;
    float dist_to_pt = TOW(direction_to_pt.Size());

    // Check if the point is within the range
    if (dist_to_pt< min_range_meter || dist_to_pt > max_range_meter)
    {
        return false;
    }


    // Normalize the direction vector
    direction_to_pt.Normalize();
    float elev_ang;
    float azim_angle;

    if (is_local) {
        local_rpy = CMath::FindRPYFromVectors(dir, direction_to_pt);
        // Calculate elevation and azimuth angles for the point
        elev_ang = local_rpy.Y;//-FMath::Asin((direction_to_pt-dir).Z)*RADTODEG;
        azim_angle = local_rpy.Z;// FMath::Atan2(direction_to_pt.Y, direction_to_pt.X)* RADTODEG;
        // Check if the point is within the elevation range
       
    }
    else {
        elev_ang = -FMath::Asin(direction_to_pt.Z) * RADTODEG;
        azim_angle = FMath::Atan2(direction_to_pt.Y, direction_to_pt.X) * RADTODEG;
        if (azim_angle < 0) {
            azim_angle += 360;
        }
    }
    
    if (elev_ang < min_elevation_deg || elev_ang > max_elevation_deg)
    {
        return false;
    }

    if (azim_angle < min_azimuth_deg || azim_angle > max_azimuth_deg)
    {
        return false;
    }
    
    // Check if the point is within the azimuth range
    return true;
}
bool CMath::CheckBoxInsideVolume(bool is_local, const FBox& box, const FVector& origin, const FVector &dir, FLOAT64 min_elevation_deg, FLOAT64 max_elevation_deg, FLOAT64 min_azimuth_deg, FLOAT64 max_azimuth_deg, FLOAT64 min_range_meter, FLOAT64 max_range_meter, FVector& local_rpy)
{
    auto corners = GetBoxCornersAndCenter(box);

    for (auto corner : corners) {
        bool ret = CMath::IsPointInsideVolume(is_local, origin, dir, corner, min_elevation_deg, max_elevation_deg, min_azimuth_deg, max_azimuth_deg, min_range_meter, max_range_meter, local_rpy);
        if (!ret) {
            return false;
        }
    }
    
    return true;
}

INT32S CMath::EncodeSigned32(FLOAT64 val, FLOAT64 lsb)
{
    INT32S ret = (INT32S) (val / lsb);
    return ret;
}

INT16S CMath::EncodeSigned16(FLOAT64 val, FLOAT64 lsb)
{
    INT16S ret = (INT16S)(val / lsb);
    return ret;
}

INT16U CMath::EncodeUnsigned16(FLOAT64 val, FLOAT64 lsb)
{
    INT16U ret = (INT16U)(val / lsb);
    return ret;
}
INT32U CMath::EncodeUnsigned32(FLOAT64 val, FLOAT64 lsb)
{
    INT32U ret = (INT32U)(val / lsb);
    return ret;
}
FVector CMath::GetEulerAnglesRPYDeg(const FVector& dir)
{


    // Calculate elevation and azimuth angles for the point
    FLOAT64 elev_ang = -FMath::Asin(dir.Z) * RADTODEG;
    FLOAT64 azim_angle = FMath::Atan2(dir.Y, dir.X) * RADTODEG;
    if (azim_angle < 0) {
        azim_angle += 360;
    }
    return FVector(0, elev_ang , azim_angle);

}




TArray<FVector> CMath:: GetBoxCornersAndCenter(const FBox& Box)
{
    TArray<FVector> Points;

    // Adding the corners
    FVector top_right_front     = (FVector(Box.Max.X, Box.Max.Y, Box.Max.Z)); // 
    FVector top_left_front      = (FVector(Box.Min.X, Box.Max.Y, Box.Max.Z)); // 
    FVector bottom_right_front  = (FVector(Box.Max.X, Box.Min.Y, Box.Max.Z)); // 
    FVector bottom_left_front   = (FVector(Box.Min.X, Box.Min.Y, Box.Max.Z)); // 
    FVector top_right_back      = (FVector(Box.Max.X, Box.Max.Y, Box.Min.Z)); // 
    FVector top_left_back       = (FVector(Box.Min.X, Box.Max.Y, Box.Min.Z)); // 
    FVector bottom_right_back   = (FVector(Box.Max.X, Box.Min.Y, Box.Min.Z)); // 
    FVector bottom_left_back    = (FVector(Box.Min.X, Box.Min.Y, Box.Min.Z)); // 

    // Adding the center
    Points.Add((Box.Min + Box.Max) * 0.5f); // center

    Points.Add(top_right_front);
    Points.Add(top_left_front);
    Points.Add(bottom_right_front);
    Points.Add(bottom_left_front);
    Points.Add(top_right_back);
    Points.Add(top_left_back);
    Points.Add(bottom_right_back);
    Points.Add(bottom_left_back);

  

    return Points;
}

void CMath::RotateRelative(AActor* p_actor, USceneComponent* p_comp, const FTransform& p_comp_initial_rel_transform, const FVector& rpy_deg)
{
    if (!p_actor || !p_comp)
    {
        UE_LOG(LogTemp, Warning, TEXT("RotateRelative: Actor or Component is null"));
        return;
    }

    FRotator RotationDelta(rpy_deg.Y, rpy_deg.Z, rpy_deg.X);


    FTransform final_trasform = p_comp_initial_rel_transform * FTransform(RotationDelta.Quaternion())  ;

   
    // Apply the final transform to the child component
    p_comp->SetRelativeTransform(final_trasform);
   
}
void CMath::SmoothTransformLerp(const FTransform& CurrentTransform, const FTransform& TargetTransform, float DeltaTime, float InterpSpeed,  FTransform& new_transform)
{
   
    // Interpolate location
    FVector NewLocation = FMath::Lerp(CurrentTransform.GetLocation(), TargetTransform.GetLocation(), InterpSpeed * DeltaTime);

    // Interpolate rotation
    // Note: RInterpTo works with FRotator, so we convert to and from FQuat as necessary
    FRotator NewRotation = FMath::RInterpTo(CurrentTransform.GetRotation().Rotator(), TargetTransform.GetRotation().Rotator(), DeltaTime, InterpSpeed);

    // Interpolate scale
    FVector NewScale = FMath::Lerp(CurrentTransform.GetScale3D(), TargetTransform.GetScale3D(), InterpSpeed * DeltaTime);


    new_transform = FTransform(NewRotation, NewLocation, NewScale);
}

double CMath::FixAngPlusMinus180(double val)
{
    double ret = val;

    if (val > 180) {
        ret = 360 - val;
    }
    else if (val <= -180) {
        ret = 360 + val;
    }

    return ret;
}

FVector CMath::FixAngPlusMinus180(FVector ang)
{
    FVector ret;
    ret.X = CMath::FixAngPlusMinus180(ang.X);
    ret.Y = CMath::FixAngPlusMinus180(ang.Y);
    ret.Z = CMath::FixAngPlusMinus180(ang.Z);

    return ret;
}

FVector CMath::FindWorldLocation(AActor* OwningActor, double AzimuthDeg, double ElevationDeg, double Range)
{
    // Ensure the OwningActor is valid
    if (!OwningActor)
    {
        return FVector::ZeroVector;
    }

    // Convert azimuth and elevation from degrees to radians
    double AzimuthRad = AzimuthDeg * DEGTORAD;
    double ElevationRad = ElevationDeg * DEGTORAD;

    // Calculate the Cartesian coordinates in local space
    float X = Range * FMath::Cos(ElevationRad) * FMath::Cos(AzimuthRad);
    float Y = Range * FMath::Cos(ElevationRad) * FMath::Sin(AzimuthRad);
    float Z = Range * FMath::Sin(ElevationRad);

    FVector LocalLocation(X, Y, Z);

    FTransform ActorTransform = OwningActor->GetActorTransform();

    // Transform the local coordinates to world coordinates
    FVector WorldLocation = ActorTransform.TransformPosition(LocalLocation);

    return WorldLocation;
}



// Function to calculate the Euclidean distance between two points
double CMath::CalculateDistance(int x1, int y1, int x2, int y2) 
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Function to get the list of N nearest neighboring points within the bounds of the grid
int CMath::GetNearestNeighbors(int x, int y, int rows, int cols, int N, SNeighbour* neighbors) 
{
    // List to store all points in the grid except the given point
    std::vector<SNeighbour> allNeighbors;

    // Iterate through the grid and add all points except the given point
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i != x || j != y) {
                allNeighbors.push_back({ i, j });
            }
        }
    }

    // Sort the neighbors by distance to the given point (x, y)
    std::sort(allNeighbors.begin(), allNeighbors.end(), [x, y](SNeighbour a, SNeighbour b) {
        return CalculateDistance(x, y, a.x, a.y) < CalculateDistance(x, y, b.x, b.y);
        });

    // Resize the neighbors vector if there are more than N neighbors
    if (allNeighbors.size() > N) {
        allNeighbors.resize(N);
    }

    // Populate the neighbors array and return the size
    int size = std::min(N, static_cast<int>(allNeighbors.size()));
    for (int i = 0; i < size; ++i) {
        neighbors[i] = allNeighbors[i];
    }

    return size;
}

double CMath::GetDistXY(const FVector& vec)
{
    return FMath::Sqrt(vec.X* vec.X + vec.Y*vec.Y);
}
