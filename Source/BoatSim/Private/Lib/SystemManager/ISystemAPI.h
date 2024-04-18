// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>
#include <Lib/Console/ConsoleBase.h>
#include <Lib/Platform/PlatformBase.h>


/**
 * 
 */

enum EActorQueryArgs
{
	ActorBasesExceptSensorsAndPaths,
	ActorBasesOnlyPaths,
	ActorBases,
	Platforms,
	OnlyAgents,
	ActorBasesExceptSensorsAndPathsAndGimbals,
	ActorBasesExceptSensorsAndPathsAndGimbalsActorAsPlatform,
	Gimbals,
	AIS,
	AISEnabledActors,
	ActorAsPlatform,
};




class UConsoleBase;
class ISystemAPI
{
public:
	ISystemAPI();
	~ISystemAPI();
	virtual ISystemAPI* GetSystemAPI() = 0;

	virtual bool Save(FString fname) = 0;
	virtual bool Load(FString fname) = 0;

	virtual  UConsoleBase* GetConsole() = 0;
	virtual  APlayerCameraManager* GetCameraManager() = 0;

	virtual TArray<AActor*>& GetSensorGlobalIgnoreList() = 0;
	virtual AActor* GetFloor() = 0;
	virtual TArray<AActor*>& GetMoveableActorList() = 0;
	/// <summary>
	/// sensor access
	/// </summary>
	/// <param name="model_name"></param>
	/// <param name="boat_name"></param>
	/// <param name="world_pos"></param>
	/// <param name="world_rot"></param>
	/// <param name="scale"></param>
	/// <returns></returns>
	virtual AActor* CreateActor(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale, FLOAT64 life_time_sec = 0)=0;
	virtual void RegisterActor(AActor* p_actor) = 0;
	virtual bool RemoveActor(AActor* p_actor) = 0;
	virtual TArray<AActor*> GetRegisteredActors() = 0;
	virtual TArray<AActor*> GetAllActorInWorld() = 0;

	virtual void QueryActors(EActorQueryArgs args, TArray<AActor*> &actors) = 0;
	virtual UObject* FindObject(FString obj_name) = 0;
	virtual AActor* FindActor(FString actor_name) = 0;
	virtual bool DestroyActor(FString name) = 0;
	virtual AActorBase* ToActorBase(AActor* p_actor) = 0;
	virtual ASensorBase* ToSensorBase(AActor* p_actor) = 0;
	virtual bool IsBakable(AActor* p_actor) = 0;
	
	virtual bool GetBeamEnabled(AActor* p_actor, bool& val) = 0;
	virtual bool SetBeamEnabled(AActor* p_actor, bool val) = 0;

	virtual bool GetActorStandaloneModeEnabled(AActor* p_actor, bool& val) = 0;
	virtual bool SetActorStandaloneModeEnabled(AActor* p_actor, bool val) = 0;
	

	virtual bool SetActorSuppressed(AActor* p_actor, bool val) = 0;
	virtual bool GetActorSuppressed(AActor* p_actor, bool& val) = 0;

	virtual bool SetActorEnabled(AActor* p_actor, bool val) = 0;
	virtual bool GetActorEnabled(AActor* p_actor) = 0;
	virtual void EnableAllActors() = 0;
	virtual void DisableAllActors() = 0;
	virtual void SetSelectedActor(AActor *p_actor) = 0;
	virtual AActor* GetSelectedActor() = 0 ;
	virtual void SetGizmoTrackedActor(AActor* p_actor) = 0;
	virtual bool IsGizmoActorMoving() = 0;

	virtual INT64U GetTimeStamp() = 0;
	virtual AActor* GetVisibleActorAt(const TArray<AActor*>& ignore_list, FVector from, FVector to, FLOAT64 tolerance_meter) = 0;

	virtual void SetCanLoadConfig(bool val) = 0;
	virtual bool GetCanLoadConfig() = 0;

	virtual INT32S GetInstanceNo() = 0;
	virtual void ForceExit() = 0;

