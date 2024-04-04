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

	UPROPERTY(EditAnywhere)
		int AISClassType = 0; // 0: none, 1: class A, 2: class B

	UPROPERTY(EditAnywhere)
		float AISMessagePublishPeriodSec = 10; // 0: none, 1: class A, 2: class B

	UPROPERTY(EditAnywhere)
		bool ShoudPublishATON = true; // 0: none, 1: class A, 2: class B


public:
	virtual void BeginPlay() override;
	virtual void InitSensor() override;
	virtual void Run(float delta_time_sec) override;
	void PublishClassAPositionReport(AActorBase* p_act);
	void PublishClassBPositionReport(AActorBase* p_act);
	void PublishATONReport(AActorBase* p_act);

	int GetAISClassType();
	void SetAISClassType(int val);

	float GetAISMessagePublishPeriodSec();
	void SetAISMessagePublishPeriodSec(float val);

	bool GetShoudPublishATON();
	void SetShoudPublishATON(bool val);

	void Save(ISaveLoader* p_save_loader);
	void SaveJSON(CJsonDataContainer& data);
};
