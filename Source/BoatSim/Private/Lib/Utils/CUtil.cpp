// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Utils/CUtil.h"

CUtil::CUtil()
{
}

CUtil::~CUtil()
{
}

bool CUtil::Trace(AActor * p_actor, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg, SScanResult* pscan_result)
{



    FVector start_pos = p_actor->GetActorLocation();
    FVector look_dir = p_actor->GetActorForwardVector();

    FCollisionQueryParams query_params;
    query_params.AddIgnoredActor(p_actor);
    query_params.bTraceComplex = false;

    float azimuth_start = -beam_width_azimuth_deg / 2.0f;
    float azimuth_end = beam_width_azimuth_deg / 2.0f;

    float elevation_start = 0;
    float elevation_end = beam_width_elavation_deg;

    FVector end;
    FHitResult result;
    bool ret = false;
    int horizantal_ind = 0;
    int vertical_ind = 0;

    int success_count = 0;

    pscan_result->Point3DList.Reset();

    for (float azimuth = azimuth_start; azimuth <= azimuth_end; azimuth += azimuth_angle_step_deg) {
        vertical_ind = 0;
        for (float elevation = elevation_start; elevation <= elevation_end; elevation += elevation_angle_step_deg) {


            FRotator euler_rot(elevation, azimuth, 0);

            FQuat qua = euler_rot.Quaternion();

            FVector new_dir = qua.RotateVector(look_dir);
            end = start_pos + new_dir * range_meter * 100;

            ret = p_actor->GetWorld()->LineTraceSingleByChannel(result, start_pos, end, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());
            if(ret){
                //DrawDebugLine(p_actor->GetWorld(), start_pos,start_pos + new_dir * 2000 ,FColor::Red,false, 0.2f);
                pscan_result->Range[horizantal_ind][vertical_ind] = result.Distance;
                FVector detection = start_pos + new_dir * result.Distance;
                pscan_result->Point3D[horizantal_ind][vertical_ind] = detection;
                pscan_result->Point3DList.Add(detection);
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