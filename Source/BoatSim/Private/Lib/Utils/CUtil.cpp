﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Utils/CUtil.h"
#include <Lib/Math/CMath.h>




CUtil::CUtil()
{
}

CUtil::~CUtil()
{
}

bool CUtil::Trace(const STraceArgs& args, SScanResult* pscan_result)
{
#if true
    auto trace_start_sec = CUtil::Tick();

    FVector start_pos = args.p_actor->GetActorLocation();
    FVector look_dir;
    FVector right_vec;

    FVector end;
    FHitResult result;
    bool ret = false;
    int horizantal_ind = 0;
    int vertical_ind = 0;

    int success_count = 0;

    if (args.is_world) {
        look_dir = FVector::ForwardVector;
        look_dir.Z = args.p_actor->GetActorForwardVector().Z;
        look_dir.Normalize();
    }
    else {
        look_dir = args.p_actor->GetActorForwardVector();
        look_dir.Normalize();
        right_vec = args.p_actor->GetActorRightVector();
        right_vec.Z = 0;
        right_vec.Normalize();

    }


    look_dir.Normalize();

    pscan_result->ScanCenter = start_pos;

    FCollisionQueryParams query_params;
    query_params.AddIgnoredActor(args.p_actor);

    for (AActor* p_temp : *args.p_ignore_list) {
        query_params.AddIgnoredActor(p_temp);
    }
    query_params.bTraceComplex = false;




    pscan_result->ResetTrackPoint3DList();

    int sector_ind = args.azimuth_start_deg / (360.0 / pscan_result->SectorCount);
    SSectorInfo* p_current_sektor = pscan_result->GetSectorContainer()->GetSector(sector_ind);
    p_current_sektor->Reset();
    pscan_result->CurrentSector = sector_ind;

    FVector start_loc = args.p_actor->GetActorLocation();

    BOOLEAN is_on_surface = false;

    pscan_result->AzimuthRange.X = args.azimuth_start_deg;
    pscan_result->AzimuthRange.Y = args.azimuth_end_deg;
    pscan_result->ScanAzimuthStepDeg = args.azimuth_angle_step_deg;

    pscan_result->ElevationRange.X = args.elevation_start_deg;
    pscan_result->ElevationRange.Y = args.elevation_end_deg;
    pscan_result->ScanElevationStepDeg = args.elevation_angle_step_deg;

    pscan_result->ScanRangeMeter = args.range_meter;

    pscan_result->ScanCenter = args.scan_center;
    pscan_result->ScanRPYWorld = args.scan_rpy_world_deg;

    for (float azimuth = args.azimuth_start_deg; azimuth <= args.azimuth_end_deg; azimuth += args.azimuth_angle_step_deg) {
        vertical_ind = 0;
        float azimuth_rad = azimuth * DEGTORAD;
        FVector new_dir;
        for (float elevation = args.elevation_start_deg; elevation <= args.elevation_end_deg; elevation += args.elevation_angle_step_deg) {
            FLOAT64 filtered_range_meter = args.range_meter;
            FLOAT32 error_meter = args.measurement_error_mean + args.measurement_error_std * 0.33f * GetRandomRange(-1.0f, 1.0f);


            if (args.is_world) {
                /**

                unreal engine pitch ve roll eksenleri sol el kuralının tersine oluyor, normalde -pitch yapmamız gerekirken bu özellikden
                dolayı pozitif pitch veriyoruz.
                */
                FRotator euler_yaw(0, azimuth, 0);
                FRotator euler_pitch(elevation, 0, 0);

                FVector temp_dir = euler_pitch.RotateVector(look_dir);
                new_dir = euler_yaw.RotateVector(temp_dir);

                start_pos = start_loc + new_dir * TOUE(args.min_range_meter);

                if (!args.clutter_params.EnableSubsurfaceScan) {
                    FLOAT64 visible_range_meter = CMath::GetVisibleDistanceOverSurfaceMeter(start_pos, new_dir, args.clutter_params.MaxSurfacePenetrationMeter);
                    if (visible_range_meter >= 0) {
                        filtered_range_meter = FMath::Min(visible_range_meter, filtered_range_meter);
                    }
                }
                end = start_pos + new_dir * TOUE(filtered_range_meter);

            }
            else {
                // quat pitch etrafinda rotasyonu tamamen sol el kuralına göre, FRotatoreden farklı olarak
                FQuat QuatPitch(right_vec, -elevation * DEGTORAD);
                FQuat Yaw(FVector::UpVector, azimuth * DEGTORAD);

                FVector temp = QuatPitch * (look_dir);
                new_dir = Yaw * temp;

                start_pos = start_loc + new_dir * TOUE(args.min_range_meter);

                if (!args.clutter_params.EnableSubsurfaceScan) {
                    float visible_range_meter = CMath::GetVisibleDistanceOverSurfaceMeter(start_pos, new_dir, args.clutter_params.MaxSurfacePenetrationMeter);
                    if (visible_range_meter >= 0) {
                        filtered_range_meter = FMath::Min(visible_range_meter, filtered_range_meter);
                    }
                }
                end = start_pos + new_dir * TOUE(filtered_range_meter);
            }

            if (args.use_render_target) {
                if (CUtil::GetRandomRange(0.0f, 1.0f) > 0.8) {
                    ret = true;
                    result.Distance = (start_pos - end).Length();
                    result.Location = end;
                    result.ImpactNormal = FVector::ForwardVector;
                }
                else {
                    ret = false;
                }
                
               
            }
            else {
                ret = args.p_actor->GetWorld()->LineTraceSingleByChannel(result, start_pos, end, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());

            }
           if (args.show_radar_beam) {
                DrawDebugLine(args.p_actor->GetWorld(), start_pos, end, FColor::Green, false, 0.2f);
            }

            if (ret) {
                //DrawDebugLine(p_actor->GetWorld(), start_pos,start_pos + new_dir * result.Distance,FColor::Red,false, 0.2f);

                FLOAT32 range_errored_meter = TOW(result.Distance) + error_meter;
                pscan_result->RangeMeter[horizantal_ind][vertical_ind] = range_errored_meter;
                pscan_result->NormalStrength[horizantal_ind][vertical_ind] = new_dir.Dot(-result.ImpactNormal.GetSafeNormal());
                FVector detected_pos_error = result.Location + TOUE(error_meter) * new_dir;
                pscan_result->Point3D[horizantal_ind][vertical_ind] = detected_pos_error;


                pscan_result->AddTrackPoint3DList(detected_pos_error, range_errored_meter);


                if (args.create_scan_line) {
                    p_current_sektor->Add(detected_pos_error, horizantal_ind);
                }
                else {
                    p_current_sektor->Add(detected_pos_error);
                }


                success_count++;
            }
            vertical_ind++;
        }
        horizantal_ind++;
    }

    pscan_result->HorizontalCount = horizantal_ind;
    pscan_result->VeriticalCount = vertical_ind;

    auto trace_elp_sec = CUtil::Tock(trace_start_sec);
    CUtil::DebugLog(FString::Printf(TEXT("scan(ms) %s %f"), *args.p_actor->GetName(), trace_elp_sec * 1000));

    return success_count > 0;

#endif
    return false;
}

