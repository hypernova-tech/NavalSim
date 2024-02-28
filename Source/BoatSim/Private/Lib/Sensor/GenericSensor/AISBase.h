// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lib/Sensor/SensorBase.h"
#include "AISDataTypes.h"
#include "AISBase.generated.h"


USTRUCT(BlueprintType)
struct FAISEntry
{
	GENERATED_BODY()

public:

	AActorBase* pActor;
	double LastTransmitTimeSec = -1;


};

/**
 * 
 */
UCLASS()
class AAISBase : public ASensorBase
{
	GENERATED_BODY()
	


protected:
	TArray<FAISEntry> AisEntries;
	virtual void ProcessEntries();
	double GetCourseOverGround(AActor* Actor);
public:

	virtual void InitSensor() override;
	virtual void Run(float delta_time_sec) override;
	void PublishClassAPositionReport(AActorBase* p_act);
	void PublishClassBPositionReport(AActorBase* p_act);
	void PublishATONReport(AActorBase* p_act);
};
