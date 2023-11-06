// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Utils/CUtil.h"
#include <Lib/Math/CMath.h>
#include <Lib/SystemManager/SystemManagerBase.h>




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

    pscan_result->TotalTimeSec = 0;
    pscan_result->TotalRaycastTimeSec = 0;

   

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
                FRotator euler_pitch(-elevation, 0, 0);

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
                FQuat QuatPitch(right_vec,   elevation * DEGTORAD);
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
                auto raytick = CUtil::Tick();
                ret = args.p_actor->GetWorld()->LineTraceSingleByChannel(result, start_pos, end, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());
                auto ray_elp = CUtil::Tock(raytick);

                pscan_result->TotalRaycastTimeSec += ray_elp;

                
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
    pscan_result->TotalTimeSec = trace_elp_sec;

    CUtil::DebugLog(FString::Printf(TEXT("scan(ms) %s total: %f only trace: %f tracePerc: %f"), *args.p_actor->GetName(), trace_elp_sec * 1000, pscan_result->TotalRaycastTimeSec*1000, pscan_result->TotalRaycastTimeSec/ trace_elp_sec*100));

    return success_count > 0;

#endif
    return false;
}

bool CUtil::DoesExist(FString name, UWorld* p_world)
{
  
    for (TActorIterator<AActor> It(p_world); It; ++It) {
        AActor* CurrentActor = *It;
        // Check if the actor's name matches the desired name
        if (CurrentActor && CurrentActor->GetName() == name) {
            return true;
        }
    }
    // If we get here, no actor with the specified name was found
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
void CUtil::DrawDebugRay(UWorld* p_world, const FVector& Start, const FVector& End, FColor Color, float Duration, float Thickness)
{
    if( p_world)
    {
        DrawDebugLine(
            p_world,
            Start,
            End,
            Color,
            false,  // Persistent (i.e., won't disappear)
            Duration  // Time to disappear
            // Line thickness
        );
     }
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

bool CUtil::ParseColor(const FString& Text, FColor& ret)
{
    FVector ParsedVector;
    TArray<FString> VectorValues;
    Text.ParseIntoArray(VectorValues, TEXT(" "), true);

    if (VectorValues.Num() >= 4)
    {
        ret.R = FCString::Atof(*VectorValues[0]);
        ret.G = FCString::Atof(*VectorValues[1]);
        ret.B = FCString::Atof(*VectorValues[2]);
        ret.A = FCString::Atof(*VectorValues[3]);
        return true;
    }

    return false;
}

FString CUtil::ColorToString(const FColor& color)
{
    return CUtil::IntToString(color.R) + " " + CUtil::IntToString(color.G) + " " + CUtil::IntToString(color.B) + " " + CUtil::IntToString(color.A);
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

void CUtil::DebugLogScreen(FString str, FLOAT32 duration_sec, FColor col)
{


    // Code that should only run in Development builds
    GEngine->AddOnScreenDebugMessage(-1, duration_sec, col, str);
    ASystemManagerBase::GetInstance()->GetUIController()->SetConsoleOutputText(str);
   

    
}

void CUtil::LookAt(AActor* p_actor, FVector& look_dir, FLOAT32 distance )
{
    FVector ActorLocation = p_actor->GetActorLocation();
    FVector TargetLocation = ActorLocation + look_dir * distance;
    FVector DirectionToTarget = (TargetLocation - ActorLocation).GetSafeNormal();
    FRotator NewRotation = DirectionToTarget.Rotation();
    p_actor->SetActorRotation(NewRotation);
}

void  CUtil::CameraLookAt(UCameraComponent* p_looker, AActor* p_target, FLOAT32 distance)
{
    if (!p_looker || !p_target)
    {
        return;
    }

    // Get the direction from the target actor to the camera
    FVector Direction = (p_looker->GetComponentLocation() - p_target->GetActorLocation()).GetSafeNormal();

    // Calculate the new camera position: target position + direction * desired distance
    FVector NewCameraLocation = p_target->GetActorLocation() + Direction * distance;

    // Set the camera's new position
    p_looker->SetWorldLocation(NewCameraLocation);

    // Get the direction from the camera to the target actor (opposite of previous direction)
    FVector LookAtDirection = (p_target->GetActorLocation() - NewCameraLocation).GetSafeNormal();

    // Compute the rotation so the camera faces the target
    FRotator NewRotation = LookAtDirection.Rotation();

    // Set the camera's rotation
    p_looker->SetWorldRotation(NewRotation);

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

FString CUtil::BoolToStringBinary(BOOLEAN val)
{
    return val ? TEXT("1") : TEXT("0");
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
FString CUtil::IntToString(INT32S val)
{

    FString str = FString::Printf(TEXT("%d"), val);
    return str;
}
FString CUtil::IntToString(INT32U val)
{

    FString str = FString::Printf(TEXT("%u"), val);
    return str;
}

FString CUtil::IntToString(INT64S val)
{
    return FString::Printf(TEXT("%lld"), val);
}
FString CUtil::IntToString(INT64U val)
{
    return FString::Printf(TEXT("%llu"), val);
}

 FString CUtil::FloatToString(FLOAT32 val)
{
    FString str = FString::Printf(TEXT("%f"), val);
    return str;
}
 FString CUtil::FloatToString(FLOAT64 val)
 {
     FString str = FString::Printf(TEXT("%lf"), val);
     return str;
 }


FLOAT64 CUtil::StringToFloat64(FString& str)
{
    double ret = FCString::Atod(*str);

    return ret;
}

BOOLEAN CUtil::StringToBool(FString& value) 
{
    return value.ToBool();
}

INT8S CUtil::StringToInt8(FString& value) 
{
    return FCString::Atoi(*value);
}

INT8U CUtil::StringToInt8U(FString& value) 
{
    return FCString::Atoi(*value);  // Beware of negative values!
}

INT32S CUtil::StringToInt32S(FString& value)
{

    return FCString::Atoi(*value);
}

INT32U CUtil::StringToInt32U(FString& value)
{

    return FCString::Atoi(*value);  // Beware of negative values!
}

FLOAT32 CUtil::StringToFloat(FString& value) 
{

    return FCString::Atof(*value);
}

FLOAT64 CUtil::StringToDouble(FString& value) 
{

    return FCString::Atod(*value);
}


FVector2D CUtil::StringToFVector2D(FString& value) 
{

    FVector2D vec;
    if (CUtil::ParseVector2D(value, vec)) {
        return vec;
    }

    return FVector2D::ZeroVector;

}

FVector CUtil::StringToFVector(FString& value) 
{
    FVector vec;
    if (CUtil::ParseVector3D(value, vec)) {
        return vec;
    }

    return FVector::ZeroVector;
}

FString CUtil::VectorToString(FVector& value)
{
    return CUtil::FloatToString(value.X) + " " + CUtil::FloatToString(value.Y) + " " + CUtil::FloatToString(value.Z);
}

FString CUtil::VectorToString(FVector2D& value)
{
    return CUtil::FloatToString(value.X) + " " + CUtil::FloatToString(value.Y);
}

FString CUtil::VectorToString(FVector4& value)
{
    return CUtil::FloatToString(value.X) + " " + CUtil::FloatToString(value.Y) + " " + CUtil::FloatToString(value.Z) + " " + CUtil::FloatToString(value.W);
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

AActor* CUtil::SpawnObjectFromBlueprint(AActor *p_template, FString blueprint_path, UWorld *p_world, AActor *p_owner, FString name, FVector pos, FVector rot_rpy, FVector scale)
{

    try {

    
    // The blueprint name should be the path to the blueprint asset, relative to the Content folder.
    FString BlueprintPath = blueprint_path;

    // Load the blueprint object from the specified path.
    UBlueprint* BlueprintObject = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *BlueprintPath));

    if (!BlueprintObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load blueprint at path: %s"), *BlueprintPath);
        //return nullptr;
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
    SpawnParams.Template = p_template;
    SpawnParams.Owner = p_owner; // Set the owner if required.
    SpawnParams.Instigator = nullptr; // Set the instigator if required.
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* SpawnedActor = nullptr;
    //SpawnedActor = World->SpawnActor<AActor>(BlueprintObject->GeneratedClass, pos, FRotator(rot_rpy.Y,rot_rpy.Z, rot_rpy.X), SpawnParams);


 
    // Yüklemek için kullan
    UClass* BlueprintClass = StaticLoadClass(AActor::StaticClass(), nullptr, *BlueprintPath.Append("_C"));


   // UClass* MyActorClass = StaticLoadClass(AActor::StaticClass(), nullptr, (*blueprint_path));
    if (BlueprintClass)
    {
        SpawnedActor = World->SpawnActor<AActor>(BlueprintClass,pos, FRotator(rot_rpy.Y, rot_rpy.Z, rot_rpy.X), SpawnParams);
    }


    if (!SpawnedActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn the actor from the blueprint."));
        return nullptr;
    }
    SpawnedActor->SetActorScale3D(scale);
    return SpawnedActor;

    }
    catch (...) {
        return nullptr;
    }
}

FLOAT64 CUtil::GetTimeSeconds()
{
    return FApp::GetCurrentTime();
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

void CUtil::SetParent(AActor* p_child, AActor* p_parent)
{
    if (p_child && p_parent)
    {
        USceneComponent* p_parent_root = p_parent->GetRootComponent();
        USceneComponent* p_child_root = p_child->GetRootComponent();

        if (p_parent_root && p_child_root)
        {
            p_child_root->AttachToComponent(p_parent_root, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void CUtil::SetActorActive(AActor* p_actor, bool is_active)
{
    p_actor->SetActorTickEnabled(is_active);
    p_actor->SetActorHiddenInGame(!is_active);
    p_actor->SetActorEnableCollision(is_active);
}

bool CUtil::GetIsActorVisible(AActor* p_actor)
{
    return !p_actor->IsHidden();
}
bool CUtil::IsPossedByPlayerController(UWorld* p_world, APawn* p_pawn)
{

    APlayerController* PlayerController = p_world->GetFirstPlayerController();

    if (PlayerController)
    {
        if (PlayerController->GetPawn() == p_pawn) {
            return true;
        }
    }

    return false;
    
}
FString CUtil::CharToFString(const char* p_char)
{
    FString str = FString( p_char);
    return str;
}

void CUtil::FStringToAsciiChar(const FString& str, char *p_dest, INT32U dest_len)
{
    FTCHARToUTF8 UTF8String(*str);


    INT32S len = (INT32S)dest_len <= UTF8String.Length() ? (INT32S)dest_len : UTF8String.Length();
    memcpy(p_dest, UTF8String.Get(), len);


#if false
    ANSICHAR* p_char_arr = TCHAR_TO_UTF8(*str); //TCHAR_TO_ANSI(*str);
    INT32U len = dest_len <= strlen(p_char_arr) ? dest_len : strlen(p_char_arr);
    memcpy(p_dest, p_char_arr, len);
#endif

}


// Function to recursively gather the hierarchy of Outers for a given UObject
void CUtil::GetActorHierarchy(const AActor* Object, TArray<FString>& Hierarchy)
{
    // Safety check if the passed object is nullptr
    if (!Object)
    {
        return;
    }
    else {
        if (Object->GetName() == "PersistentLevel") {
            return;
        }
    }

    // Add the object's name to the hierarchy array
    Hierarchy.Add(Object->GetName());

    // Recursively process the object's outer
    GetActorHierarchy(CUtil::GetParentActor((AActor*)Object), Hierarchy);
}