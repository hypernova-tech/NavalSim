// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Utils/CUtil.h"

CUtil::CUtil()
{
}

CUtil::~CUtil()
{
}

bool CUtil::Trace(AActor * p_actor, bool is_world, float min_range_meter, float range_meter, float azimuth_start_deg, float azimuth_end_deg,
                                                                          float elevation_start_deg, float elevation_end_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg,
                                                                          bool show_radar_beam, TArray<AActor*>& ignore_list, SScanResult* pscan_result)
{



    FVector start_pos = p_actor->GetActorLocation();
    FVector look_dir;
    FVector right_vec;
    
    if (is_world) {
        look_dir = FVector::ForwardVector;
        look_dir.Z = p_actor->GetActorForwardVector().Z;
        look_dir.Normalize();
    }
    else {
        look_dir = p_actor->GetActorForwardVector();
        look_dir.Normalize();
        right_vec = p_actor->GetActorRightVector();
        right_vec.Z = 0;
        right_vec.Normalize();

    }
  

    look_dir.Normalize();

    pscan_result->ScanCenter = start_pos;

    FCollisionQueryParams query_params;
    query_params.AddIgnoredActor(p_actor);

    for (AActor* p_temp : ignore_list) {
        query_params.AddIgnoredActor(p_temp);
    }
    query_params.bTraceComplex = false;





    FVector end;
    FHitResult result;
    bool ret = false;
    int horizantal_ind = 0;
    int vertical_ind = 0;

    int success_count = 0;

    pscan_result->Point3DList.Reset();
    //pscan_result->Point2DScreen.Reset();

    float one_over_range_unreal = 1.0 / WORLD_TO_UNREAL(range_meter);

    int sector_ind = azimuth_start_deg / (360.0 / pscan_result->SectorCount);
    SSectorInfo* p_current_sektor = pscan_result->GetSectorContainer()->GetSector(sector_ind);
    p_current_sektor->Reset();

    for (float azimuth = azimuth_start_deg; azimuth <= azimuth_end_deg; azimuth += azimuth_angle_step_deg) {
        vertical_ind = 0;
        float azimuth_rad = azimuth * DEGTORAD;
   
        for (float elevation = elevation_start_deg; elevation <= elevation_end_deg; elevation += elevation_angle_step_deg) {
            FVector new_dir;

            if (is_world) {

                FRotator euler_yaw(0, azimuth, 0);
                FRotator euler_pitch(elevation, 0, 0);


                //FQuat qua = euler_rot.Quaternion();

                FVector temp_dir = euler_pitch.RotateVector(look_dir);
                new_dir = euler_yaw.RotateVector(temp_dir);

                start_pos = p_actor->GetActorLocation() + new_dir * WORLD_TO_UNREAL(min_range_meter);
                end = start_pos + new_dir * WORLD_TO_UNREAL(range_meter);

            }
            else {
                FQuat QuatPitch(right_vec, elevation * DEGTORAD);
                FQuat Yaw(FVector::UpVector, azimuth * DEGTORAD);
              

                new_dir = Yaw * QuatPitch * (look_dir);

                start_pos = p_actor->GetActorLocation() + new_dir * WORLD_TO_UNREAL(min_range_meter);
                end = start_pos + new_dir * WORLD_TO_UNREAL(range_meter);
            }


            ret = p_actor->GetWorld()->LineTraceSingleByChannel(result, start_pos, end, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());
            if (show_radar_beam) {
                DrawDebugLine(p_actor->GetWorld(), start_pos, end, FColor::Green, false, 0.2f);
            }
            
            if(ret){
                //DrawDebugLine(p_actor->GetWorld(), start_pos,start_pos + new_dir * result.Distance,FColor::Red,false, 0.2f);
                pscan_result->Range[horizantal_ind][vertical_ind] = result.Distance;
                pscan_result->Point3D[horizantal_ind][vertical_ind] = result.Location;
                FVector manual_pos = start_pos + new_dir * result.Distance;
                pscan_result->Point3DList.Add(manual_pos);

                p_current_sektor->Add(result.Location);
            
                success_count++;
            }
            vertical_ind++;
        }
        horizantal_ind++;
    }

    pscan_result->HorizontalCount = horizantal_ind;
    pscan_result->VeriticalCount = vertical_ind;


    return success_count > 0;
}