	virtual bool SetActorPosition(AActor* p_actor, FVector pos) = 0;
	virtual bool SetActorRelPosition(AActor* p_actor, FVector pos) = 0;

	virtual bool SetActorRot(AActor* p_actor, FVector pos) = 0;
	virtual bool SetActorRelRot(AActor* p_actor, FVector pos) = 0;

	virtual  bool  SetMainPlayerController(FString name) = 0;
	virtual AActor* GetMainPlayerController() = 0;

	
	virtual ACBoatBase* GetPlatform() = 0;
	
	virtual void SetPlatform(APlatformBase* p_platform) = 0;


	virtual void StartSimulation() = 0;
	virtual void ResumeSimulation() = 0;
	virtual void PauseSimulation() = 0;


	virtual void SetConsoleConnection(void *p_connection) = 0;
	virtual void SendConsoleResponse(const FString& str) = 0;
	virtual bool SetActorInstanceNo(AActor* p_actor, INT32S instance_no) = 0;
	virtual int GetActorInstanceNo(AActor* p_actor) = 0;


	virtual void RegisterConnection(UConnectionBase* p_connection) = 0;
	virtual INT32U ConvertToInstancedLocalPort(INT32 port_no) = 0;


	/// <summary>
	/// sensor API
	/// </summary>
	/// <param name="p_actor"></param>
	/// <param name="ret"></param>
	/// <returns></returns>
	virtual bool GetSensorScanStepAngleDeg(AActor* p_actor, FVector2D& ret) = 0;
	virtual bool SetSensorScanStepAngleDeg(AActor* p_actor, FVector2D ang) = 0;
	virtual ESensorType StringToSensor(const FString& str) = 0;
	virtual FString SensorToString(ESensorType sensor_type) = 0;
	virtual TArray<ESensorType> GetAllSensorTypes() = 0;
	virtual TArray<ASensorBase*> GetAllSensors() = 0;
	virtual TArray<ASensorBase*> GetSensorsOfType(ESensorType sensor_type) = 0;



