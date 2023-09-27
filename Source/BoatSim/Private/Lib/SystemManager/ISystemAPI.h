// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Lib/Types/Primitives.h>

/**
 * 
 */
class ISystemAPI
{
public:
	ISystemAPI();
	~ISystemAPI();
	virtual ISystemAPI* GetSystemAPI() = 0;

	virtual TArray<AActor*>& GetSensorGlobalIgnoreList() = 0;
	virtual AActor* GetFloor() = 0;
	virtual TArray<AActor*>& GetMoveableActorList() = 0;

	virtual AActor* CreateActor(FString model_name, FString boat_name, FVector world_pos, FVector world_rot, FVector scale)=0;
	virtual void RegisterActor(AActor* p_actor) = 0;
	virtual bool RemoveActor(AActor* p_actor) = 0;
	virtual TArray<AActor*> GetRegisteredActors() = 0;
	virtual TArray<AActor*> GetAllActorInWorld() = 0;
	virtual AActor* FindActor(FString actor_name) = 0;
	virtual bool DestroyActor(FString name) = 0;
	virtual AActorBase* ToActorBase(AActor* p_actor) = 0;
	virtual ASensorBase* ToSensorBase(AActor* p_actor) = 0;
	virtual bool SetActorEnabled(AActor* p_actor, bool val) = 0;
	virtual bool GetActorEnabled(AActor* p_actor) = 0;
	virtual void EnableAllActors() = 0;
	virtual void DisableAllActors() = 0;


	virtual INT64U GetTimeStamp() = 0;
	virtual AActor* GetVisibleActorAt(const TArray<AActor*>& ignore_list, FVector from, FVector to, FLOAT64 tolerance_meter) = 0;

	virtual void SetCanLoadConfig(bool val) = 0;
	virtual bool GetCanLoadConfig() = 0;

	virtual INT32S GetInstanceNo() = 0;
	virtual void ForceExit() = 0;

	virtual  bool  SetMainPlayerController(FString name) = 0;
	virtual AActor* GetMainPlayerController() = 0;

	
	virtual ACBoatBase* GetPlatform() = 0;
	
	virtual void SetPlatform(ACBoatBase* p_platform) = 0;


	virtual void StartSimulation() = 0;
	virtual void ResumeSimulation() = 0;
	virtual void PauseSimulation() = 0;


	virtual void SetConsoleConnection(void *p_connection) = 0;
	virtual void SendConsoleResponse(const FString& str) = 0;
	virtual bool SetActorInstanceNo(AActor* p_actor, INT32S instance_no) = 0;
	virtual int GetActorInstanceNo(AActor* p_actor) = 0;
	virtual ESensorType StringToSensor(const FString& str)=0;
	virtual FString SensorToString(ESensorType sensor_type) = 0;
	virtual TArray<ESensorType> GetAllSensorTypes() = 0;
	virtual TArray<ASensorBase*> GetAllSensors() = 0;
	virtual TArray<ASensorBase*> GetSensorsOfType(ESensorType sensor_type) = 0;

	virtual void RegisterConnection(UConnectionBase* p_connection) = 0;
	virtual INT32U ConvertToInstancedLocalPort(INT32 port_no) = 0;
};
