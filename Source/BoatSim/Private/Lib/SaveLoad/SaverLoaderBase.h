// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Lib/SystemManager/ISystemAPI.h>
#include <CBoatBase.h>
#include <Lib/Console/CCLICommandManager.h>
#include "SaverLoaderBase.generated.h"





UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USaverLoaderBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USaverLoaderBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void SavePlatform(ACBoatBase* p_boat, TArray<FString> &cli);
	virtual void SaveSensor(ASensorBase* p_sensor, TArray<FString>& cli);
	virtual void SaveActor(AActorBase *p_actor, TArray<FString>& cli);

	FString CreateCommand(FString cmd);
	void AppendOption(FString& line, FString option, FString value);
	void AppendOption(FString& line, FString option, BOOLEAN value);
	void AppendOption(FString& line, FString option, INT32U value);
	void AppendOption(FString& line, FString option, INT32S value);
	void AppendOption(FString& line, FString option, INT64U value);
	void AppendOption(FString& line, FString option, INT64S value);
	void AppendOption(FString& line, FString option, INT16U value);
	void AppendOption(FString& line, FString option, FLOAT32 value);
	void AppendOption(FString& line, FString option, FLOAT64 value);
	void AppendOption(FString& line, FString option, FVector2D value);
	void AppendOption(FString& line, FString option, FVector value);

	

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool Save(ISystemAPI *p_api, FString file_name);
	virtual bool Load(ISystemAPI* p_api, FString file_name);

	

private:
	ISystemAPI* pSystemApi;
	CCLICommandManager* pCLI;
		
};