bool CUtil::ParallelTrace(AActor* p_actor, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg)
{
#if false
    AActor* p_actor = GetOwner();
    FVector start_pos = p_actor->GetActorLocation();
    FVector look_dir = p_actor->GetActorForwardVector();

    FCollisionQueryParams query_params;
    query_params.AddIgnoredActor(p_actor);
    query_params.bTraceComplex = false;

    float azimuth_start = -BeamWidthAzimuthDeg / 2.0f;
    float azimuth_end = BeamWidthAzimuthDeg / 2.0f;

    float elevation_start = 0;
    float elevation_end = BeamWidthElevationDeg;

    FVector end;
    FHitResult result;
    bool ret = false;

    // Parallelize the azimuth loop
    FParallelForTask ParallelAzimuthTask(static_cast<int32>((azimuth_end - azimuth_start) / AzimutAngleStepDeg) + 1, [&](int32 AzimuthIndex)
        {
            float azimuth = azimuth_start + (AzimuthIndex * AzimutAngleStepDeg);

            // Parallelize the elevation loop
            FParallelForTask ParallelElevationTask(static_cast<int32>((elevation_end - elevation_start) / ElevationAngleStepDeg) + 1, [&](int32 ElevationIndex)
                {
                    float elevation = elevation_start + (ElevationIndex * ElevationAngleStepDeg);

                    FRotator euler_rot(elevation, azimuth, 0);
                    FQuat qua = euler_rot.Quaternion();
                    FVector new_dir = qua.RotateVector(look_dir);
                    end = start_pos + new_dir * LidarRangeMeter * 100;

                    bool hit = p_actor->GetWorld()->LineTraceSingleByChannel(result, start_pos, end, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());
                    if (hit)
                    {
                        // DrawDebugLine(p_actor->GetWorld(), start_pos, start_pos + new_dir * 2000, FColor::Red, false, 0.2f);
                    }

                    // Set the return value to true if any trace hits
                    if (hit)
                    {
                        ret = true;
                    }
                });

            ParallelElevationTask.DoTask();
            ParallelElevationTask.EnsureCompletion();
        });

    ParallelAzimuthTask.DoTask();
    ParallelAzimuthTask.EnsureCompletion();

    return ret;

#endif
    return false;
}

float CUtil::ConvertToFloat(const char* p_str)
{
    const char* textValue = p_str;
    FString text_val = FString(textValue);
    float val = FCString::Atof(*text_val);
    return val;
}
FString CUtil::ConvertToFString(const char* p_str)
{
    
    FString text_val = FString(p_str);
   
    return text_val;
}
bool CUtil::ParseVector2D(const FString& Text, FVector2D& ret)
{
    FVector ParsedVector;
    TArray<FString> VectorValues;
    Text.ParseIntoArray(VectorValues, TEXT(" "), true);

    if (VectorValues.Num() >= 2)
    {
        ret.X = FCString::Atof(*VectorValues[0]);
        ret.Y = FCString::Atof(*VectorValues[1]);


        return true;
    }

    return false;
}
bool CUtil::ParseVector3D(const FString& Text, FVector& ret)
{
    FVector ParsedVector;
    TArray<FString> VectorValues;
    Text.ParseIntoArray(VectorValues, TEXT(" "), true);

    if (VectorValues.Num() >= 3)
    {
        ret.X = FCString::Atof(*VectorValues[0]);
        ret.Y = FCString::Atof(*VectorValues[1]);
        ret.Z = FCString::Atof(*VectorValues[2]);

        return true;
    }

    return false;
}

bool CUtil::ParsePose(const FString& Text, FVector& translation, FVector& rpy)
{
    FVector ParsedVector;
    TArray<FString> VectorValues;
    Text.ParseIntoArray(VectorValues, TEXT(" "), true);

    if (VectorValues.Num() >= 6)
    {
        translation.X = FCString::Atof(*VectorValues[0]);
        translation.Y = FCString::Atof(*VectorValues[1]);
        translation.Z = FCString::Atof(*VectorValues[2]);

        rpy.X = FCString::Atof(*VectorValues[3]);
        rpy.Y = FCString::Atof(*VectorValues[4]);
        rpy.Z = FCString::Atof(*VectorValues[5]);

        return true;
    }

    return false;
}
void CUtil::DebugLog(FString str)
{
    UE_LOG(LogTemp, Warning,TEXT("%s"), *str);

}

