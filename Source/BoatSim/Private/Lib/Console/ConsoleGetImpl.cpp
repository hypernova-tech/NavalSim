
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

bool UConsoleBase::ProcessGetCommand(TMap<FString, FString>& options, FString& error_message)
{

    error_message = "";
    FVector2D vec2d;
    bool ret;
    FString sret;
    FString name;
    AActor* p_actor;
    FVector vec;
    bool is_enabled;
    INT32S sint;
    FLOAT64 dbl;
    FVector4 vec4d;
    FColor color;
    SConnectionInfo conn_info;


    ret = CommandManager.HasController();
    if (ret) {
        auto player_controller = ASystemManagerBase::GetInstance()->GetMainPlayerController();
        if (player_controller != nullptr) {
            pSystemAPI->SendConsoleResponse("Possed: " + player_controller->GetName());
        }
        else {
            pSystemAPI->SendConsoleResponse("possed: nullptr (world)");
        }

        return true;
    }

    ret = CommandManager.HasA(CCLICommandManager::UIVisible);
    if (ret) {
        if (pSystemAPI->GetIsUIVisible(is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::UIVisible, is_enabled);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.GetValue(CCLICommandManager::Selected, sret);
    if (ret) {
        auto selected = pSystemAPI->GetSelectedActor();

        if (selected) {
            SendConsoleResponse("", CCLICommandManager::Selected, selected->GetName());
            return true;
        }
        else {
            SendConsoleResponse("", CCLICommandManager::Selected, FString("none"));
            return true;
        }
    }
    else {


    }

    ret = CommandManager.HasA(CCLICommandManager::CamFollow);
    if (ret) {
        if (pSystemAPI->GetCamFollow(sret)) {
            SendConsoleResponse(name, CCLICommandManager::CamFollow, sret);
            return true;
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

    if (name == "*") {
        auto all_actors = ASystemManagerBase::GetInstance()->GetAllActorInWorld();
        for (auto p_act : all_actors)
        {

            pSystemAPI->SendConsoleResponse("Actor: " + p_act->GetName());
        }
        return true;
    }



    if (name == "env") {
        int region = -1;
        ret = CommandManager.HasA(CCLICommandManager::TimeOfDayHour);
        if (ret) {
            if (pSystemAPI->GetTimeOfDayHr(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::TimeOfDayHour, dbl);
                return true;
            }
        }
        else {

        }


        ret = CommandManager.HasA(CCLICommandManager::RainPercent);
        if (ret) {
            if (pSystemAPI->GetRainPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::RainPercent, dbl);
                return true;
            }
        }
        else {

        }


        ret = CommandManager.HasA(CCLICommandManager::CloudPercent);
        if (ret) {
            if (pSystemAPI->GetCloudPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::CloudPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::FogPercent);
        if (ret) {
            if (pSystemAPI->GetFogPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::FogPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::SnowPercent);
        if (ret) {
            if (pSystemAPI->GetSnowPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::SnowPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::DustPercent);
        if (ret) {
            if (pSystemAPI->GetDustPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::DustPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::ThunderPercent);
        if (ret) {
            if (pSystemAPI->GetThunderPercent(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::ThunderPercent, dbl);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::EnvSoundFxEnabled);
        if (ret) {
            if (pSystemAPI->GetEnvSoundEnabled(region, is_enabled)) {
                SendConsoleResponse(name, CCLICommandManager::EnvSoundFxEnabled, is_enabled);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::WindSpeed);
        if (ret) {
            if (pSystemAPI->GetWindSpeedMeterPerSec(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::WindSpeed, is_enabled);
                return true;
            }
        }
        else {

        }

        ret = CommandManager.HasA(CCLICommandManager::WindDirection);
        if (ret) {
            if (pSystemAPI->GetWindDirectionDeg(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::WindDirection, is_enabled);
                return true;
            }
        }
        else {

        }



        ret = CommandManager.HasA(CCLICommandManager::EnvTimeFlowScale);
        if (ret) {
            if (pSystemAPI->GetEnvTimeFlowScale(region, dbl)) {
                SendConsoleResponse(name, CCLICommandManager::EnvTimeFlowScale, dbl);
                return true;
            }
        }
        else {

        }


    }

    p_actor = ASystemManagerBase::GetInstance()->FindActor(name);


    if (p_actor == nullptr) {
        error_message += (("cannot find actor "));
        return false;
    }

    int instance_no = -1;
    ret = CommandManager.GetInstance(instance_no);
    if (ret) {

        SendConsoleResponse("", CCLICommandManager::Instance, instance_no);
        return true;

    }

    ret = CommandManager.HasPosition();
    if (ret) {
        SendConsoleResponse(name, CCLICommandManager::Position, TOW(p_actor->GetActorLocation()));
        return true;

    }

    ret = CommandManager.HasRelposition();
    if (ret) {

        SendConsoleResponse(name, CCLICommandManager::RelPosition, TOW(p_actor->GetActorLocation()));
        return true;

    }

    ret = CommandManager.HasRotation();
    if (ret) {

        vec = CMath::GetActorEulerAnglesRPY(p_actor);
        SendConsoleResponse(name, CCLICommandManager::Rotation, vec);
        return true;
    }
    ret = CommandManager.HasRelrotation();
    if (ret) {
        vec = CMath::GetActorReleativeEulerAnglesRPY(p_actor);
        SendConsoleResponse(name, CCLICommandManager::RelRotation, vec);
        return true;
    }

    ret = CommandManager.HasScale();
    if (ret) {
        vec = p_actor->GetActorScale();;

        SendConsoleResponse(name, CCLICommandManager::Scale, vec);
        return true;
    }

    ret = CommandManager.HasA(CCLICommandManager::ActorId);
    if (ret) {
        if (pSystemAPI->GetActorId(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::ActorId, sint);
            return true;
        }

    }

    ret = CommandManager.HasA(CCLICommandManager::IsBlockingObject);
    if (ret) {
        if (pSystemAPI->GetIsBlockingObject(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::IsBlockingObject,is_enabled);
            return true;
        }
      
    }

    ret = CommandManager.HasA(CCLICommandManager::ThermalMode);
    if (ret) {
        INT32S thermal_mode;
        if (pSystemAPI->GetThermalMode(p_actor, thermal_mode)) {
            SendConsoleResponse(name, CCLICommandManager::ThermalMode, (INT32S)thermal_mode);
            return true;
        }
    }

    ret = CommandManager.HasA(CCLICommandManager::TempratureKelvin);
    if (ret) {
        double temp_kelvin;
        if (pSystemAPI->GetTempratureKelvin(p_actor, temp_kelvin)) {
            SendConsoleResponse(name, CCLICommandManager::TempratureKelvin, temp_kelvin);
            return true;
        }
    }

    ret = CommandManager.HasA(CCLICommandManager::IsHeatSource);
    if (ret) {
       
        if (pSystemAPI->GetIsHeatSource(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::IsHeatSource, is_enabled);
            return true;
        }
    }

    ret = CommandManager.HasA(CCLICommandManager::Func);
    if (ret) {

        SendFunctions(p_actor, "");
        return true;
    }
    else {

    }

    ret = CommandManager.GetValue(CCLICommandManager::Property, sret);
    if (ret) {
        FString prop_val;
        if (GetPropertyValue(p_actor, sret, prop_val)) {
            SendConsoleResponse(name, CCLICommandManager::Property, sret, prop_val);
        }
        else {
            error_message += " property not found";
        }

    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Properties);
    if (ret) {

        SendProperies(p_actor);
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Suppressed);
    if (ret) {
        if (pSystemAPI->GetActorSuppressed(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Suppressed, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::StandaloneModeEnabled);
    if (ret) {
        if (pSystemAPI->SetActorStandaloneModeEnabled(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::StandaloneModeEnabled, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasEnabled();
    if (ret) {
        is_enabled = pSystemAPI->GetActorEnabled(p_actor);
        FString str = is_enabled ? "1" : "0";
        pSystemAPI->SendConsoleResponse("Enabled: " + is_enabled);
        return true;
    }

    ret = CommandManager.HasParent();
    if (ret) {
        auto parent = CUtil::GetParentActor(p_actor);
        if (parent != nullptr) {
            pSystemAPI->SendConsoleResponse("Parent: " + parent->GetName());
        }
        else {
            pSystemAPI->SendConsoleResponse("Parent: nullptr (world)");
        }

        return true;
    }

    ret = CommandManager.HasA(CCLICommandManager::All);
    if (ret) {


        SendConsoleResponse(p_actor);
        return true;

    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SensorSlotIndex);
    if (ret) {

        if (pSystemAPI->GetSlotIndex(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::SensorSlotIndex, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::CamWidthPx);
    if (ret) {

        if (pSystemAPI->GetCamWidthPx(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::CamWidthPx, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::CamHeightPx);
    if (ret) {

        if (pSystemAPI->GetCamHeightPx(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::CamHeightPx, sint);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::HorizontalFov);
    if (ret) {
        if (pSystemAPI->GetHorizontalFov(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::HorizontalFov, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::VericalFov);
    if (ret) {
        if (pSystemAPI->GetVerticalFov(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::VericalFov, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::AnnotateChildren);
    if (ret) {
        if (pSystemAPI->GetAnnotateChildren(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::AnnotateChildren, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::AnnotationId);
    if (ret) {
        if (pSystemAPI->GetActorAnnotationId(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::AnnotationId, sint);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::IPAddr1);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::IPAddr1, conn_info.IpAddr);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::LocalPort1);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::LocalPort1, conn_info.LocalPort);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::RemotePort1);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 0, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::RemotePort1, conn_info.RemotePort);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::IPAddr2);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::IPAddr2, conn_info.IpAddr);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::LocalPort2);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::LocalPort2, conn_info.LocalPort);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::RemotePort2);
    if (ret) {
        if (pSystemAPI->GetConnectionInfo(p_actor, 1, conn_info)) {
            SendConsoleResponse(name, CCLICommandManager::RemotePort2, conn_info.RemotePort);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::AISClassType);
    if (ret) {
        if (pSystemAPI->GetAisClassType(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::AISClassType, sint);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::AISMessagePublishPeriodSec);
    if (ret) {
        if (pSystemAPI->GetAisPublishDurationSec(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::AISMessagePublishPeriodSec, dbl);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::AISShouldPublishAton);
    if (ret) {
        if (pSystemAPI->GetPublishAton(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::AISShouldPublishAton, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Beam);
    if (ret) {
        if (pSystemAPI->GetBeamEnabled(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Beam, is_enabled);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::RangeMin);
    if (ret) {
        if (pSystemAPI->GetRangeMinMeter(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::RangeMin, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RangeMax);
    if (ret) {
        if (pSystemAPI->GetRangeMaxMeter(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::RangeMax, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::HorizontalScanStepAngleDeg);
    if (ret) {
        if (pSystemAPI->GetHorizontalScanStepAngleDeg(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::HorizontalScanStepAngleDeg, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::VerticalScanStepAngleDeg);
    if (ret) {
        if (pSystemAPI->GetVerticalScanStepAngleDeg(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::VerticalScanStepAngleDeg, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SharedMemName);
    if (ret) {
        if (pSystemAPI->GetSharedMemoryName(p_actor, sret)) {
            SendConsoleResponse(name, CCLICommandManager::SharedMemName, sret);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::GStreamerIP);
    if (ret) {
        if (pSystemAPI->GetGStreamerIP(p_actor, sret)) {
            SendConsoleResponse(name, CCLICommandManager::GStreamerIP, sret);
            return true;
        }
    }
    else {

    }
    
    ret = CommandManager.HasA(CCLICommandManager::GStreamerPort);
    if (ret) {
        if (pSystemAPI->GetGStreamerPort(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::GStreamerPort, sint);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::IsNormalDistribution);
    if (ret) {
        if (pSystemAPI->GetIsNormalDistribution(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::IsNormalDistribution, is_enabled);
            return true;
        }
    }
    else {

    }
    ret = CommandManager.HasA(CCLICommandManager::MeasurementErrorMean);
    if (ret) {
        if (pSystemAPI->GetMeasurementErrorMean(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::MeasurementErrorMean, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MeasurementErrorStd);
    if (ret) {
        if (pSystemAPI->GetMeasurementErrorStd(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::MeasurementErrorStd, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableSurfaceDetect);
    if (ret) {
        if (pSystemAPI->GetEnableSurfaceDetect(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::EnableSurfaceDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableSubsurfaceDetect);
    if (ret) {
        if (pSystemAPI->GetEnableSubsurfaceDetect(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::EnableSubsurfaceDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::EnableFoamDetect);
    if (ret) {
        if (pSystemAPI->GetEnableFoamDetect(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::EnableFoamDetect, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SeaSurfaceDetectionProb);
    if (ret) {
        if (pSystemAPI->GetSeaSurfaceDetectionProb(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::EnableFoamDetect, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::MaxSurfacePenetration);
    if (ret) {
        if (pSystemAPI->GetMaxSurfacePenetration(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::MaxSurfacePenetration, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarScanLevel);
    if (ret) {
        if (pSystemAPI->GetRadarScanLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarScanLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarScannerRPM);
    if (ret) {
        if (pSystemAPI->GetRadarScannerRPM(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarScannerRPM, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarGainType);
    if (ret) {
        if (pSystemAPI->GetRadarGainType(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarGainType, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarGainLevel);
    if (ret) {
        if (pSystemAPI->GetRadarGainLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarGainLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterType);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterType(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarSeaClutterType, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterLevel);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarSeaClutterLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarSeaClutterAutoOffset);
    if (ret) {
        if (pSystemAPI->GetRadarSeaClutterAutoOffset(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarSeaClutterAutoOffset, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarRainClutterLevel);
    if (ret) {
        if (pSystemAPI->GetRadarRainClutterLevel(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarRainClutterLevel, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarMaxGuardZoneCount);
    if (ret) {
        if (pSystemAPI->GetRadarMaxGuardZoneCount(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarMaxGuardZoneCount, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::RadarMaxSectorBlankingZoneCount);
    if (ret) {
        if (pSystemAPI->GetRadarMaxSectorBlankingZoneCount(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::RadarMaxSectorBlankingZoneCount, sint);
            return true;
        }
    }
    else {

    }



    ret = CommandManager.HasA(CCLICommandManager::Closed);
    if (ret) {
        if (pSystemAPI->GetPathClosed(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Closed, is_enabled);
            return true;
        }
    }
    else {

    }


    ret = CommandManager.HasA(CCLICommandManager::Speed);
    if (ret) {
        if (pSystemAPI->GetPathSpeed(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::Speed, dbl);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::SegmentCount);
    if (ret) {
        if (pSystemAPI->GetPathSegmentCount(p_actor, sint)) {
            SendConsoleResponse(name, CCLICommandManager::SegmentCount, sint);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::LineColor);
    if (ret) {
        if (pSystemAPI->GetPathLineColor(p_actor, color)) {
            SendConsoleResponse(name, CCLICommandManager::LineColor, color);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Straight);
    if (ret) {
        if (pSystemAPI->GetPathStraight(p_actor, is_enabled)) {
            SendConsoleResponse(name, CCLICommandManager::Straight, is_enabled);
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::TurnRate);
    if (ret) {
        if (pSystemAPI->GetPathTurnRate(p_actor, dbl)) {
            SendConsoleResponse(name, CCLICommandManager::TurnRate, dbl);
            
            return true;
        }
    }
    else {

    }

    ret = CommandManager.HasA(CCLICommandManager::Attach);
    if (ret) {
        if (pSystemAPI->GetActorAttachedToPath(p_actor, sret)) {
            SendConsoleResponse(name, CCLICommandManager::Attach, sret);
            return true;
        }
    }
    else {

    }


    {

        INT32S wp_ind;
        ret = CommandManager.GetValue(CCLICommandManager::Wp, wp_ind);
        if (ret) {
            if (pSystemAPI->GetWaypointPosition(p_actor, wp_ind, vec)) {
                SendConsoleResponse(name, CCLICommandManager::Wp, wp_ind, TOW(vec));
            }

        }
        else {

        }

    }




    return true;
}