    /// <summary>
    /// Sensor modification
    /// </summary>
    /// <param name="p_actor"></param>
    /// <param name="slot_index"></param>
    /// <returns></returns>
	/// 
	virtual bool SetActorId(AActor* p_actor, INT32S slot_index) = 0;
	virtual bool GetActorId(AActor* p_actor, INT32S& slot_index) = 0;
	virtual bool SetSlotIndex(AActor* p_actor, INT32S slot_index) = 0;
	virtual bool GetSlotIndex(AActor* p_actor, INT32S &slot_index) = 0;
    virtual bool SetHorizontalFov(AActor* p_actor, FLOAT64 angle_deg) = 0;
	virtual bool GetHorizontalFov(AActor* p_actor, FLOAT64 &angle_deg) = 0;
	virtual bool SetRangeMinMeter(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool GetRangeMinMeter(AActor* p_actor, FLOAT64& val) = 0;
	virtual bool SetRangeMaxMeter(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool GetRangeMaxMeter(AActor* p_actor, FLOAT64& val) = 0;
    virtual bool SetVerticalFov(AActor* p_actor, FLOAT64 angle_deg) = 0;
	virtual bool GetVerticalFov(AActor* p_actor, FLOAT64& angle_deg) = 0;
    virtual bool SetHorizontalScanStepAngleDeg(AActor* p_actor, FLOAT64 angle_deg) = 0;
	virtual bool GetHorizontalScanStepAngleDeg(AActor* p_actor, FLOAT64& angle_deg) = 0;
    virtual bool SetVerticalScanStepAngleDeg(AActor* p_actor, FLOAT64 angle_deg) = 0;
	virtual bool GetVerticalScanStepAngleDeg(AActor* p_actor, FLOAT64& angle_deg) = 0;



	virtual bool GetIsNormalDistribution(AActor* p_actor, bool&val) = 0;
	virtual	bool SetIsNormalDistribution(AActor* p_actor, bool val) = 0;

    virtual bool SetMeasurementErrorMean(AActor* p_actor, FLOAT64 angle_deg) = 0;
    virtual bool SetMeasurementErrorStd(AActor* p_actor, FLOAT64 angle_deg) = 0;
	virtual bool GetMeasurementErrorMean(AActor* p_actor, FLOAT64& angle_deg) = 0;
	virtual bool GetMeasurementErrorStd(AActor* p_actor, FLOAT64& angle_deg) = 0;
    virtual bool SetEnableSurfaceDetect(AActor* p_actor, bool val) = 0;
    virtual bool SetEnableSubsurfaceDetect(AActor* p_actor, bool val) = 0;
    virtual bool SetEnableFoamDetect(AActor* p_actor, bool angle_deg) = 0;
    virtual bool SetSeaSurfaceDetectionProb(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool SetCamWidthPx(AActor* p_actor, INT32S val) = 0;
	virtual bool GetCamWidthPx(AActor* p_actor, INT32S &val) = 0;

	virtual bool SetCamHeightPx(AActor* p_actor, INT32S val) = 0;
	virtual bool GetCamHeightPx(AActor* p_actor, INT32S& val) = 0;

	virtual bool SetIsBlockingObject(AActor* p_actor, bool val) = 0;
	virtual bool GetIsBlockingObject(AActor* p_actor, bool& val) = 0;
	

	virtual bool GetEnableSurfaceDetect(AActor* p_actor, bool& val) = 0;
	virtual bool GetEnableSubsurfaceDetect(AActor* p_actor, bool& val) = 0;
	virtual bool GetEnableFoamDetect(AActor* p_actor, bool& angle_deg) = 0;
	virtual bool GetSeaSurfaceDetectionProb(AActor* p_actor, FLOAT64& val) = 0;

    virtual bool SetMaxSurfacePenetration(AActor* p_actor, FLOAT64 val) = 0;
    virtual bool SetRadarScanLevel(AActor* p_actor, INT32S val) = 0;
    virtual bool SetRadarScannerRPM(AActor* p_actor, INT32S val) = 0;
    virtual bool SetRadarGainType(AActor* p_actor, INT32S val) = 0;
    virtual bool SetRadarGainLevel(AActor* p_actor, INT32S val) = 0;
    virtual bool SetRadarSeaClutterType(AActor* p_actor, INT32S val) = 0;
    virtual bool SetRadarSeaClutterLevel(AActor* p_actor, INT32S val) = 0;
    virtual bool SetRadarSeaClutterAutoOffset(AActor* p_actor, INT32S val) = 0;
	virtual bool SetRadarRainClutterLevel(AActor* p_actor, INT32S val) = 0;
	virtual bool SetRadarMaxGuardZoneCount(AActor* p_actor, INT32S val) = 0;
	virtual bool SetRadarMaxSectorBlankingZoneCount(AActor* p_actor, INT32S val) = 0;


	virtual bool GetMaxSurfacePenetration(AActor* p_actor, FLOAT64& val) = 0;
	virtual bool GetRadarScanLevel(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarScannerRPM(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarGainType(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarGainLevel(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarSeaClutterType(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarSeaClutterLevel(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarSeaClutterAutoOffset(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarRainClutterLevel(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarMaxGuardZoneCount(AActor* p_actor, INT32S& val) = 0;
	virtual bool GetRadarMaxSectorBlankingZoneCount(AActor* p_actor, INT32S& val) = 0;


	virtual bool GetSpeedMeterPerSec(AActor* p_actor, FLOAT64& val) = 0;
	virtual bool SetSpeedMeterPerSec(AActor* p_actor, FLOAT64 val) = 0;

	virtual bool GetTurnRateDegPerSec(AActor* p_actor, FLOAT64& val) = 0;
	virtual bool SetTurnRateDegPerSec(AActor* p_actor, FLOAT64 val) = 0;

	virtual bool AddOrModifyWaypointToPath(AActor* p_actor, INT32S wp_ind, FVector position) = 0;
	virtual bool Bake(AActor* p_actor) = 0;
	virtual bool SetPathClosed(AActor* p_actor, bool is_closed) = 0;
	virtual bool SetPathSpeed(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool AttachToPath(AActor* p_path, FString actor_name) = 0;
	virtual bool DetachFromPath(AActor* p_path, FString actor_name) = 0;

	virtual bool GetWaypointPosition(AActor* p_actor, INT32S wp_ind, FVector &position) = 0;
	virtual bool GetPathClosed(AActor* p_actor, bool &is_closed) = 0;
	virtual bool GetPathSpeed(AActor* p_actor, FLOAT64 &val) = 0;
	virtual bool GetActorAttachedToPath(AActor* p_path, FString &actor_name) = 0;


	virtual bool GetPathSegmentCount(AActor* p_actor, INT32S &val) = 0;
	virtual bool SetPathSegmentCount(AActor* p_actor, INT32S& val) = 0;
 
	virtual bool GetPathLineColor(AActor* p_actor, FColor& val) = 0;
	virtual bool SetPathLineColor(AActor* p_actor, FColor& val) = 0;

	virtual bool GetPathStraight(AActor* p_actor, bool& val) = 0;
	virtual bool SetPathStraight(AActor* p_actor, bool& val) = 0;

	virtual bool GetPathTurnRate(AActor* p_actor, FLOAT64& val) = 0;
	virtual bool SetPathTurnRate(AActor* p_actor, FLOAT64 val) = 0;


	virtual bool FocusCamera(AActor* p_actor) = 0;

	virtual bool GetIsUIVisible(bool& val) = 0;
	virtual bool SetIsUIVisible(bool val) = 0;

	virtual bool GetConnectionInfo(AActor* p_actor, INT32S ind, SConnectionInfo& conn) = 0;
	virtual bool SetConnectionInfo(AActor* p_actor, INT32S ind, SConnectionInfo conn) = 0;


	//environment
	virtual bool SetTimeOfDayHr(INT32S region, FLOAT64 val) = 0;
	virtual bool GetTimeOfDayHr(INT32S region, FLOAT64& val) = 0;

	virtual bool SetTimeOfDuskHr(INT32S region, FLOAT64 val) = 0;
	virtual bool GetTimeOfDuskHr(INT32S region, FLOAT64& val) = 0;

	virtual bool SetTimeOfSunSetHr(INT32S region, FLOAT64 val) = 0;
	virtual bool GetTimeOfSunSetHr(INT32S region, FLOAT64& val) = 0;

	virtual bool SetEnvSoundEnabled(INT32S region, bool val) = 0;
	virtual bool GetEnvSoundEnabled(INT32S region, bool& val) = 0;

	virtual bool SetCloudPercent(INT32S region, FLOAT64 val) = 0;
	virtual bool GetCloudPercent(INT32S region, FLOAT64 &val) = 0;

	virtual bool SetRainPercent(INT32S region, FLOAT64 val) = 0;
	virtual bool GetRainPercent(INT32S region, FLOAT64& val) = 0;

	virtual bool SetSnowPercent(INT32S region, FLOAT64 val) = 0;
	virtual bool GetSnowPercent(INT32S region, FLOAT64& val) = 0;

	virtual bool SetDustPercent(INT32S region, FLOAT64 val) = 0;
	virtual bool GetDustPercent(INT32S region, FLOAT64& val) = 0;

	virtual bool SetFogPercent(INT32S region, FLOAT64 val) = 0;
	virtual bool GetFogPercent(INT32S region, FLOAT64& val) = 0;


	virtual bool SetThunderPercent(INT32S region, FLOAT64 val) = 0;
	virtual bool GetThunderPercent(INT32S region, FLOAT64& val) = 0;

	virtual bool SetWindSpeedMeterPerSec(INT32S region, FLOAT64 val) = 0;
	virtual bool GetWindSpeedMeterPerSec(INT32S region, FLOAT64& val) = 0;

	virtual bool SetWindDirectionDeg(INT32S region, FLOAT64 val) = 0;
	virtual bool GetWindDirectionDeg(INT32S region, FLOAT64& val) = 0;

	virtual bool SetEnvTimeFlowScale(INT32S region, FLOAT64 val) = 0;
	virtual bool GetEnvTimeFlowScale(INT32S region, FLOAT64& val) = 0;

	

	/// <summary>
	/// terrain managment
	/// </summary>
	/// <param name="p_actor"></param>
	/// <param name="path"></param>
	/// <returns></returns>
	virtual bool SetTerrainHeightMap(AActor* p_actor, FString  path) = 0;
	virtual bool GetTerrainHeightMap(AActor* p_actor, FString  &path) = 0;

	virtual bool SetTerrainDepthMap(AActor* p_actor, FString path) = 0;
	virtual bool GetTerrainDepthMap(AActor* p_actor, FString &path) = 0;

	virtual bool SetTerraiBaseTexture(AActor* p_actor, FString path) = 0;
	virtual bool GetTerraiBaseTexture(AActor* p_actor, FString &path) = 0;

	virtual bool SetTerrainHeightMapMinLevelMeter(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool GetTerrainHeightMapMinLevelMeter(AActor* p_actor, FLOAT64 &val) = 0;

	virtual bool SetTerrainHeightMapMaxLevelMeter(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool GetTerrainHeightMapMaxLevelMeter(AActor* p_actor, FLOAT64 &val) = 0;

	virtual bool SetTerrainDepthMapMinLevelMeter(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool GetTerrainDepthMapMinLevelMeter(AActor* p_actor, FLOAT64 &val) = 0;

	virtual bool SetTerrainDepthMapMaxLevelMeter(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool GetTerrainDepthMapMaxLevelMeter(AActor* p_actor, FLOAT64 &val) = 0;

	virtual bool SetTerrainTopLeftCornerCoordLLH(AActor* p_actor, FVector vec) = 0;
	virtual bool GetTerrainTopLeftCornerCoordLLH(AActor* p_actor, FVector &vec) = 0;

	virtual bool SetTerrainBottomRightCornerCoordLLH(AActor* p_actor, FVector vec) = 0;
	virtual bool GetTerrainBottomRightCornerCoordLLH(AActor* p_actor, FVector& vec) = 0;
	 


	/// map origin

	virtual bool SetMapOrigin(AActor* p_actor, FVector vec) = 0;
	virtual bool GetMapOrigin(AActor* p_actor, FVector &vec) = 0;

	

	/// <summary>
	/// Camera Management
	/// </summary>
	/// <param name="p_actor"></param>
	/// <param name="val"></param>
	/// <returns></returns>
	virtual bool SetCamSpeed(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool GetCamSpeed(AActor* p_actor, FLOAT64& val) = 0;

	

	virtual bool SetCamFollow(FString  actor_name) = 0;
	virtual bool GetCamFollow(FString& actor_name) = 0;


	virtual bool SetCamFollowTranslation(FVector val) = 0;
	virtual bool GetCamFollowTranslation(FVector& val) = 0;

	virtual bool SetCamFollowEuler(FVector  val) = 0;
	virtual bool GetCamFollowEuler(FVector& val) = 0;

	
		

	virtual bool SetCamView(ECamView view) = 0;
	virtual bool GetCamView(ECamView &view) = 0;

	//Thermal Managment
	virtual bool SetThermalMode(AActor* p_actor, INT32S mode) = 0;
	virtual bool GetThermalMode(AActor* p_actor, INT32S&mode) = 0;

	virtual bool SetTempratureKelvin(AActor* p_actor, FLOAT64 temp) = 0;
	virtual bool GetTempratureKelvin(AActor* p_actor, FLOAT64& mode) = 0;

	virtual bool SetIsHeatSource(AActor* p_actor, bool temp) = 0;
	virtual bool GetIsHeatSource(AActor* p_actor, bool& mode) = 0;

	virtual void SetAnnotationModeEnabled(bool val) = 0;
	virtual bool GetAnnotationModeEnabled() = 0;

	virtual void SetAnnotationSaveEnabled(bool val) = 0;
	virtual bool GetAnnotationSaveEnabled() = 0;

	virtual bool SetActorAnnotationId(AActor* p_actor, INT32S id) = 0;
	virtual bool GetActorAnnotationId(AActor* p_actor, INT32S &id) = 0;

	virtual bool SetAnnotateChildren(AActor* p_actor, bool val) = 0;
	virtual bool GetAnnotateChildren(AActor* p_actor, bool& val) = 0;



	virtual bool SetAisClassType(AActor* p_actor, int class_type) = 0;
	virtual bool GetAisClassType(AActor* p_actor, int &class_type) = 0;

	virtual bool SetAisPublishDurationSec(AActor* p_actor, double val) = 0;
	virtual bool GetAisPublishDurationSec(AActor* p_actor, double& val) = 0;

	virtual bool SetPublishAton(AActor* p_actor, bool val) = 0;
	virtual bool GetPublishAton(AActor* p_actor, bool& val) = 0;

	virtual bool GetSharedMemoryName(AActor* p_actor, FString& val) = 0;
	virtual	bool SetSharedMemoryName(AActor* p_actor, FString val) = 0;

	virtual bool GetGStreamerIP(AActor* p_actor, FString& val) = 0;
	virtual	bool SetGStreamerIP(AActor* p_actor, FString val) = 0;

	virtual bool GetGStreamerPort(AActor* p_actor, int& val) = 0;
	virtual	bool SetGStreamerPort(AActor* p_actor, int val) = 0;


	virtual	bool SetGimbalMode(AActor* p_actor, int val) = 0;
	virtual	bool GetGimbalMode(AActor* p_actor, int& rpy_deg) = 0;
	
	virtual	bool SetGimbalCommand(AActor* p_actor, FVector rpy_deg) = 0;
	virtual	bool GetGimbalCommand(AActor* p_actor, FVector &rpy_deg) = 0;

	virtual	bool SetGimbalAngleRPY(AActor* p_actor, FVector rpy_deg) = 0;
	virtual	bool GetGimbalAngleRPY(AActor* p_actor, FVector& rpy_deg) = 0;

	virtual	bool SetGimbalInitialAngleRPY(AActor* p_actor, FVector rpy_deg) = 0;
	virtual	bool GetGimbalInitialAngleRPY(AActor* p_actor, FVector& rpy_deg) = 0;

	virtual	bool SetGimbalAxisEnabled(AActor* p_actor, FVector rpy_en) = 0;
	virtual	bool GetGimbalAxisEnabled(AActor* p_actor, FVector& rpy_en) = 0;

	virtual	bool SetGimbalAxisRateDegPerSec(AActor* p_actor, FVector rpy_deg) = 0;
	virtual	bool GetGimbalAxisRateDegPerSec(AActor* p_actor, FVector& rpy_deg) = 0;

	virtual	bool SetGimbalAxisFixedRateDegPerSec(AActor* p_actor, FVector rpy_deg) = 0;
	virtual	bool GetGimbalAxisFixedRateDegPerSec(AActor* p_actor, FVector& rpy_deg) = 0;

	virtual	bool SetGimbalAxisMaxLimit(AActor* p_actor, FVector rpy_deg) = 0;
	virtual	bool GetGimbalAxisMaxLimit(AActor* p_actor, FVector& rpy_deg) = 0;

	virtual	bool SetGimbalAxisMinLimit(AActor* p_actor, FVector rpy_deg) = 0;
	virtual	bool GetGimbalAxisMinLimit(AActor* p_actor, FVector& rpy_deg) = 0;

	virtual	bool SetGimbalAxisIsFreeRotateEnabled(AActor* p_actor, FVector rpy_en) = 0;
	virtual	bool GetGimbalAxisIsFreeRotateEnabled(AActor* p_actor, FVector& rpy_en) = 0;

	


};