void CUtil::LookAt(AActor* p_actor, FVector& look_dir)
{
    FVector ActorLocation = p_actor->GetActorLocation();
    FVector TargetLocation = ActorLocation + look_dir * 5;
    FVector DirectionToTarget = (TargetLocation - ActorLocation).GetSafeNormal();
    FRotator NewRotation = DirectionToTarget.Rotation();
}

FRotator CUtil::GetActorRelativeRotation(AActor* p_actor)
{
   
    AActor* p_parent_actor  = p_actor->GetParentActor(); // Parent actor, if applicable

    // Get the actor's transform
    FTransform actor_transform = p_actor->GetActorTransform();

    // Get the parent's transform (if applicable)
   

    // Calculate the relative rotation
    FRotator relative_rotation;

    if (p_parent_actor)
    {
        FTransform parent_transform = p_parent_actor->GetActorTransform();
        // If the actor has a parent, calculate relative rotation
        // by subtracting parent's rotation from actor's rotation
        relative_rotation = (actor_transform.GetRotation().Inverse() * p_parent_actor->GetActorRotation().Quaternion()).Rotator();
    }
    else
    {
        // If there's no parent, the relative rotation is just the actor's rotation
        relative_rotation = p_actor->GetActorRotation();
    }
    
    return relative_rotation;
}

int CUtil::StringToInt(FString& str)
{
    
    int ret = FCString::Atoi(*str);

    return ret;
}

AActor* CUtil::GetTopParent(AActor *p_actor)
{
    AActor* p_temp = p_actor;
    int depth = 0;

    while (true) {
        if (p_temp->GetAttachParentActor() == nullptr) {
            if (depth == 0) {
                return nullptr;
            }
            else {
                return  p_temp;
            }
            
        }

        p_temp = p_temp->GetAttachParentActor();
        depth++;

   }

    return nullptr;

}

float CUtil::StringToFloat(FString& str)
{
    float ret = FCString::Atof(*str);

    return ret;
}

double CUtil::StringToFloat64(FString& str)
{
    double ret = FCString::Atod(*str);

    return ret;
}
template <typename T>
T* CUtil::FindComponent(AActor* p_parent) 
{
    TArray<AActor*> child_actors;
    T* p_ret = p_parent->FindComponentByClass<T>();

    return p_ret;

    for (AActor* p_child : child_actors)
    {
      
        // Check if the child actor is of the desired class
        if (p_child->IsA<T>())
        {
            T *ret  = Cast<T>(p_child);
            // Perform actions with the found child actor
            return ret;
        }
    }

    return nullptr;
}

template<typename T>
T* CUtil::FindChildComponent(AActor* p_parent)
{
    TArray<AActor*> child_actors;
    p_parent->GetAllChildActors(child_actors, true);

    for (AActor* child : child_actors)
    {
       
        // Check if the child actor is of the desired class
        if (child->IsA<T>())
        {
            T *p_ret = Cast<T>(child);
            // Perform actions with the found child actor
            return p_ret;
        }
    }

    return nullptr;
}

AActor* CUtil::SpawnObjectFromBlueprint(FString blueprint_path, UWorld *p_world, AActor *p_owner, FString name, FVector pos, FVector rot_rpy)
{
    // The blueprint name should be the path to the blueprint asset, relative to the Content folder.
    FString BlueprintPath = blueprint_path;

    // Load the blueprint object from the specified path.
    UBlueprint* BlueprintObject = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *BlueprintPath));

    if (!BlueprintObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load blueprint at path: %s"), *BlueprintPath);
        return nullptr;
    }

    // Get the world reference to spawn the object.
    UWorld* World = p_world;
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get the world reference."));
        return nullptr;
    }

    // Create the object from the blueprint and spawn it in the world.
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(*name);
    SpawnParams.Owner = p_owner; // Set the owner if required.
    SpawnParams.Instigator = nullptr; // Set the instigator if required.
    AActor* SpawnedActor = World->SpawnActor<AActor>(BlueprintObject->GeneratedClass, pos, FRotator(rot_rpy.Y,rot_rpy.Z, rot_rpy.X), SpawnParams);

    if (!SpawnedActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn the actor from the blueprint."));
        return nullptr;
    }

    return SpawnedActor;
}