bool CUtil::Trace(AActor * p_actor, bool is_world, float min_range_meter, float range_meter, float azimuth_start_deg, float azimuth_end_deg,
                                                                          float elevation_start_deg, float elevation_end_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg,
                                                                          float measurement_error_mean, float measurement_error_std, const SClutterParams& clutter_params,
                                                                          bool show_radar_beam, TArray<AActor*>& ignore_list, bool create_scan_line, 
                                                                          SScanResult* pscan_result)
{
#if true



    FVector start_pos = p_actor->GetActorLocation();
    FVector look_dir;
    FVector right_vec;

    FVector end;
    FHitResult result;
    bool ret = false;
    int horizantal_ind = 0;
    int vertical_ind = 0;

    int success_count = 0;
    
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




    pscan_result->ResetTrackPoint3DList();
  
    int sector_ind = azimuth_start_deg / (360.0 / pscan_result->SectorCount);
    SSectorInfo* p_current_sektor = pscan_result->GetSectorContainer()->GetSector(sector_ind);
    p_current_sektor->Reset();
    pscan_result->CurrentSector = sector_ind;

    FVector start_loc = p_actor->GetActorLocation();
  
    BOOLEAN is_on_surface = false;

    pscan_result->AzimuthRange.X = azimuth_start_deg;
    pscan_result->AzimuthRange.Y = azimuth_end_deg;
    pscan_result->ScanAzimuthStepDeg = azimuth_angle_step_deg;

    pscan_result->ElevationRange.X = elevation_start_deg;
    pscan_result->ElevationRange.Y = elevation_end_deg;
    pscan_result->ScanElevationStepDeg = elevation_angle_step_deg;

    pscan_result->ScanRangeMeter = range_meter;

    for (float azimuth = azimuth_start_deg; azimuth <= azimuth_end_deg; azimuth += azimuth_angle_step_deg) {
        vertical_ind = 0;
        float azimuth_rad = azimuth * DEGTORAD;
        FVector new_dir;
        for (float elevation = elevation_start_deg; elevation <= elevation_end_deg; elevation += elevation_angle_step_deg) {
            FLOAT64 filtered_range_meter = range_meter;
            FLOAT32 error_meter = measurement_error_mean + measurement_error_std * 0.33f * GetRandomRange(-1.0f, 1.0f);
            

            if (is_world) {
                /**
                
                unreal engine pitch ve roll eksenleri sol el kuralının tersine oluyor, normalde -pitch yapmamız gerekirken bu özellikden
                dolayı pozitif pitch veriyoruz.
                */
                FRotator euler_yaw(0, azimuth, 0);
                FRotator euler_pitch(elevation, 0, 0);

                FVector temp_dir = euler_pitch.RotateVector(look_dir);
                new_dir = euler_yaw.RotateVector(temp_dir);

                start_pos = start_loc + new_dir * TOUE(min_range_meter);

                if (!clutter_params.EnableSubsurfaceScan) {
                    FLOAT64 visible_range_meter = CMath::GetVisibleDistanceOverSurfaceMeter(start_pos, new_dir, clutter_params.MaxSurfacePenetrationMeter);
                    if (visible_range_meter >= 0) {
                        filtered_range_meter = FMath::Min(visible_range_meter, filtered_range_meter);
                    }
                }
                end = start_pos + new_dir * TOUE(filtered_range_meter);

            }
            else {
                // quat pitch etrafinda rotasyonu tamamen sol el kuralına göre, FRotatoreden farklı olarak
                FQuat QuatPitch(right_vec, -elevation * DEGTORAD);
                FQuat Yaw(FVector::UpVector, azimuth * DEGTORAD);
              
                FVector temp = QuatPitch * (look_dir);
                new_dir = Yaw * temp;

                start_pos = start_loc + new_dir * TOUE(min_range_meter);

                if (!clutter_params.EnableSubsurfaceScan) {
                    float visible_range_meter = CMath::GetVisibleDistanceOverSurfaceMeter(start_pos, new_dir, clutter_params.MaxSurfacePenetrationMeter);
                    if (visible_range_meter >= 0) {
                        filtered_range_meter = FMath::Min(visible_range_meter, filtered_range_meter);
                    }
                }
                end = start_pos + new_dir * TOUE(filtered_range_meter);
            }


            ret = p_actor->GetWorld()->LineTraceSingleByChannel(result, start_pos, end, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());
            if (show_radar_beam) {
                DrawDebugLine(p_actor->GetWorld(), start_pos, end, FColor::Green, false, 0.2f);
            }
            
            if(ret){
                //DrawDebugLine(p_actor->GetWorld(), start_pos,start_pos + new_dir * result.Distance,FColor::Red,false, 0.2f);
                
                FLOAT32 range_errored_meter = TOW(result.Distance) + error_meter;
                pscan_result->RangeMeter[horizantal_ind][vertical_ind] = range_errored_meter;
                pscan_result->NormalStrength[horizantal_ind][vertical_ind] = new_dir.Dot(-result.ImpactNormal.GetSafeNormal());
                FVector detected_pos_error = result.Location + TOUE(error_meter) * new_dir;
                pscan_result->Point3D[horizantal_ind][vertical_ind] = detected_pos_error;
              

                pscan_result->AddTrackPoint3DList(detected_pos_error, range_errored_meter);

                
                if (create_scan_line) {
                    p_current_sektor->Add(detected_pos_error, horizantal_ind);
                }
                else {
                    p_current_sektor->Add(detected_pos_error);
                }
                
            
                success_count++;
            }
            vertical_ind++;
        }
        horizantal_ind++;
    }

    pscan_result->HorizontalCount = horizantal_ind;
    pscan_result->VeriticalCount = vertical_ind;


    return success_count > 0;

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

void CUtil::ByteCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity)
{
    
    const TCHAR* chars = str.GetCharArray().GetData();
    INT32U size = str.GetCharArray().Num();

    if (size > dest_capacity) {
        size = dest_capacity;
    }

    // Copy the characters from the FString to the BYTE array
    for (INT32U i = 0; i < size; ++i)
    {
        p_dest[i] = (static_cast<INT8U>(chars[i]));
    }
    
}
/**
     *creat ip adr from input string.
     *
     * ip adree is in "192.168.233.47" format
     *
     * @return none
     * @see
     */
