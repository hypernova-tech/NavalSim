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
	virtual AActor* CreateActor(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale)=0;
	virtual void RegisterActor(AActor* p_actor) = 0;
	virtual bool RemoveActor(AActor* p_actor) = 0;
	virtual TArray<AActor*> GetRegisteredActors() = 0;
	virtual TArray<AActor*> GetAllActorInWorld() = 0;

	virtual void QueryActors(EActorQueryArgs args, TArray<AActor*> &actors) = 0;
	virtual AActor* FindActor(FString actor_name) = 0;
	virtual bool DestroyActor(FString name) = 0;
	virtual AActorBase* ToActorBase(AActor* p_actor) = 0;
	virtual ASensorBase* ToSensorBase(AActor* p_actor) = 0;
	virtual bool IsBakable(AActor* p_actor) = 0;
	
	virtual bool SetActorEnabled(AActor* p_actor, bool val) = 0;
	virtual bool GetActorEnabled(AActor* p_actor) = 0;
	virtual void EnableAllActors() = 0;
	virtual void DisableAllActors() = 0;
	virtual void SetSelectedActor(AActor *p_actor) = 0;
	virtual AActor* GetSelectedActor() = 0 ;
	virtual void SetGizmoTrackedActor(AActor* p_actor) = 0;

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
    virtual bool SetMeasurementErrorMean(AActor* p_actor, FLOAT64 angle_deg) = 0;
    virtual bool SetMeasurementErrorStd(AActor* p_actor, FLOAT64 angle_deg) = 0;
	virtual bool GetMeasurementErrorMean(AActor* p_actor, FLOAT64& angle_deg) = 0;
	virtual bool GetMeasurementErrorStd(AActor* p_actor, FLOAT64& angle_deg) = 0;
    virtual bool SetEnableSurfaceDetect(AActor* p_actor, BOOLEAN val) = 0;
    virtual bool SetEnableSubsurfaceDetect(AActor* p_actor, BOOLEAN val) = 0;
    virtual bool SetEnableFoamDetect(AActor* p_actor, BOOLEAN angle_deg) = 0;
    virtual bool SetSeaSurfaceDetectionProb(AActor* p_actor, FLOAT64 val) = 0;

	virtual bool GetEnableSurfaceDetect(AActor* p_actor, BOOLEAN& val) = 0;
	virtual bool GetEnableSubsurfaceDetect(AActor* p_actor, BOOLEAN& val) = 0;
	virtual bool GetEnableFoamDetect(AActor* p_actor, BOOLEAN& angle_deg) = 0;
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



	virtual bool AddOrModifyWaypointToPath(AActor* p_actor, INT32S wp_ind, FVector position) = 0;
	virtual bool Bake(AActor* p_actor) = 0;
	virtual bool SetPathClosed(AActor* p_actor, bool is_closed) = 0;
	virtual bool SetPathSpeed(AActor* p_actor, FLOAT64 val) = 0;
	virtual bool AttachToPath(AActor* p_path, FString actor_name) = 0;

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
};
