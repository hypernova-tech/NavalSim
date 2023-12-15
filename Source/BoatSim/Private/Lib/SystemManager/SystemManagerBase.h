// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Lib/Origin/MapOrigin.h>
#include <Lib/SOA/SOAImplementor.h>
#include <Lib/Config/ConfigManager.h>
#include <Lib/UI/UIControllerBase.h>
#include <Lib/ActorBase/ActorBase.h>
#include "CBoatBase.h"
#include <Lib/Console/ConsoleBase.h>
#include <Lib/SaveLoad/SaverLoaderBase.h>
#include <Lib/Sensor/GenericSensor/RadarBase.h>
#include <Lib/PathController/PathController.h>
#include <Lib/Platform/PlatformBase.h>
#include <Lib/DataContainer/DataManager.h>
#include <Lib/Environment/EnvManager.h>
#include <Lib/Terrain/TerrainManager.h>
#include "SystemManagerBase.generated.h"






enum ESystemState
{
	SystemStateJustLaunched,
	SystemStateDetectInstance,
	SystemStateWaitConfigLoad,
	SystemStateLoadingConfig,
	SystemStateConfigLoaded,
	SystemStateWaitingRun,
	SystemStateRunSimulation,
	SystemStateRunning,
	SystemStatePauseSimulation,
	SystemStatePaused,
	SystemStateResumeSimulation,
	SystemStateResumed,
	SystemStateConfigLoadError,

};




UCLASS()
class  ASystemManagerBase : public AActor, public ISystemAPI
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASystemManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DetectInstance();

	UPROPERTY(EditAnywhere)
		AMapOrigin* pMapOrigin;

	UPROPERTY(EditAnywhere)
		AActor* pFloor;

	UPROPERTY(EditAnywhere)
		ADataManager* pDataManager;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> MoveableActorList;

	UPROPERTY(EditAnywhere)
		ASOAImplementor* pSoaImplementor;

	UPROPERTY(EditAnywhere)
		AActor* pConfigManagerActor;

	UPROPERTY(EditAnywhere)
		AActor* pConsoleActor;

	UPROPERTY(EditAnywhere)
		AEnvManager* pEnvManager;


	UConsoleBase* pConsole = nullptr;
		

	UPROPERTY(EditAnywhere)
		AUIControllerBase* pUIController;



	UPROPERTY(EditAnywhere)
		TArray<AActor*> SensorGlobalIgnoreList;

	UConfigManager* pConfigManager;
	virtual bool LoadConfig();

	virtual void UpdateActors(float deltatime);
	virtual void UpdateActorsScenarioMode(float deltatime);
	virtual void StateMachine(float deltatime);

	ESystemState SystemState = ESystemState::SystemStateJustLaunched;
	bool CanLoadConfig = false;

	INT32S InstanceNo = -1;
	INT32S TotalInstanceCount = 1;

	virtual void HandleSimulationStart();
	virtual void HandleSimulationPause();
	virtual void HandleSimulationResume();
	void ComputeFPS(float DeltaTime);
	void StartStartupConnections();
	void StartRuntimeConnections();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	AMapOrigin* GetMapOrigin();

	UFUNCTION(BlueprintCallable)
	ASOAImplementor* GetSOAImplementor();

	UFUNCTION(BlueprintCallable)
	AUIControllerBase* GetUIController();

	UFUNCTION(BlueprintCallable)
	ADataManager* GetDataManager();


	ARadarBase* ToRadarBase(AActor* p_actor);


	static  ASystemManagerBase* GetInstance();

	void OnActorMoved(AActor* p_actor);

	
private:

	static ASystemManagerBase* pInstance;
	TMap<FString,  AActor*> AllActors;
	TArray<AActor*> ActorList;
	TArray<ASensorBase*> Sensors;
	TArray<APlatformBase*> Platforms;
	TMap<ESensorType, TArray<ASensorBase*> > SensorsOfType;
	bool IsStartReceived = false;
	bool IsResumeReceived = false;
	bool IsPauseReceived = false;
	APlatformBase* pPlatform;
	UUdpConnection* pConsoleConnection;

	TArray< UConnectionBase*> Connections;
	AActor* pSelectedActor;
	USaverLoaderBase* pSaverLoader;

	/// <summary>
	///  system API
	/// </summary>