void CUtil::IPAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity)
{
    if (p_dest == nullptr || dest_capacity < 4)
    {
        // Handle invalid destination pointer or insufficient capacity.
        // You may throw an exception, log an error, or handle it as needed.
        return;
    }

    TArray<FString> Parts;
    str.ParseIntoArray(Parts, TEXT("."), true);

    if (Parts.Num() != 4)
    {
        // Handle invalid IP address format.
        // You may throw an exception, log an error, or handle it as needed.
        return;
    }

    // Convert each part of the IP address from FString to INT8U
    for (INT32U i = 0; i < 4; ++i)
    {
        FString PartStr = Parts[i].TrimStartAndEnd();
        INT8U PartValue = FCString::Atoi(*PartStr);

        // Check if the destination buffer has enough space
        if (i < dest_capacity)
        {
            p_dest[i] = PartValue;
        }
        else
        {
            // Handle insufficient capacity.
            // You may throw an exception, log an error, or handle it as needed.
            break;
        }
    }
}

/**
     * create mac addr given str.
     *
     * str is in "aa:bb:cc:dd:ee:ff" hex format
     *
     * @return none
     * @see 
     */
void CUtil::MacAddrCopy(const FString& str, INT8U* p_dest, INT32U dest_capacity)
{
    if (p_dest == nullptr || dest_capacity < 6)
    {
        // Handle invalid destination pointer or insufficient capacity.
        // You may throw an exception, log an error, or handle it as needed.
        return;
    }

    TArray<FString> Parts;
    str.ParseIntoArray(Parts, TEXT(":"), true);

    if (Parts.Num() != 6)
    {
        // Handle invalid MAC address format.
        // You may throw an exception, log an error, or handle it as needed.
        return;
    }

    // Convert each part of the MAC address from FString to INT8U
    for (INT32U i = 0; i < 6; ++i)
    {
        FString PartStr = Parts[i].TrimStartAndEnd();
        INT8U PartValue = FCString::Atoi(*PartStr);

        // Check if the destination buffer has enough space
        if (i < dest_capacity)
        {
            p_dest[i] = PartValue;
        }
        else
        {
            // Handle insufficient capacity.
            // You may throw an exception, log an error, or handle it as needed.
            break;
        }
    }
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

AActor* CUtil::SpawnObjectFromBlueprint(FString blueprint_path, UWorld *p_world, AActor *p_owner, FString name, FVector pos, FVector rot_rpy, FVector scale)
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
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* SpawnedActor = World->SpawnActor<AActor>(BlueprintObject->GeneratedClass, pos, FRotator(rot_rpy.Y,rot_rpy.Z, rot_rpy.X), SpawnParams);

    if (!SpawnedActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn the actor from the blueprint."));
        return nullptr;
    }
    SpawnedActor->SetActorScale3D(scale);
    return SpawnedActor;
}

