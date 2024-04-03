// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/Console/ConsoleBase.h"
#include <Lib/Utils/CUtil.h>
#include <Lib/SystemManager/SystemManagerBase.h>
#include <Lib/Gimbal/GimbalBase.h>
#include <Lib/Sensor/GenericSensor/CameraBase.h>
#include <Lib/Sensor/GenericSensor/RadarBase.h>
#include <Lib/Math/CMath.h>
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

bool UConsoleBase::ProcessSetCommand(TMap<FString, FString>& options, FString& error_message)
{
    error_message = "";
    FVector2D vec2d;
    FColor color;
    bool ret;
    FString sret;
    FString name;
    AActor* p_actor;
    USceneComponent* p_scene_comp;
    FVector vec;
    bool is_enabled;
    bool is_active;
    INT32S sint;
    FLOAT64 dbl;
    SConnectionInfo conn;


    FString controller;
    ret = CommandManager.GetContoroller(controller);

    if (ret) {
        bool ctrl_ret = ASystemManagerBase::GetInstance()->SetMainPlayerController(controller);
        return ctrl_ret;
    }

    ret = CommandManager.GetValue(CCLICommandManager::UIVisible, is_enabled);
    if (ret) {
        if (pSystemAPI->SetIsUIVisible(is_enabled)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::CamSpeed, dbl);
    if (ret) {
        if (pSystemAPI->SetCamSpeed(nullptr, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::CamView, sret);
    if (ret) {
        if (SetCamView(sret)) {
            return true;
        }
    }
    else {

    }
    ret = CommandManager.GetValue(CCLICommandManager::PlatformSpeed, dbl);
    if (ret) {
        if (pSystemAPI->SetSpeedMeterPerSec(nullptr, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::PlatformTurnRate, dbl);
    if (ret) {
        if (pSystemAPI->SetTurnRateDegPerSec(nullptr, dbl)) {
            return true;
        }
    }
    else {

    }



    

    ret = CommandManager.GetValue(CCLICommandManager::Selected, sret);
    if (ret) {
        auto selected = pSystemAPI->FindActor(sret);

        if (selected) {
            pSystemAPI->SetSelectedActor(selected);
            pSystemAPI->SetGizmoTrackedActor(selected);
            return true;
        }
        else {
            error_message += (("cannot find actor "));
            return false;
        }
    }
    else {

    }

    name = CommandManager.GetName();
    if (name != "") {

    }
    else {
        error_message += "empty name value";
        return false;
    }

    int region = 0;
    bool one_success = false;

    if (name == "env") {

        ret = CommandManager.GetValue(CCLICommandManager::TimeOfDayHour, dbl);
        if (ret) {
            if (pSystemAPI->SetTimeOfDayHr(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::EnvTimeFlowScale, dbl);
        if (ret) {
            if (pSystemAPI->SetEnvTimeFlowScale(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::RainPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetRainPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }


        ret = CommandManager.GetValue(CCLICommandManager::CloudPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetCloudPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::FogPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetFogPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::SnowPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetRainPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::DustPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetDustPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::ThunderPercent, dbl);
        if (ret) {
            if (pSystemAPI->SetThunderPercent(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }


        ret = CommandManager.GetValue(CCLICommandManager::EnvSoundFxEnabled, is_enabled);
        if (ret) {
            if (pSystemAPI->SetEnvSoundEnabled(region, is_enabled)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::WindSpeed, dbl);
        if (ret) {
            if (pSystemAPI->SetWindSpeedMeterPerSec(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::WindDirection, dbl);
        if (ret) {
            if (pSystemAPI->SetWindDirectionDeg(region, dbl)) {
                one_success = true;
            }
        }
        else {

        }



        return one_success;
    }
    p_actor = ASystemManagerBase::GetInstance()->FindActor(name);

    if (p_actor == nullptr) {
        p_scene_comp = ASystemManagerBase::GetInstance()->FindComponent(name);
        if (p_scene_comp != nullptr) {
            p_actor = p_scene_comp->GetOwner();
        }
    }
    if (p_actor == nullptr) {
        error_message += (("cannot find actor "));
        return false;
    }


    { // terrain command

        ret = CommandManager.GetValue(CCLICommandManager::TerrHMap, sret);
        if (ret) {
            if (pSystemAPI->SetTerrainHeightMap(p_actor, sret)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrDMap, sret);
        if (ret) {
            if (pSystemAPI->SetTerrainDepthMap(p_actor, sret)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrBaseTexture, sret);
        if (ret) {
            if (pSystemAPI->SetTerraiBaseTexture(p_actor, sret)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrHMapMinLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainHeightMapMinLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }

        ret = CommandManager.GetValue(CCLICommandManager::TerrHMapMaxLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainHeightMapMaxLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }


        ret = CommandManager.GetValue(CCLICommandManager::TerrDMapMinLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainDepthMapMinLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }



        ret = CommandManager.GetValue(CCLICommandManager::TerrDMapMaxLvlMt, dbl);
        if (ret) {
            if (pSystemAPI->SetTerrainDepthMapMaxLevelMeter(p_actor, dbl)) {
                one_success = true;
            }
        }
        else {

        }


        ret = CommandManager.GetValue(CCLICommandManager::TerrTopLeftLLH, vec);
        if (ret) {
            if (pSystemAPI->SetTerrainTopLeftCornerCoordLLH(p_actor, vec)) {
                one_success = true;
            }
        }
        else {

        }
        ret = CommandManager.GetValue(CCLICommandManager::TerrBottomRightLLH, vec);
        if (ret) {
            if (pSystemAPI->SetTerrainBottomRightCornerCoordLLH(p_actor, vec)) {
                one_success = true;
            }
        }
        else {

        }

    }






    ret = CommandManager.GetValue(CCLICommandManager::StandaloneModeEnabled, is_enabled);
    if (ret) {
        if (pSystemAPI->SetActorStandaloneModeEnabled(p_actor, is_enabled)) {
            one_success = true;
        }

    }


    ret = CommandManager.GetValue(CCLICommandManager::Suppressed, is_enabled);
    if (ret) {
        if (pSystemAPI->SetActorSuppressed(p_actor, is_enabled)) {
            one_success = true;
        }

    }

    ret = CommandManager.GetActive(is_active);
    if (ret) {
        CUtil::SetActorActive(p_actor, is_active);
        one_success = true;

    }


    ret = CommandManager.GetEnabled(is_enabled);
    if (ret) {
        pSystemAPI->SetActorEnabled(p_actor, is_enabled);
        one_success = true;

    }

    int  instance_no = -1;
    ret = CommandManager.GetInstance(instance_no);
    if (ret) {
        pSystemAPI->SetActorInstanceNo(p_actor, instance_no);
        one_success = true;

    }

    ret = CommandManager.GetPosition(vec);
    if (ret) {
        pSystemAPI->SetActorPosition(p_actor, TOUE(vec));
        //p_actor->SetActorLocation(vec * WORLDTOUNREALUNIT);
        one_success = true;
    }

    ret = CommandManager.GetRelPosition(vec);
    if (ret) {
        pSystemAPI->SetActorRelPosition(p_actor, TOUE(vec));
        //p_actor->SetActorRelativeLocation(vec * WORLDTOUNREALUNIT);
        one_success = true;
    }

    ret = CommandManager.GetRotation(vec);
    if (ret) {
        pSystemAPI->SetActorRot(p_actor, vec);
        //CMath::SetActorRotation(p_actor, vec);
        one_success = true;
    }
    ret = CommandManager.GetRelRotation(vec);
    if (ret) {
        pSystemAPI->SetActorRelRot(p_actor, vec);
        //CMath::SetActorRelativeRotation(p_actor, vec);
        one_success = true;
    }

    ret = CommandManager.GetScale(vec);
    if (ret) {
        p_actor->SetActorScale3D(vec);
        one_success = true;
    }
    FString parent;
    ret = CommandManager.GetParent(parent);
    if (ret) {
        FString full_path = "";
        CommandManager.GetValue(CCLICommandManager::FullPath, full_path);

        if (p_actor != nullptr) {
            if (SetParent(p_actor, parent, full_path)) {
                SendActorBases();
            }
        }

    }

    ret = CommandManager.GetValue(CCLICommandManager::SensorSlotIndex, sint);
    if (ret) {
        if (pSystemAPI->SetSlotIndex(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::CamWidthPx, sint);
    if (ret) {
        if (pSystemAPI->SetCamWidthPx(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::CamHeightPx, sint);
    if (ret) {
        if (pSystemAPI->SetCamHeightPx(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Focused, is_enabled);
    if (ret) {
        if (pSystemAPI->FocusCamera(p_actor)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::ThermalMode, sint);
    if (ret) {
        if (pSystemAPI->SetThermalMode(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::TempratureKelvin, dbl);
    if (ret) {
        if (pSystemAPI->SetTempratureKelvin(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::IsHeatSource, is_enabled);
    if (ret) {
        if (pSystemAPI->SetIsHeatSource(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Property, sret);
    if (ret) {
        FString prop_value;
        ret = CommandManager.GetValue(CCLICommandManager::PropertyValue, prop_value);
        if (ret) {
            if (SetPropertyValue(p_actor, sret, prop_value)) {
                one_success = true;
            }
        }
        else {
            error_message += " value not defined";
            return false;
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::AnnotateChildren, is_enabled);
    if (ret) {

        if (pSystemAPI->SetAnnotateChildren(p_actor, is_enabled)) {

            one_success = true;
        }
    }

   

    ret = CommandManager.GetValue(CCLICommandManager::AnnotationId, sint);
    if (ret) {
       
        if (pSystemAPI->SetActorAnnotationId(p_actor, sint)) {
            
             one_success = true;
             
        }

    }




    ret = CommandManager.GetValue(CCLICommandManager::IPAddr1, sret);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn)) {
            conn.IpAddr = sret;
            if (pSystemAPI->SetConnectionInfo(p_actor, 0, conn)) {
                one_success = true;
            }
        }

    }


    ret = CommandManager.GetValue(CCLICommandManager::LocalPort1, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn)) {
            conn.LocalPort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 0, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RemotePort1, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn)) {
            conn.RemotePort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 0, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }



    ret = CommandManager.GetValue(CCLICommandManager::IPAddr2, sret);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn)) {
            conn.IpAddr = sret;
            if (pSystemAPI->SetConnectionInfo(p_actor, 1, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::LocalPort2, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn)) {
            conn.LocalPort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 1, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RemotePort2, sint);
    if (ret) {

        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn)) {
            conn.RemotePort = sint;
            if (pSystemAPI->SetConnectionInfo(p_actor, 1, conn)) {
                one_success = true;
            }
        }

    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::HorizontalFov, dbl);
    if (ret) {
        if (pSystemAPI->SetHorizontalFov(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::VericalFov, dbl);
    if (ret) {
        if (pSystemAPI->SetVerticalFov(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::HorizontalScanStepAngleDeg, dbl);
    if (ret) {
        if (pSystemAPI->SetHorizontalScanStepAngleDeg(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::VerticalScanStepAngleDeg, dbl);
    if (ret) {
        if (pSystemAPI->SetVerticalScanStepAngleDeg(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }
    ret = CommandManager.GetValue(CCLICommandManager::Beam, is_enabled);
    if (ret) {
        if (pSystemAPI->SetBeamEnabled(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }
    ret = CommandManager.GetValue(CCLICommandManager::RangeMin, dbl);
    if (ret) {
        if (pSystemAPI->SetRangeMinMeter(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RangeMax, dbl);
    if (ret) {
        if (pSystemAPI->SetRangeMaxMeter(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MeasurementErrorMean, dbl);
    if (ret) {
        if (pSystemAPI->SetMeasurementErrorMean(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MeasurementErrorStd, dbl);
    if (ret) {
        if (pSystemAPI->SetMeasurementErrorStd(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableSurfaceDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableSurfaceDetect(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableSubsurfaceDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableSubsurfaceDetect(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::EnableFoamDetect, is_enabled);
    if (ret) {
        if (pSystemAPI->SetEnableFoamDetect(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::SeaSurfaceDetectionProb, dbl);
    if (ret) {
        if (pSystemAPI->SetSeaSurfaceDetectionProb(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MaxSurfacePenetration, dbl);
    if (ret) {
        if (pSystemAPI->SetMaxSurfacePenetration(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarScanLevel, dbl);
    if (ret) {
        if (pSystemAPI->SetRadarScanLevel(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarScannerRPM, dbl);
    if (ret) {
        if (pSystemAPI->SetRadarScannerRPM(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarGainType, sint);
    if (ret) {
        if (pSystemAPI->SetRadarGainType(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarGainLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarGainLevel(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterType, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterType(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterLevel(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarSeaClutterAutoOffset, sint);
    if (ret) {
        if (pSystemAPI->SetRadarSeaClutterAutoOffset(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarRainClutterLevel, sint);
    if (ret) {
        if (pSystemAPI->SetRadarRainClutterLevel(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarMaxGuardZoneCount, sint);
    if (ret) {
        if (pSystemAPI->SetRadarMaxGuardZoneCount(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::RadarMaxSectorBlankingZoneCount, sint);
    if (ret) {
        if (pSystemAPI->SetRadarMaxSectorBlankingZoneCount(p_actor, sint)) {
            one_success = true;;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MapOriginLat, dbl);
    if (ret) {
        FVector origin;
        pSystemAPI->GetMapOrigin(p_actor, origin);
        origin.X = dbl;
        if (pSystemAPI->SetMapOrigin(p_actor, origin)) {
            one_success = true;;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MapOriginLon, dbl);
    if (ret) {
        FVector origin;
        pSystemAPI->GetMapOrigin(p_actor, origin);
        origin.Y = dbl;
        if (pSystemAPI->SetMapOrigin(p_actor, origin)) {
            one_success = true;;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::MapOriginHeight, dbl);
    if (ret) {
        FVector origin;
        pSystemAPI->GetMapOrigin(p_actor, origin);
        origin.Z = dbl;
        if (pSystemAPI->SetMapOrigin(p_actor, origin)) {
            one_success = true;;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Closed, is_enabled);
    if (ret) {
        if (pSystemAPI->SetPathClosed(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Bake, sint);
    if (ret) {
        if (pSystemAPI->Bake(p_actor)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Speed, dbl);
    if (ret) {
        if (pSystemAPI->SetPathSpeed(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Attach, sret);
    if (ret) {
        if (pSystemAPI->AttachToPath(p_actor, sret)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Detach, sret);
    if (ret) {
        if (pSystemAPI->DetachFromPath(p_actor, sret)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::LineColor, color);
    if (ret) {
        if (pSystemAPI->SetPathLineColor(p_actor, color)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::SegmentCount, sint);
    if (ret) {
        if (pSystemAPI->SetPathSegmentCount(p_actor, sint)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Straight, is_enabled);
    if (ret) {
        if (pSystemAPI->SetPathStraight(p_actor, is_enabled)) {
            one_success = true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::TurnRate, dbl);
    if (ret) {
        if (pSystemAPI->SetPathTurnRate(p_actor, dbl)) {
            one_success = true;
        }
    }
    else {

    }

    {

        INT32S wp_ind;
        ret = CommandManager.GetValue(CCLICommandManager::Wp, wp_ind);
        if (ret) {
            FVector wp_pos;
            ret = CommandManager.GetValue(CCLICommandManager::WpPos, wp_pos);
            if (ret) {

                pSystemAPI->AddOrModifyWaypointToPath(p_actor, wp_ind, wp_pos);
            }


        }
        else {

        }

    }

    ret = CommandManager.GetValue(CCLICommandManager::AISClassType, sint);
    if (ret) {
        if (pSystemAPI->SetAisClassType(p_actor, sint)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::AISMessagePublishPeriodSec, dbl);
    if (ret) {
        if (pSystemAPI->SetAisPublishDurationSec(p_actor, dbl)) {
            return true;
        }
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::AISShouldPublishAton, is_enabled);
    if (ret) {
        if (pSystemAPI->SetPublishAton(p_actor, is_enabled)) {
            return true;
        }
    }
    else {

    }


    return one_success;
}