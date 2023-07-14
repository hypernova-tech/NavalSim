// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Utils/CUtil.h"

CUtil::CUtil()
{
}

CUtil::~CUtil()
{
}

bool CUtil::Trace(AActor * p_actor, float range_meter, float beam_width_azimuth_deg, float beam_width_elavation_deg, float azimuth_angle_step_deg, float elevation_angle_step_deg)
{

    return false;

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

    for (float azimuth = azimuth_start; azimuth <= azimuth_end; azimuth += azimuth_angle_step_deg) {

        for (float elevation = elevation_start; elevation <= elevation_end; elevation += elevation_angle_step_deg) {


            FRotator euler_rot(elevation, azimuth, 0);

            FQuat qua = euler_rot.Quaternion();

            FVector new_dir = qua.RotateVector(look_dir);
            end = start_pos + new_dir * range_meter * 100;

            ret = p_actor->GetWorld()->LineTraceSingleByChannel(result, start_pos, end, ECollisionChannel::ECC_Visibility, query_params, FCollisionResponseParams());
            //if(ret){
                //DrawDebugLine(p_actor->GetWorld(), start_pos,start_pos + new_dir * 2000 ,FColor::Red,false, 0.2f);
            //}
        }
    }


    return ret;
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