public:
	virtual ISystemAPI* GetSystemAPI();

	UFUNCTION(BlueprintCallable)
	virtual	UConsoleBase* GetConsole();

	virtual TArray<AActor*>& GetSensorGlobalIgnoreList();
	virtual AActor* GetFloor();
	virtual TArray<AActor*>& GetMoveableActorList();

	virtual AActor* CreateActor(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale);
	virtual void RegisterActor(AActor* p_actor);
	virtual bool RemoveActor(AActor* p_actor);
	virtual TArray<AActor*> GetRegisteredActors();
	virtual TArray<AActor*> GetAllActorInWorld();
	virtual void QueryActors(EActorQueryArgs args, TArray<AActor*>& actors);
	virtual AActor* FindActor(FString actor_name);
	virtual bool DestroyActor(FString name);

	virtual AActorBase* ToActorBase(AActor* p_actor);
	virtual ASensorBase* ToSensorBase(AActor* p_actor);
	APathController* ToPath(AActor* p_actor);
	APlatformBase* ToPlatform(AActor* p_actor);
	AWaypointActor* ToWaypoint(AActor* p_actor);
	ATerrainManager* ToTerrain(AActor* p_actor);
	template <typename T>
	T* To(AActor* p_actor);

	virtual bool IsBakable(AActor* p_actor);
	virtual void EnableAllActors();
	virtual void DisableAllActors();
	virtual bool SetActorEnabled(AActor* p_actor, bool val);
	virtual bool GetActorEnabled(AActor* p_actor);
	virtual void SetSelectedActor(AActor* p_actor) ;
	virtual AActor* GetSelectedActor();
	virtual void SetGizmoTrackedActor(AActor* p_actor);
	virtual INT64U GetTimeStamp();
	virtual AActor* GetVisibleActorAt(const TArray<AActor*>& ignore_list, FVector from, FVector to, FLOAT64 tolerance_meter);



	virtual void SetCanLoadConfig(bool val);
	virtual bool GetCanLoadConfig();

	virtual INT32S GetInstanceNo();
	virtual void ForceExit();

	virtual  bool  SetMainPlayerController(FString name);
	virtual AActor* GetMainPlayerController();

	UFUNCTION(BlueprintCallable)
	virtual APlatformBase* GetPlatform();

	UFUNCTION(BlueprintCallable)
	virtual void SetPlatform(APlatformBase* p_platform);


	virtual void StartSimulation();
	virtual void ResumeSimulation();
	virtual void PauseSimulation();


	virtual bool SetActorInstanceNo(AActor *p_actor, INT32S instance_no);
	virtual int GetActorInstanceNo(AActor* p_actor);

	virtual void SetConsoleConnection(void* p_connection);
	virtual void SendConsoleResponse(const FString& str);
	virtual ESensorType StringToSensor(const FString& str);
	virtual FString SensorToString(ESensorType sensor_type);
	virtual TArray<ESensorType> GetAllSensorTypes();
	virtual TArray<ASensorBase*> GetAllSensors() ;
	virtual TArray<ASensorBase*> GetSensorsOfType(ESensorType sensor_type);

	virtual void RegisterConnection(UConnectionBase* p_connection);

	virtual INT32U ConvertToInstancedLocalPort(INT32 port_no);

	virtual bool GetSensorScanStepAngleDeg(AActor* p_actor, FVector2D& ret);
	virtual bool SetSensorScanStepAngleDeg(AActor* p_actor, FVector2D ang);

	virtual bool Save(FString fname);
	virtual bool Load(FString fname);


	virtual bool SetSlotIndex(AActor* p_actor, INT32S slot_index);
	virtual bool GetSlotIndex(AActor* p_actor, INT32S& slot_index);

	virtual bool GetConnectionInfo(AActor* p_actor, INT32S ind, SConnectionInfo& conn);
	virtual bool SetConnectionInfo(AActor* p_actor, INT32S ind, SConnectionInfo conn) ;

	virtual bool SetHorizontalFov(AActor* p_actor, FLOAT64 angle_deg);
	virtual bool GetHorizontalFov(AActor* p_actor, FLOAT64& angle_deg);
	virtual bool SetRangeMinMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetRangeMinMeter(AActor* p_actor, FLOAT64& val);
	virtual bool SetRangeMaxMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetRangeMaxMeter(AActor* p_actor, FLOAT64& val);
	virtual bool SetVerticalFov(AActor* p_actor, FLOAT64 angle_deg);
	virtual bool GetVerticalFov(AActor* p_actor, FLOAT64& angle_deg);
	virtual bool SetHorizontalScanStepAngleDeg(AActor* p_actor, FLOAT64 angle_deg);
	virtual bool GetHorizontalScanStepAngleDeg(AActor* p_actor, FLOAT64& angle_deg);
	virtual bool SetVerticalScanStepAngleDeg(AActor* p_actor, FLOAT64 angle_deg);
	virtual bool GetVerticalScanStepAngleDeg(AActor* p_actor, FLOAT64& angle_deg);
	virtual bool SetMeasurementErrorMean(AActor* p_actor, FLOAT64 angle_deg);
	virtual bool SetMeasurementErrorStd(AActor* p_actor, FLOAT64 angle_deg);
	virtual bool GetMeasurementErrorMean(AActor* p_actor, FLOAT64& angle_deg);
	virtual bool GetMeasurementErrorStd(AActor* p_actor, FLOAT64& angle_deg);
	virtual bool SetEnableSurfaceDetect(AActor* p_actor, BOOLEAN val);
	virtual bool SetEnableSubsurfaceDetect(AActor* p_actor, BOOLEAN val) ;
	virtual bool SetEnableFoamDetect(AActor* p_actor, BOOLEAN angle_deg);
	virtual bool SetSeaSurfaceDetectionProb(AActor* p_actor, FLOAT64 val) ;

	virtual bool GetEnableSurfaceDetect(AActor* p_actor, BOOLEAN& val) ;
	virtual bool GetEnableSubsurfaceDetect(AActor* p_actor, BOOLEAN& val) ;
	virtual bool GetEnableFoamDetect(AActor* p_actor, BOOLEAN& angle_deg) ;
	virtual bool GetSeaSurfaceDetectionProb(AActor* p_actor, FLOAT64& val);

	virtual bool SetMaxSurfacePenetration(AActor* p_actor, FLOAT64 val);
	virtual bool SetRadarScanLevel(AActor* p_actor, INT32S val);
	virtual bool SetRadarScannerRPM(AActor* p_actor, INT32S val);
	virtual bool SetRadarGainType(AActor* p_actor, INT32S val) ;
	virtual bool SetRadarGainLevel(AActor* p_actor, INT32S val);
	virtual bool SetRadarSeaClutterType(AActor* p_actor, INT32S val);
	virtual bool SetRadarSeaClutterLevel(AActor* p_actor, INT32S val);
	virtual bool SetRadarSeaClutterAutoOffset(AActor* p_actor, INT32S val);
	virtual bool SetRadarRainClutterLevel(AActor* p_actor, INT32S val);
	virtual bool SetRadarMaxGuardZoneCount(AActor* p_actor, INT32S val) ;
	virtual bool SetRadarMaxSectorBlankingZoneCount(AActor* p_actor, INT32S val);


	virtual bool GetMaxSurfacePenetration(AActor* p_actor, FLOAT64& val);
	virtual bool GetRadarScanLevel(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarScannerRPM(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarGainType(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarGainLevel(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarSeaClutterType(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarSeaClutterLevel(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarSeaClutterAutoOffset(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarRainClutterLevel(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarMaxGuardZoneCount(AActor* p_actor, INT32S& val) ;
	virtual bool GetRadarMaxSectorBlankingZoneCount(AActor* p_actor, INT32S& val);

	virtual bool AddOrModifyWaypointToPath(AActor* p_actor, INT32S wp_ind, FVector position);
	virtual bool Bake(AActor* p_actor);
	virtual bool SetPathClosed(AActor* p_actor, bool is_closed);
	virtual bool SetPathSpeed(AActor* p_actor, FLOAT64 val);
	virtual bool AttachToPath(AActor* p_path, FString actor_name);
	virtual bool DetachFromPath(AActor* p_path, FString actor_name);

	virtual bool GetWaypointPosition(AActor* p_actor, INT32S wp_ind, FVector& position);
	virtual bool GetPathClosed(AActor* p_actor, bool& is_closed);
	virtual bool GetPathSpeed(AActor* p_actor, FLOAT64& val);
	virtual bool GetActorAttachedToPath(AActor* p_path, FString& actor_name);


	virtual bool GetPathSegmentCount(AActor* p_actor, INT32S& val);
	virtual bool SetPathSegmentCount(AActor* p_actor, INT32S& val);

	virtual bool GetPathLineColor(AActor* p_actor, FColor& val);
	virtual bool SetPathLineColor(AActor* p_actor, FColor& val);

	virtual bool GetPathStraight(AActor* p_actor, bool& val);
	virtual bool SetPathStraight(AActor* p_actor, bool& val);

	virtual bool GetPathTurnRate(AActor* p_actor, FLOAT64& val);
	virtual bool SetPathTurnRate(AActor* p_actor, FLOAT64 val);


	virtual bool SetActorPosition(AActor* p_actor, FVector pos);
	virtual bool SetActorRelPosition(AActor* p_actor, FVector pos);

	virtual bool SetActorRot(AActor* p_actor, FVector pos);
	virtual bool SetActorRelRot(AActor* p_actor, FVector pos);

	virtual bool FocusCamera(AActor* p_actor);

	virtual bool GetIsUIVisible(bool& val);
	virtual bool SetIsUIVisible(bool val);

	virtual bool SetEnvSoundEnabled(INT32S region, bool val) ;
	virtual bool GetEnvSoundEnabled(INT32S region, bool& val);

	virtual bool SetCloudPercent(INT32S region, FLOAT64 val) ;
	virtual bool GetCloudPercent(INT32S region, FLOAT64& val) ;

	virtual bool SetRainPercent(INT32S region, FLOAT64 val) ;
	virtual bool GetRainPercent(INT32S region, FLOAT64& val);

	virtual bool SetSnowPercent(INT32S region, FLOAT64 val) ;
	virtual bool GetSnowPercent(INT32S region, FLOAT64& val);

	virtual bool SetDustPercent(INT32S region, FLOAT64 val) ;
	virtual bool GetDustPercent(INT32S region, FLOAT64& val);

	virtual bool SetFogPercent(INT32S region, FLOAT64 val);
	virtual bool GetFogPercent(INT32S region, FLOAT64& val);

	virtual bool SetThunderPercent(INT32S region, FLOAT64 val);
	virtual bool GetThunderPercent(INT32S region, FLOAT64& val);

	virtual bool SetWindSpeedMeterPerSec(INT32S region, FLOAT64 val);
	virtual bool GetWindSpeedMeterPerSec(INT32S region, FLOAT64& val);

	virtual bool SetWindDirectionDeg(INT32S region, FLOAT64 val);
	virtual bool GetWindDirectionDeg(INT32S region, FLOAT64& val);

	virtual bool SetTimeOfDayHr(INT32S region, FLOAT64 val) ;
	virtual bool GetTimeOfDayHr(INT32S region, FLOAT64& val);

	virtual bool SetEnvTimeFlowScale(INT32S region, FLOAT64 val) ;
	virtual bool GetEnvTimeFlowScale(INT32S region, FLOAT64& val) ;


	virtual bool SetTerrainHeightMap(AActor* p_actor, FString  path);
	virtual bool GetTerrainHeightMap(AActor* p_actor, FString& path);

	virtual bool SetTerrainDepthMap(AActor* p_actor, FString path);
	virtual bool GetTerrainDepthMap(AActor* p_actor, FString& path);

	virtual bool SetTerraiBaseTexture(AActor* p_actor, FString path);
	virtual bool GetTerraiBaseTexture(AActor* p_actor, FString& path);

	virtual bool SetTerrainHeightMapMinLevelMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetTerrainHeightMapMinLevelMeter(AActor* p_actor, FLOAT64& val);

	virtual bool SetTerrainHeightMapMaxLevelMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetTerrainHeightMapMaxLevelMeter(AActor* p_actor, FLOAT64& val);

	virtual bool SetTerrainDepthMapMinLevelMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetTerrainDepthMapMinLevelMeter(AActor* p_actor, FLOAT64& val);

	virtual bool SetTerrainDepthMapMaxLevelMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetTerrainDepthMapMaxLevelMeter(AActor* p_actor, FLOAT64& val);

	virtual bool SetTerrainWidthMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetTerrainWidthMeter(AActor* p_actor, FLOAT64& val);

	virtual bool SetTerrainLengthMeter(AActor* p_actor, FLOAT64 val);
	virtual bool GetTerrainLengthMeter(AActor* p_actor, FLOAT64& val);

	virtual bool SetTerrainLowerLeftCornerCoordXYZ(AActor* p_actor, FVector vec);
	virtual bool GetTerrainLowerLeftCornerCoordXYZ(AActor* p_actor, FVector& vec);


	virtual bool SetTerrainLowerLeftCornerCoordLLH(AActor* p_actor, FVector vec);
	virtual bool GetTerrainLowerLeftCornerCoordLLH(AActor* p_actor, FVector& vec);




};
