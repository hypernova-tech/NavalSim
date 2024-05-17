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
		int AISSrcAddr = 1; //


	UPROPERTY(EditAnywhere)
		int AISDefaultMessagePriority = 2; //
	

	UPROPERTY(EditAnywhere)
		float AISMessagePublishPeriodSecStationary = 300; 

	UPROPERTY(EditAnywhere)
		float AISMessagePublishPeriodSecMoving = 5;

	UPROPERTY(EditAnywhere)
		int AISUserId = 0; // 

	UPROPERTY(EditAnywhere)
		int ShipCargoType = 0; // 

	UPROPERTY(EditAnywhere)
		bool ShoudPublishATON = true; 

	UPROPERTY(EditAnywhere)
		int AISAtonType = 1;

	UPROPERTY(EditAnywhere)
		FString AisAtonName="atonufo";
	

	UPROPERTY(EditAnywhere)
		int AISMMSI = 0;

	UPROPERTY(EditAnywhere)
		FString AISVendorId = "";

	UPROPERTY(EditAnywhere)
		FString AISCallSign = ""; 

	UPROPERTY(EditAnywhere)
		FString AISDestination = "unknown";

	UPROPERTY(EditAnywhere)
		double AISReferencePointFromStarboard = 0;

	UPROPERTY(EditAnywhere)
		double ReferencePointPositionAftOfBow = 0;

	UPROPERTY(EditAnywhere)
		FString AISName;

	UPROPERTY(EditAnywhere)
		FVector AISDataOfArrivalYMD; // Year:X, Month: Y, Data: Z

	UPROPERTY(EditAnywhere)
		double AISTimeOfArrival; 

	void UpdateAISMotionMode();
	void InvalidateEntries();
	double GetCurrentPusblishPeriod();
	bool IsStationary = true;
	bool IsMotionModeChanged = false;
	AActorBase* pAISPublisher;
	virtual void SendMessageViaAISPusblisher(INT32U src_addr, INT32U prio, INT32U png, void* p_data, INT32S data_size);

public:
	virtual void BeginPlay() override;
	virtual void InitSensor() override;
	virtual void Run(float delta_time_sec) override;
	void PublishClassAPositionReport(AActorBase* p_act);
	void PublishClassBPositionReport(AActorBase* p_act);
	void PublishATONReport(AActorBase* p_act);
	void PublishClassBStaticDataReportPartA(AActorBase* p_act);
	void PublishClassBStaticDataReportPartB(AActorBase* p_act);
	void PublishAISClassAStaticVoyageRelatedData(AActorBase* p_act);
	int GetAISClassType();
	void SetAISClassType(int val);

	float GetAISMessagePublishPeriodSecStationary();
	void SetAISMessagePublishPeriodSecStationary(float val);

	float GetAISMessagePublishPeriodSecMoving();
	void SetAISMessagePublishPeriodSecMoving(float val);

	bool GetShoudPublishATON();
	void SetShoudPublishATON(bool val);

	void SetAISMMSI(int val);
	int  GetAISMMSI();

	void    SetAISName(FString val);
	FString GetAISName();

	void    SetDateOfArrivalYearMonthDay(FVector val);
	FVector GetDateOfArrivalYearMonthDay();

	void    SetTimeOfArrivalSec(double val);
	double GetTimeOfArrivalSec();

	void    SetAISVendorId(FString val);
	FString GetAISVendorId();

	void SetAISCallSign(FString val);
	FString GetAISCallSign();

	void SetAISReferencePointFromStarboard(double val);
	double GetAISReferencePointFromStarboard();

	void SetReferencePointPositionAftOfBow(double val);
	double GetReferencePointPositionAftOfBow();

	void SetAISUserId(int val);
	int GetAISUserId();

	void SetShipCargoType(int val);
	int GetShipCargoType();


	void Save(ISaveLoader* p_save_loader);
	void SaveJSON(CJsonDataContainer& data);
};
