// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Lib/SystemManager/ISystemAPI.h>
#include <CBoatBase.h>
#include <Lib/Console/CCLICommandManager.h>
#include <Lib/SaveLoad/ISaveLoader.h>
#include "SaverLoaderBase.generated.h"



enum ELoaderStateMachine
{
	LoaderStateMachineIdle,
	LoaderStateMachineProcess,
	LoaderStateMachineDelay,
	LoaderStateMachineLoadComplete,
	LoaderStateMachineWait
};

UCLASS(meta=(BlueprintSpawnableComponent) )
class USaverLoaderBase : public UActorComponent, public ISaveLoader
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USaverLoaderBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SavePlatform(ACBoatBase* p_boat, TArray<FString> &cli);
	virtual void SaveSensor(ASensorBase* p_sensor, TArray<FString>& cli);
	virtual void SaveActor(AActorBase *p_actor, TArray<FString>& cli);
	
	
	TArray<FString> CLIList;
	ELoaderStateMachine State = ELoaderStateMachine::LoaderStateMachineIdle;
	virtual void StateMachine();
	void ProcessCurrentCommand(INT32S &tick_delay);
	void ProcessParentingCommands();
	TArray<FString> CommandLines;
	TArray<FString> ParentingCommands;
	INT32S CurrentCommandIndex;
	INT64S DelayStartRef;
	INT32S TickDelayCount;
	
public:	
	// Called every frame

	virtual bool Save(ISystemAPI *p_api, FString file_name);
	virtual bool Load(ISystemAPI* p_api, FString file_name);

	ISystemAPI* pSystemApi;
	CCLICommandManager* pCLI;
	virtual FString CreateCommand(FString cmd);
	virtual FString CreateCommandWithName(FString cmd, FString name) ;
	virtual void NewLine();
	virtual void AddLine(FString line);
	virtual void AppendOption(FString& line, FString option, FString value) ;
	virtual void AppendOption(FString& line, FString option, BOOLEAN value) ;
	virtual void AppendOption(FString& line, FString option, INT32U value);
	virtual void AppendOption(FString& line, FString option, INT32S value);
	virtual void AppendOption(FString& line, FString option, INT64U value);
	virtual void AppendOption(FString& line, FString option, INT64S value);
	virtual void AppendOption(FString& line, FString option, INT16U value);
	virtual void AppendOption(FString& line, FString option, FLOAT32 value);
	virtual void AppendOption(FString& line, FString option, FLOAT64 value) ;
	virtual void AppendOption(FString& line, FString option, FVector2D value) ;
	virtual void AppendOption(FString& line, FString option, FVector value) ;
	virtual void AppendOption(FString& line, FString option, FColor color);
};