FLOAT64 CUtil::GetTimeSeconds()
{
    return FApp::GetCurrentTime();
}

INT16U CUtil::LittleToBig(INT16U val)
{
    return ((val >> 8) & 0xFF) | ((val << 8) & 0xFF00);
}

INT32U CUtil::LittleToBig(INT32U value)
{
    return ((value << 24) & 0xFF000000) |
        ((value << 8) & 0x00FF0000) |
        ((value >> 8) & 0x0000FF00) |
        ((value >> 24) & 0x000000FF);
}

inline INT32U CUtil::ReverseCopyBytes(INT8U* p_src, INT8U* p_dest, INT32U len)
{
    for (INT32U i = 0; i < len; i++) {
        p_dest[i] = p_src[len - 1 - i];
    }
}

FLOAT32 CUtil::GetRandomRange(FLOAT32 min_inclusive, FLOAT32 max_inclusive)
{
    return FMath::RandRange(min_inclusive, max_inclusive);
}

FVector CUtil::GetActorRPY(AActor* p_actor)
{
    auto rot = p_actor->GetActorRotation();
    return FVector(rot.Roll, -rot.Pitch, -rot.Yaw);
}

AActor* CUtil::GetParentActor(AActor* p_child)
{

    if (!p_child) return nullptr;

    // Get the root component of the actor
    USceneComponent* p_root_comp = p_child->GetRootComponent();
    if (!p_root_comp) return nullptr;

    // Get the parent component (the component to which this actor's root is attached)
    USceneComponent* p_parent_comp = p_root_comp->GetAttachParent();
    if (!p_parent_comp) return nullptr;

    // Finally, get the actor associated with that parent component
    return p_parent_comp->GetOwner();

}

void  CUtil::GetOwnAndParents(AActor* p_child, TArray<AActor*>& ret)
{
    if (p_child != nullptr) {
        ret.Add(p_child);

        AActor* p_curr = p_child;

        while (p_curr != nullptr) {
            p_curr = GetParentActor(p_curr);
            if (p_curr != nullptr) {
                ret.Add(p_curr);
            }
        }
    }
   

}

FLOAT64 CUtil::Tick()
{
    FLOAT64 ref  = FPlatformTime::Seconds();
    return ref;
}


FLOAT64 CUtil::Tock(FLOAT64 ref)
{
    return FPlatformTime::Seconds() - ref;
}

FString CUtil::CharToFString(const char* p_char)
{
    FString str = FString( p_char);
    return str;
}

void CUtil::FStringToAsciiChar(const FString& str, char *p_dest, INT32U dest_len)
{
    const char* p_char_arr = TCHAR_TO_ANSI(*str);
    INT32U len = dest_len <= strlen(p_char_arr) ? dest_len : strlen(p_char_arr);
    memcpy(p_dest, p_char_arr, len);

}
